/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_anamisc_bif.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_ANAMISC_BIF_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_ANAMISC_BIF_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470x_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define DA1470X_ANAMISC_BIF_CLK_REF_SEL_OFFSET 0x0010 /* Select clock for oscillator calibration */
#define DA1470X_ANAMISC_BIF_CLK_REF_CNT_OFFSET 0x0014 /* Count value for oscillator calibration */
#define DA1470X_ANAMISC_BIF_CLK_REF_VAL_OFFSET 0x0018 /* DIVN reference cycles, lower 16 bits */
#define DA1470X_ANAMISC_BIF_CLK_CAL_IRQ_OFFSET 0x001c /* Select clock for oscillator calibration */

/* Register Addresses *******************************************************/

#define DA1470X_ANAMISC_BIF_CLK_REF_SEL (DA1470X_ANAMISC_BIF_BASE + DA1470X_ANAMISC_BIF_CLK_REF_SEL_OFFSET)
#define DA1470X_ANAMISC_BIF_CLK_REF_CNT (DA1470X_ANAMISC_BIF_BASE + DA1470X_ANAMISC_BIF_CLK_REF_CNT_OFFSET)
#define DA1470X_ANAMISC_BIF_CLK_REF_VAL (DA1470X_ANAMISC_BIF_BASE + DA1470X_ANAMISC_BIF_CLK_REF_VAL_OFFSET)
#define DA1470X_ANAMISC_BIF_CLK_CAL_IRQ (DA1470X_ANAMISC_BIF_BASE + DA1470X_ANAMISC_BIF_CLK_CAL_IRQ_OFFSET)

/* Register Bit-field Definitions *******************************************/

/* CLK_REF_SEL register */

#define ANAMISC_BIF_CLK_REF_SEL_REF_CLK_SEL_SHIFT (0)   /* Bits 0-2 */
#define ANAMISC_BIF_CLK_REF_SEL_REF_CLK_SEL_MASK (0x7 << ANAMISC_BIF_CLK_REF_SEL_REF_CLK_SEL_SHIFT)
#  define ANAMISC_BIF_CLK_REF_SEL_REF_CLK_SEL(n) ((uint32_t)(n) << ANAMISC_BIF_CLK_REF_SEL_REF_CLK_SEL_SHIFT)
#define ANAMISC_BIF_CLK_REF_SEL_REF_CAL_START (1 << 3)  /* Bit 3 */
#define ANAMISC_BIF_CLK_REF_SEL_EXT_CNT_EN_SEL (1 << 4) /* Bit 4 */
#define ANAMISC_BIF_CLK_REF_SEL_CAL_CLK_SEL_SHIFT (5)   /* Bits 5-7 */
#define ANAMISC_BIF_CLK_REF_SEL_CAL_CLK_SEL_MASK (0x7 << ANAMISC_BIF_CLK_REF_SEL_CAL_CLK_SEL_SHIFT)
#  define ANAMISC_BIF_CLK_REF_SEL_CAL_CLK_SEL(n) ((uint32_t)(n) << ANAMISC_BIF_CLK_REF_SEL_CAL_CLK_SEL_SHIFT)

/* CLK_REF_CNT register */

#define ANAMISC_BIF_CLK_REF_CNT_REF_CNT_VAL_SHIFT (0) /* Bits 0-15 */
#define ANAMISC_BIF_CLK_REF_CNT_REF_CNT_VAL_MASK (0xffff << ANAMISC_BIF_CLK_REF_CNT_REF_CNT_VAL_SHIFT)
#  define ANAMISC_BIF_CLK_REF_CNT_REF_CNT_VAL(n) ((uint32_t)(n) << ANAMISC_BIF_CLK_REF_CNT_REF_CNT_VAL_SHIFT)

/* CLK_REF_VAL register */

#define ANAMISC_BIF_CLK_REF_VAL_XTAL_CNT_VAL_SHIFT (0) /* Bits 0-31 */
#define ANAMISC_BIF_CLK_REF_VAL_XTAL_CNT_VAL_MASK (0xffffffff << ANAMISC_BIF_CLK_REF_VAL_XTAL_CNT_VAL_SHIFT)
#  define ANAMISC_BIF_CLK_REF_VAL_XTAL_CNT_VAL(n) ((uint32_t)(n) << ANAMISC_BIF_CLK_REF_VAL_XTAL_CNT_VAL_SHIFT)

/* CLK_CAL_IRQ register */

#define ANAMISC_BIF_CLK_CAL_IRQ_CLK_CAL_IRQ_EN (1 << 0)     /* Bit 0 */
#define ANAMISC_BIF_CLK_CAL_IRQ_CLK_CAL_IRQ_STATUS (1 << 1) /* Bit 1 */
#define ANAMISC_BIF_CLK_CAL_IRQ_CLK_CAL_IRQ_CLR (1 << 2)    /* Bit 2 */

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_ANAMISC_BIF_H */
