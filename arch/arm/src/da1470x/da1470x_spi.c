/****************************************************************************
 * arch/arm/src/da1470x/da1470x_spi.c
 *
 * Polled SPI master driver for DA1470x. Implements the NuttX spi_ops_s
 * interface for SPI / SPI2 (in PD_SNC). SPI3 is in the SYS domain and
 * is not wired here yet.
 *
 * Initial scope: polled (no IRQ, no DMA), master only, 4..32-bit words,
 * software chip-select (CS_SELECT=GPIO — caller drives the CS pin).
 * Good enough for the LCDC DBI command channel and for talking to
 * external SPI peripherals during bring-up. A follow-up will add the
 * IRQ/DMA path once we have a real consumer.
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to you under the Apache License, Version
 * 2.0 (the "License"); you may not use this file except in compliance
 * with the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <assert.h>
#include <debug.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>

#include <nuttx/arch.h>
#include <nuttx/mutex.h>
#include <nuttx/spi/spi.h>

#include "arm_internal.h"
#include "hardware/da1470x_crg_snc.h"
#include "hardware/da1470x_spi.h"
#include "hardware/da1470x_memorymap.h"
#include "da1470x_pmu.h"
#include "da1470x_spi.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* SPI source clock on DA1470x with our clock tree: DivN = 32 MHz. With
 * CLK_DIV the SCLK divides by 2*(div+1). We default to ~4 MHz.
 */

#define DA1470X_SPI_SRC_CLK              32000000

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct da1470x_spi_priv_s
{
  struct spi_dev_s spidev;      /* Base spi_dev_s — MUST be first */
  uintptr_t        base;        /* Register base */
  mutex_t          lock;        /* Bus exclusive lock */
  uint32_t         frequency;   /* Last programmed frequency */
  uint32_t         actual;      /* Achieved frequency */
  enum spi_mode_e  mode;        /* Last programmed mode */
  uint8_t          nbits;       /* Last programmed bits per word */
  uint8_t          bus;         /* Bus number (0..2) */
  bool             initialized;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int      da1470x_spi_lock(struct spi_dev_s *dev, bool lock);
static uint32_t da1470x_spi_setfrequency(struct spi_dev_s *dev,
                                         uint32_t frequency);
static void     da1470x_spi_setmode(struct spi_dev_s *dev,
                                    enum spi_mode_e mode);
static void     da1470x_spi_setbits(struct spi_dev_s *dev, int nbits);
static uint32_t da1470x_spi_send(struct spi_dev_s *dev, uint32_t wd);
static void     da1470x_spi_exchange(struct spi_dev_s *dev,
                                     const void *txbuffer, void *rxbuffer,
                                     size_t nwords);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct spi_ops_s g_spi_ops =
{
  .lock         = da1470x_spi_lock,
  .select       = NULL,           /* Provided by board glue */
  .setfrequency = da1470x_spi_setfrequency,
  .setmode      = da1470x_spi_setmode,
  .setbits      = da1470x_spi_setbits,
  .status       = NULL,
  .send         = da1470x_spi_send,
  .exchange     = da1470x_spi_exchange,
  .registercallback = NULL,
};

static struct da1470x_spi_priv_s g_spi0_priv =
{
  .spidev = { .ops = &g_spi_ops },
  .base   = DA1470X_SPI_BASE,
  .lock   = NXMUTEX_INITIALIZER,
  .bus    = DA1470X_SPI_BUS_SPI,
};

static struct da1470x_spi_priv_s g_spi1_priv =
{
  .spidev = { .ops = &g_spi_ops },
  .base   = DA1470X_SPI2_BASE,
  .lock   = NXMUTEX_INITIALIZER,
  .bus    = DA1470X_SPI_BUS_SPI2,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static inline void spi_putreg(struct da1470x_spi_priv_s *p,
                              uint32_t off, uint32_t val)
{
  putreg32(val, p->base + off);
}

static inline uint32_t spi_getreg(struct da1470x_spi_priv_s *p, uint32_t off)
{
  return getreg32(p->base + off);
}

static void spi_set_enable(struct da1470x_spi_priv_s *p, bool en)
{
  uint32_t ctrl = spi_getreg(p, DA1470_SPI_CTRL_OFFSET);
  if (en)
    {
      ctrl |= SPI_CTRL_SPI_EN;
    }
  else
    {
      ctrl &= ~SPI_CTRL_SPI_EN;
    }
  spi_putreg(p, DA1470_SPI_CTRL_OFFSET, ctrl);
}

static void spi_clock_enable(struct da1470x_spi_priv_s *p)
{
  /* SPI/SPI2 sit in PD_SNC; both share CRG_SNC clock bits (SPI=bit6/7,
   * SPI2=bit8/9). Select DivN (=32 MHz) and enable.
   */

  uint32_t mask;
  if (p->bus == DA1470X_SPI_BUS_SPI)
    {
      mask = CRG_SNC_SPI_ENABLE;
      putreg32(CRG_SNC_SPI_CLK_SEL, DA1470_CRG_SNC_RESET_CLK_SNC); /* DivN */
    }
  else /* SPI2 */
    {
      mask = CRG_SNC_SPI2_ENABLE;
      putreg32(CRG_SNC_SPI2_CLK_SEL, DA1470_CRG_SNC_RESET_CLK_SNC);
    }

  /* Set the ENABLE bit via the SET_CLK_SNC register. */

  putreg32(mask, DA1470_CRG_SNC_SET_CLK_SNC);
}

/****************************************************************************
 * NuttX ops
 ****************************************************************************/

static int da1470x_spi_lock(struct spi_dev_s *dev, bool lock)
{
  struct da1470x_spi_priv_s *p = (struct da1470x_spi_priv_s *)dev;
  return lock ? nxmutex_lock(&p->lock) : nxmutex_unlock(&p->lock);
}

static uint32_t da1470x_spi_setfrequency(struct spi_dev_s *dev,
                                         uint32_t frequency)
{
  struct da1470x_spi_priv_s *p = (struct da1470x_spi_priv_s *)dev;

  if (frequency == p->frequency)
    {
      return p->actual;
    }

  /* SCLK = SRC / (2 * (div + 1)). Pick the smallest div that yields a
   * frequency <= requested.
   *   div = ceil(SRC / (2 * frequency)) - 1, clamped to [0,127].
   */

  if (frequency == 0)
    {
      frequency = 1;
    }

  uint32_t div = (DA1470X_SPI_SRC_CLK + (2 * frequency) - 1)
                 / (2 * frequency);
  if (div > 0)
    {
      div -= 1;
    }
  if (div > SPI_CLOCK_DIV_MASK)
    {
      div = SPI_CLOCK_DIV_MASK;
    }

  /* CLOCK register can only be written while the controller is disabled. */

  spi_set_enable(p, false);
  spi_putreg(p, DA1470_SPI_CLOCK_OFFSET, div);
  spi_set_enable(p, true);

  p->frequency = frequency;
  p->actual    = DA1470X_SPI_SRC_CLK / (2 * (div + 1));
  return p->actual;
}

static void da1470x_spi_setmode(struct spi_dev_s *dev, enum spi_mode_e mode)
{
  struct da1470x_spi_priv_s *p = (struct da1470x_spi_priv_s *)dev;
  uint32_t cfg;
  uint8_t spi_mode_bits;

  if (mode == p->mode)
    {
      return;
    }

  switch (mode)
    {
      case SPIDEV_MODE0: spi_mode_bits = 0; break;
      case SPIDEV_MODE1: spi_mode_bits = 1; break;
      case SPIDEV_MODE2: spi_mode_bits = 2; break;
      case SPIDEV_MODE3: spi_mode_bits = 3; break;
      default:           return;
    }

  spi_set_enable(p, false);
  cfg  = spi_getreg(p, DA1470_SPI_CONFIG_OFFSET);
  cfg &= ~SPI_CONFIG_MODE_MASK;
  cfg |= (uint32_t)spi_mode_bits << SPI_CONFIG_MODE_SHIFT;
  spi_putreg(p, DA1470_SPI_CONFIG_OFFSET, cfg);
  spi_set_enable(p, true);

  p->mode = mode;
}

static void da1470x_spi_setbits(struct spi_dev_s *dev, int nbits)
{
  struct da1470x_spi_priv_s *p = (struct da1470x_spi_priv_s *)dev;
  uint32_t cfg;

  if (nbits < 4 || nbits > 32 || nbits == p->nbits)
    {
      return;
    }

  spi_set_enable(p, false);
  cfg  = spi_getreg(p, DA1470_SPI_CONFIG_OFFSET);
  cfg &= ~SPI_CONFIG_WORD_LENGTH_MASK;
  cfg |= SPI_CONFIG_WORDLEN(nbits) & SPI_CONFIG_WORD_LENGTH_MASK;
  spi_putreg(p, DA1470_SPI_CONFIG_OFFSET, cfg);
  spi_set_enable(p, true);

  p->nbits = nbits;
}

/* Push one word into the TX FIFO when not full; pull one word from the
 * RX FIFO when not empty. Polled.
 */

static uint32_t da1470x_spi_send(struct spi_dev_s *dev, uint32_t wd)
{
  struct da1470x_spi_priv_s *p = (struct da1470x_spi_priv_s *)dev;
  uint32_t rx;

  while ((spi_getreg(p, DA1470_SPI_FIFO_STATUS_OFFSET)
          & SPI_FIFO_STATUS_TX_FULL) != 0)
    ;

  spi_putreg(p, DA1470_SPI_FIFO_WRITE_OFFSET, wd);

  while ((spi_getreg(p, DA1470_SPI_FIFO_STATUS_OFFSET)
          & SPI_FIFO_STATUS_RX_EMPTY) != 0)
    ;

  rx = spi_getreg(p, DA1470_SPI_FIFO_READ_OFFSET);
  return rx;
}

static void da1470x_spi_exchange(struct spi_dev_s *dev,
                                 const void *txbuffer, void *rxbuffer,
                                 size_t nwords)
{
  struct da1470x_spi_priv_s *p = (struct da1470x_spi_priv_s *)dev;
  size_t i;

  /* Treat all transfers as byte streams for the 8-bit common case.
   * Wider words could be handled by adjusting the buffer stride based on
   * p->nbits, but the LCDC DBI path uses 8-bit only.
   */

  const uint8_t *tx = (const uint8_t *)txbuffer;
  uint8_t       *rx = (uint8_t *)rxbuffer;

  for (i = 0; i < nwords; i++)
    {
      uint8_t out = (tx != NULL) ? tx[i] : 0xFF;
      uint8_t in  = (uint8_t)da1470x_spi_send(dev, out);
      if (rx != NULL)
        {
          rx[i] = in;
        }
    }

  UNUSED(p);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

struct spi_dev_s *da1470x_spibus_initialize(int bus)
{
  struct da1470x_spi_priv_s *p;

  switch (bus)
    {
      case DA1470X_SPI_BUS_SPI:  p = &g_spi0_priv; break;
      case DA1470X_SPI_BUS_SPI2: p = &g_spi1_priv; break;
      default:
        spierr("ERROR: SPI bus %d not supported\n", bus);
        return NULL;
    }

  if (p->initialized)
    {
      return &p->spidev;
    }

  /* PD_SNC hosts SPI/SPI2; ensure it's up before we touch the block. */

  da1470x_pd_enable(DA1470X_PD_SNC);
  spi_clock_enable(p);

  /* Disable, reset FIFOs, set master / mode 0 / 8-bit defaults. */

  spi_putreg(p, DA1470_SPI_CTRL_OFFSET, SPI_CTRL_FIFO_RESET);
  spi_putreg(p, DA1470_SPI_CONFIG_OFFSET,
             (0U << SPI_CONFIG_MODE_SHIFT) |
             SPI_CONFIG_WORDLEN(8));

  /* CS driven externally by GPIO. */

  spi_putreg(p, DA1470_SPI_CS_CONFIG_OFFSET, SPI_CS_CONFIG_GPIO);

  /* Master capture on next edge (per SDK comment). */

  spi_putreg(p, DA1470_SPI_CTRL_OFFSET,
             SPI_CTRL_CAPTURE_AT_NEXT_EDGE |
             SPI_CTRL_TX_EN | SPI_CTRL_RX_EN);

  /* Default 4 MHz. */

  p->mode      = SPIDEV_MODE0;
  p->nbits     = 8;
  p->frequency = 0;     /* force re-program */
  da1470x_spi_setfrequency(&p->spidev, 4000000);

  /* Enable the controller. */

  spi_set_enable(p, true);
  p->initialized = true;
  return &p->spidev;
}
