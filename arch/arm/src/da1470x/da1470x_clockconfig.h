/****************************************************************************
 * arch/arm/src/da1470x/da1470x_clockconfig.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_CLOCKCONFIG_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_CLOCKCONFIG_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdbool.h>
#include <stdint.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* DIVN is the fixed 32 MHz clock feeding UART/SPI/I2C/LCDC/timers.  It is
 * derived from whichever system clock is running (XTAL32M directly,
 * RCHS/2 or /3, PLL/5) and is therefore constant.
 */

#define DA1470X_DIVN_CLK_FREQ     32000000
#define DA1470X_XTAL32M_FREQ      32000000
#define DA1470X_RCHS_32_FREQ      32000000
#define DA1470X_RCHS_64_FREQ      64000000
#define DA1470X_RCHS_96_FREQ      96000000
#define DA1470X_PLL160_FREQ       160000000
#define DA1470X_RCLP_SLOW_FREQ    32000
#define DA1470X_RCLP_FAST_FREQ    512000
#define DA1470X_XTAL32K_FREQ      32768

/* Boot-time system clock frequency as selected by Kconfig */

#if defined(CONFIG_DA1470X_CLOCK_PLL160_SRC)
#  define DA1470X_SYSCLK_FREQ     DA1470X_PLL160_FREQ
#elif defined(CONFIG_DA1470X_CLOCK_XTAL32M_SRC)
#  define DA1470X_SYSCLK_FREQ     DA1470X_XTAL32M_FREQ
#elif defined(CONFIG_DA1470X_CLOCK_HFCLK_SRC_RCHS_64)
#  define DA1470X_SYSCLK_FREQ     DA1470X_RCHS_64_FREQ
#elif defined(CONFIG_DA1470X_CLOCK_HFCLK_SRC_RCHS_96)
#  define DA1470X_SYSCLK_FREQ     DA1470X_RCHS_96_FREQ
#else
#  define DA1470X_SYSCLK_FREQ     DA1470X_RCHS_32_FREQ
#endif

/****************************************************************************
 * Public Types
 ****************************************************************************/

enum da1470x_sysclk_e
{
  DA1470X_SYSCLK_RCHS_32 = 0,   /* Internal RC, 32 MHz */
  DA1470X_SYSCLK_RCHS_64,       /* Internal RC, 64 MHz */
  DA1470X_SYSCLK_RCHS_96,       /* Internal RC, 96 MHz */
  DA1470X_SYSCLK_XTAL32M,       /* External crystal, 32 MHz */
  DA1470X_SYSCLK_PLL160,        /* PLL from the crystal, 160 MHz */
  DA1470X_SYSCLK_RCLP           /* Low power RC (sleep only) */
};

enum da1470x_lpclk_e
{
  DA1470X_LPCLK_RCLP = 0,       /* Internal low power RC */
  DA1470X_LPCLK_RCX,            /* Internal RCX */
  DA1470X_LPCLK_XTAL32K,        /* External 32.768 kHz crystal */
  DA1470X_LPCLK_EXTERNAL        /* External clock on GPIO */
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_clockconfig
 *
 * Description:
 *   Establish the boot clock configuration selected by Kconfig: enable the
 *   requested high-frequency source, switch the system clock to it, set
 *   the AMBA dividers to /1 and start the low-power clock.
 *
 ****************************************************************************/

void da1470x_clockconfig(void);

/****************************************************************************
 * Name: da1470x_set_sysclk
 *
 * Description:
 *   Switch the system clock to the requested source, enabling it first if
 *   needed.  Returns OK or a negated errno.
 *
 ****************************************************************************/

int da1470x_set_sysclk(enum da1470x_sysclk_e clk);

/****************************************************************************
 * Name: da1470x_get_sysclk
 *
 * Description:
 *   Return the current system (CPU) clock frequency in Hz.
 *
 ****************************************************************************/

uint32_t da1470x_get_sysclk(void);

/****************************************************************************
 * Name: da1470x_get_sysclk_src
 *
 * Description:
 *   Return the currently selected system clock source.
 *
 ****************************************************************************/

enum da1470x_sysclk_e da1470x_get_sysclk_src(void);

/****************************************************************************
 * Name: da1470x_get_divn_clk
 *
 * Description:
 *   Return the DIVN peripheral clock frequency in Hz (always 32 MHz).
 *
 ****************************************************************************/

uint32_t da1470x_get_divn_clk(void);

/****************************************************************************
 * Name: da1470x_get_lpclk
 *
 * Description:
 *   Return the nominal low-power clock frequency in Hz.
 *
 ****************************************************************************/

uint32_t da1470x_get_lpclk(void);

/****************************************************************************
 * Name: da1470x_xtal32m_enable / da1470x_xtal32m_disable
 *
 * Description:
 *   Start (and wait for) or stop the 32 MHz crystal oscillator.
 *
 ****************************************************************************/

int  da1470x_xtal32m_enable(void);
void da1470x_xtal32m_disable(void);

/****************************************************************************
 * Name: da1470x_rchs_disable
 *
 * Description:
 *   Stop the high-speed RC oscillator.  Only allowed when the system clock
 *   runs from XTAL32M.
 *
 ****************************************************************************/

void da1470x_rchs_disable(void);

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_CLOCKCONFIG_H */
