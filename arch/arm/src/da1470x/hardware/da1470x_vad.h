/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_vad.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_VAD_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_VAD_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470x_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define DA1470X_VAD_CTRL0_OFFSET 0x0000  /* VAD Control Register 0 */
#define DA1470X_VAD_CTRL1_OFFSET 0x0004  /* VAD Control Register 1 */
#define DA1470X_VAD_CTRL2_OFFSET 0x0008  /* VAD Control Register 2 */
#define DA1470X_VAD_CTRL3_OFFSET 0x000c  /* VAD Control Register 3 */
#define DA1470X_VAD_CTRL4_OFFSET 0x0010  /* VAD Control Register 4 */
#define DA1470X_VAD_STATUS_OFFSET 0x0020 /* VAD Status Register */

/* Register Addresses *******************************************************/

#define DA1470X_VAD_CTRL0 (DA1470X_VAD_BASE + DA1470X_VAD_CTRL0_OFFSET)
#define DA1470X_VAD_CTRL1 (DA1470X_VAD_BASE + DA1470X_VAD_CTRL1_OFFSET)
#define DA1470X_VAD_CTRL2 (DA1470X_VAD_BASE + DA1470X_VAD_CTRL2_OFFSET)
#define DA1470X_VAD_CTRL3 (DA1470X_VAD_BASE + DA1470X_VAD_CTRL3_OFFSET)
#define DA1470X_VAD_CTRL4 (DA1470X_VAD_BASE + DA1470X_VAD_CTRL4_OFFSET)
#define DA1470X_VAD_STATUS (DA1470X_VAD_BASE + DA1470X_VAD_STATUS_OFFSET)

/* Register Bit-field Definitions *******************************************/

/* CTRL0 register */

#define VAD_CTRL0_PWR_LVL_SNSTVTY_SHIFT (0) /* Bits 0-2 */
#define VAD_CTRL0_PWR_LVL_SNSTVTY_MASK (0x7 << VAD_CTRL0_PWR_LVL_SNSTVTY_SHIFT)
#  define VAD_CTRL0_PWR_LVL_SNSTVTY(n) ((uint32_t)(n) << VAD_CTRL0_PWR_LVL_SNSTVTY_SHIFT)
#define VAD_CTRL0_NTRACK_SHIFT (3)          /* Bits 3-5 */
#define VAD_CTRL0_NTRACK_MASK (0x7 << VAD_CTRL0_NTRACK_SHIFT)
#  define VAD_CTRL0_NTRACK(n) ((uint32_t)(n) << VAD_CTRL0_NTRACK_SHIFT)
#define VAD_CTRL0_VTRACK_SHIFT (6)          /* Bits 6-7 */
#define VAD_CTRL0_VTRACK_MASK (0x3 << VAD_CTRL0_VTRACK_SHIFT)
#  define VAD_CTRL0_VTRACK(n) ((uint32_t)(n) << VAD_CTRL0_VTRACK_SHIFT)

/* CTRL1 register */

#define VAD_CTRL1_MINEVENT_SHIFT (0) /* Bits 0-2 */
#define VAD_CTRL1_MINEVENT_MASK (0x7 << VAD_CTRL1_MINEVENT_SHIFT)
#  define VAD_CTRL1_MINEVENT(n) ((uint32_t)(n) << VAD_CTRL1_MINEVENT_SHIFT)
#define VAD_CTRL1_MINDELAY_SHIFT (3) /* Bits 3-4 */
#define VAD_CTRL1_MINDELAY_MASK (0x3 << VAD_CTRL1_MINDELAY_SHIFT)
#  define VAD_CTRL1_MINDELAY(n) ((uint32_t)(n) << VAD_CTRL1_MINDELAY_SHIFT)

/* CTRL2 register */

#define VAD_CTRL2_NFI_DET_SHIFT (0) /* Bits 0-5 */
#define VAD_CTRL2_NFI_DET_MASK (0x3f << VAD_CTRL2_NFI_DET_SHIFT)
#  define VAD_CTRL2_NFI_DET(n) ((uint32_t)(n) << VAD_CTRL2_NFI_DET_SHIFT)

/* CTRL3 register */

#define VAD_CTRL3_MCLK_DIV_SHIFT (3) /* Bits 3-5 */
#define VAD_CTRL3_MCLK_DIV_MASK (0x7 << VAD_CTRL3_MCLK_DIV_SHIFT)
#  define VAD_CTRL3_MCLK_DIV(n) ((uint32_t)(n) << VAD_CTRL3_MCLK_DIV_SHIFT)
#define VAD_CTRL3_SLEEP (1 << 6)     /* Bit 6 */
#define VAD_CTRL3_SB (1 << 7)        /* Bit 7 */

/* CTRL4 register */

#define VAD_CTRL4_IRQ_FLAG (1 << 1) /* Bit 1 */
#define VAD_CTRL4_IRQ_MODE (1 << 2) /* Bit 2 */

/* STATUS register */

#define VAD_STATUS_NFI_RD_SHIFT (0) /* Bits 0-5 */
#define VAD_STATUS_NFI_RD_MASK (0x3f << VAD_STATUS_NFI_RD_SHIFT)
#  define VAD_STATUS_NFI_RD(n) ((uint32_t)(n) << VAD_STATUS_NFI_RD_SHIFT)

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_VAD_H */
