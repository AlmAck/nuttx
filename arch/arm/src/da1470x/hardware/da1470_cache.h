/****************************************************************************
 * ./tmp/da1470_cache.h
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

#ifndef __DA1470_CACHE_H
#define __DA1470_CACHE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register offsets *********************************************************/

#define DA1470_CACHE_CACHE_CTRL2_OFFSET             0x0020 /* Cache control register 2 (only Word (32-bits) access supported). */
#define DA1470_CACHE_CACHE_FLASH_OFFSET             0x0040 /* Cache QSPI Flash program size and base address register (only Word (32-bits) acc */
#define DA1470_CACHE_CACHE_MRM_CTRL_OFFSET          0x0030 /* Cache MRM (Miss Rate Monitor) CONTROL register (only Word (32-bits) access suppo */
#define DA1470_CACHE_CACHE_MRM_HITS1WS_OFFSET       0x0048 /* Cache MRM (Miss Rate Monitor) HITS with 1 Wait State register (only Word (32-bit */
#define DA1470_CACHE_CACHE_MRM_HITS_OFFSET          0x0028 /* Cache MRM (Miss Rate Monitor) HITS register (only Word (32-bits) access supporte */
#define DA1470_CACHE_CACHE_MRM_HITS_THRES_OFFSET    0x003C /* Cache MRM (Miss Rate Monitor) HITS THRESHOLD register (only Word (32-bits) acces */
#define DA1470_CACHE_CACHE_MRM_MISSES_OFFSET        0x002C /* Cache MRM (Miss Rate Monitor) MISSES register (only Word (32-bits) access suppor */
#define DA1470_CACHE_CACHE_MRM_MISSES_THRES_OFFSET  0x0038 /* Cache MRM (Miss Rate Monitor) THRESHOLD register (only Word (32-bits) access sup */
#define DA1470_CACHE_CACHE_MRM_TINT_OFFSET          0x0034 /* Cache MRM (Miss Rate Monitor) TIME INTERVAL register (only Word (32-bits) access */
#define DA1470_CACHE_SWD_RESET_OFFSET               0x0050 /* SWD HW reset control register (only Word (32-bits) access supported). */

/* Register addresses *******************************************************/

#define DA1470_CACHE_BASE                           0x100C0000 /* CACHE registers */

#define DA1470_CACHE_CACHE_CTRL2                    (DA1470_CACHE_BASE + DA1470_CACHE_CACHE_CTRL2_OFFSET) /* Cache control register 2 (only Word (32-bits) access supported). */
#define DA1470_CACHE_CACHE_FLASH                    (DA1470_CACHE_BASE + DA1470_CACHE_CACHE_FLASH_OFFSET) /* Cache QSPI Flash program size and base address register (only Word (32-bits) acc */
#define DA1470_CACHE_CACHE_MRM_CTRL                 (DA1470_CACHE_BASE + DA1470_CACHE_CACHE_MRM_CTRL_OFFSET) /* Cache MRM (Miss Rate Monitor) CONTROL register (only Word (32-bits) access suppo */
#define DA1470_CACHE_CACHE_MRM_HITS1WS              (DA1470_CACHE_BASE + DA1470_CACHE_CACHE_MRM_HITS1WS_OFFSET) /* Cache MRM (Miss Rate Monitor) HITS with 1 Wait State register (only Word (32-bit */
#define DA1470_CACHE_CACHE_MRM_HITS                 (DA1470_CACHE_BASE + DA1470_CACHE_CACHE_MRM_HITS_OFFSET) /* Cache MRM (Miss Rate Monitor) HITS register (only Word (32-bits) access supporte */
#define DA1470_CACHE_CACHE_MRM_HITS_THRES           (DA1470_CACHE_BASE + DA1470_CACHE_CACHE_MRM_HITS_THRES_OFFSET) /* Cache MRM (Miss Rate Monitor) HITS THRESHOLD register (only Word (32-bits) acces */
#define DA1470_CACHE_CACHE_MRM_MISSES               (DA1470_CACHE_BASE + DA1470_CACHE_CACHE_MRM_MISSES_OFFSET) /* Cache MRM (Miss Rate Monitor) MISSES register (only Word (32-bits) access suppor */
#define DA1470_CACHE_CACHE_MRM_MISSES_THRES         (DA1470_CACHE_BASE + DA1470_CACHE_CACHE_MRM_MISSES_THRES_OFFSET) /* Cache MRM (Miss Rate Monitor) THRESHOLD register (only Word (32-bits) access sup */
#define DA1470_CACHE_CACHE_MRM_TINT                 (DA1470_CACHE_BASE + DA1470_CACHE_CACHE_MRM_TINT_OFFSET) /* Cache MRM (Miss Rate Monitor) TIME INTERVAL register (only Word (32-bits) access */
#define DA1470_CACHE_SWD_RESET                      (DA1470_CACHE_BASE + DA1470_CACHE_SWD_RESET_OFFSET) /* SWD HW reset control register (only Word (32-bits) access supported). */

/* Register bit definitions *************************************************/


/* CACHE_CTRL2 Register */

#define CACHE_CACHE_READY                           (1U << 21) /* Cache Controller RO status bit. 0: Default. 1: Set to '1' when CACHE_CTRL is ena */
#define CACHE_CACHE_RAM_INIT                        (1U << 20) /* Cache Controller RO status bit. 0: Default. 1: Set to '1' when SRAM is being ini */
#define CACHE_CACHE_FLUSHED                         (1U << 19) /* 0: Cache is not flushed yet. 1: Cache is flushed.  Note 1: Setting and clearing  */
#define CACHE_CACHE_FLUSH_DISABLE                   (1U << 18) /* 0: Default. 1: Flushing of the Cache memory is disabled when SYS_CTRL_REG[CACHER */
#define CACHE_CACHE_USE_FULL_DB_RANGE_MASK          (0x3 << 16) /* 00: CACHERAM (mirrored) read/write and NO use of the full 184 bits databus (for  */
#define CACHE_CACHE_USE_FULL_DB_RANGE_POS           (16) /* 00: CACHERAM (mirrored) read/write and NO use of the full 184 bits databus (for  */
#define CACHE_CACHE_MHCLKEN_DISABLE                 (1U << 15) /* 0: Default. 1: The "m_HCLK_EN" input is ignored and the controller avoids insert */
#define CACHE_CACHE_CWF_DISABLE                     (1U << 14) /* 0: Default. 1: The cache line refill is performed with INCR type burst and "Crit */
#define CACHE_CACHE_CGEN                            (1U << 13) /* 0: Cache controller clock gating is not enabled. 1: Cache controller clock gatin */
#define CACHE_CACHE_WEN                             (1U << 12) /* 0: Cache Data and TAG memory read only. 1: Cache Data and TAG memory read/write. */
#define CACHE_CACHE_LEN_MASK                        (0xFFF << 0) /* Length of OQSPI FLASH cacheable memory. N*64 KBytes. N = 0 to 2048, incl. 2048 ( */
#define CACHE_CACHE_LEN_POS                         (0) /* Length of OQSPI FLASH cacheable memory. N*64 KBytes. N = 0 to 2048, incl. 2048 ( */

/* CACHE_FLASH Register */

#define CACHE_FLASH_REGION_BASE_MASK                (0xFFFF << 16) /* These bits corresponds with the Flash region base address bits [31:16]. Default  */
#define CACHE_FLASH_REGION_BASE_POS                 (16) /* These bits corresponds with the Flash region base address bits [31:16]. Default  */
#define CACHE_FLASH_REGION_OFFSET_MASK              (0xFFF << 4) /* Flash region offset address (in words). This value is added to the Flash (CPU) a */
#define CACHE_FLASH_REGION_OFFSET_POS               (4) /* Flash region offset address (in words). This value is added to the Flash (CPU) a */
#define CACHE_FLASH_REGION_SIZE_MASK                (0xF << 0) /* Flash region size. Default value is '1' (0.5 MBytes). 0 = 0.25 MBytes, 1 = 0.5 M */
#define CACHE_FLASH_REGION_SIZE_POS                 (0) /* Flash region size. Default value is '1' (0.5 MBytes). 0 = 0.25 MBytes, 1 = 0.5 M */

/* CACHE_MRM_CTRL Register */

#define CACHE_MRM_IRQ_HITS_THRES_STATUS             (1U << 4) /* 0: No interrupt is generated. 1: Interrupt (pulse-sensitive) is generated becaus */
#define CACHE_MRM_IRQ_MISSES_THRES_STATUS           (1U << 3) /* 0: No interrupt is generated. 1: Interrupt (pulse-sensitive) is generated becaus */
#define CACHE_MRM_IRQ_TINT_STATUS                   (1U << 2) /* 0: No interrupt is generated. 1: Interrupt (pulse-sensitive) is generated becaus */
#define CACHE_MRM_IRQ_MASK                          (1U << 1) /* 0: Disables interrupt generation. 1: Enables interrupt generation. Note: The Cac */
#define CACHE_MRM_START                             (1U << 0) /* 0: Freeze the "misses/hits" counters and reset the time interval counter to the  */

/* CACHE_MRM_HITS1WS Register */

#define CACHE_MRM_HITS1WS_MASK                      (0xFFFFFFFF << 0) /* Contains the amount of cache hits. */
#define CACHE_MRM_HITS1WS_POS                       (0) /* Contains the amount of cache hits. */

/* CACHE_MRM_HITS Register */

#define CACHE_MRM_HITS_MASK                         (0xFFFFFFFF << 0) /* Contains the amount of cache hits. */
#define CACHE_MRM_HITS_POS                          (0) /* Contains the amount of cache hits. */

/* CACHE_MRM_HITS_THRES Register */

#define CACHE_MRM_HITS_THRES_MASK                   (0xFFFFFFFF << 0) /* Defines the hits threshold to trigger the interrupt generation. See also the des */
#define CACHE_MRM_HITS_THRES_POS                    (0) /* Defines the hits threshold to trigger the interrupt generation. See also the des */

/* CACHE_MRM_MISSES Register */

#define CACHE_MRM_MISSES_MASK                       (0xFFFFFFFF << 0) /* Contains the amount of cache misses. */
#define CACHE_MRM_MISSES_POS                        (0) /* Contains the amount of cache misses. */

/* CACHE_MRM_MISSES_THRES Register */

#define CACHE_MRM_MISSES_THRES_MASK                 (0xFFFFFFFF << 0) /* Defines the misses threshold to trigger the interrupt generation. See also the d */
#define CACHE_MRM_MISSES_THRES_POS                  (0) /* Defines the misses threshold to trigger the interrupt generation. See also the d */

/* CACHE_MRM_TINT Register */

#define CACHE_MRM_TINT_MASK                         (0x7FFFF << 0) /* Defines the time interval for the monitoring in 32 MHz clock cycles. See also th */
#define CACHE_MRM_TINT_POS                          (0) /* Defines the time interval for the monitoring in 32 MHz clock cycles. See also th */

/* SWD_RESET Register */

#define CACHE_SWD_HW_RESET_REQ                      (1U << 0) /* 0: default. 1: HW reset request (from the debugger tool). The register is automa */

#endif /* __DA1470_CACHE_H */
