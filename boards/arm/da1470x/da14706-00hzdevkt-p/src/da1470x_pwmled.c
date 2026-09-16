/****************************************************************************
 * boards/arm/da1470x/da14706-00hzdevkt-p/src/da1470x_pwmled.c
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

#include <nuttx/config.h>

#include <debug.h>
#include <errno.h>

#include <nuttx/board.h>
#include <nuttx/timers/pwm.h>

#include "da1470x_pwmled.h"
#include "da14706-00hzdevkt-p.h"

#ifdef CONFIG_DA1470X_PWMLED

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* The devkit routes the three LED sinks to the LED1..LED3 pads; no
 * current trim is applied.
 */

static const struct da1470x_pwmled_config_s g_pwmled_config =
{
  .load_sel  =
  {
    0, 0, 0
  },
  .curr_trim =
  {
    0, 0, 0
  },
  .chmask    = 0x07,
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_pwmled_setup
 *
 * Description:
 *   Register the PWMLED block as /dev/pwm0.
 *
 ****************************************************************************/

int da1470x_pwmled_setup(void)
{
  struct pwm_lowerhalf_s *lower;
  int ret;

  lower = da1470x_pwmled_initialize(&g_pwmled_config);
  if (lower == NULL)
    {
      return -ENODEV;
    }

  ret = pwm_register("/dev/pwm0", lower);
  if (ret < 0)
    {
      syslog(LOG_ERR, "pwm_register failed: %d\n", ret);
    }

  return ret;
}

#endif /* CONFIG_DA1470X_PWMLED */
