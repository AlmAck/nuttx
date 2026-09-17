/****************************************************************************
 * boards/arm/da1470x/da14706-00hzdevkt-p/src/da1470x_zt2628.c
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

#include <stdbool.h>
#include <debug.h>
#include <errno.h>

#include <nuttx/arch.h>
#include <nuttx/i2c/i2c_master.h>
#include <nuttx/input/zt2628.h>
#include <arch/board/board.h>

#include "da1470x_gpio.h"
#include "da1470x_i2c.h"
#include "da14706-00hzdevkt-p.h"

#ifdef CONFIG_DA14706_TOUCH_ZT2628

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int  zt2628_attach(const struct zt2628_config_s *config,
                          xcpt_t isr, void *arg);
static void zt2628_enable(const struct zt2628_config_s *config,
                          bool enable);
static void zt2628_power(const struct zt2628_config_s *config, bool on);
static void zt2628_reset(const struct zt2628_config_s *config,
                         bool assert);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct zt2628_config_s g_zt2628_config =
{
  .address   = ZT2628_I2C_ADDRESS,
  .frequency = 400000,
  .xres      = 390,
  .yres      = 390,
  .attach    = zt2628_attach,
  .enable    = zt2628_enable,
  .power     = zt2628_power,
  .reset     = zt2628_reset,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int zt2628_attach(const struct zt2628_config_s *config,
                         xcpt_t isr, void *arg)
{
  return da1470x_gpioirq_attach(BOARD_TOUCH_INT_PIN,
                                DA1470X_GPIO_EDGE_FALLING, isr, arg);
}

static void zt2628_enable(const struct zt2628_config_s *config,
                          bool enable)
{
  if (enable)
    {
      da1470x_gpioirq_enable(BOARD_TOUCH_INT_PIN);
    }
  else
    {
      da1470x_gpioirq_disable(BOARD_TOUCH_INT_PIN);
    }
}

static void zt2628_power(const struct zt2628_config_s *config, bool on)
{
  /* The display board's supply switch also feeds the touch I2C pull-ups */

  da1470x_gpio_write(BOARD_TOUCH_PWR_PIN, on);
}

static void zt2628_reset(const struct zt2628_config_s *config,
                         bool assert)
{
  da1470x_gpio_write(BOARD_TOUCH_RST_PIN, !assert);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_zt2628_initialize
 *
 * Description:
 *   Bring up the touch controller on I2C0 and register /dev/input0.
 *
 ****************************************************************************/

int da1470x_zt2628_initialize(void)
{
  struct i2c_master_s *i2c;

  da1470x_gpio_config(BOARD_TOUCH_PWR_PIN);
  da1470x_gpio_config(BOARD_TOUCH_RST_PIN);
  da1470x_gpio_config(BOARD_TOUCH_INT_PIN);

  i2c = da1470x_i2cbus_initialize(0);
  if (i2c == NULL)
    {
      return -ENODEV;
    }

  return zt2628_register(i2c, &g_zt2628_config, 0);
}

#endif /* CONFIG_DA14706_TOUCH_ZT2628 */
