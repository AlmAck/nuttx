/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_spi.h
 *
 * SPI controller register definitions for DA1470x.
 *
 * The DA1470x has three SPI controllers (SPI, SPI2, SPI3). SPI/SPI2 live
 * in PD_SNC at 0x5002_0300 / 0x5002_0400; SPI3 lives in the SYS domain
 * at 0x5100_0200. The register layouts are identical so this header
 * defines a single set of offsets.
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to you under the Apache License, Version
 * 2.0 (the "License"); you may not use this file except in compliance
 * with the License.
 *
 ****************************************************************************/

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_SPI_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_SPI_H

#include <nuttx/config.h>

#include "hardware/da1470x_memorymap.h"

/* Register offsets *********************************************************/

#define DA1470_SPI_CTRL_OFFSET           0x0000  /* SPI control register */
#define DA1470_SPI_CONFIG_OFFSET         0x0004  /* SPI config register */
#define DA1470_SPI_CLOCK_OFFSET          0x0008  /* Clock divider */
#define DA1470_SPI_FIFO_CONFIG_OFFSET    0x000C  /* FIFO threshold levels */
#define DA1470_SPI_IRQ_MASK_OFFSET       0x0010  /* IRQ mask */
#define DA1470_SPI_STATUS_OFFSET         0x0014  /* Status */
#define DA1470_SPI_FIFO_STATUS_OFFSET    0x0018  /* FIFO status */
#define DA1470_SPI_FIFO_READ_OFFSET      0x001C  /* RX FIFO read port */
#define DA1470_SPI_FIFO_WRITE_OFFSET     0x0020  /* TX FIFO write port */
#define DA1470_SPI_CS_CONFIG_OFFSET      0x0024  /* CS line selection */
#define DA1470_SPI_TXBUFFER_FORCE_OFFSET 0x002C  /* TX buffer force value */

/* SPI_CTRL_REG bits ********************************************************/

#define SPI_CTRL_SPI_EN                  (1U << 0)  /* Enable controller */
#define SPI_CTRL_TX_EN                   (1U << 1)  /* Enable TX path */
#define SPI_CTRL_RX_EN                   (1U << 2)  /* Enable RX path */
#define SPI_CTRL_FIFO_RESET              (1U << 5)  /* Reset RX+TX FIFOs */
#define SPI_CTRL_CAPTURE_AT_NEXT_EDGE    (1U << 6)  /* MSTR: capture next edge */
#define SPI_CTRL_SWAP_BYTES              (1U << 7)  /* Byte-swap on 16/32b */

/* SPI_CONFIG_REG bits ******************************************************/

#define SPI_CONFIG_MODE_SHIFT            0          /* CPOL/CPHA: 0..3 */
#define SPI_CONFIG_MODE_MASK             (0x3U << 0)
#define SPI_CONFIG_WORD_LENGTH_SHIFT     2          /* Word length minus 1 */
#define SPI_CONFIG_WORD_LENGTH_MASK      (0x1fU << 2)
#define SPI_CONFIG_SLAVE_EN              (1U << 7)  /* 1 = slave mode */

#define SPI_CONFIG_WORDLEN(bits)         (((bits) - 1) << SPI_CONFIG_WORD_LENGTH_SHIFT)

/* SPI_CLOCK_REG bits *******************************************************/

#define SPI_CLOCK_DIV_MASK               0x7FU      /* SCLK = SPI_CLK / (2*(div+1)) */

/* SPI_FIFO_CONFIG_REG bits *************************************************/

#define SPI_FIFO_CONFIG_TX_TL_SHIFT      0
#define SPI_FIFO_CONFIG_TX_TL_MASK       (0xFFU << 0)
#define SPI_FIFO_CONFIG_RX_TL_SHIFT      8
#define SPI_FIFO_CONFIG_RX_TL_MASK       (0xFFU << 8)

/* SPI_STATUS_REG bits ******************************************************/

#define SPI_STATUS_TX_EMPTY              (1U << 0)
#define SPI_STATUS_RX_FULL               (1U << 1)

/* SPI_FIFO_STATUS_REG bits *************************************************/

#define SPI_FIFO_STATUS_RX_LEVEL_SHIFT   0
#define SPI_FIFO_STATUS_RX_LEVEL_MASK    (0x3FU << 0)
#define SPI_FIFO_STATUS_TX_LEVEL_SHIFT   6
#define SPI_FIFO_STATUS_TX_LEVEL_MASK    (0x3FU << 6)
#define SPI_FIFO_STATUS_RX_EMPTY         (1U << 12)
#define SPI_FIFO_STATUS_TX_FULL          (1U << 13)
#define SPI_FIFO_STATUS_RX_FIFO_OVFL     (1U << 14)
#define SPI_FIFO_STATUS_TRANSACTION_ACTIVE (1U << 15)

/* SPI_CS_CONFIG_REG bits ***************************************************/

#define SPI_CS_CONFIG_SELECT_SHIFT       0
#define SPI_CS_CONFIG_SELECT_MASK        (0x7U << 0)
#define SPI_CS_CONFIG_NONE               0
#define SPI_CS_CONFIG_CS0                1
#define SPI_CS_CONFIG_CS1                2
#define SPI_CS_CONFIG_GPIO               7   /* CS driven by GPIO from caller */

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_SPI_H */
