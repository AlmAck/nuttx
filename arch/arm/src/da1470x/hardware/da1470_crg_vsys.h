/****************************************************************************
 * ./tmp/da1470_crg_vsys.h
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

#ifndef __DA1470_CRG_VSYS_H
#define __DA1470_CRG_VSYS_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register offsets *********************************************************/

#define DA1470_CRG_VSYS_VSYS_GEN_CTRL_OFFSET        0x0000 /* No description available. */
#define DA1470_CRG_VSYS_VSYS_GEN_IRQ_CLEAR_OFFSET   0x0008 /* No description available. */
#define DA1470_CRG_VSYS_VSYS_GEN_IRQ_MASK_OFFSET    0x000C /* No description available. */
#define DA1470_CRG_VSYS_VSYS_GEN_IRQ_STATUS_OFFSET  0x0004 /* No description available. */

/* Register addresses *******************************************************/

#define DA1470_CRG_VSYS_BASE                        0x50000B00 /* CRG_VSYS registers */

#define DA1470_CRG_VSYS_VSYS_GEN_CTRL               (DA1470_CRG_VSYS_BASE + DA1470_CRG_VSYS_VSYS_GEN_CTRL_OFFSET) /* No description available. */
#define DA1470_CRG_VSYS_VSYS_GEN_IRQ_CLEAR          (DA1470_CRG_VSYS_BASE + DA1470_CRG_VSYS_VSYS_GEN_IRQ_CLEAR_OFFSET) /* No description available. */
#define DA1470_CRG_VSYS_VSYS_GEN_IRQ_MASK           (DA1470_CRG_VSYS_BASE + DA1470_CRG_VSYS_VSYS_GEN_IRQ_MASK_OFFSET) /* No description available. */
#define DA1470_CRG_VSYS_VSYS_GEN_IRQ_STATUS         (DA1470_CRG_VSYS_BASE + DA1470_CRG_VSYS_VSYS_GEN_IRQ_STATUS_OFFSET) /* No description available. */

/* Register bit definitions *************************************************/


/* VSYS_GEN_CTRL Register */

#define CRG_VSYS_FORCE_VBAT_VSYS_SW_MASK            (0x3 << 23) /* 0x0,0x1: VBAT_VSYS set to ideal diode 0x2: Forces VBAT_VSYS switch to be opened  */
#define CRG_VSYS_FORCE_VBAT_VSYS_SW_POS             (23) /* 0x0,0x1: VBAT_VSYS set to ideal diode 0x2: Forces VBAT_VSYS switch to be opened  */
#define CRG_VSYS_FORCE_LDO_ENABLE_MASK              (0x3 << 21) /* 0x0,0x1: LDO VSYS will be enabled when COMP_VBUS_OK & COMP_VBUS_ABOVE_VSYS. 0x2: */
#define CRG_VSYS_FORCE_LDO_ENABLE_POS               (21) /* 0x0,0x1: LDO VSYS will be enabled when COMP_VBUS_OK & COMP_VBUS_ABOVE_VSYS. 0x2: */
#define CRG_VSYS_LDO_TEMP_PROTECT_MODE_MASK         (0x3 << 19) /* 0x0: LDO_VSYS is muted (disabled) when temperature is too high, when temperature */
#define CRG_VSYS_LDO_TEMP_PROTECT_MODE_POS          (19) /* 0x0: LDO_VSYS is muted (disabled) when temperature is too high, when temperature */
#define CRG_VSYS_EN_HEADROOM                        (1U << 18) /* Enables the voltage headroom loop in the LDO_VSYS. */
#define CRG_VSYS_CURLIM_OFFSET_TRIM_MASK            (0x1F << 13) /* For adjusting the offset of the curlim range ( +/- 78 mA) 0x0 = maximum positive */
#define CRG_VSYS_CURLIM_OFFSET_TRIM_POS             (13) /* For adjusting the offset of the curlim range ( +/- 78 mA) 0x0 = maximum positive */
#define CRG_VSYS_CURLIM_GAIN_TRIM_MASK              (0x1F << 8) /* For adjusting the gain of the curlim range ( +/- 20%) 0x0 = maximum gain. 0x10 = */
#define CRG_VSYS_CURLIM_GAIN_TRIM_POS               (8) /* For adjusting the gain of the curlim range ( +/- 20%) 0x0 = maximum gain. 0x10 = */
#define CRG_VSYS_CURLIM_SET_MASK                    (0x7F << 1) /* Sets the level of the LDO_VSYS current limiter in 10 mA steps. It is reset when  */
#define CRG_VSYS_CURLIM_SET_POS                     (1) /* Sets the level of the LDO_VSYS current limiter in 10 mA steps. It is reset when  */
#define CRG_VSYS_EN_CURLIM                          (1U << 0) /* Enables the current limiter in the LDO_VSYS */

/* VSYS_GEN_IRQ_CLEAR Register */

#define CRG_VSYS_VBUS_LOW_DRIVE_IRQ_CLEAR           (1U << 1) /* Clears VBUS_LOW_DRIVE_IRQ */
#define CRG_VSYS_LDO_VSYS_HIGH_TEMP_IRQ_CLEAR       (1U << 0) /* Clears LDO_VSYS_HIGH_TEMP_IRQ */

/* VSYS_GEN_IRQ_MASK Register */

#define CRG_VSYS_VBUS_LOW_DRIVE_IRQ_MASK            (1U << 1) /* Masks VBUS_LOW_DRIVE_IRQ interrupt. It is reset when vbus is not plugged in (COM */
#define CRG_VSYS_LDO_VSYS_HIGH_TEMP_IRQ_MASK        (1U << 0) /* Masks LDO_VSYS_HIGH_TEMP_IRQ interrupt */

/* VSYS_GEN_IRQ_STATUS Register */

#define CRG_VSYS_VBUS_LOW_DRIVE_IRQ_STATUS          (1U << 1) /* Indicates vbus drive strength is not enough to keep vbus up with the set limit o */
#define CRG_VSYS_LDO_VSYS_HIGH_TEMP_IRQ_STATUS      (1U << 0) /* Indicates that a high temperature has been detected at ldo_vsys */

#endif /* __DA1470_CRG_VSYS_H */
