/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_wakeup.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_WAKEUP_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_WAKEUP_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470x_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define DA1470X_WAKEUP_WKUP_CTRL_OFFSET 0x0000         /* Control register for the wakeup counter */
#define DA1470X_WAKEUP_WKUP_RESET_IRQ_OFFSET 0x0004    /* Reset wakeup interrupt */
#define DA1470X_WAKEUP_WKUP_SELECT_P0_OFFSET 0x0008    /* Select which inputs from P0 port can tri */
#define DA1470X_WAKEUP_WKUP_SELECT_P1_OFFSET 0x000c    /* Select which inputs from P1 port can tri */
#define DA1470X_WAKEUP_WKUP_SELECT_P2_OFFSET 0x0010    /* Select which inputs from P2 port can tri */
#define DA1470X_WAKEUP_WKUP_POL_P0_OFFSET 0x0014       /* select the sesitivity polarity for each */
#define DA1470X_WAKEUP_WKUP_POL_P1_OFFSET 0x0018       /* select the sesitivity polarity for each */
#define DA1470X_WAKEUP_WKUP_POL_P2_OFFSET 0x001c       /* select the sesitivity polarity for each */
#define DA1470X_WAKEUP_WKUP_STATUS_P0_OFFSET 0x0020    /* Event status register for P0 */
#define DA1470X_WAKEUP_WKUP_STATUS_P1_OFFSET 0x0024    /* Event status register for P1 */
#define DA1470X_WAKEUP_WKUP_STATUS_P2_OFFSET 0x0028    /* Event status register for P2 */
#define DA1470X_WAKEUP_WKUP_CLEAR_P0_OFFSET 0x002c     /* Clear event register for P0 */
#define DA1470X_WAKEUP_WKUP_CLEAR_P1_OFFSET 0x0030     /* Clear event register for P1 */
#define DA1470X_WAKEUP_WKUP_CLEAR_P2_OFFSET 0x0034     /* Clear event register for P2 */
#define DA1470X_WAKEUP_WKUP_SEL_GPIO_P0_OFFSET 0x0038  /* Enable fast wakeup and enable GPIO_P0_IR */
#define DA1470X_WAKEUP_WKUP_SEL_GPIO_P1_OFFSET 0x003c  /* Enable fast wakeup and enable GPIO_P1_IR */
#define DA1470X_WAKEUP_WKUP_SEL_GPIO_P2_OFFSET 0x0040  /* Enable fast wakeup and enable GPIO_P2_IR */
#define DA1470X_WAKEUP_WKUP_SEL1_GPIO_P0_OFFSET 0x0044 /* Configure to generate level or edge sens */
#define DA1470X_WAKEUP_WKUP_SEL1_GPIO_P1_OFFSET 0x0048 /* Configure to generate level or edge sens */
#define DA1470X_WAKEUP_WKUP_SEL1_GPIO_P2_OFFSET 0x004c /* Configure to generate level or edge sens */

/* Register Addresses *******************************************************/

#define DA1470X_WAKEUP_WKUP_CTRL (DA1470X_WAKEUP_BASE + DA1470X_WAKEUP_WKUP_CTRL_OFFSET)
#define DA1470X_WAKEUP_WKUP_RESET_IRQ (DA1470X_WAKEUP_BASE + DA1470X_WAKEUP_WKUP_RESET_IRQ_OFFSET)
#define DA1470X_WAKEUP_WKUP_SELECT_P0 (DA1470X_WAKEUP_BASE + DA1470X_WAKEUP_WKUP_SELECT_P0_OFFSET)
#define DA1470X_WAKEUP_WKUP_SELECT_P1 (DA1470X_WAKEUP_BASE + DA1470X_WAKEUP_WKUP_SELECT_P1_OFFSET)
#define DA1470X_WAKEUP_WKUP_SELECT_P2 (DA1470X_WAKEUP_BASE + DA1470X_WAKEUP_WKUP_SELECT_P2_OFFSET)
#define DA1470X_WAKEUP_WKUP_POL_P0 (DA1470X_WAKEUP_BASE + DA1470X_WAKEUP_WKUP_POL_P0_OFFSET)
#define DA1470X_WAKEUP_WKUP_POL_P1 (DA1470X_WAKEUP_BASE + DA1470X_WAKEUP_WKUP_POL_P1_OFFSET)
#define DA1470X_WAKEUP_WKUP_POL_P2 (DA1470X_WAKEUP_BASE + DA1470X_WAKEUP_WKUP_POL_P2_OFFSET)
#define DA1470X_WAKEUP_WKUP_STATUS_P0 (DA1470X_WAKEUP_BASE + DA1470X_WAKEUP_WKUP_STATUS_P0_OFFSET)
#define DA1470X_WAKEUP_WKUP_STATUS_P1 (DA1470X_WAKEUP_BASE + DA1470X_WAKEUP_WKUP_STATUS_P1_OFFSET)
#define DA1470X_WAKEUP_WKUP_STATUS_P2 (DA1470X_WAKEUP_BASE + DA1470X_WAKEUP_WKUP_STATUS_P2_OFFSET)
#define DA1470X_WAKEUP_WKUP_CLEAR_P0 (DA1470X_WAKEUP_BASE + DA1470X_WAKEUP_WKUP_CLEAR_P0_OFFSET)
#define DA1470X_WAKEUP_WKUP_CLEAR_P1 (DA1470X_WAKEUP_BASE + DA1470X_WAKEUP_WKUP_CLEAR_P1_OFFSET)
#define DA1470X_WAKEUP_WKUP_CLEAR_P2 (DA1470X_WAKEUP_BASE + DA1470X_WAKEUP_WKUP_CLEAR_P2_OFFSET)
#define DA1470X_WAKEUP_WKUP_SEL_GPIO_P0 (DA1470X_WAKEUP_BASE + DA1470X_WAKEUP_WKUP_SEL_GPIO_P0_OFFSET)
#define DA1470X_WAKEUP_WKUP_SEL_GPIO_P1 (DA1470X_WAKEUP_BASE + DA1470X_WAKEUP_WKUP_SEL_GPIO_P1_OFFSET)
#define DA1470X_WAKEUP_WKUP_SEL_GPIO_P2 (DA1470X_WAKEUP_BASE + DA1470X_WAKEUP_WKUP_SEL_GPIO_P2_OFFSET)
#define DA1470X_WAKEUP_WKUP_SEL1_GPIO_P0 (DA1470X_WAKEUP_BASE + DA1470X_WAKEUP_WKUP_SEL1_GPIO_P0_OFFSET)
#define DA1470X_WAKEUP_WKUP_SEL1_GPIO_P1 (DA1470X_WAKEUP_BASE + DA1470X_WAKEUP_WKUP_SEL1_GPIO_P1_OFFSET)
#define DA1470X_WAKEUP_WKUP_SEL1_GPIO_P2 (DA1470X_WAKEUP_BASE + DA1470X_WAKEUP_WKUP_SEL1_GPIO_P2_OFFSET)

/* Register Bit-field Definitions *******************************************/

/* WKUP_CTRL register */

#define WAKEUP_WKUP_CTRL_WKUP_DEB_VALUE_SHIFT (0) /* Bits 0-5 */
#define WAKEUP_WKUP_CTRL_WKUP_DEB_VALUE_MASK (0x3f << WAKEUP_WKUP_CTRL_WKUP_DEB_VALUE_SHIFT)
#  define WAKEUP_WKUP_CTRL_WKUP_DEB_VALUE(n) ((uint32_t)(n) << WAKEUP_WKUP_CTRL_WKUP_DEB_VALUE_SHIFT)
#define WAKEUP_WKUP_CTRL_WKUP_SFT_KEYHIT (1 << 6) /* Bit 6 */
#define WAKEUP_WKUP_CTRL_WKUP_ENABLE_IRQ (1 << 7) /* Bit 7 */

/* WKUP_RESET_IRQ register */

#define WAKEUP_WKUP_RESET_IRQ_WKUP_IRQ_RST_SHIFT (0) /* Bits 0-15 */
#define WAKEUP_WKUP_RESET_IRQ_WKUP_IRQ_RST_MASK (0xffff << WAKEUP_WKUP_RESET_IRQ_WKUP_IRQ_RST_SHIFT)
#  define WAKEUP_WKUP_RESET_IRQ_WKUP_IRQ_RST(n) ((uint32_t)(n) << WAKEUP_WKUP_RESET_IRQ_WKUP_IRQ_RST_SHIFT)

/* WKUP_SELECT_P0 register */

#define WAKEUP_WKUP_SELECT_P0_WKUP_SELECT_P0_SHIFT (0) /* Bits 0-31 */
#define WAKEUP_WKUP_SELECT_P0_WKUP_SELECT_P0_MASK (0xffffffff << WAKEUP_WKUP_SELECT_P0_WKUP_SELECT_P0_SHIFT)
#  define WAKEUP_WKUP_SELECT_P0_WKUP_SELECT_P0(n) ((uint32_t)(n) << WAKEUP_WKUP_SELECT_P0_WKUP_SELECT_P0_SHIFT)

/* WKUP_SELECT_P1 register */

#define WAKEUP_WKUP_SELECT_P1_WKUP_SELECT_P1_SHIFT (0) /* Bits 0-31 */
#define WAKEUP_WKUP_SELECT_P1_WKUP_SELECT_P1_MASK (0xffffffff << WAKEUP_WKUP_SELECT_P1_WKUP_SELECT_P1_SHIFT)
#  define WAKEUP_WKUP_SELECT_P1_WKUP_SELECT_P1(n) ((uint32_t)(n) << WAKEUP_WKUP_SELECT_P1_WKUP_SELECT_P1_SHIFT)

/* WKUP_SELECT_P2 register */

#define WAKEUP_WKUP_SELECT_P2_WKUP_SELECT_P2_SHIFT (0) /* Bits 0-14 */
#define WAKEUP_WKUP_SELECT_P2_WKUP_SELECT_P2_MASK (0x7fff << WAKEUP_WKUP_SELECT_P2_WKUP_SELECT_P2_SHIFT)
#  define WAKEUP_WKUP_SELECT_P2_WKUP_SELECT_P2(n) ((uint32_t)(n) << WAKEUP_WKUP_SELECT_P2_WKUP_SELECT_P2_SHIFT)

/* WKUP_POL_P0 register */

#define WAKEUP_WKUP_POL_P0_WKUP_POL_P0_SHIFT (0) /* Bits 0-31 */
#define WAKEUP_WKUP_POL_P0_WKUP_POL_P0_MASK (0xffffffff << WAKEUP_WKUP_POL_P0_WKUP_POL_P0_SHIFT)
#  define WAKEUP_WKUP_POL_P0_WKUP_POL_P0(n) ((uint32_t)(n) << WAKEUP_WKUP_POL_P0_WKUP_POL_P0_SHIFT)

/* WKUP_POL_P1 register */

#define WAKEUP_WKUP_POL_P1_WKUP_POL_P1_SHIFT (0) /* Bits 0-31 */
#define WAKEUP_WKUP_POL_P1_WKUP_POL_P1_MASK (0xffffffff << WAKEUP_WKUP_POL_P1_WKUP_POL_P1_SHIFT)
#  define WAKEUP_WKUP_POL_P1_WKUP_POL_P1(n) ((uint32_t)(n) << WAKEUP_WKUP_POL_P1_WKUP_POL_P1_SHIFT)

/* WKUP_POL_P2 register */

#define WAKEUP_WKUP_POL_P2_WKUP_POL_P2_SHIFT (0) /* Bits 0-14 */
#define WAKEUP_WKUP_POL_P2_WKUP_POL_P2_MASK (0x7fff << WAKEUP_WKUP_POL_P2_WKUP_POL_P2_SHIFT)
#  define WAKEUP_WKUP_POL_P2_WKUP_POL_P2(n) ((uint32_t)(n) << WAKEUP_WKUP_POL_P2_WKUP_POL_P2_SHIFT)

/* WKUP_STATUS_P0 register */

#define WAKEUP_WKUP_STATUS_P0_WKUP_STAT_P0_SHIFT (0) /* Bits 0-31 */
#define WAKEUP_WKUP_STATUS_P0_WKUP_STAT_P0_MASK (0xffffffff << WAKEUP_WKUP_STATUS_P0_WKUP_STAT_P0_SHIFT)
#  define WAKEUP_WKUP_STATUS_P0_WKUP_STAT_P0(n) ((uint32_t)(n) << WAKEUP_WKUP_STATUS_P0_WKUP_STAT_P0_SHIFT)

/* WKUP_STATUS_P1 register */

#define WAKEUP_WKUP_STATUS_P1_WKUP_STAT_P1_SHIFT (0) /* Bits 0-31 */
#define WAKEUP_WKUP_STATUS_P1_WKUP_STAT_P1_MASK (0xffffffff << WAKEUP_WKUP_STATUS_P1_WKUP_STAT_P1_SHIFT)
#  define WAKEUP_WKUP_STATUS_P1_WKUP_STAT_P1(n) ((uint32_t)(n) << WAKEUP_WKUP_STATUS_P1_WKUP_STAT_P1_SHIFT)

/* WKUP_STATUS_P2 register */

#define WAKEUP_WKUP_STATUS_P2_WKUP_STAT_P2_SHIFT (0) /* Bits 0-14 */
#define WAKEUP_WKUP_STATUS_P2_WKUP_STAT_P2_MASK (0x7fff << WAKEUP_WKUP_STATUS_P2_WKUP_STAT_P2_SHIFT)
#  define WAKEUP_WKUP_STATUS_P2_WKUP_STAT_P2(n) ((uint32_t)(n) << WAKEUP_WKUP_STATUS_P2_WKUP_STAT_P2_SHIFT)

/* WKUP_CLEAR_P0 register */

#define WAKEUP_WKUP_CLEAR_P0_WKUP_CLEAR_P0_SHIFT (0) /* Bits 0-31 */
#define WAKEUP_WKUP_CLEAR_P0_WKUP_CLEAR_P0_MASK (0xffffffff << WAKEUP_WKUP_CLEAR_P0_WKUP_CLEAR_P0_SHIFT)
#  define WAKEUP_WKUP_CLEAR_P0_WKUP_CLEAR_P0(n) ((uint32_t)(n) << WAKEUP_WKUP_CLEAR_P0_WKUP_CLEAR_P0_SHIFT)

/* WKUP_CLEAR_P1 register */

#define WAKEUP_WKUP_CLEAR_P1_WKUP_CLEAR_P1_SHIFT (0) /* Bits 0-31 */
#define WAKEUP_WKUP_CLEAR_P1_WKUP_CLEAR_P1_MASK (0xffffffff << WAKEUP_WKUP_CLEAR_P1_WKUP_CLEAR_P1_SHIFT)
#  define WAKEUP_WKUP_CLEAR_P1_WKUP_CLEAR_P1(n) ((uint32_t)(n) << WAKEUP_WKUP_CLEAR_P1_WKUP_CLEAR_P1_SHIFT)

/* WKUP_CLEAR_P2 register */

#define WAKEUP_WKUP_CLEAR_P2_WKUP_CLEAR_P2_SHIFT (0) /* Bits 0-14 */
#define WAKEUP_WKUP_CLEAR_P2_WKUP_CLEAR_P2_MASK (0x7fff << WAKEUP_WKUP_CLEAR_P2_WKUP_CLEAR_P2_SHIFT)
#  define WAKEUP_WKUP_CLEAR_P2_WKUP_CLEAR_P2(n) ((uint32_t)(n) << WAKEUP_WKUP_CLEAR_P2_WKUP_CLEAR_P2_SHIFT)

/* WKUP_SEL_GPIO_P0 register */

#define WAKEUP_WKUP_SEL_GPIO_P0_WKUP_SEL_GPIO_P0_SHIFT (0) /* Bits 0-31 */
#define WAKEUP_WKUP_SEL_GPIO_P0_WKUP_SEL_GPIO_P0_MASK (0xffffffff << WAKEUP_WKUP_SEL_GPIO_P0_WKUP_SEL_GPIO_P0_SHIFT)
#  define WAKEUP_WKUP_SEL_GPIO_P0_WKUP_SEL_GPIO_P0(n) ((uint32_t)(n) << WAKEUP_WKUP_SEL_GPIO_P0_WKUP_SEL_GPIO_P0_SHIFT)

/* WKUP_SEL_GPIO_P1 register */

#define WAKEUP_WKUP_SEL_GPIO_P1_WKUP_SEL_GPIO_P1_SHIFT (0) /* Bits 0-31 */
#define WAKEUP_WKUP_SEL_GPIO_P1_WKUP_SEL_GPIO_P1_MASK (0xffffffff << WAKEUP_WKUP_SEL_GPIO_P1_WKUP_SEL_GPIO_P1_SHIFT)
#  define WAKEUP_WKUP_SEL_GPIO_P1_WKUP_SEL_GPIO_P1(n) ((uint32_t)(n) << WAKEUP_WKUP_SEL_GPIO_P1_WKUP_SEL_GPIO_P1_SHIFT)

/* WKUP_SEL_GPIO_P2 register */

#define WAKEUP_WKUP_SEL_GPIO_P2_WKUP_SEL_GPIO_P2_SHIFT (0) /* Bits 0-14 */
#define WAKEUP_WKUP_SEL_GPIO_P2_WKUP_SEL_GPIO_P2_MASK (0x7fff << WAKEUP_WKUP_SEL_GPIO_P2_WKUP_SEL_GPIO_P2_SHIFT)
#  define WAKEUP_WKUP_SEL_GPIO_P2_WKUP_SEL_GPIO_P2(n) ((uint32_t)(n) << WAKEUP_WKUP_SEL_GPIO_P2_WKUP_SEL_GPIO_P2_SHIFT)

/* WKUP_SEL1_GPIO_P0 register */

#define WAKEUP_WKUP_SEL1_GPIO_P0_WKUP_SEL1_GPIO_P0_SHIFT (0) /* Bits 0-31 */
#define WAKEUP_WKUP_SEL1_GPIO_P0_WKUP_SEL1_GPIO_P0_MASK (0xffffffff << WAKEUP_WKUP_SEL1_GPIO_P0_WKUP_SEL1_GPIO_P0_SHIFT)
#  define WAKEUP_WKUP_SEL1_GPIO_P0_WKUP_SEL1_GPIO_P0(n) ((uint32_t)(n) << WAKEUP_WKUP_SEL1_GPIO_P0_WKUP_SEL1_GPIO_P0_SHIFT)

/* WKUP_SEL1_GPIO_P1 register */

#define WAKEUP_WKUP_SEL1_GPIO_P1_WKUP_SEL1_GPIO_P1_SHIFT (0) /* Bits 0-31 */
#define WAKEUP_WKUP_SEL1_GPIO_P1_WKUP_SEL1_GPIO_P1_MASK (0xffffffff << WAKEUP_WKUP_SEL1_GPIO_P1_WKUP_SEL1_GPIO_P1_SHIFT)
#  define WAKEUP_WKUP_SEL1_GPIO_P1_WKUP_SEL1_GPIO_P1(n) ((uint32_t)(n) << WAKEUP_WKUP_SEL1_GPIO_P1_WKUP_SEL1_GPIO_P1_SHIFT)

/* WKUP_SEL1_GPIO_P2 register */

#define WAKEUP_WKUP_SEL1_GPIO_P2_WKUP_SEL1_GPIO_P2_SHIFT (0) /* Bits 0-14 */
#define WAKEUP_WKUP_SEL1_GPIO_P2_WKUP_SEL1_GPIO_P2_MASK (0x7fff << WAKEUP_WKUP_SEL1_GPIO_P2_WKUP_SEL1_GPIO_P2_SHIFT)
#  define WAKEUP_WKUP_SEL1_GPIO_P2_WKUP_SEL1_GPIO_P2(n) ((uint32_t)(n) << WAKEUP_WKUP_SEL1_GPIO_P2_WKUP_SEL1_GPIO_P2_SHIFT)

/* Per-port accessors *******************************************************/

#define DA1470X_WAKEUP_SELECT_P(p) (DA1470X_WAKEUP_BASE + 0x0008 + ((p) << 2))
#define DA1470X_WAKEUP_POL_P(p) (DA1470X_WAKEUP_BASE + 0x0014 + ((p) << 2))
#define DA1470X_WAKEUP_STATUS_P(p) (DA1470X_WAKEUP_BASE + 0x0020 + ((p) << 2))
#define DA1470X_WAKEUP_CLEAR_P(p) (DA1470X_WAKEUP_BASE + 0x002c + ((p) << 2))
#define DA1470X_WAKEUP_SEL_GPIO_P(p) (DA1470X_WAKEUP_BASE + 0x0038 + ((p) << 2))
#define DA1470X_WAKEUP_SEL1_GPIO_P(p) (DA1470X_WAKEUP_BASE + 0x0044 + ((p) << 2))

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_WAKEUP_H */
