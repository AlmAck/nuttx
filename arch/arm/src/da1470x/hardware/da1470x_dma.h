/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_dma.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_DMA_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_DMA_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470x_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define DA1470X_DMA_DMA0_A_START_OFFSET 0x0000   /* Source address register of DMA channel 0 */
#define DA1470X_DMA_DMA0_B_START_OFFSET 0x0004   /* Destination address register of DMA chan */
#define DA1470X_DMA_DMA0_INT_OFFSET 0x0008       /* Interrupt length register of DMA channel */
#define DA1470X_DMA_DMA0_LEN_OFFSET 0x000c       /* Transfer length register of DMA channel */
#define DA1470X_DMA_DMA0_CTRL_OFFSET 0x0010      /* Control register of DMA channel 0 */
#define DA1470X_DMA_DMA0_IDX_OFFSET 0x0014       /* Index pointer register of DMA channel 0 */
#define DA1470X_DMA_DMA1_A_START_OFFSET 0x0020   /* Source address register of DMA channel 1 */
#define DA1470X_DMA_DMA1_B_START_OFFSET 0x0024   /* Destination address register of DMA chan */
#define DA1470X_DMA_DMA1_INT_OFFSET 0x0028       /* Interrupt length register of DMA channel */
#define DA1470X_DMA_DMA1_LEN_OFFSET 0x002c       /* Transfer length register of DMA channel */
#define DA1470X_DMA_DMA1_CTRL_OFFSET 0x0030      /* Control register of DMA channel 1 */
#define DA1470X_DMA_DMA1_IDX_OFFSET 0x0034       /* Index pointer register of DMA channel 1 */
#define DA1470X_DMA_DMA2_A_START_OFFSET 0x0040   /* Source address register of DMA channel 2 */
#define DA1470X_DMA_DMA2_B_START_OFFSET 0x0044   /* Destination address register of DMA chan */
#define DA1470X_DMA_DMA2_INT_OFFSET 0x0048       /* Interrupt length register of DMA channel */
#define DA1470X_DMA_DMA2_LEN_OFFSET 0x004c       /* Transfer length register of DMA channel */
#define DA1470X_DMA_DMA2_CTRL_OFFSET 0x0050      /* Control register of DMA channel 2 */
#define DA1470X_DMA_DMA2_IDX_OFFSET 0x0054       /* Index pointer register of DMA channel 2 */
#define DA1470X_DMA_DMA3_A_START_OFFSET 0x0060   /* Source address register of DMA channel 3 */
#define DA1470X_DMA_DMA3_B_START_OFFSET 0x0064   /* Destination address register of DMA chan */
#define DA1470X_DMA_DMA3_INT_OFFSET 0x0068       /* Interrupt length register of DMA channel */
#define DA1470X_DMA_DMA3_LEN_OFFSET 0x006c       /* Transfer length register of DMA channel */
#define DA1470X_DMA_DMA3_CTRL_OFFSET 0x0070      /* Control register of DMA channel 3 */
#define DA1470X_DMA_DMA3_IDX_OFFSET 0x0074       /* Index pointer register of DMA channel 3 */
#define DA1470X_DMA_DMA4_A_START_OFFSET 0x0080   /* Source address register of DMA channel 4 */
#define DA1470X_DMA_DMA4_B_START_OFFSET 0x0084   /* Destination address register of DMA chan */
#define DA1470X_DMA_DMA4_INT_OFFSET 0x0088       /* Interrupt length register of DMA channel */
#define DA1470X_DMA_DMA4_LEN_OFFSET 0x008c       /* Transfer length register of DMA channel */
#define DA1470X_DMA_DMA4_CTRL_OFFSET 0x0090      /* Control register of DMA channel 4 */
#define DA1470X_DMA_DMA4_IDX_OFFSET 0x0094       /* Index pointer register of DMA channel 4 */
#define DA1470X_DMA_DMA5_A_START_OFFSET 0x00a0   /* Source address register of DMA channel 5 */
#define DA1470X_DMA_DMA5_B_START_OFFSET 0x00a4   /* Destination address register of DMA chan */
#define DA1470X_DMA_DMA5_INT_OFFSET 0x00a8       /* Interrupt length register of DMA channel */
#define DA1470X_DMA_DMA5_LEN_OFFSET 0x00ac       /* Transfer length register of DMA channel */
#define DA1470X_DMA_DMA5_CTRL_OFFSET 0x00b0      /* Control register of DMA channel 5 */
#define DA1470X_DMA_DMA5_IDX_OFFSET 0x00b4       /* Index pointer register of DMA channel 5 */
#define DA1470X_DMA_DMA6_A_START_OFFSET 0x00c0   /* Source address register of DMA channel 6 */
#define DA1470X_DMA_DMA6_B_START_OFFSET 0x00c4   /* Destination address register of DMA chan */
#define DA1470X_DMA_DMA6_INT_OFFSET 0x00c8       /* Interrupt length register of DMA channel */
#define DA1470X_DMA_DMA6_LEN_OFFSET 0x00cc       /* Transfer length register of DMA channel */
#define DA1470X_DMA_DMA6_CTRL_OFFSET 0x00d0      /* Control register of DMA channel 6 */
#define DA1470X_DMA_DMA6_IDX_OFFSET 0x00d4       /* Index pointer register of DMA channel 6 */
#define DA1470X_DMA_DMA7_A_START_OFFSET 0x00e0   /* Source address register of DMA channel 7 */
#define DA1470X_DMA_DMA7_B_START_OFFSET 0x00e4   /* Destination address register of DMA chan */
#define DA1470X_DMA_DMA7_INT_OFFSET 0x00e8       /* Interrupt length register of DMA channel */
#define DA1470X_DMA_DMA7_LEN_OFFSET 0x00ec       /* Transfer length register of DMA channel */
#define DA1470X_DMA_DMA7_CTRL_OFFSET 0x00f0      /* Control register of DMA channel 7 */
#define DA1470X_DMA_DMA7_IDX_OFFSET 0x00f4       /* Index pointer register of DMA channel 7 */
#define DA1470X_DMA_REQ_MUX_OFFSET 0x0100        /* DMA channels peripherals mapping registe */
#define DA1470X_DMA_INT_STATUS_OFFSET 0x0104     /* DMA Interrupt status register */
#define DA1470X_DMA_CLEAR_INT_OFFSET 0x0108      /* DMA Interrupt clear register */
#define DA1470X_DMA_INT_MASK_OFFSET 0x010c       /* DMA Interrupt mask register */
#define DA1470X_DMA_SET_INT_MASK_OFFSET 0x0110   /* DMA Set Interrupt mask register */
#define DA1470X_DMA_RESET_INT_MASK_OFFSET 0x0114 /* DMA Reset Interrupt mask register */

/* Register Addresses *******************************************************/

#define DA1470X_DMA_DMA0_A_START (DA1470X_DMA_BASE + DA1470X_DMA_DMA0_A_START_OFFSET)
#define DA1470X_DMA_DMA0_B_START (DA1470X_DMA_BASE + DA1470X_DMA_DMA0_B_START_OFFSET)
#define DA1470X_DMA_DMA0_INT (DA1470X_DMA_BASE + DA1470X_DMA_DMA0_INT_OFFSET)
#define DA1470X_DMA_DMA0_LEN (DA1470X_DMA_BASE + DA1470X_DMA_DMA0_LEN_OFFSET)
#define DA1470X_DMA_DMA0_CTRL (DA1470X_DMA_BASE + DA1470X_DMA_DMA0_CTRL_OFFSET)
#define DA1470X_DMA_DMA0_IDX (DA1470X_DMA_BASE + DA1470X_DMA_DMA0_IDX_OFFSET)
#define DA1470X_DMA_DMA1_A_START (DA1470X_DMA_BASE + DA1470X_DMA_DMA1_A_START_OFFSET)
#define DA1470X_DMA_DMA1_B_START (DA1470X_DMA_BASE + DA1470X_DMA_DMA1_B_START_OFFSET)
#define DA1470X_DMA_DMA1_INT (DA1470X_DMA_BASE + DA1470X_DMA_DMA1_INT_OFFSET)
#define DA1470X_DMA_DMA1_LEN (DA1470X_DMA_BASE + DA1470X_DMA_DMA1_LEN_OFFSET)
#define DA1470X_DMA_DMA1_CTRL (DA1470X_DMA_BASE + DA1470X_DMA_DMA1_CTRL_OFFSET)
#define DA1470X_DMA_DMA1_IDX (DA1470X_DMA_BASE + DA1470X_DMA_DMA1_IDX_OFFSET)
#define DA1470X_DMA_DMA2_A_START (DA1470X_DMA_BASE + DA1470X_DMA_DMA2_A_START_OFFSET)
#define DA1470X_DMA_DMA2_B_START (DA1470X_DMA_BASE + DA1470X_DMA_DMA2_B_START_OFFSET)
#define DA1470X_DMA_DMA2_INT (DA1470X_DMA_BASE + DA1470X_DMA_DMA2_INT_OFFSET)
#define DA1470X_DMA_DMA2_LEN (DA1470X_DMA_BASE + DA1470X_DMA_DMA2_LEN_OFFSET)
#define DA1470X_DMA_DMA2_CTRL (DA1470X_DMA_BASE + DA1470X_DMA_DMA2_CTRL_OFFSET)
#define DA1470X_DMA_DMA2_IDX (DA1470X_DMA_BASE + DA1470X_DMA_DMA2_IDX_OFFSET)
#define DA1470X_DMA_DMA3_A_START (DA1470X_DMA_BASE + DA1470X_DMA_DMA3_A_START_OFFSET)
#define DA1470X_DMA_DMA3_B_START (DA1470X_DMA_BASE + DA1470X_DMA_DMA3_B_START_OFFSET)
#define DA1470X_DMA_DMA3_INT (DA1470X_DMA_BASE + DA1470X_DMA_DMA3_INT_OFFSET)
#define DA1470X_DMA_DMA3_LEN (DA1470X_DMA_BASE + DA1470X_DMA_DMA3_LEN_OFFSET)
#define DA1470X_DMA_DMA3_CTRL (DA1470X_DMA_BASE + DA1470X_DMA_DMA3_CTRL_OFFSET)
#define DA1470X_DMA_DMA3_IDX (DA1470X_DMA_BASE + DA1470X_DMA_DMA3_IDX_OFFSET)
#define DA1470X_DMA_DMA4_A_START (DA1470X_DMA_BASE + DA1470X_DMA_DMA4_A_START_OFFSET)
#define DA1470X_DMA_DMA4_B_START (DA1470X_DMA_BASE + DA1470X_DMA_DMA4_B_START_OFFSET)
#define DA1470X_DMA_DMA4_INT (DA1470X_DMA_BASE + DA1470X_DMA_DMA4_INT_OFFSET)
#define DA1470X_DMA_DMA4_LEN (DA1470X_DMA_BASE + DA1470X_DMA_DMA4_LEN_OFFSET)
#define DA1470X_DMA_DMA4_CTRL (DA1470X_DMA_BASE + DA1470X_DMA_DMA4_CTRL_OFFSET)
#define DA1470X_DMA_DMA4_IDX (DA1470X_DMA_BASE + DA1470X_DMA_DMA4_IDX_OFFSET)
#define DA1470X_DMA_DMA5_A_START (DA1470X_DMA_BASE + DA1470X_DMA_DMA5_A_START_OFFSET)
#define DA1470X_DMA_DMA5_B_START (DA1470X_DMA_BASE + DA1470X_DMA_DMA5_B_START_OFFSET)
#define DA1470X_DMA_DMA5_INT (DA1470X_DMA_BASE + DA1470X_DMA_DMA5_INT_OFFSET)
#define DA1470X_DMA_DMA5_LEN (DA1470X_DMA_BASE + DA1470X_DMA_DMA5_LEN_OFFSET)
#define DA1470X_DMA_DMA5_CTRL (DA1470X_DMA_BASE + DA1470X_DMA_DMA5_CTRL_OFFSET)
#define DA1470X_DMA_DMA5_IDX (DA1470X_DMA_BASE + DA1470X_DMA_DMA5_IDX_OFFSET)
#define DA1470X_DMA_DMA6_A_START (DA1470X_DMA_BASE + DA1470X_DMA_DMA6_A_START_OFFSET)
#define DA1470X_DMA_DMA6_B_START (DA1470X_DMA_BASE + DA1470X_DMA_DMA6_B_START_OFFSET)
#define DA1470X_DMA_DMA6_INT (DA1470X_DMA_BASE + DA1470X_DMA_DMA6_INT_OFFSET)
#define DA1470X_DMA_DMA6_LEN (DA1470X_DMA_BASE + DA1470X_DMA_DMA6_LEN_OFFSET)
#define DA1470X_DMA_DMA6_CTRL (DA1470X_DMA_BASE + DA1470X_DMA_DMA6_CTRL_OFFSET)
#define DA1470X_DMA_DMA6_IDX (DA1470X_DMA_BASE + DA1470X_DMA_DMA6_IDX_OFFSET)
#define DA1470X_DMA_DMA7_A_START (DA1470X_DMA_BASE + DA1470X_DMA_DMA7_A_START_OFFSET)
#define DA1470X_DMA_DMA7_B_START (DA1470X_DMA_BASE + DA1470X_DMA_DMA7_B_START_OFFSET)
#define DA1470X_DMA_DMA7_INT (DA1470X_DMA_BASE + DA1470X_DMA_DMA7_INT_OFFSET)
#define DA1470X_DMA_DMA7_LEN (DA1470X_DMA_BASE + DA1470X_DMA_DMA7_LEN_OFFSET)
#define DA1470X_DMA_DMA7_CTRL (DA1470X_DMA_BASE + DA1470X_DMA_DMA7_CTRL_OFFSET)
#define DA1470X_DMA_DMA7_IDX (DA1470X_DMA_BASE + DA1470X_DMA_DMA7_IDX_OFFSET)
#define DA1470X_DMA_REQ_MUX (DA1470X_DMA_BASE + DA1470X_DMA_REQ_MUX_OFFSET)
#define DA1470X_DMA_INT_STATUS (DA1470X_DMA_BASE + DA1470X_DMA_INT_STATUS_OFFSET)
#define DA1470X_DMA_CLEAR_INT (DA1470X_DMA_BASE + DA1470X_DMA_CLEAR_INT_OFFSET)
#define DA1470X_DMA_INT_MASK (DA1470X_DMA_BASE + DA1470X_DMA_INT_MASK_OFFSET)
#define DA1470X_DMA_SET_INT_MASK (DA1470X_DMA_BASE + DA1470X_DMA_SET_INT_MASK_OFFSET)
#define DA1470X_DMA_RESET_INT_MASK (DA1470X_DMA_BASE + DA1470X_DMA_RESET_INT_MASK_OFFSET)

/* Register Bit-field Definitions *******************************************/

/* DMA0_A_START register */

#define DMA_DMA0_A_START_DMA0_A_START_SHIFT (0) /* Bits 0-31 */
#define DMA_DMA0_A_START_DMA0_A_START_MASK (0xffffffff << DMA_DMA0_A_START_DMA0_A_START_SHIFT)
#  define DMA_DMA0_A_START_DMA0_A_START(n) ((uint32_t)(n) << DMA_DMA0_A_START_DMA0_A_START_SHIFT)

/* DMA0_B_START register */

#define DMA_DMA0_B_START_DMA0_B_START_SHIFT (0) /* Bits 0-31 */
#define DMA_DMA0_B_START_DMA0_B_START_MASK (0xffffffff << DMA_DMA0_B_START_DMA0_B_START_SHIFT)
#  define DMA_DMA0_B_START_DMA0_B_START(n) ((uint32_t)(n) << DMA_DMA0_B_START_DMA0_B_START_SHIFT)

/* DMA0_INT register */

#define DMA_DMA0_INT_DMA0_INT_SHIFT (0) /* Bits 0-15 */
#define DMA_DMA0_INT_DMA0_INT_MASK (0xffff << DMA_DMA0_INT_DMA0_INT_SHIFT)
#  define DMA_DMA0_INT_DMA0_INT(n) ((uint32_t)(n) << DMA_DMA0_INT_DMA0_INT_SHIFT)

/* DMA0_LEN register */

#define DMA_DMA0_LEN_DMA0_LEN_SHIFT (0) /* Bits 0-15 */
#define DMA_DMA0_LEN_DMA0_LEN_MASK (0xffff << DMA_DMA0_LEN_DMA0_LEN_SHIFT)
#  define DMA_DMA0_LEN_DMA0_LEN(n) ((uint32_t)(n) << DMA_DMA0_LEN_DMA0_LEN_SHIFT)

/* DMA0_CTRL register */

#define DMA_DMA0_CTRL_ON (1 << 0)                /* Bit 0 */
#define DMA_DMA0_CTRL_BW_SHIFT (1)               /* Bits 1-2 */
#define DMA_DMA0_CTRL_BW_MASK (0x3 << DMA_DMA0_CTRL_BW_SHIFT)
#  define DMA_DMA0_CTRL_BW(n) ((uint32_t)(n) << DMA_DMA0_CTRL_BW_SHIFT)
#define DMA_DMA0_CTRL_DREQ_MODE (1 << 3)         /* Bit 3 */
#define DMA_DMA0_CTRL_BINC (1 << 4)              /* Bit 4 */
#define DMA_DMA0_CTRL_AINC (1 << 5)              /* Bit 5 */
#define DMA_DMA0_CTRL_CIRCULAR (1 << 6)          /* Bit 6 */
#define DMA_DMA0_CTRL_PRIO_SHIFT (7)             /* Bits 7-9 */
#define DMA_DMA0_CTRL_PRIO_MASK (0x7 << DMA_DMA0_CTRL_PRIO_SHIFT)
#  define DMA_DMA0_CTRL_PRIO(n) ((uint32_t)(n) << DMA_DMA0_CTRL_PRIO_SHIFT)
#define DMA_DMA0_CTRL_IDLE (1 << 10)             /* Bit 10 */
#define DMA_DMA0_CTRL_INIT (1 << 11)             /* Bit 11 */
#define DMA_DMA0_CTRL_REQ_SENSE (1 << 12)        /* Bit 12 */
#define DMA_DMA0_CTRL_BURST_MODE_SHIFT (13)      /* Bits 13-14 */
#define DMA_DMA0_CTRL_BURST_MODE_MASK (0x3 << DMA_DMA0_CTRL_BURST_MODE_SHIFT)
#  define DMA_DMA0_CTRL_BURST_MODE(n) ((uint32_t)(n) << DMA_DMA0_CTRL_BURST_MODE_SHIFT)
#define DMA_DMA0_CTRL_BUS_ERROR_DETECT (1 << 15) /* Bit 15 */
#define DMA_DMA0_CTRL_EXCLUSIVE_ACCESS (1 << 16) /* Bit 16 */

/* DMA0_IDX register */

#define DMA_DMA0_IDX_DMA0_IDX_SHIFT (0) /* Bits 0-15 */
#define DMA_DMA0_IDX_DMA0_IDX_MASK (0xffff << DMA_DMA0_IDX_DMA0_IDX_SHIFT)
#  define DMA_DMA0_IDX_DMA0_IDX(n) ((uint32_t)(n) << DMA_DMA0_IDX_DMA0_IDX_SHIFT)

/* DMA1_A_START register */

#define DMA_DMA1_A_START_DMA1_A_START_SHIFT (0) /* Bits 0-31 */
#define DMA_DMA1_A_START_DMA1_A_START_MASK (0xffffffff << DMA_DMA1_A_START_DMA1_A_START_SHIFT)
#  define DMA_DMA1_A_START_DMA1_A_START(n) ((uint32_t)(n) << DMA_DMA1_A_START_DMA1_A_START_SHIFT)

/* DMA1_B_START register */

#define DMA_DMA1_B_START_DMA1_B_START_SHIFT (0) /* Bits 0-31 */
#define DMA_DMA1_B_START_DMA1_B_START_MASK (0xffffffff << DMA_DMA1_B_START_DMA1_B_START_SHIFT)
#  define DMA_DMA1_B_START_DMA1_B_START(n) ((uint32_t)(n) << DMA_DMA1_B_START_DMA1_B_START_SHIFT)

/* DMA1_INT register */

#define DMA_DMA1_INT_DMA1_INT_SHIFT (0) /* Bits 0-15 */
#define DMA_DMA1_INT_DMA1_INT_MASK (0xffff << DMA_DMA1_INT_DMA1_INT_SHIFT)
#  define DMA_DMA1_INT_DMA1_INT(n) ((uint32_t)(n) << DMA_DMA1_INT_DMA1_INT_SHIFT)

/* DMA1_LEN register */

#define DMA_DMA1_LEN_DMA1_LEN_SHIFT (0) /* Bits 0-15 */
#define DMA_DMA1_LEN_DMA1_LEN_MASK (0xffff << DMA_DMA1_LEN_DMA1_LEN_SHIFT)
#  define DMA_DMA1_LEN_DMA1_LEN(n) ((uint32_t)(n) << DMA_DMA1_LEN_DMA1_LEN_SHIFT)

/* DMA1_CTRL register */

#define DMA_DMA1_CTRL_ON (1 << 0)                /* Bit 0 */
#define DMA_DMA1_CTRL_BW_SHIFT (1)               /* Bits 1-2 */
#define DMA_DMA1_CTRL_BW_MASK (0x3 << DMA_DMA1_CTRL_BW_SHIFT)
#  define DMA_DMA1_CTRL_BW(n) ((uint32_t)(n) << DMA_DMA1_CTRL_BW_SHIFT)
#define DMA_DMA1_CTRL_DREQ_MODE (1 << 3)         /* Bit 3 */
#define DMA_DMA1_CTRL_BINC (1 << 4)              /* Bit 4 */
#define DMA_DMA1_CTRL_AINC (1 << 5)              /* Bit 5 */
#define DMA_DMA1_CTRL_CIRCULAR (1 << 6)          /* Bit 6 */
#define DMA_DMA1_CTRL_PRIO_SHIFT (7)             /* Bits 7-9 */
#define DMA_DMA1_CTRL_PRIO_MASK (0x7 << DMA_DMA1_CTRL_PRIO_SHIFT)
#  define DMA_DMA1_CTRL_PRIO(n) ((uint32_t)(n) << DMA_DMA1_CTRL_PRIO_SHIFT)
#define DMA_DMA1_CTRL_IDLE (1 << 10)             /* Bit 10 */
#define DMA_DMA1_CTRL_INIT (1 << 11)             /* Bit 11 */
#define DMA_DMA1_CTRL_REQ_SENSE (1 << 12)        /* Bit 12 */
#define DMA_DMA1_CTRL_BURST_MODE_SHIFT (13)      /* Bits 13-14 */
#define DMA_DMA1_CTRL_BURST_MODE_MASK (0x3 << DMA_DMA1_CTRL_BURST_MODE_SHIFT)
#  define DMA_DMA1_CTRL_BURST_MODE(n) ((uint32_t)(n) << DMA_DMA1_CTRL_BURST_MODE_SHIFT)
#define DMA_DMA1_CTRL_BUS_ERROR_DETECT (1 << 15) /* Bit 15 */
#define DMA_DMA1_CTRL_EXCLUSIVE_ACCESS (1 << 16) /* Bit 16 */

/* DMA1_IDX register */

#define DMA_DMA1_IDX_DMA1_IDX_SHIFT (0) /* Bits 0-15 */
#define DMA_DMA1_IDX_DMA1_IDX_MASK (0xffff << DMA_DMA1_IDX_DMA1_IDX_SHIFT)
#  define DMA_DMA1_IDX_DMA1_IDX(n) ((uint32_t)(n) << DMA_DMA1_IDX_DMA1_IDX_SHIFT)

/* DMA2_A_START register */

#define DMA_DMA2_A_START_DMA2_A_START_SHIFT (0) /* Bits 0-31 */
#define DMA_DMA2_A_START_DMA2_A_START_MASK (0xffffffff << DMA_DMA2_A_START_DMA2_A_START_SHIFT)
#  define DMA_DMA2_A_START_DMA2_A_START(n) ((uint32_t)(n) << DMA_DMA2_A_START_DMA2_A_START_SHIFT)

/* DMA2_B_START register */

#define DMA_DMA2_B_START_DMA2_B_START_SHIFT (0) /* Bits 0-31 */
#define DMA_DMA2_B_START_DMA2_B_START_MASK (0xffffffff << DMA_DMA2_B_START_DMA2_B_START_SHIFT)
#  define DMA_DMA2_B_START_DMA2_B_START(n) ((uint32_t)(n) << DMA_DMA2_B_START_DMA2_B_START_SHIFT)

/* DMA2_INT register */

#define DMA_DMA2_INT_DMA2_INT_SHIFT (0) /* Bits 0-15 */
#define DMA_DMA2_INT_DMA2_INT_MASK (0xffff << DMA_DMA2_INT_DMA2_INT_SHIFT)
#  define DMA_DMA2_INT_DMA2_INT(n) ((uint32_t)(n) << DMA_DMA2_INT_DMA2_INT_SHIFT)

/* DMA2_LEN register */

#define DMA_DMA2_LEN_DMA2_LEN_SHIFT (0) /* Bits 0-15 */
#define DMA_DMA2_LEN_DMA2_LEN_MASK (0xffff << DMA_DMA2_LEN_DMA2_LEN_SHIFT)
#  define DMA_DMA2_LEN_DMA2_LEN(n) ((uint32_t)(n) << DMA_DMA2_LEN_DMA2_LEN_SHIFT)

/* DMA2_CTRL register */

#define DMA_DMA2_CTRL_ON (1 << 0)                /* Bit 0 */
#define DMA_DMA2_CTRL_BW_SHIFT (1)               /* Bits 1-2 */
#define DMA_DMA2_CTRL_BW_MASK (0x3 << DMA_DMA2_CTRL_BW_SHIFT)
#  define DMA_DMA2_CTRL_BW(n) ((uint32_t)(n) << DMA_DMA2_CTRL_BW_SHIFT)
#define DMA_DMA2_CTRL_DREQ_MODE (1 << 3)         /* Bit 3 */
#define DMA_DMA2_CTRL_BINC (1 << 4)              /* Bit 4 */
#define DMA_DMA2_CTRL_AINC (1 << 5)              /* Bit 5 */
#define DMA_DMA2_CTRL_CIRCULAR (1 << 6)          /* Bit 6 */
#define DMA_DMA2_CTRL_PRIO_SHIFT (7)             /* Bits 7-9 */
#define DMA_DMA2_CTRL_PRIO_MASK (0x7 << DMA_DMA2_CTRL_PRIO_SHIFT)
#  define DMA_DMA2_CTRL_PRIO(n) ((uint32_t)(n) << DMA_DMA2_CTRL_PRIO_SHIFT)
#define DMA_DMA2_CTRL_IDLE (1 << 10)             /* Bit 10 */
#define DMA_DMA2_CTRL_INIT (1 << 11)             /* Bit 11 */
#define DMA_DMA2_CTRL_REQ_SENSE (1 << 12)        /* Bit 12 */
#define DMA_DMA2_CTRL_BURST_MODE_SHIFT (13)      /* Bits 13-14 */
#define DMA_DMA2_CTRL_BURST_MODE_MASK (0x3 << DMA_DMA2_CTRL_BURST_MODE_SHIFT)
#  define DMA_DMA2_CTRL_BURST_MODE(n) ((uint32_t)(n) << DMA_DMA2_CTRL_BURST_MODE_SHIFT)
#define DMA_DMA2_CTRL_BUS_ERROR_DETECT (1 << 15) /* Bit 15 */
#define DMA_DMA2_CTRL_EXCLUSIVE_ACCESS (1 << 16) /* Bit 16 */

/* DMA2_IDX register */

#define DMA_DMA2_IDX_DMA2_IDX_SHIFT (0) /* Bits 0-15 */
#define DMA_DMA2_IDX_DMA2_IDX_MASK (0xffff << DMA_DMA2_IDX_DMA2_IDX_SHIFT)
#  define DMA_DMA2_IDX_DMA2_IDX(n) ((uint32_t)(n) << DMA_DMA2_IDX_DMA2_IDX_SHIFT)

/* DMA3_A_START register */

#define DMA_DMA3_A_START_DMA3_A_START_SHIFT (0) /* Bits 0-31 */
#define DMA_DMA3_A_START_DMA3_A_START_MASK (0xffffffff << DMA_DMA3_A_START_DMA3_A_START_SHIFT)
#  define DMA_DMA3_A_START_DMA3_A_START(n) ((uint32_t)(n) << DMA_DMA3_A_START_DMA3_A_START_SHIFT)

/* DMA3_B_START register */

#define DMA_DMA3_B_START_DMA3_B_START_SHIFT (0) /* Bits 0-31 */
#define DMA_DMA3_B_START_DMA3_B_START_MASK (0xffffffff << DMA_DMA3_B_START_DMA3_B_START_SHIFT)
#  define DMA_DMA3_B_START_DMA3_B_START(n) ((uint32_t)(n) << DMA_DMA3_B_START_DMA3_B_START_SHIFT)

/* DMA3_INT register */

#define DMA_DMA3_INT_DMA3_INT_SHIFT (0) /* Bits 0-15 */
#define DMA_DMA3_INT_DMA3_INT_MASK (0xffff << DMA_DMA3_INT_DMA3_INT_SHIFT)
#  define DMA_DMA3_INT_DMA3_INT(n) ((uint32_t)(n) << DMA_DMA3_INT_DMA3_INT_SHIFT)

/* DMA3_LEN register */

#define DMA_DMA3_LEN_DMA3_LEN_SHIFT (0) /* Bits 0-15 */
#define DMA_DMA3_LEN_DMA3_LEN_MASK (0xffff << DMA_DMA3_LEN_DMA3_LEN_SHIFT)
#  define DMA_DMA3_LEN_DMA3_LEN(n) ((uint32_t)(n) << DMA_DMA3_LEN_DMA3_LEN_SHIFT)

/* DMA3_CTRL register */

#define DMA_DMA3_CTRL_ON (1 << 0)                /* Bit 0 */
#define DMA_DMA3_CTRL_BW_SHIFT (1)               /* Bits 1-2 */
#define DMA_DMA3_CTRL_BW_MASK (0x3 << DMA_DMA3_CTRL_BW_SHIFT)
#  define DMA_DMA3_CTRL_BW(n) ((uint32_t)(n) << DMA_DMA3_CTRL_BW_SHIFT)
#define DMA_DMA3_CTRL_DREQ_MODE (1 << 3)         /* Bit 3 */
#define DMA_DMA3_CTRL_BINC (1 << 4)              /* Bit 4 */
#define DMA_DMA3_CTRL_AINC (1 << 5)              /* Bit 5 */
#define DMA_DMA3_CTRL_CIRCULAR (1 << 6)          /* Bit 6 */
#define DMA_DMA3_CTRL_PRIO_SHIFT (7)             /* Bits 7-9 */
#define DMA_DMA3_CTRL_PRIO_MASK (0x7 << DMA_DMA3_CTRL_PRIO_SHIFT)
#  define DMA_DMA3_CTRL_PRIO(n) ((uint32_t)(n) << DMA_DMA3_CTRL_PRIO_SHIFT)
#define DMA_DMA3_CTRL_IDLE (1 << 10)             /* Bit 10 */
#define DMA_DMA3_CTRL_INIT (1 << 11)             /* Bit 11 */
#define DMA_DMA3_CTRL_REQ_SENSE (1 << 12)        /* Bit 12 */
#define DMA_DMA3_CTRL_BURST_MODE_SHIFT (13)      /* Bits 13-14 */
#define DMA_DMA3_CTRL_BURST_MODE_MASK (0x3 << DMA_DMA3_CTRL_BURST_MODE_SHIFT)
#  define DMA_DMA3_CTRL_BURST_MODE(n) ((uint32_t)(n) << DMA_DMA3_CTRL_BURST_MODE_SHIFT)
#define DMA_DMA3_CTRL_BUS_ERROR_DETECT (1 << 15) /* Bit 15 */
#define DMA_DMA3_CTRL_EXCLUSIVE_ACCESS (1 << 16) /* Bit 16 */

/* DMA3_IDX register */

#define DMA_DMA3_IDX_DMA3_IDX_SHIFT (0) /* Bits 0-15 */
#define DMA_DMA3_IDX_DMA3_IDX_MASK (0xffff << DMA_DMA3_IDX_DMA3_IDX_SHIFT)
#  define DMA_DMA3_IDX_DMA3_IDX(n) ((uint32_t)(n) << DMA_DMA3_IDX_DMA3_IDX_SHIFT)

/* DMA4_A_START register */

#define DMA_DMA4_A_START_DMA4_A_START_SHIFT (0) /* Bits 0-31 */
#define DMA_DMA4_A_START_DMA4_A_START_MASK (0xffffffff << DMA_DMA4_A_START_DMA4_A_START_SHIFT)
#  define DMA_DMA4_A_START_DMA4_A_START(n) ((uint32_t)(n) << DMA_DMA4_A_START_DMA4_A_START_SHIFT)

/* DMA4_B_START register */

#define DMA_DMA4_B_START_DMA4_B_START_SHIFT (0) /* Bits 0-31 */
#define DMA_DMA4_B_START_DMA4_B_START_MASK (0xffffffff << DMA_DMA4_B_START_DMA4_B_START_SHIFT)
#  define DMA_DMA4_B_START_DMA4_B_START(n) ((uint32_t)(n) << DMA_DMA4_B_START_DMA4_B_START_SHIFT)

/* DMA4_INT register */

#define DMA_DMA4_INT_DMA4_INT_SHIFT (0) /* Bits 0-15 */
#define DMA_DMA4_INT_DMA4_INT_MASK (0xffff << DMA_DMA4_INT_DMA4_INT_SHIFT)
#  define DMA_DMA4_INT_DMA4_INT(n) ((uint32_t)(n) << DMA_DMA4_INT_DMA4_INT_SHIFT)

/* DMA4_LEN register */

#define DMA_DMA4_LEN_DMA4_LEN_SHIFT (0) /* Bits 0-15 */
#define DMA_DMA4_LEN_DMA4_LEN_MASK (0xffff << DMA_DMA4_LEN_DMA4_LEN_SHIFT)
#  define DMA_DMA4_LEN_DMA4_LEN(n) ((uint32_t)(n) << DMA_DMA4_LEN_DMA4_LEN_SHIFT)

/* DMA4_CTRL register */

#define DMA_DMA4_CTRL_ON (1 << 0)                /* Bit 0 */
#define DMA_DMA4_CTRL_BW_SHIFT (1)               /* Bits 1-2 */
#define DMA_DMA4_CTRL_BW_MASK (0x3 << DMA_DMA4_CTRL_BW_SHIFT)
#  define DMA_DMA4_CTRL_BW(n) ((uint32_t)(n) << DMA_DMA4_CTRL_BW_SHIFT)
#define DMA_DMA4_CTRL_DREQ_MODE (1 << 3)         /* Bit 3 */
#define DMA_DMA4_CTRL_BINC (1 << 4)              /* Bit 4 */
#define DMA_DMA4_CTRL_AINC (1 << 5)              /* Bit 5 */
#define DMA_DMA4_CTRL_CIRCULAR (1 << 6)          /* Bit 6 */
#define DMA_DMA4_CTRL_PRIO_SHIFT (7)             /* Bits 7-9 */
#define DMA_DMA4_CTRL_PRIO_MASK (0x7 << DMA_DMA4_CTRL_PRIO_SHIFT)
#  define DMA_DMA4_CTRL_PRIO(n) ((uint32_t)(n) << DMA_DMA4_CTRL_PRIO_SHIFT)
#define DMA_DMA4_CTRL_IDLE (1 << 10)             /* Bit 10 */
#define DMA_DMA4_CTRL_INIT (1 << 11)             /* Bit 11 */
#define DMA_DMA4_CTRL_REQ_SENSE (1 << 12)        /* Bit 12 */
#define DMA_DMA4_CTRL_BURST_MODE_SHIFT (13)      /* Bits 13-14 */
#define DMA_DMA4_CTRL_BURST_MODE_MASK (0x3 << DMA_DMA4_CTRL_BURST_MODE_SHIFT)
#  define DMA_DMA4_CTRL_BURST_MODE(n) ((uint32_t)(n) << DMA_DMA4_CTRL_BURST_MODE_SHIFT)
#define DMA_DMA4_CTRL_BUS_ERROR_DETECT (1 << 15) /* Bit 15 */
#define DMA_DMA4_CTRL_EXCLUSIVE_ACCESS (1 << 16) /* Bit 16 */

/* DMA4_IDX register */

#define DMA_DMA4_IDX_DMA4_IDX_SHIFT (0) /* Bits 0-15 */
#define DMA_DMA4_IDX_DMA4_IDX_MASK (0xffff << DMA_DMA4_IDX_DMA4_IDX_SHIFT)
#  define DMA_DMA4_IDX_DMA4_IDX(n) ((uint32_t)(n) << DMA_DMA4_IDX_DMA4_IDX_SHIFT)

/* DMA5_A_START register */

#define DMA_DMA5_A_START_DMA5_A_START_SHIFT (0) /* Bits 0-31 */
#define DMA_DMA5_A_START_DMA5_A_START_MASK (0xffffffff << DMA_DMA5_A_START_DMA5_A_START_SHIFT)
#  define DMA_DMA5_A_START_DMA5_A_START(n) ((uint32_t)(n) << DMA_DMA5_A_START_DMA5_A_START_SHIFT)

/* DMA5_B_START register */

#define DMA_DMA5_B_START_DMA5_B_START_SHIFT (0) /* Bits 0-31 */
#define DMA_DMA5_B_START_DMA5_B_START_MASK (0xffffffff << DMA_DMA5_B_START_DMA5_B_START_SHIFT)
#  define DMA_DMA5_B_START_DMA5_B_START(n) ((uint32_t)(n) << DMA_DMA5_B_START_DMA5_B_START_SHIFT)

/* DMA5_INT register */

#define DMA_DMA5_INT_DMA5_INT_SHIFT (0) /* Bits 0-15 */
#define DMA_DMA5_INT_DMA5_INT_MASK (0xffff << DMA_DMA5_INT_DMA5_INT_SHIFT)
#  define DMA_DMA5_INT_DMA5_INT(n) ((uint32_t)(n) << DMA_DMA5_INT_DMA5_INT_SHIFT)

/* DMA5_LEN register */

#define DMA_DMA5_LEN_DMA5_LEN_SHIFT (0) /* Bits 0-15 */
#define DMA_DMA5_LEN_DMA5_LEN_MASK (0xffff << DMA_DMA5_LEN_DMA5_LEN_SHIFT)
#  define DMA_DMA5_LEN_DMA5_LEN(n) ((uint32_t)(n) << DMA_DMA5_LEN_DMA5_LEN_SHIFT)

/* DMA5_CTRL register */

#define DMA_DMA5_CTRL_ON (1 << 0)                /* Bit 0 */
#define DMA_DMA5_CTRL_BW_SHIFT (1)               /* Bits 1-2 */
#define DMA_DMA5_CTRL_BW_MASK (0x3 << DMA_DMA5_CTRL_BW_SHIFT)
#  define DMA_DMA5_CTRL_BW(n) ((uint32_t)(n) << DMA_DMA5_CTRL_BW_SHIFT)
#define DMA_DMA5_CTRL_DREQ_MODE (1 << 3)         /* Bit 3 */
#define DMA_DMA5_CTRL_BINC (1 << 4)              /* Bit 4 */
#define DMA_DMA5_CTRL_AINC (1 << 5)              /* Bit 5 */
#define DMA_DMA5_CTRL_CIRCULAR (1 << 6)          /* Bit 6 */
#define DMA_DMA5_CTRL_PRIO_SHIFT (7)             /* Bits 7-9 */
#define DMA_DMA5_CTRL_PRIO_MASK (0x7 << DMA_DMA5_CTRL_PRIO_SHIFT)
#  define DMA_DMA5_CTRL_PRIO(n) ((uint32_t)(n) << DMA_DMA5_CTRL_PRIO_SHIFT)
#define DMA_DMA5_CTRL_IDLE (1 << 10)             /* Bit 10 */
#define DMA_DMA5_CTRL_INIT (1 << 11)             /* Bit 11 */
#define DMA_DMA5_CTRL_REQ_SENSE (1 << 12)        /* Bit 12 */
#define DMA_DMA5_CTRL_BURST_MODE_SHIFT (13)      /* Bits 13-14 */
#define DMA_DMA5_CTRL_BURST_MODE_MASK (0x3 << DMA_DMA5_CTRL_BURST_MODE_SHIFT)
#  define DMA_DMA5_CTRL_BURST_MODE(n) ((uint32_t)(n) << DMA_DMA5_CTRL_BURST_MODE_SHIFT)
#define DMA_DMA5_CTRL_BUS_ERROR_DETECT (1 << 15) /* Bit 15 */
#define DMA_DMA5_CTRL_EXCLUSIVE_ACCESS (1 << 16) /* Bit 16 */

/* DMA5_IDX register */

#define DMA_DMA5_IDX_DMA5_IDX_SHIFT (0) /* Bits 0-15 */
#define DMA_DMA5_IDX_DMA5_IDX_MASK (0xffff << DMA_DMA5_IDX_DMA5_IDX_SHIFT)
#  define DMA_DMA5_IDX_DMA5_IDX(n) ((uint32_t)(n) << DMA_DMA5_IDX_DMA5_IDX_SHIFT)

/* DMA6_A_START register */

#define DMA_DMA6_A_START_DMA6_A_START_SHIFT (0) /* Bits 0-31 */
#define DMA_DMA6_A_START_DMA6_A_START_MASK (0xffffffff << DMA_DMA6_A_START_DMA6_A_START_SHIFT)
#  define DMA_DMA6_A_START_DMA6_A_START(n) ((uint32_t)(n) << DMA_DMA6_A_START_DMA6_A_START_SHIFT)

/* DMA6_B_START register */

#define DMA_DMA6_B_START_DMA6_B_START_SHIFT (0) /* Bits 0-31 */
#define DMA_DMA6_B_START_DMA6_B_START_MASK (0xffffffff << DMA_DMA6_B_START_DMA6_B_START_SHIFT)
#  define DMA_DMA6_B_START_DMA6_B_START(n) ((uint32_t)(n) << DMA_DMA6_B_START_DMA6_B_START_SHIFT)

/* DMA6_INT register */

#define DMA_DMA6_INT_DMA6_INT_SHIFT (0) /* Bits 0-15 */
#define DMA_DMA6_INT_DMA6_INT_MASK (0xffff << DMA_DMA6_INT_DMA6_INT_SHIFT)
#  define DMA_DMA6_INT_DMA6_INT(n) ((uint32_t)(n) << DMA_DMA6_INT_DMA6_INT_SHIFT)

/* DMA6_LEN register */

#define DMA_DMA6_LEN_DMA6_LEN_SHIFT (0) /* Bits 0-15 */
#define DMA_DMA6_LEN_DMA6_LEN_MASK (0xffff << DMA_DMA6_LEN_DMA6_LEN_SHIFT)
#  define DMA_DMA6_LEN_DMA6_LEN(n) ((uint32_t)(n) << DMA_DMA6_LEN_DMA6_LEN_SHIFT)

/* DMA6_CTRL register */

#define DMA_DMA6_CTRL_ON (1 << 0)                /* Bit 0 */
#define DMA_DMA6_CTRL_BW_SHIFT (1)               /* Bits 1-2 */
#define DMA_DMA6_CTRL_BW_MASK (0x3 << DMA_DMA6_CTRL_BW_SHIFT)
#  define DMA_DMA6_CTRL_BW(n) ((uint32_t)(n) << DMA_DMA6_CTRL_BW_SHIFT)
#define DMA_DMA6_CTRL_DREQ_MODE (1 << 3)         /* Bit 3 */
#define DMA_DMA6_CTRL_BINC (1 << 4)              /* Bit 4 */
#define DMA_DMA6_CTRL_AINC (1 << 5)              /* Bit 5 */
#define DMA_DMA6_CTRL_CIRCULAR (1 << 6)          /* Bit 6 */
#define DMA_DMA6_CTRL_PRIO_SHIFT (7)             /* Bits 7-9 */
#define DMA_DMA6_CTRL_PRIO_MASK (0x7 << DMA_DMA6_CTRL_PRIO_SHIFT)
#  define DMA_DMA6_CTRL_PRIO(n) ((uint32_t)(n) << DMA_DMA6_CTRL_PRIO_SHIFT)
#define DMA_DMA6_CTRL_IDLE (1 << 10)             /* Bit 10 */
#define DMA_DMA6_CTRL_INIT (1 << 11)             /* Bit 11 */
#define DMA_DMA6_CTRL_REQ_SENSE (1 << 12)        /* Bit 12 */
#define DMA_DMA6_CTRL_BURST_MODE_SHIFT (13)      /* Bits 13-14 */
#define DMA_DMA6_CTRL_BURST_MODE_MASK (0x3 << DMA_DMA6_CTRL_BURST_MODE_SHIFT)
#  define DMA_DMA6_CTRL_BURST_MODE(n) ((uint32_t)(n) << DMA_DMA6_CTRL_BURST_MODE_SHIFT)
#define DMA_DMA6_CTRL_BUS_ERROR_DETECT (1 << 15) /* Bit 15 */
#define DMA_DMA6_CTRL_EXCLUSIVE_ACCESS (1 << 16) /* Bit 16 */

/* DMA6_IDX register */

#define DMA_DMA6_IDX_DMA6_IDX_SHIFT (0) /* Bits 0-15 */
#define DMA_DMA6_IDX_DMA6_IDX_MASK (0xffff << DMA_DMA6_IDX_DMA6_IDX_SHIFT)
#  define DMA_DMA6_IDX_DMA6_IDX(n) ((uint32_t)(n) << DMA_DMA6_IDX_DMA6_IDX_SHIFT)

/* DMA7_A_START register */

#define DMA_DMA7_A_START_DMA7_A_START_SHIFT (0) /* Bits 0-31 */
#define DMA_DMA7_A_START_DMA7_A_START_MASK (0xffffffff << DMA_DMA7_A_START_DMA7_A_START_SHIFT)
#  define DMA_DMA7_A_START_DMA7_A_START(n) ((uint32_t)(n) << DMA_DMA7_A_START_DMA7_A_START_SHIFT)

/* DMA7_B_START register */

#define DMA_DMA7_B_START_DMA7_B_START_SHIFT (0) /* Bits 0-31 */
#define DMA_DMA7_B_START_DMA7_B_START_MASK (0xffffffff << DMA_DMA7_B_START_DMA7_B_START_SHIFT)
#  define DMA_DMA7_B_START_DMA7_B_START(n) ((uint32_t)(n) << DMA_DMA7_B_START_DMA7_B_START_SHIFT)

/* DMA7_INT register */

#define DMA_DMA7_INT_DMA7_INT_SHIFT (0) /* Bits 0-15 */
#define DMA_DMA7_INT_DMA7_INT_MASK (0xffff << DMA_DMA7_INT_DMA7_INT_SHIFT)
#  define DMA_DMA7_INT_DMA7_INT(n) ((uint32_t)(n) << DMA_DMA7_INT_DMA7_INT_SHIFT)

/* DMA7_LEN register */

#define DMA_DMA7_LEN_DMA7_LEN_SHIFT (0) /* Bits 0-15 */
#define DMA_DMA7_LEN_DMA7_LEN_MASK (0xffff << DMA_DMA7_LEN_DMA7_LEN_SHIFT)
#  define DMA_DMA7_LEN_DMA7_LEN(n) ((uint32_t)(n) << DMA_DMA7_LEN_DMA7_LEN_SHIFT)

/* DMA7_CTRL register */

#define DMA_DMA7_CTRL_ON (1 << 0)                /* Bit 0 */
#define DMA_DMA7_CTRL_BW_SHIFT (1)               /* Bits 1-2 */
#define DMA_DMA7_CTRL_BW_MASK (0x3 << DMA_DMA7_CTRL_BW_SHIFT)
#  define DMA_DMA7_CTRL_BW(n) ((uint32_t)(n) << DMA_DMA7_CTRL_BW_SHIFT)
#define DMA_DMA7_CTRL_DREQ_MODE (1 << 3)         /* Bit 3 */
#define DMA_DMA7_CTRL_BINC (1 << 4)              /* Bit 4 */
#define DMA_DMA7_CTRL_AINC (1 << 5)              /* Bit 5 */
#define DMA_DMA7_CTRL_CIRCULAR (1 << 6)          /* Bit 6 */
#define DMA_DMA7_CTRL_PRIO_SHIFT (7)             /* Bits 7-9 */
#define DMA_DMA7_CTRL_PRIO_MASK (0x7 << DMA_DMA7_CTRL_PRIO_SHIFT)
#  define DMA_DMA7_CTRL_PRIO(n) ((uint32_t)(n) << DMA_DMA7_CTRL_PRIO_SHIFT)
#define DMA_DMA7_CTRL_IDLE (1 << 10)             /* Bit 10 */
#define DMA_DMA7_CTRL_INIT (1 << 11)             /* Bit 11 */
#define DMA_DMA7_CTRL_REQ_SENSE (1 << 12)        /* Bit 12 */
#define DMA_DMA7_CTRL_BURST_MODE_SHIFT (13)      /* Bits 13-14 */
#define DMA_DMA7_CTRL_BURST_MODE_MASK (0x3 << DMA_DMA7_CTRL_BURST_MODE_SHIFT)
#  define DMA_DMA7_CTRL_BURST_MODE(n) ((uint32_t)(n) << DMA_DMA7_CTRL_BURST_MODE_SHIFT)
#define DMA_DMA7_CTRL_BUS_ERROR_DETECT (1 << 15) /* Bit 15 */
#define DMA_DMA7_CTRL_EXCLUSIVE_ACCESS (1 << 16) /* Bit 16 */

/* DMA7_IDX register */

#define DMA_DMA7_IDX_DMA7_IDX_SHIFT (0) /* Bits 0-15 */
#define DMA_DMA7_IDX_DMA7_IDX_MASK (0xffff << DMA_DMA7_IDX_DMA7_IDX_SHIFT)
#  define DMA_DMA7_IDX_DMA7_IDX(n) ((uint32_t)(n) << DMA_DMA7_IDX_DMA7_IDX_SHIFT)

/* REQ_MUX register */

#define DMA_REQ_MUX_DMA01_SEL_SHIFT (0)  /* Bits 0-3 */
#define DMA_REQ_MUX_DMA01_SEL_MASK (0xf << DMA_REQ_MUX_DMA01_SEL_SHIFT)
#  define DMA_REQ_MUX_DMA01_SEL(n) ((uint32_t)(n) << DMA_REQ_MUX_DMA01_SEL_SHIFT)
#define DMA_REQ_MUX_DMA23_SEL_SHIFT (4)  /* Bits 4-7 */
#define DMA_REQ_MUX_DMA23_SEL_MASK (0xf << DMA_REQ_MUX_DMA23_SEL_SHIFT)
#  define DMA_REQ_MUX_DMA23_SEL(n) ((uint32_t)(n) << DMA_REQ_MUX_DMA23_SEL_SHIFT)
#define DMA_REQ_MUX_DMA45_SEL_SHIFT (8)  /* Bits 8-11 */
#define DMA_REQ_MUX_DMA45_SEL_MASK (0xf << DMA_REQ_MUX_DMA45_SEL_SHIFT)
#  define DMA_REQ_MUX_DMA45_SEL(n) ((uint32_t)(n) << DMA_REQ_MUX_DMA45_SEL_SHIFT)
#define DMA_REQ_MUX_DMA67_SEL_SHIFT (12) /* Bits 12-15 */
#define DMA_REQ_MUX_DMA67_SEL_MASK (0xf << DMA_REQ_MUX_DMA67_SEL_SHIFT)
#  define DMA_REQ_MUX_DMA67_SEL(n) ((uint32_t)(n) << DMA_REQ_MUX_DMA67_SEL_SHIFT)

/* INT_STATUS register */

#define DMA_INT_STATUS_IRQ_CH0 (1 << 0)   /* Bit 0 */
#define DMA_INT_STATUS_IRQ_CH1 (1 << 1)   /* Bit 1 */
#define DMA_INT_STATUS_IRQ_CH2 (1 << 2)   /* Bit 2 */
#define DMA_INT_STATUS_IRQ_CH3 (1 << 3)   /* Bit 3 */
#define DMA_INT_STATUS_IRQ_CH4 (1 << 4)   /* Bit 4 */
#define DMA_INT_STATUS_IRQ_CH5 (1 << 5)   /* Bit 5 */
#define DMA_INT_STATUS_IRQ_CH6 (1 << 6)   /* Bit 6 */
#define DMA_INT_STATUS_IRQ_CH7 (1 << 7)   /* Bit 7 */
#define DMA_INT_STATUS_BUS_ERR0 (1 << 8)  /* Bit 8 */
#define DMA_INT_STATUS_BUS_ERR1 (1 << 9)  /* Bit 9 */
#define DMA_INT_STATUS_BUS_ERR2 (1 << 10) /* Bit 10 */
#define DMA_INT_STATUS_BUS_ERR3 (1 << 11) /* Bit 11 */
#define DMA_INT_STATUS_BUS_ERR4 (1 << 12) /* Bit 12 */
#define DMA_INT_STATUS_BUS_ERR5 (1 << 13) /* Bit 13 */
#define DMA_INT_STATUS_BUS_ERR6 (1 << 14) /* Bit 14 */
#define DMA_INT_STATUS_BUS_ERR7 (1 << 15) /* Bit 15 */

/* CLEAR_INT register */

#define DMA_CLEAR_INT_RST_IRQ_CH0 (1 << 0) /* Bit 0 */
#define DMA_CLEAR_INT_RST_IRQ_CH1 (1 << 1) /* Bit 1 */
#define DMA_CLEAR_INT_RST_IRQ_CH2 (1 << 2) /* Bit 2 */
#define DMA_CLEAR_INT_RST_IRQ_CH3 (1 << 3) /* Bit 3 */
#define DMA_CLEAR_INT_RST_IRQ_CH4 (1 << 4) /* Bit 4 */
#define DMA_CLEAR_INT_RST_IRQ_CH5 (1 << 5) /* Bit 5 */
#define DMA_CLEAR_INT_RST_IRQ_CH6 (1 << 6) /* Bit 6 */
#define DMA_CLEAR_INT_RST_IRQ_CH7 (1 << 7) /* Bit 7 */

/* INT_MASK register */

#define DMA_INT_MASK_IRQ_ENABLE0 (1 << 0) /* Bit 0 */
#define DMA_INT_MASK_IRQ_ENABLE1 (1 << 1) /* Bit 1 */
#define DMA_INT_MASK_IRQ_ENABLE2 (1 << 2) /* Bit 2 */
#define DMA_INT_MASK_IRQ_ENABLE3 (1 << 3) /* Bit 3 */
#define DMA_INT_MASK_IRQ_ENABLE4 (1 << 4) /* Bit 4 */
#define DMA_INT_MASK_IRQ_ENABLE5 (1 << 5) /* Bit 5 */
#define DMA_INT_MASK_IRQ_ENABLE6 (1 << 6) /* Bit 6 */
#define DMA_INT_MASK_IRQ_ENABLE7 (1 << 7) /* Bit 7 */

/* SET_INT_MASK register */

#define DMA_SET_INT_MASK_SET_IRQ_ENABLE0 (1 << 0) /* Bit 0 */
#define DMA_SET_INT_MASK_SET_IRQ_ENABLE1 (1 << 1) /* Bit 1 */
#define DMA_SET_INT_MASK_SET_IRQ_ENABLE2 (1 << 2) /* Bit 2 */
#define DMA_SET_INT_MASK_SET_IRQ_ENABLE3 (1 << 3) /* Bit 3 */
#define DMA_SET_INT_MASK_SET_IRQ_ENABLE4 (1 << 4) /* Bit 4 */
#define DMA_SET_INT_MASK_SET_IRQ_ENABLE5 (1 << 5) /* Bit 5 */
#define DMA_SET_INT_MASK_SET_IRQ_ENABLE6 (1 << 6) /* Bit 6 */
#define DMA_SET_INT_MASK_SET_IRQ_ENABLE7 (1 << 7) /* Bit 7 */

/* RESET_INT_MASK register */

#define DMA_RESET_INT_MASK_RESET_IRQ_ENABLE0 (1 << 0) /* Bit 0 */
#define DMA_RESET_INT_MASK_RESET_IRQ_ENABLE1 (1 << 1) /* Bit 1 */
#define DMA_RESET_INT_MASK_RESET_IRQ_ENABLE2 (1 << 2) /* Bit 2 */
#define DMA_RESET_INT_MASK_RESET_IRQ_ENABLE3 (1 << 3) /* Bit 3 */
#define DMA_RESET_INT_MASK_RESET_IRQ_ENABLE4 (1 << 4) /* Bit 4 */
#define DMA_RESET_INT_MASK_RESET_IRQ_ENABLE5 (1 << 5) /* Bit 5 */
#define DMA_RESET_INT_MASK_RESET_IRQ_ENABLE6 (1 << 6) /* Bit 6 */
#define DMA_RESET_INT_MASK_RESET_IRQ_ENABLE7 (1 << 7) /* Bit 7 */

/* Per-channel register block accessors *************************************/

#define DA1470X_DMA_NCHANNELS 8
#define DA1470X_DMA_CHAN_BASE(n) (DA1470X_DMA_BASE + ((n) * 0x20))
#define DA1470X_DMA_A_START_OFFSET 0x0000
#define DA1470X_DMA_B_START_OFFSET 0x0004
#define DA1470X_DMA_INT_OFFSET 0x0008
#define DA1470X_DMA_LEN_OFFSET 0x000c
#define DA1470X_DMA_CTRL_OFFSET 0x0010
#define DA1470X_DMA_IDX_OFFSET 0x0014

/* DMA_REQ_MUX: 4 bits per channel pair *************************************/

#define DMA_REQ_MUX_SHIFT(pair) ((pair) * 4)
#define DMA_REQ_MUX_MASK(pair) (0xf << DMA_REQ_MUX_SHIFT(pair))
#  define DMA_REQ_MUX_SPI 0x0
#  define DMA_REQ_MUX_SPI2 0x1
#  define DMA_REQ_MUX_UART 0x2
#  define DMA_REQ_MUX_UART2 0x3
#  define DMA_REQ_MUX_I2C 0x4
#  define DMA_REQ_MUX_I2C2 0x5
#  define DMA_REQ_MUX_USB 0x6
#  define DMA_REQ_MUX_UART3 0x7
#  define DMA_REQ_MUX_PCM 0x8
#  define DMA_REQ_MUX_SRC 0x9
#  define DMA_REQ_MUX_SPI3 0xa
#  define DMA_REQ_MUX_I2C3 0xb
#  define DMA_REQ_MUX_GPADC 0xc
#  define DMA_REQ_MUX_SRC2 0xd
#  define DMA_REQ_MUX_I3C 0xe
#  define DMA_REQ_MUX_NONE 0xf

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_DMA_H */
