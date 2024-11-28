/****************************************************************************
 * arch/arm/src/da1470x/da1470x_clockconfig.c
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

#include <assert.h>
#include <debug.h>
#include <stdint.h>
#include <stdio.h>

#include <arch/board/board.h>

#include "arm_internal.h"
#include "chip.h"
//#include "da1470x_clk.h"
#include "da1470x_clockconfig.h"
#include "hardware/da1470x_clock.h"


/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Allow up to 100 milliseconds for the high speed clock to become ready.
 * that is a very long delay, but if the clock does not become ready we are
 * hosed anyway.
 */

#define HSERDY_TIMEOUT (100 * CONFIG_BOARD_LOOPSPERMSEC)

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name
 *
 * Description:
 *   Called to initialize the NRF53xxx.  This does whatever setup is needed
 *   to put the MCU in a usable state.  This includes the initialization of
 *   clocking using the settings in board.h.  This function also performs
 *   other low-level chip as necessary.
 *
 ****************************************************************************/


void da1470_clockconfig(void)
{
#if 0

#endif
#if defined(CONFIG_ARCH_BOARD_DA1470_CUSTOM_CLOCKCONFIG)

  /* Invoke Board Custom Clock Configuration */

  da1470_board_clockconfig();

#else

  /* Invoke standard, fixed clock configuration based on definitions in
   * board.h
   */

  da1470_stdclockconfig();

#endif

  /* Enable peripheral clocking */

  da1470_amba_enableperipherals();
}

static void da1470_stdclockconfig(void)
{
  // TODO extend with board.h definitions
uint32_t regval;

#ifdef CONFIG_DA1470_HFCLK
  /* Initialize HFCLK crystal oscillator */
  // XTAL
  putreg32(0x1, DA1470_CRG_TOP_CLK_SWITCH2XTAL);

      while (!(getreg32(DA1470_CRG_TOP_CLK_CTRL) & RUNNING_AT_XTAL32M_MASK)) {
    /* wait for external oscillator to start */
  }
#endif

#ifdef CONFIG_DA1470_USE_LFCLK
/* Initialize LFCLK */

/* Initialize Low Power clock */
#if defined(CLOCK_LPCLKSRC_SRC_RCLP)
  /* Enable RCLP clock */
  regval = getreg32(DA1470_CRG_TOP_CLK_RCLP);
  regval |= CRG_TOP_RCLP_ENABLE;
  putreg32(regval, DA1470_CRG_TOP_CLK_RCLP);

  /* Set LP_CLK_SEL to RCLP */
  regval = getreg32(DA1470_CRG_TOP_CLK_CTRL);
  regval = (regval & ~CRG_TOP_LP_CLK_SEL_MASK) | (0x00 << CRG_TOP_LP_CLK_SEL_POS);
  putreg32(regval, DA1470_CRG_TOP_CLK_CTRL);

#elif defined(CLOCK_LPCLKSRC_SRC_RCX)
  /* Enable RCX clock */
  regval = getreg32(DA1470_CRG_TOP_CLK_RCX);
  regval |= CRG_TOP_RCX_ENABLE;
  putreg32(regval, DA1470_CRG_TOP_CLK_RCX);

  /* Set LP_CLK_SEL to RCX */
  regval = getreg32(DA1470_CRG_TOP_CLK_CTRL);
  regval = (regval & ~CRG_TOP_LP_CLK_SEL_MASK) | (0x01 << CRG_TOP_LP_CLK_SEL_POS);
  putreg32(regval, DA1470_CRG_TOP_CLK_CTRL);

#elif defined(CLOCK_LPCLKSRC_SRC_XTAL32K)
  /* Enable XTAL32K clock */
  regval = getreg32(DA1470_CRG_TOP_CLK_XTAL32K);
  regval |= CRG_TOP_XTAL32K_ENABLE;
  putreg32(regval, DA1470_CRG_TOP_CLK_XTAL32K);

  /* Set LP_CLK_SEL to XTAL32K */
  regval = getreg32(DA1470_CRG_TOP_CLK_CTRL);
  regval = (regval & ~CRG_TOP_LP_CLK_SEL_MASK) | (0x02 << CRG_TOP_LP_CLK_SEL_POS);
  putreg32(regval, DA1470_CRG_TOP_CLK_CTRL);

#elif defined(CLOCK_LPCLKSRC_SRC_EXTERNAL)
  /* No action needed for external clock */
  /* External clock uses the default setup or is set elsewhere */

#endif /* CONFIG_DA1470_USE_LFCLK */

/* Initialize system clock */
#if defined(CLOCK_SYSCLKSRC_SRC_RCHS_32)
#elif defined(CLOCK_SYSCLKSRC_SRC_XTAL32M)
  xtal32m_settled = (REG_GETF(CRG_XTAL, XTAL32M_STAT0_REG, XTAL32M_READY));
  if (xtal32m_settled == false) {
    // XTAL32M has not been started. Use PDC to start it.
    // Check the power supply
    ASSERT_WARNING(REG_GETF(CRG_TOP, POWER_CTRL_REG, DCDC_V14_EN));
    /* Enable the XTAL oscillator. */
    GLOBAL_INT_DISABLE();
    REG_SET_BIT(CRG_XTAL, XTAL32M_CTRL_REG, XTAL32M_ENABLE);
    GLOBAL_INT_RESTORE();
  }

#elif defined(CLOCK_SYSCLKSRC_SRC_RCHS_64)
#elif defined(CLOCK_SYSCLKSRC_SRC_RCHS_96)
#elif defined(CLOCK_SYSCLKSRC_SRC_PLL160)
#endif

#if defined(CONFIG_NRF53_LFCLK_RC)
  /* TODO: calibrate LFCLK RC oscillator */
#endif
#endif
}

/****************************************************************************
 * Name: da1470_amba_enableperipherals
 ****************************************************************************/

static inline void da1470_amba_enableperipherals(void) {
  uint32_t cr;
  cr = getreg32(DA1470_CRG_TOP_CLK_AMBA);

  // TODO extend with a configurable divider

  // set divider, The AMBA High Speed Bus divider
  cr |= (CLK_AMBA_REG_HCLK_DIV_MASK & CLK_AMBA_REG_HCLK_DIV1);

  // set divider, The AMBA Peripheral Bus divider
  cr |= (CLK_AMBA_REG_PCLK_DIV_MASK & CLK_AMBA_REG_HCLK_DIV1);

  putreg32(cr, DA1470_CRG_TOP_CLK_AMBA);
}

/****************************************************************************
 * Name
 *
 * Description
 *   Re-enable the clock and restore the clock settings based on settings in
 *   board.h.  This function is only available to support low-power modes of
 *   operation
 *   re-enable/re-start the PLL
 *
 *   This function performs a subset of the operations performed by
 *   da1470_clockconfig()
 *   reset the currently enabled peripheral clocks.
 *
 *   If CONFIG_ARCH_BOARD_DA1470_CUSTOM_CLOCKCONFIG is defined, then
 *   clocking will be enabled by an externally provided, board-specific
 *   function called da1470_board_clockconfig().
 *
 * Input Parameters
 *   None
 *
 * Returned Value
 *   None
 *
 ****************************************************************************/

#ifdef CONFIG_PM
void da1470_clockenable(void) {
#if defined(CONFIG_ARCH_BOARD_DA1470_CUSTOM_CLOCKCONFIG)

  /* Invoke Board Custom Clock Configuration */

  da1470_board_clockconfig();

#else

  /* Invoke standard, fixed clock configuration based on definitions in
   * board.h
   */

  da1470_stdclockconfig();

#endif
}
#endif
