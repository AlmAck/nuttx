/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_gpio.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_GPIO_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_GPIO_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "hardware/da1470x_memorymap.h"
#include <nuttx/config.h>
#include <stdint.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define DA1470_GPIO_NPORTS 3

#define DA1470_GPIO_PORT0 0
#define DA1470_GPIO_PORT1 1
#define DA1470_GPIO_PORT2 2

#define DA1470_GPIO_NPINS 32
#define DA1470_GPIO_PORT2_NPINS 16

/* Register definitions */

/* Register offsets */
#define DA1470_GPIO_GPIO_CLK_SEL_OFFSET 0x016c
#define DA1470_GPIO_LCDC_MAP_CTRL_OFFSET 0x017c
#define DA1470_GPIO_PAD_DRIVE_CTRL_OFFSET 0x0180

#define DA1470_GPIO_MODE_OFFSET_P0(pin) (0x0024 + (pin) * 4)
#define DA1470_GPIO_P0_DATA_OFFSET 0x0000
#define DA1470_GPIO_P0_PADPWR_CTRL_OFFSET 0x0160
#define DA1470_GPIO_P0_RESET_DATA_OFFSET 0x0018
#define DA1470_GPIO_P0_SET_DATA_OFFSET 0x000c
#define DA1470_GPIO_P0_WEAK_CTRL_OFFSET 0x0170

#define DA1470_GPIO_MODE_OFFSET_P1(pin) (0x00a4 + (pin) * 4)
#define DA1470_GPIO_P1_DATA_OFFSET 0x0004
#define DA1470_GPIO_P1_PADPWR_CTRL_OFFSET 0x0164
#define DA1470_GPIO_P1_RESET_DATA_OFFSET 0x001c
#define DA1470_GPIO_P1_SET_DATA_OFFSET 0x0010
#define DA1470_GPIO_P1_WEAK_CTRL_OFFSET 0x0174

#define DA1470_GPIO_MODE_OFFSET_P2(pin) (0x0124 + (pin) * 4)
#define DA1470_GPIO_P2_DATA_OFFSET 0x0008
#define DA1470_GPIO_P2_PADPWR_CTRL_OFFSET 0x0168
#define DA1470_GPIO_P2_RESET_DATA_OFFSET 0x0020
#define DA1470_GPIO_P2_SET_DATA_OFFSET 0x0014
#define DA1470_GPIO_P2_WEAK_CTRL_OFFSET 0x0178

#define DA1470_GPIO_MODE_OFFSET(port, pin)                                     \
  (0x0024 + (port) * 0x0080 + (pin) * 4)
#define DA1470_GPIO_DATA_OFFSET(port) (0x0000 + (port) * 4)
#define DA1470_GPIO_PADPWR_CTRL_OFFSET(port) (0x0160 + (port) * 4)
#define DA1470_GPIO_RESET_DATA_OFFSET(port) (0x0018 + (port) * 4)
#define DA1470_GPIO_SET_DATA_OFFSET(port) (0x000c + (port) * 4)
#define DA1470_GPIO_WEAK_CTRL_OFFSET(port) (0x0170 + (port) * 4)

/* Register addresses *******************************************************/

#define DA1470_GPIO_BASE 0x50050100

#define DA1470_GPIO_GPIO_CLK_SEL                                               \
  (DA1470_GPIO_BASE + DA1470_GPIO_GPIO_CLK_SEL_OFFSET)
#define DA1470_GPIO_LCDC_MAP_CTRL                                              \
  (DA1470_GPIO_BASE + DA1470_GPIO_LCDC_MAP_CTRL_OFFSET)
#define DA1470_GPIO_PAD_DRIVE_CTRL                                             \
  (DA1470_GPIO_BASE + DA1470_GPIO_PAD_DRIVE_CTRL_OFFSET)

#define DA1470_GPIO_P0_MODE(n)                                                 \
  (DA1470_GPIO_BASE + DA1470_GPIO_MODE_OFFSET_P0(n))
#define DA1470_GPIO_P0_DATA (DA1470_GPIO_BASE + DA1470_GPIO_P0_DATA_OFFSET)
#define DA1470_GPIO_P0_PADPWR_CTRL                                             \
  (DA1470_GPIO_BASE + DA1470_GPIO_P0_PADPWR_CTRL_OFFSET)
#define DA1470_GPIO_P0_RESET_DATA                                              \
  (DA1470_GPIO_BASE + DA1470_GPIO_P0_RESET_DATA_OFFSET)
#define DA1470_GPIO_P0_SET_DATA                                                \
  (DA1470_GPIO_BASE + DA1470_GPIO_P0_SET_DATA_OFFSET)
#define DA1470_GPIO_P0_WEAK_CTRL                                               \
  (DA1470_GPIO_BASE + DA1470_GPIO_P0_WEAK_CTRL_OFFSET)

#define DA1470_GPIO_P1_MODE(n)                                                 \
  (DA1470_GPIO_BASE + DA1470_GPIO_MODE_OFFSET_P1(n))
#define DA1470_GPIO_P1_DATA (DA1470_GPIO_BASE + DA1470_GPIO_P1_DATA_OFFSET)
#define DA1470_GPIO_P1_PADPWR_CTRL                                             \
  (DA1470_GPIO_BASE + DA1470_GPIO_P1_PADPWR_CTRL_OFFSET)
#define DA1470_GPIO_P1_RESET_DATA                                              \
  (DA1470_GPIO_BASE + DA1470_GPIO_P1_RESET_DATA_OFFSET)
#define DA1470_GPIO_P1_SET_DATA                                                \
  (DA1470_GPIO_BASE + DA1470_GPIO_P1_SET_DATA_OFFSET)
#define DA1470_GPIO_P1_WEAK_CTRL                                               \
  (DA1470_GPIO_BASE + DA1470_GPIO_P1_WEAK_CTRL_OFFSET)

#define DA1470_GPIO_P2_MODE(n)                                                 \
  (DA1470_GPIO_BASE + DA1470_GPIO_MODE_OFFSET_P2(n))
#define DA1470_GPIO_P2_DATA (DA1470_GPIO_BASE + DA1470_GPIO_P2_DATA_OFFSET)
#define DA1470_GPIO_P2_PADPWR_CTRL                                             \
  (DA1470_GPIO_BASE + DA1470_GPIO_P2_PADPWR_CTRL_OFFSET)
#define DA1470_GPIO_P2_RESET_DATA                                              \
  (DA1470_GPIO_BASE + DA1470_GPIO_P2_RESET_DATA_OFFSET)
#define DA1470_GPIO_P2_SET_DATA                                                \
  (DA1470_GPIO_BASE + DA1470_GPIO_P2_SET_DATA_OFFSET)
#define DA1470_GPIO_P2_WEAK_CTRL                                               \
  (DA1470_GPIO_BASE + DA1470_GPIO_P2_WEAK_CTRL_OFFSET)

#define DA1470_GPIO_PX_MODE(port, pin)                                         \
  (DA1470_GPIO_BASE + DA1470_GPIO_MODE_OFFSET(port, pin))
#define DA1470_GPIO_PX_DATA(port)                                              \
  (DA1470_GPIO_BASE + DA1470_GPIO_DATA_OFFSET(port))
#define DA1470_GPIO_PX_PADPWR_CTRL(port)                                       \
  (DA1470_GPIO_BASE + DA1470_GPIO_PADPWR_CTRL_OFFSET(port))
#define DA1470_GPIO_PX_RESET_DATA(port)                                        \
  (DA1470_GPIO_BASE + DA1470_GPIO_RESET_DATA_OFFSET(port))
#define DA1470_GPIO_PX_SET_DATA(port)                                          \
  (DA1470_GPIO_BASE + DA1470_GPIO_SET_DATA_OFFSET(port))
#define DA1470_GPIO_PX_WEAK_CTRL(port)                                         \
  (DA1470_GPIO_BASE + DA1470_GPIO_WEAK_CTRL_OFFSET(port))

/* GPIO alternate function register */

#define GPIO_FUNCTION_SHIFT(n) (0)
#define GPIO_FUNCTION_MASK(n) (0x1F << GPIO_FUNCTION_SHIFT(n))

/* Register bit definitions *************************************************/

/* Mask definitions */
#define GPIO_MODE_REG_PPOD_OFFSET (10)
#define GPIO_MODE_REG_PPOD_MASK (0x01 << GPIO_MODE_REG_PPOD_OFFSET)

#define GPIO_MODE_REG_PUPD_OFFSET (8)
#define GPIO_MODE_REG_PUPD_MASK (0x03 << GPIO_MODE_REG_PUPD_OFFSET)
#define GPIO_MODE_INPUT_PULL_DISABLED (0 << GPIO_MODE_REG_PUPD_OFFSET)
#define GPIO_MODE_INPUT_PULL_UP (1 << GPIO_MODE_REG_PUPD_OFFSET)
#define GPIO_MODE_INPUT_PULL_DOWN (2 << GPIO_MODE_REG_PUPD_OFFSET)
#define GPIO_MODE_OUTPUT_PULL_DISABLED (3 << GPIO_MODE_REG_PUPD_OFFSET)

#define GPIO_MODE_REG_PID_OFFSET (0)
#define GPIO_MODE_REG_PID_MASK (0x3F << GPIO_MODE_REG_PID_OFFSET)
#define GPIO_MODE_REG_PID(n) ((n) << GPIO_MODE_REG_PID_OFFSET)

/* PID Function Codes */
#define GPIO_PID_GPIO 0
#define GPIO_PID_UART_RX 1
#define GPIO_PID_UART_TX 2
#define GPIO_PID_UART2_RX 3
#define GPIO_PID_UART2_TX 4
#define GPIO_PID_UART2_CTSN 5
#define GPIO_PID_UART2_RTSN 6
#define GPIO_PID_UART3_RX 7
#define GPIO_PID_UART3_TX 8
#define GPIO_PID_UART3_CTSN 9  /* Also ISO_RST */
#define GPIO_PID_UART3_RTSN 10 /* Also ISO_CARDINSERT */
#define GPIO_PID_ISO_CLK 11
#define GPIO_PID_ISO_DATA 12
#define GPIO_PID_SPI_DI 13
#define GPIO_PID_SPI_DO 14
#define GPIO_PID_SPI_CLK 15
#define GPIO_PID_SPI_EN 16
#define GPIO_PID_SPI_EN2 17
#define GPIO_PID_SPI2_DI 18
#define GPIO_PID_SPI2_DO 19
#define GPIO_PID_SPI2_CLK 20
#define GPIO_PID_SPI2_EN 21
#define GPIO_PID_SPI2_EN2 22
#define GPIO_PID_SPI3_EN 23
#define GPIO_PID_SPI3_EN2 24
#define GPIO_PID_I2C_SCL 25
#define GPIO_PID_I2C_SDA 26
#define GPIO_PID_I2C2_SCL 27
#define GPIO_PID_I2C2_SDA 28
#define GPIO_PID_I2C3_SCL 29
#define GPIO_PID_I2C3_SDA 30

#define GPIO_DATA_OFFSET(port) (0x0000 + (port) * 4)
#define GPIO_PADPWR_CTRL_OFFSET(port) (0x0160 + (port) * 4)
#define GPIO_RESET_DATA_OFFSET(port) (0x0018 + (port) * 4)
#define GPIO_SET_DATA_OFFSET(port) (0x000c + (port) * 4)
#define GPIO_WEAK_CTRL_OFFSET(port) (0x0170 + (port) * 4)

// /* Register adresses */
// #define PX_DATA_REG_ADDR(_port)         ((volatile uint32_t *)(GPIO_BASE +
// offsetof(GPIO_Type, P0_DATA_REG)) + _port) #define PX_DATA_REG(_port)
// *PX_DATA_REG_ADDR(_port) #define PX_SET_DATA_REG_ADDR(_port)     ((volatile
// uint32_t *)(GPIO_BASE + offsetof(GPIO_Type, P0_SET_DATA_REG)) + _port)
// #define PX_SET_DATA_REG(_port)          *PX_SET_DATA_REG_ADDR(_port)
// #define PX_RESET_DATA_REG_ADDR(_port)   ((volatile uint32_t *)(GPIO_BASE +
// offsetof(GPIO_Type, P0_RESET_DATA_REG)) + _port) #define
// PX_RESET_DATA_REG(_port)        *PX_RESET_DATA_REG_ADDR(_port)
// // #define PXX_MODE_REG_ADDR(_port, _pin)  ((volatile uint32_t *)(GPIO_BASE +
// offsetof(GPIO_Type, P0_00_MODE_REG)) + (_port * 32)  + _pin)
// // #define PXX_MODE_REG(_port, _pin)       *PXX_MODE_REG_ADDR(_port, _pin)
// #define PX_PADPWR_CTRL_REG_ADDR(_port)  ((volatile uint32_t *)(GPIO_BASE +
// offsetof(GPIO_Type, P0_PADPWR_CTRL_REG)) + _port) #define
// PX_PADPWR_CTRL_REG(_port)       *PX_PADPWR_CTRL_REG_ADDR(_port)

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_GPIO_H */
