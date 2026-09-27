/****************************************************************************
 * include/nuttx/power/battery_soc.h
 * Voltage-based state-of-charge estimation for PMICs without a fuel gauge.
 *
 * SPDX-License-Identifier: Apache-2.0
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

#ifndef __INCLUDE_NUTTX_POWER_BATTERY_SOC_H
#define __INCLUDE_NUTTX_POWER_BATTERY_SOC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdint.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Typical internal resistance of a small single-cell LiPo pack, in
 * milliohms.  Used to undo the IR drop under load before looking the
 * voltage up on the open-circuit curve.  Wildly cell-dependent; this is a
 * reasonable default for a wearable-sized cell.
 */

#define BATTERY_SOC_DEFAULT_ESR_MOHM 150

/* Discharge-curve models.  The nPM1300 and friends have no coulomb
 * counter, so "the fuel gauge model" here means which open-circuit
 * voltage curve the estimate is looked up on - and that has to match the
 * cell chemistry, because a 4.35 V cell read against a 4.2 V curve pins
 * at 100% for the first third of its discharge, and a LiFePO4 cell read
 * against either reads permanently flat.
 *
 * Select the default with CONFIG_BATTERY_SOC_MODEL_* (menuconfig:
 * Device Drivers -> Power Management -> Battery -> Voltage-based
 * state-of-charge estimator -> Cell model).  Drivers that know their own
 * cell can override per device with the _model() variants.
 */

enum battery_soc_model_e
{
  BATTERY_SOC_MODEL_LIPO_42 = 0,  /* LiCoO2, 4.2 V full - the usual case */
  BATTERY_SOC_MODEL_LIPO_HV = 1,  /* High-voltage LiCoO2, 4.35 V full */
  BATTERY_SOC_MODEL_LIFEPO4 = 2,  /* LiFePO4, 3.65 V full */
};

#if defined(CONFIG_BATTERY_SOC_MODEL_LIPO_HV)
#  define BATTERY_SOC_MODEL_DEFAULT BATTERY_SOC_MODEL_LIPO_HV
#elif defined(CONFIG_BATTERY_SOC_MODEL_LIFEPO4)
#  define BATTERY_SOC_MODEL_DEFAULT BATTERY_SOC_MODEL_LIFEPO4
#else
#  define BATTERY_SOC_MODEL_DEFAULT BATTERY_SOC_MODEL_LIPO_42
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

/****************************************************************************
 * Name: battery_soc_from_ocv
 *
 * Description:
 *   Estimate state of charge from an open-circuit terminal voltage, using a
 *   piecewise-linear discharge curve for a single-cell LiPo.
 *
 *   This is a coarse estimate and it is meant to be.  A voltage curve cannot
 *   distinguish a rested cell from a loaded one, and the flat middle of a
 *   LiPo discharge curve (roughly 3.7-3.9 V) covers most of the usable
 *   capacity, so expect several percent of error there and much better
 *   accuracy near the ends.  Where the PMIC has a real coulomb-counting
 *   gauge, prefer it and keep this for the window before it converges.
 *
 * Input Parameters:
 *   mv - Open-circuit cell voltage in millivolts.
 *
 * Returned Value:
 *   State of charge, 0-100.
 *
 ****************************************************************************/

int battery_soc_from_ocv(int mv);

/****************************************************************************
 * Name: battery_soc_from_loaded
 *
 * Description:
 *   As battery_soc_from_ocv(), but first compensates the measured terminal
 *   voltage for the IR drop across the cell's internal resistance, so a
 *   sample taken under load is not read as a flatter battery than it is.
 *
 *   Charging currents are positive and discharging currents negative, which
 *   matches the sign convention of the nPM1300's IBAT measurement.
 *
 * Input Parameters:
 *   mv        - Measured terminal voltage in millivolts.
 *   ua        - Battery current in microamps: positive charging, negative
 *               discharging.  Pass 0 if unknown, which degrades this to
 *               battery_soc_from_ocv().
 *   esr_mohm  - Cell internal resistance in milliohms, or 0 to use
 *               BATTERY_SOC_DEFAULT_ESR_MOHM.
 *
 * Returned Value:
 *   State of charge, 0-100.
 *
 ****************************************************************************/

int battery_soc_from_loaded(int mv, int ua, int esr_mohm);

/****************************************************************************
 * Name: battery_soc_from_ocv_model / battery_soc_from_loaded_model
 *
 * Description:
 *   As above, but against an explicit enum battery_soc_model_e curve
 *   instead of the one CONFIG_BATTERY_SOC_MODEL_* selected.  An
 *   out-of-range model falls back to the configured default rather than
 *   failing, so a driver may pass a value straight through from board
 *   data without validating it first.
 *
 ****************************************************************************/

int battery_soc_from_ocv_model(int mv, int model);
int battery_soc_from_loaded_model(int mv, int ua, int esr_mohm, int model);

#ifdef __cplusplus
}
#endif

#endif /* __INCLUDE_NUTTX_POWER_BATTERY_SOC_H */
