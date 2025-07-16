/****************************************************************************
 * arch/arm/src/da1470x/da1470x_clk.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_CLK_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_CLK_H

/**
 * \addtogroup CLOCK_TYPES
 * \{
 */

/**
 * \brief The type of the system clock
 */

typedef enum sys_clk_is_type
{
        SYS_CLK_IS_XTAL32M = 0,
        SYS_CLK_IS_RCHS,
        SYS_CLK_IS_RCLP,
        SYS_CLK_IS_PLL,
        SYS_CLK_IS_INVALID
} sys_clk_is_t;

/**
 * \brief The system clock type
 *
 * \note Must only be used with functions cm_sys_clk_init(), cm_sys_clk_set(),
 *      cm_sys_clk_request/release()
 */

typedef enum sys_clk_type
{
        SYSCLK_RCHS_32 = 0,     /*  RCHS 32MHz */
        SYSCLK_XTAL32M = 2,     /*  32MHz */
        SYSCLK_RCHS_64 = 4,     /*  RCHS 64MHz */
        SYSCLK_RCHS_96 = 6,     /*  RCHS 96MHz */
        SYSCLK_PLL160M = 10,    /*  160MHz */
        SYSCLK_BOOTER  = 11,    /*  leave clock decision up to the booter (defined by CS content) */
        SYSCLK_LP      = 255,   /*  not applicable */
} sys_clk_t;

/**
 * \brief The RCHS speed output
 *
 * \note Must only be used with functions hw_clk_set/get_rchs_mode()
 */

typedef enum rchs_speed_type
{
        RCHS_32 = 0,    /* 32MHz */
        RCHS_96,        /* 96MHz */
        RCHS_64,        /* 64MHz */
} rchs_speed_t;

/**
 * \brief The CPU clock type (speed)
 */

typedef enum cpu_clk_type
{
        CPUCLK_2M   = 2,    /* 2 MHz */
        CPUCLK_4M   = 4,    /* 4 MHz */
        CPUCLK_6M   = 6,    /* 6 MHz */
        CPUCLK_8M   = 8,    /* 8 MHz */
        CPUCLK_10M  = 10,   /* 10 MHz */
        CPUCLK_12M  = 12,   /* 12 MHz */
        CPUCLK_16M  = 16,   /* 16 MHz */
        CPUCLK_20M  = 20,   /* 20 MHz */
        CPUCLK_24M  = 24,   /* 24 MHz */
        CPUCLK_32M  = 32,   /* 32 MHz */
        CPUCLK_40M  = 40,   /* 40 MHz */
        CPUCLK_48M  = 48,   /* 48 MHz */
        CPUCLK_64M  = 64,   /* 64 MHz */
        CPUCLK_80M  = 80,   /* 80 MHz */
        CPUCLK_96M  = 96,   /* 96 MHz */
        CPUCLK_160M = 160   /* 160 MHz */
} cpu_clk_t;


/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/**
 * @brief Enable XTALM clock
 */
void cm_enable_xtalm(void);

/**
 * @brief Initialize system clock
 *
 * @param type System clock type
 */
static inline void cm_sys_clk_init(sys_clk_t type);

/**
 * @brief Switch system clock to PLL
 */
static inline void switch_to_pll(void);

/**
 * @brief Switch system clock to RCHS
 *
 * @param mode RCHS speed mode
 */
static inline void switch_to_rchs(rchs_speed_t mode);

/**
 * @brief Set system clock
 *
 * @param mode System clock mode
 */
static inline void hw_clk_set_sysclk(sys_clk_is_t mode);

/**
 * @brief Get the current system clock
 *
 * @return The current system clock type
 */
sys_clk_is_t hw_clk_get_sysclk(void);

/**
 * @brief Check if XTALM is started
 *
 * @return true if XTALM is started, false otherwise
 */
static inline bool hw_clk_is_xtalm_started(void);

/**
 * @brief Check if a system clock is enabled
 *
 * @param clk The clock to check
 * @return true if the clock is enabled, false otherwise
 */
static inline bool hw_clk_is_enabled_sysclk(sys_clk_is_t clk);

/**
 * @brief Check if XTALM clock is enabled
 *
 * @return true if XTALM clock is enabled, false otherwise
 */
static inline bool hw_clk_check_xtalm_status(void);

/**
 * @brief Check if RCHS clock is enabled
 *
 * @return true if RCHS clock is enabled, false otherwise
 */
static inline bool hw_clk_check_rchs_status(void);

/**
 * @brief Check if PLL clock is enabled
 *
 * @return true if PLL clock is enabled, false otherwise
 */
static inline bool hw_clk_check_pll_status(void);

/**
 * @brief Switch system clock to XTAL32M
 */
static void switch_to_xtal32m(void);

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_CLK */