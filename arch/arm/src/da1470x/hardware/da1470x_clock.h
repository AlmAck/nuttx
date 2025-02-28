/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_clock.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_CLOCK_H
#define __ARCH_ARM_SRC_DA1470X_CLOCK_H

/* CRG_TOP - CRG_TOP registers */
#include "da1470x_crg_top.h"
#include "da1470x_crg_xtal.h"


/* Register definitions */


/* Register bit definitions *************************************************/


/* Definitions for CLK_CMAC_SWITCH_REG */
#define CMAC_RUNNING_ON_XTAL   2
#define CMAC_RUNNING_ON_XTAL_MASK    (1 << CMAC_RUNNING_ON_XTAL)

#define CMAC_RUNNING_ON_DIVN   1
#define CMAC_RUNNING_ON_DIVN_MASK    (1 << CMAC_RUNNING_ON_DIVN)

#define CMAC_CLK_SEL           0
#define CMAC_CLK_SEL_MASK            (1 << CMAC_CLK_SEL)

/* Definitions for CLK_CTRL_REG */
#define RUNNING_AT_PLL                15
#define RUNNING_AT_PLL_MASK                 (1 << RUNNING_AT_PLL)

#define RUNNING_AT_XTAL32M            14
#define RUNNING_AT_XTAL32M_MASK             (1 << RUNNING_AT_XTAL32M)

#define RUNNING_AT_RCHS               13
#define RUNNING_AT_RCHS_MASK                (1 << RUNNING_AT_RCHS)

#define RUNNING_AT_RCLP               12
#define RUNNING_AT_RCLP_MASK                (1 << RUNNING_AT_RCLP)

#define VAD_CLK_SEL                   6
#define VAD_CLK_SEL_MASK                    (1 << VAD_CLK_SEL)

#define USB_CLK_SRC                   4
#define USB_CLK_SRC_MASK                    (1 << USB_CLK_SRC)

#define LP_CLK_SEL                    2
#define LP_CLK_SEL_MASK                     (3 << LP_CLK_SEL)  // Note: 2 bits for selection

#define SYS_CLK_SEL                   0
#define SYS_CLK_SEL_MASK                    (3 << SYS_CLK_SEL)  // Note: 2 bits for selection


/**
 * Enum for AHB clock divider values
 */

typedef enum ahb_div_e
{
  AHB_DIV1 = 0,      /**< Divide by 1 */
  AHB_DIV2,          /**< Divide by 2 */
  AHB_DIV4,          /**< Divide by 4 */
  AHB_DIV8,          /**< Divide by 8 */
  AHB_DIV16,         /**< Divide by 16 */
  AHB_INVALID        /**< Invalid divider */
} ahb_div_t;



/**
 * The AMBA Peripheral Bus (APB) clock divider
 */

typedef enum apb_div_e
{
  APB_DIV1 = 0,       /**< Divide by 1 */
  APB_DIV2,           /**< Divide by 2 */
  APB_DIV4,           /**< Divide by 4 */
  APB_DIV8,           /**< Divide by 8 */
  APB_DIV16,          /**< Divide by 16 - only for Slow-APB interface clock */
  APB_INVALID         /**< Invalid divider */
} apb_div_t;


/**
 * @brief The type of the system clock
 */

typedef enum sys_clk_is_e
{
  SYS_CLK_IS_XTAL32M = 0,   /**< XTAL32M clock */
  SYS_CLK_IS_RCHS,          /**< RCHS clock */
  SYS_CLK_IS_RCLP,          /**< RCLP clock */
  SYS_CLK_IS_PLL,           /**< PLL clock */
  SYS_CLK_IS_INVALID        /**< Invalid clock */
} sys_clk_is_t;

/* CLK_AMBA_REG Register */

#define CLK_AMBA_REG_PCLK_DIV             (0)       /* Bits 0-1: LCLK_DIV clock source */
#define CLK_AMBA_REG_PCLK_DIV_MASK        (3 << CLK_AMBA_REG_PCLK_DIV)
#  define CLK_AMBA_REG_PCLK_DIV1          (0 << CLK_AMBA_REG_PCLK_DIV)
#  define CLK_AMBA_REG_PCLK_DIV2          (1 << CLK_AMBA_REG_PCLK_DIV)
#  define CLK_AMBA_REG_PCLK_DIV4          (2 << CLK_AMBA_REG_PCLK_DIV)
#  define CLK_AMBA_REG_PCLK_DIV8          (3 << CLK_AMBA_REG_PCLK_DIV)


#define CLK_AMBA_REG_HCLK_DIV             (0)       /* Bits 0-3: HCLK_DIV clock source */
#define CLK_AMBA_REG_HCLK_DIV_MASK        (7 << CLK_AMBA_REG_PCLK_DIV)
#  define CLK_AMBA_REG_HCLK_DIV1          (0 << CLK_AMBA_REG_PCLK_DIV)
#  define CLK_AMBA_REG_HCLK_DIV2          (1 << CLK_AMBA_REG_PCLK_DIV)
#  define CLK_AMBA_REG_HCLK_DIV4          (2 << CLK_AMBA_REG_PCLK_DIV)
#  define CLK_AMBA_REG_HCLK_DIV8          (3 << CLK_AMBA_REG_PCLK_DIV)
#  define CLK_AMBA_REG_HCLK_DIV16         (4 << CLK_AMBA_REG_PCLK_DIV)

#endif /* __ARCH_ARM_SRC_DA1470X_CLOCK_H */

