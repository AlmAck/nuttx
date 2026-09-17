/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_dcache.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_DCACHE_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_DCACHE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470x_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define DA1470X_DCACHE_CTRL_OFFSET 0x0000             /* Dcache Control register */
#define DA1470X_DCACHE_BASE_ADDR_OFFSET 0x0004        /* Dcache base address for cacheable region */
#define DA1470X_DCACHE_MRM_HITS_OFFSET 0x0008         /* Dcache MRM (Miss Rate Monitor) HITS regi */
#define DA1470X_DCACHE_MRM_MISSES_OFFSET 0x000c       /* Dcache MRM (Miss Rate Monitor) MISSES re */
#define DA1470X_DCACHE_MRM_EVICTS_OFFSET 0x0010       /* Dcache MRM (Miss Rate Monitor) EVICTS re */
#define DA1470X_DCACHE_MRM_CTRL_OFFSET 0x0014         /* Dcache MRM (Miss Rate Monitor) CONTROL r */
#define DA1470X_DCACHE_MRM_TINT_OFFSET 0x0018         /* Dcache MRM (Miss Rate Monitor) TIME INTE */
#define DA1470X_DCACHE_MRM_MISSES_THRES_OFFSET 0x001c /* Dcache MRM (Miss Rate Monitor) THRESHOLD */
#define DA1470X_DCACHE_MRM_HITS_THRES_OFFSET 0x0020   /* Dcache MRM (Miss Rate Monitor) HITS THRE */
#define DA1470X_DCACHE_MRM_EVICTS_THRES_OFFSET 0x0024 /* Dcache MRM (Miss Rate Monitor) EVICTS TH */

/* Register Addresses *******************************************************/

#define DA1470X_DCACHE_CTRL (DA1470X_DCACHE_BASE + DA1470X_DCACHE_CTRL_OFFSET)
#define DA1470X_DCACHE_BASE_ADDR (DA1470X_DCACHE_BASE + DA1470X_DCACHE_BASE_ADDR_OFFSET)
#define DA1470X_DCACHE_MRM_HITS (DA1470X_DCACHE_BASE + DA1470X_DCACHE_MRM_HITS_OFFSET)
#define DA1470X_DCACHE_MRM_MISSES (DA1470X_DCACHE_BASE + DA1470X_DCACHE_MRM_MISSES_OFFSET)
#define DA1470X_DCACHE_MRM_EVICTS (DA1470X_DCACHE_BASE + DA1470X_DCACHE_MRM_EVICTS_OFFSET)
#define DA1470X_DCACHE_MRM_CTRL (DA1470X_DCACHE_BASE + DA1470X_DCACHE_MRM_CTRL_OFFSET)
#define DA1470X_DCACHE_MRM_TINT (DA1470X_DCACHE_BASE + DA1470X_DCACHE_MRM_TINT_OFFSET)
#define DA1470X_DCACHE_MRM_MISSES_THRES (DA1470X_DCACHE_BASE + DA1470X_DCACHE_MRM_MISSES_THRES_OFFSET)
#define DA1470X_DCACHE_MRM_HITS_THRES (DA1470X_DCACHE_BASE + DA1470X_DCACHE_MRM_HITS_THRES_OFFSET)
#define DA1470X_DCACHE_MRM_EVICTS_THRES (DA1470X_DCACHE_BASE + DA1470X_DCACHE_MRM_EVICTS_THRES_OFFSET)

/* Register Bit-field Definitions *******************************************/

/* CTRL register */

#define DCACHE_CTRL_LEN_SHIFT (0)           /* Bits 0-17 */
#define DCACHE_CTRL_LEN_MASK (0x3ffff << DCACHE_CTRL_LEN_SHIFT)
#  define DCACHE_CTRL_LEN(n) ((uint32_t)(n) << DCACHE_CTRL_LEN_SHIFT)
#define DCACHE_CTRL_ENABLE (1 << 18)        /* Bit 18 */
#define DCACHE_CTRL_INIT (1 << 19)          /* Bit 19 */
#define DCACHE_CTRL_WFLUSH (1 << 20)        /* Bit 20 */
#define DCACHE_CTRL_READY (1 << 21)         /* Bit 21 */
#define DCACHE_CTRL_WFLUSHED (1 << 22)      /* Bit 22 */
#define DCACHE_CTRL_WBUFFER_EMPTY (1 << 23) /* Bit 23 */
#define DCACHE_CTRL_WBUFFER_FLUSH (1 << 24) /* Bit 24 */
#define DCACHE_CTRL_BYPASS (1 << 26)        /* Bit 26 */

/* BASE_ADDR register */

#define DCACHE_BASE_ADDR_BASE_ADDR_SHIFT (0) /* Bits 0-16 */
#define DCACHE_BASE_ADDR_BASE_ADDR_MASK (0x1ffff << DCACHE_BASE_ADDR_BASE_ADDR_SHIFT)
#  define DCACHE_BASE_ADDR_BASE_ADDR(n) ((uint32_t)(n) << DCACHE_BASE_ADDR_BASE_ADDR_SHIFT)

/* MRM_HITS register */

#define DCACHE_MRM_HITS_MRM_HITS_SHIFT (0) /* Bits 0-31 */
#define DCACHE_MRM_HITS_MRM_HITS_MASK (0xffffffff << DCACHE_MRM_HITS_MRM_HITS_SHIFT)
#  define DCACHE_MRM_HITS_MRM_HITS(n) ((uint32_t)(n) << DCACHE_MRM_HITS_MRM_HITS_SHIFT)

/* MRM_MISSES register */

#define DCACHE_MRM_MISSES_MRM_MISSES_SHIFT (0) /* Bits 0-31 */
#define DCACHE_MRM_MISSES_MRM_MISSES_MASK (0xffffffff << DCACHE_MRM_MISSES_MRM_MISSES_SHIFT)
#  define DCACHE_MRM_MISSES_MRM_MISSES(n) ((uint32_t)(n) << DCACHE_MRM_MISSES_MRM_MISSES_SHIFT)

/* MRM_EVICTS register */

#define DCACHE_MRM_EVICTS_MRM_EVICTS_SHIFT (0) /* Bits 0-31 */
#define DCACHE_MRM_EVICTS_MRM_EVICTS_MASK (0xffffffff << DCACHE_MRM_EVICTS_MRM_EVICTS_SHIFT)
#  define DCACHE_MRM_EVICTS_MRM_EVICTS(n) ((uint32_t)(n) << DCACHE_MRM_EVICTS_MRM_EVICTS_SHIFT)

/* MRM_CTRL register */

#define DCACHE_MRM_CTRL_MRM_START (1 << 0)                   /* Bit 0 */
#define DCACHE_MRM_CTRL_MRM_IRQ_MASK (1 << 1)                /* Bit 1 */
#define DCACHE_MRM_CTRL_MRM_IRQ_TINT_STATUS (1 << 2)         /* Bit 2 */
#define DCACHE_MRM_CTRL_MRM_IRQ_MISSES_THRES_STATUS (1 << 3) /* Bit 3 */
#define DCACHE_MRM_CTRL_MRM_IRQ_HITS_THRES_STATUS (1 << 4)   /* Bit 4 */
#define DCACHE_MRM_CTRL_MRM_IRQ_EVICTS_THRES_STATUS (1 << 5) /* Bit 5 */

/* MRM_TINT register */

#define DCACHE_MRM_TINT_MRM_TINT_SHIFT (0) /* Bits 0-18 */
#define DCACHE_MRM_TINT_MRM_TINT_MASK (0x7ffff << DCACHE_MRM_TINT_MRM_TINT_SHIFT)
#  define DCACHE_MRM_TINT_MRM_TINT(n) ((uint32_t)(n) << DCACHE_MRM_TINT_MRM_TINT_SHIFT)

/* MRM_MISSES_THRES register */

#define DCACHE_MRM_MISSES_THRES_MRM_MISSES_THRES_SHIFT (0) /* Bits 0-31 */
#define DCACHE_MRM_MISSES_THRES_MRM_MISSES_THRES_MASK (0xffffffff << DCACHE_MRM_MISSES_THRES_MRM_MISSES_THRES_SHIFT)
#  define DCACHE_MRM_MISSES_THRES_MRM_MISSES_THRES(n) ((uint32_t)(n) << DCACHE_MRM_MISSES_THRES_MRM_MISSES_THRES_SHIFT)

/* MRM_HITS_THRES register */

#define DCACHE_MRM_HITS_THRES_MRM_HITS_THRES_SHIFT (0) /* Bits 0-31 */
#define DCACHE_MRM_HITS_THRES_MRM_HITS_THRES_MASK (0xffffffff << DCACHE_MRM_HITS_THRES_MRM_HITS_THRES_SHIFT)
#  define DCACHE_MRM_HITS_THRES_MRM_HITS_THRES(n) ((uint32_t)(n) << DCACHE_MRM_HITS_THRES_MRM_HITS_THRES_SHIFT)

/* MRM_EVICTS_THRES register */

#define DCACHE_MRM_EVICTS_THRES_MRM_EVICTS_THRES_SHIFT (0) /* Bits 0-31 */
#define DCACHE_MRM_EVICTS_THRES_MRM_EVICTS_THRES_MASK (0xffffffff << DCACHE_MRM_EVICTS_THRES_MRM_EVICTS_THRES_SHIFT)
#  define DCACHE_MRM_EVICTS_THRES_MRM_EVICTS_THRES(n) ((uint32_t)(n) << DCACHE_MRM_EVICTS_THRES_MRM_EVICTS_THRES_SHIFT)

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_DCACHE_H */
