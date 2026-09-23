/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_crg_aud.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_CRG_AUD_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_CRG_AUD_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470x_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define DA1470X_CRG_AUD_PCM_DIV_OFFSET 0x0040  /* PCM divider and enables */
#define DA1470X_CRG_AUD_PCM_FDIV_OFFSET 0x0044 /* PCM fractional division register */
#define DA1470X_CRG_AUD_PDM_DIV_OFFSET 0x0048  /* PDM divider and enables */
#define DA1470X_CRG_AUD_SRC_DIV_OFFSET 0x004c  /* SRC divider and enables */

/* Register Addresses *******************************************************/

#define DA1470X_CRG_AUD_PCM_DIV (DA1470X_CRG_AUD_BASE + DA1470X_CRG_AUD_PCM_DIV_OFFSET)
#define DA1470X_CRG_AUD_PCM_FDIV (DA1470X_CRG_AUD_BASE + DA1470X_CRG_AUD_PCM_FDIV_OFFSET)
#define DA1470X_CRG_AUD_PDM_DIV (DA1470X_CRG_AUD_BASE + DA1470X_CRG_AUD_PDM_DIV_OFFSET)
#define DA1470X_CRG_AUD_SRC_DIV (DA1470X_CRG_AUD_BASE + DA1470X_CRG_AUD_SRC_DIV_OFFSET)

/* Register Bit-field Definitions *******************************************/

/* PCM_DIV register */

#define CRG_AUD_PCM_DIV_PCM_DIV_SHIFT (0)     /* Bits 0-11 */
#define CRG_AUD_PCM_DIV_PCM_DIV_MASK (0xfff << CRG_AUD_PCM_DIV_PCM_DIV_SHIFT)
#  define CRG_AUD_PCM_DIV_PCM_DIV(n) ((uint32_t)(n) << CRG_AUD_PCM_DIV_PCM_DIV_SHIFT)
#define CRG_AUD_PCM_DIV_CLK_PCM_EN (1 << 12)  /* Bit 12 */
#define CRG_AUD_PCM_DIV_PCM_SRC_SEL (1 << 13) /* Bit 13 */

/* PCM_FDIV register */

#define CRG_AUD_PCM_FDIV_PCM_FDIV_SHIFT (0) /* Bits 0-15 */
#define CRG_AUD_PCM_FDIV_PCM_FDIV_MASK (0xffff << CRG_AUD_PCM_FDIV_PCM_FDIV_SHIFT)
#  define CRG_AUD_PCM_FDIV_PCM_FDIV(n) ((uint32_t)(n) << CRG_AUD_PCM_FDIV_PCM_FDIV_SHIFT)

/* PDM_DIV register */

#define CRG_AUD_PDM_DIV_PDM_DIV_SHIFT (0)        /* Bits 0-7 */
#define CRG_AUD_PDM_DIV_PDM_DIV_MASK (0xff << CRG_AUD_PDM_DIV_PDM_DIV_SHIFT)
#  define CRG_AUD_PDM_DIV_PDM_DIV(n) ((uint32_t)(n) << CRG_AUD_PDM_DIV_PDM_DIV_SHIFT)
#define CRG_AUD_PDM_DIV_CLK_PDM_EN (1 << 8)      /* Bit 8 */
#define CRG_AUD_PDM_DIV_PDM_MASTER_MODE (1 << 9) /* Bit 9 */

/* SRC_DIV register */

#define CRG_AUD_SRC_DIV_SRC_DIV_SHIFT (0)     /* Bits 0-7 */
#define CRG_AUD_SRC_DIV_SRC_DIV_MASK (0xff << CRG_AUD_SRC_DIV_SRC_DIV_SHIFT)
#  define CRG_AUD_SRC_DIV_SRC_DIV(n) ((uint32_t)(n) << CRG_AUD_SRC_DIV_SRC_DIV_SHIFT)
#define CRG_AUD_SRC_DIV_SRC2_DIV_SHIFT (8)    /* Bits 8-15 */
#define CRG_AUD_SRC_DIV_SRC2_DIV_MASK (0xff << CRG_AUD_SRC_DIV_SRC2_DIV_SHIFT)
#  define CRG_AUD_SRC_DIV_SRC2_DIV(n) ((uint32_t)(n) << CRG_AUD_SRC_DIV_SRC2_DIV_SHIFT)
#define CRG_AUD_SRC_DIV_CLK_SRC_EN (1 << 16)  /* Bit 16 */
#define CRG_AUD_SRC_DIV_CLK_SRC2_EN (1 << 17) /* Bit 17 */

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_CRG_AUD_H */
