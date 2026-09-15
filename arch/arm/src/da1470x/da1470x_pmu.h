/****************************************************************************
 * arch/arm/src/da1470x/da1470x_pmu.h
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

#include <stdbool.h>
#include <stdint.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* POWER_LVL_REG encodings */

#define DA1470X_V12_LEVEL_0P75V    0x0
#define DA1470X_V12_LEVEL_0P90V    0x1
#define DA1470X_V12_LEVEL_1P20V    0x3
#define DA1470X_V30_LEVEL_3P00V    0x8
#define DA1470X_V30_LEVEL_3P30V    0xa

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* Power-domain identifiers.  Each domain is gated through a
 * PMU_CTRL.<x>_SLEEP bit, readiness is reported by SYS_STAT.<x>_IS_UP.
 * PD_SYS hosts the M33 and is handled by the PM code only; PD_MEM follows
 * PD_SYS and has no SLEEP bit.
 */

enum da1470x_pd_e
{
  DA1470X_PD_SNC = 0,   /* Sensor node: UART, SPI, I2C, DMA, GPADC */
  DA1470X_PD_TIM,       /* General purpose timers, PWM LED */
  DA1470X_PD_AUD,       /* Audio */
  DA1470X_PD_GPU,       /* GPU and LCD controller */
  DA1470X_PD_CTRL,      /* External memory controller (QSPIC2 PSRAM) */
  DA1470X_PD_RAD,       /* Radio and CMAC */
  DA1470X_NPD
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_pwr_init
 *
 * Description:
 *   Enable the V12, V18 and V30 rails at their nominal levels and wait for
 *   them to settle.  Called once from __start before any peripheral is
 *   touched.
 *
 ****************************************************************************/

void da1470x_pwr_init(void);

/****************************************************************************
 * Name: da1470x_pmu_set_v12
 *
 * Description:
 *   Program the V12 rail level (DA1470X_V12_LEVEL_*) and wait until the
 *   DCDC reports the rail OK.  1.2 V is required for RCHS_64/96 and PLL.
 *
 ****************************************************************************/

int da1470x_pmu_set_v12(uint32_t level);

/****************************************************************************
 * Name: da1470x_pd_enable / da1470x_pd_disable / da1470x_pd_is_up
 *
 * Description:
 *   Power-domain control.  da1470x_pd_enable() blocks (bounded) until the
 *   domain reports IS_UP and returns -ETIMEDOUT if it never does.
 *
 ****************************************************************************/

int  da1470x_pd_enable(enum da1470x_pd_e pd);
void da1470x_pd_disable(enum da1470x_pd_e pd);
bool da1470x_pd_is_up(enum da1470x_pd_e pd);

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_PMU_H */
