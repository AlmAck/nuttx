/****************************************************************************
 * include/nuttx/power/npm1300.h
 * Nordic nPM1300 PMIC: charger + battery gauge.
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

#ifndef __INCLUDE_NUTTX_POWER_NPM1300_H
#define __INCLUDE_NUTTX_POWER_NPM1300_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/i2c/i2c_master.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Fixed 7-bit I2C address.  Nordic's own boards describe the part as
 * "npm1300@6b" and the address is not strappable.
 */

#define NPM1300_I2C_ADDR 0x6b

/* Charger configuration limits, from the register encodings:
 *
 *   VTERM  3500-3650 mV in 50 mV steps, then 4000-4450 mV in 50 mV steps.
 *          The gap between 3650 and 4000 mV is not representable.
 *   ICHG   32-800 mA in 2 mA steps.
 */

#define NPM1300_VTERM_MIN_MV 3500
#define NPM1300_VTERM_MAX_MV 4450
#define NPM1300_ICHG_MIN_MA    32
#define NPM1300_ICHG_MAX_MA   800

/* BUCK1/BUCK2 and the two load switches share one encoding: 1.0 V plus
 * 100 mV per step, 1.0-3.3 V.  A buck ignores its VOUT register until
 * software takes control of it (BUCKSWCTRLSEL); out of reset each rail
 * follows its external VSETx resistor pins instead.
 */

#define NPM1300_VOUT_MIN_MV  1000
#define NPM1300_VOUT_MAX_MV  3300
#define NPM1300_VOUT_STEP_MV  100

/* Watchdog timer: 24-bit count of 16 ms ticks. */

#define NPM1300_WDT_TICK_MS        16
#define NPM1300_WDT_MAX_TICKS  0xffffff
#define NPM1300_WDT_MAX_MS     (NPM1300_WDT_MAX_TICKS * NPM1300_WDT_TICK_MS)

/* Watchdog expiry action, written to the timer MODE register. */

#define NPM1300_WDT_MODE_WARN   1  /* Pulse the interrupt line only */
#define NPM1300_WDT_MODE_RESET  2  /* Power-cycle the whole system */

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* Charger setup applied at registration.  Zero any field to leave the
 * charger's reset default in place for it.
 */

struct npm1300_config_s
{
  uint16_t vterm_mv;      /* Termination voltage, see the limits above */
  uint16_t ichg_ma;       /* Charge current */
  bool     enable_charge; /* Enable charging once configured */
  bool     disable_ntc;   /* Set when no NTC thermistor is fitted; without
                           * this the charger refuses to run because it
                           * reads the missing thermistor as out of range */
  uint16_t esr_mohm;      /* Cell internal resistance for the state-of-charge
                           * estimate, 0 for the default */

  /* Buck rails.  0 leaves the rail exactly as it is - still following its
   * VSETx pins - which is the safe default: these rails usually power the
   * host that is doing the programming.  A non-zero value switches that
   * rail to software control, programs it and enables it.
   */

  uint16_t buck1_mv;
  uint16_t buck2_mv;
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: npm1300_wdt_register
 *
 * Description:
 *   Register the PMIC's system watchdog as a NuttX watchdog device.  This
 *   only makes the device available; it does not start it.  Starting a
 *   watchdog in NPM1300_WDT_MODE_RESET that nothing then feeds will
 *   power-cycle the board, so the decision to start belongs to whoever
 *   owns the feeding.
 *
 *   Unlike the SoC's own watchdog this one survives a CPU lockup that also
 *   wedges the SoC watchdog, and its reset action cycles the rails rather
 *   than just the core.
 *
 * Input Parameters:
 *   i2c     - I2C bus the PMIC is on.
 *   devpath - Device path, e.g. "/dev/watchdog1".
 *   mode    - NPM1300_WDT_MODE_WARN or NPM1300_WDT_MODE_RESET.
 *
 ****************************************************************************/

struct i2c_master_s;
int npm1300_wdt_register(FAR struct i2c_master_s *i2c,
                         FAR const char *devpath, uint8_t mode);

#ifdef __cplusplus
extern "C"
{
#endif

/****************************************************************************
 * Name: npm1300_register
 *
 * Description:
 *   Probe the nPM1300, apply the charger configuration and register it as a
 *   battery gauge device.
 *
 *   The nPM1300 has no fuel gauge in hardware - it measures battery voltage,
 *   current and temperature, and Nordic's coulomb-counting gauge is a
 *   separate library shipped with nRF Connect SDK.  BATIOC_CAPACITY is
 *   therefore answered from the voltage curve in battery_soc.c, compensated
 *   for the IR drop using the measured battery current.  Treat it as an
 *   estimate, not a gauge reading.
 *
 * Input Parameters:
 *   i2c     - An instance of the I2C interface the PMIC is on
 *   devpath - Device path for the gauge, e.g. "/dev/bat0"
 *   config  - Charger setup, or NULL to leave the charger untouched
 *
 * Returned Value:
 *   Zero on success, a negated errno otherwise.  -ENODEV means nothing
 *   answered at the PMIC's address.
 *
 ****************************************************************************/

int npm1300_register(FAR struct i2c_master_s *i2c, FAR const char *devpath,
                     FAR const struct npm1300_config_s *config);

#ifdef __cplusplus
}
#endif

#endif /* __INCLUDE_NUTTX_POWER_NPM1300_H */
