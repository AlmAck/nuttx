/****************************************************************************
 * arch/arm/src/da1470x/da1470x_pwm.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_PWM_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_PWM_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <nuttx/timers/pwm.h>

#include "da1470x_gpio.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Which timer.  TIMER and TIMER2 are left out on purpose: the tickless
 * clock is built on TIMER2, and taking its counter away would stop the
 * system clock rather than make a noise.
 */

#define DA1470X_PWM_TIMER3 3
#define DA1470X_PWM_TIMER4 4
#define DA1470X_PWM_TIMER5 5
#define DA1470X_PWM_TIMER6 6

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Name: da1470x_pwm_initialize
 *
 * Description:
 *   Prepare one of the general purpose timers to drive a pin as a pulse
 *   width modulated output, and return the lower half for the board to
 *   register with pwm_register().
 *
 *   Each of these timers has exactly one output, so each device has one
 *   channel.  What is on the other end of the pin is the board's business:
 *   a vibration motor is the reason this exists, but nothing here assumes
 *   one.
 *
 *   The timer is clocked from the divided system clock, so the output
 *   stops when that clock does.  A caller that needs the output to survive
 *   a sleep state has to hold the system awake itself.
 *
 * Input Parameters:
 *   timer  - DA1470X_PWM_TIMER3 .. DA1470X_PWM_TIMER6
 *   pinset - the pin, already carrying the matching GPIO_FUNC_TIMn_PWM
 *
 * Returned Value:
 *   The lower half on success, NULL on failure.
 *
 ****************************************************************************/

struct pwm_lowerhalf_s *da1470x_pwm_initialize(int timer,
                                               da1470x_pinset_t pinset);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_PWM_H */
