/****************************************************************************
 * arch/arm/src/da1470x/snc/snc_rt.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include "hardware/da1470x_crg_top.h"
#include "hardware/da1470x_crg_xtal.h"
#include "hardware/da1470x_pdc.h"
#include "hardware/da1470x_timer.h"

#include "snc_rt.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Cortex-M0+ NVIC */

#define NVIC_ISER         0xe000e100
#define NVIC_ICER         0xe000e180
#define NVIC_ICPR         0xe000e280
#define NVIC_IPR(n)       (0xe000e400 + 4 * (n))
#define NVIC_NIPR         8           /* 32 interrupts, 4 per register */
#define SCB_SCR           0xe000ed10
#define SCB_SCR_SLEEPDEEP (1 << 2)

#define SNC_TIMER_BASE    DA1470X_TIMER6_BASE
#define SNC_TIMER_CTRL    (SNC_TIMER_BASE + DA1470X_TIMER_CTRL_OFFSET)
#define SNC_TIMER_STATUS  (SNC_TIMER_BASE + DA1470X_TIMER_STATUS_OFFSET)
#define SNC_TIMER_SET     (SNC_TIMER_BASE + DA1470X_TIMER_SETTINGS_OFFSET)
#define SNC_TIMER_CLR     (SNC_TIMER_BASE + DA1470X_TIMER_CLEAR_IRQ_OFFSET)

#define SNC_TIMER_BUSY_LOOPS 10000

/****************************************************************************
 * Public Data
 ****************************************************************************/

volatile bool g_snc_cmd_pending;
volatile uint32_t g_snc_timer_ticks;

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* What deep sleep has to put back if PD_SNC went down meanwhile: the
 * callee-saved registers of snc_deepsleep() and the NVIC.  RAM1 and RAM2
 * are not in PD_SNC and keep their contents.
 */

static uint32_t g_ctx[10];
static uint32_t g_nvic_iser;
static uint32_t g_nvic_ipr[NVIC_NIPR];

/****************************************************************************
 * Assembly Functions
 ****************************************************************************/

int  snc_ctx_save(uint32_t *ctx);
void snc_ctx_restore(uint32_t *ctx);

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void snc_doorbell(void)
{
  putreg32(CRG_XTAL_SET_SYS_IRQ_CTRL_SNC2SYS_IRQ_BIT,
           DA1470X_CRG_XTAL_SET_SYS_IRQ_CTRL);
}

/* A write to the timer's settings takes a few low power clock cycles to
 * reach its counter; a second write in that window is lost.
 */

static void snc_timer_wait_busy(void)
{
  int i;

  for (i = 0; i < SNC_TIMER_BUSY_LOOPS; i++)
    {
      if ((getreg32(SNC_TIMER_STATUS) & TIMER_STATUS_TIM_TIMER_BUSY) == 0)
        {
          return;
        }
    }
}

/* Acknowledge the power domain controller entries that woke the
 * controller, or they keep it awake and PD_SNC up.
 */

static void snc_ack_pdc(void)
{
  uint32_t pending = getreg32(DA1470X_PDC_PENDING_SNC);
  int i;

  for (i = 0; pending != 0; i++, pending >>= 1)
    {
      if (pending & 1)
        {
          putreg32(i, DA1470X_PDC_ACKNOWLEDGE);
        }
    }
}

/****************************************************************************
 * Name: snc_deepsleep
 *
 * Description:
 *   Sleep with SLEEPDEEP set, allowing PD_SNC to power down.  If it does
 *   not -- the M33 keeps the domain up while it needs its console and
 *   serial interfaces -- this is an ordinary sleep and WFI returns.  If
 *   it does, the next wake-up from the power domain controller starts
 *   the controller from reset; snc_reset() sees SNC_STATE_RETAINED and
 *   snc_resume() returns here through snc_ctx_restore().  Interrupts are
 *   masked on entry.
 *
 ****************************************************************************/

static void snc_deepsleep(void)
{
  int i;

  g_nvic_iser = getreg32(NVIC_ISER);
  for (i = 0; i < NVIC_NIPR; i++)
    {
      g_nvic_ipr[i] = getreg32(NVIC_IPR(i));
    }

  g_snc_shared->sleeps++;

  if (snc_ctx_save(g_ctx) == 0)
    {
      snc_ack_pdc();
      putreg32(getreg32(DA1470X_CRG_TOP_CLK_SNC_CTRL) |
               CRG_TOP_CLK_SNC_CTRL_SNC_STATE_RETAINED,
               DA1470X_CRG_TOP_CLK_SNC_CTRL);
      putreg32(getreg32(SCB_SCR) | SCB_SCR_SLEEPDEEP, SCB_SCR);
      __asm__ __volatile__ ("dsb\n\twfi" : : : "memory");

      /* Still powered: nothing was lost */

      putreg32(getreg32(SCB_SCR) & ~SCB_SCR_SLEEPDEEP, SCB_SCR);
      putreg32(getreg32(DA1470X_CRG_TOP_CLK_SNC_CTRL) &
               ~CRG_TOP_CLK_SNC_CTRL_SNC_STATE_RETAINED,
               DA1470X_CRG_TOP_CLK_SNC_CTRL);
    }
}

/****************************************************************************
 * Name: snc_resume
 *
 * Description:
 *   Called by snc_reset() on its own stack, interrupts masked, when the
 *   controller comes back from a power loss in deep sleep.  Puts the
 *   interrupt controller back and continues in snc_deepsleep().
 *
 ****************************************************************************/

void snc_resume(void)
{
  int i;

  putreg32(getreg32(DA1470X_CRG_TOP_CLK_SNC_CTRL) &
           ~CRG_TOP_CLK_SNC_CTRL_SNC_STATE_RETAINED,
           DA1470X_CRG_TOP_CLK_SNC_CTRL);

  for (i = 0; i < NVIC_NIPR; i++)
    {
      putreg32(g_nvic_ipr[i], NVIC_IPR(i));
    }

  putreg32(g_nvic_iser, NVIC_ISER);

  g_snc_shared->resumes++;
  snc_ctx_restore(g_ctx);
}

/****************************************************************************
 * Interrupt Handlers
 ****************************************************************************/

void snc_irq_sys2snc(void)
{
  putreg32(CRG_XTAL_RESET_SYS_IRQ_CTRL_SYS2SNC_IRQ_BIT,
           DA1470X_CRG_XTAL_RESET_SYS_IRQ_CTRL);
  g_snc_cmd_pending = true;
}

void snc_irq_timer6(void)
{
  putreg32(0, SNC_TIMER_CLR);
  g_snc_timer_ticks++;
}

/* Called from the fault vectors with the exception frame */

void snc_fault(uint32_t *frame)
{
  g_snc_shared->fault_lr = frame[5];
  g_snc_shared->fault_pc = frame[6];
  snc_ipc_barrier();
  g_snc_shared->state = SNC_STATE_FAULT;
  snc_doorbell();
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void snc_irq_enable(int irq)
{
  putreg32(1u << irq, NVIC_ICPR);
  putreg32(1u << irq, NVIC_ISER);
}

void snc_irq_disable(int irq)
{
  putreg32(1u << irq, NVIC_ICER);
}

void snc_rt_init(void)
{
  g_snc_shared->state = SNC_STATE_BOOTING;
  snc_irq_enable(SNC_IRQ_SYS2SNC);
  __asm__ __volatile__ ("cpsie i" : : : "memory");

  /* A command may have been queued before the interrupt was enabled */

  g_snc_cmd_pending = true;
}

void snc_rt_ready(void)
{
  snc_ipc_barrier();
  g_snc_shared->state = SNC_STATE_READY;
  snc_doorbell();
}

int snc_send(uint16_t type, const void *payload, uint16_t len)
{
  if (snc_ring_put(&g_snc_shared->to_m33, type, payload, len) < 0)
    {
      g_snc_shared->dropped++;
      return -1;
    }

  snc_doorbell();
  return 0;
}

bool snc_rt_system_message(uint16_t type, const void *payload, int len)
{
  switch (type)
    {
      case SNC_MSG_PING:
        snc_send(SNC_MSG_PONG, payload, len);
        return true;

      case SNC_MSG_SLEEP_POLICY:
        if (len >= 4)
          {
            const uint8_t *p = payload;

            g_snc_shared->deep_sleep = p[0] != 0;
          }

        return true;

      default:
        return false;
    }
}

int snc_recv(uint16_t *type, void *payload, uint16_t size)
{
  return snc_ring_get(&g_snc_shared->to_snc, type, payload, size);
}

void snc_rt_wait(void)
{
  static uint32_t seen;

  /* Look with interrupts masked so that one arriving between the check
   * and the WFI still wakes it; WFI returns on a pending interrupt even
   * while they are masked.  The M33 rings the doorbell both when it has
   * queued a command and when it has made room in the event ring, so the
   * flag is all there is to check.
   */

  __asm__ __volatile__ ("cpsid i" : : : "memory");
  if (!g_snc_cmd_pending && g_snc_timer_ticks == seen)
    {
      if (g_snc_shared->deep_sleep)
        {
          snc_deepsleep();
        }
      else
        {
          __asm__ __volatile__ ("wfi" : : : "memory");
        }
    }

  __asm__ __volatile__ ("cpsie i" : : : "memory");

  seen = g_snc_timer_ticks;
  g_snc_shared->heartbeat++;
}

void snc_timer_start(uint32_t period_ms)
{
  uint32_t hz = g_snc_shared->lpclk_hz;
  uint32_t reload;

  putreg32(0, SNC_TIMER_CTRL);
  snc_irq_disable(SNC_IRQ_TIMER6);

  if (period_ms == 0 || hz == 0)
    {
      return;
    }

  reload = (uint32_t)(((uint64_t)hz * period_ms + 500) / 1000);
  if (reload < 2)
    {
      reload = 2;
    }

  if (reload > 0xffffff)
    {
      reload = 0xffffff;
    }

  /* Count up from the low power clock to the reload value and start over,
   * raising the interrupt each time.
   */

  snc_timer_wait_busy();
  putreg32(TIMER_SETTINGS_TIM_RELOAD(reload - 1), SNC_TIMER_SET);
  snc_timer_wait_busy();
  putreg32(0, SNC_TIMER_CLR);
  putreg32(TIMER_CTRL_TIM_CLK_EN | TIMER_CTRL_TIM_IRQ_EN |
           TIMER_CTRL_TIM_EN, SNC_TIMER_CTRL);

  snc_irq_enable(SNC_IRQ_TIMER6);
}
