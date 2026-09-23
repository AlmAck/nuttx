/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_chip_version.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_CHIP_VERSION_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_CHIP_VERSION_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470x_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define DA1470X_CHIP_VERSION_CHIP_ID1_OFFSET 0x0000      /* Chip identification register 1. */
#define DA1470X_CHIP_VERSION_CHIP_ID2_OFFSET 0x0004      /* Chip identification register 2. */
#define DA1470X_CHIP_VERSION_CHIP_ID3_OFFSET 0x0008      /* Chip identification register 3. */
#define DA1470X_CHIP_VERSION_CHIP_ID4_OFFSET 0x000c      /* Chip identification register 4. */
#define DA1470X_CHIP_VERSION_CHIP_SWC_OFFSET 0x0010      /* Software compatibility register. */
#define DA1470X_CHIP_VERSION_CHIP_REVISION_OFFSET 0x0014 /* Chip revision register. */
#define DA1470X_CHIP_VERSION_CHIP_TEST1_OFFSET 0x00f8    /* Chip test register 1. */
#define DA1470X_CHIP_VERSION_CHIP_TEST2_OFFSET 0x00fc    /* Chip test register 2. */

/* Register Addresses *******************************************************/

#define DA1470X_CHIP_VERSION_CHIP_ID1 (DA1470X_CHIP_VERSION_BASE + DA1470X_CHIP_VERSION_CHIP_ID1_OFFSET)
#define DA1470X_CHIP_VERSION_CHIP_ID2 (DA1470X_CHIP_VERSION_BASE + DA1470X_CHIP_VERSION_CHIP_ID2_OFFSET)
#define DA1470X_CHIP_VERSION_CHIP_ID3 (DA1470X_CHIP_VERSION_BASE + DA1470X_CHIP_VERSION_CHIP_ID3_OFFSET)
#define DA1470X_CHIP_VERSION_CHIP_ID4 (DA1470X_CHIP_VERSION_BASE + DA1470X_CHIP_VERSION_CHIP_ID4_OFFSET)
#define DA1470X_CHIP_VERSION_CHIP_SWC (DA1470X_CHIP_VERSION_BASE + DA1470X_CHIP_VERSION_CHIP_SWC_OFFSET)
#define DA1470X_CHIP_VERSION_CHIP_REVISION (DA1470X_CHIP_VERSION_BASE + DA1470X_CHIP_VERSION_CHIP_REVISION_OFFSET)
#define DA1470X_CHIP_VERSION_CHIP_TEST1 (DA1470X_CHIP_VERSION_BASE + DA1470X_CHIP_VERSION_CHIP_TEST1_OFFSET)
#define DA1470X_CHIP_VERSION_CHIP_TEST2 (DA1470X_CHIP_VERSION_BASE + DA1470X_CHIP_VERSION_CHIP_TEST2_OFFSET)

/* Register Bit-field Definitions *******************************************/

/* CHIP_ID1 register */

#define CHIP_VERSION_CHIP_ID1_CHIP_ID1_SHIFT (0) /* Bits 0-7 */
#define CHIP_VERSION_CHIP_ID1_CHIP_ID1_MASK (0xff << CHIP_VERSION_CHIP_ID1_CHIP_ID1_SHIFT)
#  define CHIP_VERSION_CHIP_ID1_CHIP_ID1(n) ((uint32_t)(n) << CHIP_VERSION_CHIP_ID1_CHIP_ID1_SHIFT)

/* CHIP_ID2 register */

#define CHIP_VERSION_CHIP_ID2_CHIP_ID2_SHIFT (0) /* Bits 0-7 */
#define CHIP_VERSION_CHIP_ID2_CHIP_ID2_MASK (0xff << CHIP_VERSION_CHIP_ID2_CHIP_ID2_SHIFT)
#  define CHIP_VERSION_CHIP_ID2_CHIP_ID2(n) ((uint32_t)(n) << CHIP_VERSION_CHIP_ID2_CHIP_ID2_SHIFT)

/* CHIP_ID3 register */

#define CHIP_VERSION_CHIP_ID3_CHIP_ID3_SHIFT (0) /* Bits 0-7 */
#define CHIP_VERSION_CHIP_ID3_CHIP_ID3_MASK (0xff << CHIP_VERSION_CHIP_ID3_CHIP_ID3_SHIFT)
#  define CHIP_VERSION_CHIP_ID3_CHIP_ID3(n) ((uint32_t)(n) << CHIP_VERSION_CHIP_ID3_CHIP_ID3_SHIFT)

/* CHIP_ID4 register */

#define CHIP_VERSION_CHIP_ID4_CHIP_ID4_SHIFT (0) /* Bits 0-7 */
#define CHIP_VERSION_CHIP_ID4_CHIP_ID4_MASK (0xff << CHIP_VERSION_CHIP_ID4_CHIP_ID4_SHIFT)
#  define CHIP_VERSION_CHIP_ID4_CHIP_ID4(n) ((uint32_t)(n) << CHIP_VERSION_CHIP_ID4_CHIP_ID4_SHIFT)

/* CHIP_SWC register */

#define CHIP_VERSION_CHIP_SWC_CHIP_SWC_SHIFT (0) /* Bits 0-3 */
#define CHIP_VERSION_CHIP_SWC_CHIP_SWC_MASK (0xf << CHIP_VERSION_CHIP_SWC_CHIP_SWC_SHIFT)
#  define CHIP_VERSION_CHIP_SWC_CHIP_SWC(n) ((uint32_t)(n) << CHIP_VERSION_CHIP_SWC_CHIP_SWC_SHIFT)

/* CHIP_REVISION register */

#define CHIP_VERSION_CHIP_REVISION_CHIP_REVISION_SHIFT (0) /* Bits 0-7 */
#define CHIP_VERSION_CHIP_REVISION_CHIP_REVISION_MASK (0xff << CHIP_VERSION_CHIP_REVISION_CHIP_REVISION_SHIFT)
#  define CHIP_VERSION_CHIP_REVISION_CHIP_REVISION(n) ((uint32_t)(n) << CHIP_VERSION_CHIP_REVISION_CHIP_REVISION_SHIFT)

/* CHIP_TEST1 register */

#define CHIP_VERSION_CHIP_TEST1_CHIP_LAYOUT_REVISION_SHIFT (0) /* Bits 0-7 */
#define CHIP_VERSION_CHIP_TEST1_CHIP_LAYOUT_REVISION_MASK (0xff << CHIP_VERSION_CHIP_TEST1_CHIP_LAYOUT_REVISION_SHIFT)
#  define CHIP_VERSION_CHIP_TEST1_CHIP_LAYOUT_REVISION(n) ((uint32_t)(n) << CHIP_VERSION_CHIP_TEST1_CHIP_LAYOUT_REVISION_SHIFT)

/* CHIP_TEST2 register */

#define CHIP_VERSION_CHIP_TEST2_CHIP_METAL_OPTION_SHIFT (0) /* Bits 0-3 */
#define CHIP_VERSION_CHIP_TEST2_CHIP_METAL_OPTION_MASK (0xf << CHIP_VERSION_CHIP_TEST2_CHIP_METAL_OPTION_SHIFT)
#  define CHIP_VERSION_CHIP_TEST2_CHIP_METAL_OPTION(n) ((uint32_t)(n) << CHIP_VERSION_CHIP_TEST2_CHIP_METAL_OPTION_SHIFT)

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_CHIP_VERSION_H */
