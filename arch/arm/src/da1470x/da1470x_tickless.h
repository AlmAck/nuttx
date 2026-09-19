/****************************************************************************
 * arch/arm/src/da1470x/da1470x_tickless.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_TICKLESS_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_TICKLESS_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifdef CONFIG_DA1470X_TICKLESS

/* The peripheral trigger the timer raises towards the power domain
 * controller, so that the wake-up the system time asks for still arrives
 * when PD_SYS has been switched off.
 */

#if CONFIG_DA1470X_TICKLESS_TIMER == 1
#  define DA1470X_TICKLESS_PDC_TRIG  DA1470X_PDC_PERIPH_TIMER
#else
#  define DA1470X_TICKLESS_PDC_TRIG  DA1470X_PDC_PERIPH_TIMER2
#endif

#endif /* CONFIG_DA1470X_TICKLESS */

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_TICKLESS_H */
