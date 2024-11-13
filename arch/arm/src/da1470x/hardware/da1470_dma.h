/****************************************************************************
 * ./tmp/da1470_dma.h
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

#ifndef __DA1470_DMA_H
#define __DA1470_DMA_H

/* Register offsets *********************************************************/

#define DA1470_DMA_DMA0_A_START_OFFSET                0x0000 /* Source address register of DMA channel 0 */
#define DA1470_DMA_DMA0_B_START_OFFSET                0x0004 /* Destination address register of DMA channel 0 */
#define DA1470_DMA_DMA0_CTRL_OFFSET                   0x0010 /* Control register of DMA channel 0 */
#define DA1470_DMA_DMA0_IDX_OFFSET                    0x0014 /* Index pointer register of DMA channel 0 */
#define DA1470_DMA_DMA0_INT_OFFSET                    0x0008 /* Interrupt length register of DMA channel 0 */
#define DA1470_DMA_DMA0_LEN_OFFSET                    0x000C /* Transfer length register of DMA channel 0 */
#define DA1470_DMA_DMA1_A_START_OFFSET                0x0020 /* Source address register of DMA channel 1 */
#define DA1470_DMA_DMA1_B_START_OFFSET                0x0024 /* Destination address register of DMA channel 1 */
#define DA1470_DMA_DMA1_CTRL_OFFSET                   0x0030 /* Control register of DMA channel 1 */
#define DA1470_DMA_DMA1_IDX_OFFSET                    0x0034 /* Index pointer register of DMA channel 1 */
#define DA1470_DMA_DMA1_INT_OFFSET                    0x0028 /* Interrupt length register of DMA channel 1 */
#define DA1470_DMA_DMA1_LEN_OFFSET                    0x002C /* Transfer length register of DMA channel 1 */
#define DA1470_DMA_DMA2_A_START_OFFSET                0x0040 /* Source address register of DMA channel 2 */
#define DA1470_DMA_DMA2_B_START_OFFSET                0x0044 /* Destination address register of DMA channel 2 */
#define DA1470_DMA_DMA2_CTRL_OFFSET                   0x0050 /* Control register of DMA channel 2 */
#define DA1470_DMA_DMA2_IDX_OFFSET                    0x0054 /* Index pointer register of DMA channel 2 */
#define DA1470_DMA_DMA2_INT_OFFSET                    0x0048 /* Interrupt length register of DMA channel 2 */
#define DA1470_DMA_DMA2_LEN_OFFSET                    0x004C /* Transfer length register of DMA channel 2 */
#define DA1470_DMA_DMA3_A_START_OFFSET                0x0060 /* Source address register of DMA channel 3 */
#define DA1470_DMA_DMA3_B_START_OFFSET                0x0064 /* Destination address register of DMA channel 3 */
#define DA1470_DMA_DMA3_CTRL_OFFSET                   0x0070 /* Control register of DMA channel 3 */
#define DA1470_DMA_DMA3_IDX_OFFSET                    0x0074 /* Index pointer register of DMA channel 3 */
#define DA1470_DMA_DMA3_INT_OFFSET                    0x0068 /* Interrupt length register of DMA channel 3 */
#define DA1470_DMA_DMA3_LEN_OFFSET                    0x006C /* Transfer length register of DMA channel 3 */
#define DA1470_DMA_DMA4_A_START_OFFSET                0x0080 /* Source address register of DMA channel 4 */
#define DA1470_DMA_DMA4_B_START_OFFSET                0x0084 /* Destination address register of DMA channel 4 */
#define DA1470_DMA_DMA4_CTRL_OFFSET                   0x0090 /* Control register of DMA channel 4 */
#define DA1470_DMA_DMA4_IDX_OFFSET                    0x0094 /* Index pointer register of DMA channel 4 */
#define DA1470_DMA_DMA4_INT_OFFSET                    0x0088 /* Interrupt length register of DMA channel 4 */
#define DA1470_DMA_DMA4_LEN_OFFSET                    0x008C /* Transfer length register of DMA channel 4 */
#define DA1470_DMA_DMA5_A_START_OFFSET                0x00A0 /* Source address register of DMA channel 5 */
#define DA1470_DMA_DMA5_B_START_OFFSET                0x00A4 /* Destination address register of DMA channel 5 */
#define DA1470_DMA_DMA5_CTRL_OFFSET                   0x00B0 /* Control register of DMA channel 5 */
#define DA1470_DMA_DMA5_IDX_OFFSET                    0x00B4 /* Index pointer register of DMA channel 5 */
#define DA1470_DMA_DMA5_INT_OFFSET                    0x00A8 /* Interrupt length register of DMA channel 5 */
#define DA1470_DMA_DMA5_LEN_OFFSET                    0x00AC /* Transfer length register of DMA channel 5 */
#define DA1470_DMA_DMA6_A_START_OFFSET                0x00C0 /* Source address register of DMA channel 6 */
#define DA1470_DMA_DMA6_B_START_OFFSET                0x00C4 /* Destination address register of DMA channel 6 */
#define DA1470_DMA_DMA6_CTRL_OFFSET                   0x00D0 /* Control register of DMA channel 6 */
#define DA1470_DMA_DMA6_IDX_OFFSET                    0x00D4 /* Index pointer register of DMA channel 6 */
#define DA1470_DMA_DMA6_INT_OFFSET                    0x00C8 /* Interrupt length register of DMA channel 6 */
#define DA1470_DMA_DMA6_LEN_OFFSET                    0x00CC /* Transfer length register of DMA channel 6 */
#define DA1470_DMA_DMA7_A_START_OFFSET                0x00E0 /* Source address register of DMA channel 7 */
#define DA1470_DMA_DMA7_B_START_OFFSET                0x00E4 /* Destination address register of DMA channel 7 */
#define DA1470_DMA_DMA7_CTRL_OFFSET                   0x00F0 /* Control register of DMA channel 7 */
#define DA1470_DMA_DMA7_IDX_OFFSET                    0x00F4 /* Index pointer register of DMA channel 7 */
#define DA1470_DMA_DMA7_INT_OFFSET                    0x00E8 /* Interrupt length register of DMA channel 7 */
#define DA1470_DMA_DMA7_LEN_OFFSET                    0x00EC /* Transfer length register of DMA channel 7 */
#define DA1470_DMA_DMA_CLEAR_INT_OFFSET               0x0108 /* DMA Interrupt clear register */
#define DA1470_DMA_DMA_INT_MASK_OFFSET                0x010C /* DMA Interrupt mask register */
#define DA1470_DMA_DMA_INT_STATUS_OFFSET              0x0104 /* DMA Interrupt status register */
#define DA1470_DMA_DMA_REQ_MUX_OFFSET                 0x0100 /* DMA channels peripherals mapping register */
#define DA1470_DMA_DMA_RESET_INT_MASK_OFFSET          0x0114 /* DMA Reset Interrupt mask register */
#define DA1470_DMA_DMA_SET_INT_MASK_OFFSET            0x0110 /* DMA Set Interrupt mask register */

/* Register addresses *******************************************************/

#define DA1470_DMA_BASE                               0x51000400 /* DMA registers */

#define DA1470_DMA_DMA0_A_START                       (DA1470_DMA_BASE + DA1470_DMA_DMA0_A_START_OFFSET) /* Source address register of DMA channel 0 */
#define DA1470_DMA_DMA0_B_START                       (DA1470_DMA_BASE + DA1470_DMA_DMA0_B_START_OFFSET) /* Destination address register of DMA channel 0 */
#define DA1470_DMA_DMA0_CTRL                          (DA1470_DMA_BASE + DA1470_DMA_DMA0_CTRL_OFFSET) /* Control register of DMA channel 0 */
#define DA1470_DMA_DMA0_IDX                           (DA1470_DMA_BASE + DA1470_DMA_DMA0_IDX_OFFSET) /* Index pointer register of DMA channel 0 */
#define DA1470_DMA_DMA0_INT                           (DA1470_DMA_BASE + DA1470_DMA_DMA0_INT_OFFSET) /* Interrupt length register of DMA channel 0 */
#define DA1470_DMA_DMA0_LEN                           (DA1470_DMA_BASE + DA1470_DMA_DMA0_LEN_OFFSET) /* Transfer length register of DMA channel 0 */
#define DA1470_DMA_DMA1_A_START                       (DA1470_DMA_BASE + DA1470_DMA_DMA1_A_START_OFFSET) /* Source address register of DMA channel 1 */
#define DA1470_DMA_DMA1_B_START                       (DA1470_DMA_BASE + DA1470_DMA_DMA1_B_START_OFFSET) /* Destination address register of DMA channel 1 */
#define DA1470_DMA_DMA1_CTRL                          (DA1470_DMA_BASE + DA1470_DMA_DMA1_CTRL_OFFSET) /* Control register of DMA channel 1 */
#define DA1470_DMA_DMA1_IDX                           (DA1470_DMA_BASE + DA1470_DMA_DMA1_IDX_OFFSET) /* Index pointer register of DMA channel 1 */
#define DA1470_DMA_DMA1_INT                           (DA1470_DMA_BASE + DA1470_DMA_DMA1_INT_OFFSET) /* Interrupt length register of DMA channel 1 */
#define DA1470_DMA_DMA1_LEN                           (DA1470_DMA_BASE + DA1470_DMA_DMA1_LEN_OFFSET) /* Transfer length register of DMA channel 1 */
#define DA1470_DMA_DMA2_A_START                       (DA1470_DMA_BASE + DA1470_DMA_DMA2_A_START_OFFSET) /* Source address register of DMA channel 2 */
#define DA1470_DMA_DMA2_B_START                       (DA1470_DMA_BASE + DA1470_DMA_DMA2_B_START_OFFSET) /* Destination address register of DMA channel 2 */
#define DA1470_DMA_DMA2_CTRL                          (DA1470_DMA_BASE + DA1470_DMA_DMA2_CTRL_OFFSET) /* Control register of DMA channel 2 */
#define DA1470_DMA_DMA2_IDX                           (DA1470_DMA_BASE + DA1470_DMA_DMA2_IDX_OFFSET) /* Index pointer register of DMA channel 2 */
#define DA1470_DMA_DMA2_INT                           (DA1470_DMA_BASE + DA1470_DMA_DMA2_INT_OFFSET) /* Interrupt length register of DMA channel 2 */
#define DA1470_DMA_DMA2_LEN                           (DA1470_DMA_BASE + DA1470_DMA_DMA2_LEN_OFFSET) /* Transfer length register of DMA channel 2 */
#define DA1470_DMA_DMA3_A_START                       (DA1470_DMA_BASE + DA1470_DMA_DMA3_A_START_OFFSET) /* Source address register of DMA channel 3 */
#define DA1470_DMA_DMA3_B_START                       (DA1470_DMA_BASE + DA1470_DMA_DMA3_B_START_OFFSET) /* Destination address register of DMA channel 3 */
#define DA1470_DMA_DMA3_CTRL                          (DA1470_DMA_BASE + DA1470_DMA_DMA3_CTRL_OFFSET) /* Control register of DMA channel 3 */
#define DA1470_DMA_DMA3_IDX                           (DA1470_DMA_BASE + DA1470_DMA_DMA3_IDX_OFFSET) /* Index pointer register of DMA channel 3 */
#define DA1470_DMA_DMA3_INT                           (DA1470_DMA_BASE + DA1470_DMA_DMA3_INT_OFFSET) /* Interrupt length register of DMA channel 3 */
#define DA1470_DMA_DMA3_LEN                           (DA1470_DMA_BASE + DA1470_DMA_DMA3_LEN_OFFSET) /* Transfer length register of DMA channel 3 */
#define DA1470_DMA_DMA4_A_START                       (DA1470_DMA_BASE + DA1470_DMA_DMA4_A_START_OFFSET) /* Source address register of DMA channel 4 */
#define DA1470_DMA_DMA4_B_START                       (DA1470_DMA_BASE + DA1470_DMA_DMA4_B_START_OFFSET) /* Destination address register of DMA channel 4 */
#define DA1470_DMA_DMA4_CTRL                          (DA1470_DMA_BASE + DA1470_DMA_DMA4_CTRL_OFFSET) /* Control register of DMA channel 4 */
#define DA1470_DMA_DMA4_IDX                           (DA1470_DMA_BASE + DA1470_DMA_DMA4_IDX_OFFSET) /* Index pointer register of DMA channel 4 */
#define DA1470_DMA_DMA4_INT                           (DA1470_DMA_BASE + DA1470_DMA_DMA4_INT_OFFSET) /* Interrupt length register of DMA channel 4 */
#define DA1470_DMA_DMA4_LEN                           (DA1470_DMA_BASE + DA1470_DMA_DMA4_LEN_OFFSET) /* Transfer length register of DMA channel 4 */
#define DA1470_DMA_DMA5_A_START                       (DA1470_DMA_BASE + DA1470_DMA_DMA5_A_START_OFFSET) /* Source address register of DMA channel 5 */
#define DA1470_DMA_DMA5_B_START                       (DA1470_DMA_BASE + DA1470_DMA_DMA5_B_START_OFFSET) /* Destination address register of DMA channel 5 */
#define DA1470_DMA_DMA5_CTRL                          (DA1470_DMA_BASE + DA1470_DMA_DMA5_CTRL_OFFSET) /* Control register of DMA channel 5 */
#define DA1470_DMA_DMA5_IDX                           (DA1470_DMA_BASE + DA1470_DMA_DMA5_IDX_OFFSET) /* Index pointer register of DMA channel 5 */
#define DA1470_DMA_DMA5_INT                           (DA1470_DMA_BASE + DA1470_DMA_DMA5_INT_OFFSET) /* Interrupt length register of DMA channel 5 */
#define DA1470_DMA_DMA5_LEN                           (DA1470_DMA_BASE + DA1470_DMA_DMA5_LEN_OFFSET) /* Transfer length register of DMA channel 5 */
#define DA1470_DMA_DMA6_A_START                       (DA1470_DMA_BASE + DA1470_DMA_DMA6_A_START_OFFSET) /* Source address register of DMA channel 6 */
#define DA1470_DMA_DMA6_B_START                       (DA1470_DMA_BASE + DA1470_DMA_DMA6_B_START_OFFSET) /* Destination address register of DMA channel 6 */
#define DA1470_DMA_DMA6_CTRL                          (DA1470_DMA_BASE + DA1470_DMA_DMA6_CTRL_OFFSET) /* Control register of DMA channel 6 */
#define DA1470_DMA_DMA6_IDX                           (DA1470_DMA_BASE + DA1470_DMA_DMA6_IDX_OFFSET) /* Index pointer register of DMA channel 6 */
#define DA1470_DMA_DMA6_INT                           (DA1470_DMA_BASE + DA1470_DMA_DMA6_INT_OFFSET) /* Interrupt length register of DMA channel 6 */
#define DA1470_DMA_DMA6_LEN                           (DA1470_DMA_BASE + DA1470_DMA_DMA6_LEN_OFFSET) /* Transfer length register of DMA channel 6 */
#define DA1470_DMA_DMA7_A_START                       (DA1470_DMA_BASE + DA1470_DMA_DMA7_A_START_OFFSET) /* Source address register of DMA channel 7 */
#define DA1470_DMA_DMA7_B_START                       (DA1470_DMA_BASE + DA1470_DMA_DMA7_B_START_OFFSET) /* Destination address register of DMA channel 7 */
#define DA1470_DMA_DMA7_CTRL                          (DA1470_DMA_BASE + DA1470_DMA_DMA7_CTRL_OFFSET) /* Control register of DMA channel 7 */
#define DA1470_DMA_DMA7_IDX                           (DA1470_DMA_BASE + DA1470_DMA_DMA7_IDX_OFFSET) /* Index pointer register of DMA channel 7 */
#define DA1470_DMA_DMA7_INT                           (DA1470_DMA_BASE + DA1470_DMA_DMA7_INT_OFFSET) /* Interrupt length register of DMA channel 7 */
#define DA1470_DMA_DMA7_LEN                           (DA1470_DMA_BASE + DA1470_DMA_DMA7_LEN_OFFSET) /* Transfer length register of DMA channel 7 */
#define DA1470_DMA_DMA_CLEAR_INT                      (DA1470_DMA_BASE + DA1470_DMA_DMA_CLEAR_INT_OFFSET) /* DMA Interrupt clear register */
#define DA1470_DMA_DMA_INT_MASK                       (DA1470_DMA_BASE + DA1470_DMA_DMA_INT_MASK_OFFSET) /* DMA Interrupt mask register */
#define DA1470_DMA_DMA_INT_STATUS                     (DA1470_DMA_BASE + DA1470_DMA_DMA_INT_STATUS_OFFSET) /* DMA Interrupt status register */
#define DA1470_DMA_DMA_REQ_MUX                        (DA1470_DMA_BASE + DA1470_DMA_DMA_REQ_MUX_OFFSET) /* DMA channels peripherals mapping register */
#define DA1470_DMA_DMA_RESET_INT_MASK                 (DA1470_DMA_BASE + DA1470_DMA_DMA_RESET_INT_MASK_OFFSET) /* DMA Reset Interrupt mask register */
#define DA1470_DMA_DMA_SET_INT_MASK                   (DA1470_DMA_BASE + DA1470_DMA_DMA_SET_INT_MASK_OFFSET) /* DMA Set Interrupt mask register */

/* Register bit definitions *************************************************/

#define DMA_DMA0_A_START_DMA0_A_START_MASK            (0xFFFFFFFF << 0) /* Source start address of channel 0 */
#define DMA_DMA0_A_START_DMA0_A_START_POS             (0) /* Source start address of channel 0 */
#define DMA_DMA0_B_START_DMA0_B_START_MASK            (0xFFFFFFFF << 0) /* Destination start address of channel 0 */
#define DMA_DMA0_B_START_DMA0_B_START_POS             (0) /* Destination start address of channel 0 */
#define DMA_DMA0_CTRL_DMA_EXCLUSIVE_ACCESS            (1U << 16) /* 0: DMA channel de-asserts the bus request upon completion of the write transfer  */
#define DMA_DMA0_CTRL_BUS_ERROR_DETECT                (1U << 15) /* 0: Ignores bus error response from the AHB bus, so DMA continues normally. 1: De */
#define DMA_DMA0_CTRL_BURST_MODE_MASK                 (0x3 << 13) /* Enables the DMA read/write bursts, according to the following configuration: 00  */
#define DMA_DMA0_CTRL_BURST_MODE_POS                  (13) /* Enables the DMA read/write bursts, according to the following configuration: 00  */
#define DMA_DMA0_CTRL_REQ_SENSE                       (1U << 12) /* 0: DMA operates with level-sensitive peripheral requests (default) 1: DMA operat */
#define DMA_DMA0_CTRL_DMA_INIT                        (1U << 11) /* 0: DMA performs copy A1 to B1, A2 to B2, etc ... 1: DMA performs copy of A1 to B */
#define DMA_DMA0_CTRL_DMA_IDLE                        (1U << 10) /* 0: Blocking mode, the DMA performs a fast back-to-back copy, disabling bus acces */
#define DMA_DMA0_CTRL_DMA_PRIO_MASK                   (0x7 << 7) /* The priority level determines which DMA channel will be granted access for trans */
#define DMA_DMA0_CTRL_DMA_PRIO_POS                    (7) /* The priority level determines which DMA channel will be granted access for trans */
#define DMA_DMA0_CTRL_CIRCULAR                        (1U << 6) /* 0: Normal mode. The DMA channel stops after having completed the transfer of len */
#define DMA_DMA0_CTRL_AINC                            (1U << 5) /* Enable increment of source address. 0 = do not increment (source address stays t */
#define DMA_DMA0_CTRL_BINC                            (1U << 4) /* Enable increment of destination address. 0 = do not increment (destination addre */
#define DMA_DMA0_CTRL_DREQ_MODE                       (1U << 3) /* 0: DMA channel starts immediately 1: DMA channel must be triggered by peripheral */
#define DMA_DMA0_CTRL_BW_MASK                         (0x3 << 1) /* Bus transfer width: 00 = 1 Byte (suggested for peripherals like UART and 8-bit S */
#define DMA_DMA0_CTRL_BW_POS                          (1) /* Bus transfer width: 00 = 1 Byte (suggested for peripherals like UART and 8-bit S */
#define DMA_DMA0_CTRL_DMA_ON                          (1U << 0) /* 0: DMA channel is off, clocks are disabled 1: DMA channel is enabled. This bit w */
#define DMA_DMA0_IDX_DMA0_IDX_MASK                    (0xFFFF << 0) /* This (read-only) register determines the data items already transferred by the D */
#define DMA_DMA0_IDX_DMA0_IDX_POS                     (0) /* This (read-only) register determines the data items already transferred by the D */
#define DMA_DMA0_INT_DMA0_INT_MASK                    (0xFFFF << 0) /* Number of transfers until an interrupt is generated. The interrupt is generated  */
#define DMA_DMA0_INT_DMA0_INT_POS                     (0) /* Number of transfers until an interrupt is generated. The interrupt is generated  */
#define DMA_DMA0_LEN_DMA0_LEN_MASK                    (0xFFFF << 0) /* DMA channel's transfer length. DMA0_LEN of value 0, 1, 2, ... results into an ac */
#define DMA_DMA0_LEN_DMA0_LEN_POS                     (0) /* DMA channel's transfer length. DMA0_LEN of value 0, 1, 2, ... results into an ac */
#define DMA_DMA1_A_START_DMA1_A_START_MASK            (0xFFFFFFFF << 0) /* Source start address of channel 1 */
#define DMA_DMA1_A_START_DMA1_A_START_POS             (0) /* Source start address of channel 1 */
#define DMA_DMA1_B_START_DMA1_B_START_MASK            (0xFFFFFFFF << 0) /* Destination start address of channel 1 */
#define DMA_DMA1_B_START_DMA1_B_START_POS             (0) /* Destination start address of channel 1 */
#define DMA_DMA1_CTRL_DMA_EXCLUSIVE_ACCESS            (1U << 16) /* 0: DMA channel de-asserts the bus request upon completion of the write transfer  */
#define DMA_DMA1_CTRL_BUS_ERROR_DETECT                (1U << 15) /* 0: Ignores bus error response from the AHB bus, so DMA continues normally. 1: De */
#define DMA_DMA1_CTRL_BURST_MODE_MASK                 (0x3 << 13) /* Enables the DMA read/write bursts, according to the following configuration: 00  */
#define DMA_DMA1_CTRL_BURST_MODE_POS                  (13) /* Enables the DMA read/write bursts, according to the following configuration: 00  */
#define DMA_DMA1_CTRL_REQ_SENSE                       (1U << 12) /* 0: DMA operates with level-sensitive peripheral requests (default) 1: DMA operat */
#define DMA_DMA1_CTRL_DMA_INIT                        (1U << 11) /* 0: DMA performs copy A1 to B1, A2 to B2, etc ... 1: DMA performs copy of A1 to B */
#define DMA_DMA1_CTRL_DMA_IDLE                        (1U << 10) /* 0: Blocking mode, the DMA performs a fast back-to-back copy, disabling bus acces */
#define DMA_DMA1_CTRL_DMA_PRIO_MASK                   (0x7 << 7) /* The priority level determines which DMA channel will be granted access for trans */
#define DMA_DMA1_CTRL_DMA_PRIO_POS                    (7) /* The priority level determines which DMA channel will be granted access for trans */
#define DMA_DMA1_CTRL_CIRCULAR                        (1U << 6) /* 0: Normal mode. The DMA channel stops after having completed the transfer of len */
#define DMA_DMA1_CTRL_AINC                            (1U << 5) /* Enable increment of source address. 0 = do not increment (source address stays t */
#define DMA_DMA1_CTRL_BINC                            (1U << 4) /* Enable increment of destination address. 0 = do not increment (destination addre */
#define DMA_DMA1_CTRL_DREQ_MODE                       (1U << 3) /* 0: DMA channel starts immediately 1: DMA channel must be triggered by peripheral */
#define DMA_DMA1_CTRL_BW_MASK                         (0x3 << 1) /* Bus transfer width: 00 = 1 Byte (suggested for peripherals like UART and 8-bit S */
#define DMA_DMA1_CTRL_BW_POS                          (1) /* Bus transfer width: 00 = 1 Byte (suggested for peripherals like UART and 8-bit S */
#define DMA_DMA1_CTRL_DMA_ON                          (1U << 0) /* 0: DMA channel is off, clocks are disabled 1: DMA channel is enabled. This bit w */
#define DMA_DMA1_IDX_DMA1_IDX_MASK                    (0xFFFF << 0) /* This (read-only) register determines the data items already transferred by the D */
#define DMA_DMA1_IDX_DMA1_IDX_POS                     (0) /* This (read-only) register determines the data items already transferred by the D */
#define DMA_DMA1_INT_DMA1_INT_MASK                    (0xFFFF << 0) /* Number of transfers until an interrupt is generated. The interrupt is generated  */
#define DMA_DMA1_INT_DMA1_INT_POS                     (0) /* Number of transfers until an interrupt is generated. The interrupt is generated  */
#define DMA_DMA1_LEN_DMA1_LEN_MASK                    (0xFFFF << 0) /* DMA channel's transfer length. DMA1_LEN of value 0, 1, 2, ... results into an ac */
#define DMA_DMA1_LEN_DMA1_LEN_POS                     (0) /* DMA channel's transfer length. DMA1_LEN of value 0, 1, 2, ... results into an ac */
#define DMA_DMA2_A_START_DMA2_A_START_MASK            (0xFFFFFFFF << 0) /* Source start address of channel 2 */
#define DMA_DMA2_A_START_DMA2_A_START_POS             (0) /* Source start address of channel 2 */
#define DMA_DMA2_B_START_DMA2_B_START_MASK            (0xFFFFFFFF << 0) /* Destination start address of channel 2 */
#define DMA_DMA2_B_START_DMA2_B_START_POS             (0) /* Destination start address of channel 2 */
#define DMA_DMA2_CTRL_DMA_EXCLUSIVE_ACCESS            (1U << 16) /* 0: DMA channel de-asserts the bus request upon completion of the write transfer  */
#define DMA_DMA2_CTRL_BUS_ERROR_DETECT                (1U << 15) /* 0: Ignores bus error response from the AHB bus, so DMA continues normally. 1: De */
#define DMA_DMA2_CTRL_BURST_MODE_MASK                 (0x3 << 13) /* Enables the DMA read/write bursts, according to the following configuration: 00  */
#define DMA_DMA2_CTRL_BURST_MODE_POS                  (13) /* Enables the DMA read/write bursts, according to the following configuration: 00  */
#define DMA_DMA2_CTRL_REQ_SENSE                       (1U << 12) /* 0: DMA operates with level-sensitive peripheral requests (default) 1: DMA operat */
#define DMA_DMA2_CTRL_DMA_INIT                        (1U << 11) /* 0: DMA performs copy A1 to B1, A2 to B2, etc ... 1: DMA performs copy of A1 to B */
#define DMA_DMA2_CTRL_DMA_IDLE                        (1U << 10) /* 0: Blocking mode, the DMA performs a fast back-to-back copy, disabling bus acces */
#define DMA_DMA2_CTRL_DMA_PRIO_MASK                   (0x7 << 7) /* The priority level determines which DMA channel will be granted access for trans */
#define DMA_DMA2_CTRL_DMA_PRIO_POS                    (7) /* The priority level determines which DMA channel will be granted access for trans */
#define DMA_DMA2_CTRL_CIRCULAR                        (1U << 6) /* 0: Normal mode. The DMA channel stops after having completed the transfer of len */
#define DMA_DMA2_CTRL_AINC                            (1U << 5) /* Enable increment of destination address. 0 = do not increment (destination addre */
#define DMA_DMA2_CTRL_BINC                            (1U << 4) /* Enable increment of destination address 0 = do not increment 1 = increment accor */
#define DMA_DMA2_CTRL_DREQ_MODE                       (1U << 3) /* 0: DMA channel starts immediately 1: DMA channel must be triggered by peripheral */
#define DMA_DMA2_CTRL_BW_MASK                         (0x3 << 1) /* Bus transfer width: 00 = 1 Byte (suggested for peripherals like UART and 8-bit S */
#define DMA_DMA2_CTRL_BW_POS                          (1) /* Bus transfer width: 00 = 1 Byte (suggested for peripherals like UART and 8-bit S */
#define DMA_DMA2_CTRL_DMA_ON                          (1U << 0) /* 0: DMA channel is off, clocks are disabled 1: DMA channel is enabled. This bit w */
#define DMA_DMA2_IDX_DMA2_IDX_MASK                    (0xFFFF << 0) /* This (read-only) register determines the data items already transferred by the D */
#define DMA_DMA2_IDX_DMA2_IDX_POS                     (0) /* This (read-only) register determines the data items already transferred by the D */
#define DMA_DMA2_INT_DMA2_INT_MASK                    (0xFFFF << 0) /* Number of transfers until an interrupt is generated. The interrupt is generated  */
#define DMA_DMA2_INT_DMA2_INT_POS                     (0) /* Number of transfers until an interrupt is generated. The interrupt is generated  */
#define DMA_DMA2_LEN_DMA2_LEN_MASK                    (0xFFFF << 0) /* DMA channel's transfer length. DMA2_LEN of value 0, 1, 2, ... results into an ac */
#define DMA_DMA2_LEN_DMA2_LEN_POS                     (0) /* DMA channel's transfer length. DMA2_LEN of value 0, 1, 2, ... results into an ac */
#define DMA_DMA3_A_START_DMA3_A_START_MASK            (0xFFFFFFFF << 0) /* Source start address of channel 3 */
#define DMA_DMA3_A_START_DMA3_A_START_POS             (0) /* Source start address of channel 3 */
#define DMA_DMA3_B_START_DMA3_B_START_MASK            (0xFFFFFFFF << 0) /* Destination start address of channel 3 */
#define DMA_DMA3_B_START_DMA3_B_START_POS             (0) /* Destination start address of channel 3 */
#define DMA_DMA3_CTRL_DMA_EXCLUSIVE_ACCESS            (1U << 16) /* 0: DMA channel de-asserts the bus request upon completion of the write transfer  */
#define DMA_DMA3_CTRL_BUS_ERROR_DETECT                (1U << 15) /* 0: Ignores bus error response from the AHB bus, so DMA continues normally. 1: De */
#define DMA_DMA3_CTRL_BURST_MODE_MASK                 (0x3 << 13) /* Enables the DMA read/write bursts, according to the following configuration: 00  */
#define DMA_DMA3_CTRL_BURST_MODE_POS                  (13) /* Enables the DMA read/write bursts, according to the following configuration: 00  */
#define DMA_DMA3_CTRL_REQ_SENSE                       (1U << 12) /* 0: DMA operates with level-sensitive peripheral requests (default) 1: DMA operat */
#define DMA_DMA3_CTRL_DMA_INIT                        (1U << 11) /* 0: DMA performs copy A1 to B1, A2 to B2, etc ... 1: DMA performs copy of A1 to B */
#define DMA_DMA3_CTRL_DMA_IDLE                        (1U << 10) /* 0: Blocking mode, the DMA performs a fast back-to-back copy, disabling bus acces */
#define DMA_DMA3_CTRL_DMA_PRIO_MASK                   (0x7 << 7) /* The priority level determines which DMA channel will be granted access for trans */
#define DMA_DMA3_CTRL_DMA_PRIO_POS                    (7) /* The priority level determines which DMA channel will be granted access for trans */
#define DMA_DMA3_CTRL_CIRCULAR                        (1U << 6) /* 0: Normal mode. The DMA channel stops after having completed the transfer of len */
#define DMA_DMA3_CTRL_AINC                            (1U << 5) /* Enable increment of source address. 0 = do not increment (source address stays t */
#define DMA_DMA3_CTRL_BINC                            (1U << 4) /* Enable increment of destination address. 0 = do not increment (destination addre */
#define DMA_DMA3_CTRL_DREQ_MODE                       (1U << 3) /* 0: DMA channel starts immediately 1: DMA channel must be triggered by peripheral */
#define DMA_DMA3_CTRL_BW_MASK                         (0x3 << 1) /* Bus transfer width: 00 = 1 Byte (suggested for peripherals like UART and 8-bit S */
#define DMA_DMA3_CTRL_BW_POS                          (1) /* Bus transfer width: 00 = 1 Byte (suggested for peripherals like UART and 8-bit S */
#define DMA_DMA3_CTRL_DMA_ON                          (1U << 0) /* 0: DMA channel is off, clocks are disabled 1: DMA channel is enabled. This bit w */
#define DMA_DMA3_IDX_DMA3_IDX_MASK                    (0xFFFF << 0) /* This (read-only) register determines the data items already transferred by the D */
#define DMA_DMA3_IDX_DMA3_IDX_POS                     (0) /* This (read-only) register determines the data items already transferred by the D */
#define DMA_DMA3_INT_DMA3_INT_MASK                    (0xFFFF << 0) /* Number of transfers until an interrupt is generated. The interrupt is generated  */
#define DMA_DMA3_INT_DMA3_INT_POS                     (0) /* Number of transfers until an interrupt is generated. The interrupt is generated  */
#define DMA_DMA3_LEN_DMA3_LEN_MASK                    (0xFFFF << 0) /* DMA channel's transfer length. DMA3_LEN of value 0, 1, 2, ... results into an ac */
#define DMA_DMA3_LEN_DMA3_LEN_POS                     (0) /* DMA channel's transfer length. DMA3_LEN of value 0, 1, 2, ... results into an ac */
#define DMA_DMA4_A_START_DMA4_A_START_MASK            (0xFFFFFFFF << 0) /* Source start address of channel 4 */
#define DMA_DMA4_A_START_DMA4_A_START_POS             (0) /* Source start address of channel 4 */
#define DMA_DMA4_B_START_DMA4_B_START_MASK            (0xFFFFFFFF << 0) /* Destination start address of channel 4 */
#define DMA_DMA4_B_START_DMA4_B_START_POS             (0) /* Destination start address of channel 4 */
#define DMA_DMA4_CTRL_DMA_EXCLUSIVE_ACCESS            (1U << 16) /* 0: DMA channel de-asserts the bus request upon completion of the write transfer  */
#define DMA_DMA4_CTRL_BUS_ERROR_DETECT                (1U << 15) /* 0: Ignores bus error response from the AHB bus, so DMA continues normally. 1: De */
#define DMA_DMA4_CTRL_BURST_MODE_MASK                 (0x3 << 13) /* Enables the DMA read/write bursts, according to the following configuration: 00  */
#define DMA_DMA4_CTRL_BURST_MODE_POS                  (13) /* Enables the DMA read/write bursts, according to the following configuration: 00  */
#define DMA_DMA4_CTRL_REQ_SENSE                       (1U << 12) /* 0: DMA operates with level-sensitive peripheral requests (default) 1: DMA operat */
#define DMA_DMA4_CTRL_DMA_INIT                        (1U << 11) /* 0: DMA performs copy A1 to B1, A2 to B2, etc ... 1: DMA performs copy of A1 to B */
#define DMA_DMA4_CTRL_DMA_IDLE                        (1U << 10) /* 0: Blocking mode, the DMA performs a fast back-to-back copy, disabling bus acces */
#define DMA_DMA4_CTRL_DMA_PRIO_MASK                   (0x7 << 7) /* The priority level determines which DMA channel will be granted access for trans */
#define DMA_DMA4_CTRL_DMA_PRIO_POS                    (7) /* The priority level determines which DMA channel will be granted access for trans */
#define DMA_DMA4_CTRL_CIRCULAR                        (1U << 6) /* 0: Normal mode. The DMA channel stops after having completed the transfer of len */
#define DMA_DMA4_CTRL_AINC                            (1U << 5) /* Enable increment of source address. 0 = do not increment (source address stays t */
#define DMA_DMA4_CTRL_BINC                            (1U << 4) /* Enable increment of destination address. 0 = do not increment (destination addre */
#define DMA_DMA4_CTRL_DREQ_MODE                       (1U << 3) /* 0: DMA channel starts immediately 1: DMA channel must be triggered by peripheral */
#define DMA_DMA4_CTRL_BW_MASK                         (0x3 << 1) /* Bus transfer width: 00 = 1 Byte (suggested for peripherals like UART and 8-bit S */
#define DMA_DMA4_CTRL_BW_POS                          (1) /* Bus transfer width: 00 = 1 Byte (suggested for peripherals like UART and 8-bit S */
#define DMA_DMA4_CTRL_DMA_ON                          (1U << 0) /* 0: DMA channel is off, clocks are disabled 1: DMA channel is enabled. This bit w */
#define DMA_DMA4_IDX_DMA4_IDX_MASK                    (0xFFFF << 0) /* This (read-only) register determines the data items already transferred by the D */
#define DMA_DMA4_IDX_DMA4_IDX_POS                     (0) /* This (read-only) register determines the data items already transferred by the D */
#define DMA_DMA4_INT_DMA4_INT_MASK                    (0xFFFF << 0) /* Number of transfers until an interrupt is generated. The interrupt is generated  */
#define DMA_DMA4_INT_DMA4_INT_POS                     (0) /* Number of transfers until an interrupt is generated. The interrupt is generated  */
#define DMA_DMA4_LEN_DMA4_LEN_MASK                    (0xFFFF << 0) /* DMA channel's transfer length. DMA4_LEN of value 0, 1, 2, ... results into an ac */
#define DMA_DMA4_LEN_DMA4_LEN_POS                     (0) /* DMA channel's transfer length. DMA4_LEN of value 0, 1, 2, ... results into an ac */
#define DMA_DMA5_A_START_DMA5_A_START_MASK            (0xFFFFFFFF << 0) /* Source start address of channel 5 */
#define DMA_DMA5_A_START_DMA5_A_START_POS             (0) /* Source start address of channel 5 */
#define DMA_DMA5_B_START_DMA5_B_START_MASK            (0xFFFFFFFF << 0) /* Destination start address of channel 5 */
#define DMA_DMA5_B_START_DMA5_B_START_POS             (0) /* Destination start address of channel 5 */
#define DMA_DMA5_CTRL_DMA_EXCLUSIVE_ACCESS            (1U << 16) /* 0: DMA channel de-asserts the bus request upon completion of the write transfer  */
#define DMA_DMA5_CTRL_BUS_ERROR_DETECT                (1U << 15) /* 0: Ignores bus error response from the AHB bus, so DMA continues normally. 1: De */
#define DMA_DMA5_CTRL_BURST_MODE_MASK                 (0x3 << 13) /* Enables the DMA read/write bursts, according to the following configuration: 00  */
#define DMA_DMA5_CTRL_BURST_MODE_POS                  (13) /* Enables the DMA read/write bursts, according to the following configuration: 00  */
#define DMA_DMA5_CTRL_REQ_SENSE                       (1U << 12) /* 0: DMA operates with level-sensitive peripheral requests (default) 1: DMA operat */
#define DMA_DMA5_CTRL_DMA_INIT                        (1U << 11) /* 0: DMA performs copy A1 to B1, A2 to B2, etc ... 1: DMA performs copy of A1 to B */
#define DMA_DMA5_CTRL_DMA_IDLE                        (1U << 10) /* 0: Blocking mode, the DMA performs a fast back-to-back copy, disabling bus acces */
#define DMA_DMA5_CTRL_DMA_PRIO_MASK                   (0x7 << 7) /* The priority level determines which DMA channel will be granted access for trans */
#define DMA_DMA5_CTRL_DMA_PRIO_POS                    (7) /* The priority level determines which DMA channel will be granted access for trans */
#define DMA_DMA5_CTRL_CIRCULAR                        (1U << 6) /* 0: Normal mode. The DMA channel stops after having completed the transfer of len */
#define DMA_DMA5_CTRL_AINC                            (1U << 5) /* Enable increment of source address. 0 = do not increment (source address stays t */
#define DMA_DMA5_CTRL_BINC                            (1U << 4) /* Enable increment of destination address. 0 = do not increment (destination addre */
#define DMA_DMA5_CTRL_DREQ_MODE                       (1U << 3) /* 0: DMA channel starts immediately 1: DMA channel must be triggered by peripheral */
#define DMA_DMA5_CTRL_BW_MASK                         (0x3 << 1) /* Bus transfer width: 00 = 1 Byte (suggested for peripherals like UART and 8-bit S */
#define DMA_DMA5_CTRL_BW_POS                          (1) /* Bus transfer width: 00 = 1 Byte (suggested for peripherals like UART and 8-bit S */
#define DMA_DMA5_CTRL_DMA_ON                          (1U << 0) /* 0: DMA channel is off, clocks are disabled 1: DMA channel is enabled. This bit w */
#define DMA_DMA5_IDX_DMA5_IDX_MASK                    (0xFFFF << 0) /* This (read-only) register determines the data items already transferred by the D */
#define DMA_DMA5_IDX_DMA5_IDX_POS                     (0) /* This (read-only) register determines the data items already transferred by the D */
#define DMA_DMA5_INT_DMA5_INT_MASK                    (0xFFFF << 0) /* Number of transfers until an interrupt is generated. The interrupt is generated  */
#define DMA_DMA5_INT_DMA5_INT_POS                     (0) /* Number of transfers until an interrupt is generated. The interrupt is generated  */
#define DMA_DMA5_LEN_DMA5_LEN_MASK                    (0xFFFF << 0) /* DMA channel's transfer length. DMA5_LEN of value 0, 1, 2, ... results into an ac */
#define DMA_DMA5_LEN_DMA5_LEN_POS                     (0) /* DMA channel's transfer length. DMA5_LEN of value 0, 1, 2, ... results into an ac */
#define DMA_DMA6_A_START_DMA6_A_START_MASK            (0xFFFFFFFF << 0) /* Source start address of channel 6 */
#define DMA_DMA6_A_START_DMA6_A_START_POS             (0) /* Source start address of channel 6 */
#define DMA_DMA6_B_START_DMA6_B_START_MASK            (0xFFFFFFFF << 0) /* Destination start address of channel 6 */
#define DMA_DMA6_B_START_DMA6_B_START_POS             (0) /* Destination start address of channel 6 */
#define DMA_DMA6_CTRL_DMA_EXCLUSIVE_ACCESS            (1U << 16) /* 0: DMA channel de-asserts the bus request upon completion of the write transfer  */
#define DMA_DMA6_CTRL_BUS_ERROR_DETECT                (1U << 15) /* 0: Ignores bus error response from the AHB bus, so DMA continues normally. 1: De */
#define DMA_DMA6_CTRL_BURST_MODE_MASK                 (0x3 << 13) /* Enables the DMA read/write bursts, according to the following configuration: 00  */
#define DMA_DMA6_CTRL_BURST_MODE_POS                  (13) /* Enables the DMA read/write bursts, according to the following configuration: 00  */
#define DMA_DMA6_CTRL_REQ_SENSE                       (1U << 12) /* 0: DMA operates with level-sensitive peripheral requests (default) 1: DMA operat */
#define DMA_DMA6_CTRL_DMA_INIT                        (1U << 11) /* 0: DMA performs copy A1 to B1, A2 to B2, etc ... 1: DMA performs copy of A1 to B */
#define DMA_DMA6_CTRL_DMA_IDLE                        (1U << 10) /* 0: Blocking mode, the DMA performs a fast back-to-back copy, disabling bus acces */
#define DMA_DMA6_CTRL_DMA_PRIO_MASK                   (0x7 << 7) /* The priority level determines which DMA channel will be granted access for trans */
#define DMA_DMA6_CTRL_DMA_PRIO_POS                    (7) /* The priority level determines which DMA channel will be granted access for trans */
#define DMA_DMA6_CTRL_CIRCULAR                        (1U << 6) /* 0: Normal mode. The DMA channel stops after having completed the transfer of len */
#define DMA_DMA6_CTRL_AINC                            (1U << 5) /* Enable increment of source address. 0 = do not increment (source address stays t */
#define DMA_DMA6_CTRL_BINC                            (1U << 4) /* Enable increment of destination address. 0 = do not increment (destination addre */
#define DMA_DMA6_CTRL_DREQ_MODE                       (1U << 3) /* 0: DMA channel starts immediately 1: DMA channel must be triggered by peripheral */
#define DMA_DMA6_CTRL_BW_MASK                         (0x3 << 1) /* Bus transfer width: 00 = 1 Byte (suggested for peripherals like UART and 8-bit S */
#define DMA_DMA6_CTRL_BW_POS                          (1) /* Bus transfer width: 00 = 1 Byte (suggested for peripherals like UART and 8-bit S */
#define DMA_DMA6_CTRL_DMA_ON                          (1U << 0) /* 0: DMA channel is off, clocks are disabled 1: DMA channel is enabled. This bit w */
#define DMA_DMA6_IDX_DMA6_IDX_MASK                    (0xFFFF << 0) /* This (read-only) register determines the data items already transferred by the D */
#define DMA_DMA6_IDX_DMA6_IDX_POS                     (0) /* This (read-only) register determines the data items already transferred by the D */
#define DMA_DMA6_INT_DMA6_INT_MASK                    (0xFFFF << 0) /* Number of transfers until an interrupt is generated. The interrupt is generated  */
#define DMA_DMA6_INT_DMA6_INT_POS                     (0) /* Number of transfers until an interrupt is generated. The interrupt is generated  */
#define DMA_DMA6_LEN_DMA6_LEN_MASK                    (0xFFFF << 0) /* DMA channel's transfer length. DMA6_LEN of value 0, 1, 2, ... results into an ac */
#define DMA_DMA6_LEN_DMA6_LEN_POS                     (0) /* DMA channel's transfer length. DMA6_LEN of value 0, 1, 2, ... results into an ac */
#define DMA_DMA7_A_START_DMA7_A_START_MASK            (0xFFFFFFFF << 0) /* Source start address of channel 7 */
#define DMA_DMA7_A_START_DMA7_A_START_POS             (0) /* Source start address of channel 7 */
#define DMA_DMA7_B_START_DMA7_B_START_MASK            (0xFFFFFFFF << 0) /* Destination start address of channel 7 */
#define DMA_DMA7_B_START_DMA7_B_START_POS             (0) /* Destination start address of channel 7 */
#define DMA_DMA7_CTRL_DMA_EXCLUSIVE_ACCESS            (1U << 16) /* 0: DMA channel de-asserts the bus request upon completion of the write transfer  */
#define DMA_DMA7_CTRL_BUS_ERROR_DETECT                (1U << 15) /* 0: Ignores bus error response from the AHB bus, so DMA continues normally. 1: De */
#define DMA_DMA7_CTRL_BURST_MODE_MASK                 (0x3 << 13) /* Enables the DMA read/write bursts, according to the following configuration: 00  */
#define DMA_DMA7_CTRL_BURST_MODE_POS                  (13) /* Enables the DMA read/write bursts, according to the following configuration: 00  */
#define DMA_DMA7_CTRL_REQ_SENSE                       (1U << 12) /* 0: DMA operates with level-sensitive peripheral requests (default) 1: DMA operat */
#define DMA_DMA7_CTRL_DMA_INIT                        (1U << 11) /* 0: DMA performs copy A1 to B1, A2 to B2, etc ... 1: DMA performs copy of A1 to B */
#define DMA_DMA7_CTRL_DMA_IDLE                        (1U << 10) /* 0: Blocking mode, the DMA performs a fast back-to-back copy, disabling bus acces */
#define DMA_DMA7_CTRL_DMA_PRIO_MASK                   (0x7 << 7) /* The priority level determines which DMA channel will be granted access for trans */
#define DMA_DMA7_CTRL_DMA_PRIO_POS                    (7) /* The priority level determines which DMA channel will be granted access for trans */
#define DMA_DMA7_CTRL_CIRCULAR                        (1U << 6) /* 0: Normal mode. The DMA channel stops after having completed the transfer of len */
#define DMA_DMA7_CTRL_AINC                            (1U << 5) /* Enable increment of source address. 0 = do not increment (source address stays t */
#define DMA_DMA7_CTRL_BINC                            (1U << 4) /* Enable increment of destination address. 0 = do not increment (destination addre */
#define DMA_DMA7_CTRL_DREQ_MODE                       (1U << 3) /* 0: DMA channel starts immediately 1: DMA channel must be triggered by peripheral */
#define DMA_DMA7_CTRL_BW_MASK                         (0x3 << 1) /* Bus transfer width: 00 = 1 Byte (suggested for peripherals like UART and 8-bit S */
#define DMA_DMA7_CTRL_BW_POS                          (1) /* Bus transfer width: 00 = 1 Byte (suggested for peripherals like UART and 8-bit S */
#define DMA_DMA7_CTRL_DMA_ON                          (1U << 0) /* 0: DMA channel is off, clocks are disabled 1: DMA channel is enabled. This bit w */
#define DMA_DMA7_IDX_DMA7_IDX_MASK                    (0xFFFF << 0) /* This (read-only) register determines the data items already transferred by the D */
#define DMA_DMA7_IDX_DMA7_IDX_POS                     (0) /* This (read-only) register determines the data items already transferred by the D */
#define DMA_DMA7_INT_DMA7_INT_MASK                    (0xFFFF << 0) /* Number of transfers until an interrupt is generated. The interrupt is generated  */
#define DMA_DMA7_INT_DMA7_INT_POS                     (0) /* Number of transfers until an interrupt is generated. The interrupt is generated  */
#define DMA_DMA7_LEN_DMA7_LEN_MASK                    (0xFFFF << 0) /* DMA channel's transfer length. DMA7_LEN of value 0, 1, 2, ... results into an ac */
#define DMA_DMA7_LEN_DMA7_LEN_POS                     (0) /* DMA channel's transfer length. DMA7_LEN of value 0, 1, 2, ... results into an ac */
#define DMA_DMA_CLEAR_INT_DMA_RST_IRQ_CH7             (1U << 7) /* Writing a 1 will reset the status bit of DMA_INT_STATUS_REG for channel 7 ; writ */
#define DMA_DMA_CLEAR_INT_DMA_RST_IRQ_CH6             (1U << 6) /* Writing a 1 will reset the status bit of DMA_INT_STATUS_REG for channel 6 ; writ */
#define DMA_DMA_CLEAR_INT_DMA_RST_IRQ_CH5             (1U << 5) /* Writing a 1 will reset the status bit of DMA_INT_STATUS_REG for channel 5 ; writ */
#define DMA_DMA_CLEAR_INT_DMA_RST_IRQ_CH4             (1U << 4) /* Writing a 1 will reset the status bit of DMA_INT_STATUS_REG for channel 4 ; writ */
#define DMA_DMA_CLEAR_INT_DMA_RST_IRQ_CH3             (1U << 3) /* Writing a 1 will reset the status bit of DMA_INT_STATUS_REG for channel 3 ; writ */
#define DMA_DMA_CLEAR_INT_DMA_RST_IRQ_CH2             (1U << 2) /* Writing a 1 will reset the status bit of DMA_INT_STATUS_REG for channel 2 ; writ */
#define DMA_DMA_CLEAR_INT_DMA_RST_IRQ_CH1             (1U << 1) /* Writing a 1 will reset the status bit of DMA_INT_STATUS_REG for channel 1 ; writ */
#define DMA_DMA_CLEAR_INT_DMA_RST_IRQ_CH0             (1U << 0) /* Writing a 1 will reset the status bit of DMA_INT_STATUS_REG for channel 0 ; writ */
#define DMA_DMA_INT_MASK_DMA_IRQ_ENABLE7              (1U << 7) /* 0: disable interrupts on channel 7 1: enable interrupts on channel 7 */
#define DMA_DMA_INT_MASK_DMA_IRQ_ENABLE6              (1U << 6) /* 0: disable interrupts on channel 6 1: enable interrupts on channel 6 */
#define DMA_DMA_INT_MASK_DMA_IRQ_ENABLE5              (1U << 5) /* 0: disable interrupts on channel 5 1: enable interrupts on channel 5 */
#define DMA_DMA_INT_MASK_DMA_IRQ_ENABLE4              (1U << 4) /* 0: disable interrupts on channel 4 1: enable interrupts on channel 4 */
#define DMA_DMA_INT_MASK_DMA_IRQ_ENABLE3              (1U << 3) /* 0: disable interrupts on channel 3 1: enable interrupts on channel 3 */
#define DMA_DMA_INT_MASK_DMA_IRQ_ENABLE2              (1U << 2) /* 0: disable interrupts on channel 2 1: enable interrupts on channel 2 */
#define DMA_DMA_INT_MASK_DMA_IRQ_ENABLE1              (1U << 1) /* 0: disable interrupts on channel 1 1: enable interrupts on channel 1 */
#define DMA_DMA_INT_MASK_DMA_IRQ_ENABLE0              (1U << 0) /* 0: disable interrupts on channel 0 1: enable interrupts on channel 0 */
#define DMA_DMA_INT_STATUS_DMA_BUS_ERR7               (1U << 15) /* 0: No bus error response is detected for channel 7 1: Bus error response detecte */
#define DMA_DMA_INT_STATUS_DMA_BUS_ERR6               (1U << 14) /* 0: No bus error response is detected for channel 6 1: Bus error response detecte */
#define DMA_DMA_INT_STATUS_DMA_BUS_ERR5               (1U << 13) /* 0: No bus error response is detected for channel 5 1: Bus error response detecte */
#define DMA_DMA_INT_STATUS_DMA_BUS_ERR4               (1U << 12) /* 0: No bus error response is detected for channel 4 1: Bus error response detecte */
#define DMA_DMA_INT_STATUS_DMA_BUS_ERR3               (1U << 11) /* 0: No bus error response is detected for channel 3 1: Bus error response detecte */
#define DMA_DMA_INT_STATUS_DMA_BUS_ERR2               (1U << 10) /* 0: No bus error response is detected for channel 2 1: Bus error response detecte */
#define DMA_DMA_INT_STATUS_DMA_BUS_ERR1               (1U << 9) /* 0: No bus error response is detected for channel 1 1: Bus error response detecte */
#define DMA_DMA_INT_STATUS_DMA_BUS_ERR0               (1U << 8) /* 0: No bus error response is detected for channel 0 1: Bus error response detecte */
#define DMA_DMA_INT_STATUS_DMA_IRQ_CH7                (1U << 7) /* 0: IRQ on channel 7 is not set 1: IRQ on channel 7 is set */
#define DMA_DMA_INT_STATUS_DMA_IRQ_CH6                (1U << 6) /* 0: IRQ on channel 6 is not set 1: IRQ on channel 6 is set */
#define DMA_DMA_INT_STATUS_DMA_IRQ_CH5                (1U << 5) /* 0: IRQ on channel 5 is not set 1: IRQ on channel 5 is set */
#define DMA_DMA_INT_STATUS_DMA_IRQ_CH4                (1U << 4) /* 0: IRQ on channel 4 is not set 1: IRQ on channel 4 is set */
#define DMA_DMA_INT_STATUS_DMA_IRQ_CH3                (1U << 3) /* 0: IRQ on channel 3 is not set 1: IRQ on channel 3 is set */
#define DMA_DMA_INT_STATUS_DMA_IRQ_CH2                (1U << 2) /* 0: IRQ on channel 2 is not set 1: IRQ on channel 2 is set */
#define DMA_DMA_INT_STATUS_DMA_IRQ_CH1                (1U << 1) /* 0: IRQ on channel 1 is not set 1: IRQ on channel 1 is set */
#define DMA_DMA_INT_STATUS_DMA_IRQ_CH0                (1U << 0) /* 0: IRQ on channel 0 is not set 1: IRQ on channel 0 is set */
#define DMA_DMA_REQ_MUX_DMA67_SEL_MASK                (0xF << 12) /* Select which combination of peripherals are mapped on the DMA channels. The peri */
#define DMA_DMA_REQ_MUX_DMA67_SEL_POS                 (12) /* Select which combination of peripherals are mapped on the DMA channels. The peri */
#define DMA_DMA_REQ_MUX_DMA45_SEL_MASK                (0xF << 8) /* Select which combination of peripherals are mapped on the DMA channels. The peri */
#define DMA_DMA_REQ_MUX_DMA45_SEL_POS                 (8) /* Select which combination of peripherals are mapped on the DMA channels. The peri */
#define DMA_DMA_REQ_MUX_DMA23_SEL_MASK                (0xF << 4) /* Select which combination of peripherals are mapped on the DMA channels. The peri */
#define DMA_DMA_REQ_MUX_DMA23_SEL_POS                 (4) /* Select which combination of peripherals are mapped on the DMA channels. The peri */
#define DMA_DMA_REQ_MUX_DMA01_SEL_MASK                (0xF << 0) /* Select which combination of peripherals are mapped on the DMA channels. The peri */
#define DMA_DMA_REQ_MUX_DMA01_SEL_POS                 (0) /* Select which combination of peripherals are mapped on the DMA channels. The peri */
#define DMA_DMA_RESET_INT_MASK_DMA_RESET_IRQ_ENABLE7  (1U << 7) /* Writing a '1' will disable the IRQs in the DMA channel 7, writing a '0' has no e */
#define DMA_DMA_RESET_INT_MASK_DMA_RESET_IRQ_ENABLE6  (1U << 6) /* Writing a '1' will disable the IRQs in the DMA channel 6, writing a '0' has no e */
#define DMA_DMA_RESET_INT_MASK_DMA_RESET_IRQ_ENABLE5  (1U << 5) /* Writing a '1' will disable the IRQs in the DMA channel 5, writing a '0' has no e */
#define DMA_DMA_RESET_INT_MASK_DMA_RESET_IRQ_ENABLE4  (1U << 4) /* Writing a '1' will disable the IRQs in the DMA channel 4, writing a '0' has no e */
#define DMA_DMA_RESET_INT_MASK_DMA_RESET_IRQ_ENABLE3  (1U << 3) /* Writing a '1' will disable the IRQs in the DMA channel 3, writing a '0' has no e */
#define DMA_DMA_RESET_INT_MASK_DMA_RESET_IRQ_ENABLE2  (1U << 2) /* Writing a '1' will disable the IRQs in the DMA channel 2, writing a '0' has no e */
#define DMA_DMA_RESET_INT_MASK_DMA_RESET_IRQ_ENABLE1  (1U << 1) /* Writing a '1' will disable the IRQs in the DMA channel 1, writing a '0' has no e */
#define DMA_DMA_RESET_INT_MASK_DMA_RESET_IRQ_ENABLE0  (1U << 0) /* Writing a '1' will disable the IRQs in the DMA channel 0, writing a '0' has no e */
#define DMA_DMA_SET_INT_MASK_DMA_SET_IRQ_ENABLE7      (1U << 7) /* Writing a '1' will enable the IRQs in the DMA channel 7, writing a '0' has no ef */
#define DMA_DMA_SET_INT_MASK_DMA_SET_IRQ_ENABLE6      (1U << 6) /* Writing a '1' will enable the IRQs in the DMA channel 6, writing a '0' has no ef */
#define DMA_DMA_SET_INT_MASK_DMA_SET_IRQ_ENABLE5      (1U << 5) /* Writing a '1' will enable the IRQs in the DMA channel 5, writing a '0' has no ef */
#define DMA_DMA_SET_INT_MASK_DMA_SET_IRQ_ENABLE4      (1U << 4) /* Writing a '1' will enable the IRQs in the DMA channel 4, writing a '0' has no ef */
#define DMA_DMA_SET_INT_MASK_DMA_SET_IRQ_ENABLE3      (1U << 3) /* Writing a '1' will enable the IRQs in the DMA channel 3, writing a '0' has no ef */
#define DMA_DMA_SET_INT_MASK_DMA_SET_IRQ_ENABLE2      (1U << 2) /* Writing a '1' will enable the IRQs in the DMA channel 2, writing a '0' has no ef */
#define DMA_DMA_SET_INT_MASK_DMA_SET_IRQ_ENABLE1      (1U << 1) /* Writing a '1' will enable the IRQs in the DMA channel 1, writing a '0' has no ef */
#define DMA_DMA_SET_INT_MASK_DMA_SET_IRQ_ENABLE0      (1U << 0) /* Writing a '1' will enable the IRQs in the DMA channel 0, writing a '0' has no ef */

#endif /* __DA1470_DMA_H */
