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
#include <arch/board/board.h>

#include <nuttx/timers/arch_timer.h>
#include "systick.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Function:  up_timer_initialize
 *
 * Description:
 *   This function is called during start-up to initialize
 *   the timer interrupt.
 *
 ****************************************************************************/

void up_timer_initialize(void)
{
  /* Use SysTick to drive system timer. coreclk=true selects the CPU
   * clock as the SysTick source (with CLKSOURCE bit); BOARD_SYSTICK_CLOCK
   * must match the actual sysclk frequency programmed in
   * da1470_clockconfig() or system time will be wrong proportionally.
   *
   * Requires CONFIG_ARMV8M_SYSTICK=y (provides systick_initialize) and
   * CONFIG_TIMER=y + CONFIG_TIMER_ARCH=y (provides up_timer_set_lowerhalf
   * and the TIMER_START hookup that flips the ENABLE bit).
   */

  up_timer_set_lowerhalf(systick_initialize(true, BOARD_SYSTICK_CLOCK, -1));
}
