/****************************************************************************
 * arch/arm/src/da1470x/da1470x_spi.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_SPI_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_SPI_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdbool.h>
#include <stdint.h>

#include <nuttx/spi/spi.h>

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
 * Name: da1470x_spibus_initialize
 *
 * Description:
 *   Initialize the selected SPI bus (0, 1 or 2) and return its device
 *   structure.  The board must configure the SCLK/MOSI/MISO pins before
 *   calling this function.
 *
 ****************************************************************************/

struct spi_dev_s *da1470x_spibus_initialize(int bus);

/****************************************************************************
 * Name: da1470x_spiNselect, da1470x_spiNstatus, da1470x_spiNcmddata
 *
 * Description:
 *   Board-specific chip-select, status and command/data callbacks.  The
 *   board must provide these for every bus it enables; weak defaults are
 *   provided that do nothing.
 *
 ****************************************************************************/

#ifdef CONFIG_DA1470X_SPI0
void    da1470x_spi0select(struct spi_dev_s *dev, uint32_t devid,
                           bool selected);
uint8_t da1470x_spi0status(struct spi_dev_s *dev, uint32_t devid);
int     da1470x_spi0cmddata(struct spi_dev_s *dev, uint32_t devid,
                            bool cmd);
#endif

#ifdef CONFIG_DA1470X_SPI1
void    da1470x_spi1select(struct spi_dev_s *dev, uint32_t devid,
                           bool selected);
uint8_t da1470x_spi1status(struct spi_dev_s *dev, uint32_t devid);
int     da1470x_spi1cmddata(struct spi_dev_s *dev, uint32_t devid,
                            bool cmd);
#endif

#ifdef CONFIG_DA1470X_SPI2
void    da1470x_spi2select(struct spi_dev_s *dev, uint32_t devid,
                           bool selected);
uint8_t da1470x_spi2status(struct spi_dev_s *dev, uint32_t devid);
int     da1470x_spi2cmddata(struct spi_dev_s *dev, uint32_t devid,
                            bool cmd);
#endif

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_SPI_H */
