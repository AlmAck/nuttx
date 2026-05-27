/****************************************************************************
 * arch/arm/src/da1470x/da1470x_dma.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_DMA_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_DMA_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdbool.h>
#include <stdint.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* DMA bus width */

#define DA1470X_DMA_BW_8 0  /* 8-bit */
#define DA1470X_DMA_BW_16 1 /* 16-bit */
#define DA1470X_DMA_BW_32 2 /* 32-bit */

/* DMA burst mode */

#define DA1470X_DMA_BURST_1 0 /* 1 transfer */
#define DA1470X_DMA_BURST_4 1 /* 4 transfers */
#define DA1470X_DMA_BURST_8 2 /* 8 transfers */

/* DMA priority */

#define DA1470X_DMA_PRIO_0 0 /* Lowest */
#define DA1470X_DMA_PRIO_7 7 /* Highest */

/* DMA peripheral IDs */

#define DA1470X_DMA_PERIPH_SPI 0x0
#define DA1470X_DMA_PERIPH_SPI2 0x1
#define DA1470X_DMA_PERIPH_UART 0x2
#define DA1470X_DMA_PERIPH_UART2 0x3
#define DA1470X_DMA_PERIPH_I2C 0x4
#define DA1470X_DMA_PERIPH_I2C2 0x5
#define DA1470X_DMA_PERIPH_USB 0x6
#define DA1470X_DMA_PERIPH_UART3 0x7
#define DA1470X_DMA_PERIPH_PCM 0x8
#define DA1470X_DMA_PERIPH_SRC 0x9
#define DA1470X_DMA_PERIPH_SPI3 0xa
#define DA1470X_DMA_PERIPH_I2C3 0xb
#define DA1470X_DMA_PERIPH_ADC 0xc
#define DA1470X_DMA_PERIPH_SRC2 0xd
#define DA1470X_DMA_PERIPH_I3C 0xe
#define DA1470X_DMA_PERIPH_NONE 0xf

/****************************************************************************
 * Public Types
 ****************************************************************************/

typedef void *DMA_HANDLE;
typedef void (*dma_callback_t)(DMA_HANDLE handle, void *arg, int result);

struct da1470x_dma_config_s {
  uintptr_t src;         /* Source address */
  uintptr_t dest;        /* Destination address */
  uint16_t len;          /* Transfer length */
  uint16_t int_len;      /* Interrupt length */
  uint8_t bw;            /* Bus width (DA1470X_DMA_BW_*) */
  uint8_t burst;         /* Burst mode (DA1470X_DMA_BURST_*) */
  uint8_t prio;          /* Priority (0-7) */
  bool ainc;             /* Source address increment */
  bool binc;             /* Destination address increment */
  bool circular;         /* Circular mode */
  bool dreq;             /* HW triggered (true) or software (false) */
  uint8_t peripheral;    /* Peripheral ID (if dreq is true) */
  bool idle;             /* Non-blocking mode (can be interrupted) */
  bool init;             /* Memory initialization mode */
  bool req_sense;        /* Edge-sensitive request (true) or level (false) */
  bool bus_err_detect;   /* Enable bus error detection */
  bool exclusive_access; /* Enable exclusive access (do not de-assert bus
                            request) */
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_dmach_alloc
 *
 * Description:
 *   Allocate a DMA channel.
 *
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_dma_initialize
 *
 * Description:
 *   One-time DMA controller initialization. Brings up the PD_SNC power
 *   domain that hosts the DMA block, clears all channels into a safe
 *   state, and attaches the shared DMA NVIC interrupt. Safe to call
 *   more than once.
 *
 ****************************************************************************/

void da1470x_dma_initialize(void);

DMA_HANDLE da1470x_dmach_alloc(void);

/****************************************************************************
 * Name: da1470x_dmach_free
 *
 * Description:
 *   Release a DMA channel.
 *
 ****************************************************************************/

void da1470x_dmach_free(DMA_HANDLE handle);

/****************************************************************************
 * Name: da1470x_dma_setup
 *
 * Description:
 *   Configure a DMA channel for a transfer.
 *
 ****************************************************************************/

int da1470x_dma_setup(DMA_HANDLE handle,
                      const struct da1470x_dma_config_s *config,
                      dma_callback_t callback, void *arg);

/****************************************************************************
 * Name: da1470x_dma_start
 *
 * Description:
 *   Start the DMA transfer.
 *
 ****************************************************************************/

int da1470x_dma_start(DMA_HANDLE handle);

/****************************************************************************
 * Name: da1470x_dma_stop
 *
 * Description:
 *   Stop the DMA transfer.
 *
 ****************************************************************************/

int da1470x_dma_stop(DMA_HANDLE handle);

/****************************************************************************
 * Name: da1470x_dma_get_residue
 *
 * Description:
 *   Return the number of remaining items.
 *
 ****************************************************************************/

uint16_t da1470x_dma_get_residue(DMA_HANDLE handle);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_DMA_H */
