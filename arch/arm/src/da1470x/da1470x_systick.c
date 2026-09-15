/****************************************************************************
 * arch/arm/src/da1470x/da1470x_systick.c
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

#include <stdint.h>
#include <time.h>
#include <debug.h>

#include <nuttx/arch.h>
#include <nuttx/timers/arch_timer.h>

#include "systick.h"
#include "da1470x_clockconfig.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_timer_initialize
 *
 * Description:
 *   This function is called during start-up to initialize the system
 *   timer interrupt.  SysTick runs from the CPU clock, whose frequency is
 *   read back from the clock controller so that any Kconfig clock choice
 *   yields correct system time.
 *
 ****************************************************************************/

void up_timer_initialize(void)
{
  up_timer_set_lowerhalf(systick_initialize(true, da1470x_get_sysclk(),
                                            -1));
}
