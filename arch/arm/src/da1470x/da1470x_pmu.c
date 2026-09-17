/****************************************************************************
 * arch/arm/src/da1470x/da1470x_pmu.c
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
#include <errno.h>

#include <nuttx/arch.h>

#include "arm_internal.h"
#include "hardware/da1470x_crg_top.h"
#include "da1470x_pmu.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Rails and domains settle in tens of microseconds; the loop count below
 * is a generous upper bound at any supported core clock.
 */

#define PMU_WAIT_LOOPS      200000

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct da1470x_pd_s
{
  uint32_t sleep_mask;  /* PMU_CTRL bit that puts the domain to sleep */
  uint32_t up_mask;     /* SYS_STAT bit that reports the domain is up */
};

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct da1470x_pd_s g_pd_table[DA1470X_NPD] =
{
  [DA1470X_PD_SNC]  =
  {
    CRG_TOP_PMU_CTRL_SNC_SLEEP,   CRG_TOP_SYS_STAT_SNC_IS_UP
  },
  [DA1470X_PD_TIM]  =
  {
    CRG_TOP_PMU_CTRL_TIM_SLEEP,   CRG_TOP_SYS_STAT_TIM_IS_UP
  },
  [DA1470X_PD_AUD]  =
  {
    CRG_TOP_PMU_CTRL_AUD_SLEEP,   CRG_TOP_SYS_STAT_AUD_IS_UP
  },
  [DA1470X_PD_GPU]  =
  {
    CRG_TOP_PMU_CTRL_GPU_SLEEP,   CRG_TOP_SYS_STAT_GPU_IS_UP
  },
  [DA1470X_PD_CTRL] =
  {
    CRG_TOP_PMU_CTRL_CTRL_SLEEP,  CRG_TOP_SYS_STAT_CTRL_IS_UP
  },
  [DA1470X_PD_RAD]  =
  {
    CRG_TOP_PMU_CTRL_RADIO_SLEEP, CRG_TOP_SYS_STAT_RAD_IS_UP
  },
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: pmu_wait_bits
 *
 * Description:
 *   Spin until all bits in mask are set in the register at addr, or the
 *   loop budget expires.
 *
 ****************************************************************************/

static int pmu_wait_bits(uintptr_t addr, uint32_t mask)
{
  int i;

  for (i = 0; i < PMU_WAIT_LOOPS; i++)
    {
      if ((getreg32(addr) & mask) == mask)
        {
          return OK;
        }
    }

  return -ETIMEDOUT;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_pwr_init
 ****************************************************************************/

void da1470x_pwr_init(void)
{
  uint32_t regval;

  /* Enable the V12 buck, the V18 buck and the V30 LDO */

  regval  = getreg32(DA1470X_CRG_TOP_POWER_CTRL);
  regval |= CRG_TOP_POWER_CTRL_DCDC_V12_EN |
            CRG_TOP_POWER_CTRL_DCDC_V18_EN |
            CRG_TOP_POWER_CTRL_LDO_V30_EN;
  putreg32(regval, DA1470X_CRG_TOP_POWER_CTRL);

  /* Nominal levels: 1.2 V core, 3.3 V I/O */

  regval  = getreg32(DA1470X_CRG_TOP_POWER_LVL);
  regval &= ~(CRG_TOP_POWER_LVL_V12_LEVEL_MASK |
              CRG_TOP_POWER_LVL_V30_LEVEL_MASK);
  regval |= CRG_TOP_POWER_LVL_V12_LEVEL(DA1470X_V12_LEVEL_1P20V) |
            CRG_TOP_POWER_LVL_V30_LEVEL(DA1470X_V30_LEVEL_3P30V);
  putreg32(regval, DA1470X_CRG_TOP_POWER_LVL);

  /* Wait for the rails.  On a failure there is nothing sensible to do this
   * early; continue and let the console show the problem.
   */

  pmu_wait_bits(DA1470X_CRG_TOP_ANA_STATUS,
                CRG_TOP_ANA_STATUS_BUCK_DCDC_V12_OK |
                CRG_TOP_ANA_STATUS_BUCK_DCDC_V18_OK |
                CRG_TOP_ANA_STATUS_LDO_V30_OK);
}

/****************************************************************************
 * Name: da1470x_pmu_set_v12
 ****************************************************************************/

int da1470x_pmu_set_v12(uint32_t level)
{
  uint32_t regval;

  regval  = getreg32(DA1470X_CRG_TOP_POWER_LVL);
  regval &= ~CRG_TOP_POWER_LVL_V12_LEVEL_MASK;
  regval |= CRG_TOP_POWER_LVL_V12_LEVEL(level);
  putreg32(regval, DA1470X_CRG_TOP_POWER_LVL);

  return pmu_wait_bits(DA1470X_CRG_TOP_ANA_STATUS,
                       CRG_TOP_ANA_STATUS_BUCK_DCDC_V12_OK);
}

/****************************************************************************
 * Name: da1470x_pd_enable
 ****************************************************************************/

int da1470x_pd_enable(enum da1470x_pd_e pd)
{
  irqstate_t flags;

  if ((unsigned int)pd >= DA1470X_NPD)
    {
      return -EINVAL;
    }

  /* Clear the SLEEP bit to request power-up */

  flags = enter_critical_section();
  modifyreg32(DA1470X_CRG_TOP_PMU_CTRL, g_pd_table[pd].sleep_mask, 0);
  leave_critical_section(flags);

  return pmu_wait_bits(DA1470X_CRG_TOP_SYS_STAT, g_pd_table[pd].up_mask);
}

/****************************************************************************
 * Name: da1470x_pd_disable
 ****************************************************************************/

void da1470x_pd_disable(enum da1470x_pd_e pd)
{
  irqstate_t flags;

  if ((unsigned int)pd >= DA1470X_NPD)
    {
      return;
    }

  /* Set the SLEEP bit; the domain goes down asynchronously */

  flags = enter_critical_section();
  modifyreg32(DA1470X_CRG_TOP_PMU_CTRL, 0, g_pd_table[pd].sleep_mask);
  leave_critical_section(flags);
}

/****************************************************************************
 * Name: da1470x_pd_is_up
 ****************************************************************************/

bool da1470x_pd_is_up(enum da1470x_pd_e pd)
{
  if ((unsigned int)pd >= DA1470X_NPD)
    {
      return false;
    }

  return (getreg32(DA1470X_CRG_TOP_SYS_STAT) & g_pd_table[pd].up_mask) != 0;
}
