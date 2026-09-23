/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_gpadc.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_GPADC_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_GPADC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include "da1470x_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define DA1470X_GPADC_CTRL_OFFSET 0x0000      /* General Purpose ADC Control Register */
#define DA1470X_GPADC_CTRL2_OFFSET 0x0004     /* General Purpose ADC Second Control Register */
#define DA1470X_GPADC_CTRL3_OFFSET 0x0008     /* General Purpose ADC Third Control Register */
#define DA1470X_GPADC_SEL_OFFSET 0x000c       /* General Purpose ADC Input Selection Register */
#define DA1470X_GPADC_OFFP_OFFSET 0x0010      /* General Purpose ADC Positive Offset Register */
#define DA1470X_GPADC_OFFN_OFFSET 0x0014      /* General Purpose ADC Negative Offset Register */
#define DA1470X_GPADC_TRIM_OFFSET 0x0018      /* General Purpose ADC Trim Register */
#define DA1470X_GPADC_CLEAR_INT_OFFSET 0x001c /* General Purpose ADC Clear Interrupt Register */
#define DA1470X_GPADC_RESULT_OFFSET 0x0020    /* General Purpose ADC Result Register */

/* Register Addresses *******************************************************/

#define DA1470X_GPADC_CTRL (DA1470X_GPADC_BASE + DA1470X_GPADC_CTRL_OFFSET)
#define DA1470X_GPADC_CTRL2 (DA1470X_GPADC_BASE + DA1470X_GPADC_CTRL2_OFFSET)
#define DA1470X_GPADC_CTRL3 (DA1470X_GPADC_BASE + DA1470X_GPADC_CTRL3_OFFSET)
#define DA1470X_GPADC_SEL (DA1470X_GPADC_BASE + DA1470X_GPADC_SEL_OFFSET)
#define DA1470X_GPADC_OFFP (DA1470X_GPADC_BASE + DA1470X_GPADC_OFFP_OFFSET)
#define DA1470X_GPADC_OFFN (DA1470X_GPADC_BASE + DA1470X_GPADC_OFFN_OFFSET)
#define DA1470X_GPADC_TRIM (DA1470X_GPADC_BASE + DA1470X_GPADC_TRIM_OFFSET)
#define DA1470X_GPADC_CLEAR_INT (DA1470X_GPADC_BASE + DA1470X_GPADC_CLEAR_INT_OFFSET)
#define DA1470X_GPADC_RESULT (DA1470X_GPADC_BASE + DA1470X_GPADC_RESULT_OFFSET)

/* Register Bit-field Definitions *******************************************/

/* CTRL register */

#define GPADC_CTRL_EN (1 << 0)                 /* Bit 0 */
#define GPADC_CTRL_START (1 << 1)              /* Bit 1 */
#define GPADC_CTRL_CONT (1 << 2)               /* Bit 2 */
#define GPADC_CTRL_DMA_EN (1 << 3)             /* Bit 3 */
#define GPADC_CTRL_INT (1 << 4)                /* Bit 4 */
#define GPADC_CTRL_MINT (1 << 5)               /* Bit 5 */
#define GPADC_CTRL_SE (1 << 6)                 /* Bit 6 */
#define GPADC_CTRL_MUTE (1 << 7)               /* Bit 7 */
#define GPADC_CTRL_SIGN (1 << 8)               /* Bit 8 */
#define GPADC_CTRL_CHOP (1 << 9)               /* Bit 9 */
#define GPADC_CTRL_LDO_HOLD (1 << 10)          /* Bit 10 */
#define GPADC_CTRL_DIFF_TEMP_EN (1 << 11)      /* Bit 11 */
#define GPADC_CTRL_DIFF_TEMP_SEL_SHIFT (12)    /* Bits 12-13 */
#define GPADC_CTRL_DIFF_TEMP_SEL_MASK (0x3 << GPADC_CTRL_DIFF_TEMP_SEL_SHIFT)
#  define GPADC_CTRL_DIFF_TEMP_SEL(n) ((uint32_t)(n) << GPADC_CTRL_DIFF_TEMP_SEL_SHIFT)
#define GPADC_CTRL_DIE_TEMP_EN (1 << 14)       /* Bit 14 */
#define GPADC_CTRL_RESULT_MODE_SHIFT (15)      /* Bits 15-16 */
#define GPADC_CTRL_RESULT_MODE_MASK (0x3 << GPADC_CTRL_RESULT_MODE_SHIFT)
#  define GPADC_CTRL_RESULT_MODE(n) ((uint32_t)(n) << GPADC_CTRL_RESULT_MODE_SHIFT)

/* CTRL2 register */

#define GPADC_CTRL2_ATTN_SHIFT (0)             /* Bits 0-1 */
#define GPADC_CTRL2_ATTN_MASK (0x3 << GPADC_CTRL2_ATTN_SHIFT)
#  define GPADC_CTRL2_ATTN(n) ((uint32_t)(n) << GPADC_CTRL2_ATTN_SHIFT)
#define GPADC_CTRL2_I20U (1 << 2)              /* Bit 2 */
#define GPADC_CTRL2_CONV_NRS_SHIFT (6)         /* Bits 6-8 */
#define GPADC_CTRL2_CONV_NRS_MASK (0x7 << GPADC_CTRL2_CONV_NRS_SHIFT)
#  define GPADC_CTRL2_CONV_NRS(n) ((uint32_t)(n) << GPADC_CTRL2_CONV_NRS_SHIFT)
#define GPADC_CTRL2_SMPL_TIME_SHIFT (9)        /* Bits 9-12 */
#define GPADC_CTRL2_SMPL_TIME_MASK (0xf << GPADC_CTRL2_SMPL_TIME_SHIFT)
#  define GPADC_CTRL2_SMPL_TIME(n) ((uint32_t)(n) << GPADC_CTRL2_SMPL_TIME_SHIFT)
#define GPADC_CTRL2_STORE_DEL_SHIFT (13)       /* Bits 13-15 */
#define GPADC_CTRL2_STORE_DEL_MASK (0x7 << GPADC_CTRL2_STORE_DEL_SHIFT)
#  define GPADC_CTRL2_STORE_DEL(n) ((uint32_t)(n) << GPADC_CTRL2_STORE_DEL_SHIFT)

/* CTRL3 register */

#define GPADC_CTRL3_EN_DEL_SHIFT (0)           /* Bits 0-7 */
#define GPADC_CTRL3_EN_DEL_MASK (0xff << GPADC_CTRL3_EN_DEL_SHIFT)
#  define GPADC_CTRL3_EN_DEL(n) ((uint32_t)(n) << GPADC_CTRL3_EN_DEL_SHIFT)
#define GPADC_CTRL3_INTERVAL_SHIFT (8)         /* Bits 8-15 */
#define GPADC_CTRL3_INTERVAL_MASK (0xff << GPADC_CTRL3_INTERVAL_SHIFT)
#  define GPADC_CTRL3_INTERVAL(n) ((uint32_t)(n) << GPADC_CTRL3_INTERVAL_SHIFT)

/* SEL register */

#define GPADC_SEL_N_SHIFT (0)                  /* Bits 0-2 */
#define GPADC_SEL_N_MASK (0x7 << GPADC_SEL_N_SHIFT)
#  define GPADC_SEL_N(n) ((uint32_t)(n) << GPADC_SEL_N_SHIFT)
#define GPADC_SEL_P_SHIFT (4)                  /* Bits 4-6 */
#define GPADC_SEL_P_MASK (0x7 << GPADC_SEL_P_SHIFT)
#  define GPADC_SEL_P(n) ((uint32_t)(n) << GPADC_SEL_P_SHIFT)
#define GPADC_SEL_P_TST (1 << 7)               /* Bit 7 */
#define GPADC_SEL_MUX1_SHIFT (8)               /* Bits 8-10 */
#define GPADC_SEL_MUX1_MASK (0x7 << GPADC_SEL_MUX1_SHIFT)
#  define GPADC_SEL_MUX1(n) ((uint32_t)(n) << GPADC_SEL_MUX1_SHIFT)
#define GPADC_SEL_MUX2_SHIFT (11)              /* Bits 11-13 */
#define GPADC_SEL_MUX2_MASK (0x7 << GPADC_SEL_MUX2_SHIFT)
#  define GPADC_SEL_MUX2(n) ((uint32_t)(n) << GPADC_SEL_MUX2_SHIFT)
#define GPADC_SEL_LDO_SENSE_SEL_SHIFT (14)     /* Bits 14-16 */
#define GPADC_SEL_LDO_SENSE_SEL_MASK (0x7 << GPADC_SEL_LDO_SENSE_SEL_SHIFT)
#  define GPADC_SEL_LDO_SENSE_SEL(n) ((uint32_t)(n) << GPADC_SEL_LDO_SENSE_SEL_SHIFT)

/* MUX2 selections.  These are the rails the part can measure without any
 * pin being involved, each behind a scaler of its own.
 */

/* The part's own register description and the vendor's driver disagree
 * about which code is V12 and which is V18P.  Measured: code 1 reads
 * 1185 mV on this board, so the driver is right and the register
 * description is not.
 */

#define GPADC_MUX2_NONE 0
#define GPADC_MUX2_V12  1
#define GPADC_MUX2_V18  2
#define GPADC_MUX2_V14  3
#define GPADC_MUX2_V18P 4
#define GPADC_MUX2_VSYS 5                      /* Scaler 0.157 */
#define GPADC_MUX2_VBUS 6                      /* Scaler 0.164 */
#define GPADC_MUX2_VBAT 7                      /* Scaler 0.189 */

/* MUX1 selections */

#define GPADC_MUX1_NONE        0
#define GPADC_MUX1_NC          1
#define GPADC_MUX1_I_SENSE_BUS 3
#define GPADC_MUX1_V30         5
#define GPADC_MUX1_V18F        7

/* OFFP/OFFN registers */

#define GPADC_OFF_SHIFT (0)                    /* Bits 0-9 */
#define GPADC_OFF_MASK (0x3ff << GPADC_OFF_SHIFT)
#  define GPADC_OFF(n) ((uint32_t)(n) << GPADC_OFF_SHIFT)
#define GPADC_OFF_DEFAULT 0x200                /* Uncalibrated mid-scale */

/* TRIM register */

#define GPADC_TRIM_LDO_LEVEL_SHIFT (0)         /* Bits 0-2 */
#define GPADC_TRIM_LDO_LEVEL_MASK (0x7 << GPADC_TRIM_LDO_LEVEL_SHIFT)
#  define GPADC_TRIM_LDO_LEVEL(n) ((uint32_t)(n) << GPADC_TRIM_LDO_LEVEL_SHIFT)

/* RESULT register */

#define GPADC_RESULT_VAL_SHIFT (0)             /* Bits 0-15 */
#define GPADC_RESULT_VAL_MASK (0xffff << GPADC_RESULT_VAL_SHIFT)

/* The reference the result is measured against, with no attenuator.  Each
 * step of ATTN multiplies the range by one of these again.
 */

#define GPADC_VREF_MV 900

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_GPADC_H */
