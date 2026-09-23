/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_dcdc.h
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

/* Generated from the Renesas CMSIS device header DA1470x-00.h (SDK
 * 10.2.6.49).  Do not edit by hand; regenerate with tools/genhdr.py.
 */

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_DCDC_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_DCDC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470x_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define DA1470X_DCDC_BUCK_CTRL_OFFSET 0x0000 /* DCDC Buck Control Register */

/* Register Addresses *******************************************************/

#define DA1470X_DCDC_BUCK_CTRL (DA1470X_DCDC_BASE + DA1470X_DCDC_BUCK_CTRL_OFFSET)

/* Register Bit-field Definitions *******************************************/

/* BUCK_CTRL register */

#define DCDC_BUCK_CTRL_CFG_HYST_SHIFT (0)       /* Bits 0-1 */
#define DCDC_BUCK_CTRL_CFG_HYST_MASK (0x3 << DCDC_BUCK_CTRL_CFG_HYST_SHIFT)
#  define DCDC_BUCK_CTRL_CFG_HYST(n) ((uint32_t)(n) << DCDC_BUCK_CTRL_CFG_HYST_SHIFT)
#define DCDC_BUCK_CTRL_CFG_IMAX_LOWER_SHIFT (2) /* Bits 2-4 */
#define DCDC_BUCK_CTRL_CFG_IMAX_LOWER_MASK (0x7 << DCDC_BUCK_CTRL_CFG_IMAX_LOWER_SHIFT)
#  define DCDC_BUCK_CTRL_CFG_IMAX_LOWER(n) ((uint32_t)(n) << DCDC_BUCK_CTRL_CFG_IMAX_LOWER_SHIFT)
#define DCDC_BUCK_CTRL_CFG_IMAX_UPPER_SHIFT (5) /* Bits 5-7 */
#define DCDC_BUCK_CTRL_CFG_IMAX_UPPER_MASK (0x7 << DCDC_BUCK_CTRL_CFG_IMAX_UPPER_SHIFT)
#  define DCDC_BUCK_CTRL_CFG_IMAX_UPPER(n) ((uint32_t)(n) << DCDC_BUCK_CTRL_CFG_IMAX_UPPER_SHIFT)
#define DCDC_BUCK_CTRL_TRIM_BIAS_SHIFT (10)     /* Bits 10-12 */
#define DCDC_BUCK_CTRL_TRIM_BIAS_MASK (0x7 << DCDC_BUCK_CTRL_TRIM_BIAS_SHIFT)
#  define DCDC_BUCK_CTRL_TRIM_BIAS(n) ((uint32_t)(n) << DCDC_BUCK_CTRL_TRIM_BIAS_SHIFT)
#define DCDC_BUCK_CTRL_EN_FREEWHEEL (1 << 13)   /* Bit 13 */
#define DCDC_BUCK_CTRL_CFG_TIMEOUT_SHIFT (14)   /* Bits 14-15 */
#define DCDC_BUCK_CTRL_CFG_TIMEOUT_MASK (0x3 << DCDC_BUCK_CTRL_CFG_TIMEOUT_SHIFT)
#  define DCDC_BUCK_CTRL_CFG_TIMEOUT(n) ((uint32_t)(n) << DCDC_BUCK_CTRL_CFG_TIMEOUT_SHIFT)

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_DCDC_H */
