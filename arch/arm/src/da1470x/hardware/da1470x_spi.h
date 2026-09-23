/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_spi.h
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

/* Generated from the Renesas CMSIS device header DA1470x-00.h (SDK
 * 10.2.6.49).  Do not edit by hand; regenerate with tools/genhdr.py.
 */

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_SPI_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_SPI_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470x_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define DA1470X_SPI_CTRL_OFFSET 0x0000           /* Spi control register */
#define DA1470X_SPI_CONFIG_OFFSET 0x0004         /* Spi control register */
#define DA1470X_SPI_CLOCK_OFFSET 0x0008          /* Spi clock register */
#define DA1470X_SPI_FIFO_CONFIG_OFFSET 0x000c    /* Spi fifo configuration register */
#define DA1470X_SPI_IRQ_MASK_OFFSET 0x0010       /* Spi interrupt mask register */
#define DA1470X_SPI_STATUS_OFFSET 0x0014         /* Spi status register */
#define DA1470X_SPI_FIFO_STATUS_OFFSET 0x0018    /* SPI RX/TX fifo status register */
#define DA1470X_SPI_FIFO_READ_OFFSET 0x001c      /* Spi RX fifo read register */
#define DA1470X_SPI_FIFO_WRITE_OFFSET 0x0020     /* Spi TX fifo wtite register */
#define DA1470X_SPI_CS_CONFIG_OFFSET 0x0024      /* Spi cs configuration register */
#define DA1470X_SPI_TXBUFFER_FORCE_OFFSET 0x002c /* SPI TX buffer force low value */

/* Register Addresses *******************************************************/

#define DA1470X_SPI0_CTRL (DA1470X_SPI0_BASE + DA1470X_SPI_CTRL_OFFSET)
#define DA1470X_SPI0_CONFIG (DA1470X_SPI0_BASE + DA1470X_SPI_CONFIG_OFFSET)
#define DA1470X_SPI0_CLOCK (DA1470X_SPI0_BASE + DA1470X_SPI_CLOCK_OFFSET)
#define DA1470X_SPI0_FIFO_CONFIG (DA1470X_SPI0_BASE + DA1470X_SPI_FIFO_CONFIG_OFFSET)
#define DA1470X_SPI0_IRQ_MASK (DA1470X_SPI0_BASE + DA1470X_SPI_IRQ_MASK_OFFSET)
#define DA1470X_SPI0_STATUS (DA1470X_SPI0_BASE + DA1470X_SPI_STATUS_OFFSET)
#define DA1470X_SPI0_FIFO_STATUS (DA1470X_SPI0_BASE + DA1470X_SPI_FIFO_STATUS_OFFSET)
#define DA1470X_SPI0_FIFO_READ (DA1470X_SPI0_BASE + DA1470X_SPI_FIFO_READ_OFFSET)
#define DA1470X_SPI0_FIFO_WRITE (DA1470X_SPI0_BASE + DA1470X_SPI_FIFO_WRITE_OFFSET)
#define DA1470X_SPI0_CS_CONFIG (DA1470X_SPI0_BASE + DA1470X_SPI_CS_CONFIG_OFFSET)
#define DA1470X_SPI0_TXBUFFER_FORCE (DA1470X_SPI0_BASE + DA1470X_SPI_TXBUFFER_FORCE_OFFSET)

#define DA1470X_SPI1_CTRL (DA1470X_SPI1_BASE + DA1470X_SPI_CTRL_OFFSET)
#define DA1470X_SPI1_CONFIG (DA1470X_SPI1_BASE + DA1470X_SPI_CONFIG_OFFSET)
#define DA1470X_SPI1_CLOCK (DA1470X_SPI1_BASE + DA1470X_SPI_CLOCK_OFFSET)
#define DA1470X_SPI1_FIFO_CONFIG (DA1470X_SPI1_BASE + DA1470X_SPI_FIFO_CONFIG_OFFSET)
#define DA1470X_SPI1_IRQ_MASK (DA1470X_SPI1_BASE + DA1470X_SPI_IRQ_MASK_OFFSET)
#define DA1470X_SPI1_STATUS (DA1470X_SPI1_BASE + DA1470X_SPI_STATUS_OFFSET)
#define DA1470X_SPI1_FIFO_STATUS (DA1470X_SPI1_BASE + DA1470X_SPI_FIFO_STATUS_OFFSET)
#define DA1470X_SPI1_FIFO_READ (DA1470X_SPI1_BASE + DA1470X_SPI_FIFO_READ_OFFSET)
#define DA1470X_SPI1_FIFO_WRITE (DA1470X_SPI1_BASE + DA1470X_SPI_FIFO_WRITE_OFFSET)
#define DA1470X_SPI1_CS_CONFIG (DA1470X_SPI1_BASE + DA1470X_SPI_CS_CONFIG_OFFSET)
#define DA1470X_SPI1_TXBUFFER_FORCE (DA1470X_SPI1_BASE + DA1470X_SPI_TXBUFFER_FORCE_OFFSET)

#define DA1470X_SPI2_CTRL (DA1470X_SPI2_BASE + DA1470X_SPI_CTRL_OFFSET)
#define DA1470X_SPI2_CONFIG (DA1470X_SPI2_BASE + DA1470X_SPI_CONFIG_OFFSET)
#define DA1470X_SPI2_CLOCK (DA1470X_SPI2_BASE + DA1470X_SPI_CLOCK_OFFSET)
#define DA1470X_SPI2_FIFO_CONFIG (DA1470X_SPI2_BASE + DA1470X_SPI_FIFO_CONFIG_OFFSET)
#define DA1470X_SPI2_IRQ_MASK (DA1470X_SPI2_BASE + DA1470X_SPI_IRQ_MASK_OFFSET)
#define DA1470X_SPI2_STATUS (DA1470X_SPI2_BASE + DA1470X_SPI_STATUS_OFFSET)
#define DA1470X_SPI2_FIFO_STATUS (DA1470X_SPI2_BASE + DA1470X_SPI_FIFO_STATUS_OFFSET)
#define DA1470X_SPI2_FIFO_READ (DA1470X_SPI2_BASE + DA1470X_SPI_FIFO_READ_OFFSET)
#define DA1470X_SPI2_FIFO_WRITE (DA1470X_SPI2_BASE + DA1470X_SPI_FIFO_WRITE_OFFSET)
#define DA1470X_SPI2_CS_CONFIG (DA1470X_SPI2_BASE + DA1470X_SPI_CS_CONFIG_OFFSET)
#define DA1470X_SPI2_TXBUFFER_FORCE (DA1470X_SPI2_BASE + DA1470X_SPI_TXBUFFER_FORCE_OFFSET)

/* Register Bit-field Definitions *******************************************/

/* CTRL register */

#define SPI_CTRL_EN (1 << 0)                   /* Bit 0 */
#define SPI_CTRL_TX_EN (1 << 1)                /* Bit 1 */
#define SPI_CTRL_RX_EN (1 << 2)                /* Bit 2 */
#define SPI_CTRL_DMA_TX_EN (1 << 3)            /* Bit 3 */
#define SPI_CTRL_DMA_RX_EN (1 << 4)            /* Bit 4 */
#define SPI_CTRL_FIFO_RESET (1 << 5)           /* Bit 5 */
#define SPI_CTRL_CAPTURE_AT_NEXT_EDGE (1 << 6) /* Bit 6 */
#define SPI_CTRL_SWAP_BYTES (1 << 7)           /* Bit 7 */

/* CONFIG register */

#define SPI_CONFIG_MODE_SHIFT (0)        /* Bits 0-1 */
#define SPI_CONFIG_MODE_MASK (0x3 << SPI_CONFIG_MODE_SHIFT)
#  define SPI_CONFIG_MODE(n) ((uint32_t)(n) << SPI_CONFIG_MODE_SHIFT)
#define SPI_CONFIG_WORD_LENGTH_SHIFT (2) /* Bits 2-6 */
#define SPI_CONFIG_WORD_LENGTH_MASK (0x1f << SPI_CONFIG_WORD_LENGTH_SHIFT)
#  define SPI_CONFIG_WORD_LENGTH(n) ((uint32_t)(n) << SPI_CONFIG_WORD_LENGTH_SHIFT)
#define SPI_CONFIG_SLAVE_EN (1 << 7)     /* Bit 7 */

/* CLOCK register */

#define SPI_CLOCK_CLK_DIV_SHIFT (0) /* Bits 0-6 */
#define SPI_CLOCK_CLK_DIV_MASK (0x7f << SPI_CLOCK_CLK_DIV_SHIFT)
#  define SPI_CLOCK_CLK_DIV(n) ((uint32_t)(n) << SPI_CLOCK_CLK_DIV_SHIFT)

/* FIFO_CONFIG register */

#define SPI_FIFO_CONFIG_TX_TL_SHIFT (0) /* Bits 0-7 */
#define SPI_FIFO_CONFIG_TX_TL_MASK (0xff << SPI_FIFO_CONFIG_TX_TL_SHIFT)
#  define SPI_FIFO_CONFIG_TX_TL(n) ((uint32_t)(n) << SPI_FIFO_CONFIG_TX_TL_SHIFT)
#define SPI_FIFO_CONFIG_RX_TL_SHIFT (8) /* Bits 8-15 */
#define SPI_FIFO_CONFIG_RX_TL_MASK (0xff << SPI_FIFO_CONFIG_RX_TL_SHIFT)
#  define SPI_FIFO_CONFIG_RX_TL(n) ((uint32_t)(n) << SPI_FIFO_CONFIG_RX_TL_SHIFT)

/* IRQ_MASK register */

#define SPI_IRQ_MASK_IRQ_MASK_TX_EMPTY (1 << 0) /* Bit 0 */
#define SPI_IRQ_MASK_IRQ_MASK_RX_FULL (1 << 1)  /* Bit 1 */

/* STATUS register */

#define SPI_STATUS_STATUS_TX_EMPTY (1 << 0) /* Bit 0 */
#define SPI_STATUS_STATUS_RX_FULL (1 << 1)  /* Bit 1 */

/* FIFO_STATUS register */

#define SPI_FIFO_STATUS_RX_FIFO_LEVEL_SHIFT (0)      /* Bits 0-5 */
#define SPI_FIFO_STATUS_RX_FIFO_LEVEL_MASK (0x3f << SPI_FIFO_STATUS_RX_FIFO_LEVEL_SHIFT)
#  define SPI_FIFO_STATUS_RX_FIFO_LEVEL(n) ((uint32_t)(n) << SPI_FIFO_STATUS_RX_FIFO_LEVEL_SHIFT)
#define SPI_FIFO_STATUS_TX_FIFO_LEVEL_SHIFT (6)      /* Bits 6-11 */
#define SPI_FIFO_STATUS_TX_FIFO_LEVEL_MASK (0x3f << SPI_FIFO_STATUS_TX_FIFO_LEVEL_SHIFT)
#  define SPI_FIFO_STATUS_TX_FIFO_LEVEL(n) ((uint32_t)(n) << SPI_FIFO_STATUS_TX_FIFO_LEVEL_SHIFT)
#define SPI_FIFO_STATUS_STATUS_RX_EMPTY (1 << 12)    /* Bit 12 */
#define SPI_FIFO_STATUS_STATUS_TX_FULL (1 << 13)     /* Bit 13 */
#define SPI_FIFO_STATUS_RX_FIFO_OVFL (1 << 14)       /* Bit 14 */
#define SPI_FIFO_STATUS_TRANSACTION_ACTIVE (1 << 15) /* Bit 15 */

/* FIFO_READ register */

#define SPI_FIFO_READ_FIFO_READ_SHIFT (0) /* Bits 0-31 */
#define SPI_FIFO_READ_FIFO_READ_MASK (0xffffffff << SPI_FIFO_READ_FIFO_READ_SHIFT)
#  define SPI_FIFO_READ_FIFO_READ(n) ((uint32_t)(n) << SPI_FIFO_READ_FIFO_READ_SHIFT)

/* FIFO_WRITE register */

#define SPI_FIFO_WRITE_FIFO_WRITE_SHIFT (0) /* Bits 0-31 */
#define SPI_FIFO_WRITE_FIFO_WRITE_MASK (0xffffffff << SPI_FIFO_WRITE_FIFO_WRITE_SHIFT)
#  define SPI_FIFO_WRITE_FIFO_WRITE(n) ((uint32_t)(n) << SPI_FIFO_WRITE_FIFO_WRITE_SHIFT)

/* CS_CONFIG register */

#define SPI_CS_CONFIG_CS_SELECT_SHIFT (0) /* Bits 0-2 */
#define SPI_CS_CONFIG_CS_SELECT_MASK (0x7 << SPI_CS_CONFIG_CS_SELECT_SHIFT)
#  define SPI_CS_CONFIG_CS_SELECT(n) ((uint32_t)(n) << SPI_CS_CONFIG_CS_SELECT_SHIFT)

/* TXBUFFER_FORCE register */

#define SPI_TXBUFFER_FORCE_TXBUFFER_FORCE_SHIFT (0) /* Bits 0-31 */
#define SPI_TXBUFFER_FORCE_TXBUFFER_FORCE_MASK (0xffffffff << SPI_TXBUFFER_FORCE_TXBUFFER_FORCE_SHIFT)
#  define SPI_TXBUFFER_FORCE_TXBUFFER_FORCE(n) ((uint32_t)(n) << SPI_TXBUFFER_FORCE_TXBUFFER_FORCE_SHIFT)

/* Helpers ******************************************************************/

#define SPI_CONFIG_WORDLEN(bits) (((bits) - 1) << SPI_CONFIG_WORD_LENGTH_SHIFT)
#define SPI_CS_CONFIG_NONE 0
#define SPI_CS_CONFIG_CS0 1
#define SPI_CS_CONFIG_CS1 2
#define SPI_CS_CONFIG_GPIO 7
#define DA1470X_SPI_FIFO_DEPTH 32

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_SPI_H */
