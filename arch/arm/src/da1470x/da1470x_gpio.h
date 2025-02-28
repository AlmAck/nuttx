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

#ifndef __ASSEMBLY__
#  include <stdint.h>
#  include <stdbool.h>
#endif

#include <arch/da1470x/chip.h>
#include "hardware/da1470x_gpio.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/



// /* Pin configuration */
// #define GPIO_INPUT                 (0)    /* 0000 0000 */
// #define GPIO_PULLUP                (1 << 8)    /* 0001 0000 0000 */
// #define GPIO_PULLDOWN              (2 << 8)    /* 0010 0000 0000 */
// #define GPIO_OUTPUT                (3 << 8)    /* 0011 0000 0000 */
// #define GPIO_OUTPUT_PUSHPULL       GPIO_OUTPUT /* Explicit push-pull is the same as basic output */
// #define GPIO_OUTPUT_OPENDRAIN      (7 << 8)    /* 0111 0000 0000 */
// #define GPIO_INVALID               (0xFFF)  /* 1111 1111 1111 */

/* Macro to define GPIO pin configuration */
#define GPIO_PIN_CONFIG(mode, port, pin) (((mode) & 0xFFF) | ((port) << 12) | ((pin) << 16))

/* Usage Example */
#define GPIO_P0_PIN0_INPUT               GPIO_PIN_CONFIG(GPIO_INPUT, 0, 0)
#define GPIO_P0_PIN1_OUTPUT              GPIO_PIN_CONFIG(GPIO_OUTPUT, 0, 1)
#define GPIO_P1_PIN2_PULLUP              GPIO_PIN_CONFIG(GPIO_PULLUP, 1, 2)
#define GPIO_P1_PIN3_OPENDRAIN           GPIO_PIN_CONFIG(GPIO_OUTPUT_OPENDRAIN, 1, 3)


/* GPIO Power Source Configuration */
#define GPIO_POWER_V33      (0)  /* 3.3 V power rail */
#define GPIO_POWER_VDD1V8P  (1)  /* 1.8 V power rail */
#define GPIO_POWER_NONE     (2)  /* Invalid power rail */

/* Macro to define GPIO power setting */
#define GPIO_POWER_CONFIG(power) ((power) << 2)


/* GPIO Port Configuration */
#define GPIO_PORT_0         (0)  /* GPIO Port 0 */
#define GPIO_PORT_1         (1)  /* GPIO Port 1 */
#define GPIO_PORT_2         (2)  /* GPIO Port 2 */
#define GPIO_PORT_MAX       (3)  /* Maximum port number for error checking */

/* Bit-encoded input to da1470x_gpio_config() *********************************/

/* 32-Bit Encoding: .... .... ...G GDDF  FFFF FWMM VPPN NNNN
 *
 *   MCU selection:         GG
 *   Pin Direction:         DD
 *   Pin Function:          FFFFFF
 *   Pin Mode bits:         MM
 *   Initial value:         V (output pins)
 *   Port number:           P (0-1)
 *   Pin number:            NNNNN (0-31)
 */

/* MCU selection bits:
 *
 * .... .... ...G G... .... .... .... ....
 * 1098 7654 3210 9876 5432 1098 7654 3210
 */

// #define GPIO_MCUSEL_SHIFT       (19)    /* Bits 19-20: MCUSEL mode */
// #define GPIO_MCUSEL_MASK        (0x03 << GPIO_MCUSEL_SHIFT)
// #  define GPIO_MCUSEL_APP       (0x00 << GPIO_MCUSEL_SHIFT)  /* 00000 CPU APP */
// #  define GPIO_MCUSEL_NET       (0x01 << GPIO_MCUSEL_SHIFT)  /* 00001 CPU NET */
// #  define GPIO_MCUSEL_PERIP     (0x02 << GPIO_MCUSEL_SHIFT)  /* 00002 Periphneral */
// #  define GPIO_MCUSEL_TND       (0x03 << GPIO_MCUSEL_SHIFT)  /* 00003 Trace and Debug System */

/* Pin Direction bits:
 * Only meaningful when the GPIO function is GPIO_PIN
 *
 * .... .... .... .DD. .... .... .... ....
 * 1098 7654 3210 9876 5432 1098 7654 3210
 */

#define GPIO_DIR_SHIFT         (17)    /* Bits 17-18: GPIO mode */
#define GPIO_DIR_MASK          (0x03 << GPIO_DIR_SHIFT)
#  define GPIO_INPUT           (0x00 << GPIO_DIR_SHIFT)  /* 00000 GPIO input pin */
#  define GPIO_OUTPUT          (0x01 << GPIO_DIR_SHIFT)  /* 00001 GPIO output pin */


/* Pin Function bits:
 *
 * .... .... .... ...F FFFF F... .... ....
 * 1098 7654 3210 9876 5432 1098 7654 3210
 */

/* Shift and Mask Definitions */
#define GPIO_FUNC_SHIFT                 (11)                     /* Bits 11-16: GPIO function selection */
#define GPIO_FUNC_MASK                  (0x3F << GPIO_FUNC_SHIFT) /* Mask for GPIO function bits */
#  define GPIO_FUNC_GPIO                (0  << GPIO_FUNC_SHIFT)  /* General Purpose Input/Output */
#  define GPIO_FUNC_UART_RX             (1  << GPIO_FUNC_SHIFT)  /* UART Receive */
#  define GPIO_FUNC_UART_TX             (2  << GPIO_FUNC_SHIFT)  /* UART Transmit */
#  define GPIO_FUNC_UART2_RX            (3  << GPIO_FUNC_SHIFT)  /* UART2 Receive */
#  define GPIO_FUNC_UART2_TX            (4  << GPIO_FUNC_SHIFT)  /* UART2 Transmit */
#  define GPIO_FUNC_UART2_CTSN          (5  << GPIO_FUNC_SHIFT)  /* UART2 Clear To Send (CTSN) */
#  define GPIO_FUNC_UART2_RTSN          (6  << GPIO_FUNC_SHIFT)  /* UART2 Request To Send (RTSN) */
#  define GPIO_FUNC_UART3_RX            (7  << GPIO_FUNC_SHIFT)  /* UART3 Receive */
#  define GPIO_FUNC_UART3_TX            (8  << GPIO_FUNC_SHIFT)  /* UART3 Transmit */
#  define GPIO_FUNC_UART3_CTSN          (9  << GPIO_FUNC_SHIFT)  /* UART3 Clear To Send (CTSN) */
#  define GPIO_FUNC_ISO_RST             (9  << GPIO_FUNC_SHIFT)  /* ISO Reset (Shared with UART3_CTSN) */
#  define GPIO_FUNC_UART3_RTSN          (10 << GPIO_FUNC_SHIFT)  /* UART3 Request To Send (RTSN) */
#  define GPIO_FUNC_ISO_CARDINSERT      (10 << GPIO_FUNC_SHIFT)  /* ISO Card Insert (Shared with UART3_RTSN) */
#  define GPIO_FUNC_ISO_CLK             (11 << GPIO_FUNC_SHIFT)  /* ISO Clock */
#  define GPIO_FUNC_ISO_DATA            (12 << GPIO_FUNC_SHIFT)  /* ISO Data */
#  define GPIO_FUNC_SPI_DI              (13 << GPIO_FUNC_SHIFT)  /* SPI Data In */
#  define GPIO_FUNC_SPI_DO              (14 << GPIO_FUNC_SHIFT)  /* SPI Data Out */
#  define GPIO_FUNC_SPI_CLK             (15 << GPIO_FUNC_SHIFT)  /* SPI Clock */
#  define GPIO_FUNC_SPI_EN              (16 << GPIO_FUNC_SHIFT)  /* SPI Enable */
#  define GPIO_FUNC_SPI_EN2             (17 << GPIO_FUNC_SHIFT)  /* SPI Enable 2 */
#  define GPIO_FUNC_SPI2_DI             (18 << GPIO_FUNC_SHIFT)  /* SPI2 Data In */
#  define GPIO_FUNC_SPI2_DO             (19 << GPIO_FUNC_SHIFT)  /* SPI2 Data Out */
#  define GPIO_FUNC_SPI2_CLK            (20 << GPIO_FUNC_SHIFT)  /* SPI2 Clock */
#  define GPIO_FUNC_SPI2_EN             (21 << GPIO_FUNC_SHIFT)  /* SPI2 Enable */
#  define GPIO_FUNC_SPI2_EN2            (22 << GPIO_FUNC_SHIFT)  /* SPI2 Enable 2 */
#  define GPIO_FUNC_SPI3_EN             (23 << GPIO_FUNC_SHIFT)  /* SPI3 Enable */
#  define GPIO_FUNC_SPI3_EN2            (24 << GPIO_FUNC_SHIFT)  /* SPI3 Enable 2 */
#  define GPIO_FUNC_I2C_SCL             (25 << GPIO_FUNC_SHIFT)  /* I2C Clock Line */
#  define GPIO_FUNC_I2C_SDA             (26 << GPIO_FUNC_SHIFT)  /* I2C Data Line */
#  define GPIO_FUNC_I2C2_SCL            (27 << GPIO_FUNC_SHIFT)  /* I2C2 Clock Line */
#  define GPIO_FUNC_I2C2_SDA            (28 << GPIO_FUNC_SHIFT)  /* I2C2 Data Line */
#  define GPIO_FUNC_I2C3_SCL            (29 << GPIO_FUNC_SHIFT)  /* I2C3 Clock Line */
#  define GPIO_FUNC_I2C3_SDA            (30 << GPIO_FUNC_SHIFT)  /* I2C3 Data Line */
#  define GPIO_FUNC_I3C_SCL             (31 << GPIO_FUNC_SHIFT)  /* I3C Clock Line */
#  define GPIO_FUNC_I3C_SDA             (32 << GPIO_FUNC_SHIFT)  /* I3C Data Line */
#  define GPIO_FUNC_USB_SOF             (33 << GPIO_FUNC_SHIFT)  /* USB Start of Frame */
#  define GPIO_FUNC_ADC                 (34 << GPIO_FUNC_SHIFT)  /* Analog-to-Digital Converter */
#  define GPIO_FUNC_USB                 (35 << GPIO_FUNC_SHIFT)  /* USB Function */
#  define GPIO_FUNC_PCM_DI              (36 << GPIO_FUNC_SHIFT)  /* PCM Data In */
#  define GPIO_FUNC_PCM_DO              (37 << GPIO_FUNC_SHIFT)  /* PCM Data Out */
#  define GPIO_FUNC_PCM_FSC             (38 << GPIO_FUNC_SHIFT)  /* PCM Frame Sync Clock */
#  define GPIO_FUNC_PCM_CLK             (39 << GPIO_FUNC_SHIFT)  /* PCM Clock */
#  define GPIO_FUNC_PDM_DATA            (40 << GPIO_FUNC_SHIFT)  /* PDM Data */
#  define GPIO_FUNC_PDM_CLK             (41 << GPIO_FUNC_SHIFT)  /* PDM Clock */
#  define GPIO_FUNC_COEX_EXT_ACT        (42 << GPIO_FUNC_SHIFT)  /* Coexistence External Activity */
#  define GPIO_FUNC_COEX_SMART_ACT      (43 << GPIO_FUNC_SHIFT)  /* Coexistence Smart Activity */
#  define GPIO_FUNC_COEX_SMART_PRI      (44 << GPIO_FUNC_SHIFT)  /* Coexistence Smart Priority */
#  define GPIO_FUNC_PORT0_DCF           (45 << GPIO_FUNC_SHIFT)  /* Port0 DCF */
#  define GPIO_FUNC_PORT1_DCF           (46 << GPIO_FUNC_SHIFT)  /* Port1 DCF */
#  define GPIO_FUNC_PORT2_DCF           (47 << GPIO_FUNC_SHIFT)  /* Port2 DCF */
#  define GPIO_FUNC_PORT3_DCF           (48 << GPIO_FUNC_SHIFT)  /* Port3 DCF */
#  define GPIO_FUNC_PORT4_DCF           (49 << GPIO_FUNC_SHIFT)  /* Port4 DCF */
#  define GPIO_FUNC_CLOCK               (50 << GPIO_FUNC_SHIFT)  /* Clock Function */
#  define GPIO_FUNC_TIM_PWM             (51 << GPIO_FUNC_SHIFT)  /* Timer PWM */
#  define GPIO_FUNC_TIM2_PWM            (52 << GPIO_FUNC_SHIFT)  /* Timer2 PWM */
#  define GPIO_FUNC_TIM3_PWM            (53 << GPIO_FUNC_SHIFT)  /* Timer3 PWM */
#  define GPIO_FUNC_TIM4_PWM            (54 << GPIO_FUNC_SHIFT)  /* Timer4 PWM */
#  define GPIO_FUNC_TIM5_PWM            (55 << GPIO_FUNC_SHIFT)  /* Timer5 PWM */
#  define GPIO_FUNC_TIM6_PWM            (56 << GPIO_FUNC_SHIFT)  /* Timer6 PWM */
#  define GPIO_FUNC_TIM_1SHOT           (57 << GPIO_FUNC_SHIFT)  /* Timer 1-Shot */
#  define GPIO_FUNC_TIM2_1SHOT          (58 << GPIO_FUNC_SHIFT)  /* Timer2 1-Shot */
#  define GPIO_FUNC_TIM3_1SHOT          (59 << GPIO_FUNC_SHIFT)  /* Timer3 1-Shot */
#  define GPIO_FUNC_TIM4_1SHOT          (60 << GPIO_FUNC_SHIFT)  /* Timer4 1-Shot */
#  define GPIO_FUNC_TIM5_1SHOT          (61 << GPIO_FUNC_SHIFT)  /* Timer5 1-Shot */
#  define GPIO_FUNC_TIM6_1SHOT          (62 << GPIO_FUNC_SHIFT)  /* Timer6 1-Shot */
#  define GPIO_FUNC_CMAC_DIAG           (63 << GPIO_FUNC_SHIFT)  /* CMAC Diagnostic */

/* Port number: W
 *
 * .... .... .... .... .... .W.. .... ....
 * 1098 7654 3210 9876 5432 1098 7654 3210
 */

#define GPIO_PADPWR_SHIFT       (10)       /* Bit 10:  Output Power Control Register */
#define GPIO_PADPWR_MASK        (0x1 << GPIO_PORT_SHIFT)
#  define GPIO_V30              (0 << GPIO_PORT_SHIFT)
#  define GPIO_1V8P             (1 << GPIO_PORT_SHIFT)

/* Pin Mode: MM
 *
 * .... .... .... .... .... ..MM .... ....
 * 1098 7654 3210 9876 5432 1098 7654 3210
 */

#define GPIO_MODE_SHIFT         (8)      /* Bits 8-9: Pin pull-up mode */
#define GPIO_MODE_MASK          (0x3 << GPIO_MODE_SHIFT)
#  define GPIO_FLOAT            (0 << GPIO_MODE_SHIFT) /* Neither pull-up nor -down */
#  define GPIO_PULLDOWN         (1 << GPIO_MODE_SHIFT) /* Pull-down resistor enabled */
#  define GPIO_PULLUP           (2 << GPIO_MODE_SHIFT) /* Pull-up resistor enabled */

/* Initial value: V
 *
 * .... .... .... .... .... .... V... ....
 * 1098 7654 3210 9876 5432 1098 7654 3210
 */

#define GPIO_VALUE_SHIFT        (7)      /* Bit 7: Initial GPIO output value */
#define GPIO_VALUE              (1 << GPIO_VALUE_SHIFT)  
#  define GPIO_VALUE_ONE        GPIO_VALUE
#  define GPIO_VALUE_ZERO       (0)

/* Port number: PPP (0-5)
 *
 * .... .... .... .... .... .... .PP. ....
 * 1098 7654 3210 9876 5432 1098 7654 3210
 */

#define GPIO_PORT_SHIFT         (5)       /* Bit 5-6:  Port number */
#define GPIO_PORT_MASK          (0x3 << GPIO_PORT_SHIFT)
#  define GPIO_PORT0            (0 << GPIO_PORT_SHIFT)
#  define GPIO_PORT1            (1 << GPIO_PORT_SHIFT)
#  define GPIO_PORT2            (2 << GPIO_PORT_SHIFT)

/* Pin number: NNNNN (0-31)
 *
 * .... .... .... .... .... .... ...N NNNN
 * 1098 7654 3210 9876 5432 1098 7654 3210
 */

#define GPIO_PIN_SHIFT          0         /* Bits 0-4: GPIO number: 0-31 */
#define GPIO_PIN_MASK           (0x1f << GPIO_PIN_SHIFT)
#  define GPIO_PIN0             (0  << GPIO_PIN_SHIFT)
#  define GPIO_PIN1             (1  << GPIO_PIN_SHIFT)
#  define GPIO_PIN2             (2  << GPIO_PIN_SHIFT)
#  define GPIO_PIN3             (3  << GPIO_PIN_SHIFT)
#  define GPIO_PIN4             (4  << GPIO_PIN_SHIFT)
#  define GPIO_PIN5             (5  << GPIO_PIN_SHIFT)
#  define GPIO_PIN6             (6  << GPIO_PIN_SHIFT)
#  define GPIO_PIN7             (7  << GPIO_PIN_SHIFT)
#  define GPIO_PIN8             (8  << GPIO_PIN_SHIFT)
#  define GPIO_PIN9             (9  << GPIO_PIN_SHIFT)
#  define GPIO_PIN10            (10 << GPIO_PIN_SHIFT)
#  define GPIO_PIN11            (11 << GPIO_PIN_SHIFT)
#  define GPIO_PIN12            (12 << GPIO_PIN_SHIFT)
#  define GPIO_PIN13            (13 << GPIO_PIN_SHIFT)
#  define GPIO_PIN14            (14 << GPIO_PIN_SHIFT)
#  define GPIO_PIN15            (15 << GPIO_PIN_SHIFT)
#  define GPIO_PIN16            (16 << GPIO_PIN_SHIFT)
#  define GPIO_PIN17            (17 << GPIO_PIN_SHIFT)
#  define GPIO_PIN18            (18 << GPIO_PIN_SHIFT)
#  define GPIO_PIN19            (19 << GPIO_PIN_SHIFT)
#  define GPIO_PIN20            (20 << GPIO_PIN_SHIFT)
#  define GPIO_PIN21            (21 << GPIO_PIN_SHIFT)
#  define GPIO_PIN22            (22 << GPIO_PIN_SHIFT)
#  define GPIO_PIN23            (23 << GPIO_PIN_SHIFT)
#  define GPIO_PIN24            (24 << GPIO_PIN_SHIFT)
#  define GPIO_PIN25            (25 << GPIO_PIN_SHIFT)
#  define GPIO_PIN26            (26 << GPIO_PIN_SHIFT)
#  define GPIO_PIN27            (27 << GPIO_PIN_SHIFT)
#  define GPIO_PIN28            (28 << GPIO_PIN_SHIFT)
#  define GPIO_PIN29            (29 << GPIO_PIN_SHIFT)
#  define GPIO_PIN30            (30 << GPIO_PIN_SHIFT)
#  define GPIO_PIN31            (31 << GPIO_PIN_SHIFT)
#  define GPIO_PIN(n)           ((n) << GPIO_PIN_SHIFT)

/* Helper macros */

#define GPIO_PIN_DECODE(p)  (((p) & GPIO_PIN_MASK)  >> GPIO_PIN_SHIFT)
#define GPIO_PORT_DECODE(p) (((p) & GPIO_PORT_MASK) >> GPIO_PORT_SHIFT)

/****************************************************************************
 * Public Types
 ****************************************************************************/

typedef uint32_t da1470x_pinset_t;

enum da1470x_gpio_detectmode_e
{
  da1470x_GPIO_DETECTMODE_DETECT,
  da1470x_GPIO_DETECTMODE_LDETECT,
};

/****************************************************************************
 * Public Data
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
 * Public Function Prototypes
 ****************************************************************************/

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
 *   Unconfigure a GPIO pin based on bit-encoded description of the pin.
 *
 ****************************************************************************/

int da1470x_gpio_unconfig(da1470x_pinset_t cfgset);

/****************************************************************************
 * Name: rnf52_gpio_write
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
 * Function:  nf52_gpio_dump
 *
 * Description:
 *   Dump all GPIO registers associated with the base address of the provided
 * pinset.
 *
 ****************************************************************************/

#ifdef CONFIG_DEBUG_GPIO_INFO
int da1470x_gpio_dump(da1470x_pinset_t pinset, const char *msg);
#else
#  define da1470x_gpio_dump(p,m)
#endif

#ifdef CONFIG_da1470x_APPCORE
/****************************************************************************
 * Name: da1470x_gpio_cpunet_allow
 *
 * Description:
 *  Allow GPIO to be used by the net core.
 *  Can be used only with te app core.
 *
 ****************************************************************************/

void da1470x_gpio_cpunet_allow(uint32_t gpio);

/****************************************************************************
 * Name: da1470x_gpio_cpunet_allow_all
 *
 * Description:
 *  Allow all GPIO to be used by the net core.
 *  This can be overwritten by the app core.
 *
 ****************************************************************************/

void da1470x_gpio_cpunet_allow_all(void);
#endif

#ifdef __cplusplus
}
#endif
#endif /* __ASSEMBLY__ */

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_GPIO_H */
