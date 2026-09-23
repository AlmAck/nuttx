/****************************************************************************
 * arch/arm/src/da1470x/da1470x_serial.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_SERIAL_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_SERIAL_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470x_config.h"

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_earlyserialinit
 *
 * Description:
 *   Performs the low level UART initialization early in debug so that the
 *   serial console will be available during bootup.  This must be called
 *   before da1470x_serialinit.  NOTE:  This function depends on GPIO pin
 *   configuration performed in xxxx_lowsetup() and main clock initialization
 *   performed in xxx_clock_configure().
 *
 ****************************************************************************/

#ifdef USE_EARLYSERIALINIT
void da1470x_earlyserialinit(void);
#endif

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_SERIAL_H */
