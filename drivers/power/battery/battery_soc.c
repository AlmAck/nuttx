/****************************************************************************
 * drivers/power/battery/battery_soc.c
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <stddef.h>
#include <stdint.h>

#include <nuttx/power/battery_soc.h>

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* Open-circuit voltage against state of charge for a single-cell LiPo,
 * highest first.  Interpolated linearly between points.
 *
 * The curve is deliberately dense at the ends and sparse through the middle
 * because that is where the cell chemistry puts the information: below about
 * 3.6 V and above about 4.1 V a small voltage change means a large capacity
 * change, while 3.7-3.9 V is nearly flat and carries most of the capacity.
 * That flat region is exactly why a voltage-only estimate cannot be better
 * than a few percent there, and why a coulomb counter is worth having when
 * the hardware offers one.
 */

struct soc_point_s
{
  int16_t mv;
  int8_t  pct;
};

/* Single-cell LiCoO2 ("LiPo"), 4.2 V fully charged.  The default, and the
 * right curve for the overwhelming majority of wearable cells.
 */

static const struct soc_point_s g_ocv_lipo42[] =
{
  { 4200, 100 },
  { 4150,  95 },
  { 4110,  90 },
  { 4080,  85 },
  { 4020,  80 },
  { 3980,  75 },
  { 3950,  70 },
  { 3910,  65 },
  { 3870,  60 },
  { 3850,  55 },
  { 3840,  50 },
  { 3820,  45 },
  { 3800,  40 },
  { 3790,  35 },
  { 3770,  30 },
  { 3750,  25 },
  { 3730,  20 },
  { 3710,  15 },
  { 3690,  10 },
  { 3610,   5 },
  { 3270,   0 },
};

/* High-voltage LiCoO2, 4.35 V fully charged.  Same chemistry shape as
 * above with the top of the curve stretched; using the 4.2 V table on one
 * of these pins it at 100% for the whole first stretch of the discharge.
 */

static const struct soc_point_s g_ocv_lipo_hv[] =
{
  { 4350, 100 },
  { 4300,  95 },
  { 4250,  90 },
  { 4200,  85 },
  { 4150,  80 },
  { 4100,  75 },
  { 4050,  70 },
  { 4000,  65 },
  { 3950,  60 },
  { 3900,  55 },
  { 3870,  50 },
  { 3840,  45 },
  { 3820,  40 },
  { 3800,  35 },
  { 3780,  30 },
  { 3760,  25 },
  { 3740,  20 },
  { 3720,  15 },
  { 3690,  10 },
  { 3610,   5 },
  { 3270,   0 },
};

/* LiFePO4, 3.65 V fully charged.  Far flatter than LiCoO2 - roughly 80% of
 * the capacity sits inside a 60 mV band - so a voltage-only estimate on
 * this chemistry is close to useless through the middle and only means
 * anything at the two ends.  Selectable because reporting a LiFePO4 cell
 * against a LiPo curve reads as permanently flat, which is worse.
 */

static const struct soc_point_s g_ocv_lifepo4[] =
{
  { 3650, 100 },
  { 3400,  95 },
  { 3330,  90 },
  { 3300,  80 },
  { 3280,  70 },
  { 3270,  60 },
  { 3260,  50 },
  { 3250,  40 },
  { 3230,  30 },
  { 3200,  20 },
  { 3150,  10 },
  { 3000,   5 },
  { 2800,   0 },
};

struct soc_curve_s
{
  FAR const struct soc_point_s *pts;
  size_t                        npts;
};

static const struct soc_curve_s g_soc_curves[] =
{
  [BATTERY_SOC_MODEL_LIPO_42] =
  {
    g_ocv_lipo42, sizeof(g_ocv_lipo42) / sizeof(g_ocv_lipo42[0])
  },
  [BATTERY_SOC_MODEL_LIPO_HV] =
  {
    g_ocv_lipo_hv, sizeof(g_ocv_lipo_hv) / sizeof(g_ocv_lipo_hv[0])
  },
  [BATTERY_SOC_MODEL_LIFEPO4] =
  {
    g_ocv_lifepo4, sizeof(g_ocv_lifepo4) / sizeof(g_ocv_lifepo4[0])
  },
};

#define SOC_NMODELS (sizeof(g_soc_curves) / sizeof(g_soc_curves[0]))

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: battery_soc_from_ocv_model
 ****************************************************************************/

int battery_soc_from_ocv_model(int mv, int model)
{
  FAR const struct soc_point_s *pts;
  size_t npts;
  size_t i;

  if (model < 0 || (size_t)model >= SOC_NMODELS ||
      g_soc_curves[model].pts == NULL)
    {
      model = BATTERY_SOC_MODEL_DEFAULT;
    }

  pts  = g_soc_curves[model].pts;
  npts = g_soc_curves[model].npts;

  if (mv >= pts[0].mv)
    {
      return 100;
    }

  if (mv <= pts[npts - 1].mv)
    {
      return 0;
    }

  for (i = 1; i < npts; i++)
    {
      if (mv >= pts[i].mv)
        {
          /* Between points i-1 (higher) and i (lower): interpolate. */

          int span_mv  = pts[i - 1].mv  - pts[i].mv;
          int span_pct = pts[i - 1].pct - pts[i].pct;

          if (span_mv <= 0)
            {
              return pts[i].pct;
            }

          return pts[i].pct + ((mv - pts[i].mv) * span_pct) / span_mv;
        }
    }

  return 0;
}

/****************************************************************************
 * Name: battery_soc_from_ocv
 ****************************************************************************/

int battery_soc_from_ocv(int mv)
{
  return battery_soc_from_ocv_model(mv, BATTERY_SOC_MODEL_DEFAULT);
}

/****************************************************************************
 * Name: battery_soc_from_loaded_model
 ****************************************************************************/

int battery_soc_from_loaded_model(int mv, int ua, int esr_mohm, int model)
{
  int drop_mv;

  if (esr_mohm <= 0)
    {
      esr_mohm = BATTERY_SOC_DEFAULT_ESR_MOHM;
    }

  /* V_ocv = V_terminal - I * R, with I positive while charging.  Charging
   * lifts the terminal voltage above open circuit and discharging pulls it
   * below, so subtracting the signed IR drop moves the sample back toward
   * the rested curve in both directions.
   *
   * uA * mohm = nV; divide by 1e6 for millivolts.  Use a 64-bit
   * intermediate: 800 mA into 150 mohm is 1.2e11 nV.
   */

  drop_mv = (int)(((int64_t)ua * (int64_t)esr_mohm) / 1000000);

  return battery_soc_from_ocv_model(mv - drop_mv, model);
}

/****************************************************************************
 * Name: battery_soc_from_loaded
 ****************************************************************************/

int battery_soc_from_loaded(int mv, int ua, int esr_mohm)
{
  return battery_soc_from_loaded_model(mv, ua, esr_mohm,
                                       BATTERY_SOC_MODEL_DEFAULT);
}
