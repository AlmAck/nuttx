/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_src.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_SRC_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_SRC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470x_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define DA1470X_SRC_CTRL_OFFSET 0x0000        /* SRC control register */
#define DA1470X_SRC_IN_FS_OFFSET 0x0004       /* SRC Sample input rate */
#define DA1470X_SRC_OUT_FS_OFFSET 0x0008      /* SRC Sample output rate */
#define DA1470X_SRC_IN1_OFFSET 0x000c         /* SRC data in 1 */
#define DA1470X_SRC_IN2_OFFSET 0x0010         /* SRC data in 2 */
#define DA1470X_SRC_OUT1_OFFSET 0x0014        /* SRC data out 1 */
#define DA1470X_SRC_OUT2_OFFSET 0x0018        /* SRC data out 2 */
#define DA1470X_SRC_MUX_OFFSET 0x001c         /* SRC mux register */
#define DA1470X_SRC_COEF10_SET1_OFFSET 0x0020 /* SRC coefficient 1,0 set 1 */
#define DA1470X_SRC_COEF32_SET1_OFFSET 0x0024 /* SRC coefficient 3,2 set 1 */
#define DA1470X_SRC_COEF54_SET1_OFFSET 0x0028 /* SRC coefficient 5,4 set 1 */
#define DA1470X_SRC_COEF76_SET1_OFFSET 0x002c /* SRC coefficient 7,6 set 1 */
#define DA1470X_SRC_COEF98_SET1_OFFSET 0x0030 /* SRC coefficient 9,8 set 1 */
#define DA1470X_SRC_COEF0A_SET1_OFFSET 0x0034 /* SRC coefficient 10 set 1 */

/* Register Addresses *******************************************************/

#define DA1470X_SRC1_CTRL (DA1470X_SRC1_BASE + DA1470X_SRC_CTRL_OFFSET)
#define DA1470X_SRC1_IN_FS (DA1470X_SRC1_BASE + DA1470X_SRC_IN_FS_OFFSET)
#define DA1470X_SRC1_OUT_FS (DA1470X_SRC1_BASE + DA1470X_SRC_OUT_FS_OFFSET)
#define DA1470X_SRC1_IN1 (DA1470X_SRC1_BASE + DA1470X_SRC_IN1_OFFSET)
#define DA1470X_SRC1_IN2 (DA1470X_SRC1_BASE + DA1470X_SRC_IN2_OFFSET)
#define DA1470X_SRC1_OUT1 (DA1470X_SRC1_BASE + DA1470X_SRC_OUT1_OFFSET)
#define DA1470X_SRC1_OUT2 (DA1470X_SRC1_BASE + DA1470X_SRC_OUT2_OFFSET)
#define DA1470X_SRC1_MUX (DA1470X_SRC1_BASE + DA1470X_SRC_MUX_OFFSET)
#define DA1470X_SRC1_COEF10_SET1 (DA1470X_SRC1_BASE + DA1470X_SRC_COEF10_SET1_OFFSET)
#define DA1470X_SRC1_COEF32_SET1 (DA1470X_SRC1_BASE + DA1470X_SRC_COEF32_SET1_OFFSET)
#define DA1470X_SRC1_COEF54_SET1 (DA1470X_SRC1_BASE + DA1470X_SRC_COEF54_SET1_OFFSET)
#define DA1470X_SRC1_COEF76_SET1 (DA1470X_SRC1_BASE + DA1470X_SRC_COEF76_SET1_OFFSET)
#define DA1470X_SRC1_COEF98_SET1 (DA1470X_SRC1_BASE + DA1470X_SRC_COEF98_SET1_OFFSET)
#define DA1470X_SRC1_COEF0A_SET1 (DA1470X_SRC1_BASE + DA1470X_SRC_COEF0A_SET1_OFFSET)

#define DA1470X_SRC2_CTRL (DA1470X_SRC2_BASE + DA1470X_SRC_CTRL_OFFSET)
#define DA1470X_SRC2_IN_FS (DA1470X_SRC2_BASE + DA1470X_SRC_IN_FS_OFFSET)
#define DA1470X_SRC2_OUT_FS (DA1470X_SRC2_BASE + DA1470X_SRC_OUT_FS_OFFSET)
#define DA1470X_SRC2_IN1 (DA1470X_SRC2_BASE + DA1470X_SRC_IN1_OFFSET)
#define DA1470X_SRC2_IN2 (DA1470X_SRC2_BASE + DA1470X_SRC_IN2_OFFSET)
#define DA1470X_SRC2_OUT1 (DA1470X_SRC2_BASE + DA1470X_SRC_OUT1_OFFSET)
#define DA1470X_SRC2_OUT2 (DA1470X_SRC2_BASE + DA1470X_SRC_OUT2_OFFSET)
#define DA1470X_SRC2_MUX (DA1470X_SRC2_BASE + DA1470X_SRC_MUX_OFFSET)
#define DA1470X_SRC2_COEF10_SET1 (DA1470X_SRC2_BASE + DA1470X_SRC_COEF10_SET1_OFFSET)
#define DA1470X_SRC2_COEF32_SET1 (DA1470X_SRC2_BASE + DA1470X_SRC_COEF32_SET1_OFFSET)
#define DA1470X_SRC2_COEF54_SET1 (DA1470X_SRC2_BASE + DA1470X_SRC_COEF54_SET1_OFFSET)
#define DA1470X_SRC2_COEF76_SET1 (DA1470X_SRC2_BASE + DA1470X_SRC_COEF76_SET1_OFFSET)
#define DA1470X_SRC2_COEF98_SET1 (DA1470X_SRC2_BASE + DA1470X_SRC_COEF98_SET1_OFFSET)
#define DA1470X_SRC2_COEF0A_SET1 (DA1470X_SRC2_BASE + DA1470X_SRC_COEF0A_SET1_OFFSET)

/* Register Bit-field Definitions *******************************************/

/* CTRL register */

#define SRC_CTRL_SRC_EN (1 << 0)              /* Bit 0 */
#define SRC_CTRL_SRC_IN_AMODE (1 << 1)        /* Bit 1 */
#define SRC_CTRL_SRC_IN_CAL_BYPASS (1 << 2)   /* Bit 2 */
#define SRC_CTRL_SRC_PDM_IN_INV (1 << 3)      /* Bit 3 */
#define SRC_CTRL_SRC_IN_DS_SHIFT (4)          /* Bits 4-5 */
#define SRC_CTRL_SRC_IN_DS_MASK (0x3 << SRC_CTRL_SRC_IN_DS_SHIFT)
#  define SRC_CTRL_SRC_IN_DS(n) ((uint32_t)(n) << SRC_CTRL_SRC_IN_DS_SHIFT)
#define SRC_CTRL_SRC_IN_OK (1 << 6)           /* Bit 6 */
#define SRC_CTRL_SRC_DITHER_DISABLE (1 << 7)  /* Bit 7 */
#define SRC_CTRL_SRC_IN_DSD_MODE (1 << 8)     /* Bit 8 */
#define SRC_CTRL_SRC_OUT_DSD_MODE (1 << 9)    /* Bit 9 */
#define SRC_CTRL_SRC_FIFO_ENABLE (1 << 10)    /* Bit 10 */
#define SRC_CTRL_SRC_FIFO_DIRECTION (1 << 11) /* Bit 11 */
#define SRC_CTRL_SRC_PDM_OUT_INV (1 << 12)    /* Bit 12 */
#define SRC_CTRL_SRC_OUT_AMODE (1 << 13)      /* Bit 13 */
#define SRC_CTRL_SRC_OUT_CAL_BYPASS (1 << 14) /* Bit 14 */
#define SRC_CTRL_SRC_OUT_US_SHIFT (16)        /* Bits 16-17 */
#define SRC_CTRL_SRC_OUT_US_MASK (0x3 << SRC_CTRL_SRC_OUT_US_SHIFT)
#  define SRC_CTRL_SRC_OUT_US(n) ((uint32_t)(n) << SRC_CTRL_SRC_OUT_US_SHIFT)
#define SRC_CTRL_SRC_OUT_OK (1 << 18)         /* Bit 18 */
#define SRC_CTRL_SRC_RESYNC (1 << 19)         /* Bit 19 */
#define SRC_CTRL_SRC_IN_OVFLOW (1 << 20)      /* Bit 20 */
#define SRC_CTRL_SRC_IN_UNFLOW (1 << 21)      /* Bit 21 */
#define SRC_CTRL_SRC_OUT_OVFLOW (1 << 22)     /* Bit 22 */
#define SRC_CTRL_SRC_OUT_UNFLOW (1 << 23)     /* Bit 23 */
#define SRC_CTRL_SRC_IN_FLOWCLR (1 << 24)     /* Bit 24 */
#define SRC_CTRL_SRC_OUT_FLOWCLR (1 << 25)    /* Bit 25 */
#define SRC_CTRL_SRC_PDM_DI_DEL_SHIFT (26)    /* Bits 26-27 */
#define SRC_CTRL_SRC_PDM_DI_DEL_MASK (0x3 << SRC_CTRL_SRC_PDM_DI_DEL_SHIFT)
#  define SRC_CTRL_SRC_PDM_DI_DEL(n) ((uint32_t)(n) << SRC_CTRL_SRC_PDM_DI_DEL_SHIFT)
#define SRC_CTRL_SRC_PDM_MODE_SHIFT (28)      /* Bits 28-29 */
#define SRC_CTRL_SRC_PDM_MODE_MASK (0x3 << SRC_CTRL_SRC_PDM_MODE_SHIFT)
#  define SRC_CTRL_SRC_PDM_MODE(n) ((uint32_t)(n) << SRC_CTRL_SRC_PDM_MODE_SHIFT)
#define SRC_CTRL_SRC_PDM_DO_DEL_SHIFT (30)    /* Bits 30-31 */
#define SRC_CTRL_SRC_PDM_DO_DEL_MASK (0x3 << SRC_CTRL_SRC_PDM_DO_DEL_SHIFT)
#  define SRC_CTRL_SRC_PDM_DO_DEL(n) ((uint32_t)(n) << SRC_CTRL_SRC_PDM_DO_DEL_SHIFT)

/* IN_FS register */

#define SRC_IN_FS_SRC_IN_FS_SHIFT (0) /* Bits 0-23 */
#define SRC_IN_FS_SRC_IN_FS_MASK (0xffffff << SRC_IN_FS_SRC_IN_FS_SHIFT)
#  define SRC_IN_FS_SRC_IN_FS(n) ((uint32_t)(n) << SRC_IN_FS_SRC_IN_FS_SHIFT)

/* OUT_FS register */

#define SRC_OUT_FS_SRC_OUT_FS_SHIFT (0) /* Bits 0-23 */
#define SRC_OUT_FS_SRC_OUT_FS_MASK (0xffffff << SRC_OUT_FS_SRC_OUT_FS_SHIFT)
#  define SRC_OUT_FS_SRC_OUT_FS(n) ((uint32_t)(n) << SRC_OUT_FS_SRC_OUT_FS_SHIFT)

/* IN1 register */

#define SRC_IN1_SRC_IN_SHIFT (0) /* Bits 0-31 */
#define SRC_IN1_SRC_IN_MASK (0xffffffff << SRC_IN1_SRC_IN_SHIFT)
#  define SRC_IN1_SRC_IN(n) ((uint32_t)(n) << SRC_IN1_SRC_IN_SHIFT)

/* IN2 register */

#define SRC_IN2_SRC_IN_SHIFT (0) /* Bits 0-31 */
#define SRC_IN2_SRC_IN_MASK (0xffffffff << SRC_IN2_SRC_IN_SHIFT)
#  define SRC_IN2_SRC_IN(n) ((uint32_t)(n) << SRC_IN2_SRC_IN_SHIFT)

/* OUT1 register */

#define SRC_OUT1_SRC_OUT_SHIFT (0) /* Bits 0-31 */
#define SRC_OUT1_SRC_OUT_MASK (0xffffffff << SRC_OUT1_SRC_OUT_SHIFT)
#  define SRC_OUT1_SRC_OUT(n) ((uint32_t)(n) << SRC_OUT1_SRC_OUT_SHIFT)

/* OUT2 register */

#define SRC_OUT2_SRC_OUT_SHIFT (0) /* Bits 0-31 */
#define SRC_OUT2_SRC_OUT_MASK (0xffffffff << SRC_OUT2_SRC_OUT_SHIFT)
#  define SRC_OUT2_SRC_OUT(n) ((uint32_t)(n) << SRC_OUT2_SRC_OUT_SHIFT)

/* MUX register */

#define SRC_MUX_MUX_IN_SHIFT (0)      /* Bits 0-2 */
#define SRC_MUX_MUX_IN_MASK (0x7 << SRC_MUX_MUX_IN_SHIFT)
#  define SRC_MUX_MUX_IN(n) ((uint32_t)(n) << SRC_MUX_MUX_IN_SHIFT)
#define SRC_MUX_PCM1_MUX_IN_SHIFT (3) /* Bits 3-5 */
#define SRC_MUX_PCM1_MUX_IN_MASK (0x7 << SRC_MUX_PCM1_MUX_IN_SHIFT)
#  define SRC_MUX_PCM1_MUX_IN(n) ((uint32_t)(n) << SRC_MUX_PCM1_MUX_IN_SHIFT)
#define SRC_MUX_PDM1_MUX_IN (1 << 6)  /* Bit 6 */

/* COEF10_SET1 register */

#define SRC_COEF10_SET1_SRC_COEF0_SHIFT (0)  /* Bits 0-15 */
#define SRC_COEF10_SET1_SRC_COEF0_MASK (0xffff << SRC_COEF10_SET1_SRC_COEF0_SHIFT)
#  define SRC_COEF10_SET1_SRC_COEF0(n) ((uint32_t)(n) << SRC_COEF10_SET1_SRC_COEF0_SHIFT)
#define SRC_COEF10_SET1_SRC_COEF1_SHIFT (16) /* Bits 16-31 */
#define SRC_COEF10_SET1_SRC_COEF1_MASK (0xffff << SRC_COEF10_SET1_SRC_COEF1_SHIFT)
#  define SRC_COEF10_SET1_SRC_COEF1(n) ((uint32_t)(n) << SRC_COEF10_SET1_SRC_COEF1_SHIFT)

/* COEF32_SET1 register */

#define SRC_COEF32_SET1_SRC_COEF2_SHIFT (0)  /* Bits 0-15 */
#define SRC_COEF32_SET1_SRC_COEF2_MASK (0xffff << SRC_COEF32_SET1_SRC_COEF2_SHIFT)
#  define SRC_COEF32_SET1_SRC_COEF2(n) ((uint32_t)(n) << SRC_COEF32_SET1_SRC_COEF2_SHIFT)
#define SRC_COEF32_SET1_SRC_COEF3_SHIFT (16) /* Bits 16-31 */
#define SRC_COEF32_SET1_SRC_COEF3_MASK (0xffff << SRC_COEF32_SET1_SRC_COEF3_SHIFT)
#  define SRC_COEF32_SET1_SRC_COEF3(n) ((uint32_t)(n) << SRC_COEF32_SET1_SRC_COEF3_SHIFT)

/* COEF54_SET1 register */

#define SRC_COEF54_SET1_SRC_COEF4_SHIFT (0)  /* Bits 0-15 */
#define SRC_COEF54_SET1_SRC_COEF4_MASK (0xffff << SRC_COEF54_SET1_SRC_COEF4_SHIFT)
#  define SRC_COEF54_SET1_SRC_COEF4(n) ((uint32_t)(n) << SRC_COEF54_SET1_SRC_COEF4_SHIFT)
#define SRC_COEF54_SET1_SRC_COEF5_SHIFT (16) /* Bits 16-31 */
#define SRC_COEF54_SET1_SRC_COEF5_MASK (0xffff << SRC_COEF54_SET1_SRC_COEF5_SHIFT)
#  define SRC_COEF54_SET1_SRC_COEF5(n) ((uint32_t)(n) << SRC_COEF54_SET1_SRC_COEF5_SHIFT)

/* COEF76_SET1 register */

#define SRC_COEF76_SET1_SRC_COEF6_SHIFT (0)  /* Bits 0-15 */
#define SRC_COEF76_SET1_SRC_COEF6_MASK (0xffff << SRC_COEF76_SET1_SRC_COEF6_SHIFT)
#  define SRC_COEF76_SET1_SRC_COEF6(n) ((uint32_t)(n) << SRC_COEF76_SET1_SRC_COEF6_SHIFT)
#define SRC_COEF76_SET1_SRC_COEF7_SHIFT (16) /* Bits 16-31 */
#define SRC_COEF76_SET1_SRC_COEF7_MASK (0xffff << SRC_COEF76_SET1_SRC_COEF7_SHIFT)
#  define SRC_COEF76_SET1_SRC_COEF7(n) ((uint32_t)(n) << SRC_COEF76_SET1_SRC_COEF7_SHIFT)

/* COEF98_SET1 register */

#define SRC_COEF98_SET1_SRC_COEF8_SHIFT (0)  /* Bits 0-15 */
#define SRC_COEF98_SET1_SRC_COEF8_MASK (0xffff << SRC_COEF98_SET1_SRC_COEF8_SHIFT)
#  define SRC_COEF98_SET1_SRC_COEF8(n) ((uint32_t)(n) << SRC_COEF98_SET1_SRC_COEF8_SHIFT)
#define SRC_COEF98_SET1_SRC_COEF9_SHIFT (16) /* Bits 16-31 */
#define SRC_COEF98_SET1_SRC_COEF9_MASK (0xffff << SRC_COEF98_SET1_SRC_COEF9_SHIFT)
#  define SRC_COEF98_SET1_SRC_COEF9(n) ((uint32_t)(n) << SRC_COEF98_SET1_SRC_COEF9_SHIFT)

/* COEF0A_SET1 register */

#define SRC_COEF0A_SET1_SRC_COEF10_SHIFT (0) /* Bits 0-15 */
#define SRC_COEF0A_SET1_SRC_COEF10_MASK (0xffff << SRC_COEF0A_SET1_SRC_COEF10_SHIFT)
#  define SRC_COEF0A_SET1_SRC_COEF10(n) ((uint32_t)(n) << SRC_COEF0A_SET1_SRC_COEF10_SHIFT)

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_SRC_H */
