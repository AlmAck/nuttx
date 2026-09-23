/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_pcm.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_PCM_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_PCM_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470x_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define DA1470X_PCM_CTRL_OFFSET 0x0000 /* PCM1 Control register */
#define DA1470X_PCM_IN1_OFFSET 0x0004  /* PCM1 data in 1 */
#define DA1470X_PCM_IN2_OFFSET 0x0008  /* PCM1 data in 2 */
#define DA1470X_PCM_OUT1_OFFSET 0x000c /* PCM1 data out 1 */
#define DA1470X_PCM_OUT2_OFFSET 0x0010 /* PCM1 data out 2 */

/* Register Addresses *******************************************************/

#define DA1470X_PCM1_CTRL (DA1470X_PCM1_BASE + DA1470X_PCM_CTRL_OFFSET)
#define DA1470X_PCM1_IN1 (DA1470X_PCM1_BASE + DA1470X_PCM_IN1_OFFSET)
#define DA1470X_PCM1_IN2 (DA1470X_PCM1_BASE + DA1470X_PCM_IN2_OFFSET)
#define DA1470X_PCM1_OUT1 (DA1470X_PCM1_BASE + DA1470X_PCM_OUT1_OFFSET)
#define DA1470X_PCM1_OUT2 (DA1470X_PCM1_BASE + DA1470X_PCM_OUT2_OFFSET)

/* Register Bit-field Definitions *******************************************/

/* CTRL register */

#define PCM_CTRL_PCM_EN (1 << 0)        /* Bit 0 */
#define PCM_CTRL_PCM_MASTER (1 << 1)    /* Bit 1 */
#define PCM_CTRL_PCM_FSCLEN_SHIFT (2)   /* Bits 2-5 */
#define PCM_CTRL_PCM_FSCLEN_MASK (0xf << PCM_CTRL_PCM_FSCLEN_SHIFT)
#  define PCM_CTRL_PCM_FSCLEN(n) ((uint32_t)(n) << PCM_CTRL_PCM_FSCLEN_SHIFT)
#define PCM_CTRL_PCM_FSCDEL (1 << 6)    /* Bit 6 */
#define PCM_CTRL_PCM_PPOD (1 << 7)      /* Bit 7 */
#define PCM_CTRL_PCM_CLKINV (1 << 8)    /* Bit 8 */
#define PCM_CTRL_PCM_FSCINV (1 << 9)    /* Bit 9 */
#define PCM_CTRL_PCM_CLK_BIT (1 << 10)  /* Bit 10 */
#define PCM_CTRL_PCM_CH_DEL_SHIFT (11)  /* Bits 11-15 */
#define PCM_CTRL_PCM_CH_DEL_MASK (0x1f << PCM_CTRL_PCM_CH_DEL_SHIFT)
#  define PCM_CTRL_PCM_CH_DEL(n) ((uint32_t)(n) << PCM_CTRL_PCM_CH_DEL_SHIFT)
#define PCM_CTRL_PCM_FSC_EDGE (1 << 16) /* Bit 16 */
#define PCM_CTRL_PCM_FSC_DIV_SHIFT (20) /* Bits 20-31 */
#define PCM_CTRL_PCM_FSC_DIV_MASK (0xfff << PCM_CTRL_PCM_FSC_DIV_SHIFT)
#  define PCM_CTRL_PCM_FSC_DIV(n) ((uint32_t)(n) << PCM_CTRL_PCM_FSC_DIV_SHIFT)

/* IN1 register */

#define PCM_IN1_PCM_IN_SHIFT (0) /* Bits 0-31 */
#define PCM_IN1_PCM_IN_MASK (0xffffffff << PCM_IN1_PCM_IN_SHIFT)
#  define PCM_IN1_PCM_IN(n) ((uint32_t)(n) << PCM_IN1_PCM_IN_SHIFT)

/* IN2 register */

#define PCM_IN2_PCM_IN_SHIFT (0) /* Bits 0-31 */
#define PCM_IN2_PCM_IN_MASK (0xffffffff << PCM_IN2_PCM_IN_SHIFT)
#  define PCM_IN2_PCM_IN(n) ((uint32_t)(n) << PCM_IN2_PCM_IN_SHIFT)

/* OUT1 register */

#define PCM_OUT1_PCM_OUT_SHIFT (0) /* Bits 0-31 */
#define PCM_OUT1_PCM_OUT_MASK (0xffffffff << PCM_OUT1_PCM_OUT_SHIFT)
#  define PCM_OUT1_PCM_OUT(n) ((uint32_t)(n) << PCM_OUT1_PCM_OUT_SHIFT)

/* OUT2 register */

#define PCM_OUT2_PCM_OUT_SHIFT (0) /* Bits 0-31 */
#define PCM_OUT2_PCM_OUT_MASK (0xffffffff << PCM_OUT2_PCM_OUT_SHIFT)
#  define PCM_OUT2_PCM_OUT(n) ((uint32_t)(n) << PCM_OUT2_PCM_OUT_SHIFT)

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_PCM_H */
