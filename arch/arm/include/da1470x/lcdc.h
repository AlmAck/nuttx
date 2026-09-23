/****************************************************************************
 * arch/arm/include/da1470x/lcdc.h
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

#ifndef __ARCH_ARM_INCLUDE_DA1470X_LCDC_H
#define __ARCH_ARM_INCLUDE_DA1470X_LCDC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdint.h>

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

/****************************************************************************
 * Name: da1470x_lcdc_set_brightness
 *
 * Description:
 *   Set the panel's brightness.  Only panels that carry a brightness hook
 *   answer this; the rest return -ENOTSUP.
 *
 *   The level is remembered across power transitions.  Powering the panel
 *   on re-runs its init, which resets it to full, so the driver reapplies
 *   the stored level afterwards -- a level set while the panel is dark is
 *   kept rather than dropped, and callers need not order the two.
 *
 * Input Parameters:
 *   level - 0 (darkest) to 255 (full).  Note that 0 does not power the
 *           panel down: it stays on, and keeps the system at PM_NORMAL.
 *           Use FBIOSET_POWER to actually put it out.
 *
 * Returned Value:
 *   OK on success; -ENODEV if the LCDC is not up, -ENOTSUP if the panel
 *   has no brightness control.
 *
 ****************************************************************************/

int da1470x_lcdc_set_brightness(uint8_t level);

#ifdef __cplusplus
}
#endif

#endif /* __ARCH_ARM_INCLUDE_DA1470X_LCDC_H */
