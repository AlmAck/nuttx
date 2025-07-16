/* da1470x_pmu.c - Power Management Unit (PMU) Driver for DA1470x */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdint.h>
#include <nuttx/arch.h>
#include "arm_internal.h"

#include "da1470x_pmu.h"
#include "hardware/da1470x_crg_top.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void da1470x_pwr_init(void)
{
  uint32_t regval;

  /* Enable V12 DCDC or LDO depending on configuration */

  regval = getreg32(DA1470_CRG_TOP_POWER_CTRL);

  /* Ensure V12 rail is enabled */

  regval |= CRG_TOP_DCDC_V12_EN;

  /* Enable V18 and V30 rails */

  regval |= CRG_TOP_DCDC_V18_EN;
  regval |= CRG_TOP_LDO_V30_EN;

  putreg32(regval, DA1470_CRG_TOP_POWER_CTRL);

  /* Set desired voltage levels if needed (default is usually fine) */

  regval = getreg32(DA1470_CRG_TOP_POWER_LVL);
  regval &= ~(CRG_TOP_V12_LEVEL_MASK |
              CRG_TOP_V18_LEVEL_MASK |
              CRG_TOP_V30_LEVEL_MASK);

  regval |= (V12_LEVEL_1P20V << CRG_TOP_V12_LEVEL_POS);
  regval |= (V18_LEVEL_1P80V << CRG_TOP_V18_LEVEL_POS);
  regval |= (V30_LEVEL_3P30V << CRG_TOP_V30_LEVEL_POS);

  putreg32(regval, DA1470_CRG_TOP_POWER_LVL);

  /* Wait for voltage to stabilize */

  while ((getreg32(DA1470_CRG_TOP_ANA_STATUS) &
         (CRG_TOP_BUCK_DCDC_V12_OK |
          CRG_TOP_BUCK_DCDC_V18_OK |
          CRG_TOP_LDO_V30_OK)) !=
         (CRG_TOP_BUCK_DCDC_V12_OK |
          CRG_TOP_BUCK_DCDC_V18_OK |
          CRG_TOP_LDO_V30_OK))
    {
      /* Busy wait */
    }
}

void da1470x_pmu_set_1v2_max(void)
{
  uint32_t regval = getreg32(DA1470_CRG_TOP_POWER_LVL);

  regval &= ~POWER_LVL_REG_V12_LEVEL_MASK;
  regval |= (POWER_LVL_REG_V12_LEVEL_1P2V << POWER_LVL_REG_V12_LEVEL_POS);

  putreg32(regval, DA1470_CRG_TOP_POWER_LVL);

  while ((getreg32(DA1470_CRG_TOP_ANA_STATUS) &
          CRG_TOP_BUCK_DCDC_V12_OK) == 0)
    {
      /* Wait for 1.2V rail to stabilize */
    }
}

void da1470x_pmu_enable_v12_if_needed(void)
{
  uint32_t regval;

  regval = getreg32(DA1470_CRG_TOP_POWER_LVL);

  if ((regval & CRG_TOP_V12_LEVEL_MASK) != V12_LEVEL_1P20V)
    {
      regval = getreg32(DA1470_CRG_TOP_POWER_CTRL);
      regval |= CRG_TOP_DCDC_V12_EN;
      putreg32(regval, DA1470_CRG_TOP_POWER_CTRL);

      regval &= ~CRG_TOP_V12_LEVEL_MASK;
      regval |= V12_LEVEL_1P20V;
      putreg32(regval, DA1470_CRG_TOP_POWER_LVL);
    }

  /* Wait until V12_OK is asserted */

  while ((getreg32(DA1470_CRG_TOP_ANA_STATUS) & CRG_TOP_BUCK_DCDC_V12_OK) == 0)
    {
      /* Spin until 1.2V rail stabilizes */
    }
}