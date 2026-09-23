/****************************************************************************
 * arch/arm/src/da1470x/da1470x_charger.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_CHARGER_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_CHARGER_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdbool.h>
#include <stdint.h>

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
 * Name: da1470x_charger_initialize
 *
 * Description:
 *   Configure the battery charger and register it as a NuttX battery
 *   charger device, conventionally "/dev/charger0".
 *
 *   The charger is a state machine in hardware: give it the charge
 *   voltage, the currents and the temperature limits and it runs the
 *   pre-charge, constant-current, constant-voltage and end-of-charge
 *   phases by itself, reporting where it is through its status register
 *   and interrupting on changes and on errors.  Nothing here has to
 *   supervise a charge in progress.
 *
 *   It is registered with charging DISABLED.  A charger that starts
 *   pushing current into whatever happens to be attached, before anything
 *   has said what that is, is not a good default; call the voltage and
 *   current methods and then enable it through BATIOC_OPERATE.
 *
 * Input Parameters:
 *   devpath - where to register it
 *
 * Returned Value:
 *   Zero on success, a negated errno otherwise.
 *
 ****************************************************************************/

int da1470x_charger_initialize(const char *devpath);

/****************************************************************************
 * Name: da1470x_charger_vbus_present
 *
 * Description:
 *   Whether a supply is plugged in, read from the analogue comparator in
 *   the always-on domain rather than from the charger, so it answers even
 *   with the charger disabled.
 *
 ****************************************************************************/

bool da1470x_charger_vbus_present(void);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_CHARGER_H */
