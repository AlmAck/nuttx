/****************************************************************************
 * boards/arm/da1470x/da14706-00hzdevkt-p/src/da14706-00hzdevkt-p.h
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

#ifndef __BOARDS_ARM_DA1470X_DA14706_00HZDEVKT_P_SRC_DA14706_00HZDEVKT_P_H
#define __BOARDS_ARM_DA1470X_DA14706_00HZDEVKT_P_SRC_DA14706_00HZDEVKT_P_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/compiler.h>

#include "da1470x_gpio.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* LED definitions **********************************************************/

/* LED2 (P1.1) is the touch controller's reset line when the display
 * board is fitted; with the touch driver enabled the LED set shrinks to
 * LED1 and LED3 so the kernel's status LEDs cannot hold the controller
 * in reset.
 */

#define GPIO_LED1  (GPIO_OUTPUT | GPIO_VALUE_ONE | GPIO_PORT0 | GPIO_PIN(31))
#ifdef CONFIG_DA14706_TOUCH_ZT2628
#  define GPIO_LED2 (GPIO_OUTPUT | GPIO_VALUE_ONE | GPIO_PORT1 | GPIO_PIN(2))
#else
#  define GPIO_LED2 (GPIO_OUTPUT | GPIO_VALUE_ONE | GPIO_PORT1 | GPIO_PIN(1))
#  define GPIO_LED3 (GPIO_OUTPUT | GPIO_VALUE_ONE | GPIO_PORT1 | GPIO_PIN(2))
#endif

/* Button definitions *******************************************************/

#define GPIO_BUTTON1 (GPIO_INPUT | GPIO_PULLUP | GPIO_PORT1 | GPIO_PIN(22))
#define GPIO_BUTTON2 (GPIO_INPUT | GPIO_PULLUP | GPIO_PORT1 | GPIO_PIN(23))

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifndef __ASSEMBLY__

/****************************************************************************
 * Name: da1470x_bringup
 *
 * Description:
 *   Perform architecture-specific initialization
 *
 *   CONFIG_BOARD_LATE_INITIALIZE=y :
 *     Called from board_late_initialize().
 *
 *   CONFIG_BOARD_LATE_INITIALIZE=n && CONFIG_BOARDCTL=y :
 *     Called from the NSH library
 *
 ****************************************************************************/

int da1470x_bringup(void);

/****************************************************************************
 * Name: da1470x_gpio_initialize
 *
 * Description:
 *   Register the /dev/gpioN devices (LED, button).
 *
 ****************************************************************************/

#ifdef CONFIG_DEV_GPIO
int da1470x_gpio_initialize(void);
#endif

/****************************************************************************
 * Name: da1470x_spidev_initialize
 *
 * Description:
 *   Configure the SPI pins and register /dev/spiN.
 *
 ****************************************************************************/

#ifdef CONFIG_DA1470X_SPI
int da1470x_spidev_initialize(void);
#endif

/****************************************************************************
 * Name: da1470x_i2cdev_initialize
 *
 * Description:
 *   Configure the I2C pins and register /dev/i2cN.
 *
 ****************************************************************************/

#ifdef CONFIG_DA1470X_I2C
int da1470x_i2cdev_initialize(void);
#endif

/****************************************************************************
 * Name: da1470x_e120a390_initialize
 *
 * Description:
 *   Power, reset and configure the E120A390QSR panel and register it with
 *   the LCDC framebuffer driver.
 *
 ****************************************************************************/

#ifdef CONFIG_DA14706_LCD_E120A390QSR
int da1470x_e120a390_initialize(void);
#endif

/****************************************************************************
 * Name: da1470x_lpm012m134b_initialize
 *
 * Description:
 *   Register a JDI LPM012M134B panel on the JDI parallel interface with
 *   the LCDC framebuffer driver.
 *
 ****************************************************************************/

#ifdef CONFIG_DA14706_LCD_LPM012M134B
int da1470x_lpm012m134b_initialize(void);

/****************************************************************************
 * Name: da1470x_pwmled_setup
 *
 * Description:
 *   Register the PWMLED block as /dev/pwm0.
 *
 ****************************************************************************/

#ifdef CONFIG_DA1470X_PWMLED
int da1470x_pwmled_setup(void);
#endif

/****************************************************************************
 * Name: da1470x_zt2628_initialize
 *
 * Description:
 *   Register the display board's touch controller as /dev/input0.
 *
 ****************************************************************************/

#ifdef CONFIG_DA14706_TOUCH_ZT2628
int da1470x_zt2628_initialize(void);
#endif
#endif

#endif /* __ASSEMBLY__ */
#endif /* __BOARDS_ARM_DA1470X_DA14706_00HZDEVKT_P_SRC_DA14706_00HZDEVKT_P_H */
