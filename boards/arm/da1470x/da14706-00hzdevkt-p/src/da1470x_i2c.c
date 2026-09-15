/****************************************************************************
 * boards/arm/da1470x/da14706-00hzdevkt-p/src/da1470x_i2c.c
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

#include <nuttx/i2c/i2c_master.h>
#include <arch/board/board.h>

#include "da1470x_gpio.h"
#include "da1470x_i2c.h"
#include "da14706-00hzdevkt-p.h"

#ifdef CONFIG_DA1470X_I2C

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_i2cdev_initialize
 *
 * Description:
 *   Configure the I2C0 pins, initialize the bus and register /dev/i2c0.
 *
 ****************************************************************************/

int da1470x_i2cdev_initialize(void)
{
  int ret = OK;

#ifdef CONFIG_DA1470X_I2C0
  struct i2c_master_s *i2c;

  da1470x_gpio_config(BOARD_I2C0_SCL_PIN);
  da1470x_gpio_config(BOARD_I2C0_SDA_PIN);

  i2c = da1470x_i2cbus_initialize(0);
  if (i2c == NULL)
    {
      i2cerr("Failed to initialize I2C0\n");
      return -ENODEV;
    }

#ifdef CONFIG_I2C_DRIVER
  ret = i2c_register(i2c, 0);
  if (ret < 0)
    {
      i2cerr("Failed to register /dev/i2c0: %d\n", ret);
    }
#endif
#endif

  return ret;
}

#endif /* CONFIG_DA1470X_I2C */
