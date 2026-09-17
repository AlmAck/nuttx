/****************************************************************************
 * arch/arm/src/da1470x/da1470x_gpio.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_GPIO_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_GPIO_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/irq.h>

#ifndef __ASSEMBLY__
#  include <stdint.h>
#  include <stdbool.h>
#endif

#include "hardware/da1470x_gpio.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Bit-encoded input to da1470x_gpio_config()
 *
 * 32-bit encoding: .... .... .... ODDF FFFF FWMM VPPN NNNN
 *
 *   Open drain:      O (bit 19)
 *   Pin direction:   DD (bits 17-18)
 *   Pin function:    FFFFFF (bits 11-16, peripheral ID)
 *   Pad power:       W (bit 10)
 *   Pull mode:       MM (bits 8-9)
 *   Initial value:   V (bit 7, output pins)
 *   Port number:     PP (bits 5-6)
 *   Pin number:      NNNNN (bits 0-4)
 */

/* Open drain output */

#define GPIO_OPENDRAIN          (1 << 19)

/* Pin direction */

#define GPIO_DIR_SHIFT          (17)
#define GPIO_DIR_MASK           (0x03 << GPIO_DIR_SHIFT)
#  define GPIO_INPUT            (0x00 << GPIO_DIR_SHIFT)
#  define GPIO_OUTPUT           (0x01 << GPIO_DIR_SHIFT)

/* Pin function (peripheral ID, datasheet Pxy_MODE_REG.PID) */

#define GPIO_FUNC_SHIFT         (11)
#define GPIO_FUNC_MASK          (0x3f << GPIO_FUNC_SHIFT)
#  define GPIO_FUNC(n)          ((n) << GPIO_FUNC_SHIFT)
#  define GPIO_FUNC_GPIO        GPIO_FUNC(0)
#  define GPIO_FUNC_UART_RX     GPIO_FUNC(1)
#  define GPIO_FUNC_UART_TX     GPIO_FUNC(2)
#  define GPIO_FUNC_UART2_RX    GPIO_FUNC(3)
#  define GPIO_FUNC_UART2_TX    GPIO_FUNC(4)
#  define GPIO_FUNC_UART2_CTSN  GPIO_FUNC(5)
#  define GPIO_FUNC_UART2_RTSN  GPIO_FUNC(6)
#  define GPIO_FUNC_UART3_RX    GPIO_FUNC(7)
#  define GPIO_FUNC_UART3_TX    GPIO_FUNC(8)
#  define GPIO_FUNC_UART3_CTSN  GPIO_FUNC(9)
#  define GPIO_FUNC_ISO_RST     GPIO_FUNC(9)
#  define GPIO_FUNC_UART3_RTSN  GPIO_FUNC(10)
#  define GPIO_FUNC_ISO_CARDINSERT GPIO_FUNC(10)
#  define GPIO_FUNC_ISO_CLK     GPIO_FUNC(11)
#  define GPIO_FUNC_ISO_DATA    GPIO_FUNC(12)
#  define GPIO_FUNC_SPI_DI      GPIO_FUNC(13)
#  define GPIO_FUNC_SPI_DO      GPIO_FUNC(14)
#  define GPIO_FUNC_SPI_CLK     GPIO_FUNC(15)
#  define GPIO_FUNC_SPI_EN      GPIO_FUNC(16)
#  define GPIO_FUNC_SPI_EN2     GPIO_FUNC(17)
#  define GPIO_FUNC_SPI2_DI     GPIO_FUNC(18)
#  define GPIO_FUNC_SPI2_DO     GPIO_FUNC(19)
#  define GPIO_FUNC_SPI2_CLK    GPIO_FUNC(20)
#  define GPIO_FUNC_SPI2_EN     GPIO_FUNC(21)
#  define GPIO_FUNC_SPI2_EN2    GPIO_FUNC(22)
#  define GPIO_FUNC_SPI3_EN     GPIO_FUNC(23)
#  define GPIO_FUNC_SPI3_EN2    GPIO_FUNC(24)
#  define GPIO_FUNC_I2C_SCL     GPIO_FUNC(25)
#  define GPIO_FUNC_I2C_SDA     GPIO_FUNC(26)
#  define GPIO_FUNC_I2C2_SCL    GPIO_FUNC(27)
#  define GPIO_FUNC_I2C2_SDA    GPIO_FUNC(28)
#  define GPIO_FUNC_I2C3_SCL    GPIO_FUNC(29)
#  define GPIO_FUNC_I2C3_SDA    GPIO_FUNC(30)
#  define GPIO_FUNC_I3C_SCL     GPIO_FUNC(31)
#  define GPIO_FUNC_I3C_SDA     GPIO_FUNC(32)
#  define GPIO_FUNC_USB_SOF     GPIO_FUNC(33)
#  define GPIO_FUNC_ADC         GPIO_FUNC(34)
#  define GPIO_FUNC_USB         GPIO_FUNC(35)
#  define GPIO_FUNC_PCM_DI      GPIO_FUNC(36)
#  define GPIO_FUNC_PCM_DO      GPIO_FUNC(37)
#  define GPIO_FUNC_PCM_FSC     GPIO_FUNC(38)
#  define GPIO_FUNC_PCM_CLK     GPIO_FUNC(39)
#  define GPIO_FUNC_PDM_DATA    GPIO_FUNC(40)
#  define GPIO_FUNC_PDM_CLK     GPIO_FUNC(41)
#  define GPIO_FUNC_COEX_EXT_ACT   GPIO_FUNC(42)
#  define GPIO_FUNC_COEX_SMART_ACT GPIO_FUNC(43)
#  define GPIO_FUNC_COEX_SMART_PRI GPIO_FUNC(44)
#  define GPIO_FUNC_PORT0_DCF   GPIO_FUNC(45)
#  define GPIO_FUNC_PORT1_DCF   GPIO_FUNC(46)
#  define GPIO_FUNC_PORT2_DCF   GPIO_FUNC(47)
#  define GPIO_FUNC_PORT3_DCF   GPIO_FUNC(48)
#  define GPIO_FUNC_PORT4_DCF   GPIO_FUNC(49)
#  define GPIO_FUNC_CLOCK       GPIO_FUNC(50)
#  define GPIO_FUNC_TIM_PWM     GPIO_FUNC(51)
#  define GPIO_FUNC_TIM2_PWM    GPIO_FUNC(52)
#  define GPIO_FUNC_TIM3_PWM    GPIO_FUNC(53)
#  define GPIO_FUNC_TIM4_PWM    GPIO_FUNC(54)
#  define GPIO_FUNC_TIM5_PWM    GPIO_FUNC(55)
#  define GPIO_FUNC_TIM6_PWM    GPIO_FUNC(56)
#  define GPIO_FUNC_TIM_1SHOT   GPIO_FUNC(57)
#  define GPIO_FUNC_TIM2_1SHOT  GPIO_FUNC(58)
#  define GPIO_FUNC_TIM3_1SHOT  GPIO_FUNC(59)
#  define GPIO_FUNC_TIM4_1SHOT  GPIO_FUNC(60)
#  define GPIO_FUNC_TIM5_1SHOT  GPIO_FUNC(61)
#  define GPIO_FUNC_TIM6_1SHOT  GPIO_FUNC(62)
#  define GPIO_FUNC_CMAC_DIAG   GPIO_FUNC(63)

/* Pad power rail */

#define GPIO_PADPWR_SHIFT       (10)
#define GPIO_PADPWR_MASK        (0x1 << GPIO_PADPWR_SHIFT)
#  define GPIO_V30              (0 << GPIO_PADPWR_SHIFT)
#  define GPIO_V18P             (1 << GPIO_PADPWR_SHIFT)

/* Pull mode (inputs only) */

#define GPIO_MODE_SHIFT         (8)
#define GPIO_MODE_MASK          (0x3 << GPIO_MODE_SHIFT)
#  define GPIO_FLOAT            (0 << GPIO_MODE_SHIFT)
#  define GPIO_PULLDOWN         (1 << GPIO_MODE_SHIFT)
#  define GPIO_PULLUP           (2 << GPIO_MODE_SHIFT)

/* Initial value (outputs only) */

#define GPIO_VALUE_SHIFT        (7)
#define GPIO_VALUE              (1 << GPIO_VALUE_SHIFT)
#  define GPIO_VALUE_ONE        GPIO_VALUE
#  define GPIO_VALUE_ZERO       (0)

/* Port number */

#define GPIO_PORT_SHIFT         (5)
#define GPIO_PORT_MASK          (0x3 << GPIO_PORT_SHIFT)
#  define GPIO_PORT0            (0 << GPIO_PORT_SHIFT)
#  define GPIO_PORT1            (1 << GPIO_PORT_SHIFT)
#  define GPIO_PORT2            (2 << GPIO_PORT_SHIFT)

/* Pin number */

#define GPIO_PIN_SHIFT          (0)
#define GPIO_PIN_MASK           (0x1f << GPIO_PIN_SHIFT)
#  define GPIO_PIN(n)           ((n) << GPIO_PIN_SHIFT)

/* Decode helpers */

#define GPIO_PIN_DECODE(p)      (((p) & GPIO_PIN_MASK) >> GPIO_PIN_SHIFT)
#define GPIO_PORT_DECODE(p)     (((p) & GPIO_PORT_MASK) >> GPIO_PORT_SHIFT)
#define GPIO_FUNC_DECODE(p)     (((p) & GPIO_FUNC_MASK) >> GPIO_FUNC_SHIFT)

/****************************************************************************
 * Public Types
 ****************************************************************************/

typedef uint32_t da1470x_pinset_t;

enum da1470x_gpio_edge_e
{
  DA1470X_GPIO_EDGE_RISING = 0,
  DA1470X_GPIO_EDGE_FALLING
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifndef __ASSEMBLY__

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Name: da1470x_gpio_config
 *
 * Description:
 *   Configure a GPIO pin based on bit-encoded description of the pin.
 *
 ****************************************************************************/

int da1470x_gpio_config(da1470x_pinset_t cfgset);

/****************************************************************************
 * Name: da1470x_gpio_unconfig
 *
 * Description:
 *   Return a pin to its reset state (GPIO input, no pull).
 *
 ****************************************************************************/

int da1470x_gpio_unconfig(da1470x_pinset_t cfgset);

/****************************************************************************
 * Name: da1470x_gpio_write
 *
 * Description:
 *   Write one or zero to the selected GPIO pin
 *
 ****************************************************************************/

void da1470x_gpio_write(da1470x_pinset_t pinset, bool value);

/****************************************************************************
 * Name: da1470x_gpio_read
 *
 * Description:
 *   Read one or zero from the selected GPIO pin
 *
 ****************************************************************************/

bool da1470x_gpio_read(da1470x_pinset_t pinset);

/****************************************************************************
 * Name: da1470x_gpio_latch_enable / da1470x_gpio_latch_disable
 *
 * Description:
 *   Open or close the pad latch of a port.  While the latch is closed the
 *   pads keep their state across a PD_COM power down and Pxy_MODE writes
 *   are not applied.
 *
 ****************************************************************************/

void da1470x_gpio_latch_enable(int port);
void da1470x_gpio_latch_disable(int port);

/****************************************************************************
 * Name: da1470x_gpio_dump
 *
 * Description:
 *   Dump all GPIO registers associated with the provided pinset.
 *
 ****************************************************************************/

#ifdef CONFIG_DEBUG_GPIO_INFO
int da1470x_gpio_dump(da1470x_pinset_t pinset, const char *msg);
#else
#  define da1470x_gpio_dump(p,m)
#endif

#ifdef CONFIG_DA1470X_GPIO_IRQ

/****************************************************************************
 * Name: da1470x_gpioirq_initialize
 *
 * Description:
 *   Reset the WKUP per-port selection state and attach the three per-port
 *   NVIC interrupts.
 *
 ****************************************************************************/

void da1470x_gpioirq_initialize(void);

/****************************************************************************
 * Name: da1470x_gpioirq_attach
 *
 * Description:
 *   Register a handler for edge events on the pin encoded in pinset.
 *   Pass a NULL handler to detach.  The handler receives the encoded
 *   pinset as the irq argument.
 *
 ****************************************************************************/

int da1470x_gpioirq_attach(da1470x_pinset_t pinset,
                           enum da1470x_gpio_edge_e edge,
                           xcpt_t handler, void *arg);

/****************************************************************************
 * Name: da1470x_gpioirq_enable / da1470x_gpioirq_disable
 *
 * Description:
 *   Unmask or mask the event source of a pin.
 *
 ****************************************************************************/

void da1470x_gpioirq_enable(da1470x_pinset_t pinset);
void da1470x_gpioirq_disable(da1470x_pinset_t pinset);

#endif /* CONFIG_DA1470X_GPIO_IRQ */

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_GPIO_H */
