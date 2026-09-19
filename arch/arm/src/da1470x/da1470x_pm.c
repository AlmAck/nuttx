/****************************************************************************
 * arch/arm/src/da1470x/da1470x_pm.c
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

/* The NuttX power states map onto the chip like this:
 *
 *   PM_NORMAL   the clock the application asked for, nothing gated.
 *   PM_IDLE     the same clocks; the core stops at WFI until an interrupt.
 *   PM_STANDBY  the system clock drops to the crystal, so the PLL and the
 *               high speed RC oscillator stop, and the core is allowed to
 *               deep sleep: the PMU gates everything nobody is holding.
 *               Every power domain and all of the RAM stay alive, so the
 *               wake-up is the wake-up of an interrupt and nothing else.
 *   PM_SLEEP    as PM_STANDBY, and with CONFIG_DA1470X_PM_EXTENDED_SLEEP
 *               the PMU is also allowed to switch PD_SYS off.
 *
 * Whatever this file lowers it has to be able to put back, so the entry
 * path records the clock source it found and the exit path restores it.
 * Nothing here decides *when* to descend: that is the governor's job, fed
 * by the drivers that hold the system awake with pm_stay() while they have
 * work in flight.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <stdbool.h>
#include <debug.h>

#include <nuttx/arch.h>
#include <nuttx/irq.h>
#include <nuttx/power/pm.h>

#include "arm_internal.h"
#include "nvic.h"
#include "hardware/da1470x_crg_top.h"
#include "da1470x_clockconfig.h"
#include "da1470x_pdc.h"
#include "da1470x_pmu.h"
#include "da1470x_pm.h"

#ifdef CONFIG_PM

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* Clock source in use before a low-power state was entered */

static enum da1470x_sysclk_e g_pm_saved_sysclk = DA1470X_SYSCLK_XTAL32M;
static bool g_pm_clock_lowered;

#ifdef CONFIG_DA1470X_PM_EXTENDED_SLEEP
/* Set while PM_SLEEP is the current state, so that the idle loop knows to
 * save its context before stopping.  Retained across the sleep itself.
 */

static bool g_pm_deepsleep_armed;
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: pm_set_sleepdeep
 ****************************************************************************/

static void pm_set_sleepdeep(bool enable)
{
  uint32_t regval = getreg32(NVIC_SYSCON);

  if (enable)
    {
      regval |= NVIC_SYSCON_SLEEPDEEP;
    }
  else
    {
      regval &= ~NVIC_SYSCON_SLEEPDEEP;
    }

  putreg32(regval, NVIC_SYSCON);
}

/****************************************************************************
 * Name: pm_debugger
 *
 * Description:
 *   Leave the debug port enabled or not.  An enabled debugger keeps parts
 *   of the clock tree alive whether or not a probe is attached, which is
 *   current spent for nothing once the system is asleep.
 *
 *   Turning it off means a probe cannot attach while the part sleeps.  The
 *   way back in is a reset through the reset pin, which restarts the boot
 *   ROM and re-enables the port -- the same reset the flashing script
 *   already performs.
 *
 ****************************************************************************/

#ifdef CONFIG_DA1470X_PM_DISABLE_DEBUGGER
static void pm_debugger(bool enable)
{
  if (enable)
    {
      modifyreg32(DA1470X_CRG_TOP_SYS_CTRL, 0,
                  CRG_TOP_SYS_CTRL_DEBUGGER_ENABLE);
    }
  else
    {
      modifyreg32(DA1470X_CRG_TOP_SYS_CTRL,
                  CRG_TOP_SYS_CTRL_DEBUGGER_ENABLE, 0);
    }
}
#else
#  define pm_debugger(enable)
#endif

/****************************************************************************
 * Name: pm_lower_clock
 *
 * Description:
 *   Drop the system clock to the crystal.  Going through
 *   da1470x_set_sysclk() is what stops the PLL and re-times the flash
 *   controller for the slower clock; the high speed RC oscillator is then
 *   stopped separately because nothing is left running from it.
 *
 *   The crystal itself is left running: it is the reference the radio and
 *   the PLL restart from, and stopping it would cost a millisecond of
 *   settling on every wake-up.
 *
 ****************************************************************************/

static void pm_lower_clock(void)
{
  if (g_pm_clock_lowered)
    {
      return;
    }

  g_pm_saved_sysclk = da1470x_get_sysclk_src();

  if (g_pm_saved_sysclk == DA1470X_SYSCLK_XTAL32M)
    {
      /* Already as slow as this path goes */

      return;
    }

  if (da1470x_set_sysclk(DA1470X_SYSCLK_XTAL32M) < 0)
    {
      /* No crystal, or the switch was refused.  Stay where we are rather
       * than end up on an unknown clock.
       */

      return;
    }

  da1470x_rchs_disable();
  g_pm_clock_lowered = true;

#ifdef CONFIG_DA1470X_PM_LOWER_V12
  /* 32 MHz does not need the full core voltage.  The flash read timing the
   * boot ROM left behind is the constraint here, which is why this is a
   * separate option rather than something the clock switch does by itself.
   */

  da1470x_pmu_set_v12(DA1470X_V12_LEVEL_0P90V);
#endif
}

/****************************************************************************
 * Name: pm_restore_clock
 ****************************************************************************/

static void pm_restore_clock(void)
{
  if (!g_pm_clock_lowered)
    {
      return;
    }

  g_pm_clock_lowered = false;

#ifdef CONFIG_DA1470X_PM_LOWER_V12
  /* Raise the rail before the clock: da1470x_set_sysclk() asks for 1.2 V
   * itself on the way to the PLL, but the RC oscillator speeds need it too
   * and the order matters in both cases.
   */

  da1470x_pmu_set_v12(DA1470X_V12_LEVEL_1P20V);
#endif

  if (g_pm_saved_sysclk != da1470x_get_sysclk_src())
    {
      da1470x_set_sysclk(g_pm_saved_sysclk);
    }
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_pm_normal
 ****************************************************************************/

void da1470x_pm_normal(void)
{
  pm_set_sleepdeep(false);
  pm_debugger(true);

#ifdef CONFIG_DA1470X_PM_EXTENDED_SLEEP
  g_pm_deepsleep_armed = false;
  modifyreg32(DA1470X_CRG_TOP_PMU_CTRL,
              CRG_TOP_PMU_CTRL_SYS_SLEEP |
              CRG_TOP_PMU_CTRL_RESET_ON_WAKEUP, 0);
#endif

  /* The core's FPU access and context-control registers do not survive
   * the deep-sleep states of this chip; restore them before any task
   * touches the FPU again.
   */

#ifdef CONFIG_ARCH_FPU
  arm_fpuconfig();
#endif

  pm_restore_clock();
}

/****************************************************************************
 * Name: da1470x_pm_idle
 *
 * Description:
 *   PM_IDLE keeps the clock tree the application asked for and only stops
 *   the core at WFI.  Waking from it costs nothing, so it is the state to
 *   sit in between two pieces of work that are close together.
 *
 ****************************************************************************/

void da1470x_pm_idle(void)
{
  pm_set_sleepdeep(false);
  pm_debugger(true);

#ifdef CONFIG_DA1470X_PM_EXTENDED_SLEEP
  g_pm_deepsleep_armed = false;
  modifyreg32(DA1470X_CRG_TOP_PMU_CTRL,
              CRG_TOP_PMU_CTRL_SYS_SLEEP |
              CRG_TOP_PMU_CTRL_RESET_ON_WAKEUP, 0);
#endif

  pm_restore_clock();
}

/****************************************************************************
 * Name: da1470x_pm_standby
 ****************************************************************************/

void da1470x_pm_standby(void)
{
  pm_lower_clock();
  pm_debugger(false);
  pm_set_sleepdeep(true);

#ifdef CONFIG_DA1470X_PM_EXTENDED_SLEEP
  g_pm_deepsleep_armed = false;
  modifyreg32(DA1470X_CRG_TOP_PMU_CTRL,
              CRG_TOP_PMU_CTRL_SYS_SLEEP |
              CRG_TOP_PMU_CTRL_RESET_ON_WAKEUP, 0);
#endif
}

/****************************************************************************
 * Name: da1470x_pm_sleep
 ****************************************************************************/

void da1470x_pm_sleep(void)
{
  pm_lower_clock();
  pm_debugger(false);
  pm_set_sleepdeep(true);

#ifdef CONFIG_DA1470X_PM_EXTENDED_SLEEP
  /* Let the PMU switch PD_SYS off on the next stop.  The core context goes
   * away with it, so the idle loop has to save it first -- which is what
   * the armed flag tells it to do.
   *
   * RESET_ON_WAKEUP sends the wake-up through the boot ROM.  That costs a
   * few milliseconds of latency, and buys a processor that comes back with
   * its flash controller and clock tree already set up, which is the only
   * way the resume path can be ordinary code executing in place from
   * flash.
   */

  da1470x_pdc_ack_all_cm33();
  modifyreg32(DA1470X_CRG_TOP_PMU_CTRL, 0,
              CRG_TOP_PMU_CTRL_SYS_SLEEP |
              CRG_TOP_PMU_CTRL_RESET_ON_WAKEUP);
  g_pm_deepsleep_armed = true;
#endif
}

#ifdef CONFIG_DA1470X_PM_EXTENDED_SLEEP

/****************************************************************************
 * Name: da1470x_pm_deepsleep_armed
 ****************************************************************************/

bool da1470x_pm_deepsleep_armed(void)
{
  return g_pm_deepsleep_armed;
}

/****************************************************************************
 * Name: da1470x_pm_resume
 ****************************************************************************/

void da1470x_pm_resume(void)
{
  /* We came back through the reset vector, so the boot ROM left the system
   * on its own clock and the core's coprocessor access as it found it.
   */

  modifyreg32(DA1470X_CRG_TOP_PMU_CTRL,
              CRG_TOP_PMU_CTRL_SYS_SLEEP |
              CRG_TOP_PMU_CTRL_RESET_ON_WAKEUP, 0);
  g_pm_deepsleep_armed = false;

#ifdef CONFIG_ARCH_FPU
  arm_fpuconfig();
#endif

  pm_debugger(true);
  pm_set_sleepdeep(false);

  /* pm_restore_clock() compares against what the hardware says, so it puts
   * back the source the application was using whatever the ROM chose.
   */

  pm_restore_clock();

  /* Anything the PMU switched off with PD_SYS and that the drivers expect
   * to find running again.
   */

  da1470x_pd_enable(DA1470X_PD_TIM);
}

#endif /* CONFIG_DA1470X_PM_EXTENDED_SLEEP */

#endif /* CONFIG_PM */
