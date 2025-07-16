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

// #include <cstdint>
#include <nuttx/config.h>

#include <assert.h>
#include <debug.h>
#include <stdint.h>
#include <stdio.h>

#include <arch/board/board.h>

#include "arm_internal.h"
#include "chip.h"
#include "da1470x_clockconfig.h"
#include "da1470x_clk.h"
#include "hardware/da1470x_clock.h"
#include "hardware/da1470x_crg_xtal.h"
#include "da1470x_pmu.h"

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

static void da1470x_xtal32m_enable_and_wait(bool boost)
{
  uint32_t regval;

  DEBUGASSERT((getreg32(DA1470_CRG_TOP_POWER_LVL) & CRG_TOP_V12_LEVEL_MASK) ==
              V12_LEVEL_1P20V);

  if (boost)
  {
    regval = getreg32(DA1470_CRG_XTAL_XTAL32M_TRIM);
    regval &= ~CRG_XTAL_XTAL32M_BOOST_TRIM_MASK;
    regval |= (9 << CRG_XTAL_XTAL32M_BOOST_TRIM_POS);
    putreg32(regval, DA1470_CRG_XTAL_XTAL32M_TRIM);
  }

  regval = getreg32(DA1470_CRG_XTAL_XTAL32M_CTRL);
  regval |= CRG_XTAL_XTAL32M_ENABLE;
  putreg32(regval, DA1470_CRG_XTAL_XTAL32M_CTRL);

  while ((getreg32(DA1470_CRG_XTAL_XTAL32M_STAT0) &
          CRG_XTAL_XTAL32M_READY) == 0)
  {
    /* Spin until XTAL32M stabilizes (typically 140–350 us) */
  }
}

static void hw_clk_set_sysclk_xtal32m(void)
{
  putreg32(0x01, CRG_TOP_SWITCH2XTAL);

  while ((getreg32(DA1470_CRG_TOP_CLK_CTRL) & CRG_TOP_RUNNING_AT_XTAL32M) == 0)
  {
    /* Wait for clock switch */
  }
}

static void da1470_stdclockconfig(void)
{
  uint32_t regval;

  irqstate_t flags = enter_critical_section();

  // Todo implement sysclk_RCHS_32 sysclk_RCHS_64 sysclk_RCHS_96


  da1470x_set_sysclk(SYSCLK_RCHS_96); // Set the system clock to RCHS 32MHz

#ifdef DA1470_CLOCK_HFCLK_SRC_RCHS_32
  da1470x_set_sysclk(SYSCLK_RCHS_32);
#elif defined(DA1470_CLOCK_HFCLK_SRC_RCHS_64)
  da1470x_set_sysclk(SYSCLK_RCHS_64);
#elif defined(DA1470_CLOCK_HFCLK_SRC_RCHS_96)
  da1470x_set_sysclk(SYSCLK_RCHS_96);
#endif

#ifdef CONFIG_DA1470_CLOCK_XTAL32M_SRC // CONFIG_DA1470_HFCLK //TODO
  // da1470x_pmu_enable_v12_if_needed();
  // da1470x_xtal32m_enable_and_wait(false);
  // // hw_clk_set_sysclk_xtal32m();
  // da1470x_set_sysclk(SYSCLK_XTAL32M);
#endif

#ifdef CONFIG_DA1470_USE_LFCLK
  /* Initialize LFCLK (Low Power clock) */

#if defined(CONFIG_DA1470_CLOCK_LPCLK_SRC_RCLP)
  /* Enable RCLP clock */
  regval = getreg32(DA1470_CRG_TOP_CLK_RCLP);
  regval |= CRG_TOP_RCLP_ENABLE;
  putreg32(regval, DA1470_CRG_TOP_CLK_RCLP);

  /* Set LP_CLK_SEL to RCLP */
  regval = getreg32(DA1470_CRG_TOP_CLK_CTRL);
  regval &= ~CRG_TOP_LP_CLK_SEL_MASK;
  regval |= (0x00 << CRG_TOP_LP_CLK_SEL_POS);
  putreg32(regval, DA1470_CRG_TOP_CLK_CTRL);

#elif defined(CONFIG_DA1470_CLOCK_LPCLK_SRC_RCX)
  /* Enable RCX clock */
  regval = getreg32(DA1470_CRG_TOP_CLK_RCX);
  regval |= CRG_TOP_RCX_ENABLE;
  putreg32(regval, DA1470_CRG_TOP_CLK_RCX);

  /* Set LP_CLK_SEL to RCX */
  regval = getreg32(DA1470_CRG_TOP_CLK_CTRL);
  regval &= ~CRG_TOP_LP_CLK_SEL_MASK;
  regval |= (0x01 << CRG_TOP_LP_CLK_SEL_POS);
  putreg32(regval, DA1470_CRG_TOP_CLK_CTRL);

#elif defined(CONFIG_DA1470_CLOCK_LPCLK_SRC_XTAL32K)
  /* Enable XTAL32K clock */
  regval = getreg32(DA1470_CRG_TOP_CLK_XTAL32K);
  regval |= CRG_TOP_XTAL32K_ENABLE;
  putreg32(regval, DA1470_CRG_TOP_CLK_XTAL32K);

  /* Set LP_CLK_SEL to XTAL32K */
  regval = getreg32(DA1470_CRG_TOP_CLK_CTRL);
  regval &= ~CRG_TOP_LP_CLK_SEL_MASK;
  regval |= (0x02 << CRG_TOP_LP_CLK_SEL_POS);
  putreg32(regval, DA1470_CRG_TOP_CLK_CTRL);

#elif defined(CONFIG_DA1470_CLOCK_LPCLK_SRC_EXTERNAL)
  /* No action needed for external clock */
  /* External clock uses the default setup or is set elsewhere */
#endif /* CONFIG_DA1470_CLOCK_LPCLK_SRC_EXTERNAL */

#endif /* CONFIG_DA1470_USE_LFCLK */

  /* Set clock dividers */

  // TODO extend with dinamic value

  // set_hclk_div(0);
  // set_pclk_div(0);

  leave_critical_section(flags);
}

static inline void set_hclk_div(uint32_t div)
{
  uint32_t reg_val;

  // Read the current value of the register
  reg_val = getreg32(DA1470_CRG_TOP_CLK_AMBA);

  // Clear the HCLK_DIV field
  reg_val &= ~CRG_TOP_HCLK_DIV_MASK;

  // Set the new value in the HCLK_DIV field
  reg_val |= (div << CRG_TOP_HCLK_DIV_POS) & CRG_TOP_HCLK_DIV_MASK;

  // Write the updated value back to the register
  putreg32(reg_val, DA1470_CRG_TOP_CLK_AMBA);
}

static inline void set_pclk_div(uint32_t div)
{
  uint32_t reg_val;

  // Read the current value of the register
  reg_val = getreg32(DA1470_CRG_TOP_CLK_AMBA);

  // Clear the PCLK_DIV field
  reg_val &= ~CRG_TOP_PCLK_DIV_MASK;

  // Set the new value in the PCLK_DIV field
  reg_val |= (div << CRG_TOP_PCLK_DIV_POS) & CRG_TOP_PCLK_DIV_MASK;

  // Write the updated value back to the register
  putreg32(reg_val, DA1470_CRG_TOP_CLK_AMBA);
}

/****************************************************************************
 * Name: da1470_amba_enableperipherals
 ****************************************************************************/

static inline void da1470_amba_enableperipherals(void)
{
  uint32_t cr;
  cr = getreg32(DA1470_CRG_TOP_CLK_AMBA);

  // TODO extend with a configurable divider

  // set divider, The AMBA High Speed Bus divider
  cr |= (CLK_AMBA_REG_HCLK_DIV_MASK & CLK_AMBA_REG_HCLK_DIV1);

  // set divider, The AMBA Peripheral Bus divider
  cr |= (CLK_AMBA_REG_PCLK_DIV_MASK & CLK_AMBA_REG_HCLK_DIV1);

  putreg32(cr, DA1470_CRG_TOP_CLK_AMBA);
}

static void da1470x_switch_to_rchs(rchs_speed_t mode)
{
  uint32_t regval;

  regval = getreg32(DA1470_CRG_TOP_CLK_RCHS);
  regval |= CRG_TOP_RCHS_ENABLE;
  regval &= ~CRG_TOP_RCHS_SPEED_MASK;
  regval |= (mode << CRG_TOP_RCHS_SPEED_POS);
  putreg32(regval, DA1470_CRG_TOP_CLK_RCHS);

  // hw_clk_enable_rchs();

  //da1470x_switch_sysclk(SYSCLK_RCHS_32);

  /* Switch SYS_CLK to RCHS */
  regval = getreg32(DA1470_CRG_TOP_CLK_CTRL);
  regval &= ~CRG_TOP_SYS_CLK_SEL_MASK;
  regval |= CLK_SEL_RCHS;
  putreg32(regval, DA1470_CRG_TOP_CLK_CTRL);

  /* Wait for SYS_CLK to be running at RCHS */
  while ((getreg32(DA1470_CRG_TOP_CLK_CTRL) &
          CRG_TOP_RUNNING_AT_RCHS) == 0)
  {
    /* Busy wait */
  }
}

void da1470x_set_sysclk(sys_clk_t type)
{
  irqstate_t flags;
  sys_clk_t sysclk_booter;
  sys_clk_t sysclk;

  flags = enter_critical_section();

  /* Get current clock (stubbed here for now) */
  sysclk = sysclk_booter = SYSCLK_RCHS_32;

  if (type == SYSCLK_BOOTER)
  {
    type = sysclk_booter;
  }

  /* Handle RCHS clock selection */

  if (type == SYSCLK_RCHS_32 ||
      type == SYSCLK_RCHS_64 ||
      type == SYSCLK_RCHS_96)
  {
    rchs_speed_t rchs_mode = (type == SYSCLK_RCHS_32) ? RCHS_32 : (type == SYSCLK_RCHS_64) ? RCHS_64
                                                                                           : RCHS_96;

    if (type != SYSCLK_RCHS_32)
    {
      /* Set 1.2V domain to maximum voltage if needed */
      da1470x_pmu_set_1v2_max(); /* You must implement this */
    }

    da1470x_switch_to_rchs(rchs_mode);
  }

  leave_critical_section(flags);
}

/**
 * \brief Switch System clock.
 *
 * \param[in] mode The new system clock.
 *
 * \note System clock switch to PLL is only allowed when current system clock is XTAL32M.
 * System clock switch from PLL is only allowed when new system clock is XTAL32M.
 */

static inline void da1470x_switch_sysclk(sys_clk_is_t mode)
{
  /* Make sure a valid sys clock is requested */

  DEBUGASSERT(mode <= SYS_CLK_IS_PLL);

  /* Switch to PLL is only allowed when current system clock is XTAL32M */

  DEBUGASSERT(mode != SYS_CLK_IS_PLL ||
              (getreg32(DA1470_CRG_TOP_CLK_CTRL) & CRG_TOP_RUNNING_AT_XTAL32M) ||
              (getreg32(DA1470_CRG_TOP_CLK_CTRL) & CRG_TOP_RUNNING_AT_PLL));

  /* Switch from PLL is only allowed when new system clock is XTAL32M */

  DEBUGASSERT(!(getreg32(DA1470_CRG_TOP_CLK_CTRL) & CRG_TOP_RUNNING_AT_PLL) ||
              mode == SYS_CLK_IS_XTAL32M ||
              mode == SYS_CLK_IS_PLL);

  uint32_t regval = getreg32(DA1470_CRG_TOP_CLK_CTRL);

  if (mode == SYS_CLK_IS_XTAL32M && (regval & CRG_TOP_RUNNING_AT_RCHS) != 0)
  {
    // When writing to this register, the clock switch will
    // happen from RC32M to XTAL32M. If any other
    // clock is selected than RC32M, the selection is
    // discarded.
    putreg32(0x01, DA1470_CRG_TOP_CLK_SWITCH2XTAL);
  }
  else
  {
    regval &= ~CRG_TOP_SYS_CLK_SEL_MASK;                                    /* Clear the bits for SYS_CLK_SEL */
    regval |= (mode << CRG_TOP_SYS_CLK_SEL_POS) & CRG_TOP_SYS_CLK_SEL_MASK; /* Set new mode */
    putreg32(regval, DA1470_CRG_TOP_CLK_CTRL);
  }

  /* Wait until the switch is done */

  switch (mode)
  {
  case SYS_CLK_IS_XTAL32M:
    while (!(getreg32(DA1470_CRG_TOP_CLK_CTRL) & CRG_TOP_RUNNING_AT_XTAL32M))
      ;
    return;

  case SYS_CLK_IS_RCHS:
    while (!(getreg32(DA1470_CRG_TOP_CLK_CTRL) & CRG_TOP_RUNNING_AT_RCHS))
      ;
    return;

  case SYS_CLK_IS_RCLP:
    while (!(getreg32(DA1470_CRG_TOP_CLK_CTRL) & CRG_TOP_RUNNING_AT_RCLP))
      ;
    return;

  case SYS_CLK_IS_PLL:
    while (!(getreg32(DA1470_CRG_TOP_CLK_CTRL) & CRG_TOP_RUNNING_AT_PLL))
      ;
    return;
  default:
    DEBUGASSERT(0);
  }
}

// /**
//  * @brief Return the clock used as the system clock.
//  *
//  * @return The type of the system clock
//  */
// static inline sys_clk_is_t hw_clk_get_sysclk(void)
// {
//     static const uint32_t freq_msk = CRG_TOP_RUNNING_AT_RCLP |
//                                      CRG_TOP_RUNNING_AT_RCHS |
//                                      CRG_TOP_RUNNING_AT_XTAL32M |
//                                      CRG_TOP_RUNNING_AT_PLL;

//     static const sys_clk_is_t clocks[] = {
//         SYS_CLK_IS_RCLP,        /* 0b000 */
//         SYS_CLK_IS_RCHS,        /* 0b001 */
//         SYS_CLK_IS_XTAL32M,     /* 0b010 */
//         SYS_CLK_IS_INVALID,
//         SYS_CLK_IS_PLL          /* 0b100 */
//     };

//     /* Drop bit0 to reduce the size of clocks[] */

//     uint32_t index = (getreg32(DA1470_CRG_TOP_CLK_CTRL) & freq_msk) >> (CRG_TOP_RUNNING_AT_RCLP + 1);

//     DEBUGASSERT(index <= 4);

//     sys_clk_is_t clk = clocks[index];
//     DEBUGASSERT(clk != SYS_CLK_IS_INVALID);

//     return clk;
// }

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
void da1470_clockenable(void)
{
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
