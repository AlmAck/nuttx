/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_sdadc.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_SDADC_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_SDADC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470x_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define DA1470X_SDADC_CTRL_OFFSET 0x0000       /* Sigma Delta ADC Control Register */
#define DA1470X_SDADC_PGA_CTRL_OFFSET 0x0004   /* Sigma Delta ADC PGA Control Registers */
#define DA1470X_SDADC_CLEAR_INT_OFFSET 0x000c  /* Sigma Delta ADC Clear Interrupt Register */
#define DA1470X_SDADC_RESULT_OFFSET 0x0010     /* Sigma Delta ADC Result Register */
#define DA1470X_SDADC_AUDIO_FILT_OFFSET 0x0014 /* Sigma Delta ADC Audio Filter Register */

/* Register Addresses *******************************************************/

#define DA1470X_SDADC_CTRL (DA1470X_SDADC_BASE + DA1470X_SDADC_CTRL_OFFSET)
#define DA1470X_SDADC_PGA_CTRL (DA1470X_SDADC_BASE + DA1470X_SDADC_PGA_CTRL_OFFSET)
#define DA1470X_SDADC_CLEAR_INT (DA1470X_SDADC_BASE + DA1470X_SDADC_CLEAR_INT_OFFSET)
#define DA1470X_SDADC_RESULT (DA1470X_SDADC_BASE + DA1470X_SDADC_RESULT_OFFSET)
#define DA1470X_SDADC_AUDIO_FILT (DA1470X_SDADC_BASE + DA1470X_SDADC_AUDIO_FILT_OFFSET)

/* Register Bit-field Definitions *******************************************/

/* CTRL register */

#define SDADC_CTRL_EN (1 << 0)               /* Bit 0 */
#define SDADC_CTRL_START (1 << 1)            /* Bit 1 */
#define SDADC_CTRL_RESULT_MODE_SHIFT (10)    /* Bits 10-11 */
#define SDADC_CTRL_RESULT_MODE_MASK (0x3 << SDADC_CTRL_RESULT_MODE_SHIFT)
#  define SDADC_CTRL_RESULT_MODE(n) ((uint32_t)(n) << SDADC_CTRL_RESULT_MODE_SHIFT)
#define SDADC_CTRL_AUDIO_FILTER_EN (1 << 12) /* Bit 12 */
#define SDADC_CTRL_LDO_OK (1 << 15)          /* Bit 15 */
#define SDADC_CTRL_INT (1 << 16)             /* Bit 16 */
#define SDADC_CTRL_MINT (1 << 17)            /* Bit 17 */
#define SDADC_CTRL_DMA_EN (1 << 18)          /* Bit 18 */

/* PGA_CTRL register */

#define SDADC_PGA_CTRL_PGA_EN_SHIFT (0)     /* Bits 0-1 */
#define SDADC_PGA_CTRL_PGA_EN_MASK (0x3 << SDADC_PGA_CTRL_PGA_EN_SHIFT)
#  define SDADC_PGA_CTRL_PGA_EN(n) ((uint32_t)(n) << SDADC_PGA_CTRL_PGA_EN_SHIFT)
#define SDADC_PGA_CTRL_PGA_SHORTIN (1 << 2) /* Bit 2 */
#define SDADC_PGA_CTRL_PGA_BIAS_SHIFT (3)   /* Bits 3-5 */
#define SDADC_PGA_CTRL_PGA_BIAS_MASK (0x7 << SDADC_PGA_CTRL_PGA_BIAS_SHIFT)
#  define SDADC_PGA_CTRL_PGA_BIAS(n) ((uint32_t)(n) << SDADC_PGA_CTRL_PGA_BIAS_SHIFT)
#define SDADC_PGA_CTRL_PGA_MUTE (1 << 6)    /* Bit 6 */
#define SDADC_PGA_CTRL_PGA_MODE_SHIFT (7)   /* Bits 7-8 */
#define SDADC_PGA_CTRL_PGA_MODE_MASK (0x3 << SDADC_PGA_CTRL_PGA_MODE_SHIFT)
#  define SDADC_PGA_CTRL_PGA_MODE(n) ((uint32_t)(n) << SDADC_PGA_CTRL_PGA_MODE_SHIFT)
#define SDADC_PGA_CTRL_PGA_GAIN_SHIFT (9)   /* Bits 9-11 */
#define SDADC_PGA_CTRL_PGA_GAIN_MASK (0x7 << SDADC_PGA_CTRL_PGA_GAIN_SHIFT)
#  define SDADC_PGA_CTRL_PGA_GAIN(n) ((uint32_t)(n) << SDADC_PGA_CTRL_PGA_GAIN_SHIFT)

/* CLEAR_INT register */

#define SDADC_CLEAR_INT_CLR_INT_SHIFT (0) /* Bits 0-31 */
#define SDADC_CLEAR_INT_CLR_INT_MASK (0xffffffff << SDADC_CLEAR_INT_CLR_INT_SHIFT)
#  define SDADC_CLEAR_INT_CLR_INT(n) ((uint32_t)(n) << SDADC_CLEAR_INT_CLR_INT_SHIFT)

/* RESULT register */

#define SDADC_RESULT_VAL_SHIFT (0) /* Bits 0-15 */
#define SDADC_RESULT_VAL_MASK (0xffff << SDADC_RESULT_VAL_SHIFT)
#  define SDADC_RESULT_VAL(n) ((uint32_t)(n) << SDADC_RESULT_VAL_SHIFT)

/* AUDIO_FILT register */

#define SDADC_AUDIO_FILT_CIC_OFFSET_SHIFT (0) /* Bits 0-20 */
#define SDADC_AUDIO_FILT_CIC_OFFSET_MASK (0x1fffff << SDADC_AUDIO_FILT_CIC_OFFSET_SHIFT)
#  define SDADC_AUDIO_FILT_CIC_OFFSET(n) ((uint32_t)(n) << SDADC_AUDIO_FILT_CIC_OFFSET_SHIFT)

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_SDADC_H */
