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
#include "da1470x_pmu.h"
#include "hardware/da1470x_dma.h"

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct da1470x_dmach_s
{
  uint8_t        chan;      /* Channel number (0-7) */
  bool           used;      /* True if channel is allocated */
  uintptr_t      base;      /* Base address of channel registers */
  dma_callback_t callback;  /* User callback */
  void          *arg;       /* User argument */
};

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct da1470x_dmach_s g_dmach[DA1470X_DMA_NCHANNELS] =
{
  { .chan = 0, .base = DA1470X_DMA_CHAN_BASE(0) },
  { .chan = 1, .base = DA1470X_DMA_CHAN_BASE(1) },
  { .chan = 2, .base = DA1470X_DMA_CHAN_BASE(2) },
  { .chan = 3, .base = DA1470X_DMA_CHAN_BASE(3) },
  { .chan = 4, .base = DA1470X_DMA_CHAN_BASE(4) },
  { .chan = 5, .base = DA1470X_DMA_CHAN_BASE(5) },
  { .chan = 6, .base = DA1470X_DMA_CHAN_BASE(6) },
  { .chan = 7, .base = DA1470X_DMA_CHAN_BASE(7) },
};

static bool g_dma_initialized;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_dma_interrupt
 *
 * Description:
 *   Shared interrupt of all eight channels.  Bits 0-7 of INT_STATUS flag
 *   completion, bits 8-15 flag a bus error on the same channel.
 *
 ****************************************************************************/

static int da1470x_dma_interrupt(int irq, void *context, void *arg)
{
  uint32_t status;
  int i;

  status = getreg32(DA1470X_DMA_INT_STATUS);

  for (i = 0; i < DA1470X_DMA_NCHANNELS; i++)
    {
      if ((status & (1u << i)) != 0)
        {
          struct da1470x_dmach_s *dmach = &g_dmach[i];
          int result = OK;

          if ((status & (1u << (i + 8))) != 0)
            {
              result = -EIO;
              dmaerr("DMA channel %d bus error\n", i);
            }

          putreg32(1u << i, DA1470X_DMA_CLEAR_INT);

          if (dmach->callback != NULL)
            {
              dmach->callback((DMA_HANDLE)dmach, dmach->arg, result);
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

void da1470x_dma_initialize(void)
{
  int i;

  if (g_dma_initialized)
    {
      return;
    }

  /* The DMA controller lives in PD_SNC */

  da1470x_pd_enable(DA1470X_PD_SNC);

  for (i = 0; i < DA1470X_DMA_NCHANNELS; i++)
    {
      putreg32(0, g_dmach[i].base + DA1470X_DMA_CTRL_OFFSET);
    }

  putreg32(0xff, DA1470X_DMA_CLEAR_INT);
  putreg32(0, DA1470X_DMA_INT_MASK);

  irq_attach(DA1470X_IRQ_DMA, da1470x_dma_interrupt, NULL);
  up_enable_irq(DA1470X_IRQ_DMA);

  g_dma_initialized = true;
}

/****************************************************************************
 * Name: da1470x_dmach_alloc
 ****************************************************************************/

DMA_HANDLE da1470x_dmach_alloc(int chan)
{
  irqstate_t flags;
  int first = 0;
  int last  = DA1470X_DMA_NCHANNELS;
  int i;

  if (chan != DA1470X_DMA_ANY_CHANNEL)
    {
      if (chan < 0 || chan >= DA1470X_DMA_NCHANNELS)
        {
          return NULL;
        }

      first = chan;
      last  = chan + 1;
    }

  flags = enter_critical_section();

  for (i = first; i < last; i++)
    {
      if (!g_dmach[i].used)
        {
          g_dmach[i].used = true;
          putreg32(0, g_dmach[i].base + DA1470X_DMA_CTRL_OFFSET);
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

void da1470x_dmach_free(DMA_HANDLE handle)
{
  struct da1470x_dmach_s *dmach = (struct da1470x_dmach_s *)handle;
  irqstate_t flags;

  DEBUGASSERT(dmach != NULL && dmach->used);

  flags = enter_critical_section();

  putreg32(0, dmach->base + DA1470X_DMA_CTRL_OFFSET);
  putreg32(1u << dmach->chan, DA1470X_DMA_RESET_INT_MASK);

  dmach->used     = false;
  dmach->callback = NULL;
  dmach->arg      = NULL;

  leave_critical_section(flags);
}

/****************************************************************************
 * Name: da1470x_dma_setup
 ****************************************************************************/

int da1470x_dma_setup(DMA_HANDLE handle,
                      const struct da1470x_dma_config_s *config,
                      dma_callback_t callback, void *arg)
{
  struct da1470x_dmach_s *dmach = (struct da1470x_dmach_s *)handle;
  uint32_t ctrl = 0;

  DEBUGASSERT(dmach != NULL && config != NULL);

  dmach->callback = callback;
  dmach->arg      = arg;

  /* Both length registers count from zero: a register value of n moves
   * n + 1 items.  The interface here takes a plain count, so take one off
   * each.  Writing the count itself moves one item too many, which lands
   * outside the caller's buffer.
   */

  if (config->len == 0)
    {
      return -EINVAL;
    }

  putreg32(config->src,  dmach->base + DA1470X_DMA_A_START_OFFSET);
  putreg32(config->dest, dmach->base + DA1470X_DMA_B_START_OFFSET);
  putreg32(config->len - 1, dmach->base + DA1470X_DMA_LEN_OFFSET);
  putreg32(config->int_len > 0 ? config->int_len - 1 : 0,
           dmach->base + DA1470X_DMA_INT_OFFSET);

  if (config->ainc)
    {
      ctrl |= DMA_DMA0_CTRL_AINC;
    }

  if (config->binc)
    {
      ctrl |= DMA_DMA0_CTRL_BINC;
    }

  if (config->circular)
    {
      ctrl |= DMA_DMA0_CTRL_CIRCULAR;
    }

  if (config->idle)
    {
      ctrl |= DMA_DMA0_CTRL_IDLE;
    }

  if (config->init)
    {
      /* Memory initialisation: AINC must be 0 and BINC must be 1 */

      ctrl |= DMA_DMA0_CTRL_INIT | DMA_DMA0_CTRL_BINC;
      ctrl &= ~DMA_DMA0_CTRL_AINC;
    }

  if (config->req_sense)
    {
      ctrl |= DMA_DMA0_CTRL_REQ_SENSE;
    }

  if (config->bus_err_detect)
    {
      ctrl |= DMA_DMA0_CTRL_BUS_ERROR_DETECT;
    }

  if (config->exclusive_access)
    {
      ctrl |= DMA_DMA0_CTRL_EXCLUSIVE_ACCESS;
    }

  if (config->dreq)
    {
      int pair = dmach->chan / 2;

      ctrl |= DMA_DMA0_CTRL_DREQ_MODE;
      modifyreg32(DA1470X_DMA_REQ_MUX, DMA_REQ_MUX_MASK(pair),
                  (config->peripheral & 0xf) << DMA_REQ_MUX_SHIFT(pair));
    }

  ctrl |= DMA_DMA0_CTRL_BW(config->bw);
  ctrl |= DMA_DMA0_CTRL_BURST_MODE(config->burst);
  ctrl |= DMA_DMA0_CTRL_PRIO(config->prio);

  putreg32(ctrl, dmach->base + DA1470X_DMA_CTRL_OFFSET);
  return OK;
}

/****************************************************************************
 * Name: da1470x_dma_start
 ****************************************************************************/

int da1470x_dma_start(DMA_HANDLE handle)
{
  struct da1470x_dmach_s *dmach = (struct da1470x_dmach_s *)handle;

  DEBUGASSERT(dmach != NULL);

  putreg32(1u << dmach->chan, DA1470X_DMA_CLEAR_INT);
  putreg32(1u << dmach->chan, DA1470X_DMA_SET_INT_MASK);
  modifyreg32(dmach->base + DA1470X_DMA_CTRL_OFFSET, 0, DMA_DMA0_CTRL_ON);
  return OK;
}

/****************************************************************************
 * Name: da1470x_dma_stop
 ****************************************************************************/

int da1470x_dma_stop(DMA_HANDLE handle)
{
  struct da1470x_dmach_s *dmach = (struct da1470x_dmach_s *)handle;

  DEBUGASSERT(dmach != NULL);

  modifyreg32(dmach->base + DA1470X_DMA_CTRL_OFFSET, DMA_DMA0_CTRL_ON, 0);
  putreg32(1u << dmach->chan, DA1470X_DMA_RESET_INT_MASK);
  return OK;
}

/****************************************************************************
 * Name: da1470x_dma_get_residue
 ****************************************************************************/

uint16_t da1470x_dma_get_residue(DMA_HANDLE handle)
{
  struct da1470x_dmach_s *dmach = (struct da1470x_dmach_s *)handle;
  uint16_t len;
  uint16_t idx;

  DEBUGASSERT(dmach != NULL);

  len = getreg32(dmach->base + DA1470X_DMA_LEN_OFFSET) + 1;
  idx = getreg32(dmach->base + DA1470X_DMA_IDX_OFFSET);
  return len - idx;
}

/****************************************************************************
 * Name: da1470x_dma_channel
 ****************************************************************************/

int da1470x_dma_channel(DMA_HANDLE handle)
{
  struct da1470x_dmach_s *dmach = (struct da1470x_dmach_s *)handle;

  DEBUGASSERT(dmach != NULL);
  return dmach->chan;
}
