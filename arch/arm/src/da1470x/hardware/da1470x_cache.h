/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_cache.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_CACHE_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_CACHE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470x_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define DA1470X_CACHE_CTRL2_OFFSET 0x0020            /* Cache control register 2 (only Word (32- */
#define DA1470X_CACHE_MRM_HITS_OFFSET 0x0028         /* Cache MRM (Miss Rate Monitor) HITS regis */
#define DA1470X_CACHE_MRM_MISSES_OFFSET 0x002c       /* Cache MRM (Miss Rate Monitor) MISSES reg */
#define DA1470X_CACHE_MRM_CTRL_OFFSET 0x0030         /* Cache MRM (Miss Rate Monitor) CONTROL re */
#define DA1470X_CACHE_MRM_TINT_OFFSET 0x0034         /* Cache MRM (Miss Rate Monitor) TIME INTER */
#define DA1470X_CACHE_MRM_MISSES_THRES_OFFSET 0x0038 /* Cache MRM (Miss Rate Monitor) THRESHOLD */
#define DA1470X_CACHE_MRM_HITS_THRES_OFFSET 0x003c   /* Cache MRM (Miss Rate Monitor) HITS THRES */
#define DA1470X_CACHE_FLASH_OFFSET 0x0040            /* Cache QSPI Flash program size and base a */
#define DA1470X_CACHE_MRM_HITS1WS_OFFSET 0x0048      /* Cache MRM (Miss Rate Monitor) HITS with */
#define DA1470X_CACHE_SWD_RESET_OFFSET 0x0050        /* SWD HW reset control register (only Word */

/* Register Addresses *******************************************************/

#define DA1470X_CACHE_CTRL2 (DA1470X_CACHE_BASE + DA1470X_CACHE_CTRL2_OFFSET)
#define DA1470X_CACHE_MRM_HITS (DA1470X_CACHE_BASE + DA1470X_CACHE_MRM_HITS_OFFSET)
#define DA1470X_CACHE_MRM_MISSES (DA1470X_CACHE_BASE + DA1470X_CACHE_MRM_MISSES_OFFSET)
#define DA1470X_CACHE_MRM_CTRL (DA1470X_CACHE_BASE + DA1470X_CACHE_MRM_CTRL_OFFSET)
#define DA1470X_CACHE_MRM_TINT (DA1470X_CACHE_BASE + DA1470X_CACHE_MRM_TINT_OFFSET)
#define DA1470X_CACHE_MRM_MISSES_THRES (DA1470X_CACHE_BASE + DA1470X_CACHE_MRM_MISSES_THRES_OFFSET)
#define DA1470X_CACHE_MRM_HITS_THRES (DA1470X_CACHE_BASE + DA1470X_CACHE_MRM_HITS_THRES_OFFSET)
#define DA1470X_CACHE_FLASH (DA1470X_CACHE_BASE + DA1470X_CACHE_FLASH_OFFSET)
#define DA1470X_CACHE_MRM_HITS1WS (DA1470X_CACHE_BASE + DA1470X_CACHE_MRM_HITS1WS_OFFSET)
#define DA1470X_CACHE_SWD_RESET (DA1470X_CACHE_BASE + DA1470X_CACHE_SWD_RESET_OFFSET)

/* Register Bit-field Definitions *******************************************/

/* CTRL2 register */

#define CACHE_CTRL2_LEN_SHIFT (0)                /* Bits 0-11 */
#define CACHE_CTRL2_LEN_MASK (0xfff << CACHE_CTRL2_LEN_SHIFT)
#  define CACHE_CTRL2_LEN(n) ((uint32_t)(n) << CACHE_CTRL2_LEN_SHIFT)
#define CACHE_CTRL2_WEN (1 << 12)                /* Bit 12 */
#define CACHE_CTRL2_CGEN (1 << 13)               /* Bit 13 */
#define CACHE_CTRL2_CWF_DISABLE (1 << 14)        /* Bit 14 */
#define CACHE_CTRL2_MHCLKEN_DISABLE (1 << 15)    /* Bit 15 */
#define CACHE_CTRL2_USE_FULL_DB_RANGE_SHIFT (16) /* Bits 16-17 */
#define CACHE_CTRL2_USE_FULL_DB_RANGE_MASK (0x3 << CACHE_CTRL2_USE_FULL_DB_RANGE_SHIFT)
#  define CACHE_CTRL2_USE_FULL_DB_RANGE(n) ((uint32_t)(n) << CACHE_CTRL2_USE_FULL_DB_RANGE_SHIFT)
#define CACHE_CTRL2_FLUSH_DISABLE (1 << 18)      /* Bit 18 */
#define CACHE_CTRL2_FLUSHED (1 << 19)            /* Bit 19 */
#define CACHE_CTRL2_RAM_INIT (1 << 20)           /* Bit 20 */
#define CACHE_CTRL2_READY (1 << 21)              /* Bit 21 */

/* MRM_HITS register */

#define CACHE_MRM_HITS_MRM_HITS_SHIFT (0) /* Bits 0-31 */
#define CACHE_MRM_HITS_MRM_HITS_MASK (0xffffffff << CACHE_MRM_HITS_MRM_HITS_SHIFT)
#  define CACHE_MRM_HITS_MRM_HITS(n) ((uint32_t)(n) << CACHE_MRM_HITS_MRM_HITS_SHIFT)

/* MRM_MISSES register */

#define CACHE_MRM_MISSES_MRM_MISSES_SHIFT (0) /* Bits 0-31 */
#define CACHE_MRM_MISSES_MRM_MISSES_MASK (0xffffffff << CACHE_MRM_MISSES_MRM_MISSES_SHIFT)
#  define CACHE_MRM_MISSES_MRM_MISSES(n) ((uint32_t)(n) << CACHE_MRM_MISSES_MRM_MISSES_SHIFT)

/* MRM_CTRL register */

#define CACHE_MRM_CTRL_MRM_START (1 << 0)                   /* Bit 0 */
#define CACHE_MRM_CTRL_MRM_IRQ_MASK (1 << 1)                /* Bit 1 */
#define CACHE_MRM_CTRL_MRM_IRQ_TINT_STATUS (1 << 2)         /* Bit 2 */
#define CACHE_MRM_CTRL_MRM_IRQ_MISSES_THRES_STATUS (1 << 3) /* Bit 3 */
#define CACHE_MRM_CTRL_MRM_IRQ_HITS_THRES_STATUS (1 << 4)   /* Bit 4 */

/* MRM_TINT register */

#define CACHE_MRM_TINT_MRM_TINT_SHIFT (0) /* Bits 0-18 */
#define CACHE_MRM_TINT_MRM_TINT_MASK (0x7ffff << CACHE_MRM_TINT_MRM_TINT_SHIFT)
#  define CACHE_MRM_TINT_MRM_TINT(n) ((uint32_t)(n) << CACHE_MRM_TINT_MRM_TINT_SHIFT)

/* MRM_MISSES_THRES register */

#define CACHE_MRM_MISSES_THRES_MRM_MISSES_THRES_SHIFT (0) /* Bits 0-31 */
#define CACHE_MRM_MISSES_THRES_MRM_MISSES_THRES_MASK (0xffffffff << CACHE_MRM_MISSES_THRES_MRM_MISSES_THRES_SHIFT)
#  define CACHE_MRM_MISSES_THRES_MRM_MISSES_THRES(n) ((uint32_t)(n) << CACHE_MRM_MISSES_THRES_MRM_MISSES_THRES_SHIFT)

/* MRM_HITS_THRES register */

#define CACHE_MRM_HITS_THRES_MRM_HITS_THRES_SHIFT (0) /* Bits 0-31 */
#define CACHE_MRM_HITS_THRES_MRM_HITS_THRES_MASK (0xffffffff << CACHE_MRM_HITS_THRES_MRM_HITS_THRES_SHIFT)
#  define CACHE_MRM_HITS_THRES_MRM_HITS_THRES(n) ((uint32_t)(n) << CACHE_MRM_HITS_THRES_MRM_HITS_THRES_SHIFT)

/* FLASH register */

#define CACHE_FLASH_FLASH_REGION_SIZE_SHIFT (0)   /* Bits 0-3 */
#define CACHE_FLASH_FLASH_REGION_SIZE_MASK (0xf << CACHE_FLASH_FLASH_REGION_SIZE_SHIFT)
#  define CACHE_FLASH_FLASH_REGION_SIZE(n) ((uint32_t)(n) << CACHE_FLASH_FLASH_REGION_SIZE_SHIFT)
#define CACHE_FLASH_FLASH_REGION_OFFSET_SHIFT (4) /* Bits 4-15 */
#define CACHE_FLASH_FLASH_REGION_OFFSET_MASK (0xfff << CACHE_FLASH_FLASH_REGION_OFFSET_SHIFT)
#  define CACHE_FLASH_FLASH_REGION_OFFSET(n) ((uint32_t)(n) << CACHE_FLASH_FLASH_REGION_OFFSET_SHIFT)
#define CACHE_FLASH_FLASH_REGION_BASE_SHIFT (16)  /* Bits 16-31 */
#define CACHE_FLASH_FLASH_REGION_BASE_MASK (0xffff << CACHE_FLASH_FLASH_REGION_BASE_SHIFT)
#  define CACHE_FLASH_FLASH_REGION_BASE(n) ((uint32_t)(n) << CACHE_FLASH_FLASH_REGION_BASE_SHIFT)

/* MRM_HITS1WS register */

#define CACHE_MRM_HITS1WS_MRM_HITS1WS_SHIFT (0) /* Bits 0-31 */
#define CACHE_MRM_HITS1WS_MRM_HITS1WS_MASK (0xffffffff << CACHE_MRM_HITS1WS_MRM_HITS1WS_SHIFT)
#  define CACHE_MRM_HITS1WS_MRM_HITS1WS(n) ((uint32_t)(n) << CACHE_MRM_HITS1WS_MRM_HITS1WS_SHIFT)

/* SWD_RESET register */

#define CACHE_SWD_RESET_SWD_HW_RESET_REQ (1 << 0) /* Bit 0 */

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_CACHE_H */
