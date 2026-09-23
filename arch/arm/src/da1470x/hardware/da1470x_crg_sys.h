/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_crg_sys.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_CRG_SYS_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_CRG_SYS_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470x_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define DA1470X_CRG_SYS_CLK_SYS_OFFSET 0x0000       /* Peripheral divider register */
#define DA1470X_CRG_SYS_SET_CLK_SYS_OFFSET 0x0008   /* Peripheral divider SET register */
#define DA1470X_CRG_SYS_RESET_CLK_SYS_OFFSET 0x000c /* Peripheral divider RESET register */
#define DA1470X_CRG_SYS_BATCHECK_OFFSET 0x0010      /* BATCHECK_REG */

/* Register Addresses *******************************************************/

#define DA1470X_CRG_SYS_CLK_SYS (DA1470X_CRG_SYS_BASE + DA1470X_CRG_SYS_CLK_SYS_OFFSET)
#define DA1470X_CRG_SYS_SET_CLK_SYS (DA1470X_CRG_SYS_BASE + DA1470X_CRG_SYS_SET_CLK_SYS_OFFSET)
#define DA1470X_CRG_SYS_RESET_CLK_SYS (DA1470X_CRG_SYS_BASE + DA1470X_CRG_SYS_RESET_CLK_SYS_OFFSET)
#define DA1470X_CRG_SYS_BATCHECK (DA1470X_CRG_SYS_BASE + DA1470X_CRG_SYS_BATCHECK_OFFSET)

/* Register Bit-field Definitions *******************************************/

/* CLK_SYS register */

#define CRG_SYS_CLK_SYS_LCD_ENABLE (1 << 0)    /* Bit 0 */
#define CRG_SYS_CLK_SYS_LCD_CLK_SEL (1 << 1)   /* Bit 1 */
#define CRG_SYS_CLK_SYS_LCD_RESET_REQ (1 << 4) /* Bit 4 */
#define CRG_SYS_CLK_SYS_CLK_CHG_EN (1 << 5)    /* Bit 5 */
#define CRG_SYS_CLK_SYS_SPI3_ENABLE (1 << 6)   /* Bit 6 */
#define CRG_SYS_CLK_SYS_SPI3_CLK_SEL (1 << 7)  /* Bit 7 */

/* SET_CLK_SYS register */

#define CRG_SYS_SET_CLK_SYS_LCD_ENABLE (1 << 0)    /* Bit 0 */
#define CRG_SYS_SET_CLK_SYS_LCD_CLK_SEL (1 << 1)   /* Bit 1 */
#define CRG_SYS_SET_CLK_SYS_LCD_RESET_REQ (1 << 4) /* Bit 4 */
#define CRG_SYS_SET_CLK_SYS_CLK_CHG_EN (1 << 5)    /* Bit 5 */
#define CRG_SYS_SET_CLK_SYS_SPI3_ENABLE (1 << 6)   /* Bit 6 */
#define CRG_SYS_SET_CLK_SYS_SPI3_CLK_SEL (1 << 7)  /* Bit 7 */

/* RESET_CLK_SYS register */

#define CRG_SYS_RESET_CLK_SYS_LCD_ENABLE (1 << 0)    /* Bit 0 */
#define CRG_SYS_RESET_CLK_SYS_LCD_CLK_SEL (1 << 1)   /* Bit 1 */
#define CRG_SYS_RESET_CLK_SYS_LCD_RESET_REQ (1 << 4) /* Bit 4 */
#define CRG_SYS_RESET_CLK_SYS_CLK_CHG_EN (1 << 5)    /* Bit 5 */
#define CRG_SYS_RESET_CLK_SYS_SPI3_ENABLE (1 << 6)   /* Bit 6 */
#define CRG_SYS_RESET_CLK_SYS_SPI3_CLK_SEL (1 << 7)  /* Bit 7 */

/* BATCHECK register */

#define CRG_SYS_BATCHECK_BATCHECK_TRIM_SHIFT (0)       /* Bits 0-3 */
#define CRG_SYS_BATCHECK_BATCHECK_TRIM_MASK (0xf << CRG_SYS_BATCHECK_BATCHECK_TRIM_SHIFT)
#  define CRG_SYS_BATCHECK_BATCHECK_TRIM(n) ((uint32_t)(n) << CRG_SYS_BATCHECK_BATCHECK_TRIM_SHIFT)
#define CRG_SYS_BATCHECK_BATCHECK_ILOAD_SHIFT (4)      /* Bits 4-6 */
#define CRG_SYS_BATCHECK_BATCHECK_ILOAD_MASK (0x7 << CRG_SYS_BATCHECK_BATCHECK_ILOAD_SHIFT)
#  define CRG_SYS_BATCHECK_BATCHECK_ILOAD(n) ((uint32_t)(n) << CRG_SYS_BATCHECK_BATCHECK_ILOAD_SHIFT)
#define CRG_SYS_BATCHECK_BATCHECK_LOAD_ENABLE (1 << 7) /* Bit 7 */

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_CRG_SYS_H */
