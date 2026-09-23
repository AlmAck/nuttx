/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_uart.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_UART_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_UART_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470x_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define DA1470X_UART_RBR_THR_DLL_OFFSET 0x0000 /* Receive Buffer Register */
#define DA1470X_UART_IER_DLH_OFFSET 0x0004     /* Interrupt Enable Register */
#define DA1470X_UART_IIR_FCR_OFFSET 0x0008     /* Interrupt Identification Register/FIFO C */
#define DA1470X_UART_LCR_OFFSET 0x000c         /* Line Control Register */
#define DA1470X_UART_MCR_OFFSET 0x0010         /* Modem Control Register */
#define DA1470X_UART_LSR_OFFSET 0x0014         /* Line Status Register */
#define DA1470X_UART_SCR_OFFSET 0x001c         /* Scratchpad Register */
#define DA1470X_UART_SRBR_STHR0_OFFSET 0x0030  /* Shadow Receive/Transmit Buffer Register */
#define DA1470X_UART_SRBR_STHR1_OFFSET 0x0034  /* Shadow Receive/Transmit Buffer Register */
#define DA1470X_UART_SRBR_STHR2_OFFSET 0x0038  /* Shadow Receive/Transmit Buffer Register */
#define DA1470X_UART_SRBR_STHR3_OFFSET 0x003c  /* Shadow Receive/Transmit Buffer Register */
#define DA1470X_UART_SRBR_STHR4_OFFSET 0x0040  /* Shadow Receive/Transmit Buffer Register */
#define DA1470X_UART_SRBR_STHR5_OFFSET 0x0044  /* Shadow Receive/Transmit Buffer Register */
#define DA1470X_UART_SRBR_STHR6_OFFSET 0x0048  /* Shadow Receive/Transmit Buffer Register */
#define DA1470X_UART_SRBR_STHR7_OFFSET 0x004c  /* Shadow Receive/Transmit Buffer Register */
#define DA1470X_UART_SRBR_STHR8_OFFSET 0x0050  /* Shadow Receive/Transmit Buffer Register */
#define DA1470X_UART_SRBR_STHR9_OFFSET 0x0054  /* Shadow Receive/Transmit Buffer Register */
#define DA1470X_UART_SRBR_STHR10_OFFSET 0x0058 /* Shadow Receive/Transmit Buffer Register */
#define DA1470X_UART_SRBR_STHR11_OFFSET 0x005c /* Shadow Receive/Transmit Buffer Register */
#define DA1470X_UART_SRBR_STHR12_OFFSET 0x0060 /* Shadow Receive/Transmit Buffer Register */
#define DA1470X_UART_SRBR_STHR13_OFFSET 0x0064 /* Shadow Receive/Transmit Buffer Register */
#define DA1470X_UART_SRBR_STHR14_OFFSET 0x0068 /* Shadow Receive/Transmit Buffer Register */
#define DA1470X_UART_SRBR_STHR15_OFFSET 0x006c /* Shadow Receive/Transmit Buffer Register */
#define DA1470X_UART_USR_OFFSET 0x007c         /* UART Status register. */
#define DA1470X_UART_TFL_OFFSET 0x0080         /* Transmit FIFO Level */
#define DA1470X_UART_RFL_OFFSET 0x0084         /* Receive FIFO Level. */
#define DA1470X_UART_SRR_OFFSET 0x0088         /* Software Reset Register. */
#define DA1470X_UART_SBCR_OFFSET 0x0090        /* Shadow Break Control Register */
#define DA1470X_UART_SDMAM_OFFSET 0x0094       /* Shadow DMA Mode */
#define DA1470X_UART_SFE_OFFSET 0x0098         /* Shadow FIFO Enable */
#define DA1470X_UART_SRT_OFFSET 0x009c         /* Shadow RCVR Trigger */
#define DA1470X_UART_STET_OFFSET 0x00a0        /* Shadow TX Empty Trigger */
#define DA1470X_UART_HTX_OFFSET 0x00a4         /* Halt TX */
#define DA1470X_UART_DMASA_OFFSET 0x00a8       /* DMA Software Acknowledge */
#define DA1470X_UART_DLF_OFFSET 0x00c0         /* Divisor Latch Fraction Register */
#define DA1470X_UART_UCV_OFFSET 0x00f8         /* Component Version */
#define DA1470X_UART_CTR_OFFSET 0x00fc         /* Component Type Register */

/* Register Addresses *******************************************************/

#define DA1470X_UART_RBR_THR_DLL (DA1470X_UART_BASE + DA1470X_UART_RBR_THR_DLL_OFFSET)
#define DA1470X_UART_IER_DLH (DA1470X_UART_BASE + DA1470X_UART_IER_DLH_OFFSET)
#define DA1470X_UART_IIR_FCR (DA1470X_UART_BASE + DA1470X_UART_IIR_FCR_OFFSET)
#define DA1470X_UART_LCR (DA1470X_UART_BASE + DA1470X_UART_LCR_OFFSET)
#define DA1470X_UART_MCR (DA1470X_UART_BASE + DA1470X_UART_MCR_OFFSET)
#define DA1470X_UART_LSR (DA1470X_UART_BASE + DA1470X_UART_LSR_OFFSET)
#define DA1470X_UART_SCR (DA1470X_UART_BASE + DA1470X_UART_SCR_OFFSET)
#define DA1470X_UART_SRBR_STHR0 (DA1470X_UART_BASE + DA1470X_UART_SRBR_STHR0_OFFSET)
#define DA1470X_UART_SRBR_STHR1 (DA1470X_UART_BASE + DA1470X_UART_SRBR_STHR1_OFFSET)
#define DA1470X_UART_SRBR_STHR2 (DA1470X_UART_BASE + DA1470X_UART_SRBR_STHR2_OFFSET)
#define DA1470X_UART_SRBR_STHR3 (DA1470X_UART_BASE + DA1470X_UART_SRBR_STHR3_OFFSET)
#define DA1470X_UART_SRBR_STHR4 (DA1470X_UART_BASE + DA1470X_UART_SRBR_STHR4_OFFSET)
#define DA1470X_UART_SRBR_STHR5 (DA1470X_UART_BASE + DA1470X_UART_SRBR_STHR5_OFFSET)
#define DA1470X_UART_SRBR_STHR6 (DA1470X_UART_BASE + DA1470X_UART_SRBR_STHR6_OFFSET)
#define DA1470X_UART_SRBR_STHR7 (DA1470X_UART_BASE + DA1470X_UART_SRBR_STHR7_OFFSET)
#define DA1470X_UART_SRBR_STHR8 (DA1470X_UART_BASE + DA1470X_UART_SRBR_STHR8_OFFSET)
#define DA1470X_UART_SRBR_STHR9 (DA1470X_UART_BASE + DA1470X_UART_SRBR_STHR9_OFFSET)
#define DA1470X_UART_SRBR_STHR10 (DA1470X_UART_BASE + DA1470X_UART_SRBR_STHR10_OFFSET)
#define DA1470X_UART_SRBR_STHR11 (DA1470X_UART_BASE + DA1470X_UART_SRBR_STHR11_OFFSET)
#define DA1470X_UART_SRBR_STHR12 (DA1470X_UART_BASE + DA1470X_UART_SRBR_STHR12_OFFSET)
#define DA1470X_UART_SRBR_STHR13 (DA1470X_UART_BASE + DA1470X_UART_SRBR_STHR13_OFFSET)
#define DA1470X_UART_SRBR_STHR14 (DA1470X_UART_BASE + DA1470X_UART_SRBR_STHR14_OFFSET)
#define DA1470X_UART_SRBR_STHR15 (DA1470X_UART_BASE + DA1470X_UART_SRBR_STHR15_OFFSET)
#define DA1470X_UART_USR (DA1470X_UART_BASE + DA1470X_UART_USR_OFFSET)
#define DA1470X_UART_TFL (DA1470X_UART_BASE + DA1470X_UART_TFL_OFFSET)
#define DA1470X_UART_RFL (DA1470X_UART_BASE + DA1470X_UART_RFL_OFFSET)
#define DA1470X_UART_SRR (DA1470X_UART_BASE + DA1470X_UART_SRR_OFFSET)
#define DA1470X_UART_SBCR (DA1470X_UART_BASE + DA1470X_UART_SBCR_OFFSET)
#define DA1470X_UART_SDMAM (DA1470X_UART_BASE + DA1470X_UART_SDMAM_OFFSET)
#define DA1470X_UART_SFE (DA1470X_UART_BASE + DA1470X_UART_SFE_OFFSET)
#define DA1470X_UART_SRT (DA1470X_UART_BASE + DA1470X_UART_SRT_OFFSET)
#define DA1470X_UART_STET (DA1470X_UART_BASE + DA1470X_UART_STET_OFFSET)
#define DA1470X_UART_HTX (DA1470X_UART_BASE + DA1470X_UART_HTX_OFFSET)
#define DA1470X_UART_DMASA (DA1470X_UART_BASE + DA1470X_UART_DMASA_OFFSET)
#define DA1470X_UART_DLF (DA1470X_UART_BASE + DA1470X_UART_DLF_OFFSET)
#define DA1470X_UART_UCV (DA1470X_UART_BASE + DA1470X_UART_UCV_OFFSET)
#define DA1470X_UART_CTR (DA1470X_UART_BASE + DA1470X_UART_CTR_OFFSET)

/* Register Bit-field Definitions *******************************************/

/* RBR_THR_DLL register */

#define UART_RBR_THR_DLL_RBR_THR_DLL_SHIFT (0) /* Bits 0-7 */
#define UART_RBR_THR_DLL_RBR_THR_DLL_MASK (0xff << UART_RBR_THR_DLL_RBR_THR_DLL_SHIFT)
#  define UART_RBR_THR_DLL_RBR_THR_DLL(n) ((uint32_t)(n) << UART_RBR_THR_DLL_RBR_THR_DLL_SHIFT)

/* IER_DLH register */

#define UART_IER_DLH_ERBFI_DLH0 (1 << 0)  /* Bit 0 */
#define UART_IER_DLH_ETBEI_DLH1 (1 << 1)  /* Bit 1 */
#define UART_IER_DLH_ELSI_DLH2 (1 << 2)   /* Bit 2 */
#define UART_IER_DLH_EDSSI_DLH3 (1 << 3)  /* Bit 3 */
#define UART_IER_DLH_ELCOLR_DLH4 (1 << 4) /* Bit 4 */
#define UART_IER_DLH_DLH6_5_SHIFT (5)     /* Bits 5-6 */
#define UART_IER_DLH_DLH6_5_MASK (0x3 << UART_IER_DLH_DLH6_5_SHIFT)
#  define UART_IER_DLH_DLH6_5(n) ((uint32_t)(n) << UART_IER_DLH_DLH6_5_SHIFT)
#define UART_IER_DLH_PTIME_DLH7 (1 << 7)  /* Bit 7 */

/* IIR_FCR register */

#define UART_IIR_FCR_IIR_FCR_SHIFT (0) /* Bits 0-7 */
#define UART_IIR_FCR_IIR_FCR_MASK (0xff << UART_IIR_FCR_IIR_FCR_SHIFT)
#  define UART_IIR_FCR_IIR_FCR(n) ((uint32_t)(n) << UART_IIR_FCR_IIR_FCR_SHIFT)

/* LCR register */

#define UART_LCR_DLS_SHIFT (0) /* Bits 0-1 */
#define UART_LCR_DLS_MASK (0x3 << UART_LCR_DLS_SHIFT)
#  define UART_LCR_DLS(n) ((uint32_t)(n) << UART_LCR_DLS_SHIFT)
#define UART_LCR_STOP (1 << 2) /* Bit 2 */
#define UART_LCR_PEN (1 << 3)  /* Bit 3 */
#define UART_LCR_EPS (1 << 4)  /* Bit 4 */
#define UART_LCR_BC (1 << 6)   /* Bit 6 */
#define UART_LCR_DLAB (1 << 7) /* Bit 7 */

/* MCR register */

#define UART_MCR_LB (1 << 4) /* Bit 4 */

/* LSR register */

#define UART_LSR_DR (1 << 0)   /* Bit 0 */
#define UART_LSR_OE (1 << 1)   /* Bit 1 */
#define UART_LSR_PE (1 << 2)   /* Bit 2 */
#define UART_LSR_FE (1 << 3)   /* Bit 3 */
#define UART_LSR_BI (1 << 4)   /* Bit 4 */
#define UART_LSR_THRE (1 << 5) /* Bit 5 */
#define UART_LSR_TEMT (1 << 6) /* Bit 6 */
#define UART_LSR_RFE (1 << 7)  /* Bit 7 */

/* SCR register */

#define UART_SCR_SCRATCH_PAD_SHIFT (0) /* Bits 0-7 */
#define UART_SCR_SCRATCH_PAD_MASK (0xff << UART_SCR_SCRATCH_PAD_SHIFT)
#  define UART_SCR_SCRATCH_PAD(n) ((uint32_t)(n) << UART_SCR_SCRATCH_PAD_SHIFT)

/* SRBR_STHR0 register */

#define UART_SRBR_STHR0_SRBR_STHRx_SHIFT (0) /* Bits 0-7 */
#define UART_SRBR_STHR0_SRBR_STHRx_MASK (0xff << UART_SRBR_STHR0_SRBR_STHRx_SHIFT)
#  define UART_SRBR_STHR0_SRBR_STHRx(n) ((uint32_t)(n) << UART_SRBR_STHR0_SRBR_STHRx_SHIFT)

/* SRBR_STHR1 register */

#define UART_SRBR_STHR1_SRBR_STHRx_SHIFT (0) /* Bits 0-7 */
#define UART_SRBR_STHR1_SRBR_STHRx_MASK (0xff << UART_SRBR_STHR1_SRBR_STHRx_SHIFT)
#  define UART_SRBR_STHR1_SRBR_STHRx(n) ((uint32_t)(n) << UART_SRBR_STHR1_SRBR_STHRx_SHIFT)

/* SRBR_STHR2 register */

#define UART_SRBR_STHR2_SRBR_STHRx_SHIFT (0) /* Bits 0-7 */
#define UART_SRBR_STHR2_SRBR_STHRx_MASK (0xff << UART_SRBR_STHR2_SRBR_STHRx_SHIFT)
#  define UART_SRBR_STHR2_SRBR_STHRx(n) ((uint32_t)(n) << UART_SRBR_STHR2_SRBR_STHRx_SHIFT)

/* SRBR_STHR3 register */

#define UART_SRBR_STHR3_SRBR_STHRx_SHIFT (0) /* Bits 0-7 */
#define UART_SRBR_STHR3_SRBR_STHRx_MASK (0xff << UART_SRBR_STHR3_SRBR_STHRx_SHIFT)
#  define UART_SRBR_STHR3_SRBR_STHRx(n) ((uint32_t)(n) << UART_SRBR_STHR3_SRBR_STHRx_SHIFT)

/* SRBR_STHR4 register */

#define UART_SRBR_STHR4_SRBR_STHRx_SHIFT (0) /* Bits 0-7 */
#define UART_SRBR_STHR4_SRBR_STHRx_MASK (0xff << UART_SRBR_STHR4_SRBR_STHRx_SHIFT)
#  define UART_SRBR_STHR4_SRBR_STHRx(n) ((uint32_t)(n) << UART_SRBR_STHR4_SRBR_STHRx_SHIFT)

/* SRBR_STHR5 register */

#define UART_SRBR_STHR5_SRBR_STHRx_SHIFT (0) /* Bits 0-7 */
#define UART_SRBR_STHR5_SRBR_STHRx_MASK (0xff << UART_SRBR_STHR5_SRBR_STHRx_SHIFT)
#  define UART_SRBR_STHR5_SRBR_STHRx(n) ((uint32_t)(n) << UART_SRBR_STHR5_SRBR_STHRx_SHIFT)

/* SRBR_STHR6 register */

#define UART_SRBR_STHR6_SRBR_STHRx_SHIFT (0) /* Bits 0-7 */
#define UART_SRBR_STHR6_SRBR_STHRx_MASK (0xff << UART_SRBR_STHR6_SRBR_STHRx_SHIFT)
#  define UART_SRBR_STHR6_SRBR_STHRx(n) ((uint32_t)(n) << UART_SRBR_STHR6_SRBR_STHRx_SHIFT)

/* SRBR_STHR7 register */

#define UART_SRBR_STHR7_SRBR_STHRx_SHIFT (0) /* Bits 0-7 */
#define UART_SRBR_STHR7_SRBR_STHRx_MASK (0xff << UART_SRBR_STHR7_SRBR_STHRx_SHIFT)
#  define UART_SRBR_STHR7_SRBR_STHRx(n) ((uint32_t)(n) << UART_SRBR_STHR7_SRBR_STHRx_SHIFT)

/* SRBR_STHR8 register */

#define UART_SRBR_STHR8_SRBR_STHRx_SHIFT (0) /* Bits 0-7 */
#define UART_SRBR_STHR8_SRBR_STHRx_MASK (0xff << UART_SRBR_STHR8_SRBR_STHRx_SHIFT)
#  define UART_SRBR_STHR8_SRBR_STHRx(n) ((uint32_t)(n) << UART_SRBR_STHR8_SRBR_STHRx_SHIFT)

/* SRBR_STHR9 register */

#define UART_SRBR_STHR9_SRBR_STHRx_SHIFT (0) /* Bits 0-7 */
#define UART_SRBR_STHR9_SRBR_STHRx_MASK (0xff << UART_SRBR_STHR9_SRBR_STHRx_SHIFT)
#  define UART_SRBR_STHR9_SRBR_STHRx(n) ((uint32_t)(n) << UART_SRBR_STHR9_SRBR_STHRx_SHIFT)

/* SRBR_STHR10 register */

#define UART_SRBR_STHR10_SRBR_STHRx_SHIFT (0) /* Bits 0-7 */
#define UART_SRBR_STHR10_SRBR_STHRx_MASK (0xff << UART_SRBR_STHR10_SRBR_STHRx_SHIFT)
#  define UART_SRBR_STHR10_SRBR_STHRx(n) ((uint32_t)(n) << UART_SRBR_STHR10_SRBR_STHRx_SHIFT)

/* SRBR_STHR11 register */

#define UART_SRBR_STHR11_SRBR_STHRx_SHIFT (0) /* Bits 0-7 */
#define UART_SRBR_STHR11_SRBR_STHRx_MASK (0xff << UART_SRBR_STHR11_SRBR_STHRx_SHIFT)
#  define UART_SRBR_STHR11_SRBR_STHRx(n) ((uint32_t)(n) << UART_SRBR_STHR11_SRBR_STHRx_SHIFT)

/* SRBR_STHR12 register */

#define UART_SRBR_STHR12_SRBR_STHRx_SHIFT (0) /* Bits 0-7 */
#define UART_SRBR_STHR12_SRBR_STHRx_MASK (0xff << UART_SRBR_STHR12_SRBR_STHRx_SHIFT)
#  define UART_SRBR_STHR12_SRBR_STHRx(n) ((uint32_t)(n) << UART_SRBR_STHR12_SRBR_STHRx_SHIFT)

/* SRBR_STHR13 register */

#define UART_SRBR_STHR13_SRBR_STHRx_SHIFT (0) /* Bits 0-7 */
#define UART_SRBR_STHR13_SRBR_STHRx_MASK (0xff << UART_SRBR_STHR13_SRBR_STHRx_SHIFT)
#  define UART_SRBR_STHR13_SRBR_STHRx(n) ((uint32_t)(n) << UART_SRBR_STHR13_SRBR_STHRx_SHIFT)

/* SRBR_STHR14 register */

#define UART_SRBR_STHR14_SRBR_STHRx_SHIFT (0) /* Bits 0-7 */
#define UART_SRBR_STHR14_SRBR_STHRx_MASK (0xff << UART_SRBR_STHR14_SRBR_STHRx_SHIFT)
#  define UART_SRBR_STHR14_SRBR_STHRx(n) ((uint32_t)(n) << UART_SRBR_STHR14_SRBR_STHRx_SHIFT)

/* SRBR_STHR15 register */

#define UART_SRBR_STHR15_SRBR_STHRx_SHIFT (0) /* Bits 0-7 */
#define UART_SRBR_STHR15_SRBR_STHRx_MASK (0xff << UART_SRBR_STHR15_SRBR_STHRx_SHIFT)
#  define UART_SRBR_STHR15_SRBR_STHRx(n) ((uint32_t)(n) << UART_SRBR_STHR15_SRBR_STHRx_SHIFT)

/* USR register */

#define UART_USR_BUSY (1 << 0) /* Bit 0 */
#define UART_USR_TFNF (1 << 1) /* Bit 1 */
#define UART_USR_TFE (1 << 2)  /* Bit 2 */
#define UART_USR_RFNE (1 << 3) /* Bit 3 */
#define UART_USR_RFF (1 << 4)  /* Bit 4 */

/* TFL register */

#define UART_TFL_TRANSMIT_FIFO_LEVEL_SHIFT (0) /* Bits 0-4 */
#define UART_TFL_TRANSMIT_FIFO_LEVEL_MASK (0x1f << UART_TFL_TRANSMIT_FIFO_LEVEL_SHIFT)
#  define UART_TFL_TRANSMIT_FIFO_LEVEL(n) ((uint32_t)(n) << UART_TFL_TRANSMIT_FIFO_LEVEL_SHIFT)

/* RFL register */

#define UART_RFL_RECEIVE_FIFO_LEVEL_SHIFT (0) /* Bits 0-4 */
#define UART_RFL_RECEIVE_FIFO_LEVEL_MASK (0x1f << UART_RFL_RECEIVE_FIFO_LEVEL_SHIFT)
#  define UART_RFL_RECEIVE_FIFO_LEVEL(n) ((uint32_t)(n) << UART_RFL_RECEIVE_FIFO_LEVEL_SHIFT)

/* SRR register */

#define UART_SRR_UR (1 << 0)  /* Bit 0 */
#define UART_SRR_RFR (1 << 1) /* Bit 1 */
#define UART_SRR_XFR (1 << 2) /* Bit 2 */

/* SBCR register */

#define UART_SBCR_SHADOW_BREAK_CONTROL (1 << 0) /* Bit 0 */

/* SDMAM register */

#define UART_SDMAM_SHADOW_DMA_MODE (1 << 0) /* Bit 0 */

/* SFE register */

#define UART_SFE_SHADOW_FIFO_ENABLE (1 << 0) /* Bit 0 */

/* SRT register */

#define UART_SRT_SHADOW_RCVR_TRIGGER_SHIFT (0) /* Bits 0-1 */
#define UART_SRT_SHADOW_RCVR_TRIGGER_MASK (0x3 << UART_SRT_SHADOW_RCVR_TRIGGER_SHIFT)
#  define UART_SRT_SHADOW_RCVR_TRIGGER(n) ((uint32_t)(n) << UART_SRT_SHADOW_RCVR_TRIGGER_SHIFT)

/* STET register */

#define UART_STET_SHADOW_TX_EMPTY_TRIGGER_SHIFT (0) /* Bits 0-1 */
#define UART_STET_SHADOW_TX_EMPTY_TRIGGER_MASK (0x3 << UART_STET_SHADOW_TX_EMPTY_TRIGGER_SHIFT)
#  define UART_STET_SHADOW_TX_EMPTY_TRIGGER(n) ((uint32_t)(n) << UART_STET_SHADOW_TX_EMPTY_TRIGGER_SHIFT)

/* HTX register */

#define UART_HTX_HALT_TX (1 << 0) /* Bit 0 */

/* DMASA register */

#define UART_DMASA_DMASA (1 << 0) /* Bit 0 */

/* DLF register */

#define UART_DLF_DLF_SHIFT (0) /* Bits 0-3 */
#define UART_DLF_DLF_MASK (0xf << UART_DLF_DLF_SHIFT)
#  define UART_DLF_DLF(n) ((uint32_t)(n) << UART_DLF_DLF_SHIFT)

/* UCV register */

#define UART_UCV_UCV_SHIFT (0) /* Bits 0-31 */
#define UART_UCV_UCV_MASK (0xffffffff << UART_UCV_UCV_SHIFT)
#  define UART_UCV_UCV(n) ((uint32_t)(n) << UART_UCV_UCV_SHIFT)

/* CTR register */

#define UART_CTR_CTR_SHIFT (0) /* Bits 0-31 */
#define UART_CTR_CTR_MASK (0xffffffff << UART_CTR_CTR_SHIFT)
#  define UART_CTR_CTR(n) ((uint32_t)(n) << UART_CTR_CTR_SHIFT)

/* IIR (read) interrupt identification **************************************/

#define UART_IIR_IID_SHIFT (0)                           /* Bits 0-3: Interrupt ID */
#define UART_IIR_IID_MASK (0xf << UART_IIR_IID_SHIFT)
#  define UART_IIR_IID_MODEM (0 << UART_IIR_IID_SHIFT)   /* Modem status */
#  define UART_IIR_IID_NONE (1 << UART_IIR_IID_SHIFT)    /* No interrupt pending */
#  define UART_IIR_IID_THRE (2 << UART_IIR_IID_SHIFT)    /* THR empty */
#  define UART_IIR_IID_RDA (4 << UART_IIR_IID_SHIFT)     /* Received data available */
#  define UART_IIR_IID_RLS (6 << UART_IIR_IID_SHIFT)     /* Receiver line status */
#  define UART_IIR_IID_BUSY (7 << UART_IIR_IID_SHIFT)    /* Busy detect */
#  define UART_IIR_IID_CTI (12 << UART_IIR_IID_SHIFT)    /* Character timeout */
#define UART_IIR_FIFOSE_SHIFT (6)                        /* Bits 6-7: FIFOs enabled */
#define UART_IIR_FIFOSE_MASK (3 << UART_IIR_FIFOSE_SHIFT)

/* FCR (write) FIFO control *************************************************/

#define UART_FCR_FIFOE (1 << 0)                          /* Bit 0: FIFO enable */
#define UART_FCR_RFIFOR (1 << 1)                         /* Bit 1: RX FIFO reset */
#define UART_FCR_XFIFOR (1 << 2)                         /* Bit 2: TX FIFO reset */
#define UART_FCR_DMAM (1 << 3)                           /* Bit 3: DMA mode */
#define UART_FCR_TET_SHIFT (4)                           /* Bits 4-5: TX empty trigger */
#define UART_FCR_TET_MASK (3 << UART_FCR_TET_SHIFT)
#  define UART_FCR_TET_EMPTY (0 << UART_FCR_TET_SHIFT)   /* FIFO empty */
#  define UART_FCR_TET_2CHAR (1 << UART_FCR_TET_SHIFT)   /* 2 characters */
#  define UART_FCR_TET_QUARTER (2 << UART_FCR_TET_SHIFT) /* 1/4 full */
#  define UART_FCR_TET_HALF (3 << UART_FCR_TET_SHIFT)    /* 1/2 full */
#define UART_FCR_RT_SHIFT (6)                            /* Bits 6-7: RX trigger */
#define UART_FCR_RT_MASK (3 << UART_FCR_RT_SHIFT)
#  define UART_FCR_RT_1CHAR (0 << UART_FCR_RT_SHIFT)     /* 1 character */
#  define UART_FCR_RT_QUARTER (1 << UART_FCR_RT_SHIFT)   /* 1/4 full */
#  define UART_FCR_RT_HALF (2 << UART_FCR_RT_SHIFT)      /* 1/2 full */
#  define UART_FCR_RT_2LESS (3 << UART_FCR_RT_SHIFT)     /* 2 less than full */

/* MCR modem control (UART2/UART3 only for AFCE/RTS) ************************/

#define UART_MCR_RTS (1 << 1)                            /* Bit 1: Request to send */
#define UART_MCR_AFCE (1 << 5)                           /* Bit 5: Auto flow control enable */

/* LCR data length select values ********************************************/

#define UART_LCR_DLS_5BITS (0 << UART_LCR_DLS_SHIFT)
#define UART_LCR_DLS_6BITS (1 << UART_LCR_DLS_SHIFT)
#define UART_LCR_DLS_7BITS (2 << UART_LCR_DLS_SHIFT)
#define UART_LCR_DLS_8BITS (3 << UART_LCR_DLS_SHIFT)

#define DA1470X_UART_FIFO_DEPTH 16                       /* TX/RX FIFO depth */

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_UART_H */
