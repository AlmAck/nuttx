/****************************************************************************
 * arch/arm/src/da1470x/da1470x_snc.c
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

/* The sensor node controller is the third processor on this part: a
 * Cortex-M0+ at up to 32 MHz, with the peripherals of PD_SNC -- the UARTs,
 * the SPI and I2C controllers, the DMA -- wired to it, and its own view of
 * two RAM cells the datasheet names "SNC code" and "SNC data".
 *
 * What it is for is the work that would otherwise wake the application
 * processor for no good reason.  A wake of the M33 was measured on this
 * board at about 39 uC, so polling a sensor at a hundred hertz from the
 * M33 costs several milliamps before the sensor is even read; the same
 * poll on this core costs a fraction of that, and the M33 hears about it
 * only when something interesting has happened.
 *
 * Starting it is three registers: power its domain, give it a clock, let
 * go of its reset.  Everything else is the firmware's business.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>
#include <debug.h>

#include <nuttx/arch.h>
#include <nuttx/irq.h>
#include <nuttx/clock.h>

#include "arm_internal.h"
#include "hardware/da1470x_crg_top.h"
#include "hardware/da1470x_gpreg.h"
#include "da1470x_pmu.h"
#include "da1470x_snc.h"

#ifdef CONFIG_DA1470X_SNC

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Status register of the controller itself */

#define DA1470X_SNC_STATUS      (DA1470X_SNC_BASE + 0x0000)

#define SNC_STATUS_CPU_IDLE     (1 << 1)
#define SNC_STATUS_CPU_LOCKED   (1 << 2)
#define SNC_STATUS_WDOG_EXPIRED (1 << 3)

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: snc_hold_reset
 ****************************************************************************/

static void snc_hold_reset(void)
{
  modifyreg32(DA1470X_CRG_TOP_CLK_SNC_CTRL, 0,
              CRG_TOP_CLK_SNC_CTRL_SNC_RESET_REQ);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_snc_start
 ****************************************************************************/

int da1470x_snc_start(const void *fw, size_t len)
{
  irqstate_t flags;
  int ret;

  if (fw == NULL || len < 8 || len > DA1470X_SNC_CODE_SIZE)
    {
      return -EINVAL;
    }

  /* The RAM cells and the processor both live in PD_SNC */

  ret = da1470x_pd_enable(DA1470X_PD_SNC);
  if (ret < 0)
    {
      _err("PD_SNC did not come up: %d\n", ret);
      return ret;
    }

  flags = enter_critical_section();

  /* Load with the processor held still.  Its reset is asserted out of
   * reset, but say so anyway: this is also the path that replaces the
   * firmware of a controller that is already running.
   */

  snc_hold_reset();

  /* Its watchdog is a separate timer that drives the controller's NMI.
   * Freeze it: a firmware that wants it can unfreeze it itself, and one
   * that does not should not be reset by a timer it never knew about.
   */

  putreg32(GPREG_SET_FREEZE_FRZ_SNC_WDOG, DA1470X_GPREG_SET_FREEZE);

  memcpy((void *)DA1470X_SNC_CODE_BASE, fw, len);

  /* The processor reads this through a different port than we wrote it,
   * so make sure the write has landed before it is allowed to fetch.
   */

  __asm__ __volatile__ ("dsb" : : : "memory");

  modifyreg32(DA1470X_CRG_TOP_CLK_SNC_CTRL, 0,
              CRG_TOP_CLK_SNC_CTRL_SNC_CLK_ENABLE);
  modifyreg32(DA1470X_CRG_TOP_CLK_SNC_CTRL,
              CRG_TOP_CLK_SNC_CTRL_SNC_RESET_REQ, 0);

  leave_critical_section(flags);

  _info("started, %zu bytes, SP %08" PRIx32 " PC %08" PRIx32 "\n",
         len, getreg32(DA1470X_SNC_CODE_BASE),
         getreg32(DA1470X_SNC_CODE_BASE + 4));

  return OK;
}

/****************************************************************************
 * Name: da1470x_snc_stop
 ****************************************************************************/

void da1470x_snc_stop(void)
{
  irqstate_t flags = enter_critical_section();

  snc_hold_reset();
  modifyreg32(DA1470X_CRG_TOP_CLK_SNC_CTRL,
              CRG_TOP_CLK_SNC_CTRL_SNC_CLK_ENABLE, 0);

  leave_critical_section(flags);
}

/****************************************************************************
 * Name: da1470x_snc_running
 ****************************************************************************/

bool da1470x_snc_running(void)
{
  uint32_t ctrl = getreg32(DA1470X_CRG_TOP_CLK_SNC_CTRL);

  return (ctrl & CRG_TOP_CLK_SNC_CTRL_SNC_CLK_ENABLE) != 0 &&
         (ctrl & CRG_TOP_CLK_SNC_CTRL_SNC_RESET_REQ) == 0;
}

/****************************************************************************
 * Name: da1470x_snc_status
 ****************************************************************************/

uint32_t da1470x_snc_status(void)
{
  return getreg32(DA1470X_SNC_STATUS);
}

/****************************************************************************
 * Name: da1470x_snc_smoketest
 ****************************************************************************/

#ifdef CONFIG_DA1470X_SNC_SMOKETEST

/* A firmware that proves the core is executing and nothing more.  In the
 * controller's own addresses, RAM2 starts at 0x8000, so it writes a
 * signature there and then counts in the word above it.
 *
 *      .syntax unified
 *      .cpu    cortex-m0plus
 *      .thumb
 *      .word   0x00010000      @ initial SP: top of RAM2
 *      .word   reset + 1       @ reset vector
 *  reset:
 *      ldr     r0, =0x00008000
 *      ldr     r1, =0x5DC00001
 *      str     r1, [r0]
 *      ldr     r0, =0x00008004
 *      movs    r1, #0
 *  loop:
 *      adds    r1, #1
 *      str     r1, [r0]
 *      b       loop
 *
 * Rebuild with tools/da1470x_snc_fw.sh if it ever needs to change.
 */

#define SNC_TEST_SIGNATURE  0x5dc00001
#define SNC_TEST_SIGN_ADDR  (DA1470X_SNC_DATA_BASE + 0)
#define SNC_TEST_COUNT_ADDR (DA1470X_SNC_DATA_BASE + 4)

static const uint32_t g_snc_testfw[] =
{
  0x00010000, 0x00000009, 0x49044803, 0x48046001,
  0x31012100, 0xe7fc6001, 0x00008000, 0x5dc00001,
  0x00008004
};

/****************************************************************************
 * Name: snc_wait_ticks
 *
 * Description:
 *   Spin for at least the given number of system clock ticks and report
 *   how many actually passed.  Bring-up runs before there is anything to
 *   yield to, so spinning is all there is; the point is only that the
 *   measurement is against a clock rather than against a loop count.
 *
 ****************************************************************************/

static clock_t snc_wait_ticks(clock_t ticks)
{
  clock_t start = clock_systime_ticks();
  clock_t now;

  do
    {
      now = clock_systime_ticks();
    }
  while (now - start < ticks);

  return now - start;
}

int da1470x_snc_smoketest(void)
{
  uint32_t first;
  uint32_t second;
  clock_t elapsed;
  int ret;

  /* Poison the words the firmware is going to write, so that finding them
   * set means the controller wrote them and not that the RAM happened to
   * hold something.
   */

  putreg32(0, SNC_TEST_SIGN_ADDR);
  putreg32(0, SNC_TEST_COUNT_ADDR);

  ret = da1470x_snc_start(g_snc_testfw, sizeof(g_snc_testfw));
  if (ret < 0)
    {
      return ret;
    }

  /* Measured against the system clock, not up_udelay(): the delay loop is
   * calibrated by a constant that is easy to get wrong, and a rate
   * reported against a wrong constant is worse than no rate at all.
   */

  snc_wait_ticks(MSEC2TICK(10));

  if (getreg32(SNC_TEST_SIGN_ADDR) != SNC_TEST_SIGNATURE)
    {
      _err("no signature: the controller did not run (status %08" PRIx32
            ")\n", da1470x_snc_status());
      da1470x_snc_stop();
      return -EIO;
    }

  first = getreg32(SNC_TEST_COUNT_ADDR);
  elapsed = snc_wait_ticks(MSEC2TICK(10));
  second = getreg32(SNC_TEST_COUNT_ADDR);

  if (second == first)
    {
      _err("signature but no progress: the controller stalled at %08"
            PRIx32 " (status %08" PRIx32 ")\n",
            first, da1470x_snc_status());
      da1470x_snc_stop();
      return -EIO;
    }

  /* Three instructions to the iteration, so the rate says roughly what
   * clock the controller is being given.
   */

  syslog(LOG_INFO,
         "SNC: running, %" PRIu32 " iterations in %" PRIu32 " ms\n",
         second - first, (uint32_t)TICK2MSEC(elapsed));

  return OK;
}

#endif /* CONFIG_DA1470X_SNC_SMOKETEST */

#endif /* CONFIG_DA1470X_SNC */
