/****************************************************************************
 * arch/arm/src/da1470x/da1470x_spi.h
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to you under the Apache License, Version
 * 2.0 (the "License"); you may not use this file except in compliance
 * with the License.
 *
 ****************************************************************************/

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_SPI_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_SPI_H

#include <nuttx/config.h>
#include <nuttx/spi/spi.h>

/* Bus numbering: 0 = SPI, 1 = SPI2, 2 = SPI3. */

#define DA1470X_SPI_BUS_SPI    0
#define DA1470X_SPI_BUS_SPI2   1
#define DA1470X_SPI_BUS_SPI3   2

/****************************************************************************
 * Name: da1470x_spibus_initialize
 *
 * Description:
 *   Bring up a SPI master, return its NuttX spi_dev_s. Powers the
 *   peripheral's CRG_SNC/CRG_SYS clock, resets the FIFOs, and leaves the
 *   block in master mode 0 / 8-bit / ~4 MHz until the caller calls
 *   SPI_SETFREQUENCY / SPI_SETMODE / SPI_SETBITS.
 *
 *   Pin mux is the board's responsibility (see board.h BOARD_SPI*_PINS).
 *
 * Returned Value:
 *   A non-NULL spi_dev_s on success; NULL on bad bus index.
 ****************************************************************************/

struct spi_dev_s *da1470x_spibus_initialize(int bus);

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_SPI_H */
