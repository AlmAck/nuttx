/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_crg_snc.h
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

/* Generated from the Renesas CMSIS device header DA1470x-00.h (SDK
 * 10.2.6.49).  Do not edit by hand; regenerate with tools/genhdr.py.
 */

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_CRG_SNC_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_CRG_SNC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470x_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define DA1470X_CRG_SNC_CLK_SNC_OFFSET 0x0004       /* Peripheral divider register */
#define DA1470X_CRG_SNC_SET_CLK_SNC_OFFSET 0x0008   /* Peripheral divider register SET register */
#define DA1470X_CRG_SNC_RESET_CLK_SNC_OFFSET 0x000c /* Peripheral divider register RESET regist */

/* Register Addresses *******************************************************/

#define DA1470X_CRG_SNC_CLK_SNC (DA1470X_CRG_SNC_BASE + DA1470X_CRG_SNC_CLK_SNC_OFFSET)
#define DA1470X_CRG_SNC_SET_CLK_SNC (DA1470X_CRG_SNC_BASE + DA1470X_CRG_SNC_SET_CLK_SNC_OFFSET)
#define DA1470X_CRG_SNC_RESET_CLK_SNC (DA1470X_CRG_SNC_BASE + DA1470X_CRG_SNC_RESET_CLK_SNC_OFFSET)

/* Register Bit-field Definitions *******************************************/

/* CLK_SNC register */

#define CRG_SNC_CLK_SNC_UART_ENABLE (1 << 0)   /* Bit 0 */
#define CRG_SNC_CLK_SNC_UART_CLK_SEL (1 << 1)  /* Bit 1 */
#define CRG_SNC_CLK_SNC_UART2_ENABLE (1 << 2)  /* Bit 2 */
#define CRG_SNC_CLK_SNC_UART2_CLK_SEL (1 << 3) /* Bit 3 */
#define CRG_SNC_CLK_SNC_UART3_ENABLE (1 << 4)  /* Bit 4 */
#define CRG_SNC_CLK_SNC_UART3_CLK_SEL (1 << 5) /* Bit 5 */
#define CRG_SNC_CLK_SNC_SPI_ENABLE (1 << 6)    /* Bit 6 */
#define CRG_SNC_CLK_SNC_SPI_CLK_SEL (1 << 7)   /* Bit 7 */
#define CRG_SNC_CLK_SNC_SPI2_ENABLE (1 << 8)   /* Bit 8 */
#define CRG_SNC_CLK_SNC_SPI2_CLK_SEL (1 << 9)  /* Bit 9 */
#define CRG_SNC_CLK_SNC_I2C_ENABLE (1 << 10)   /* Bit 10 */
#define CRG_SNC_CLK_SNC_I2C_CLK_SEL (1 << 11)  /* Bit 11 */
#define CRG_SNC_CLK_SNC_I2C2_ENABLE (1 << 12)  /* Bit 12 */
#define CRG_SNC_CLK_SNC_I2C2_CLK_SEL (1 << 13) /* Bit 13 */
#define CRG_SNC_CLK_SNC_I2C3_ENABLE (1 << 14)  /* Bit 14 */
#define CRG_SNC_CLK_SNC_I2C3_CLK_SEL (1 << 15) /* Bit 15 */
#define CRG_SNC_CLK_SNC_I3C_ENABLE (1 << 16)   /* Bit 16 */
#define CRG_SNC_CLK_SNC_I3C_CLK_SEL (1 << 17)  /* Bit 17 */

/* SET_CLK_SNC register */

#define CRG_SNC_SET_CLK_SNC_UART_ENABLE (1 << 0)   /* Bit 0 */
#define CRG_SNC_SET_CLK_SNC_UART_CLK_SEL (1 << 1)  /* Bit 1 */
#define CRG_SNC_SET_CLK_SNC_UART2_ENABLE (1 << 2)  /* Bit 2 */
#define CRG_SNC_SET_CLK_SNC_UART2_CLK_SEL (1 << 3) /* Bit 3 */
#define CRG_SNC_SET_CLK_SNC_UART3_ENABLE (1 << 4)  /* Bit 4 */
#define CRG_SNC_SET_CLK_SNC_UART3_CLK_SEL (1 << 5) /* Bit 5 */
#define CRG_SNC_SET_CLK_SNC_SPI_ENABLE (1 << 6)    /* Bit 6 */
#define CRG_SNC_SET_CLK_SNC_SPI_CLK_SEL (1 << 7)   /* Bit 7 */
#define CRG_SNC_SET_CLK_SNC_SPI2_ENABLE (1 << 8)   /* Bit 8 */
#define CRG_SNC_SET_CLK_SNC_SPI2_CLK_SEL (1 << 9)  /* Bit 9 */
#define CRG_SNC_SET_CLK_SNC_I2C_ENABLE (1 << 10)   /* Bit 10 */
#define CRG_SNC_SET_CLK_SNC_I2C_CLK_SEL (1 << 11)  /* Bit 11 */
#define CRG_SNC_SET_CLK_SNC_I2C2_ENABLE (1 << 12)  /* Bit 12 */
#define CRG_SNC_SET_CLK_SNC_I2C2_CLK_SEL (1 << 13) /* Bit 13 */
#define CRG_SNC_SET_CLK_SNC_I2C3_ENABLE (1 << 14)  /* Bit 14 */
#define CRG_SNC_SET_CLK_SNC_I2C3_CLK_SEL (1 << 15) /* Bit 15 */
#define CRG_SNC_SET_CLK_SNC_I3C_ENABLE (1 << 16)   /* Bit 16 */
#define CRG_SNC_SET_CLK_SNC_I3C_CLK_SEL (1 << 17)  /* Bit 17 */

/* RESET_CLK_SNC register */

#define CRG_SNC_RESET_CLK_SNC_UART_ENABLE (1 << 0)   /* Bit 0 */
#define CRG_SNC_RESET_CLK_SNC_UART_CLK_SEL (1 << 1)  /* Bit 1 */
#define CRG_SNC_RESET_CLK_SNC_UART2_ENABLE (1 << 2)  /* Bit 2 */
#define CRG_SNC_RESET_CLK_SNC_UART2_CLK_SEL (1 << 3) /* Bit 3 */
#define CRG_SNC_RESET_CLK_SNC_UART3_ENABLE (1 << 4)  /* Bit 4 */
#define CRG_SNC_RESET_CLK_SNC_UART3_CLK_SEL (1 << 5) /* Bit 5 */
#define CRG_SNC_RESET_CLK_SNC_SPI_ENABLE (1 << 6)    /* Bit 6 */
#define CRG_SNC_RESET_CLK_SNC_SPI_CLK_SEL (1 << 7)   /* Bit 7 */
#define CRG_SNC_RESET_CLK_SNC_SPI2_ENABLE (1 << 8)   /* Bit 8 */
#define CRG_SNC_RESET_CLK_SNC_SPI2_CLK_SEL (1 << 9)  /* Bit 9 */
#define CRG_SNC_RESET_CLK_SNC_I2C_ENABLE (1 << 10)   /* Bit 10 */
#define CRG_SNC_RESET_CLK_SNC_I2C_CLK_SEL (1 << 11)  /* Bit 11 */
#define CRG_SNC_RESET_CLK_SNC_I2C2_ENABLE (1 << 12)  /* Bit 12 */
#define CRG_SNC_RESET_CLK_SNC_I2C2_CLK_SEL (1 << 13) /* Bit 13 */
#define CRG_SNC_RESET_CLK_SNC_I2C3_ENABLE (1 << 14)  /* Bit 14 */
#define CRG_SNC_RESET_CLK_SNC_I2C3_CLK_SEL (1 << 15) /* Bit 15 */
#define CRG_SNC_RESET_CLK_SNC_I3C_ENABLE (1 << 16)   /* Bit 16 */
#define CRG_SNC_RESET_CLK_SNC_I3C_CLK_SEL (1 << 17)  /* Bit 17 */

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_CRG_SNC_H */
