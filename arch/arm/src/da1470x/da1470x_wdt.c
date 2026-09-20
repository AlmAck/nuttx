/****************************************************************************
 * arch/arm/src/da1470x/da1470x_wdt.c
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

/* There are three watchdogs on this part and they are the same circuit
 * three times over, each decrementing once every 10 ms from a 13-bit
 * reload -- so a little under 82 seconds is the longest any of them can be
 * asked to wait.  What differs is where each one lives and who is supposed
 * to feed it:
 *
 *   SYS   in PD_SLP, and ours.  This is the one that becomes a NuttX
 *         watchdog device.
 *   CMAC  in PD_RAD, reloaded by the radio controller's own firmware.  It
 *         starts as soon as the radio domain is powered, and the datasheet
 *         says plainly that intervention from this processor is not
 *         needed, so all that is offered here is the freeze.
 *   SNC   in PD_SNC, driving the sensor node controller's NMI rather than
 *         ours.  Its firmware should be feeding it; the reload is exposed
 *         because the hardware allows it and a controller running
 *         something simple may not want a watchdog at all.
 *
 * Two things about the system watchdog are easy to be caught by.
 *
 * It does not stay stopped.  Freezing it works while the processor is
 * awake, but the datasheet states that entering any sleep mode with PD_SYS
 * switched off resumes it regardless.  A system that sleeps longer than
 * the timeout is reset while it sleeps, however deliberately the watchdog
 * was stopped first.
 *
 * And it does not simply reset.  With NMI_RST clear it raises an NMI when
 * the count reaches zero and only resets at -16, which is 160 ms later.
 * That window is what NuttX's capture method is wired to here: a handler
 * gets those 160 ms, and unless it reloads the counter itself the reset
 * still arrives.  Nothing on this part can promise a timeout that never
 * resets.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <errno.h>
#include <debug.h>

#include <nuttx/arch.h>
#include <nuttx/irq.h>
#include <nuttx/timers/watchdog.h>

#include "arm_internal.h"
#include "hardware/da1470x_sys_wdog.h"
#include "hardware/da1470x_gpreg.h"
#include "hardware/da1470x_crg_top.h"
#include "hardware/da1470x_memorymap.h"
#include "da1470x_wdt.h"

#ifdef CONFIG_DA1470X_WDT

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* The sensor node controller's watchdog register, which unlike the other
 * two takes its reload through a guarded field.
 */

#define DA1470X_SNC_WDOG            (DA1470X_SNC_BASE + 0x0004)
#define SNC_WDOG_CNT_MASK           0x00001fff
#define SNC_WDOG_WRITE_VALID        (3 << 17)

#define WDT_BUSY_LOOPS              100000

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct da1470x_wdt_lower_s
{
  const struct watchdog_ops_s *ops;  /* Lower half operations */
  uint32_t timeout;                  /* The timeout asked for, in ms */
  uint32_t reload;                   /* What it became, in ticks */
  xcpt_t   handler;                  /* Capture handler, if any */
  bool     started;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int wdt_start(struct watchdog_lowerhalf_s *lower);
static int wdt_stop(struct watchdog_lowerhalf_s *lower);
static int wdt_keepalive(struct watchdog_lowerhalf_s *lower);
static int wdt_getstatus(struct watchdog_lowerhalf_s *lower,
                         struct watchdog_status_s *status);
static int wdt_settimeout(struct watchdog_lowerhalf_s *lower,
                          uint32_t timeout);
static xcpt_t wdt_capture(struct watchdog_lowerhalf_s *lower,
                          xcpt_t handler);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct watchdog_ops_s g_wdt_ops =
{
  .start      = wdt_start,
  .stop       = wdt_stop,
  .keepalive  = wdt_keepalive,
  .getstatus  = wdt_getstatus,
  .settimeout = wdt_settimeout,
  .capture    = wdt_capture,
  .ioctl      = NULL,
};

static struct da1470x_wdt_lower_s g_wdt_lower =
{
  .ops     = &g_wdt_ops,
  .timeout = CONFIG_DA1470X_WDT_DEFTIMEOUT,
  .reload  = CONFIG_DA1470X_WDT_DEFTIMEOUT / DA1470X_WDT_TICK_MS,
};

/* The freeze bit of each watchdog, indexed by enum da1470x_wdt_e */

static const uint32_t g_wdt_freezebit[] =
{
  GPREG_SET_FREEZE_FRZ_SYS_WDOG,
  GPREG_SET_FREEZE_FRZ_CMAC_WDOG,
  GPREG_SET_FREEZE_FRZ_SNC_WDOG
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: wdt_wait_write
 *
 * Description:
 *   The counter is written across a clock boundary and the control
 *   register says when the previous write has landed.  Writing before it
 *   has is simply lost, which would leave the watchdog holding an older
 *   and shorter value than the caller believes.
 *
 ****************************************************************************/

static void wdt_wait_write(void)
{
  int i;

  for (i = 0; i < WDT_BUSY_LOOPS; i++)
    {
      if ((getreg32(DA1470X_SYS_WDOG_WATCHDOG_CTRL) &
           SYS_WDOG_WATCHDOG_CTRL_WRITE_BUSY) == 0)
        {
          return;
        }
    }

  wderr("The watchdog is not accepting writes\n");
}

/****************************************************************************
 * Name: wdt_reload
 *
 * Description:
 *   Put the reload value back into the counter.  Bits 31:14 must be zero
 *   for the write to be taken at all: the hardware uses them to reject the
 *   all-ones a runaway is most likely to produce.
 *
 ****************************************************************************/

static void wdt_reload(struct da1470x_wdt_lower_s *priv)
{
  wdt_wait_write();
  putreg32(priv->reload & SYS_WDOG_WATCHDOG_WDOG_VAL_MASK,
           DA1470X_SYS_WDOG_WATCHDOG);
}

/****************************************************************************
 * Name: wdt_start
 ****************************************************************************/

static int wdt_start(struct watchdog_lowerhalf_s *lower)
{
  struct da1470x_wdt_lower_s *priv = (struct da1470x_wdt_lower_s *)lower;
  irqstate_t flags = enter_critical_section();

  wdt_reload(priv);

  /* Let go of both holds: the one this driver applies and the one the
   * start-up code applies before any driver exists.
   */

  modifyreg32(DA1470X_SYS_WDOG_WATCHDOG_CTRL,
              SYS_WDOG_WATCHDOG_CTRL_WDOG_FREEZE_EN, 0);
  putreg32(GPREG_SET_FREEZE_FRZ_SYS_WDOG, DA1470X_GPREG_RESET_FREEZE);

  priv->started = true;
  leave_critical_section(flags);

  wdinfo("started, %" PRIu32 " ms\n", priv->timeout);
  return OK;
}

/****************************************************************************
 * Name: wdt_stop
 ****************************************************************************/

static int wdt_stop(struct watchdog_lowerhalf_s *lower)
{
  struct da1470x_wdt_lower_s *priv = (struct da1470x_wdt_lower_s *)lower;
  irqstate_t flags = enter_critical_section();

  putreg32(GPREG_SET_FREEZE_FRZ_SYS_WDOG, DA1470X_GPREG_SET_FREEZE);
  modifyreg32(DA1470X_SYS_WDOG_WATCHDOG_CTRL, 0,
              SYS_WDOG_WATCHDOG_CTRL_WDOG_FREEZE_EN);

  priv->started = false;
  leave_critical_section(flags);

  /* Saying so once is worth more than a comment nobody reads: this hold
   * is dropped by the hardware the moment PD_SYS goes away.
   */

  wdinfo("stopped -- note that sleep resumes it regardless\n");
  return OK;
}

/****************************************************************************
 * Name: wdt_keepalive
 ****************************************************************************/

static int wdt_keepalive(struct watchdog_lowerhalf_s *lower)
{
  struct da1470x_wdt_lower_s *priv = (struct da1470x_wdt_lower_s *)lower;
  irqstate_t flags = enter_critical_section();

  wdt_reload(priv);

  leave_critical_section(flags);
  return OK;
}

/****************************************************************************
 * Name: wdt_getstatus
 ****************************************************************************/

static int wdt_getstatus(struct watchdog_lowerhalf_s *lower,
                         struct watchdog_status_s *status)
{
  struct da1470x_wdt_lower_s *priv = (struct da1470x_wdt_lower_s *)lower;

  status->flags = 0;
  if (priv->started)
    {
      status->flags |= WDFLAGS_ACTIVE;
    }

  if (priv->handler != NULL)
    {
      status->flags |= WDFLAGS_CAPTURE;
    }
  else
    {
      status->flags |= WDFLAGS_RESET;
    }

  status->timeout  = priv->timeout;
  status->timeleft = da1470x_wdt_timeleft_ms();

  return OK;
}

/****************************************************************************
 * Name: wdt_settimeout
 ****************************************************************************/

static int wdt_settimeout(struct watchdog_lowerhalf_s *lower,
                          uint32_t timeout)
{
  struct da1470x_wdt_lower_s *priv = (struct da1470x_wdt_lower_s *)lower;
  irqstate_t flags;
  uint32_t ticks;

  if (timeout == 0 || timeout > DA1470X_WDT_MAX_TIMEOUT)
    {
      wderr("%" PRIu32 " ms is outside 1..%d\n",
            timeout, DA1470X_WDT_MAX_TIMEOUT);
      return -ERANGE;
    }

  /* Round up: a watchdog that bites early is worse than one that waits a
   * few milliseconds longer than asked.
   */

  ticks = (timeout + DA1470X_WDT_TICK_MS - 1) / DA1470X_WDT_TICK_MS;

  flags = enter_critical_section();
  priv->timeout = ticks * DA1470X_WDT_TICK_MS;
  priv->reload  = ticks;

  if (priv->started)
    {
      wdt_reload(priv);
    }

  leave_critical_section(flags);
  return OK;
}

/****************************************************************************
 * Name: wdt_capture
 *
 * Description:
 *   With a handler installed the watchdog raises an NMI at zero instead of
 *   resetting at once, and the reset follows at -16.  That is 160 ms of
 *   handler, not a reprieve: unless the handler reloads the counter the
 *   part still resets.  Removing the handler puts the plain reset back.
 *
 ****************************************************************************/

static xcpt_t wdt_capture(struct watchdog_lowerhalf_s *lower,
                          xcpt_t handler)
{
  struct da1470x_wdt_lower_s *priv = (struct da1470x_wdt_lower_s *)lower;
  irqstate_t flags = enter_critical_section();
  xcpt_t previous = priv->handler;

  if (handler != NULL)
    {
      irq_attach(DA1470X_IRQ_NMI, handler, NULL);
      modifyreg32(DA1470X_SYS_WDOG_WATCHDOG_CTRL,
                  SYS_WDOG_WATCHDOG_CTRL_NMI_RST, 0);
    }
  else
    {
      modifyreg32(DA1470X_SYS_WDOG_WATCHDOG_CTRL, 0,
                  SYS_WDOG_WATCHDOG_CTRL_NMI_RST);
      irq_detach(DA1470X_IRQ_NMI);
    }

  priv->handler = handler;
  leave_critical_section(flags);

  return previous;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_wdt_initialize
 ****************************************************************************/

int da1470x_wdt_initialize(const char *devpath)
{
  struct da1470x_wdt_lower_s *priv = &g_wdt_lower;
  void *handle;

  /* Reset on timeout until somebody asks for a handler */

  modifyreg32(DA1470X_SYS_WDOG_WATCHDOG_CTRL, 0,
              SYS_WDOG_WATCHDOG_CTRL_NMI_RST);

  /* Register it stopped.  The watchdog is running out of reset and the
   * start-up code freezes it; leaving it that way until something opens
   * the device means nothing resets the board by surprise.
   */

  wdt_stop((struct watchdog_lowerhalf_s *)priv);

  handle = watchdog_register(devpath,
                             (struct watchdog_lowerhalf_s *)priv);
  if (handle == NULL)
    {
      wderr("Failed to register %s\n", devpath);
      return -EEXIST;
    }

  return OK;
}

/****************************************************************************
 * Name: da1470x_wdt_freeze
 ****************************************************************************/

void da1470x_wdt_freeze(enum da1470x_wdt_e which, bool freeze)
{
  uint32_t bit;

  if ((unsigned int)which > DA1470X_WDT_SNC)
    {
      return;
    }

  bit = g_wdt_freezebit[which];
  putreg32(bit, freeze ? DA1470X_GPREG_SET_FREEZE :
                         DA1470X_GPREG_RESET_FREEZE);
}

/****************************************************************************
 * Name: da1470x_wdt_is_frozen
 ****************************************************************************/

bool da1470x_wdt_is_frozen(enum da1470x_wdt_e which)
{
  if ((unsigned int)which > DA1470X_WDT_SNC)
    {
      return false;
    }

  return (getreg32(DA1470X_GPREG_SET_FREEZE) &
          g_wdt_freezebit[which]) != 0;
}

/****************************************************************************
 * Name: da1470x_wdt_timeleft_ms
 ****************************************************************************/

uint32_t da1470x_wdt_timeleft_ms(void)
{
  uint32_t value = getreg32(DA1470X_SYS_WDOG_WATCHDOG);

  /* Past zero the counter runs on negative, which is the sixteen ticks of
   * grace between the NMI and the reset.  There is no time left to report.
   */

  if ((value & SYS_WDOG_WATCHDOG_WDOG_VAL_NEG) != 0)
    {
      return 0;
    }

  return (value & SYS_WDOG_WATCHDOG_WDOG_VAL_MASK) * DA1470X_WDT_TICK_MS;
}

/****************************************************************************
 * Name: da1470x_wdt_snc_reload
 ****************************************************************************/

void da1470x_wdt_snc_reload(uint32_t ticks)
{
  /* Every field of this register is guarded: the write is ignored unless
   * the valid field carries 3 in the same 32-bit access.
   */

  putreg32((ticks & SNC_WDOG_CNT_MASK) | SNC_WDOG_WRITE_VALID,
           DA1470X_SNC_WDOG);
}

/****************************************************************************
 * Name: da1470x_wdt_reset_cause
 ****************************************************************************/

uint32_t da1470x_wdt_reset_cause(void)
{
  uint32_t stat = getreg32(DA1470X_CRG_TOP_RESET_STAT);
  uint32_t cause = 0;

  /* Deliberately read-only.  These bits are sticky and clearing them is
   * tempting, but a zero in this register is what the extended sleep path
   * uses to tell a wake-up from a reset -- so zeroing it here would make
   * the next boot look like a resume.  Whoever wants the bits cleared has
   * to think about that first.
   */

  if ((stat & CRG_TOP_RESET_STAT_WDOGRESET_STAT) != 0)
    {
      cause |= 1 << DA1470X_WDT_SYS;
    }

  if ((stat & CRG_TOP_RESET_STAT_CMAC_WDOGRESET_STAT) != 0)
    {
      cause |= 1 << DA1470X_WDT_CMAC;
    }

  if ((stat & CRG_TOP_RESET_STAT_SNC_WDOGRESET_STAT) != 0)
    {
      cause |= 1 << DA1470X_WDT_SNC;
    }

  return cause;
}

#endif /* CONFIG_DA1470X_WDT */
