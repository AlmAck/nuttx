/****************************************************************************
 * arch/arm/src/da1470x/da1470x_pm.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_PM_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_PM_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdbool.h>

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

#ifdef CONFIG_PM

/****************************************************************************
 * Name: da1470x_pm_normal
 *
 * Description:
 *   Leave any low-power state: restore the full clock tree and clear the
 *   deep-sleep request.
 *
 ****************************************************************************/

void da1470x_pm_normal(void);

/****************************************************************************
 * Name: da1470x_pm_standby
 *
 * Description:
 *   Prepare the PM_STANDBY state: the core clock is gated on WFI with all
 *   power domains and RAM retained (SCR.SLEEPDEEP with PMU SYS_SLEEP off).
 *   Any NVIC interrupt wakes the core.
 *
 ****************************************************************************/

void da1470x_pm_standby(void);

/****************************************************************************
 * Name: da1470x_pm_sleep
 *
 * Description:
 *   Prepare the PM_SLEEP state.  With CONFIG_DA1470X_PM_EXTENDED_SLEEP the
 *   PMU is allowed to switch PD_SYS off and the wake-up goes through the
 *   PDC; otherwise this behaves like PM_STANDBY.
 *
 ****************************************************************************/

void da1470x_pm_sleep(void);

#endif /* CONFIG_PM */

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_PM_H */
