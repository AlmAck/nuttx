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

/* Generated from the Renesas CMSIS device header DA1470x-00.h (SDK
 * 10.2.6.49).  Do not edit by hand; regenerate with tools/genhdr.py.
 */

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

/* Register Offsets *********************************************************/

#define DA1470X_PWMLED_LEDS_PWM_CTRL_OFFSET 0x0000  /* LED PWM control register */
#define DA1470X_PWMLED_LEDS_STATUS_OFFSET 0x0004    /* LEDS status register */
#define DA1470X_PWMLED_LED1_PWM_CONF_OFFSET 0x0008  /* Defines duty cycle and load sel for LED1 */
#define DA1470X_PWMLED_LED2_PWM_CONF_OFFSET 0x000c  /* Defines duty cycle and load sel for LED2 */
#define DA1470X_PWMLED_LED3_PWM_CONF_OFFSET 0x0010  /* Defines duty cycle and load sel for LED3 */
#define DA1470X_PWMLED_LEDS_DRV_CTRL_OFFSET 0x0014  /* LED driver control register */
#define DA1470X_PWMLED_LEDS_FREQUENCY_OFFSET 0x0018 /* Defines the frequency of all the LEDs */
#define DA1470X_PWMLED_LED_LOAD_SEL_OFFSET 0x001c   /* Load sel for LEDS */
#define DA1470X_PWMLED_LED_CURR_TRIM_OFFSET 0x0020  /* Trim values for LEDS */

/* Register Addresses *******************************************************/

#define DA1470X_PWMLED_LEDS_PWM_CTRL (DA1470X_PWMLED_BASE + DA1470X_PWMLED_LEDS_PWM_CTRL_OFFSET)
#define DA1470X_PWMLED_LEDS_STATUS (DA1470X_PWMLED_BASE + DA1470X_PWMLED_LEDS_STATUS_OFFSET)
#define DA1470X_PWMLED_LED1_PWM_CONF (DA1470X_PWMLED_BASE + DA1470X_PWMLED_LED1_PWM_CONF_OFFSET)
#define DA1470X_PWMLED_LED2_PWM_CONF (DA1470X_PWMLED_BASE + DA1470X_PWMLED_LED2_PWM_CONF_OFFSET)
#define DA1470X_PWMLED_LED3_PWM_CONF (DA1470X_PWMLED_BASE + DA1470X_PWMLED_LED3_PWM_CONF_OFFSET)
#define DA1470X_PWMLED_LEDS_DRV_CTRL (DA1470X_PWMLED_BASE + DA1470X_PWMLED_LEDS_DRV_CTRL_OFFSET)
#define DA1470X_PWMLED_LEDS_FREQUENCY (DA1470X_PWMLED_BASE + DA1470X_PWMLED_LEDS_FREQUENCY_OFFSET)
#define DA1470X_PWMLED_LED_LOAD_SEL (DA1470X_PWMLED_BASE + DA1470X_PWMLED_LED_LOAD_SEL_OFFSET)
#define DA1470X_PWMLED_LED_CURR_TRIM (DA1470X_PWMLED_BASE + DA1470X_PWMLED_LED_CURR_TRIM_OFFSET)

/* Register Bit-field Definitions *******************************************/

/* LEDS_PWM_CTRL register */

#define PWMLED_LEDS_PWM_CTRL_LED1_PWM_ENABLE (1 << 0)          /* Bit 0 */
#define PWMLED_LEDS_PWM_CTRL_LED2_PWM_ENABLE (1 << 1)          /* Bit 1 */
#define PWMLED_LEDS_PWM_CTRL_LED3_PWM_ENABLE (1 << 2)          /* Bit 2 */
#define PWMLED_LEDS_PWM_CTRL_PWM_LEDS_SW_PAUSE (1 << 3)        /* Bit 3 */
#define PWMLED_LEDS_PWM_CTRL_PWM_LEDS_HW_PAUSE_ENABLE (1 << 4) /* Bit 4 */

/* LEDS_STATUS register */

#define PWMLED_LEDS_STATUS_LED1_CONF_BUSY (1 << 0)     /* Bit 0 */
#define PWMLED_LEDS_STATUS_LED2_CONF_BUSY (1 << 1)     /* Bit 1 */
#define PWMLED_LEDS_STATUS_LED3_CONF_BUSY (1 << 2)     /* Bit 2 */
#define PWMLED_LEDS_STATUS_LED_FREQUENCY_BUSY (1 << 3) /* Bit 3 */

/* LED1_PWM_CONF register */

#define PWMLED_LED1_PWM_CONF_STOP_CYCLE_SHIFT (0)   /* Bits 0-11 */
#define PWMLED_LED1_PWM_CONF_STOP_CYCLE_MASK (0xfff << PWMLED_LED1_PWM_CONF_STOP_CYCLE_SHIFT)
#  define PWMLED_LED1_PWM_CONF_STOP_CYCLE(n) ((uint32_t)(n) << PWMLED_LED1_PWM_CONF_STOP_CYCLE_SHIFT)
#define PWMLED_LED1_PWM_CONF_START_CYCLE_SHIFT (12) /* Bits 12-23 */
#define PWMLED_LED1_PWM_CONF_START_CYCLE_MASK (0xfff << PWMLED_LED1_PWM_CONF_START_CYCLE_SHIFT)
#  define PWMLED_LED1_PWM_CONF_START_CYCLE(n) ((uint32_t)(n) << PWMLED_LED1_PWM_CONF_START_CYCLE_SHIFT)

/* LED2_PWM_CONF register */

#define PWMLED_LED2_PWM_CONF_STOP_CYCLE_SHIFT (0)   /* Bits 0-11 */
#define PWMLED_LED2_PWM_CONF_STOP_CYCLE_MASK (0xfff << PWMLED_LED2_PWM_CONF_STOP_CYCLE_SHIFT)
#  define PWMLED_LED2_PWM_CONF_STOP_CYCLE(n) ((uint32_t)(n) << PWMLED_LED2_PWM_CONF_STOP_CYCLE_SHIFT)
#define PWMLED_LED2_PWM_CONF_START_CYCLE_SHIFT (12) /* Bits 12-23 */
#define PWMLED_LED2_PWM_CONF_START_CYCLE_MASK (0xfff << PWMLED_LED2_PWM_CONF_START_CYCLE_SHIFT)
#  define PWMLED_LED2_PWM_CONF_START_CYCLE(n) ((uint32_t)(n) << PWMLED_LED2_PWM_CONF_START_CYCLE_SHIFT)

/* LED3_PWM_CONF register */

#define PWMLED_LED3_PWM_CONF_STOP_CYCLE_SHIFT (0)   /* Bits 0-11 */
#define PWMLED_LED3_PWM_CONF_STOP_CYCLE_MASK (0xfff << PWMLED_LED3_PWM_CONF_STOP_CYCLE_SHIFT)
#  define PWMLED_LED3_PWM_CONF_STOP_CYCLE(n) ((uint32_t)(n) << PWMLED_LED3_PWM_CONF_STOP_CYCLE_SHIFT)
#define PWMLED_LED3_PWM_CONF_START_CYCLE_SHIFT (12) /* Bits 12-23 */
#define PWMLED_LED3_PWM_CONF_START_CYCLE_MASK (0xfff << PWMLED_LED3_PWM_CONF_START_CYCLE_SHIFT)
#  define PWMLED_LED3_PWM_CONF_START_CYCLE(n) ((uint32_t)(n) << PWMLED_LED3_PWM_CONF_START_CYCLE_SHIFT)

/* LEDS_DRV_CTRL register */

#define PWMLED_LEDS_DRV_CTRL_LED1_EN (1 << 0) /* Bit 0 */
#define PWMLED_LEDS_DRV_CTRL_LED2_EN (1 << 1) /* Bit 1 */
#define PWMLED_LEDS_DRV_CTRL_LED3_EN (1 << 2) /* Bit 2 */

/* LEDS_FREQUENCY register */

#define PWMLED_LEDS_FREQUENCY_PWM_LEDS_PERIOD_SHIFT (0)    /* Bits 0-11 */
#define PWMLED_LEDS_FREQUENCY_PWM_LEDS_PERIOD_MASK (0xfff << PWMLED_LEDS_FREQUENCY_PWM_LEDS_PERIOD_SHIFT)
#  define PWMLED_LEDS_FREQUENCY_PWM_LEDS_PERIOD(n) ((uint32_t)(n) << PWMLED_LEDS_FREQUENCY_PWM_LEDS_PERIOD_SHIFT)
#define PWMLED_LEDS_FREQUENCY_PWM_LEDS_PRESCALE_SHIFT (12) /* Bits 12-19 */
#define PWMLED_LEDS_FREQUENCY_PWM_LEDS_PRESCALE_MASK (0xff << PWMLED_LEDS_FREQUENCY_PWM_LEDS_PRESCALE_SHIFT)
#  define PWMLED_LEDS_FREQUENCY_PWM_LEDS_PRESCALE(n) ((uint32_t)(n) << PWMLED_LEDS_FREQUENCY_PWM_LEDS_PRESCALE_SHIFT)

/* LED_LOAD_SEL register */

#define PWMLED_LED_LOAD_SEL_LED1_LOAD_SEL_SHIFT (0) /* Bits 0-2 */
#define PWMLED_LED_LOAD_SEL_LED1_LOAD_SEL_MASK (0x7 << PWMLED_LED_LOAD_SEL_LED1_LOAD_SEL_SHIFT)
#  define PWMLED_LED_LOAD_SEL_LED1_LOAD_SEL(n) ((uint32_t)(n) << PWMLED_LED_LOAD_SEL_LED1_LOAD_SEL_SHIFT)
#define PWMLED_LED_LOAD_SEL_LED2_LOAD_SEL_SHIFT (3) /* Bits 3-5 */
#define PWMLED_LED_LOAD_SEL_LED2_LOAD_SEL_MASK (0x7 << PWMLED_LED_LOAD_SEL_LED2_LOAD_SEL_SHIFT)
#  define PWMLED_LED_LOAD_SEL_LED2_LOAD_SEL(n) ((uint32_t)(n) << PWMLED_LED_LOAD_SEL_LED2_LOAD_SEL_SHIFT)
#define PWMLED_LED_LOAD_SEL_LED3_LOAD_SEL_SHIFT (6) /* Bits 6-8 */
#define PWMLED_LED_LOAD_SEL_LED3_LOAD_SEL_MASK (0x7 << PWMLED_LED_LOAD_SEL_LED3_LOAD_SEL_SHIFT)
#  define PWMLED_LED_LOAD_SEL_LED3_LOAD_SEL(n) ((uint32_t)(n) << PWMLED_LED_LOAD_SEL_LED3_LOAD_SEL_SHIFT)

/* LED_CURR_TRIM register */

#define PWMLED_LED_CURR_TRIM_LED1_CURR_TRIM_SHIFT (0) /* Bits 0-3 */
#define PWMLED_LED_CURR_TRIM_LED1_CURR_TRIM_MASK (0xf << PWMLED_LED_CURR_TRIM_LED1_CURR_TRIM_SHIFT)
#  define PWMLED_LED_CURR_TRIM_LED1_CURR_TRIM(n) ((uint32_t)(n) << PWMLED_LED_CURR_TRIM_LED1_CURR_TRIM_SHIFT)
#define PWMLED_LED_CURR_TRIM_LED2_CURR_TRIM_SHIFT (4) /* Bits 4-7 */
#define PWMLED_LED_CURR_TRIM_LED2_CURR_TRIM_MASK (0xf << PWMLED_LED_CURR_TRIM_LED2_CURR_TRIM_SHIFT)
#  define PWMLED_LED_CURR_TRIM_LED2_CURR_TRIM(n) ((uint32_t)(n) << PWMLED_LED_CURR_TRIM_LED2_CURR_TRIM_SHIFT)
#define PWMLED_LED_CURR_TRIM_LED3_CURR_TRIM_SHIFT (8) /* Bits 8-11 */
#define PWMLED_LED_CURR_TRIM_LED3_CURR_TRIM_MASK (0xf << PWMLED_LED_CURR_TRIM_LED3_CURR_TRIM_SHIFT)
#  define PWMLED_LED_CURR_TRIM_LED3_CURR_TRIM(n) ((uint32_t)(n) << PWMLED_LED_CURR_TRIM_LED3_CURR_TRIM_SHIFT)

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_PWMLED_H */
