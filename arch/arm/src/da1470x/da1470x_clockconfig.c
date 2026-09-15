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

#include <stdint.h>
#include <errno.h>

#include <nuttx/arch.h>
#include <nuttx/irq.h>

#include "arm_internal.h"
#include "hardware/da1470x_crg_top.h"
#include "hardware/da1470x_crg_xtal.h"
#include "da1470x_clockconfig.h"
#include "da1470x_pmu.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* RCHS_SPEED field encoding (CLK_RCHS_REG) */

#define RCHS_SPEED_32           0
#define RCHS_SPEED_96           1
#define RCHS_SPEED_64           2

/* SYS_CLK_SEL field encoding (CLK_CTRL_REG) */

#define SYS_CLK_SEL_XTAL32M     0
#define SYS_CLK_SEL_RCHS        1
#define SYS_CLK_SEL_RCLP        2
#define SYS_CLK_SEL_PLL         3

/* LP_CLK_SEL field encoding (CLK_CTRL_REG) */

#define LP_CLK_SEL_RCLP         0
#define LP_CLK_SEL_RCX          1
#define LP_CLK_SEL_XTAL32K      2
#define LP_CLK_SEL_EXTERNAL     3

/* The crystal settles within 2 ms; allow a wide margin. */

#define XTAL32M_WAIT_LOOPS      2000000
#define CLKSW_WAIT_LOOPS        100000

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: clk_wait_bits
 ****************************************************************************/

static int clk_wait_bits(uintptr_t addr, uint32_t mask, int loops)
{
  while (loops-- > 0)
    {
      if ((getreg32(addr) & mask) != 0)
        {
          return OK;
        }
    }

  return -ETIMEDOUT;
}

/****************************************************************************
 * Name: clk_rchs_enable
 *
 * Description:
 *   Program the RCHS speed and make sure the oscillator runs.
 *
 ****************************************************************************/

static void clk_rchs_enable(uint32_t speed)
{
  uint32_t regval;

  regval  = getreg32(DA1470X_CRG_TOP_CLK_RCHS);
  regval &= ~CRG_TOP_CLK_RCHS_RCHS_SPEED_MASK;
  regval |= CRG_TOP_CLK_RCHS_RCHS_SPEED(speed) |
            CRG_TOP_CLK_RCHS_RCHS_ENABLE;
  putreg32(regval, DA1470X_CRG_TOP_CLK_RCHS);
}

/****************************************************************************
 * Name: clk_select_sysclk
 *
 * Description:
 *   Perform the actual system clock multiplexer switch and wait for the
 *   RUNNING_AT_* status bit.
 *
 ****************************************************************************/

static int clk_select_sysclk(uint32_t sel, uint32_t running)
{
  uint32_t regval;

  regval = getreg32(DA1470X_CRG_TOP_CLK_CTRL);

  /* Switching from RCHS to XTAL32M must go through the dedicated
   * SWITCH2XTAL register; any other transition uses SYS_CLK_SEL.
   */

  if (sel == SYS_CLK_SEL_XTAL32M &&
      (regval & CRG_TOP_CLK_CTRL_RUNNING_AT_RCHS) != 0)
    {
      putreg32(CRG_TOP_CLK_SWITCH2XTAL_SWITCH2XTAL,
               DA1470X_CRG_TOP_CLK_SWITCH2XTAL);
    }
  else
    {
      regval &= ~CRG_TOP_CLK_CTRL_SYS_CLK_SEL_MASK;
      regval |= CRG_TOP_CLK_CTRL_SYS_CLK_SEL(sel);
      putreg32(regval, DA1470X_CRG_TOP_CLK_CTRL);
    }

  return clk_wait_bits(DA1470X_CRG_TOP_CLK_CTRL, running, CLKSW_WAIT_LOOPS);
}

/****************************************************************************
 * Name: clk_lpclk_config
 *
 * Description:
 *   Enable and select the low power clock chosen by Kconfig.
 *
 ****************************************************************************/

static void clk_lpclk_config(void)
{
#ifdef CONFIG_DA1470X_USE_LFCLK
  uint32_t regval;
  uint32_t sel;

#if defined(CONFIG_DA1470X_CLOCK_LPCLK_SRC_RCX)
  modifyreg32(DA1470X_CRG_TOP_CLK_RCX, 0, CRG_TOP_CLK_RCX_RCX_ENABLE);
  sel = LP_CLK_SEL_RCX;
#elif defined(CONFIG_DA1470X_CLOCK_LPCLK_SRC_XTAL32K)
  modifyreg32(DA1470X_CRG_TOP_CLK_XTAL32K, 0,
              CRG_TOP_CLK_XTAL32K_XTAL32K_ENABLE);
  sel = LP_CLK_SEL_XTAL32K;
#elif defined(CONFIG_DA1470X_CLOCK_LPCLK_SRC_EXTERNAL)
  sel = LP_CLK_SEL_EXTERNAL;
#else
  modifyreg32(DA1470X_CRG_TOP_CLK_RCLP, 0, CRG_TOP_CLK_RCLP_RCLP_ENABLE);
  sel = LP_CLK_SEL_RCLP;
#endif

  regval  = getreg32(DA1470X_CRG_TOP_CLK_CTRL);
  regval &= ~CRG_TOP_CLK_CTRL_LP_CLK_SEL_MASK;
  regval |= CRG_TOP_CLK_CTRL_LP_CLK_SEL(sel);
  putreg32(regval, DA1470X_CRG_TOP_CLK_CTRL);
#endif
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_xtal32m_enable
 ****************************************************************************/

int da1470x_xtal32m_enable(void)
{
  if ((getreg32(DA1470X_CRG_XTAL_XTAL32M_STAT0) &
       CRG_XTAL_XTAL32M_STAT0_XTAL32M_READY) != 0)
    {
      return OK;
    }

  /* The crystal driver is supplied from the V14 rail */

  modifyreg32(DA1470X_CRG_TOP_POWER_CTRL, 0,
              CRG_TOP_POWER_CTRL_DCDC_V14_EN);

  modifyreg32(DA1470X_CRG_XTAL_XTAL32M_CTRL, 0,
              CRG_XTAL_XTAL32M_CTRL_XTAL32M_ENABLE);

  return clk_wait_bits(DA1470X_CRG_XTAL_XTAL32M_STAT0,
                       CRG_XTAL_XTAL32M_STAT0_XTAL32M_READY,
                       XTAL32M_WAIT_LOOPS);
}

/****************************************************************************
 * Name: da1470x_xtal32m_disable
 ****************************************************************************/

void da1470x_xtal32m_disable(void)
{
  modifyreg32(DA1470X_CRG_XTAL_XTAL32M_CTRL,
              CRG_XTAL_XTAL32M_CTRL_XTAL32M_ENABLE, 0);
}

/****************************************************************************
 * Name: da1470x_rchs_disable
 ****************************************************************************/

void da1470x_rchs_disable(void)
{
  if ((getreg32(DA1470X_CRG_TOP_CLK_CTRL) &
       CRG_TOP_CLK_CTRL_RUNNING_AT_RCHS) == 0)
    {
      modifyreg32(DA1470X_CRG_TOP_CLK_RCHS,
                  CRG_TOP_CLK_RCHS_RCHS_ENABLE, 0);
    }
}

/****************************************************************************
 * Name: da1470x_set_sysclk
 ****************************************************************************/

int da1470x_set_sysclk(enum da1470x_sysclk_e clk)
{
  irqstate_t flags;
  int ret = OK;

  flags = enter_critical_section();

  switch (clk)
    {
      case DA1470X_SYSCLK_RCHS_32:
      case DA1470X_SYSCLK_RCHS_64:
      case DA1470X_SYSCLK_RCHS_96:
        {
          uint32_t speed = RCHS_SPEED_32;

          if (clk == DA1470X_SYSCLK_RCHS_64)
            {
              speed = RCHS_SPEED_64;
            }
          else if (clk == DA1470X_SYSCLK_RCHS_96)
            {
              speed = RCHS_SPEED_96;
            }

          /* 64/96 MHz need the core at 1.2 V */

          if (speed != RCHS_SPEED_32)
            {
              ret = da1470x_pmu_set_v12(DA1470X_V12_LEVEL_1P20V);
              if (ret < 0)
                {
                  break;
                }
            }

          clk_rchs_enable(speed);
          ret = clk_select_sysclk(SYS_CLK_SEL_RCHS,
                                  CRG_TOP_CLK_CTRL_RUNNING_AT_RCHS);
        }
        break;

      case DA1470X_SYSCLK_XTAL32M:
        ret = da1470x_xtal32m_enable();
        if (ret == OK)
          {
            ret = clk_select_sysclk(SYS_CLK_SEL_XTAL32M,
                                    CRG_TOP_CLK_CTRL_RUNNING_AT_XTAL32M);
          }
        break;

      case DA1470X_SYSCLK_RCLP:
        ret = clk_select_sysclk(SYS_CLK_SEL_RCLP,
                                CRG_TOP_CLK_CTRL_RUNNING_AT_RCLP);
        break;

      default:
        ret = -EINVAL;
        break;
    }

  leave_critical_section(flags);
  return ret;
}

/****************************************************************************
 * Name: da1470x_get_sysclk_src
 ****************************************************************************/

enum da1470x_sysclk_e da1470x_get_sysclk_src(void)
{
  uint32_t ctrl = getreg32(DA1470X_CRG_TOP_CLK_CTRL);

  if ((ctrl & CRG_TOP_CLK_CTRL_RUNNING_AT_XTAL32M) != 0)
    {
      return DA1470X_SYSCLK_XTAL32M;
    }
  else if ((ctrl & CRG_TOP_CLK_CTRL_RUNNING_AT_RCLP) != 0)
    {
      return DA1470X_SYSCLK_RCLP;
    }
  else
    {
      uint32_t rchs = getreg32(DA1470X_CRG_TOP_CLK_RCHS);
      uint32_t speed = (rchs & CRG_TOP_CLK_RCHS_RCHS_SPEED_MASK) >>
                       CRG_TOP_CLK_RCHS_RCHS_SPEED_SHIFT;

      if (speed == RCHS_SPEED_96)
        {
          return DA1470X_SYSCLK_RCHS_96;
        }
      else if (speed == RCHS_SPEED_64)
        {
          return DA1470X_SYSCLK_RCHS_64;
        }

      return DA1470X_SYSCLK_RCHS_32;
    }
}

/****************************************************************************
 * Name: da1470x_get_sysclk
 ****************************************************************************/

uint32_t da1470x_get_sysclk(void)
{
  uint32_t freq;
  uint32_t hdiv;

  switch (da1470x_get_sysclk_src())
    {
      case DA1470X_SYSCLK_RCHS_64:
        freq = DA1470X_RCHS_64_FREQ;
        break;

      case DA1470X_SYSCLK_RCHS_96:
        freq = DA1470X_RCHS_96_FREQ;
        break;

      case DA1470X_SYSCLK_RCLP:
        freq = DA1470X_RCLP_SLOW_FREQ;
        break;

      default:
        freq = DA1470X_XTAL32M_FREQ;
        break;
    }

  /* Apply the AHB divider (HCLK_DIV is a power of two) */

  hdiv = (getreg32(DA1470X_CRG_TOP_CLK_AMBA) &
          CRG_TOP_CLK_AMBA_HCLK_DIV_MASK) >>
         CRG_TOP_CLK_AMBA_HCLK_DIV_SHIFT;

  return freq >> hdiv;
}

/****************************************************************************
 * Name: da1470x_get_divn_clk
 ****************************************************************************/

uint32_t da1470x_get_divn_clk(void)
{
  return DA1470X_DIVN_CLK_FREQ;
}

/****************************************************************************
 * Name: da1470x_get_lpclk
 ****************************************************************************/

uint32_t da1470x_get_lpclk(void)
{
  uint32_t sel = (getreg32(DA1470X_CRG_TOP_CLK_CTRL) &
                  CRG_TOP_CLK_CTRL_LP_CLK_SEL_MASK) >>
                 CRG_TOP_CLK_CTRL_LP_CLK_SEL_SHIFT;

  switch (sel)
    {
      case LP_CLK_SEL_XTAL32K:
        return DA1470X_XTAL32K_FREQ;

      case LP_CLK_SEL_RCX:
        return 32000;   /* Nominal; needs calibration for accuracy */

      case LP_CLK_SEL_RCLP:
      default:
        if ((getreg32(DA1470X_CRG_TOP_CLK_RCLP) &
             CRG_TOP_CLK_RCLP_RCLP_LOW_SPEED_FORCE) != 0)
          {
            return DA1470X_RCLP_SLOW_FREQ;
          }

        return DA1470X_RCLP_FAST_FREQ;
    }
}

/****************************************************************************
 * Name: da1470x_clockconfig
 ****************************************************************************/

void da1470x_clockconfig(void)
{
  uint32_t regval;

  /* The boot ROM leaves us on RCHS at 32 MHz.  Select the requested
   * high-frequency source.
   */

#if defined(CONFIG_DA1470X_CLOCK_XTAL32M_SRC)
  if (da1470x_set_sysclk(DA1470X_SYSCLK_XTAL32M) < 0)
    {
      /* No crystal: fall back to the RC oscillator so we still boot */

      da1470x_set_sysclk(DA1470X_SYSCLK_RCHS_32);
    }
#elif defined(CONFIG_DA1470X_CLOCK_HFCLK_SRC_RCHS_64)
  da1470x_set_sysclk(DA1470X_SYSCLK_RCHS_64);
#elif defined(CONFIG_DA1470X_CLOCK_HFCLK_SRC_RCHS_96)
  da1470x_set_sysclk(DA1470X_SYSCLK_RCHS_96);
#else
  da1470x_set_sysclk(DA1470X_SYSCLK_RCHS_32);
#endif

  /* AHB and APB dividers to /1 so the core runs at the full system clock
   * (the reset value of HCLK_DIV is /4).
   */

  regval  = getreg32(DA1470X_CRG_TOP_CLK_AMBA);
  regval &= ~(CRG_TOP_CLK_AMBA_HCLK_DIV_MASK |
              CRG_TOP_CLK_AMBA_PCLK_DIV_MASK);
  putreg32(regval, DA1470X_CRG_TOP_CLK_AMBA);

  /* Low power clock */

  clk_lpclk_config();
}
