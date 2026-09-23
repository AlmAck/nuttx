/****************************************************************************
 * boards/arm/da1470x/da14706-00hzdevkt-p/src/da1470x_spi.c
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

#include <stdint.h>
#include <stdbool.h>
#include <debug.h>
#include <errno.h>

#include <nuttx/spi/spi.h>
#include <nuttx/spi/spi_transfer.h>
#include <arch/board/board.h>

#include "da1470x_gpio.h"
#include "da1470x_spi.h"
#include "da14706-00hzdevkt-p.h"

#ifdef CONFIG_DA1470X_SPI

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_spidev_initialize
 *
 * Description:
 *   Configure the SPI0 pins, initialize the bus and register /dev/spi0.
 *
 ****************************************************************************/

int da1470x_spidev_initialize(void)
{
  int ret = OK;

#ifdef CONFIG_DA1470X_SPI0
  struct spi_dev_s *spi;

  da1470x_gpio_config(BOARD_SPI0_SCLK_PIN);
  da1470x_gpio_config(BOARD_SPI0_MOSI_PIN);
  da1470x_gpio_config(BOARD_SPI0_MISO_PIN);
  da1470x_gpio_config(BOARD_SPI0_CS_PIN);

  spi = da1470x_spibus_initialize(0);
  if (spi == NULL)
    {
      spierr("Failed to initialize SPI0\n");
      return -ENODEV;
    }

#ifdef CONFIG_SPI_DRIVER
  ret = spi_register(spi, 0);
  if (ret < 0)
    {
      spierr("Failed to register /dev/spi0: %d\n", ret);
    }
#endif
#endif

  return ret;
}

/****************************************************************************
 * Name: da1470x_spi0select / da1470x_spi0status
 *
 * Description:
 *   Board chip-select and status callbacks for SPI0.  The single CS line
 *   on the expansion header is driven as a GPIO, active low.
 *
 ****************************************************************************/

#ifdef CONFIG_DA1470X_SPI0
void da1470x_spi0select(struct spi_dev_s *dev, uint32_t devid,
                        bool selected)
{
  spiinfo("devid: %" PRIu32 " CS: %s\n", devid,
          selected ? "assert" : "de-assert");

  da1470x_gpio_write(BOARD_SPI0_CS_PIN, !selected);
}

uint8_t da1470x_spi0status(struct spi_dev_s *dev, uint32_t devid)
{
  return 0;
}
#endif

#endif /* CONFIG_DA1470X_SPI */
