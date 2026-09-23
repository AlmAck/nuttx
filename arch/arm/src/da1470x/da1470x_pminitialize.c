/****************************************************************************
 * arch/arm/src/da1470x/da1470x_pminitialize.c
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

#include <nuttx/power/pm.h>

#include "arm_internal.h"
#include "da1470x_pdc.h"
#include "da1470x_pm.h"
#include "da1470x_tickless.h"

#ifdef CONFIG_PM

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: arm_pminitialize
 *
 * Description:
 *   This function is called by MCU-specific logic at power-on reset in
 *   order to provide one-time initialization the power management
 *   subsystem.  This function must be called *very* early in the
 *   initialization sequence *before* any other device drivers are
 *   initialized (since they may attempt to register with the power
 *   management subsystem).
 *
 ****************************************************************************/

void arm_pminitialize(void)
{
  /* Initialize the NuttX power management subsystem proper */

  pm_initialize();

#ifdef CONFIG_DA1470X_PDC
  /* The PDC must be ready before any wake-up source is registered */

  da1470x_pdc_initialize();

  /* Keep the debugger able to wake the part */

  da1470x_pdc_add(DA1470X_PDC_TRIG_PERIPHERAL, DA1470X_PDC_PERIPH_COMBO,
                  DA1470X_PDC_MASTER_CM33, DA1470X_PDC_FLAG_EN_XTAL);

#ifdef CONFIG_DA1470X_TICKLESS
  /* Once PD_SYS can be switched off, an interrupt is no longer enough to
   * bring the processor back: only the power domain controller can.  The
   * timer carrying the system time therefore needs an entry of its own, or
   * every sleep would last until something else happened.
   */

  da1470x_pdc_add(DA1470X_PDC_TRIG_PERIPHERAL, DA1470X_TICKLESS_PDC_TRIG,
                  DA1470X_PDC_MASTER_CM33,
                  DA1470X_PDC_FLAG_EN_XTAL | DA1470X_PDC_FLAG_EN_TMR);
#endif

#ifdef CONFIG_DA1470X_PM_WAKE_GPIO
  /* A pin that should bring the system back -- conventionally the console
   * receive line, so that a keystroke still reaches a sleeping board.
   */

  da1470x_pdc_add(DA1470X_PDC_TRIG_P0_GPIO +
                  CONFIG_DA1470X_PM_WAKE_GPIO_PORT,
                  CONFIG_DA1470X_PM_WAKE_GPIO_PIN,
                  DA1470X_PDC_MASTER_CM33, DA1470X_PDC_FLAG_EN_XTAL);
#endif
#endif
}

#endif /* CONFIG_PM */
