/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_pwmled.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_PWMLED_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_PWMLED_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470x_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register offsets *********************************************************/

#define DA1470X_PWMLED_LED1_PWM_CONF_OFFSET    0x0008 /* Defines duty cycle and load sel for LED1 */
#define DA1470X_PWMLED_LED2_PWM_CONF_OFFSET    0x000C /* Defines duty cycle and load sel for LED2 */
#define DA1470X_PWMLED_LED3_PWM_CONF_OFFSET    0x0010 /* Defines duty cycle and load sel for LED3 */
#define DA1470X_PWMLED_LEDS_DRV_CTRL_OFFSET    0x0014 /* LED driver control register */
#define DA1470X_PWMLED_LEDS_FREQUENCY_OFFSET   0x0018 /* Defines the frequency of all the LEDs */
#define DA1470X_PWMLED_LEDS_PWM_CTRL_OFFSET    0x0000 /* LED PWM control register */
#define DA1470X_PWMLED_LEDS_STATUS_OFFSET      0x0004 /* LEDS status register */
#define DA1470X_PWMLED_LED_CURR_TRIM_OFFSET    0x0020 /* Trim values for LEDS */
#define DA1470X_PWMLED_LED_LOAD_SEL_OFFSET     0x001C /* Load sel for LEDS */

/* Register addresses *******************************************************/

//#define DA1470X_PWMLED_BASE                    0x50010600 /* PWMLED registers */

#define DA1470X_PWMLED_LED1_PWM_CONF           (DA1470X_PWMLED_BASE + DA1470X_PWMLED_LED1_PWM_CONF_OFFSET) /* Defines duty cycle and load sel for LED1 */
#define DA1470X_PWMLED_LED2_PWM_CONF           (DA1470X_PWMLED_BASE + DA1470X_PWMLED_LED2_PWM_CONF_OFFSET) /* Defines duty cycle and load sel for LED2 */
#define DA1470X_PWMLED_LED3_PWM_CONF           (DA1470X_PWMLED_BASE + DA1470X_PWMLED_LED3_PWM_CONF_OFFSET) /* Defines duty cycle and load sel for LED3 */
#define DA1470X_PWMLED_LEDS_DRV_CTRL           (DA1470X_PWMLED_BASE + DA1470X_PWMLED_LEDS_DRV_CTRL_OFFSET) /* LED driver control register */
#define DA1470X_PWMLED_LEDS_FREQUENCY          (DA1470X_PWMLED_BASE + DA1470X_PWMLED_LEDS_FREQUENCY_OFFSET) /* Defines the frequency of all the LEDs */
#define DA1470X_PWMLED_LEDS_PWM_CTRL           (DA1470X_PWMLED_BASE + DA1470X_PWMLED_LEDS_PWM_CTRL_OFFSET) /* LED PWM control register */
#define DA1470X_PWMLED_LEDS_STATUS             (DA1470X_PWMLED_BASE + DA1470X_PWMLED_LEDS_STATUS_OFFSET) /* LEDS status register */
#define DA1470X_PWMLED_LED_CURR_TRIM           (DA1470X_PWMLED_BASE + DA1470X_PWMLED_LED_CURR_TRIM_OFFSET) /* Trim values for LEDS */
#define DA1470X_PWMLED_LED_LOAD_SEL            (DA1470X_PWMLED_BASE + DA1470X_PWMLED_LED_LOAD_SEL_OFFSET) /* Load sel for LEDS */

/* Register bit definitions *************************************************/

/* LEDx_PWM_CONF Register */

#define PWMLED_START_CYCLE_MASK               (0xFFF << 12) /* Defines the cycle at which the PWM becomes high. Setting start_cycle = stop_cycl */
#define PWMLED_START_CYCLE_POS                (12) /* Defines the cycle at which the PWM becomes high. Setting start_cycle = stop_cycl */
#define PWMLED_STOP_CYCLE_MASK                (0xFFF << 0) /* Defines the cycle at which the PWM becomes low. Setting start_cycle = stop_cycle */
#define PWMLED_STOP_CYCLE_POS                 (0) /* Defines the cycle at which the PWM becomes low. Setting start_cycle = stop_cycle */

/* LEDS_DRV_CTRL Register */

#define PWMLED_LED3_EN                        (1U << 2) /* 0 = LED disabled, 1 = LED enabled */
#define PWMLED_LED2_EN                        (1U << 1) /* 0 = LED disabled, 1 = LED enabled */
#define PWMLED_LED1_EN                        (1U << 0) /* 0 = LED disabled, 1 = LED enabled */

/* LEDS_FREQUENCY Register */

#define PWMLED_PWM_LEDS_PRESCALE_MASK         (0xFF << 12) /* Defines the clock prescaler that is used for the input clock for PWM LED1-3. Div */
#define PWMLED_PWM_LEDS_PRESCALE_POS          (12) /* Defines the clock prescaler that is used for the input clock for PWM LED1-3. Div */
#define PWMLED_PWM_LEDS_PERIOD_MASK           (0xFFF << 0) /* Defines the period of PWM signals 1-3. Period = Period constant system clock * ( */
#define PWMLED_PWM_LEDS_PERIOD_POS            (0) /* Defines the period of PWM signals 1-3. Period = Period constant system clock * ( */

/* LEDS_PWM_CTRL Register */

#define PWMLED_PWM_LEDS_HW_PAUSE_ENABLE       (1U << 4) /* When this bit is set, PWM engines 1-3 are paused when the radio enable is high */
#define PWMLED_PWM_LEDS_SW_PAUSE              (1U << 3) /* PWM engines1-3 are paused when this bit is set by SW */
#define PWMLED_LED3_PWM_ENABLE                (1U << 2) /* Enables/Disables the PWM engine */
#define PWMLED_LED2_PWM_ENABLE                (1U << 1) /* Enables/Disables the PWM engine */
#define PWMLED_LED1_PWM_ENABLE                (1U << 0) /* Enables/Disables the PWM engine */

/* LEDS_STATUS Register */

#define PWMLED_LED_FREQUENCY_BUSY             (1U << 3) /* LEDS_FREQUENCY_REG is busy with synchronisation. Wait unti this bit is 0 before  */
#define PWMLED_LED3_CONF_BUSY                 (1U << 2) /* LED3_CONF_REG is busy with synchronisation. Wait unti this bit is 0 before writi */
#define PWMLED_LED2_CONF_BUSY                 (1U << 1) /* LED2_CONF_REG is busy with synchronisation. Wait unti this bit is 0 before writi */
#define PWMLED_LED1_CONF_BUSY                 (1U << 0) /* LED1_CONF_REG is busy with synchronisation. Wait unti this bit is 0 before writi */

/* LED_CURR_TRIM Register */

#define PWMLED_LED3_CURR_TRIM_MASK            (0xF << 8) /* LED current trimming */
#define PWMLED_LED3_CURR_TRIM_POS             (8) /* LED current trimming */
#define PWMLED_LED2_CURR_TRIM_MASK            (0xF << 4) /* LED current trimming */
#define PWMLED_LED2_CURR_TRIM_POS             (4) /* LED current trimming */
#define PWMLED_LED1_CURR_TRIM_MASK            (0xF << 0) /* LED current trimming */
#define PWMLED_LED1_CURR_TRIM_POS             (0) /* LED current trimming */

/* LED_LOAD_SEL Register */

#define PWMLED_LED3_LOAD_SEL_MASK             (0x7 << 6) /* Defines LED sinking current: 2.5 mA + (LED_LOAD_SEL*2.5 mA). Max = 20 mA */
#define PWMLED_LED3_LOAD_SEL_POS              (6) /* Defines LED sinking current: 2.5 mA + (LED_LOAD_SEL*2.5 mA). Max = 20 mA */
#define PWMLED_LED2_LOAD_SEL_MASK             (0x7 << 3) /* Defines LEDsinking current: 2.5 mA + (LED_LOAD_SEL*2.5 mA). Max = 20 mA */
#define PWMLED_LED2_LOAD_SEL_POS              (3) /* Defines LEDsinking current: 2.5 mA + (LED_LOAD_SEL*2.5 mA). Max = 20 mA */
#define PWMLED_LED1_LOAD_SEL_MASK             (0x7 << 0) /* Defines LED sinking current: 2.5 mA + (LED_LOAD_SEL*2.5 mA). Max = 20 mA */
#define PWMLED_LED1_LOAD_SEL_POS              (0) /* Defines LED sinking current: 2.5 mA + (LED_LOAD_SEL*2.5 mA). Max = 20 mA */


/* LEDx Helper Offsets */
#define PWMLED_PWM_CONF_OFFSET_INTERVAL       (4)
#define PWMLED_LED_LOAD_OFFSET_INTERVAL       (3)
#define PWMLED_LED_CURR_TRIM_INTERVAL         (4)

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_PWMLED_H */
