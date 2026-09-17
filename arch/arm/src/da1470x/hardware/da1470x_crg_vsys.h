/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_crg_vsys.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_CRG_VSYS_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_CRG_VSYS_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470x_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define DA1470X_CRG_VSYS_VSYS_GEN_CTRL_OFFSET 0x0000       /* VSYS_GEN_CTRL_REG */
#define DA1470X_CRG_VSYS_VSYS_GEN_IRQ_STATUS_OFFSET 0x0004 /* VSYS_GEN_IRQ_STATUS_REG */
#define DA1470X_CRG_VSYS_VSYS_GEN_IRQ_CLEAR_OFFSET 0x0008  /* VSYS_GEN_IRQ_CLEAR_REG */
#define DA1470X_CRG_VSYS_VSYS_GEN_IRQ_MASK_OFFSET 0x000c   /* VSYS_GEN_IRQ_MASK_REG */

/* Register Addresses *******************************************************/

#define DA1470X_CRG_VSYS_VSYS_GEN_CTRL (DA1470X_CRG_VSYS_BASE + DA1470X_CRG_VSYS_VSYS_GEN_CTRL_OFFSET)
#define DA1470X_CRG_VSYS_VSYS_GEN_IRQ_STATUS (DA1470X_CRG_VSYS_BASE + DA1470X_CRG_VSYS_VSYS_GEN_IRQ_STATUS_OFFSET)
#define DA1470X_CRG_VSYS_VSYS_GEN_IRQ_CLEAR (DA1470X_CRG_VSYS_BASE + DA1470X_CRG_VSYS_VSYS_GEN_IRQ_CLEAR_OFFSET)
#define DA1470X_CRG_VSYS_VSYS_GEN_IRQ_MASK (DA1470X_CRG_VSYS_BASE + DA1470X_CRG_VSYS_VSYS_GEN_IRQ_MASK_OFFSET)

/* Register Bit-field Definitions *******************************************/

/* VSYS_GEN_CTRL register */

#define CRG_VSYS_VSYS_GEN_CTRL_EN_CURLIM (1 << 0)               /* Bit 0 */
#define CRG_VSYS_VSYS_GEN_CTRL_CURLIM_SET_SHIFT (1)             /* Bits 1-7 */
#define CRG_VSYS_VSYS_GEN_CTRL_CURLIM_SET_MASK (0x7f << CRG_VSYS_VSYS_GEN_CTRL_CURLIM_SET_SHIFT)
#  define CRG_VSYS_VSYS_GEN_CTRL_CURLIM_SET(n) ((uint32_t)(n) << CRG_VSYS_VSYS_GEN_CTRL_CURLIM_SET_SHIFT)
#define CRG_VSYS_VSYS_GEN_CTRL_CURLIM_GAIN_TRIM_SHIFT (8)       /* Bits 8-12 */
#define CRG_VSYS_VSYS_GEN_CTRL_CURLIM_GAIN_TRIM_MASK (0x1f << CRG_VSYS_VSYS_GEN_CTRL_CURLIM_GAIN_TRIM_SHIFT)
#  define CRG_VSYS_VSYS_GEN_CTRL_CURLIM_GAIN_TRIM(n) ((uint32_t)(n) << CRG_VSYS_VSYS_GEN_CTRL_CURLIM_GAIN_TRIM_SHIFT)
#define CRG_VSYS_VSYS_GEN_CTRL_CURLIM_OFFSET_TRIM_SHIFT (13)    /* Bits 13-17 */
#define CRG_VSYS_VSYS_GEN_CTRL_CURLIM_OFFSET_TRIM_MASK (0x1f << CRG_VSYS_VSYS_GEN_CTRL_CURLIM_OFFSET_TRIM_SHIFT)
#  define CRG_VSYS_VSYS_GEN_CTRL_CURLIM_OFFSET_TRIM(n) ((uint32_t)(n) << CRG_VSYS_VSYS_GEN_CTRL_CURLIM_OFFSET_TRIM_SHIFT)
#define CRG_VSYS_VSYS_GEN_CTRL_EN_HEADROOM (1 << 18)            /* Bit 18 */
#define CRG_VSYS_VSYS_GEN_CTRL_LDO_TEMP_PROTECT_MODE_SHIFT (19) /* Bits 19-20 */
#define CRG_VSYS_VSYS_GEN_CTRL_LDO_TEMP_PROTECT_MODE_MASK (0x3 << CRG_VSYS_VSYS_GEN_CTRL_LDO_TEMP_PROTECT_MODE_SHIFT)
#  define CRG_VSYS_VSYS_GEN_CTRL_LDO_TEMP_PROTECT_MODE(n) ((uint32_t)(n) << CRG_VSYS_VSYS_GEN_CTRL_LDO_TEMP_PROTECT_MODE_SHIFT)
#define CRG_VSYS_VSYS_GEN_CTRL_FORCE_LDO_ENABLE_SHIFT (21)      /* Bits 21-22 */
#define CRG_VSYS_VSYS_GEN_CTRL_FORCE_LDO_ENABLE_MASK (0x3 << CRG_VSYS_VSYS_GEN_CTRL_FORCE_LDO_ENABLE_SHIFT)
#  define CRG_VSYS_VSYS_GEN_CTRL_FORCE_LDO_ENABLE(n) ((uint32_t)(n) << CRG_VSYS_VSYS_GEN_CTRL_FORCE_LDO_ENABLE_SHIFT)
#define CRG_VSYS_VSYS_GEN_CTRL_FORCE_VBAT_VSYS_SW_SHIFT (23)    /* Bits 23-24 */
#define CRG_VSYS_VSYS_GEN_CTRL_FORCE_VBAT_VSYS_SW_MASK (0x3 << CRG_VSYS_VSYS_GEN_CTRL_FORCE_VBAT_VSYS_SW_SHIFT)
#  define CRG_VSYS_VSYS_GEN_CTRL_FORCE_VBAT_VSYS_SW(n) ((uint32_t)(n) << CRG_VSYS_VSYS_GEN_CTRL_FORCE_VBAT_VSYS_SW_SHIFT)

/* VSYS_GEN_IRQ_STATUS register */

#define CRG_VSYS_VSYS_GEN_IRQ_STATUS_LDO_VSYS_HIGH_TEMP_IRQ_STATUS (1 << 0) /* Bit 0 */
#define CRG_VSYS_VSYS_GEN_IRQ_STATUS_VBUS_LOW_DRIVE_IRQ_STATUS (1 << 1)     /* Bit 1 */

/* VSYS_GEN_IRQ_CLEAR register */

#define CRG_VSYS_VSYS_GEN_IRQ_CLEAR_LDO_VSYS_HIGH_TEMP_IRQ_CLEAR (1 << 0) /* Bit 0 */
#define CRG_VSYS_VSYS_GEN_IRQ_CLEAR_VBUS_LOW_DRIVE_IRQ_CLEAR (1 << 1)     /* Bit 1 */

/* VSYS_GEN_IRQ_MASK register */

#define CRG_VSYS_VSYS_GEN_IRQ_MASK_LDO_VSYS_HIGH_TEMP_IRQ_MASK (1 << 0) /* Bit 0 */
#define CRG_VSYS_VSYS_GEN_IRQ_MASK_VBUS_LOW_DRIVE_IRQ_MASK (1 << 1)     /* Bit 1 */

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_CRG_VSYS_H */
