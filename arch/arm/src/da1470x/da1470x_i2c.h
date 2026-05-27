/****************************************************************************
 * arch/arm/src/da1470x/da1470x_i2c.h
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to you under the Apache License, Version
 * 2.0 (the "License"); you may not use this file except in compliance
 * with the License.
 *
 ****************************************************************************/

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_I2C_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_I2C_H

#include <nuttx/config.h>
#include <nuttx/i2c/i2c_master.h>

/* Bus numbering: 0 = I2C, 1 = I2C2, 2 = I2C3. All three live in PD_SNC. */

#define DA1470X_I2C_BUS_I2C    0
#define DA1470X_I2C_BUS_I2C2   1
#define DA1470X_I2C_BUS_I2C3   2

/****************************************************************************
 * Name: da1470x_i2cbus_initialize
 *
 * Description:
 *   Bring up an I2C master controller and return its NuttX i2c_master_s.
 *   The controller is configured as 7-bit-address master in Fast-mode
 *   (~400 kHz). Pin mux is the board's responsibility.
 *
 * Returned Value:
 *   Non-NULL i2c_master_s on success; NULL on bad bus index.
 ****************************************************************************/

struct i2c_master_s *da1470x_i2cbus_initialize(int bus);

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_I2C_H */
