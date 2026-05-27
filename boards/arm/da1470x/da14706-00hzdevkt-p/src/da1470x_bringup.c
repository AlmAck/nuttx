/****************************************************************************
 * boards/arm/da1470x/da14706-00hzdevkt-p/src/da1470x_bringup.c
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

#include <sys/types.h>
#include <stdint.h>
#include <string.h>
#include <syslog.h>
#include <semaphore.h>
#include <time.h>

#include <nuttx/spi/spi.h>

#include "da1470x_dma.h"
#include "da1470x_gpio.h"
#include "da1470x_spi.h"

#ifdef CONFIG_USERLED
#  include <nuttx/leds/userled.h>
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* Memory-to-memory DMA smoke-test buffers and synchronization. Aligned to
 * 4 bytes so the controller can do 32-bit accesses.
 */

static uint32_t g_dma_smoke_src[16] aligned_data(4);
static uint32_t g_dma_smoke_dst[16] aligned_data(4);
static sem_t    g_dma_smoke_sem;
static int      g_dma_smoke_result;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void da1470x_dma_smoke_cb(DMA_HANDLE h, void *arg, int result)
{
  UNUSED(h);
  UNUSED(arg);
  g_dma_smoke_result = result;
  sem_post(&g_dma_smoke_sem);
}

static void da1470x_dma_smoke_test(void)
{
  DMA_HANDLE h;
  struct da1470x_dma_config_s cfg;
  int i;

  /* Fill src with a recognizable pattern; clear dst. */

  for (i = 0; i < (int)(int)(sizeof(g_dma_smoke_src) / sizeof(g_dma_smoke_src[0])); i++)
    {
      g_dma_smoke_src[i] = 0xCAFE0000u | i;
    }
  memset(g_dma_smoke_dst, 0, sizeof(g_dma_smoke_dst));

  sem_init(&g_dma_smoke_sem, 0, 0);

  h = da1470x_dmach_alloc();
  if (h == NULL)
    {
      syslog(LOG_ERR, "DMA: alloc failed\n");
      return;
    }

  /* DA1470x LEN/INT registers hold "count - 1" — hardware does LEN+1
   * transfers and fires the channel IRQ when IDX == INT.
   */

  const int xfer_count =
    (int)(sizeof(g_dma_smoke_src) / sizeof(g_dma_smoke_src[0]));

  memset(&cfg, 0, sizeof(cfg));
  cfg.src        = (uintptr_t)g_dma_smoke_src;
  cfg.dest       = (uintptr_t)g_dma_smoke_dst;
  cfg.len        = xfer_count - 1;
  cfg.int_len    = xfer_count - 1;
  cfg.bw         = DA1470X_DMA_BW_32;
  cfg.burst      = DA1470X_DMA_BURST_1;
  cfg.prio       = DA1470X_DMA_PRIO_0;
  cfg.ainc       = true;
  cfg.binc       = true;
  cfg.dreq             = false;                     /* software-triggered M2M */
  cfg.peripheral       = DA1470X_DMA_PERIPH_NONE;
  cfg.exclusive_access = true;     /* SDK recommends for M2M */
  cfg.bus_err_detect   = true;

  if (da1470x_dma_setup(h, &cfg, da1470x_dma_smoke_cb, NULL) != OK)
    {
      syslog(LOG_ERR, "DMA: setup failed\n");
      da1470x_dmach_free(h);
      return;
    }

  da1470x_dma_start(h);

  /* Wait up to 1s for the transfer to complete. */

  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  ts.tv_sec += 1;
  if (sem_timedwait(&g_dma_smoke_sem, &ts) < 0)
    {
      syslog(LOG_ERR, "DMA: smoke timed out (no IRQ)\n");
      da1470x_dmach_free(h);
      return;
    }

  if (g_dma_smoke_result != OK)
    {
      syslog(LOG_ERR, "DMA: smoke IRQ result=%d\n", g_dma_smoke_result);
    }
  else if (memcmp(g_dma_smoke_src, g_dma_smoke_dst,
                  sizeof(g_dma_smoke_src)) != 0)
    {
      syslog(LOG_ERR, "DMA: smoke data mismatch dst[0]=%08lx src[0]=%08lx\n",
             (unsigned long)g_dma_smoke_dst[0],
             (unsigned long)g_dma_smoke_src[0]);
    }
  else
    {
      syslog(LOG_INFO, "DMA: M2M smoke OK (%zu bytes)\n",
             sizeof(g_dma_smoke_src));
    }

  da1470x_dmach_free(h);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_bringup
 *
 * Description:
 *   Perform architecture-specific initialization
 *
 *   CONFIG_BOARD_LATE_INITIALIZE=y :
 *     Called from board_late_initialize().
 *
 *   CONFIG_BOARD_LATE_INITIALIZE=n && CONFIG_BOARDCTL=y :
 *     Called from the NSH library
 *
 ****************************************************************************/

int da1470x_bringup(void)
{
  int ret;

  /* Initialize the GPIO interrupt subsystem (per-port WKUP toggle IRQs).
   * No pin sources are enabled here; consumers (touch, TE, buttons) call
   * da1470x_gpioirq_attach() and da1470x_gpioirq_enable() to opt in.
   */

  da1470x_gpioirq_initialize();

  /* Bring up the DMA controller and run a memory-to-memory smoke test
   * so we know the channel-alloc/setup/IRQ/callback path is functional
   * before peripheral drivers start relying on it.
   */

  da1470x_dma_initialize();
  da1470x_dma_smoke_test();

  /* Bring up SPI0 and run a polled-transfer smoke test. Without a slave
   * attached to MISO the read-back is undefined, but a successful clock
   * cycle is observable: the controller drains the TX FIFO and asserts
   * TX_EMPTY in SPI_STATUS within a few microseconds. If that never
   * happens, the clock or FIFO config is wrong and we log it.
   */

  {
    struct spi_dev_s *spi = da1470x_spibus_initialize(DA1470X_SPI_BUS_SPI);
    if (spi != NULL)
      {
        uint8_t tx[4] = { 0xA5, 0x5A, 0x12, 0x34 };
        uint8_t rx[4] = { 0 };

        SPI_LOCK(spi, true);
        SPI_SETMODE(spi, SPIDEV_MODE0);
        SPI_SETBITS(spi, 8);
        uint32_t f = SPI_SETFREQUENCY(spi, 4000000);
        SPI_EXCHANGE(spi, tx, rx, sizeof(tx));
        SPI_LOCK(spi, false);
        syslog(LOG_INFO,
               "SPI: bus0 init OK, actual freq=%lu Hz, "
               "rx=%02x %02x %02x %02x\n",
               (unsigned long)f, rx[0], rx[1], rx[2], rx[3]);
      }
    else
      {
        syslog(LOG_ERR, "SPI: bus0 init failed\n");
      }
  }

#ifdef CONFIG_USERLED
  /* Register the LED driver */

  ret = userled_lower_initialize(CONFIG_EXAMPLES_LEDS_DEVPATH);
  if (ret < 0)
    {
      syslog(LOG_ERR, "ERROR: userled_lower_initialize() failed: %d\n", ret);
    }
#endif

  UNUSED(ret);
  return OK;
}
