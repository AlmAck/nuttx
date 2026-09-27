/****************************************************************************
 * drivers/power/battery/npm1300.c
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

/* Register map and encodings follow Zephyr's npm13xx drivers (Apache-2.0):
 * drivers/mfd/mfd_npm13xx.c and drivers/sensor/nordic/npm13xx_charger/.
 * This is an independent implementation against NuttX's battery_gauge
 * interface, not a port of that code.
 *
 * Every register access is a two-byte address - peripheral base, then the
 * offset within it - followed by the data.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <debug.h>

#include <nuttx/kmalloc.h>
#include <nuttx/mutex.h>
#include <nuttx/signal.h>
#include <nuttx/i2c/i2c_master.h>
#include <nuttx/power/battery_gauge.h>
#include <nuttx/power/battery_ioctl.h>
#include <nuttx/power/battery_soc.h>
#include <nuttx/power/npm1300.h>
#ifdef CONFIG_NPM1300_WATCHDOG
#  include <nuttx/timers/watchdog.h>
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifndef CONFIG_NPM1300_I2C_FREQUENCY
#  define CONFIG_NPM1300_I2C_FREQUENCY 400000
#endif

/* Peripheral bases */

#define NPM_BASE_MAIN  0x00
#define NPM_BASE_VBUS  0x02
#define NPM_BASE_CHGR  0x03
#define NPM_BASE_BUCK  0x04
#define NPM_BASE_ADC   0x05
#define NPM_BASE_TIME  0x07

/* CHGR offsets */

#define CHGR_ERR_CLR       0x00
#define CHGR_EN_SET        0x04
#define CHGR_EN_CLR        0x05
#define CHGR_DIS_SET       0x06
#define CHGR_ISET          0x08
#define CHGR_VTERM         0x0c
#define CHGR_VTERM_R       0x0d
#define CHGR_CHG_STAT      0x34
#define CHGR_ERR_REASON    0x36

/* CHG_STAT bits (nPM1300 datasheet BCHGCHARGESTATUS) */

#define CHG_STAT_BATTDETECTED  (1 << 0)
#define CHG_STAT_COMPLETED     (1 << 1)
#define CHG_STAT_TRICKLE       (1 << 2)
#define CHG_STAT_CC            (1 << 3)
#define CHG_STAT_CV            (1 << 4)

/* CHGR_DIS_SET bits */

#define CHGR_DIS_RECHARGE      (1 << 0)
#define CHGR_DIS_NTC           (1 << 1)

/* VBUS offsets */

#define VBUS_STATUS        0x07
#define VBUS_STATUS_PRESENT (1 << 0)

/* BUCK offsets.  BUCK2's enable/voltage registers sit two on from BUCK1's,
 * so one set of offsets plus a channel stride covers both.
 */

#define BUCK_EN_SET        0x00
#define BUCK_EN_CLR        0x01
#define BUCK_VOUT_NORM     0x08
#define BUCK_SW_CTRL       0x0f
#define BUCK_VOUT_STATUS   0x10
#define BUCK_CH_STRIDE     0x02

/* VOUT encoding, shared with the load switches: 1.0 V + 100 mV per step. */

#define VOUT_CODE(mv)  (((mv) - NPM1300_VOUT_MIN_MV) / NPM1300_VOUT_STEP_MV)
#define VOUT_CODE_MAX  VOUT_CODE(NPM1300_VOUT_MAX_MV)

/* TIMER offsets.  The timer doubles as the system watchdog. */

#define TIME_TASK_START    0x00
#define TIME_TASK_STOP     0x01
#define TIME_TASK_LOAD     0x03
#define TIME_TASK_KICK     0x04
#define TIME_MODE          0x05
#define TIME_VALUE         0x08   /* 24-bit, big endian, 16 ms per tick */

/* MAIN event block.  Events latch until the bit is written back to the
 * matching CLR register one offset up.
 */

#define MAIN_EVENT_WDT     0x12
#define MAIN_EVENT_WDT_CLR 0x13
#define MAIN_EVENT_WDT_BIT 0x08

/* ADC offsets */

#define ADC_TASK_VBAT      0x00
#define ADC_TASK_AUTO      0x0c
#define ADC_RESULTS        0x10
#define ADC_IBAT_EN        0x24

/* ADC results block, read as one burst from ADC_RESULTS */

#define ADC_RES_IBAT_STAT  0
#define ADC_RES_MSB_VBAT   1
#define ADC_RES_LSB_A      5
#define ADC_RES_MSB_IBAT   8
#define ADC_RES_LSB_B      10
#define ADC_RES_LEN        11

#define ADC_MSB_SHIFT      2
#define ADC_LSB_MASK       0x03
#define ADC_LSB_VBAT_SHIFT 0
#define ADC_LSB_IBAT_SHIFT 4

/* One ADC conversion takes 250us; we trigger VBAT and let the burst read
 * pick up whatever the auto-measure loop has for current.
 */

#define ADC_CONV_DELAY_US  (250 * 4)

/* VBAT is a 10-bit result against a 5000 mV full scale. */

#define ADC_VFS_MV         5000
#define ADC_FULL_SCALE     1024

/* IBAT full-scale discharge factor, 112/100 of the configured charge
 * current (Zephyr: NPM1300_FULL_SCALE_DISCHARGE_FACTORS).
 */

#define IBAT_DISCHG_NUM    112
#define IBAT_DISCHG_DEN    100
#define IBAT_CHG_NUM       125
#define IBAT_CHG_DEN       100

/* IBAT_STAT encodes what the current measurement means. */

#define IBAT_STAT_DISCHARGING 0x04
#define IBAT_STAT_CHARGING    0x0c

/* VTERM encoding: two linear runs with an unrepresentable gap between. */

#define VTERM_LO_BASE_MV   3500
#define VTERM_LO_MAX_MV    3650
#define VTERM_HI_BASE_MV   4000
#define VTERM_HI_MAX_MV    4450
#define VTERM_STEP_MV        50
#define VTERM_HI_BASE_IDX     4

/* ICHG encoding: index 16 is 32 mA, 2 mA per step.  The register pair holds
 * idx/2 and idx&1.
 */

#define ICHG_BASE_IDX        16
#define ICHG_BASE_UA      32000
#define ICHG_STEP_UA       2000

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct npm1300_dev_s
{
  struct battery_gauge_dev_s     gauge; /* Must be first */
  FAR struct i2c_master_s       *i2c;
  mutex_t                        lock;
  uint16_t                       ichg_ma;   /* As configured, for IBAT scale */
  uint16_t                       esr_mohm;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int npm1300_state(FAR struct battery_gauge_dev_s *lower,
                         FAR int *status);
static int npm1300_online(FAR struct battery_gauge_dev_s *lower,
                          FAR bool *status);
static int npm1300_voltage(FAR struct battery_gauge_dev_s *lower,
                           FAR int *value);
static int npm1300_capacity(FAR struct battery_gauge_dev_s *lower,
                            FAR int *value);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct battery_gauge_operations_s g_npm1300_gaugeops =
{
  .state    = npm1300_state,
  .online   = npm1300_online,
  .voltage  = npm1300_voltage,
  .capacity = npm1300_capacity,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: reg_write
 *
 * Description:
 *   Write one register: [base, offset, value].
 *
 ****************************************************************************/

static int reg_write(FAR struct npm1300_dev_s *dev, uint8_t base,
                     uint8_t offset, uint8_t value)
{
  struct i2c_msg_s msg;
  uint8_t buf[3];

  buf[0] = base;
  buf[1] = offset;
  buf[2] = value;

  msg.frequency = CONFIG_NPM1300_I2C_FREQUENCY;
  msg.addr      = NPM1300_I2C_ADDR;
  msg.flags     = 0;
  msg.buffer    = buf;
  msg.length    = sizeof(buf);

  return I2C_TRANSFER(dev->i2c, &msg, 1);
}

/****************************************************************************
 * Name: reg_read_burst
 *
 * Description:
 *   Address with [base, offset], then read len bytes.
 *
 ****************************************************************************/

static int reg_read_burst(FAR struct npm1300_dev_s *dev, uint8_t base,
                          uint8_t offset, FAR uint8_t *data, size_t len)
{
  struct i2c_msg_s msg[2];
  uint8_t addr[2];

  addr[0] = base;
  addr[1] = offset;

  msg[0].frequency = CONFIG_NPM1300_I2C_FREQUENCY;
  msg[0].addr      = NPM1300_I2C_ADDR;
  msg[0].flags     = 0;
  msg[0].buffer    = addr;
  msg[0].length    = sizeof(addr);

  msg[1].frequency = CONFIG_NPM1300_I2C_FREQUENCY;
  msg[1].addr      = NPM1300_I2C_ADDR;
  msg[1].flags     = I2C_M_READ;
  msg[1].buffer    = data;
  msg[1].length    = len;

  return I2C_TRANSFER(dev->i2c, msg, 2);
}

static int reg_read(FAR struct npm1300_dev_s *dev, uint8_t base,
                    uint8_t offset, FAR uint8_t *value)
{
  return reg_read_burst(dev, base, offset, value, 1);
}

/****************************************************************************
 * Name: reg_write_burst
 *
 * Description:
 *   Write [base, offset] followed by len payload bytes in one transfer.
 *
 ****************************************************************************/

static int reg_write_burst(FAR struct i2c_master_s *i2c, uint8_t base,
                           uint8_t offset, FAR const uint8_t *data,
                           size_t len)
{
  uint8_t buf[8];
  struct i2c_msg_s msg;

  DEBUGASSERT(len <= sizeof(buf) - 2);

  buf[0] = base;
  buf[1] = offset;
  memcpy(buf + 2, data, len);

  msg.frequency = CONFIG_NPM1300_I2C_FREQUENCY;
  msg.addr      = NPM1300_I2C_ADDR;
  msg.flags     = 0;
  msg.buffer    = buf;
  msg.length    = len + 2;

  return I2C_TRANSFER(i2c, &msg, 1);
}

/****************************************************************************
 * Name: npm1300_set_buck
 *
 * Description:
 *   Program one buck rail and enable it.  Out of reset a buck follows its
 *   external VSETx resistor pins and ignores VOUT_NORM entirely; the
 *   SW_CTRL bit is what hands the rail over to the register, so the order
 *   here is voltage first, then hand-over, then enable.  Setting the
 *   voltage after the hand-over would briefly output whatever VOUT_NORM
 *   happened to hold.
 *
 * Input Parameters:
 *   ch - 0 for BUCK1, 1 for BUCK2.
 *
 ****************************************************************************/

static int npm1300_set_buck(FAR struct npm1300_dev_s *dev, int ch,
                            uint16_t mv)
{
  uint8_t stride = (uint8_t)(ch * BUCK_CH_STRIDE);
  uint8_t sw;
  int ret;

  if (mv < NPM1300_VOUT_MIN_MV || mv > NPM1300_VOUT_MAX_MV ||
      (mv % NPM1300_VOUT_STEP_MV) != 0)
    {
      baterr("ERROR: buck%d %u mV invalid (%u-%u in %u mV steps)\n",
             ch + 1, mv, NPM1300_VOUT_MIN_MV, NPM1300_VOUT_MAX_MV,
             NPM1300_VOUT_STEP_MV);
      return -EINVAL;
    }

  ret = reg_write(dev, NPM_BASE_BUCK, BUCK_VOUT_NORM + stride,
                  (uint8_t)VOUT_CODE(mv));
  if (ret < 0)
    {
      return ret;
    }

  /* SW_CTRL holds one bit per buck, so read-modify-write: taking BUCK1
   * over must not drop BUCK2 back onto its pins.
   */

  ret = reg_read(dev, NPM_BASE_BUCK, BUCK_SW_CTRL, &sw);
  if (ret < 0)
    {
      return ret;
    }

  sw |= (uint8_t)(1 << ch);

  ret = reg_write(dev, NPM_BASE_BUCK, BUCK_SW_CTRL, sw);
  if (ret < 0)
    {
      return ret;
    }

  return reg_write(dev, NPM_BASE_BUCK, BUCK_EN_SET + stride, 1);
}

/****************************************************************************
 * Name: adc_sample
 *
 * Description:
 *   Trigger a VBAT conversion and read the whole results block.  Returns
 *   the battery voltage in millivolts and, if the part has a valid current
 *   measurement, the battery current in microamps (positive charging,
 *   negative discharging).
 *
 ****************************************************************************/

static int adc_sample(FAR struct npm1300_dev_s *dev, FAR int *mv,
                      FAR int *ua)
{
  uint8_t res[ADC_RES_LEN];
  uint16_t raw;
  int ret;

  ret = reg_write(dev, NPM_BASE_ADC, ADC_TASK_VBAT, 1);
  if (ret < 0)
    {
      return ret;
    }

  nxsig_usleep(ADC_CONV_DELAY_US);

  ret = reg_read_burst(dev, NPM_BASE_ADC, ADC_RESULTS, res, sizeof(res));
  if (ret < 0)
    {
      return ret;
    }

  raw = ((uint16_t)res[ADC_RES_MSB_VBAT] << ADC_MSB_SHIFT) |
        ((res[ADC_RES_LSB_A] >> ADC_LSB_VBAT_SHIFT) & ADC_LSB_MASK);

  *mv = ((int)raw * ADC_VFS_MV) / ADC_FULL_SCALE;

  /* Current is only meaningful once the auto-measure loop has run and
   * IBAT_STAT says which direction it is flowing.  Report 0 otherwise so
   * the estimator falls back to a plain open-circuit lookup.
   */

  *ua = 0;

  if (dev->ichg_ma > 0)
    {
      uint8_t stat = res[ADC_RES_IBAT_STAT];

      raw = ((uint16_t)res[ADC_RES_MSB_IBAT] << ADC_MSB_SHIFT) |
            ((res[ADC_RES_LSB_B] >> ADC_LSB_IBAT_SHIFT) & ADC_LSB_MASK);

      if (stat == IBAT_STAT_CHARGING)
        {
          *ua = (int)(((uint32_t)raw * dev->ichg_ma * 1000 * IBAT_CHG_NUM) /
                      (ADC_FULL_SCALE * IBAT_CHG_DEN));
        }
      else if (stat == IBAT_STAT_DISCHARGING)
        {
          *ua = -(int)(((uint32_t)raw * dev->ichg_ma * 1000 *
                        IBAT_DISCHG_NUM) /
                       (ADC_FULL_SCALE * IBAT_DISCHG_DEN));
        }
    }

  return OK;
}

/****************************************************************************
 * Name: npm1300_state
 ****************************************************************************/

static int npm1300_state(FAR struct battery_gauge_dev_s *lower,
                         FAR int *status)
{
  FAR struct npm1300_dev_s *dev = (FAR struct npm1300_dev_s *)lower;
  uint8_t chg;
  int ret;

  nxmutex_lock(&dev->lock);
  ret = reg_read(dev, NPM_BASE_CHGR, CHGR_CHG_STAT, &chg);
  nxmutex_unlock(&dev->lock);

  if (ret < 0)
    {
      *status = BATTERY_UNKNOWN;
      return ret;
    }

  if ((chg & CHG_STAT_BATTDETECTED) == 0)
    {
      *status = BATTERY_UNKNOWN;
    }
  else if (chg & CHG_STAT_COMPLETED)
    {
      *status = BATTERY_FULL;
    }
  else if (chg & (CHG_STAT_TRICKLE | CHG_STAT_CC | CHG_STAT_CV))
    {
      *status = BATTERY_CHARGING;
    }
  else
    {
      *status = BATTERY_DISCHARGING;
    }

  return OK;
}

/****************************************************************************
 * Name: npm1300_online
 ****************************************************************************/

static int npm1300_online(FAR struct battery_gauge_dev_s *lower,
                          FAR bool *status)
{
  FAR struct npm1300_dev_s *dev = (FAR struct npm1300_dev_s *)lower;
  uint8_t chg;
  int ret;

  nxmutex_lock(&dev->lock);
  ret = reg_read(dev, NPM_BASE_CHGR, CHGR_CHG_STAT, &chg);
  nxmutex_unlock(&dev->lock);

  if (ret < 0)
    {
      *status = false;
      return ret;
    }

  *status = (chg & CHG_STAT_BATTDETECTED) != 0;
  return OK;
}

/****************************************************************************
 * Name: npm1300_voltage
 ****************************************************************************/

static int npm1300_voltage(FAR struct battery_gauge_dev_s *lower,
                           FAR int *value)
{
  FAR struct npm1300_dev_s *dev = (FAR struct npm1300_dev_s *)lower;
  int mv;
  int ua;
  int ret;

  nxmutex_lock(&dev->lock);
  ret = adc_sample(dev, &mv, &ua);
  nxmutex_unlock(&dev->lock);

  if (ret < 0)
    {
      return ret;
    }

  *value = mv;
  return OK;
}

/****************************************************************************
 * Name: npm1300_capacity
 *
 * Description:
 *   The nPM1300 has no fuel gauge, so this is an estimate from the voltage
 *   curve with the IR drop backed out using the measured current.  See
 *   battery_soc.c for what that is and is not good for.
 *
 ****************************************************************************/

static int npm1300_capacity(FAR struct battery_gauge_dev_s *lower,
                            FAR int *value)
{
  FAR struct npm1300_dev_s *dev = (FAR struct npm1300_dev_s *)lower;
  int mv;
  int ua;
  int ret;

  nxmutex_lock(&dev->lock);
  ret = adc_sample(dev, &mv, &ua);
  nxmutex_unlock(&dev->lock);

  if (ret < 0)
    {
      return ret;
    }

  *value = battery_soc_from_loaded(mv, ua, dev->esr_mohm);
  return OK;
}

/****************************************************************************
 * Name: npm1300_vterm_index
 *
 * Description:
 *   Map a termination voltage in mV onto the register encoding, rounding
 *   down.  Returns a negated errno if the value falls in the gap between the
 *   two representable runs.
 *
 ****************************************************************************/

static int npm1300_vterm_index(uint16_t mv)
{
  if (mv >= VTERM_HI_BASE_MV && mv <= VTERM_HI_MAX_MV)
    {
      return VTERM_HI_BASE_IDX + (mv - VTERM_HI_BASE_MV) / VTERM_STEP_MV;
    }

  if (mv >= VTERM_LO_BASE_MV && mv <= VTERM_LO_MAX_MV)
    {
      return (mv - VTERM_LO_BASE_MV) / VTERM_STEP_MV;
    }

  return -EINVAL;
}

/****************************************************************************
 * Name: npm1300_configure
 ****************************************************************************/

static int npm1300_configure(FAR struct npm1300_dev_s *dev,
                             FAR const struct npm1300_config_s *config)
{
  uint8_t dis = 0;
  int ret;

  /* Clear any latched charger error first; the charger will not start
   * while one is pending.
   */

  ret = reg_write(dev, NPM_BASE_CHGR, CHGR_ERR_CLR, 1);
  if (ret < 0)
    {
      return ret;
    }

  /* The charger must be disabled while its setpoints are changed. */

  ret = reg_write(dev, NPM_BASE_CHGR, CHGR_EN_CLR, 1);
  if (ret < 0)
    {
      return ret;
    }

  if (config->vterm_mv != 0)
    {
      int idx = npm1300_vterm_index(config->vterm_mv);

      if (idx < 0)
        {
          baterr("ERROR: vterm %u mV is not representable "
                 "(3500-3650 or 4000-4450 in 50 mV steps)\n",
                 config->vterm_mv);
          return idx;
        }

      ret = reg_write(dev, NPM_BASE_CHGR, CHGR_VTERM, (uint8_t)idx);
      if (ret < 0)
        {
          return ret;
        }

      /* Warm-battery termination tracks the normal one unless a thermal
       * profile says otherwise.
       */

      ret = reg_write(dev, NPM_BASE_CHGR, CHGR_VTERM_R, (uint8_t)idx);
      if (ret < 0)
        {
          return ret;
        }
    }

  if (config->ichg_ma != 0)
    {
      uint32_t ua = (uint32_t)config->ichg_ma * 1000;
      uint8_t pair[2];
      int idx;

      if (config->ichg_ma < NPM1300_ICHG_MIN_MA ||
          config->ichg_ma > NPM1300_ICHG_MAX_MA)
        {
          baterr("ERROR: ichg %u mA out of range (%u-%u)\n",
                 config->ichg_ma, NPM1300_ICHG_MIN_MA, NPM1300_ICHG_MAX_MA);
          return -EINVAL;
        }

      idx = ICHG_BASE_IDX + (int)((ua - ICHG_BASE_UA) / ICHG_STEP_UA);

      /* The setpoint is split across two registers: the high bits and a
       * single low bit, giving the 2 mA resolution.
       */

      pair[0] = (uint8_t)(idx / 2);
      pair[1] = (uint8_t)(idx & 1);

      ret = reg_write(dev, NPM_BASE_CHGR, CHGR_ISET, pair[0]);
      if (ret < 0)
        {
          return ret;
        }

      ret = reg_write(dev, NPM_BASE_CHGR, CHGR_ISET + 1, pair[1]);
      if (ret < 0)
        {
          return ret;
        }

      dev->ichg_ma = config->ichg_ma;
    }

  /* Without a thermistor the charger reads the NTC input as out of range
   * and refuses to charge, so a board with no NTC must say so.
   */

  if (config->disable_ntc)
    {
      dis |= CHGR_DIS_NTC;
    }

  ret = reg_write(dev, NPM_BASE_CHGR, CHGR_DIS_SET, dis);
  if (ret < 0)
    {
      return ret;
    }

  /* Rails.  Left alone unless the board explicitly asks for a voltage -
   * these usually power the host doing the programming.
   */

  if (config->buck1_mv != 0)
    {
      ret = npm1300_set_buck(dev, 0, config->buck1_mv);
      if (ret < 0)
        {
          return ret;
        }
    }

  if (config->buck2_mv != 0)
    {
      ret = npm1300_set_buck(dev, 1, config->buck2_mv);
      if (ret < 0)
        {
          return ret;
        }
    }

  /* Enable battery current measurement and the periodic auto-measurement
   * loop, so IBAT is populated for the state-of-charge estimate.
   */

  ret = reg_write(dev, NPM_BASE_ADC, ADC_IBAT_EN, 1);
  if (ret < 0)
    {
      return ret;
    }

  ret = reg_write(dev, NPM_BASE_ADC, ADC_TASK_AUTO, 1);
  if (ret < 0)
    {
      return ret;
    }

  if (config->enable_charge)
    {
      ret = reg_write(dev, NPM_BASE_CHGR, CHGR_EN_SET, 1);
      if (ret < 0)
        {
          return ret;
        }
    }

  return OK;
}

/****************************************************************************
 * Watchdog lower half
 *
 * The PMIC timer in watchdog mode is a second, independent watchdog: it
 * lives on the other side of the I2C bus, so it still fires when the SoC
 * and its on-die watchdog are both wedged, and in RESET mode it cycles the
 * rails rather than just the core.  Registered as a normal NuttX watchdog
 * so the usual WDIOC_* ioctls drive it.
 ****************************************************************************/

#ifdef CONFIG_NPM1300_WATCHDOG

struct npm1300_wdt_s
{
  struct watchdog_lowerhalf_s  lower;   /* Must be first */
  FAR struct i2c_master_s     *i2c;
  uint32_t                     timeout; /* ms, as last set */
  uint8_t                      mode;
  bool                         started;
};

/* The timer helpers take a bus rather than a gauge, so the watchdog can be
 * registered with or without the gauge.
 */

static int wdt_task(FAR struct i2c_master_s *i2c, uint8_t offset)
{
  uint8_t v = 1;

  return reg_write_burst(i2c, NPM_BASE_TIME, offset, &v, 1);
}

static int wdt_set_timeout_ms(FAR struct i2c_master_s *i2c, uint32_t ms)
{
  uint32_t ticks = ms / NPM1300_WDT_TICK_MS;
  uint8_t be24[3];
  int ret;

  if (ticks == 0 || ticks > NPM1300_WDT_MAX_TICKS)
    {
      return -ERANGE;
    }

  be24[0] = (uint8_t)(ticks >> 16);
  be24[1] = (uint8_t)(ticks >> 8);
  be24[2] = (uint8_t)ticks;

  ret = reg_write_burst(i2c, NPM_BASE_TIME, TIME_VALUE, be24, sizeof(be24));
  if (ret < 0)
    {
      return ret;
    }

  /* The count is double-buffered: it does not take effect until LOAD. */

  return wdt_task(i2c, TIME_TASK_LOAD);
}

static int npm1300_wdt_start(FAR struct watchdog_lowerhalf_s *lower)
{
  FAR struct npm1300_wdt_s *wdt = (FAR struct npm1300_wdt_s *)lower;
  int ret;

  /* Mode must be set while the timer is stopped, and the count must
   * already be loaded, or the first window is whatever was there before.
   */

  ret = wdt_set_timeout_ms(wdt->i2c, wdt->timeout);
  if (ret < 0)
    {
      return ret;
    }

  ret = reg_write_burst(wdt->i2c, NPM_BASE_TIME, TIME_MODE, &wdt->mode, 1);
  if (ret < 0)
    {
      return ret;
    }

  ret = wdt_task(wdt->i2c, TIME_TASK_START);
  if (ret >= 0)
    {
      wdt->started = true;
    }

  return ret;
}

static int npm1300_wdt_stop(FAR struct watchdog_lowerhalf_s *lower)
{
  FAR struct npm1300_wdt_s *wdt = (FAR struct npm1300_wdt_s *)lower;
  int ret = wdt_task(wdt->i2c, TIME_TASK_STOP);

  if (ret >= 0)
    {
      wdt->started = false;
    }

  return ret;
}

static int npm1300_wdt_keepalive(FAR struct watchdog_lowerhalf_s *lower)
{
  FAR struct npm1300_wdt_s *wdt = (FAR struct npm1300_wdt_s *)lower;

  return wdt_task(wdt->i2c, TIME_TASK_KICK);
}

static int npm1300_wdt_settimeout(FAR struct watchdog_lowerhalf_s *lower,
                                  uint32_t timeout)
{
  FAR struct npm1300_wdt_s *wdt = (FAR struct npm1300_wdt_s *)lower;
  int ret;

  if (timeout < NPM1300_WDT_TICK_MS || timeout > NPM1300_WDT_MAX_MS)
    {
      return -ERANGE;
    }

  ret = wdt_set_timeout_ms(wdt->i2c, timeout);
  if (ret < 0)
    {
      return ret;
    }

  wdt->timeout = timeout;
  return OK;
}

static int npm1300_wdt_getstatus(FAR struct watchdog_lowerhalf_s *lower,
                                 FAR struct watchdog_status_s *status)
{
  FAR struct npm1300_wdt_s *wdt = (FAR struct npm1300_wdt_s *)lower;

  status->flags   = WDFLAGS_RESET;
  status->timeout = wdt->timeout;

  /* The timer's remaining count is not read back over I2C on this part,
   * so report the full window rather than inventing a number.
   */

  status->timeleft = wdt->timeout;

  if (wdt->started)
    {
      status->flags |= WDFLAGS_ACTIVE;
    }

  return OK;
}

static const struct watchdog_ops_s g_npm1300_wdtops =
{
  .start      = npm1300_wdt_start,
  .stop       = npm1300_wdt_stop,
  .keepalive  = npm1300_wdt_keepalive,
  .getstatus  = npm1300_wdt_getstatus,
  .settimeout = npm1300_wdt_settimeout,
  .capture    = NULL,
  .ioctl      = NULL,
};

int npm1300_wdt_register(FAR struct i2c_master_s *i2c,
                         FAR const char *devpath, uint8_t mode)
{
  FAR struct npm1300_wdt_s *wdt;

  DEBUGASSERT(i2c != NULL && devpath != NULL);

  if (mode != NPM1300_WDT_MODE_WARN && mode != NPM1300_WDT_MODE_RESET)
    {
      return -EINVAL;
    }

  wdt = kmm_zalloc(sizeof(struct npm1300_wdt_s));
  if (wdt == NULL)
    {
      return -ENOMEM;
    }

  wdt->lower.ops = &g_npm1300_wdtops;
  wdt->i2c       = i2c;
  wdt->mode      = mode;
  wdt->timeout   = CONFIG_NPM1300_WATCHDOG_TIMEOUT_MS;

  /* Make sure the timer is not left running from a previous boot, and drop
   * any latched expiry event with it: the PMIC is not reset when the SoC
   * is, so a warm reset can land here with the watchdog already counting
   * and nothing feeding it yet, and with a WATCHDOG_WARN from a previous
   * life still set.  Reporting that stale bit as a live expiry sends
   * whoever reads it chasing a fault that already happened.
   */

  wdt_task(i2c, TIME_TASK_STOP);

  {
    uint8_t bit = MAIN_EVENT_WDT_BIT;

    reg_write_burst(i2c, NPM_BASE_MAIN, MAIN_EVENT_WDT_CLR, &bit, 1);
  }

  if (watchdog_register(devpath, &wdt->lower) == NULL)
    {
      kmm_free(wdt);
      return -EEXIST;
    }

  return OK;
}

#endif /* CONFIG_NPM1300_WATCHDOG */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: npm1300_register
 ****************************************************************************/

int npm1300_register(FAR struct i2c_master_s *i2c, FAR const char *devpath,
                     FAR const struct npm1300_config_s *config)
{
  FAR struct npm1300_dev_s *dev;
  uint8_t probe;
  int ret;

  DEBUGASSERT(i2c != NULL && devpath != NULL);

  dev = kmm_zalloc(sizeof(struct npm1300_dev_s));
  if (dev == NULL)
    {
      return -ENOMEM;
    }

  dev->gauge.ops = &g_npm1300_gaugeops;
  dev->i2c       = i2c;
  nxmutex_init(&dev->lock);

  /* Probe: read the charger status.  There is no dedicated ID register on
   * this part, so a successful transfer at the fixed address is the test.
   */

  ret = reg_read(dev, NPM_BASE_CHGR, CHGR_CHG_STAT, &probe);
  if (ret < 0)
    {
      batinfo("no nPM1300 at 0x%02x (%d)\n", NPM1300_I2C_ADDR, ret);
      nxmutex_destroy(&dev->lock);
      kmm_free(dev);
      return -ENODEV;
    }

  if (config != NULL)
    {
      dev->esr_mohm = config->esr_mohm;

      ret = npm1300_configure(dev, config);
      if (ret < 0)
        {
          baterr("ERROR: charger config failed: %d\n", ret);
          nxmutex_destroy(&dev->lock);
          kmm_free(dev);
          return ret;
        }
    }

  ret = battery_gauge_register(devpath, &dev->gauge);
  if (ret < 0)
    {
      baterr("ERROR: battery_gauge_register: %d\n", ret);
      nxmutex_destroy(&dev->lock);
      kmm_free(dev);
      return ret;
    }

  return OK;
}
