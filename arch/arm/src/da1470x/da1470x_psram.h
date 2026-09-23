/****************************************************************************
 * arch/arm/src/da1470x/da1470x_psram.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_PSRAM_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_PSRAM_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "hardware/da1470x_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Where the memory appears once the controller is in its automatic mode.
 * The window is 128 MiB of address space; how much of it is backed by
 * memory depends on the part fitted.
 */

#define DA1470X_PSRAM_BASE  DA1470X_QSPIC2_MEM_BASE

#ifdef CONFIG_DA1470X_PSRAM_SIZE
#  define DA1470X_PSRAM_SIZE CONFIG_DA1470X_PSRAM_SIZE
#else
#  define DA1470X_PSRAM_SIZE (8 * 1024 * 1024)
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Name: da1470x_psram_initialize
 *
 * Description:
 *   Bring up an AP Memory APS6404 quad-SPI PSRAM on QSPIC2 and map it at
 *   DA1470X_PSRAM_BASE, readable and writable by the processor.
 *
 *   The part is identified before anything else is done to it, so a board
 *   without one, or with a wiring fault, fails here with -ENODEV rather
 *   than handing out memory that is not there.  The window is then tested
 *   for stuck and shorted address and data lines before it is declared
 *   usable.
 *
 *   The window is cacheable.  That is what you want for memory the
 *   processor uses; it is not what you want for memory a DMA engine
 *   reads, which has to be cleaned from the cache first.
 *
 * Returned Value:
 *   Zero on success, a negated errno otherwise.  On failure the window
 *   must not be touched: an access to it with the controller not in its
 *   automatic mode is ignored, and a write with it not in RAM mode is a
 *   hard fault.
 *
 ****************************************************************************/

int da1470x_psram_initialize(void);

/****************************************************************************
 * Name: da1470x_psram_clock_changed
 *
 * Description:
 *   Tell the driver the system clock has moved, so that it can move the
 *   point at which it samples read data.  A no-op when there is no PSRAM.
 *
 ****************************************************************************/

void da1470x_psram_clock_changed(bool fast);

#ifdef CONFIG_DA1470X_PSRAM_HEAP
/****************************************************************************
 * Name: da1470x_psram_malloc / da1470x_psram_free
 *
 * Description:
 *   Allocate from a heap of its own that covers the PSRAM, kept separate
 *   from the system heap so that nothing lands out there by accident.
 *   Memory from here is slower than internal RAM and, being cacheable,
 *   has to be cleaned before a DMA engine reads it.
 *
 ****************************************************************************/

void *da1470x_psram_malloc(size_t size);
void  da1470x_psram_free(void *mem);
#endif

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_PSRAM_H */
