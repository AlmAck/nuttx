/****************************************************************************
 * arch/arm/src/da1470x/da1470x_pwmled.h
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_PWMLED_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_PWMLED_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdint.h>
#include <sys/ioctl.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Device Path */
#define DA1470X_LED_DEVPATH "/dev/led_pwm"

/* Maximum number of LEDs supported */
#define DA1470X_LED_MAX HW_LED_ID_MAX

/* IOCTL Command Definitions */

/* Base for LED IOCTL commands */
#define LEDIOC_BASE       0xF0

/* Define a macro to create LED IOCTL commands */
#define LEDIOC(nr)        _IO(LEDIOC_BASE, nr)

/* LED IOCTL Commands */
#define LEDIOC_SET_DUTY_CYCLE      LEDIOC(0)  /* Set PWM duty cycle */
#define LEDIOC_SET_LOAD_SEL        LEDIOC(1)  /* Set load selection */
#define LEDIOC_GET_DUTY_CYCLE      LEDIOC(2)  /* Get PWM duty cycle */
#define LEDIOC_GET_LOAD_SEL        LEDIOC(3)  /* Get load selection */
#define LEDIOC_SET_CURRENT_TRIM    LEDIOC(4)  /* Set current trim */
#define LEDIOC_SET_FREQUENCY       LEDIOC(5)  /* Set PWM frequency */

/* Enumeration of LED IDs */
typedef enum
{
  HW_LED_ID_LED_1 = 0,  /* LED 1 */
  HW_LED_ID_LED_2,      /* LED 2 */
  HW_LED_ID_LED_3,      /* LED 3 */
  HW_LED_ID_MAX         /* Maximum number of LEDs */
} HW_LED_ID;

/* Structure for PWM Duty Cycle */
typedef struct
{
  uint16_t hw_led_pwm_start;  /* PWM Start Cycle */
  uint16_t hw_led_pwm_end;    /* PWM End Cycle */
} hw_led_pwm_duty_cycle_t;

/* Structure for LED PWM Duty Cycle IOCTL */
struct led_pwm_duty_cycle_s
{
  HW_LED_ID led_id;                       /* LED Identifier */
  hw_led_pwm_duty_cycle_t duty_cycle;      /* Duty cycle configuration */
};

/* Structure for LED Load Selection IOCTL */
struct led_load_sel_s
{
  HW_LED_ID led_id;       /* LED Identifier */
  uint8_t load_sel;       /* Load selection value */
};

/* Structure for LED Current Trim IOCTL */
struct led_current_trim_s
{
  HW_LED_ID led_id;       /* LED Identifier */
  uint32_t trim;          /* Current trim value */
};

/* Structure for LED Configuration */
struct hw_led_config
{
  uint32_t leds_pwm_frequency;                     /* PWM frequency in Hz */
  uint32_t leds_pwm_duty_cycle[DA1470X_LED_MAX];   /* PWM duty cycles for each LED */
  uint32_t leds_pwm_start_cycle[DA1470X_LED_MAX];  /* PWM start cycles for each LED */
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/**
 * @brief Register the DA1470x LED driver
 *
 * This function initializes the DA1470x LED driver with the specified
 * configuration and registers it with the NuttX device filesystem.
 *
 * @param conf A pointer to the LED configuration structure
 *
 * @return Zero (OK) on success; a negated errno value on failure
 */
int da1470x_led_register(const struct hw_led_config *conf);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __INCLUDE_NUTTX_LEDS_DA1470X_PWMLED_H */
