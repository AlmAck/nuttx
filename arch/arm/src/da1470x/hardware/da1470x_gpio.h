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

#ifndef __ARCH_ARM_SRC_DA1470X_GPIO_H
#define __ARCH_ARM_SRC_DA1470X_GPIO_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdint.h>
#include "hardware/da1470x_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/


#define DA1470_GPIO_NPORTS           3

#define DA1470_GPIO_PORT0            0
#define DA1470_GPIO_PORT1            1
#define DA1470_GPIO_PORT2            2

#define DA1470_GPIO_NPINS            32
#define DA1470_GPIO_PORT2_NPINS      16

/* Register definitions */

/* Register offsets */
#define DA1470_GPIO_GPIO_CLK_SEL_OFFSET     0x016c
#define DA1470_GPIO_LCDC_MAP_CTRL_OFFSET    0x017c
#define DA1470_GPIO_PAD_DRIVE_CTRL_OFFSET   0x0180

#define DA1470_GPIO_MODE_OFFSET_P0(pin)       (0x0024 + (pin) * 4)
#define DA1470_GPIO_P0_DATA_OFFSET          0x0000
#define DA1470_GPIO_P0_PADPWR_CTRL_OFFSET   0x0160
#define DA1470_GPIO_P0_RESET_DATA_OFFSET    0x0018
#define DA1470_GPIO_P0_SET_DATA_OFFSET      0x000c
#define DA1470_GPIO_P0_WEAK_CTRL_OFFSET     0x0170

#define DA1470_GPIO_MODE_OFFSET_P1(pin)       (0x00a4 + (pin) * 4)
#define DA1470_GPIO_P1_DATA_OFFSET          0x0004
#define DA1470_GPIO_P1_PADPWR_CTRL_OFFSET   0x0164
#define DA1470_GPIO_P1_RESET_DATA_OFFSET    0x001c
#define DA1470_GPIO_P1_SET_DATA_OFFSET      0x0010
#define DA1470_GPIO_P1_WEAK_CTRL_OFFSET     0x0174

#define DA1470_GPIO_MODE_OFFSET_P2(pin)       (0x0124 + (pin) * 4)
#define DA1470_GPIO_P2_DATA_OFFSET          0x0008
#define DA1470_GPIO_P2_PADPWR_CTRL_OFFSET   0x0168
#define DA1470_GPIO_P2_RESET_DATA_OFFSET    0x0020
#define DA1470_GPIO_P2_SET_DATA_OFFSET      0x0014
#define DA1470_GPIO_P2_WEAK_CTRL_OFFSET     0x0178


#define DA1470_GPIO_MODE_OFFSET(port, pin)     (0x0024 + (port) * 0x0080 + (pin) * 4)
#define DA1470_GPIO_DATA_OFFSET(port)          (0x0000 + (port) * 4)
#define DA1470_GPIO_PADPWR_CTRL_OFFSET(port)   (0x0160 + (port) * 4)
#define DA1470_GPIO_RESET_DATA_OFFSET(port)    (0x0018 + (port) * 4)
#define DA1470_GPIO_SET_DATA_OFFSET(port)      (0x000c + (port) * 4)
#define DA1470_GPIO_WEAK_CTRL_OFFSET(port)     (0x0170 + (port) * 4)

/* Register addresses *******************************************************/

#define DA1470_GPIO_BASE                0x50050100

#define DA1470_GPIO_GPIO_CLK_SEL        (DA1470_GPIO_BASE + DA1470_GPIO_GPIO_CLK_SEL_OFFSET)
#define DA1470_GPIO_LCDC_MAP_CTRL       (DA1470_GPIO_BASE + DA1470_GPIO_LCDC_MAP_CTRL_OFFSET)
#define DA1470_GPIO_PAD_DRIVE_CTRL      (DA1470_GPIO_BASE + DA1470_GPIO_PAD_DRIVE_CTRL_OFFSET)

#define DA1470_GPIO_P0_MODE(n)          (DA1470_GPIO_BASE + DA1470_GPIO_MODE_OFFSET_P0(n))
#define DA1470_GPIO_P0_DATA             (DA1470_GPIO_BASE + DA1470_GPIO_P0_DATA_OFFSET)
#define DA1470_GPIO_P0_PADPWR_CTRL      (DA1470_GPIO_BASE + DA1470_GPIO_P0_PADPWR_CTRL_OFFSET)
#define DA1470_GPIO_P0_RESET_DATA       (DA1470_GPIO_BASE + DA1470_GPIO_P0_RESET_DATA_OFFSET)
#define DA1470_GPIO_P0_SET_DATA         (DA1470_GPIO_BASE + DA1470_GPIO_P0_SET_DATA_OFFSET)
#define DA1470_GPIO_P0_WEAK_CTRL        (DA1470_GPIO_BASE + DA1470_GPIO_P0_WEAK_CTRL_OFFSET)

#define DA1470_GPIO_P1_MODE(n)          (DA1470_GPIO_BASE + DA1470_GPIO_MODE_OFFSET_P1(n))
#define DA1470_GPIO_P1_DATA             (DA1470_GPIO_BASE + DA1470_GPIO_P1_DATA_OFFSET)
#define DA1470_GPIO_P1_PADPWR_CTRL      (DA1470_GPIO_BASE + DA1470_GPIO_P1_PADPWR_CTRL_OFFSET)
#define DA1470_GPIO_P1_RESET_DATA       (DA1470_GPIO_BASE + DA1470_GPIO_P1_RESET_DATA_OFFSET)
#define DA1470_GPIO_P1_SET_DATA         (DA1470_GPIO_BASE + DA1470_GPIO_P1_SET_DATA_OFFSET)
#define DA1470_GPIO_P1_WEAK_CTRL        (DA1470_GPIO_BASE + DA1470_GPIO_P1_WEAK_CTRL_OFFSET)

#define DA1470_GPIO_P2_MODE(n)          (DA1470_GPIO_BASE + DA1470_GPIO_MODE_OFFSET_P2(n))
#define DA1470_GPIO_P2_DATA             (DA1470_GPIO_BASE + DA1470_GPIO_P2_DATA_OFFSET)
#define DA1470_GPIO_P2_PADPWR_CTRL      (DA1470_GPIO_BASE + DA1470_GPIO_P2_PADPWR_CTRL_OFFSET)
#define DA1470_GPIO_P2_RESET_DATA       (DA1470_GPIO_BASE + DA1470_GPIO_P2_RESET_DATA_OFFSET)
#define DA1470_GPIO_P2_SET_DATA         (DA1470_GPIO_BASE + DA1470_GPIO_P2_SET_DATA_OFFSET)
#define DA1470_GPIO_P2_WEAK_CTRL        (DA1470_GPIO_BASE + DA1470_GPIO_P2_WEAK_CTRL_OFFSET)


// /* Register bit definitions *************************************************/

/* Generic GPIO Mode Register Macros for Any GPIO Pin */
/* Bit positions */
#define GPIO_MODE_REG_PPOD(pin)       ((pin) + 10)
#define GPIO_MODE_REG_PUPD(pin)       ((pin) + 8)
#define GPIO_MODE_REG_PID(pin)        ((pin))

/* Mask definitions */
#define GPIO_MODE_REG_PPOD_MASK(pin)      (0x01UL << GPIO_MODE_REG_PPOD(pin))
#define GPIO_MODE_REG_PUPD_MASK(pin)      (0x03UL << GPIO_MODE_REG_PUPD(pin))
#define GPIO_MODE_REG_PID_MASK(pin)       (0x3FUL << GPIO_MODE_REG_PID(pin))

/* Register adresses */
#define PX_DATA_REG_ADDR(_port)         ((volatile uint32_t *)(GPIO_BASE + offsetof(GPIO_Type, P0_DATA_REG)) + _port)
#define PX_DATA_REG(_port)              *PX_DATA_REG_ADDR(_port)
#define PX_SET_DATA_REG_ADDR(_port)     ((volatile uint32_t *)(GPIO_BASE + offsetof(GPIO_Type, P0_SET_DATA_REG)) + _port)
#define PX_SET_DATA_REG(_port)          *PX_SET_DATA_REG_ADDR(_port)
#define PX_RESET_DATA_REG_ADDR(_port)   ((volatile uint32_t *)(GPIO_BASE + offsetof(GPIO_Type, P0_RESET_DATA_REG)) + _port)
#define PX_RESET_DATA_REG(_port)        *PX_RESET_DATA_REG_ADDR(_port)
// #define PXX_MODE_REG_ADDR(_port, _pin)  ((volatile uint32_t *)(GPIO_BASE + offsetof(GPIO_Type, P0_00_MODE_REG)) + (_port * 32)  + _pin)
// #define PXX_MODE_REG(_port, _pin)       *PXX_MODE_REG_ADDR(_port, _pin)
#define PX_PADPWR_CTRL_REG_ADDR(_port)  ((volatile uint32_t *)(GPIO_BASE + offsetof(GPIO_Type, P0_PADPWR_CTRL_REG)) + _port)
#define PX_PADPWR_CTRL_REG(_port)       *PX_PADPWR_CTRL_REG_ADDR(_port)



// /* Register bit definitions *************************************************/

// #define GPIO_DETECTMODE_DEFAULT         (0)
// #define GPIO_DETECTMODE_LDETECT         (1)

#define GPIO_CNF_DIR                    (1 << 10) /* Bit 10: PPOD */
#define GPIO_CNF_PULL_SHIFT             (1 << 9) /* Bit 8-9: Pin PUPD */
#define GPIO_CNF_PULL_MASK              (0x3 << GPIO_CNF_PULL_SHIFT)
#  define GPIO_CNF_PULL_DISABLED        (0 << GPIO_CNF_PULL_SHIFT)
#  define GPIO_CNF_PULL_UP              (1 << GPIO_CNF_PULL_SHIFT)
#  define GPIO_CNF_PULL_DOWN            (2 << GPIO_CNF_PULL_SHIFT)
// #define GPIO_CNF_DRIVE_SHIFT            (8)
// #define GPIO_CNF_DRIVE_MASK             (0xf << GPIO_CNF_DRIVE_SHIFT)
// #  define GPIO_CNF_DRIVE_S0S1           (0 << GPIO_CNF_DRIVE_SHIFT)
// #  define GPIO_CNF_DRIVE_H0S1           (1 << GPIO_CNF_DRIVE_SHIFT)
// #  define GPIO_CNF_DRIVE_S0H1           (2 << GPIO_CNF_DRIVE_SHIFT)
// #  define GPIO_CNF_DRIVE_H0H1           (3 << GPIO_CNF_DRIVE_SHIFT)
// #  define GPIO_CNF_DRIVE_D0S1           (4 << GPIO_CNF_DRIVE_SHIFT)
// #  define GPIO_CNF_DRIVE_D0H1           (5 << GPIO_CNF_DRIVE_SHIFT)
// #  define GPIO_CNF_DRIVE_S0D1           (6 << GPIO_CNF_DRIVE_SHIFT)
// #  define GPIO_CNF_DRIVE_H0D1           (7 << GPIO_CNF_DRIVE_SHIFT)
// #  define GPIO_CNF_DRIVE_EOS1           (9 << GPIO_CNF_DRIVE_SHIFT)
// #  define GPIO_CNF_DRIVE_SOE1           (10 << GPIO_CNF_DRIVE_SHIFT)
// #  define GPIO_CNF_DRIVE_EOE1           (11 << GPIO_CNF_DRIVE_SHIFT)
// #  define GPIO_CNF_DRIVE_DOE1           (13 << GPIO_CNF_DRIVE_SHIFT)
// #  define GPIO_CNF_DRIVE_EOD1           (15 << GPIO_CNF_DRIVE_SHIFT)
// #define GPIO_CNF_SENSE_SHIFT            (16)
// #define GPIO_CNF_SENSE_MASK             (0x3 << GPIO_CNF_SENSE_SHIFT)
// #  define GPIO_CNF_SENSE_DISABLED       (0 << GPIO_CNF_SENSE_SHIFT)
// #  define GPIO_CNF_SENSE_HIGH           (2 << GPIO_CNF_SENSE_SHIFT)
// #  define GPIO_CNF_SENSE_LOW            (3 << GPIO_CNF_SENSE_SHIFT)
// #define GPIO_CNF_MCUSEL_SHIFT           (28)
// #define GPIO_CNF_MCUSEL_MASK            (0x7 << GPIO_CNF_MCUSEL_SHIFT)
// #  define GPIO_CNF_MCUSEL_CPUAPP        (0 << GPIO_CNF_MCUSEL_SHIFT)
// #  define GPIO_CNF_MCUSEL_CPUNET        (1 << GPIO_CNF_MCUSEL_SHIFT)
// #  define GPIO_CNF_MCUSEL_PERIP         (3 << GPIO_CNF_MCUSEL_SHIFT)
// #  define GPIO_CNF_MCUSEL_TND           (4 << GPIO_CNF_MCUSEL_SHIFT)

// #endif /* __ARCH_ARM_SRC_DA1470X_GPIO_H */
