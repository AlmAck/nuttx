/****************************************************************************
 * arch/arm/src/da1470x/da1470x_spi.c
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
#include <stdint.h>
#include <stdbool.h>

#include <nuttx/arch.h>
#include <nuttx/mutex.h>
#include <nuttx/semaphore.h>
#include <nuttx/spi/spi.h>

#include "arm_internal.h"
#include "hardware/da1470x_crg_snc.h"
#include "hardware/da1470x_crg_sys.h"
#include "hardware/da1470x_spi.h"
#include "hardware/da1470x_memorymap.h"
#include "da1470x_clockconfig.h"
#include "da1470x_pmu.h"
#include "da1470x_spi.h"

#ifdef CONFIG_DA1470X_SPI

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Bound on the per-word busy wait (loops), generous for the slowest clock
 * divider.
 */

#define SPI_POLL_LOOPS          2000000

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct da1470x_spi_priv_s
{
  struct spi_dev_s spidev;      /* Externally visible part of the SPI interface */
  const struct spi_ops_s *ops;  /* Bus specific operations */
  uintptr_t        base;        /* Register base */
  mutex_t          lock;        /* Bus exclusive lock */
  uint32_t         frequency;   /* Last requested frequency */
  uint32_t         actual;      /* Achieved frequency */
  enum spi_mode_e  mode;        /* Last programmed mode */
  uint8_t          nbits;       /* Last programmed bits per word */
  uint8_t          bus;         /* Bus number (0..2) */
  uint8_t          irq;         /* NVIC interrupt number */
  bool             initialized; /* True once the block is set up */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int      spi_lock(struct spi_dev_s *dev, bool lock);
static uint32_t spi_setfrequency(struct spi_dev_s *dev, uint32_t frequency);
static void     spi_setmode(struct spi_dev_s *dev, enum spi_mode_e mode);
static void     spi_setbits(struct spi_dev_s *dev, int nbits);
#ifdef CONFIG_SPI_HWFEATURES
static int      spi_hwfeatures(struct spi_dev_s *dev,
                               spi_hwfeatures_t features);
#endif
static uint32_t spi_send(struct spi_dev_s *dev, uint32_t wd);
static void     spi_exchange(struct spi_dev_s *dev, const void *txbuffer,
                             void *rxbuffer, size_t nwords);
#ifndef CONFIG_SPI_EXCHANGE
static void     spi_sndblock(struct spi_dev_s *dev, const void *txbuffer,
                             size_t nwords);
static void     spi_recvblock(struct spi_dev_s *dev, void *rxbuffer,
                              size_t nwords);
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

#ifdef CONFIG_DA1470X_SPI0
static const struct spi_ops_s g_spi0ops =
{
  .lock              = spi_lock,
  .select            = da1470x_spi0select,
  .setfrequency      = spi_setfrequency,
  .setmode           = spi_setmode,
  .setbits           = spi_setbits,
#ifdef CONFIG_SPI_HWFEATURES
  .hwfeatures        = spi_hwfeatures,
#endif
  .status            = da1470x_spi0status,
#ifdef CONFIG_SPI_CMDDATA
  .cmddata           = da1470x_spi0cmddata,
#endif
  .send              = spi_send,
#ifdef CONFIG_SPI_EXCHANGE
  .exchange          = spi_exchange,
#else
  .sndblock          = spi_sndblock,
  .recvblock         = spi_recvblock,
#endif
  .registercallback  = NULL,
};

static struct da1470x_spi_priv_s g_spi0priv =
{
  .spidev     =
  {
    .ops = &g_spi0ops
  },
  .base    = DA1470X_SPI0_BASE,
  .lock    = NXMUTEX_INITIALIZER,
  .bus     = 0,
  .irq     = DA1470X_IRQ_SPI0,
};
#endif

#ifdef CONFIG_DA1470X_SPI1
static const struct spi_ops_s g_spi1ops =
{
  .lock              = spi_lock,
  .select            = da1470x_spi1select,
  .setfrequency      = spi_setfrequency,
  .setmode           = spi_setmode,
  .setbits           = spi_setbits,
#ifdef CONFIG_SPI_HWFEATURES
  .hwfeatures        = spi_hwfeatures,
#endif
  .status            = da1470x_spi1status,
#ifdef CONFIG_SPI_CMDDATA
  .cmddata           = da1470x_spi1cmddata,
#endif
  .send              = spi_send,
#ifdef CONFIG_SPI_EXCHANGE
  .exchange          = spi_exchange,
#else
  .sndblock          = spi_sndblock,
  .recvblock         = spi_recvblock,
#endif
  .registercallback  = NULL,
};

static struct da1470x_spi_priv_s g_spi1priv =
{
  .spidev     =
  {
    .ops = &g_spi1ops
  },
  .base    = DA1470X_SPI1_BASE,
  .lock    = NXMUTEX_INITIALIZER,
  .bus     = 1,
  .irq     = DA1470X_IRQ_SPI1,
};
#endif

#ifdef CONFIG_DA1470X_SPI2
static const struct spi_ops_s g_spi2ops =
{
  .lock              = spi_lock,
  .select            = da1470x_spi2select,
  .setfrequency      = spi_setfrequency,
  .setmode           = spi_setmode,
  .setbits           = spi_setbits,
#ifdef CONFIG_SPI_HWFEATURES
  .hwfeatures        = spi_hwfeatures,
#endif
  .status            = da1470x_spi2status,
#ifdef CONFIG_SPI_CMDDATA
  .cmddata           = da1470x_spi2cmddata,
#endif
  .send              = spi_send,
#ifdef CONFIG_SPI_EXCHANGE
  .exchange          = spi_exchange,
#else
  .sndblock          = spi_sndblock,
  .recvblock         = spi_recvblock,
#endif
  .registercallback  = NULL,
};

static struct da1470x_spi_priv_s g_spi2priv =
{
  .spidev     =
  {
    .ops = &g_spi2ops
  },
  .base    = DA1470X_SPI2_BASE,
  .lock    = NXMUTEX_INITIALIZER,
  .bus     = 2,
  .irq     = DA1470X_IRQ_SPI2,
};
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: spi_getreg / spi_putreg
 ****************************************************************************/

static inline uint32_t spi_getreg(struct da1470x_spi_priv_s *priv,
                                  uint32_t offset)
{
  return getreg32(priv->base + offset);
}

static inline void spi_putreg(struct da1470x_spi_priv_s *priv,
                              uint32_t offset, uint32_t value)
{
  putreg32(value, priv->base + offset);
}

/****************************************************************************
 * Name: spi_enable
 *
 * Description:
 *   Enable or disable the controller.  Configuration registers may only be
 *   written while it is disabled.
 *
 ****************************************************************************/

static void spi_enable(struct da1470x_spi_priv_s *priv, bool enable)
{
  uint32_t ctrl = spi_getreg(priv, DA1470X_SPI_CTRL_OFFSET);

  if (enable)
    {
      ctrl |= SPI_CTRL_EN;
    }
  else
    {
      ctrl &= ~SPI_CTRL_EN;
    }

  spi_putreg(priv, DA1470X_SPI_CTRL_OFFSET, ctrl);
}

/****************************************************************************
 * Name: spi_clock_enable
 *
 * Description:
 *   Route DIVN (32 MHz) to the block and enable its clock.  SPI0/SPI1 live
 *   in PD_SNC (CRG_SNC), SPI2 in PD_SYS (CRG_SYS).
 *
 ****************************************************************************/

static void spi_clock_enable(struct da1470x_spi_priv_s *priv)
{
  switch (priv->bus)
    {
      case 0:
        da1470x_pd_enable(DA1470X_PD_SNC);
        putreg32(CRG_SNC_CLK_SNC_SPI_CLK_SEL, DA1470X_CRG_SNC_RESET_CLK_SNC);
        putreg32(CRG_SNC_CLK_SNC_SPI_ENABLE, DA1470X_CRG_SNC_SET_CLK_SNC);
        break;

      case 1:
        da1470x_pd_enable(DA1470X_PD_SNC);
        putreg32(CRG_SNC_CLK_SNC_SPI2_CLK_SEL,
                 DA1470X_CRG_SNC_RESET_CLK_SNC);
        putreg32(CRG_SNC_CLK_SNC_SPI2_ENABLE, DA1470X_CRG_SNC_SET_CLK_SNC);
        break;

      case 2:
      default:
        putreg32(CRG_SYS_CLK_SYS_SPI3_CLK_SEL,
                 DA1470X_CRG_SYS_RESET_CLK_SYS);
        putreg32(CRG_SYS_CLK_SYS_SPI3_ENABLE, DA1470X_CRG_SYS_SET_CLK_SYS);
        break;
    }
}

/****************************************************************************
 * Name: spi_wait_txspace / spi_wait_rxdata
 ****************************************************************************/

static int spi_wait_txspace(struct da1470x_spi_priv_s *priv)
{
  int i;

  for (i = 0; i < SPI_POLL_LOOPS; i++)
    {
      if ((spi_getreg(priv, DA1470X_SPI_FIFO_STATUS_OFFSET) &
           SPI_FIFO_STATUS_STATUS_TX_FULL) == 0)
        {
          return OK;
        }
    }

  return -ETIMEDOUT;
}

static int spi_wait_rxdata(struct da1470x_spi_priv_s *priv)
{
  int i;

  for (i = 0; i < SPI_POLL_LOOPS; i++)
    {
      if ((spi_getreg(priv, DA1470X_SPI_FIFO_STATUS_OFFSET) &
           SPI_FIFO_STATUS_STATUS_RX_EMPTY) == 0)
        {
          return OK;
        }
    }

  return -ETIMEDOUT;
}

/****************************************************************************
 * Name: spi_lock
 *
 * Description:
 *   On SPI buses where there are multiple devices, it will be necessary to
 *   lock SPI to have exclusive access to the buses for a sequence of
 *   transfers.  The bus should be locked before the chip is selected. After
 *   locking the SPI bus, the caller should then also call the setfrequency,
 *   setbits, and setmode methods to make sure that the SPI is properly
 *   configured for the device.  If the SPI bus is being shared, then it
 *   may have been left in an incompatible state.
 *
 ****************************************************************************/

static int spi_lock(struct spi_dev_s *dev, bool lock)
{
  struct da1470x_spi_priv_s *priv = (struct da1470x_spi_priv_s *)dev;

  if (lock)
    {
      return nxmutex_lock(&priv->lock);
    }

  return nxmutex_unlock(&priv->lock);
}

/****************************************************************************
 * Name: spi_setfrequency
 *
 * Description:
 *   Set the SPI frequency.  SCLK = DIVN / (2 * (div + 1)); the largest
 *   frequency not exceeding the request is selected.
 *
 ****************************************************************************/

static uint32_t spi_setfrequency(struct spi_dev_s *dev, uint32_t frequency)
{
  struct da1470x_spi_priv_s *priv = (struct da1470x_spi_priv_s *)dev;
  uint32_t src = da1470x_get_divn_clk();
  uint32_t div;

  if (frequency == priv->frequency && priv->actual != 0)
    {
      return priv->actual;
    }

  if (frequency == 0)
    {
      frequency = 1;
    }

  div = (src + (2 * frequency) - 1) / (2 * frequency);
  if (div > 0)
    {
      div -= 1;
    }

  if (div > (SPI_CLOCK_CLK_DIV_MASK >> SPI_CLOCK_CLK_DIV_SHIFT))
    {
      div = SPI_CLOCK_CLK_DIV_MASK >> SPI_CLOCK_CLK_DIV_SHIFT;
    }

  spi_enable(priv, false);
  spi_putreg(priv, DA1470X_SPI_CLOCK_OFFSET, SPI_CLOCK_CLK_DIV(div));
  spi_enable(priv, true);

  priv->frequency = frequency;
  priv->actual    = src / (2 * (div + 1));

  spiinfo("Frequency %" PRIu32 " -> %" PRIu32 "\n", frequency,
          priv->actual);
  return priv->actual;
}

/****************************************************************************
 * Name: spi_setmode
 *
 * Description:
 *   Set the SPI mode.  The controller CPOL/CPHA field matches the NuttX
 *   mode numbering.
 *
 ****************************************************************************/

static void spi_setmode(struct spi_dev_s *dev, enum spi_mode_e mode)
{
  struct da1470x_spi_priv_s *priv = (struct da1470x_spi_priv_s *)dev;
  uint32_t cfg;

  if (mode == priv->mode)
    {
      return;
    }

  if (mode > SPIDEV_MODE3)
    {
      spierr("Unsupported mode %d\n", mode);
      return;
    }

  spi_enable(priv, false);
  cfg  = spi_getreg(priv, DA1470X_SPI_CONFIG_OFFSET);
  cfg &= ~SPI_CONFIG_MODE_MASK;
  cfg |= SPI_CONFIG_MODE((uint32_t)mode);
  spi_putreg(priv, DA1470X_SPI_CONFIG_OFFSET, cfg);
  spi_enable(priv, true);

  priv->mode = mode;
}

/****************************************************************************
 * Name: spi_setbits
 *
 * Description:
 *   Set the number of bits per word (4..32).
 *
 ****************************************************************************/

static void spi_setbits(struct spi_dev_s *dev, int nbits)
{
  struct da1470x_spi_priv_s *priv = (struct da1470x_spi_priv_s *)dev;
  uint32_t cfg;

  if (nbits == priv->nbits)
    {
      return;
    }

  if (nbits < 4 || nbits > 32)
    {
      spierr("Unsupported word size %d\n", nbits);
      return;
    }

  spi_enable(priv, false);
  cfg  = spi_getreg(priv, DA1470X_SPI_CONFIG_OFFSET);
  cfg &= ~SPI_CONFIG_WORD_LENGTH_MASK;
  cfg |= SPI_CONFIG_WORDLEN(nbits);
  spi_putreg(priv, DA1470X_SPI_CONFIG_OFFSET, cfg);
  spi_enable(priv, true);

  priv->nbits = nbits;
}

/****************************************************************************
 * Name: spi_hwfeatures
 ****************************************************************************/

#ifdef CONFIG_SPI_HWFEATURES
static int spi_hwfeatures(struct spi_dev_s *dev, spi_hwfeatures_t features)
{
  /* No special features are supported */

  return (features == 0) ? OK : -ENOSYS;
}
#endif

/****************************************************************************
 * Name: spi_send
 *
 * Description:
 *   Exchange one word on SPI
 *
 ****************************************************************************/

static uint32_t spi_send(struct spi_dev_s *dev, uint32_t wd)
{
  struct da1470x_spi_priv_s *priv = (struct da1470x_spi_priv_s *)dev;

  if (spi_wait_txspace(priv) < 0)
    {
      spierr("TX FIFO stuck full\n");
      return 0;
    }

  spi_putreg(priv, DA1470X_SPI_FIFO_WRITE_OFFSET, wd);

  if (spi_wait_rxdata(priv) < 0)
    {
      spierr("RX FIFO timeout\n");
      return 0;
    }

  return spi_getreg(priv, DA1470X_SPI_FIFO_READ_OFFSET);
}

/****************************************************************************
 * Name: spi_exchange
 *
 * Description:
 *   Exchange a block of data on SPI.  The FIFO is kept as full as possible
 *   without overrunning the receive side.
 *
 ****************************************************************************/

static void spi_exchange(struct spi_dev_s *dev, const void *txbuffer,
                         void *rxbuffer, size_t nwords)
{
  struct da1470x_spi_priv_s *priv = (struct da1470x_spi_priv_s *)dev;
  size_t sent = 0;
  size_t received = 0;
  uint32_t wd;

  spiinfo("txbuffer=%p rxbuffer=%p nwords=%zu\n", txbuffer, rxbuffer,
          nwords);

  while (received < nwords)
    {
      /* Fill the TX FIFO but never queue more than the FIFO depth ahead
       * of the receiver so RX can not overflow.
       */

      while (sent < nwords && (sent - received) < DA1470X_SPI_FIFO_DEPTH &&
             (spi_getreg(priv, DA1470X_SPI_FIFO_STATUS_OFFSET) &
              SPI_FIFO_STATUS_STATUS_TX_FULL) == 0)
        {
          if (txbuffer == NULL)
            {
              wd = 0xffffffff;
            }
          else if (priv->nbits > 16)
            {
              wd = ((const uint32_t *)txbuffer)[sent];
            }
          else if (priv->nbits > 8)
            {
              wd = ((const uint16_t *)txbuffer)[sent];
            }
          else
            {
              wd = ((const uint8_t *)txbuffer)[sent];
            }

          spi_putreg(priv, DA1470X_SPI_FIFO_WRITE_OFFSET, wd);
          sent++;
        }

      /* Drain whatever has been received */

      if (spi_wait_rxdata(priv) < 0)
        {
          spierr("RX FIFO timeout after %zu words\n", received);
          return;
        }

      while (received < sent &&
             (spi_getreg(priv, DA1470X_SPI_FIFO_STATUS_OFFSET) &
              SPI_FIFO_STATUS_STATUS_RX_EMPTY) == 0)
        {
          wd = spi_getreg(priv, DA1470X_SPI_FIFO_READ_OFFSET);

          if (rxbuffer != NULL)
            {
              if (priv->nbits > 16)
                {
                  ((uint32_t *)rxbuffer)[received] = wd;
                }
              else if (priv->nbits > 8)
                {
                  ((uint16_t *)rxbuffer)[received] = (uint16_t)wd;
                }
              else
                {
                  ((uint8_t *)rxbuffer)[received] = (uint8_t)wd;
                }
            }

          received++;
        }
    }
}

/****************************************************************************
 * Name: spi_sndblock / spi_recvblock
 ****************************************************************************/

#ifndef CONFIG_SPI_EXCHANGE
static void spi_sndblock(struct spi_dev_s *dev, const void *txbuffer,
                         size_t nwords)
{
  spi_exchange(dev, txbuffer, NULL, nwords);
}

static void spi_recvblock(struct spi_dev_s *dev, void *rxbuffer,
                          size_t nwords)
{
  spi_exchange(dev, NULL, rxbuffer, nwords);
}
#endif

/****************************************************************************
 * Name: spi_bus_initialize
 *
 * Description:
 *   Bring the controller to a known master-mode configuration.
 *
 ****************************************************************************/

static void spi_bus_initialize(struct da1470x_spi_priv_s *priv)
{
  spi_clock_enable(priv);

  /* Disable, flush the FIFOs and select master mode 0, 8-bit words */

  spi_putreg(priv, DA1470X_SPI_CTRL_OFFSET, SPI_CTRL_FIFO_RESET);
  spi_putreg(priv, DA1470X_SPI_CONFIG_OFFSET,
             SPI_CONFIG_MODE(0) | SPI_CONFIG_WORDLEN(8));

  /* Chip select is driven by the board through GPIO */

  spi_putreg(priv, DA1470X_SPI_CS_CONFIG_OFFSET,
             SPI_CS_CONFIG_CS_SELECT(SPI_CS_CONFIG_GPIO));

  /* No FIFO interrupts (polled) */

  spi_putreg(priv, DA1470X_SPI_IRQ_MASK_OFFSET, 0);
  spi_putreg(priv, DA1470X_SPI_FIFO_CONFIG_OFFSET, 0);

  spi_putreg(priv, DA1470X_SPI_CTRL_OFFSET,
             SPI_CTRL_CAPTURE_AT_NEXT_EDGE | SPI_CTRL_TX_EN |
             SPI_CTRL_RX_EN);

  priv->mode      = SPIDEV_MODE0;
  priv->nbits     = 8;
  priv->frequency = 0;
  priv->actual    = 0;
  spi_setfrequency(&priv->spidev, 1000000);

  spi_enable(priv, true);
  priv->initialized = true;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_spibus_initialize
 ****************************************************************************/

struct spi_dev_s *da1470x_spibus_initialize(int bus)
{
  struct da1470x_spi_priv_s *priv;

  switch (bus)
    {
#ifdef CONFIG_DA1470X_SPI0
      case 0:
        priv = &g_spi0priv;
        break;
#endif
#ifdef CONFIG_DA1470X_SPI1
      case 1:
        priv = &g_spi1priv;
        break;
#endif
#ifdef CONFIG_DA1470X_SPI2
      case 2:
        priv = &g_spi2priv;
        break;
#endif
      default:
        spierr("SPI bus %d not supported\n", bus);
        return NULL;
    }

  if (!priv->initialized)
    {
      nxmutex_lock(&priv->lock);
      spi_bus_initialize(priv);
      nxmutex_unlock(&priv->lock);
    }

  return &priv->spidev;
}

/****************************************************************************
 * Name: da1470x_spiNselect / status / cmddata weak defaults
 ****************************************************************************/

#ifdef CONFIG_DA1470X_SPI0
void weak_function da1470x_spi0select(struct spi_dev_s *dev,
                                      uint32_t devid, bool selected)
{
}

uint8_t weak_function da1470x_spi0status(struct spi_dev_s *dev,
                                         uint32_t devid)
{
  return 0;
}

int weak_function da1470x_spi0cmddata(struct spi_dev_s *dev,
                                      uint32_t devid, bool cmd)
{
  return -ENODEV;
}
#endif

#ifdef CONFIG_DA1470X_SPI1
void weak_function da1470x_spi1select(struct spi_dev_s *dev,
                                      uint32_t devid, bool selected)
{
}

uint8_t weak_function da1470x_spi1status(struct spi_dev_s *dev,
                                         uint32_t devid)
{
  return 0;
}

int weak_function da1470x_spi1cmddata(struct spi_dev_s *dev,
                                      uint32_t devid, bool cmd)
{
  return -ENODEV;
}
#endif

#ifdef CONFIG_DA1470X_SPI2
void weak_function da1470x_spi2select(struct spi_dev_s *dev,
                                      uint32_t devid, bool selected)
{
}

uint8_t weak_function da1470x_spi2status(struct spi_dev_s *dev,
                                         uint32_t devid)
{
  return 0;
}

int weak_function da1470x_spi2cmddata(struct spi_dev_s *dev,
                                      uint32_t devid, bool cmd)
{
  return -ENODEV;
}
#endif

#endif /* CONFIG_DA1470X_SPI */
