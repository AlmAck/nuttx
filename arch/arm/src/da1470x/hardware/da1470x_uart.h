/***************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_uarte.h
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
 ***************************************************************************/

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_UART_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_UART_H

/***************************************************************************
 * Included Files
 ***************************************************************************/

#include <nuttx/config.h>
#include "hardware/da1470x_memorymap.h"

/***************************************************************************
 * Pre-processor Definitions
 ***************************************************************************/

/* UART Register Offsets *********************************************/


/* Register offsets *********************************************************/

#define DA1470_UART_CTR_OFFSET            0x00FC /* Component Type Register */
#define DA1470_UART_DLF_OFFSET            0x00C0 /* Divisor Latch Fraction Register */
#define DA1470_UART_DMASA_OFFSET          0x00A8 /* DMA Software Acknowledge */
#define DA1470_UART_HTX_OFFSET            0x00A4 /* Halt TX */
#define DA1470_UART_IER_DLH_OFFSET        0x0004 /* Interrupt Enable Register */
#define DA1470_UART_IIR_FCR_OFFSET        0x0008 /* Interrupt Identification Register/FIFO Control Register */
#define DA1470_UART_LCR_OFFSET            0x000C /* Line Control Register */
#define DA1470_UART_LSR_OFFSET            0x0014 /* Line Status Register */
#define DA1470_UART_MCR_OFFSET            0x0010 /* Modem Control Register */
#define DA1470_UART_RBR_THR_DLL_OFFSET    0x0000 /* Receive Buffer Register */
#define DA1470_UART_RFL_OFFSET            0x0084 /* Receive FIFO Level. */
#define DA1470_UART_SBCR_OFFSET           0x0090 /* Shadow Break Control Register */
#define DA1470_UART_SCR_OFFSET            0x001C /* Scratchpad Register */
#define DA1470_UART_SDMAM_OFFSET          0x0094 /* Shadow DMA Mode */
#define DA1470_UART_SFE_OFFSET            0x0098 /* Shadow FIFO Enable */
#define DA1470_UART_SRBR_STHR0_OFFSET     0x0030 /* Shadow Receive/Transmit Buffer Register */
#define DA1470_UART_SRBR_STHR10_OFFSET    0x0058 /* Shadow Receive/Transmit Buffer Register */
#define DA1470_UART_SRBR_STHR11_OFFSET    0x005C /* Shadow Receive/Transmit Buffer Register */
#define DA1470_UART_SRBR_STHR12_OFFSET    0x0060 /* Shadow Receive/Transmit Buffer Register */
#define DA1470_UART_SRBR_STHR13_OFFSET    0x0064 /* Shadow Receive/Transmit Buffer Register */
#define DA1470_UART_SRBR_STHR14_OFFSET    0x0068 /* Shadow Receive/Transmit Buffer Register */
#define DA1470_UART_SRBR_STHR15_OFFSET    0x006C /* Shadow Receive/Transmit Buffer Register */
#define DA1470_UART_SRBR_STHR1_OFFSET     0x0034 /* Shadow Receive/Transmit Buffer Register */
#define DA1470_UART_SRBR_STHR2_OFFSET     0x0038 /* Shadow Receive/Transmit Buffer Register */
#define DA1470_UART_SRBR_STHR3_OFFSET     0x003C /* Shadow Receive/Transmit Buffer Register */
#define DA1470_UART_SRBR_STHR4_OFFSET     0x0040 /* Shadow Receive/Transmit Buffer Register */
#define DA1470_UART_SRBR_STHR5_OFFSET     0x0044 /* Shadow Receive/Transmit Buffer Register */
#define DA1470_UART_SRBR_STHR6_OFFSET     0x0048 /* Shadow Receive/Transmit Buffer Register */
#define DA1470_UART_SRBR_STHR7_OFFSET     0x004C /* Shadow Receive/Transmit Buffer Register */
#define DA1470_UART_SRBR_STHR8_OFFSET     0x0050 /* Shadow Receive/Transmit Buffer Register */
#define DA1470_UART_SRBR_STHR9_OFFSET     0x0054 /* Shadow Receive/Transmit Buffer Register */
#define DA1470_UART_SRR_OFFSET            0x0088 /* Software Reset Register. */
#define DA1470_UART_SRT_OFFSET            0x009C /* Shadow RCVR Trigger */
#define DA1470_UART_STET_OFFSET           0x00A0 /* Shadow TX Empty Trigger */
#define DA1470_UART_TFL_OFFSET            0x0080 /* Transmit FIFO Level */
#define DA1470_UART_UCV_OFFSET            0x00F8 /* Component Version */
#define DA1470_UART_USR_OFFSET            0x007C /* UART Status register. */


/* UART/UARTE Register Addresses *******************************************/

/* Register addresses are calculated as base address + register offset */
#define DA1470_UART0_CTR          (DA1470X_UART0_BASE + DA1470_UART_CTR_OFFSET)
#define DA1470_UART0_DLF          (DA1470X_UART0_BASE + DA1470_UART_DLF_OFFSET)
#define DA1470_UART0_DMASA        (DA1470X_UART0_BASE + DA1470_UART_DMASA_OFFSET)
#define DA1470_UART0_HTX          (DA1470X_UART0_BASE + DA1470_UART_HTX_OFFSET)
#define DA1470_UART0_IER_DLH      (DA1470X_UART0_BASE + DA1470_UART_IER_DLH_OFFSET)
#define DA1470_UART0_IIR_FCR      (DA1470X_UART0_BASE + DA1470_UART_IIR_FCR_OFFSET)
#define DA1470_UART0_LCR          (DA1470X_UART0_BASE + DA1470_UART_LCR_OFFSET)
#define DA1470_UART0_LSR          (DA1470X_UART0_BASE + DA1470_UART_LSR_OFFSET)
#define DA1470_UART0_MCR          (DA1470X_UART0_BASE + DA1470_UART_MCR_OFFSET)
#define DA1470_UART0_RBR_THR_DLL  (DA1470X_UART0_BASE + DA1470_UART_RBR_THR_DLL_OFFSET)
#define DA1470_UART0_RFL          (DA1470X_UART0_BASE + DA1470_UART_RFL_OFFSET)
#define DA1470_UART0_SBCR         (DA1470X_UART0_BASE + DA1470_UART_SBCR_OFFSET)
#define DA1470_UART0_SCR          (DA1470X_UART0_BASE + DA1470_UART_SCR_OFFSET)
#define DA1470_UART0_SDMAM        (DA1470X_UART0_BASE + DA1470_UART_SDMAM_OFFSET)
#define DA1470_UART0_SFE          (DA1470X_UART0_BASE + DA1470_UART_SFE_OFFSET)
#define DA1470_UART0_SRBR_STHR0   (DA1470X_UART0_BASE + DA1470_UART_SRBR_STHR0_OFFSET)
#define DA1470_UART0_SRBR_STHR10  (DA1470X_UART0_BASE + DA1470_UART_SRBR_STHR10_OFFSET)
#define DA1470_UART0_SRBR_STHR11  (DA1470X_UART0_BASE + DA1470_UART_SRBR_STHR11_OFFSET)
#define DA1470_UART0_SRBR_STHR12  (DA1470X_UART0_BASE + DA1470_UART_SRBR_STHR12_OFFSET)
#define DA1470_UART0_SRBR_STHR13  (DA1470X_UART0_BASE + DA1470_UART_SRBR_STHR13_OFFSET)
#define DA1470_UART0_SRBR_STHR14  (DA1470X_UART0_BASE + DA1470_UART_SRBR_STHR14_OFFSET)
#define DA1470_UART0_SRBR_STHR15  (DA1470X_UART0_BASE + DA1470_UART_SRBR_STHR15_OFFSET)
#define DA1470_UART0_SRBR_STHR1   (DA1470X_UART0_BASE + DA1470_UART_SRBR_STHR1_OFFSET)
#define DA1470_UART0_SRBR_STHR2   (DA1470X_UART0_BASE + DA1470_UART_SRBR_STHR2_OFFSET)
#define DA1470_UART0_SRBR_STHR3   (DA1470X_UART0_BASE + DA1470_UART_SRBR_STHR3_OFFSET)
#define DA1470_UART0_SRBR_STHR4   (DA1470X_UART0_BASE + DA1470_UART_SRBR_STHR4_OFFSET)
#define DA1470_UART0_SRBR_STHR5   (DA1470X_UART0_BASE + DA1470_UART_SRBR_STHR5_OFFSET)
#define DA1470_UART0_SRBR_STHR6   (DA1470X_UART0_BASE + DA1470_UART_SRBR_STHR6_OFFSET)
#define DA1470_UART0_SRBR_STHR7   (DA1470X_UART0_BASE + DA1470_UART_SRBR_STHR7_OFFSET)
#define DA1470_UART0_SRBR_STHR8   (DA1470X_UART0_BASE + DA1470_UART_SRBR_STHR8_OFFSET)
#define DA1470_UART0_SRBR_STHR9   (DA1470X_UART0_BASE + DA1470_UART_SRBR_STHR9_OFFSET)
#define DA1470_UART0_SRR          (DA1470X_UART0_BASE + DA1470_UART_SRR_OFFSET)
#define DA1470_UART0_SRT          (DA1470X_UART0_BASE + DA1470_UART_SRT_OFFSET)
#define DA1470_UART0_STET         (DA1470X_UART0_BASE + DA1470_UART_STET_OFFSET)
#define DA1470_UART0_TFL          (DA1470X_UART0_BASE + DA1470_UART_TFL_OFFSET)
#define DA1470_UART0_UCV          (DA1470X_UART0_BASE + DA1470_UART_UCV_OFFSET)
#define DA1470_UART0_USR          (DA1470X_UART0_BASE + DA1470_UART_USR_OFFSET)


#ifdef CONFIG_DA1470X_UART1
#  define DA1470_UART1_CTR          (DA1470X_UART1_BASE + DA1470_UART_CTR_OFFSET)
#  define DA1470_UART1_DLF          (DA1470X_UART1_BASE + DA1470_UART_DLF_OFFSET)
#  define DA1470_UART1_DMASA        (DA1470X_UART1_BASE + DA1470_UART_DMASA_OFFSET)
#  define DA1470_UART1_HTX          (DA1470X_UART1_BASE + DA1470_UART_HTX_OFFSET)
#  define DA1470_UART1_IER_DLH      (DA1470X_UART1_BASE + DA1470_UART_IER_DLH_OFFSET)
#  define DA1470_UART1_IIR_FCR      (DA1470X_UART1_BASE + DA1470_UART_IIR_FCR_OFFSET)
#  define DA1470_UART1_LCR          (DA1470X_UART1_BASE + DA1470_UART_LCR_OFFSET)
#  define DA1470_UART1_LSR          (DA1470X_UART1_BASE + DA1470_UART_LSR_OFFSET)
#  define DA1470_UART1_MCR          (DA1470X_UART1_BASE + DA1470_UART_MCR_OFFSET)
#  define DA1470_UART1_RBR_THR_DLL  (DA1470X_UART1_BASE + DA1470_UART_RBR_THR_DLL_OFFSET)
#  define DA1470_UART1_RFL          (DA1470X_UART1_BASE + DA1470_UART_RFL_OFFSET)
#  define DA1470_UART1_SBCR         (DA1470X_UART1_BASE + DA1470_UART_SBCR_OFFSET)
#  define DA1470_UART1_SCR          (DA1470X_UART1_BASE + DA1470_UART_SCR_OFFSET)
#  define DA1470_UART1_SDMAM        (DA1470X_UART1_BASE + DA1470_UART_SDMAM_OFFSET)
#  define DA1470_UART1_SFE          (DA1470X_UART1_BASE + DA1470_UART_SFE_OFFSET)
#  define DA1470_UART1_SRBR_STHR0   (DA1470X_UART1_BASE + DA1470_UART_SRBR_STHR0_OFFSET)
#  define DA1470_UART1_SRBR_STHR10  (DA1470X_UART1_BASE + DA1470_UART_SRBR_STHR10_OFFSET)
#  define DA1470_UART1_SRBR_STHR11  (DA1470X_UART1_BASE + DA1470_UART_SRBR_STHR11_OFFSET)
#  define DA1470_UART1_SRBR_STHR12  (DA1470X_UART1_BASE + DA1470_UART_SRBR_STHR12_OFFSET)
#  define DA1470_UART1_SRBR_STHR13  (DA1470X_UART1_BASE + DA1470_UART_SRBR_STHR13_OFFSET)
#  define DA1470_UART1_SRBR_STHR14  (DA1470X_UART1_BASE + DA1470_UART_SRBR_STHR14_OFFSET)
#  define DA1470_UART1_SRBR_STHR15  (DA1470X_UART1_BASE + DA1470_UART_SRBR_STHR15_OFFSET)
#  define DA1470_UART1_SRBR_STHR1   (DA1470X_UART1_BASE + DA1470_UART_SRBR_STHR1_OFFSET)
#  define DA1470_UART1_SRBR_STHR2   (DA1470X_UART1_BASE + DA1470_UART_SRBR_STHR2_OFFSET)
#  define DA1470_UART1_SRBR_STHR3   (DA1470X_UART1_BASE + DA1470_UART_SRBR_STHR3_OFFSET)
#  define DA1470_UART1_SRBR_STHR4   (DA1470X_UART1_BASE + DA1470_UART_SRBR_STHR4_OFFSET)
#  define DA1470_UART1_SRBR_STHR5   (DA1470X_UART1_BASE + DA1470_UART_SRBR_STHR5_OFFSET)
#  define DA1470_UART1_SRBR_STHR6   (DA1470X_UART1_BASE + DA1470_UART_SRBR_STHR6_OFFSET)
#  define DA1470_UART1_SRBR_STHR7   (DA1470X_UART1_BASE + DA1470_UART_SRBR_STHR7_OFFSET)
#  define DA1470_UART1_SRBR_STHR8   (DA1470X_UART1_BASE + DA1470_UART_SRBR_STHR8_OFFSET)
#  define DA1470_UART1_SRBR_STHR9   (DA1470X_UART1_BASE + DA1470_UART_SRBR_STHR9_OFFSET)
#  define DA1470_UART1_SRR          (DA1470X_UART1_BASE + DA1470_UART_SRR_OFFSET)
#  define DA1470_UART1_SRT          (DA1470X_UART1_BASE + DA1470_UART_SRT_OFFSET)
#  define DA1470_UART1_STET         (DA1470X_UART1_BASE + DA1470_UART_STET_OFFSET)
#  define DA1470_UART1_TFL          (DA1470X_UART1_BASE + DA1470_UART_TFL_OFFSET)
#  define DA1470_UART1_UCV          (DA1470X_UART1_BASE + DA1470_UART_UCV_OFFSET)
#  define DA1470_UART1_USR          (DA1470X_UART1_BASE + DA1470_UART_USR_OFFSET)

#elifdef CONFIG_DA1470X_UART2
#  define DA1470_UART2_CTR          (DA1470X_UART2_BASE + DA1470_UART_CTR_OFFSET)
#  define DA1470_UART2_DLF          (DA1470X_UART2_BASE + DA1470_UART_DLF_OFFSET)
#  define DA1470_UART2_DMASA        (DA1470X_UART2_BASE + DA1470_UART_DMASA_OFFSET)
#  define DA1470_UART2_HTX          (DA1470X_UART2_BASE + DA1470_UART_HTX_OFFSET)
#  define DA1470_UART2_IER_DLH      (DA1470X_UART2_BASE + DA1470_UART_IER_DLH_OFFSET)
#  define DA1470_UART2_IIR_FCR      (DA1470X_UART2_BASE + DA1470_UART_IIR_FCR_OFFSET)
#  define DA1470_UART2_LCR          (DA1470X_UART2_BASE + DA1470_UART_LCR_OFFSET)
#  define DA1470_UART2_LSR          (DA1470X_UART2_BASE + DA1470_UART_LSR_OFFSET)
#  define DA1470_UART2_MCR          (DA1470X_UART2_BASE + DA1470_UART_MCR_OFFSET)
#  define DA1470_UART2_RBR_THR_DLL  (DA1470X_UART2_BASE + DA1470_UART_RBR_THR_DLL_OFFSET)
#  define DA1470_UART2_RFL          (DA1470X_UART2_BASE + DA1470_UART_RFL_OFFSET)
#  define DA1470_UART2_SBCR         (DA1470X_UART2_BASE + DA1470_UART_SBCR_OFFSET)
#  define DA1470_UART2_SCR          (DA1470X_UART2_BASE + DA1470_UART_SCR_OFFSET)
#  define DA1470_UART2_SDMAM        (DA1470X_UART2_BASE + DA1470_UART_SDMAM_OFFSET)
#  define DA1470_UART2_SFE          (DA1470X_UART2_BASE + DA1470_UART_SFE_OFFSET)
#  define DA1470_UART2_SRBR_STHR0   (DA1470X_UART2_BASE + DA1470_UART_SRBR_STHR0_OFFSET)
#  define DA1470_UART2_SRBR_STHR10  (DA1470X_UART2_BASE + DA1470_UART_SRBR_STHR10_OFFSET)
#  define DA1470_UART2_SRBR_STHR11  (DA1470X_UART2_BASE + DA1470_UART_SRBR_STHR11_OFFSET)
#  define DA1470_UART2_SRBR_STHR12  (DA1470X_UART2_BASE + DA1470_UART_SRBR_STHR12_OFFSET)
#  define DA1470_UART2_SRBR_STHR13  (DA1470X_UART2_BASE + DA1470_UART_SRBR_STHR13_OFFSET)
#  define DA1470_UART2_SRBR_STHR14  (DA1470X_UART2_BASE + DA1470_UART_SRBR_STHR14_OFFSET)
#  define DA1470_UART2_SRBR_STHR15  (DA1470X_UART2_BASE + DA1470_UART_SRBR_STHR15_OFFSET)
#  define DA1470_UART2_SRBR_STHR1   (DA1470X_UART2_BASE + DA1470_UART_SRBR_STHR1_OFFSET)
#  define DA1470_UART2_SRBR_STHR2   (DA1470X_UART2_BASE + DA1470_UART_SRBR_STHR2_OFFSET)
#  define DA1470_UART2_SRBR_STHR3   (DA1470X_UART2_BASE + DA1470_UART_SRBR_STHR3_OFFSET)
#  define DA1470_UART2_SRBR_STHR4   (DA1470X_UART2_BASE + DA1470_UART_SRBR_STHR4_OFFSET)
#  define DA1470_UART2_SRBR_STHR5   (DA1470X_UART2_BASE + DA1470_UART_SRBR_STHR5_OFFSET)
#  define DA1470_UART2_SRBR_STHR6   (DA1470X_UART2_BASE + DA1470_UART_SRBR_STHR6_OFFSET)
#  define DA1470_UART2_SRBR_STHR7   (DA1470X_UART2_BASE + DA1470_UART_SRBR_STHR7_OFFSET)
#  define DA1470_UART2_SRBR_STHR8   (DA1470X_UART2_BASE + DA1470_UART_SRBR_STHR8_OFFSET)
#  define DA1470_UART2_SRBR_STHR9   (DA1470X_UART2_BASE + DA1470_UART_SRBR_STHR9_OFFSET)
#  define DA1470_UART2_SRR          (DA1470X_UART2_BASE + DA1470_UART_SRR_OFFSET)
#  define DA1470_UART2_SRT          (DA1470X_UART2_BASE + DA1470_UART_SRT_OFFSET)
#  define DA1470_UART2_STET         (DA1470X_UART2_BASE + DA1470_UART_STET_OFFSET)
#  define DA1470_UART2_TFL          (DA1470X_UART2_BASE + DA1470_UART_TFL_OFFSET)
#  define DA1470_UART2_UCV          (DA1470X_UART2_BASE + DA1470_UART_UCV_OFFSET)
#  define DA1470_UART2_USR          (DA1470X_UART2_BASE + DA1470_UART_USR_OFFSET)


#endif

/* UART Register Bitfield Definitions **************************************/

/****************************************************************************
 * Name: Baud rate dividers
 *
 * Description:
 *   The defined values represent the contents of three registers: DLH, DLL, and DLF.
 *   The encoding for each register is as follows:
 *
 *   +--------+--------+--------+--------+
 *   | unused |   DLH  |   DLL  |   DLF  |
 *   +--------+--------+--------+--------+
 *
 * Notes:
 *   - Baud rates higher than 2000000 require PLL160M or RCHS running at 96 MHz as
 *     the system clock. Using other clock configurations for such baud rates will
 *     trigger a warning or assertion.
 *
 *   - For high baud rates (greater than 2000000), the enumerated values do not
 *     directly correspond to the values applied to the divider registers (DLH, DLL,
 *     DLF). Instead, they are internally mapped to different divider values based on
 *     the active high-speed clock.
 *
 ****************************************************************************/

/* BAUDRATE Register */

//TODO
#if MAIN_PROCESSOR_BUILD
#  define UART_BAUDRATE_6000000               (0x00000005) /**< Requires either PLL160M or RCHS@96MHz! */
#endif
#define UART_BAUDRATE_3000000               (0x0000000B) /**< Requires either PLL160M or RCHS@96MHz! */
#define UART_BAUDRATE_2000000               (0x00000100)
#define UART_BAUDRATE_1000000               (0x00000200)
#define UART_BAUDRATE_921600                (0x00000203)
#define UART_BAUDRATE_500000                (0x00000400)
#define UART_BAUDRATE_460800                (0x00000405)
#define UART_BAUDRATE_256000                (0x0000070D)
#define UART_BAUDRATE_230400                (0x0000080b)
#define UART_BAUDRATE_115200                (0x00001106)
#define UART_BAUDRATE_57600                 (0x0000220c)
#define UART_BAUDRATE_38400                 (0x00003401)
#define UART_BAUDRATE_28800                 (0x00004507)
#define UART_BAUDRATE_19200                 (0x00006803)
#define UART_BAUDRATE_14400                 (0x00008a0e)
#define UART_BAUDRATE_9600                  (0x0000d005)
#define UART_BAUDRATE_4800                  (0x0001a00b)
#define UART_BAUDRATE_2400                  (0x00034105)
#define UART_BAUDRATE_1200                  (0x0006820B)

/* Character format */
#define UART_DATABITS_5                    (0) /**< 5 data bits */
#define UART_DATABITS_6                    (1) /**< 6 data bits */
#define UART_DATABITS_7                    (2) /**< 7 data bits */
#define UART_DATABITS_8                    (3) /**< 8 data bits */

/* Parity */
#define UART_PARITY_NONE                   (0) /**< No parity */
#define UART_PARITY_ODD                    (1) /**< Odd parity */
#define UART_PARITY_EVEN                   (3) /**< Even parity */

/* Stop bits */
#define UART_STOPBITS_1                    (0) /**< 1 stop bit */
#define UART_STOPBITS_2                    (1) /**< 2 stop bits */

/* Uart errors */
#define UART_ERR_NOERROR                   (0) /**< No error */
#define UART_ERR_OE                        (1) /**< Overrun error */
#define UART_ERR_PE                        (2) /**< Parity error */
#define UART_ERR_FE                        (3) /**< Framing error */
#define UART_ERR_BI                        (4) /**< Break interrupt indication */
#define UART_ERR_RFE                       (5) /**< Receive FIFO error */

/* Configuration errors */
#define UART_CONFIG_ERR_NOERR              (0) /**< No error */
#define UART_CONFIG_ERR_RX_FIFO            (1) /**< RX FIFO level different than RX burst mode */
#define UART_CONFIG_ERR_TX_FIFO            (2) /**< TX FIFO level different than TX burst mode */
#define UART_CONFIG_ERR_RXTX_FIFO          (3) /**< RX and TX FIFO level different than TX RX burst mode */
#define UART_CONFIG_ERR_RX_SIZE            (4) /**< Received buffer size is not aligned with burst mode */
#define UART_CONFIG_ERR_TX_SIZE            (5) /**< Transmitted buffer size is not aligned with burst mode */

/* UART Register Bitfield Definitions */
/* UART_CTR_REG */
#define UART_CTR_UART_CTR              (0xFFFFFFFF << 0)  /* UART_CTR (Bit 0) */

/* UART_DLF_REG */
#define UART_DLF_UART_DLF              (0xF << 0)  /* UART_DLF (Bit 0) */

/* UART_DMASA_REG */
#define UART_DMASA_UART_DMASA          (1 << 0)  /* UART_DMASA (Bit 0) */

/* UART_HTX_REG */
#define UART_HTX_UART_HALT_TX          (1 << 0)  /* UART_HALT_TX (Bit 0) */

/* UART_IER_DLH_REG */
#define UART_IER_DLH_PTIME_DLH7        (1 << 7)  /* PTIME_DLH7 (Bit 7) */
#define UART_IER_DLH_DLH6_5            (0x3 << 5)  /* DLH6_5 (Bit 5) */
#define UART_IER_DLH_ELCOLR_DLH4       (1 << 4)  /* ELCOLR_DLH4 (Bit 4) */
#define UART_IER_DLH_EDSSI_DLH3        (1 << 3)  /* EDSSI_DLH3 (Bit 3) */
#define UART_IER_DLH_ELSI_DLH2         (1 << 2)  /* ELSI_DLH2 (Bit 2) */
#define UART_IER_DLH_ETBEI_DLH1        (1 << 1)  /* ETBEI_DLH1 (Bit 1) */
#define UART_IER_DLH_ERBFI_DLH0        (1 << 0)  /* ERBFI_DLH0 (Bit 0) */

/* UART_IIR_FCR_REG */
#define UART_IIR_FCR                   (0xFF << 0)  /* IIR_FCR (Bit 0) */

/* UART_LCR_REG */
#define UART_LCR_UART_DLAB             (1 << 7)  /* UART_DLAB (Bit 7) */
#define UART_LCR_UART_BC               (1 << 6)  /* UART_BC (Bit 6) */
#define UART_LCR_UART_EPS              (1 << 4)  /* UART_EPS (Bit 4) */
#define UART_LCR_UART_PEN              (1 << 3)  /* UART_PEN (Bit 3) */
#define UART_LCR_UART_STOP             (1 << 2)  /* UART_STOP (Bit 2) */
#define UART_LCR_UART_DLS              (0x3 << 0)  /* UART_DLS (Bit 0) */
       /* UART_DLS Mask: 0x03 */

/* UART_LSR_REG */
#define UART_LSR_UART_RFE            (1 << 7)  /* UART_RFE (Bit 7) */
#define UART_LSR_UART_TEMT           (1 << 6)  /* UART_TEMT (Bit 6) */
#define UART_LSR_UART_THRE           (1 << 5)  /* UART_THRE (Bit 5) */
#define UART_LSR_UART_BI             (1 << 4)  /* UART_BI (Bit 4) */
#define UART_LSR_UART_FE             (1 << 3)  /* UART_FE (Bit 3) */
#define UART_LSR_UART_PE             (1 << 2)  /* UART_PE (Bit 2) */
#define UART_LSR_UART_OE             (1 << 1)  /* UART_OE (Bit 1) */
#define UART_LSR_UART_DR             (1 << 0)  /* UART_DR (Bit 0) */

/* UART_DR Mask: 0x01 */
/* UART_MCR_REG */
#define UART_MCR_UART_LB              (1 << 4)  /* UART_LB (Bit 4) */

/* UART_RBR_THR_DLL_REG */
#define UART_RBR_THR_DLL              (0xFF << 0)  /* RBR_THR_DLL (Bit 0) */

/* UART_RFL_REG */
#define UART_RFL_UART_RECEIVE_FIFO_LEVEL (0x1F << 0)  /* UART_RECEIVE_FIFO_LEVEL (Bit 0) */

/* UART_SBCR_REG */
#define UART_SBCR_UART_SHADOW_BREAK_CONTROL (1 << 0)  /* UART_SHADOW_BREAK_CONTROL (Bit 0) */

/* UART_SCR_REG */
#define UART_SCR_UART_SCRATCH_PAD     (0xFF << 0)  /* UART_SCRATCH_PAD (Bit 0) */

/* UART_SDMAM_REG */
#define UART_SDMAM_UART_SHADOW_DMA_MODE (1 << 0)  /* UART_SHADOW_DMA_MODE (Bit 0) */

/* UART_SFE_REG */
#define UART_SFE_UART_SHADOW_FIFO_ENABLE (1 << 0)  /* UART_SHADOW_FIFO_ENABLE (Bit 0) */

/* UART_SRBR_STHR0_REG */
#define UART_SRBR_STHR0               (0xFF << 0)  /* SRBR_STHRx (Bit 0) */

/* UART_SRBR_STHR10_REG */
#define UART_SRBR_STHR10              (0xFF << 0)  /* SRBR_STHRx (Bit 0) */

/* UART_SRBR_STHR11_REG */
#define UART_SRBR_STHR11              (0xFF << 0)  /* SRBR_STHRx (Bit 0) */

/* UART_SRBR_STHR12_REG */
#define UART_SRBR_STHR12              (0xFF << 0)  /* SRBR_STHRx (Bit 0) */

/* UART_SRBR_STHR13_REG */
#define UART_SRBR_STHR13              (0xFF << 0)  /* SRBR_STHRx (Bit 0) */

/* UART_SRBR_STHR14_REG */
#define UART_SRBR_STHR14              (0xFF << 0)  /* SRBR_STHRx (Bit 0) */

/* UART_SRBR_STHR15_REG */
#define UART_SRBR_STHR15              (0xFF << 0)  /* SRBR_STHRx (Bit 0) */

/* UART_SRBR_STHR1_REG */
#define UART_SRBR_STHR1               (0xFF << 0)  /* SRBR_STHRx (Bit 0) */

/* UART_SRBR_STHR2_REG */
#define UART_SRBR_STHR2               (0xFF << 0)  /* SRBR_STHRx (Bit 0) */

/* UART_SRBR_STHR3_REG */
#define UART_SRBR_STHR3               (0xFF << 0)  /* SRBR_STHRx (Bit 0) */

/* UART_SRBR_STHR4_REG */
#define UART_SRBR_STHR4               (0xFF << 0)  /* SRBR_STHRx (Bit 0) */

/* UART_SRBR_STHR5_REG */
#define UART_SRBR_STHR5               (0xFF << 0)  /* SRBR_STHRx (Bit 0) */

/* UART_SRBR_STHR6_REG */
#define UART_SRBR_STHR6               (0xFF << 0)  /* SRBR_STHRx (Bit 0) */

/* UART_SRBR_STHR7_REG */
#define UART_SRBR_STHR7               (0xFF << 0)  /* SRBR_STHRx (Bit 0) */

/* UART_SRBR_STHR8_REG */
#define UART_SRBR_STHR8               (0xFF << 0)  /* SRBR_STHRx (Bit 0) */

/* UART_SRBR_STHR9_REG */
#define UART_SRBR_STHR9               (0xFF << 0)  /* SRBR_STHRx (Bit 0) */

/* UART_SRR_REG */
#define UART_SRR_UART_XFR             (1 << 2)  /* UART_XFR (Bit 2) */
#define UART_SRR_UART_RFR             (1 << 1)  /* UART_RFR (Bit 1) */
#define UART_SRR_UART_UR              (1 << 0)  /* UART_UR (Bit 0) */

/* UART_SRT_REG */
#define UART_SRT_UART_SHADOW_RCVR_TRIGGER (0x3 << 0)  /* UART_SHADOW_RCVR_TRIGGER (Bit 0) */

/* UART_STET_REG */
#define UART_STET_UART_SHADOW_TX_EMPTY_TRIGGER (0x3 << 0)  /* UART_SHADOW_TX_EMPTY_TRIGGER (Bit 0) */

/* UART_TFL_REG */
#define UART_TFL_UART_TRANSMIT_FIFO_LEVEL (0x1F << 0)  /* UART_TRANSMIT_FIFO_LEVEL (Bit 0) */

/* UART_UCV_REG */
#define UART_UCV_UART_UCV             (0xFFFFFFFF << 0)  /* UART_UCV (Bit 0) */

/* UART_USR_REG */
#define UART_USR_UART_RFF             (1 << 4)  /* UART_RFF (Bit 4) */
#define UART_USR_UART_RFNE            (1 << 3)  /* UART_RFNE (Bit 3) */
#define UART_USR_UART_TFE             (1 << 2)  /* UART_TFE (Bit 2) */
#define UART_USR_UART_TFNF            (1 << 1)  /* UART_TFNF (Bit 1) */
#define UART_USR_UART_BUSY            (1 << 0)  /* UART_BUSY (Bit 0) */

#define UART_MCR_AFCE                 (1 << 5)  /* Auto Flow Control Enable (AFCE) bit */
#define UART_MCR_RTS                  (1 << 1)  /* Request To Send (RTS) control bit */

/*!< UART_BUSY Mask: 0x01 */
#endif