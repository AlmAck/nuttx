/****************************************************************************
 * ./tmp/da1470_crg_sys.h
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

#ifndef __DA1470_CRG_SYS_H
#define __DA1470_CRG_SYS_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register offsets *********************************************************/

#define DA1470_CRG_SYS_BATCHECK_OFFSET        0x0010 /* No description available. */
#define DA1470_CRG_SYS_CLK_SYS_OFFSET         0x0000 /* Peripheral divider register */
#define DA1470_CRG_SYS_RESET_CLK_SYS_OFFSET   0x000C /* Peripheral divider RESET register */
#define DA1470_CRG_SYS_SET_CLK_SYS_OFFSET     0x0008 /* Peripheral divider SET register */

/* Register addresses *******************************************************/

#define DA1470_CRG_SYS_BASE                   0x50040400 /* CRG_SYS registers */

#define DA1470_CRG_SYS_BATCHECK               (DA1470_CRG_SYS_BASE + DA1470_CRG_SYS_BATCHECK_OFFSET) /* No description available. */
#define DA1470_CRG_SYS_CLK_SYS                (DA1470_CRG_SYS_BASE + DA1470_CRG_SYS_CLK_SYS_OFFSET) /* Peripheral divider register */
#define DA1470_CRG_SYS_RESET_CLK_SYS          (DA1470_CRG_SYS_BASE + DA1470_CRG_SYS_RESET_CLK_SYS_OFFSET) /* Peripheral divider RESET register */
#define DA1470_CRG_SYS_SET_CLK_SYS            (DA1470_CRG_SYS_BASE + DA1470_CRG_SYS_SET_CLK_SYS_OFFSET) /* Peripheral divider SET register */

/* Register bit definitions *************************************************/


/* BATCHECK Register */

#define CRG_SYS_BATCHECK_LOAD_ENABLE          (1U << 7) /* Enable a current load on the battery. */
#define CRG_SYS_BATCHECK_ILOAD_MASK           (0x7 << 4) /* Set the current load to (ILOAD+1) mA. */
#define CRG_SYS_BATCHECK_ILOAD_POS            (4) /* Set the current load to (ILOAD+1) mA. */
#define CRG_SYS_BATCHECK_TRIM_MASK            (0xF << 0) /* Trim the current load with steps of 2.7% from -19.1% to +19.1%. 0: +0.0% , 8: -0 */
#define CRG_SYS_BATCHECK_TRIM_POS             (0) /* Trim the current load with steps of 2.7% from -19.1% to +19.1%. 0: +0.0% , 8: -0 */

/* CLK_SYS Register */

#define CRG_SYS_SPI3_CLK_SEL                  (1U << 7) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SYS_SPI3_ENABLE                   (1U << 6) /* Enables the clock */
#define CRG_SYS_CLK_CHG_EN                    (1U << 5) /* Enables the clocks for the charger FSM block */
#define CRG_SYS_LCD_RESET_REQ                 (1U << 4) /* Generates a SW reset towards the LCD controller. */
#define CRG_SYS_LCD_DPHYCLK_SEL               (1U << 3) /* This bitfield selects the clock source for the LCD controller. This bit may not  */
#define CRG_SYS_LCD_CLK_SEL                   (1U << 1) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SYS_LCD_ENABLE                    (1U << 0) /* Enables the clock */

/* RESET_CLK_SYS Register */

#define CRG_SYS_SPI3_CLK_SEL                  (1U << 7) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SYS_SPI3_ENABLE                   (1U << 6) /* Enables the clock */
#define CRG_SYS_CLK_CHG_EN                    (1U << 5) /* Enables the clocks for the charger FSM block */
#define CRG_SYS_LCD_RESET_REQ                 (1U << 4) /* Generates a SW reset towards the LCD controller. */
#define CRG_SYS_LCD_DPHYCLK_SEL               (1U << 3) /* This bitfield selects the clock source for the LCD controller. This bit may not  */
#define CRG_SYS_LCD_CLK_SEL                   (1U << 1) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SYS_LCD_ENABLE                    (1U << 0) /* Enables the clock */

/* SET_CLK_SYS Register */

#define CRG_SYS_SPI3_CLK_SEL                  (1U << 7) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SYS_SPI3_ENABLE                   (1U << 6) /* Enables the clock */
#define CRG_SYS_CLK_CHG_EN                    (1U << 5) /* Enables the clocks for the charger FSM block */
#define CRG_SYS_LCD_RESET_REQ                 (1U << 4) /* Generates a SW reset towards the LCD controller. */
#define CRG_SYS_LCD_DPHYCLK_SEL               (1U << 3) /* This bitfield selects the clock source for the LCD controller. This bit may not  */
#define CRG_SYS_LCD_CLK_SEL                   (1U << 1) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SYS_LCD_ENABLE                    (1U << 0) /* Enables the clock */

#endif /* __DA1470_CRG_SYS_H */
