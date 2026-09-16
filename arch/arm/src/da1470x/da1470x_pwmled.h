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

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_PWMLED_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_PWMLED_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>

#include <nuttx/timers/pwm.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* The PWMLED block drives three constant-current LED outputs (LED1..LED3)
 * from one shared PWM period.  Channels are numbered 1..3 as the NuttX
 * PWM upper half expects.
 */

#define DA1470X_PWMLED_NCHANNELS   3

/* Load selection: which sink each LED output drives (LED_LOAD_SEL_REG).
 * The value is board dependent, see the datasheet PWMLED chapter.
 */

#define DA1470X_PWMLED_LOAD_MAX    7

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* Board-supplied static configuration */

struct da1470x_pwmled_config_s
{
  uint8_t load_sel[DA1470X_PWMLED_NCHANNELS];  /* Sink selection per LED */
  uint8_t curr_trim[DA1470X_PWMLED_NCHANNELS]; /* Current trim, 0..15 */
  uint8_t chmask;                              /* Channels used when
                                                * CONFIG_PWM_MULTICHAN is
                                                * off (bit n = LED n+1) */
};

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
 * Name: da1470x_pwmled_initialize
 *
 * Description:
 *   Initialize the PWMLED block and return its PWM lower half, to be
 *   passed to pwm_register().
 *
 ****************************************************************************/

struct pwm_lowerhalf_s *
da1470x_pwmled_initialize(const struct da1470x_pwmled_config_s *config);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_PWMLED_H */
