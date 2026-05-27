/****************************************************************************
 * arch/arm/src/da1470x/da1470x_dma.c
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <assert.h>
#include <debug.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>

#include <nuttx/arch.h>
#include <nuttx/irq.h>

#include "arm_internal.h"
#include "da1470x_dma.h"
#include "da1470x_irq.h"
#include "da1470x_pmu.h"
#include "hardware/da1470x_dma.h"

/* `dmaerr` and `OK` come from <debug.h> / <errno.h> respectively. */

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define DA1470X_DMA_NCHANNELS 8

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct da1470x_dmach_s {
  uint8_t chan;            /* Channel number (0-7) */
  bool used;               /* True if channel is allocated */
  uintptr_t base;          /* Base address of channel registers */
  dma_callback_t callback; /* User callback */
  void *arg;               /* User argument */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int da1470x_dma_interrupt(int irq, void *context, void *arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct da1470x_dmach_s g_dmach[DA1470X_DMA_NCHANNELS] = {
    {.chan = 0, .base = DA1470_DMA_BASE + 0x00},
    {.chan = 1, .base = DA1470_DMA_BASE + 0x20},
    {.chan = 2, .base = DA1470_DMA_BASE + 0x40},
    {.chan = 3, .base = DA1470_DMA_BASE + 0x60},
    {.chan = 4, .base = DA1470_DMA_BASE + 0x80},
    {.chan = 5, .base = DA1470_DMA_BASE + 0xA0},
    {.chan = 6, .base = DA1470_DMA_BASE + 0xC0},
    {.chan = 7, .base = DA1470_DMA_BASE + 0xE0},
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_dma_interrupt
 ****************************************************************************/

static int da1470x_dma_interrupt(int irq, void *context, void *arg) {
  uint32_t status;
  int i;

  /* Read interrupt status */

  status = getreg32(DA1470_DMA_DMA_INT_STATUS);

  /* Process each channel */

  for (i = 0; i < DA1470X_DMA_NCHANNELS; i++) {
    if (status & (1 << i)) {
      struct da1470x_dmach_s *dmachan = &g_dmach[i];
      int result = OK;

      /* Check for bus error */

      if (status & (1 << (i + 8))) {
        result = -EIO;
        dmaerr("DMA channel %d bus error detected\n", i);
      }

      /* Clear interrupt */

      putreg32(1 << i, DA1470_DMA_DMA_CLEAR_INT);

      /* Invoke callback */

      if (dmachan->callback) {
        dmachan->callback((DMA_HANDLE)dmachan, dmachan->arg, result);
      }
    }
  }

  return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_dma_initialize
 ****************************************************************************/

void da1470x_dma_initialize(void) {
  static bool initialized = false;
  int i;

  if (initialized) {
    return;
  }

  /* DMA controller lives in PD_SNC -- bring the domain up so register
   * writes stick. lowsetup() already does this for the console UART,
   * but da1470x_dma_initialize() may also be called from a board
   * bring-up path that runs before any UART, so be defensive.
   */

  da1470x_pd_enable(DA1470X_PD_SNC);

  /* Put every channel in a known-stopped state and clear any latched
   * interrupt status.
   */

  for (i = 0; i < DA1470X_DMA_NCHANNELS; i++) {
    putreg32(0, g_dmach[i].base + DA1470_DMA_DMA0_CTRL_OFFSET);
  }
  putreg32(0xff, DA1470_DMA_DMA_CLEAR_INT);
  putreg32(0, DA1470_DMA_DMA_INT_MASK);

  /* Attach the shared DMA interrupt now (used to be done lazily on the
   * first da1470x_dma_start, which raced if two callers started at the
   * same time).
   */

  irq_attach(DA1470X_IRQ_DMA, da1470x_dma_interrupt, NULL);
  up_enable_irq(DA1470X_IRQ_DMA);

  initialized = true;
}

/****************************************************************************
 * Name: da1470x_dmach_alloc
 ****************************************************************************/

DMA_HANDLE da1470x_dmach_alloc(void) {
  irqstate_t flags;
  int i;

  flags = enter_critical_section();

  for (i = 0; i < DA1470X_DMA_NCHANNELS; i++) {
    if (!g_dmach[i].used) {
      g_dmach[i].used = true;

      /* Initialize the channel in a safe state */

      putreg32(0, g_dmach[i].base + DA1470_DMA_DMA0_CTRL_OFFSET);

      leave_critical_section(flags);
      return (DMA_HANDLE)&g_dmach[i];
    }
  }

  leave_critical_section(flags);
  return NULL;
}

/****************************************************************************
 * Name: da1470x_dmach_free
 ****************************************************************************/

void da1470x_dmach_free(DMA_HANDLE handle) {
  struct da1470x_dmach_s *dmachan = (struct da1470x_dmach_s *)handle;
  irqstate_t flags;

  DEBUGASSERT(dmachan != NULL && dmachan->used);

  flags = enter_critical_section();

  /* Stop DMA and disable interrupts for this channel */

  putreg32(0, dmachan->base + DA1470_DMA_DMA0_CTRL_OFFSET);
  modreg32(0, 1 << dmachan->chan, DA1470_DMA_DMA_INT_MASK);

  dmachan->used = false;
  dmachan->callback = NULL;

  leave_critical_section(flags);
}

/****************************************************************************
 * Name: da1470x_dma_setup
 ****************************************************************************/

int da1470x_dma_setup(DMA_HANDLE handle,
                      const struct da1470x_dma_config_s *config,
                      dma_callback_t callback, void *arg) {
  struct da1470x_dmach_s *dmachan = (struct da1470x_dmach_s *)handle;
  uint32_t ctrl = 0;

  DEBUGASSERT(dmachan != NULL && config != NULL);

  dmachan->callback = callback;
  dmachan->arg = arg;

  /* Set addresses and length */

  putreg32(config->src, dmachan->base + DA1470_DMA_DMA0_A_START_OFFSET);
  putreg32(config->dest, dmachan->base + DA1470_DMA_DMA0_B_START_OFFSET);
  putreg32(config->len, dmachan->base + DA1470_DMA_DMA0_LEN_OFFSET);
  putreg32(config->int_len, dmachan->base + DA1470_DMA_DMA0_INT_OFFSET);

  /* Configure CTRL register */

  if (config->ainc) {
    ctrl |= DMA_DMA0_CTRL_AINC;
  }

  if (config->binc) {
    ctrl |= DMA_DMA0_CTRL_BINC;
  }

  if (config->circular) {
    ctrl |= DMA_DMA0_CTRL_CIRCULAR;
  }

  if (config->idle) {
    ctrl |= DMA_DMA0_CTRL_DMA_IDLE;
  }

  if (config->init) {
    ctrl |= DMA_DMA0_CTRL_DMA_INIT;

    /* Per datasheet: AINC must be 0 and BINC must be 1 for DMA_INIT */

    ctrl &= ~DMA_DMA0_CTRL_AINC;
    ctrl |= DMA_DMA0_CTRL_BINC;
  }

  if (config->req_sense) {
    ctrl |= DMA_DMA0_CTRL_REQ_SENSE;
  }

  if (config->bus_err_detect) {
    ctrl |= DMA_DMA0_CTRL_BUS_ERROR_DETECT;
  }

  if (config->exclusive_access) {
    ctrl |= DMA_DMA0_CTRL_DMA_EXCLUSIVE_ACCESS;
  }

  if (config->dreq) {
    ctrl |= DMA_DMA0_CTRL_DREQ_MODE;

    /* Configure MUX for peripheral request */

    uint32_t mux_mask = 0xf << (4 * (dmachan->chan / 2));
    uint32_t mux_val = (config->peripheral & 0xf) << (4 * (dmachan->chan / 2));

    modreg32(mux_val, mux_mask, DA1470_DMA_DMA_REQ_MUX);
  }

  ctrl |= (config->bw << DMA_DMA0_CTRL_BW_POS) & DMA_DMA0_CTRL_BW_MASK;
  ctrl |= (config->burst << DMA_DMA0_CTRL_BURST_MODE_POS) &
          DMA_DMA0_CTRL_BURST_MODE_MASK;
  ctrl |= (config->prio << DMA_DMA0_CTRL_DMA_PRIO_POS) &
          DMA_DMA0_CTRL_DMA_PRIO_MASK;

  putreg32(ctrl, dmachan->base + DA1470_DMA_DMA0_CTRL_OFFSET);

  return OK;
}

/****************************************************************************
 * Name: da1470x_dma_start
 ****************************************************************************/

int da1470x_dma_start(DMA_HANDLE handle) {
  struct da1470x_dmach_s *dmachan = (struct da1470x_dmach_s *)handle;

  /* Enable interrupt for this channel */

  modreg32(1 << dmachan->chan, 1 << dmachan->chan, DA1470_DMA_DMA_INT_MASK);

  /* Turn on DMA */

  modreg32(DMA_DMA0_CTRL_DMA_ON, DMA_DMA0_CTRL_DMA_ON,
           dmachan->base + DA1470_DMA_DMA0_CTRL_OFFSET);

  return OK;
}

/****************************************************************************
 * Name: da1470x_dma_stop
 ****************************************************************************/

int da1470x_dma_stop(DMA_HANDLE handle) {
  struct da1470x_dmach_s *dmachan = (struct da1470x_dmach_s *)handle;

  /* Turn off DMA */

  modreg32(0, DMA_DMA0_CTRL_DMA_ON,
           dmachan->base + DA1470_DMA_DMA0_CTRL_OFFSET);

  /* Disable interrupt for this channel */

  modreg32(0, 1 << dmachan->chan, DA1470_DMA_DMA_INT_MASK);

  return OK;
}

/****************************************************************************
 * Name: da1470x_dma_get_residue
 ****************************************************************************/

uint16_t da1470x_dma_get_residue(DMA_HANDLE handle) {
  struct da1470x_dmach_s *dmachan = (struct da1470x_dmach_s *)handle;
  uint16_t len = getreg32(dmachan->base + DA1470_DMA_DMA0_LEN_OFFSET);
  uint16_t idx = getreg32(dmachan->base + DA1470_DMA_DMA0_IDX_OFFSET);

  return len - idx;
}
