/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_crg_ctrl.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_CRG_CTRL_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_CRG_CTRL_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470x_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define DA1470X_CRG_CTRL_CLK_PDCTRL_OFFSET 0x0004 /* Clock control settings for PD_CTRL */

/* Register Addresses *******************************************************/

#define DA1470X_CRG_CTRL_CLK_PDCTRL (DA1470X_CRG_CTRL_BASE + DA1470X_CRG_CTRL_CLK_PDCTRL_OFFSET)

/* Register Bit-field Definitions *******************************************/

/* CLK_PDCTRL register */

#define CRG_CTRL_CLK_PDCTRL_EMMC_CLK_DIV_SHIFT (7)    /* Bits 7-10 */
#define CRG_CTRL_CLK_PDCTRL_EMMC_CLK_DIV_MASK (0xf << CRG_CTRL_CLK_PDCTRL_EMMC_CLK_DIV_SHIFT)
#  define CRG_CTRL_CLK_PDCTRL_EMMC_CLK_DIV(n) ((uint32_t)(n) << CRG_CTRL_CLK_PDCTRL_EMMC_CLK_DIV_SHIFT)
#define CRG_CTRL_CLK_PDCTRL_EMMC_ENABLE (1 << 11)     /* Bit 11 */
#define CRG_CTRL_CLK_PDCTRL_EMMC_INV_RX_CLK (1 << 12) /* Bit 12 */
#define CRG_CTRL_CLK_PDCTRL_EMMC_INV_TX_CLK (1 << 13) /* Bit 13 */

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_CRG_CTRL_H */
