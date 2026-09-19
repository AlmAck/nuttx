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

#include "hardware/da1470x_dma.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Bus width */

#define DA1470X_DMA_BW_8          0
#define DA1470X_DMA_BW_16         1
#define DA1470X_DMA_BW_32         2

/* Burst mode */

#define DA1470X_DMA_BURST_1       0
#define DA1470X_DMA_BURST_4       1
#define DA1470X_DMA_BURST_8       2

/* Priority */

#define DA1470X_DMA_PRIO_LOWEST   0
#define DA1470X_DMA_PRIO_HIGHEST  7

/* Peripheral request sources (DMA_REQ_MUX encodings).  Peripheral
 * channels come in pairs: the even channel is RX (peripheral to memory)
 * and the odd channel is TX (memory to peripheral) for the same source.
 */

#define DA1470X_DMA_PERIPH_SPI0   DMA_REQ_MUX_SPI
#define DA1470X_DMA_PERIPH_SPI1   DMA_REQ_MUX_SPI2
#define DA1470X_DMA_PERIPH_UART0  DMA_REQ_MUX_UART
#define DA1470X_DMA_PERIPH_UART1  DMA_REQ_MUX_UART2
#define DA1470X_DMA_PERIPH_I2C0   DMA_REQ_MUX_I2C
#define DA1470X_DMA_PERIPH_I2C1   DMA_REQ_MUX_I2C2
#define DA1470X_DMA_PERIPH_USB    DMA_REQ_MUX_USB
#define DA1470X_DMA_PERIPH_UART2  DMA_REQ_MUX_UART3
#define DA1470X_DMA_PERIPH_PCM    DMA_REQ_MUX_PCM
#define DA1470X_DMA_PERIPH_SRC    DMA_REQ_MUX_SRC
#define DA1470X_DMA_PERIPH_SPI2   DMA_REQ_MUX_SPI3
#define DA1470X_DMA_PERIPH_I2C2   DMA_REQ_MUX_I2C3
#define DA1470X_DMA_PERIPH_GPADC  DMA_REQ_MUX_GPADC
#define DA1470X_DMA_PERIPH_SRC2   DMA_REQ_MUX_SRC2
#define DA1470X_DMA_PERIPH_I3C    DMA_REQ_MUX_I3C
#define DA1470X_DMA_PERIPH_NONE   DMA_REQ_MUX_NONE

/* Channel allocation hints */

#define DA1470X_DMA_ANY_CHANNEL   (-1)

/****************************************************************************
 * Public Types
 ****************************************************************************/

typedef void *DMA_HANDLE;
typedef void (*dma_callback_t)(DMA_HANDLE handle, void *arg, int result);

struct da1470x_dma_config_s
{
  uintptr_t src;         /* Source address */
  uintptr_t dest;        /* Destination address */
  uint16_t  len;         /* Number of items to move, in bus-width units */
  uint16_t  int_len;     /* Interrupt after this many items */
  uint8_t   bw;          /* Bus width (DA1470X_DMA_BW_*) */
  uint8_t   burst;       /* Burst mode (DA1470X_DMA_BURST_*) */
  uint8_t   prio;        /* Priority 0..7 */
  uint8_t   peripheral;  /* Request source when dreq is true */
  bool      ainc;        /* Increment source address */
  bool      binc;        /* Increment destination address */
  bool      circular;    /* Circular mode */
  bool      dreq;        /* Peripheral (true) or software (false) trigger */
  bool      idle;        /* Interruptible transfer */
  bool      init;        /* Memory fill: source is a constant */
  bool      req_sense;   /* Edge (true) or level (false) request sense */
  bool      bus_err_detect;
  bool      exclusive_access;
};

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
 * Name: da1470x_dma_initialize
 *
 * Description:
 *   Initialize the DMA controller and attach its interrupt.  Safe to call
 *   more than once.
 *
 ****************************************************************************/

void da1470x_dma_initialize(void);

/****************************************************************************
 * Name: da1470x_dmach_alloc
 *
 * Description:
 *   Allocate a DMA channel.  chan is a specific channel number or
 *   DA1470X_DMA_ANY_CHANNEL.  Returns NULL if none is available.
 *
 ****************************************************************************/

DMA_HANDLE da1470x_dmach_alloc(int chan);

/****************************************************************************
 * Name: da1470x_dmach_free
 ****************************************************************************/

void da1470x_dmach_free(DMA_HANDLE handle);

/****************************************************************************
 * Name: da1470x_dma_setup
 *
 * Description:
 *   Program a channel for a transfer described by config.  The callback is
 *   invoked from interrupt context with result OK or -EIO.
 *
 ****************************************************************************/

int da1470x_dma_setup(DMA_HANDLE handle,
                      const struct da1470x_dma_config_s *config,
                      dma_callback_t callback, void *arg);

/****************************************************************************
 * Name: da1470x_dma_start / da1470x_dma_stop
 ****************************************************************************/

int da1470x_dma_start(DMA_HANDLE handle);
int da1470x_dma_stop(DMA_HANDLE handle);

/****************************************************************************
 * Name: da1470x_dma_get_residue
 *
 * Description:
 *   Number of transfers remaining on the channel.
 *
 ****************************************************************************/

uint16_t da1470x_dma_get_residue(DMA_HANDLE handle);

/****************************************************************************
 * Name: da1470x_dma_channel
 *
 * Description:
 *   Return the channel number of a handle.
 *
 ****************************************************************************/

int da1470x_dma_channel(DMA_HANDLE handle);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_DMA_H */
