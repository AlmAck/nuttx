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
static bool g_pm_in_lowpower;

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
 * Name: pm_enter_lowpower
 *
 * Description:
 *   Common preparation: remember the clock source, drop to XTAL32M when a
 *   faster RC clock is running and allow the core to stop its clock.
 *
 ****************************************************************************/

static void pm_enter_lowpower(void)
{
  if (!g_pm_in_lowpower)
    {
      g_pm_saved_sysclk = da1470x_get_sysclk_src();

      if (g_pm_saved_sysclk == DA1470X_SYSCLK_RCHS_64 ||
          g_pm_saved_sysclk == DA1470X_SYSCLK_RCHS_96)
        {
          /* Only if the crystal is available; otherwise stay put */

          if (da1470x_set_sysclk(DA1470X_SYSCLK_XTAL32M) == OK)
            {
              da1470x_rchs_disable();
            }
        }

      g_pm_in_lowpower = true;
    }

  pm_set_sleepdeep(true);
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

  /* The core's FPU access and context-control registers do not survive
   * the deep-sleep states of this chip; restore them before any task
   * touches the FPU again.
   */

#ifdef CONFIG_ARCH_FPU
  arm_fpuconfig();
#endif

#ifdef CONFIG_DA1470X_PM_EXTENDED_SLEEP
  modifyreg32(DA1470X_CRG_TOP_PMU_CTRL, CRG_TOP_PMU_CTRL_SYS_SLEEP, 0);
#endif

  if (g_pm_in_lowpower)
    {
      if (g_pm_saved_sysclk != da1470x_get_sysclk_src())
        {
          da1470x_set_sysclk(g_pm_saved_sysclk);
        }

      g_pm_in_lowpower = false;
    }
}

/****************************************************************************
 * Name: da1470x_pm_standby
 ****************************************************************************/

void da1470x_pm_standby(void)
{
  pm_enter_lowpower();
}

/****************************************************************************
 * Name: da1470x_pm_sleep
 ****************************************************************************/

void da1470x_pm_sleep(void)
{
  pm_enter_lowpower();

#ifdef CONFIG_DA1470X_PM_EXTENDED_SLEEP
  /* Let the PMU switch PD_SYS off on the next WFI.  The core context is
   * lost; the PDC brings PD_SYS back on a registered trigger and the
   * reset handler resumes through the retained state.
   */

#  ifdef CONFIG_DA1470X_PDC
  da1470x_pdc_ack_all_cm33();
#  endif
  modifyreg32(DA1470X_CRG_TOP_PMU_CTRL, 0, CRG_TOP_PMU_CTRL_SYS_SLEEP);
#endif
}

#endif /* CONFIG_PM */
