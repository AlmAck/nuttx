/****************************************************************************
 * ./tmp/da1470_crg_snc.h
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

#ifndef __DA1470_CRG_SNC_H
#define __DA1470_CRG_SNC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register offsets *********************************************************/

#define DA1470_CRG_SNC_CLK_SNC_OFFSET         0x0004 /* Peripheral divider register */
#define DA1470_CRG_SNC_RESET_CLK_SNC_OFFSET   0x000C /* Peripheral divider register RESET register. Reads back 0x0000 */
#define DA1470_CRG_SNC_SET_CLK_SNC_OFFSET     0x0008 /* Peripheral divider register SET register. Reads back 0x0000 */

/* Register addresses *******************************************************/

#define DA1470_CRG_SNC_BASE                   0x50020900 /* CRG_SNC registers */

#define DA1470_CRG_SNC_CLK_SNC                (DA1470_CRG_SNC_BASE + DA1470_CRG_SNC_CLK_SNC_OFFSET) /* Peripheral divider register */
#define DA1470_CRG_SNC_RESET_CLK_SNC          (DA1470_CRG_SNC_BASE + DA1470_CRG_SNC_RESET_CLK_SNC_OFFSET) /* Peripheral divider register RESET register. Reads back 0x0000 */
#define DA1470_CRG_SNC_SET_CLK_SNC            (DA1470_CRG_SNC_BASE + DA1470_CRG_SNC_SET_CLK_SNC_OFFSET) /* Peripheral divider register SET register. Reads back 0x0000 */

/* Register bit definitions *************************************************/


/* CLK_SNC Register */

#define CRG_SNC_I3C_CLK_SEL                   (1U << 17) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SNC_I3C_ENABLE                    (1U << 16) /* Enables the clock */
#define CRG_SNC_I2C3_CLK_SEL                  (1U << 15) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SNC_I2C3_ENABLE                   (1U << 14) /* Enables the clock */
#define CRG_SNC_I2C2_CLK_SEL                  (1U << 13) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SNC_I2C2_ENABLE                   (1U << 12) /* Enables the clock */
#define CRG_SNC_I2C_CLK_SEL                   (1U << 11) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SNC_I2C_ENABLE                    (1U << 10) /* Enables the clock */
#define CRG_SNC_SPI2_CLK_SEL                  (1U << 9) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SNC_SPI2_ENABLE                   (1U << 8) /* Enables the clock */
#define CRG_SNC_SPI_CLK_SEL                   (1U << 7) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SNC_SPI_ENABLE                    (1U << 6) /* Enables the clock */
#define CRG_SNC_UART3_CLK_SEL                 (1U << 5) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SNC_UART3_ENABLE                  (1U << 4) /* Enables the clock */
#define CRG_SNC_UART2_CLK_SEL                 (1U << 3) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SNC_UART2_ENABLE                  (1U << 2) /* Enables the clock */
#define CRG_SNC_UART_CLK_SEL                  (1U << 1) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SNC_UART_ENABLE                   (1U << 0) /* Enables the clock */

/* RESET_CLK_SNC Register */

#define CRG_SNC_I3C_CLK_SEL                   (1U << 17) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SNC_I3C_ENABLE                    (1U << 16) /* Enables the clock */
#define CRG_SNC_I2C3_CLK_SEL                  (1U << 15) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SNC_I2C3_ENABLE                   (1U << 14) /* Enables the clock */
#define CRG_SNC_I2C2_CLK_SEL                  (1U << 13) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SNC_I2C2_ENABLE                   (1U << 12) /* Enables the clock */
#define CRG_SNC_I2C_CLK_SEL                   (1U << 11) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SNC_I2C_ENABLE                    (1U << 10) /* Enables the clock */
#define CRG_SNC_SPI2_CLK_SEL                  (1U << 9) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SNC_SPI2_ENABLE                   (1U << 8) /* Enables the clock */
#define CRG_SNC_SPI_CLK_SEL                   (1U << 7) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SNC_SPI_ENABLE                    (1U << 6) /* Enables the clock */
#define CRG_SNC_UART3_CLK_SEL                 (1U << 5) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SNC_UART3_ENABLE                  (1U << 4) /* Enables the clock */
#define CRG_SNC_UART2_CLK_SEL                 (1U << 3) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SNC_UART2_ENABLE                  (1U << 2) /* Enables the clock */
#define CRG_SNC_UART_CLK_SEL                  (1U << 1) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SNC_UART_ENABLE                   (1U << 0) /* Enables the clock */

/* SET_CLK_SNC Register */

#define CRG_SNC_I3C_CLK_SEL                   (1U << 17) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SNC_I3C_ENABLE                    (1U << 16) /* Enables the clock */
#define CRG_SNC_I2C3_CLK_SEL                  (1U << 15) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SNC_I2C3_ENABLE                   (1U << 14) /* Enables the clock */
#define CRG_SNC_I2C2_CLK_SEL                  (1U << 13) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SNC_I2C2_ENABLE                   (1U << 12) /* Enables the clock */
#define CRG_SNC_I2C_CLK_SEL                   (1U << 11) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SNC_I2C_ENABLE                    (1U << 10) /* Enables the clock */
#define CRG_SNC_SPI2_CLK_SEL                  (1U << 9) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SNC_SPI2_ENABLE                   (1U << 8) /* Enables the clock */
#define CRG_SNC_SPI_CLK_SEL                   (1U << 7) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SNC_SPI_ENABLE                    (1U << 6) /* Enables the clock */
#define CRG_SNC_UART3_CLK_SEL                 (1U << 5) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SNC_UART3_ENABLE                  (1U << 4) /* Enables the clock */
#define CRG_SNC_UART2_CLK_SEL                 (1U << 3) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SNC_UART2_ENABLE                  (1U << 2) /* Enables the clock */
#define CRG_SNC_UART_CLK_SEL                  (1U << 1) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_SNC_UART_ENABLE                   (1U << 0) /* Enables the clock */

#endif /* __DA1470_CRG_SNC_H */
