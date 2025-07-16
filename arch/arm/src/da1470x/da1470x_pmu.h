/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_pmu.h
 *
 * Power Management Unit (PMU) Register Definitions for DA1470x
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

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_PMU_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_PMU_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

// /* Register Base Addresses */

// #define DA1470_CRG_TOP_POWER_CTRL        0x500000F0
// #define DA1470_CRG_TOP_POWER_LVL         0x500000F8
// #define DA1470_CRG_TOP_ANA_STATUS        0x500000EC

// /* POWER_CTRL_REG Bit Definitions */

// #define CRG_TOP_POWER_CTRL_DCDC_V12_EN   (1 << 1)
// #define CRG_TOP_POWER_CTRL_DCDC_V18_EN   (1 << 3)
// #define CRG_TOP_POWER_CTRL_LDO_V30_EN    (1 << 5)

// /* POWER_LVL_REG Field Positions and Masks */

#define POWER_LVL_REG_V12_LEVEL_POS      0
#define POWER_LVL_REG_V12_LEVEL_MASK     (0xF << POWER_LVL_REG_V12_LEVEL_POS)
#define POWER_LVL_REG_V12_LEVEL_1P2V     0x3

#define POWER_LVL_REG_V18_LEVEL_POS      4
#define POWER_LVL_REG_V18_LEVEL_MASK     (0xF << POWER_LVL_REG_V18_LEVEL_POS)
#define POWER_LVL_REG_V18_LEVEL_1P8V     0x5

#define POWER_LVL_REG_V30_LEVEL_POS      8
#define POWER_LVL_REG_V30_LEVEL_MASK     (0xF << POWER_LVL_REG_V30_LEVEL_POS)
#define POWER_LVL_REG_V30_LEVEL_3P0V     0xA

// /* ANA_STATUS_REG Bit Definitions */

// #define CRG_TOP_BUCK_DCDC_V12_OK         (1 << 1)
// #define CRG_TOP_BUCK_DCDC_V18_OK         (1 << 3)
// #define CRG_TOP_LDO_V30_OK               (1 << 5)

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/* Initialize power domains (V12, V18, V30) for DA1470x */

void da1470x_pwr_init(void);

void da1470x_pmu_set_1v2_max(void);

void da1470x_pmu_enable_v12_if_needed(void);

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_PMU_H */
