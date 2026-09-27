/****************************************************************************
 * drivers/power/battery/max20303.c
 *
 * Driver for the Maxim MAX20303 wearable PMIC: ModelGauge battery fuel
 * gauge (battery_gauge lower half), Li+ charger configuration, and the
 * haptic motor driver (simple character device).
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements. See the NOTICE file distributed with
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
 ***************************************************************************
 */

/****************************************************************************
 * Included Files
 ***************************************************************************
 */

#include <nuttx/config.h>
#include <nuttx/nuttx.h>

#include <nuttx/debug.h>
#include <nuttx/fs/fs.h>
#include <nuttx/i2c/i2c_master.h>
#include <nuttx/kmalloc.h>
#include <nuttx/mutex.h>
#include <nuttx/wqueue.h>
#include <nuttx/power/battery_gauge.h>
#ifdef CONFIG_BATTERY_SOC_ESTIMATOR
#  include <nuttx/power/battery_soc.h>
#endif
#include <nuttx/power/battery_ioctl.h>
#include <nuttx/power/max20303.h>
#include <nuttx/signal.h>

#include <fixedmath.h>
#include <string.h>
#include <syslog.h>

/****************************************************************************
 * Pre-processor Definitions
 ***************************************************************************
 */

/* Two I2C targets in one package (MAX20303 datasheet):
 * - The PMIC proper at 0x28: direct registers + the AP command interface
 *   (write APDataOut0..5 then APCmdOut; poll APResponse for cmd echo).
 * - The ModelGauge fuel gauge at 0x36: MAX17048-class 16-bit registers.
 */

#define MAX20303_PMIC_ADDR    0x28
#define MAX20303_GAUGE_ADDR   0x36

/* PMIC direct registers */

#define REG_HARDWARE_ID       0x00  /* 0x02 = MAX20303 */
#define REG_STATUS0           0x06  /* ChgStat[2:0] */
#define REG_STATUS1           0x07  /* UsbOk bit 3 */
#define REG_AP_DATOUT0        0x0f
#define REG_AP_CMDOUT         0x17
#define REG_AP_RESPONSE       0x18
#define REG_AP_DATAIN0        0x19
#define REG_STATUS3           0x09  /* LRALock bit 4, LRAAct bit 3 */
#define REG_HPT_RAM_ADDR      0x28  /* NB: a register number, not an I2C addr */
#define REG_HPT_RAM_DH        0x29
#define REG_HPT_RAM_DM        0x2a
#define REG_HPT_RAM_DL        0x2b
#define REG_HPT_DIRECT0       0x30
#define REG_HPT_DIRECT1       0x31
#define REG_HPT_RTI2C_AMP     0x32

#define HW_ID_MAX20303        0x02

/* Status0 ChgStat[2:0] */

#define CHGSTAT_OFF           0x0
#define CHGSTAT_SUSPENDED     0x1
#define CHGSTAT_PRECHARGE     0x2
#define CHGSTAT_FAST_CC       0x3
#define CHGSTAT_FAST_CV       0x4
#define CHGSTAT_MAINTAIN      0x5
#define CHGSTAT_MAINT_DONE    0x6
#define CHGSTAT_FAULT         0x7

#define STATUS1_USBOK         (1 << 3)

/* AP command opcodes */

#define CMD_INPUTCURRENT_CFG  0x10
#define CMD_CHARGER_CFG       0x14
#define CMD_CHARGER_CTRL      0x1a
#define CMD_LDO1_CFG          0x40
#define CMD_LDO1_READ         0x41
#define CMD_LDO2_CFG          0x42
#define CMD_LDO2_READ         0x43
#define CMD_HPT_CONFIG0       0xa0
#define CMD_HPT_SETLOCK       0xa8  /* 0x00 clears HptLock              */
#define CMD_HPT_SETFULLSCALE  0xb2  /* HptVfs[7:0], LSB = 21.57 mV      */
#define CMD_HPT_SETPATTERN    0xb3  /* the six ETRG bytes, in one go    */
#define CMD_HPT_READRESONANCE 0xb6  /* -> BEMFPeriod[11:0] in DataIn0/1 */
#define CMD_HPT_SETTIMEOUT    0xb7  /* HptDrvTmo[5:0], LSB = 1 s        */

/* LDO config Data0: mode + enable (verified against the per-variant OTP
 * defaults in Table 192 vs the LDOx_Config_Read defaults -- En[1:0] is in
 * Data0 bits 1:0, Md in bit 2, NOT in Data1 as Table 101's layout
 * suggests; Data1 is the VSet code alone).
 */

#define LDO_MD_LOADSW         (1 << 2)
#define LDO_EN_ENABLED        0x01

/* HptDirect1: HptDrvEn (bit 5) + HptDrvMode 00110 = RTI2C (amplitude from
 * REG_HPT_RTI2C_AMP, LSB = VSYS/128).
 */

#define HPT_DRV_EN            (1 << 5)
#define HPT_EXT_TRIG          (1 << 7)
#define HPT_MODE_OFF          0x00
#define HPT_MODE_RTI2C        0x06
#define HPT_MODE_ETRGI        0x0c  /* triggered by HptExtTrig, not a pin */
#define HPT_MODE_RAMHPI       0x12  /* RAM pattern, triggered over I2C     */
#define HPT_RAM_EN            (1 << 6)
#define REG_HPT_PATRAM_ADDR   0x33  /* start address of the pattern to play */

#define STATUS3_LRALOCK       (1 << 4)

/* Back-EMF tracker units. The chip counts a (25.6 MHz / 64) clock over one
 * resonant period, so period code and frequency are reciprocal:
 *
 *     f_Hz = HPT_BEMF_CLK_HZ / period_code
 *
 * Used both to program the initial guess and to interpret 0xB6's answer.
 */

#define HPT_BEMF_CLK_HZ       400000u

/* Trackable range per the datasheet; anything outside means the tracker
 * did not really lock and the reading is noise.
 */

#define HPT_LRA_FREQ_MIN_HZ   120
#define HPT_LRA_FREQ_MAX_HZ   305

/* Fuel gauge (MAX17048-class) registers: 16-bit big-endian */

#define GAUGE_REG_VCELL       0x02  /* 78.125 uV/LSB */
#define GAUGE_REG_SOC         0x04  /* MSB = percent, LSB = 1/256 % */

#ifndef CONFIG_MAX20303_I2C_FREQUENCY
#  define CONFIG_MAX20303_I2C_FREQUENCY 400000
#endif

/****************************************************************************
 * Private Types
 ***************************************************************************
 */

struct max20303_dev_s
{
  /* battery_gauge lower half -- must be first for the upper half */

  struct battery_gauge_dev_s gauge;
  FAR struct i2c_master_s *i2c;
  mutex_t lock;               /* Serializes PMIC/haptic access */
  struct work_s hpt_work;     /* deferred end-of-pattern teardown */
};

/****************************************************************************
 * Private Function Prototypes
 ***************************************************************************
 */

static int max20303_state(FAR struct battery_gauge_dev_s *lower,
                          FAR int *status);
static int max20303_online(FAR struct battery_gauge_dev_s *lower,
                           FAR bool *status);
static int max20303_voltage(FAR struct battery_gauge_dev_s *lower,
                            FAR int *value);
static int max20303_capacity(FAR struct battery_gauge_dev_s *lower,
                             FAR int *value);

static ssize_t haptic_write(FAR struct file *filep, FAR const char *buffer,
                            size_t buflen);
static int haptic_ioctl(FAR struct file *filep, int cmd,
                        unsigned long arg);

/****************************************************************************
 * Private Data
 ***************************************************************************
 */

static const struct battery_gauge_operations_s g_max20303_gaugeops =
{
  .state    = max20303_state,
  .online   = max20303_online,
  .voltage  = max20303_voltage,
  .capacity = max20303_capacity,
};

static const struct file_operations g_haptic_fops =
{
  NULL,          /* open */
  NULL,          /* close */
  NULL,          /* read */
  haptic_write,  /* write */
  NULL,          /* seek */
  haptic_ioctl,  /* ioctl */
};

/* The haptic chardev needs the device struct; single instance. */

static FAR struct max20303_dev_s *g_max20303;

/****************************************************************************
 * Private Functions -- I2C plumbing
 ***************************************************************************
 */

static int reg_read(FAR struct max20303_dev_s *dev, uint8_t addr,
                    uint8_t reg, FAR uint8_t *buf, size_t len)
{
  struct i2c_msg_s msgs[2] =
  {
    {
      .frequency = CONFIG_MAX20303_I2C_FREQUENCY,
      .addr = addr, .flags = 0, .buffer = &reg, .length = 1
    },
    {
      .frequency = CONFIG_MAX20303_I2C_FREQUENCY,
      .addr = addr, .flags = I2C_M_READ, .buffer = buf, .length = len
    },
  };

  return I2C_TRANSFER(dev->i2c, msgs, 2);
}

static int reg_write(FAR struct max20303_dev_s *dev, uint8_t addr,
                     uint8_t reg, FAR const uint8_t *val, size_t len)
{
  uint8_t buf[8];

  struct i2c_msg_s msg =
  {
    .frequency = CONFIG_MAX20303_I2C_FREQUENCY,
    .addr = addr, .flags = 0, .buffer = buf, .length = len + 1
  };

  if (len > sizeof(buf) - 1)
    {
      return -EINVAL;
    }

  buf[0] = reg;
  memcpy(&buf[1], val, len);
  return I2C_TRANSFER(dev->i2c, &msg, 1);
}

static int reg_write1(FAR struct max20303_dev_s *dev, uint8_t addr,
                      uint8_t reg, uint8_t val)
{
  return reg_write(dev, addr, reg, &val, 1);
}

/* AP command interface: load APDataOut0..n, write the opcode to APCmdOut,
 * then verify APResponse echoes the opcode (the PMIC's ack).
 */

static int ap_command(FAR struct max20303_dev_s *dev, uint8_t opcode,
                      FAR const uint8_t *data, size_t len)
{
  uint8_t resp = 0;
  int ret;

  if (len > 0)
    {
      ret = reg_write(dev, MAX20303_PMIC_ADDR, REG_AP_DATOUT0, data, len);
      if (ret < 0)
        {
          return ret;
        }
    }

  ret = reg_write1(dev, MAX20303_PMIC_ADDR, REG_AP_CMDOUT, opcode);
  if (ret < 0)
    {
      return ret;
    }

  nxsig_usleep(10000);

  ret = reg_read(dev, MAX20303_PMIC_ADDR, REG_AP_RESPONSE, &resp, 1);
  if (ret < 0)
    {
      return ret;
    }

  if (resp != opcode)
    {
      baterr("ERROR: AP cmd 0x%02x response 0x%02x\n", opcode, resp);
      return -EIO;
    }

  return OK;
}

/****************************************************************************
 * Private Functions -- battery gauge lower half
 ***************************************************************************
 */

static int max20303_state(FAR struct battery_gauge_dev_s *lower,
                          FAR int *status)
{
  FAR struct max20303_dev_s *dev = (FAR struct max20303_dev_s *)lower;
  uint8_t s0 = 0;
  int ret;

  nxmutex_lock(&dev->lock);
  ret = reg_read(dev, MAX20303_PMIC_ADDR, REG_STATUS0, &s0, 1);
  nxmutex_unlock(&dev->lock);

  if (ret < 0)
    {
      *status = BATTERY_UNKNOWN;
      return ret;
    }

  switch (s0 & 0x07)
    {
      case CHGSTAT_PRECHARGE:
      case CHGSTAT_FAST_CC:
      case CHGSTAT_FAST_CV:
        *status = BATTERY_CHARGING;
        break;

      case CHGSTAT_MAINTAIN:
      case CHGSTAT_MAINT_DONE:
        *status = BATTERY_FULL;
        break;

      case CHGSTAT_FAULT:
        *status = BATTERY_FAULT;
        break;

      case CHGSTAT_OFF:
      case CHGSTAT_SUSPENDED:
      default:
        *status = BATTERY_DISCHARGING;
        break;
    }

  return OK;
}

static int max20303_online(FAR struct battery_gauge_dev_s *lower,
                           FAR bool *status)
{
  *status = true;
  return OK;
}

static int max20303_voltage(FAR struct battery_gauge_dev_s *lower,
                            FAR int *value)
{
  FAR struct max20303_dev_s *dev = (FAR struct max20303_dev_s *)lower;
  uint8_t raw[2];
  int ret;

  nxmutex_lock(&dev->lock);
  ret = reg_read(dev, MAX20303_GAUGE_ADDR, GAUGE_REG_VCELL, raw, 2);
  nxmutex_unlock(&dev->lock);

  if (ret < 0)
    {
      return ret;
    }

  /* 78.125 uV/LSB -> millivolts (x * 78125 / 1000000 == x * 5 / 64) */

  *value = (int)(((uint32_t)((raw[0] << 8) | raw[1]) * 5) / 64);
  return OK;
}

static int max20303_capacity(FAR struct battery_gauge_dev_s *lower,
                             FAR int *value)
{
  FAR struct max20303_dev_s *dev = (FAR struct max20303_dev_s *)lower;
  uint8_t raw[2];
  int ret;

  nxmutex_lock(&dev->lock);
  ret = reg_read(dev, MAX20303_GAUGE_ADDR, GAUGE_REG_SOC, raw, 2);
  nxmutex_unlock(&dev->lock);

  if (ret < 0)
    {
      return ret;
    }

  /* MSB is percent (1/256 % in the LSB). */

  if (raw[0] <= 100)
    {
      *value = raw[0];
      return OK;
    }

  /* Out of range means the ModelGauge has not converged yet (it needs a few
   * minutes after a cold start) or there is no battery. Clamping to 100
   * would read as "fully charged": the worst possible answer for a flat
   * cell, and indistinguishable from a real 100%. So when the voltage
   * estimator is available, answer from the cell voltage instead.
   */

#ifdef CONFIG_BATTERY_SOC_ESTIMATOR
    {
      int mv;
      int ret2 = max20303_voltage(lower, &mv);

      if (ret2 == OK)
        {
          /* No current measurement here, so this is a plain open-circuit
           * lookup: coarse, but honest about a flat battery.
           */

          *value = battery_soc_from_ocv(mv);
          return OK;
        }
    }
#endif

  *value = 100;
  return OK;
}

/****************************************************************************
 * Private Functions -- charger + haptic configuration
 ***************************************************************************
 */

/* Charger bring-up, as validated on real hardware:
 * 4.20 V regulation, autostop + autorestart (BatReg - 170 mV), precharge
 * 3.15 V threshold at 0.05 x IFChg, done at 0.1 x IFChg, fast-charge
 * timer 150 min; CHGIN input limit 200 mA / 10 ms blanking; thermal
 * monitor off (no NTC fitted), charger enabled.
 */

static int charger_init(FAR struct max20303_dev_s *dev)
{
  static const uint8_t chg_cfg[4] =
  {
    0x04, 0x71, 0xe3, 0x06
  };

  static const uint8_t chg_en[1] =
  {
    0x01
  };

  static const uint8_t input_cfg[1] =
  {
    0x1b
  };

  int ret;

  ret = ap_command(dev, CMD_CHARGER_CFG, chg_cfg, sizeof(chg_cfg));
  if (ret >= 0)
    {
      ret = ap_command(dev, CMD_CHARGER_CTRL, chg_en, sizeof(chg_en));
    }

  if (ret >= 0)
    {
      ret = ap_command(dev, CMD_INPUTCURRENT_CFG, input_cfg,
                       sizeof(input_cfg));
    }

  return ret;
}

/* LDO output voltage + enable (LDOx_Config_Write, opcodes 0x40/0x42).
 * Boards may power sensor rails from these; the MAX20303B OTP
 * defaults leave both LDOs *disabled* (the ~1 V measurable on the rail
 * is back-feed through the sensors' ESD diodes), so the sensors never
 * ACK on I2C1. Only compiled in when CONFIG_MAX20303_LDOx_MV is set --
 * the driver's default remains "never touch LDO/buck rails".
 *
 * VSet codes: LDO1 = 0.5 V + 25 mV/LSB, LDO2 = 0.9 V + 100 mV/LSB.
 * The enable takes effect because both LDOxSeq default to 111
 * (AP-controlled) on every OTP variant.
 *
 * After writing, read the config back (read opcodes take no data; the
 * result lands in APDataIn0..2) and verify enable + VSet actually
 * latched -- a silent mis-write here costs hours at the bench.
 */

#if CONFIG_MAX20303_LDO1_MV != 0 || CONFIG_MAX20303_LDO2_MV != 0

#  if CONFIG_MAX20303_LDO1_MV != 0 && \
      (CONFIG_MAX20303_LDO1_MV < 500 || CONFIG_MAX20303_LDO1_MV > 1950)
#    error "MAX20303_LDO1_MV out of range (500..1950 mV)"
#  elif (CONFIG_MAX20303_LDO1_MV - 500) % 25 != 0
#    error "MAX20303_LDO1_MV must be a multiple of 25 mV"
#  endif

#  if CONFIG_MAX20303_LDO2_MV != 0 && \
      (CONFIG_MAX20303_LDO2_MV < 900 || CONFIG_MAX20303_LDO2_MV > 4000)
#    error "MAX20303_LDO2_MV out of range (900..4000 mV)"
#  elif (CONFIG_MAX20303_LDO2_MV - 900) % 100 != 0
#    error "MAX20303_LDO2_MV must be a multiple of 100 mV"
#  endif

static int ldo_config(FAR struct max20303_dev_s *dev, int ldo,
                      uint8_t vset)
{
  uint8_t cfg_op = ldo == 1 ? CMD_LDO1_CFG : CMD_LDO2_CFG;
  uint8_t read_op = ldo == 1 ? CMD_LDO1_READ : CMD_LDO2_READ;
  uint8_t cfg[2];
  uint8_t back[3];
  int ret;

  cfg[0] = LDO_EN_ENABLED;      /* normal LDO mode, enabled */
  cfg[1] = vset;

  ret = ap_command(dev, cfg_op, cfg, sizeof(cfg));
  if (ret < 0)
    {
      syslog(LOG_ERR, "MAX20303: LDO%d config write failed: %d\n",
             ldo, ret);
      return ret;
    }

  ret = ap_command(dev, read_op, NULL, 0);
  if (ret >= 0)
    {
      ret = reg_read(dev, MAX20303_PMIC_ADDR, REG_AP_DATAIN0, back, 3);
    }

  if (ret < 0)
    {
      syslog(LOG_ERR, "MAX20303: LDO%d config readback failed: %d\n",
             ldo, ret);
      return ret;
    }

  syslog(LOG_INFO, "MAX20303: LDO%d en/md 0x%02x vset 0x%02x seq 0x%02x"
         " (wanted en/md 0x%02x vset 0x%02x)\n",
         ldo, back[0], back[1], back[2], cfg[0], cfg[1]);

  if ((back[0] & 0x03) != LDO_EN_ENABLED || back[1] != vset)
    {
      return -EIO;
    }

  return OK;
}

static int ldo_init(FAR struct max20303_dev_s *dev)
{
  int ret = OK;

#  if CONFIG_MAX20303_LDO1_MV != 0
  ret = ldo_config(dev, 1, (CONFIG_MAX20303_LDO1_MV - 500) / 25);
#  endif

#  if CONFIG_MAX20303_LDO2_MV != 0
  if (ret >= 0)
    {
      ret = ldo_config(dev, 2, (CONFIG_MAX20303_LDO2_MV - 900) / 100);
    }
#  endif

  return ret;
}
#endif /* CONFIG_MAX20303_LDO1_MV || CONFIG_MAX20303_LDO2_MV */

/* Haptic configuration (HptConfigWrite0, opcode 0xA0). The motor type is
 * a Kconfig choice: LRA uses resonance parameters validated on real
 * hardware; ERM is a plain voltage drive.
 */

static int haptic_init(FAR struct max20303_dev_s *dev)
{
#ifdef CONFIG_MAX20303_HAPTIC_LRA
  /* Data0 bit layout (opcode 0xAD, Table 153):
   *   b3 EmfEn | b2 HptSel (1 = LRA) | b1 AlcMod | b0 ZccHysEn
   *
   * 0x0f = all four. ZccHysEn is easy to leave off by mistake (0x0e);
   * its hysteresis (6 mV typ) is noise immunity on the zero-crossing
   * comparator that feeds resonance tracking, so it belongs on for an LRA.
   *
   * IniGss[11:0] is the back-EMF tracker's starting point, as a period:
   * code = 400000 / f_Hz. It splits across Data1 (low 8) and Data2's low
   * nibble. It is a Kconfig (MAX20303_HAPTIC_LRA_HZ) because it is a
   * property of the fitted motor, not of the driver.
   */

  const uint16_t inigss = (uint16_t)
      (HPT_BEMF_CLK_HZ / (unsigned)CONFIG_MAX20303_HAPTIC_LRA_HZ);

  const uint8_t hpt_cfg[6] =
  {
    0x0f,                             /* EmfEn + LRA + ALC + ZccHys      */
    (uint8_t)(inigss & 0xff),         /* IniGss[7:0]                     */
    (uint8_t)((inigss >> 8) & 0x0f),  /* IniGss[11:8]                    */
    0x00,                             /* Skip periods before BEMF meas.  */
    0x07,                             /* Wide zero-crossing window       */
    0x02,                             /* Narrow zero-crossing window     */
  };
#else
  /* ERM: plain voltage drive, no resonance tracking (there is nothing to
   * track -- an eccentric-rotating-mass motor has no resonant frequency).
   *
   * ALC (b1) IS enabled rather than left at 0x00. It compensates the output
   * duty cycle as VSYS sags, which is what makes an amplitude percentage
   * mean a fixed VOLTAGE rather than "some fraction of whatever the battery
   * happens to be at". On a motor whose usable band runs from a 2.5 V
   * starting voltage to a 4.0 V absolute maximum, that distinction is the
   * difference between a reliable tap and one that fades as the battery
   * drains.
   */

  static const uint8_t hpt_cfg[6] =
  {
    0x02,       /* ERM + ALC */
    0x00, 0x00, 0x00, 0x00, 0x00,
  };
#endif

  int ret = ap_command(dev, CMD_HPT_CONFIG0, hpt_cfg, sizeof(hpt_cfg));
  if (ret < 0)
    {
      return ret;
    }

#if CONFIG_MAX20303_HAPTIC_VFS_MV > 0
  /* Full-scale voltage: every amplitude is a percentage of this, so
   * without it "35%" refers to an unknown OTP default. LSB = 21.57 mV;
   * round to nearest rather than truncating so the programmed voltage
   * brackets the requested one.
   */

  {
    unsigned code = ((unsigned)CONFIG_MAX20303_HAPTIC_VFS_MV * 100 + 1078)
                    / 2157;
    uint8_t vfs = (uint8_t)(code > 255 ? 255 : code);

    ret = ap_command(dev, CMD_HPT_SETFULLSCALE, &vfs, 1);
    if (ret < 0)
      {
        baterr("ERROR: MAX20303 haptic VFS: %d\n", ret);
        return ret;
      }
  }
#endif

  /* Hardware backstop against a runaway vibration.
   *
   * If any vibration lasts longer than HptDrvTmo the PMIC aborts it,
   * latches HptLock and raises SystemError 0x04. Our envelopes are tens
   * of milliseconds, so a 1 s timeout (the coarsest-but-shortest the
   * field allows, LSB = 1 s) never fires in normal use -- it exists
   * purely so a driver bug cannot leave the motor running indefinitely,
   * which is a battery and thermal problem, not just an annoyance.
   *
   * Clear any latched lock first: the lock survives a reset of the host,
   * so a previous runaway would otherwise leave haptics permanently dead
   * with no obvious cause. Writing 0x00 via 0xA8 releases it.
   */

  {
    uint8_t v = 0x00;
    ap_command(dev, CMD_HPT_SETLOCK, &v, 1);

    v = 0x01;   /* 1 s */
    ap_command(dev, CMD_HPT_SETTIMEOUT, &v, 1);
  }

  /* Driver off, outputs strongly grounded when idle */

  reg_write1(dev, MAX20303_PMIC_ADDR, REG_HPT_DIRECT0, 0x00);
  return reg_write1(dev, MAX20303_PMIC_ADDR, REG_HPT_DIRECT1, 0x00);
}

/* Measure the fitted motor's resonant frequency.
 *
 * The back-EMF tracker only runs while the motor is actually driven, so
 * this has to make a real (brief, low-amplitude) buzz to get an answer.
 * Sequence: drive in RTI2C long enough for the tracker to converge, check
 * LRALock, read the measured period, stop.
 *
 * Returns the frequency in Hz, or 0 if the tracker never locked or
 * reported something outside the trackable range -- both of which mean
 * "do not trust this number", and are worth distinguishing from a clean
 * measurement by the caller.
 *
 * Caller must hold dev->lock.
 */

static int haptic_measure_resonance(FAR struct max20303_dev_s *dev)
{
  uint8_t din[2];
  uint8_t st3 = 0;
  unsigned period;
  unsigned hz;
  int ret;

  din[0] = 0;
  din[1] = 0;

  reg_write1(dev, MAX20303_PMIC_ADDR, REG_HPT_RTI2C_AMP, 0x00);
  ret = reg_write1(dev, MAX20303_PMIC_ADDR, REG_HPT_DIRECT1,
                   HPT_DRV_EN | HPT_MODE_RTI2C);
  if (ret < 0)
    {
      return 0;
    }

  /* ~50 % of full scale: enough back-EMF to track cleanly, and the
   * tracker needs a number of resonant periods to settle -- at 120 Hz
   * worst case a period is 8.3 ms, so 200 ms is ~24 periods.
   */

  reg_write1(dev, MAX20303_PMIC_ADDR, REG_HPT_RTI2C_AMP, 64);
  nxsig_usleep(200000);

  ret = reg_read(dev, MAX20303_PMIC_ADDR, REG_STATUS3, &st3, 1);
  if (ret >= 0)
    {
      ret = ap_command(dev, CMD_HPT_READRESONANCE, NULL, 0);
      if (ret >= 0)
        {
          ret = reg_read(dev, MAX20303_PMIC_ADDR, REG_AP_DATAIN0, din, 2);
        }
    }

  /* Stop in the order the datasheet requires: amplitude, then mode, then
   * enable.
   */

  reg_write1(dev, MAX20303_PMIC_ADDR, REG_HPT_RTI2C_AMP, 0x00);
  reg_write1(dev, MAX20303_PMIC_ADDR, REG_HPT_DIRECT1, HPT_DRV_EN);
  reg_write1(dev, MAX20303_PMIC_ADDR, REG_HPT_DIRECT1, 0x00);

  if (ret < 0)
    {
      return 0;
    }

  if ((st3 & STATUS3_LRALOCK) == 0)
    {
      batwarn("MAX20303: BEMF tracker never locked (status3 0x%02x)\n", st3);
      return 0;
    }

  period = (unsigned)din[0] | (((unsigned)din[1] & 0x0f) << 8);
  if (period == 0)
    {
      return 0;
    }

  hz = HPT_BEMF_CLK_HZ / period;
  if (hz < HPT_LRA_FREQ_MIN_HZ || hz > HPT_LRA_FREQ_MAX_HZ)
    {
      batwarn("MAX20303: resonance %u Hz outside trackable range\n", hz);
      return 0;
    }

  return (int)hz;
}

/****************************************************************************
 * Private Functions -- haptic character device
 ***************************************************************************
 */

/* Play a pattern synchronously: RTI2C direct-drive mode, amplitude from
 * REG_HPT_RTI2C_AMP (7 bits, LSB = VSYS/128). Blocks the caller for the
 * pattern's total duration, so call it from a thread that can wait
 * (a vibration service thread, not an interrupt or a UI loop).
 */

static ssize_t haptic_write(FAR struct file *filep, FAR const char *buffer,
                            size_t buflen)
{
  FAR struct max20303_dev_s *dev = g_max20303;
  FAR const struct max20303_haptic_sample_s *samples;
  size_t nsamples = buflen / sizeof(struct max20303_haptic_sample_s);
  size_t i;
  int ret;

  if (dev == NULL)
    {
      return -ENODEV;
    }

  if (nsamples == 0)
    {
      return -EINVAL;
    }

  samples = (FAR const struct max20303_haptic_sample_s *)buffer;

  nxmutex_lock(&dev->lock);

  /* Enable the driver in RTI2C mode, amplitude 0 */

  reg_write1(dev, MAX20303_PMIC_ADDR, REG_HPT_RTI2C_AMP, 0x00);
  ret = reg_write1(dev, MAX20303_PMIC_ADDR, REG_HPT_DIRECT1,
                   HPT_DRV_EN | HPT_MODE_RTI2C);
  if (ret < 0)
    {
      nxmutex_unlock(&dev->lock);
      return ret;
    }

  for (i = 0; i < nsamples; i++)
    {
      uint8_t pct = samples[i].amplitude > 100 ? 100 : samples[i].amplitude;
      uint8_t amp = (uint8_t)(((unsigned)pct * 127) / 100);

      reg_write1(dev, MAX20303_PMIC_ADDR, REG_HPT_RTI2C_AMP, amp);
      nxsig_usleep((useconds_t)samples[i].duration_ms * 1000);
    }

  /* Amplitude to zero, mode to 0 with the driver still enabled, then
   * disable -- the shutdown order the datasheet requires.
   */

  reg_write1(dev, MAX20303_PMIC_ADDR, REG_HPT_RTI2C_AMP, 0x00);
  reg_write1(dev, MAX20303_PMIC_ADDR, REG_HPT_DIRECT1, HPT_DRV_EN);
  reg_write1(dev, MAX20303_PMIC_ADDR, REG_HPT_DIRECT1, 0x00);

  nxmutex_unlock(&dev->lock);
  return buflen;
}

/* Pack one logical sample into the three RAM data bytes.
 *
 * Layout is Table 3 "RAMHP Pattern Storage Format", verified against the
 * datasheet with column alignment preserved (the field order is easy to
 * mis-read from a reflowed copy):
 *
 *   HptDataH  B7:B6 nLS[1:0] | B5 AmpSign | B4:B0 Amp[6:2]
 *   HptDataM  B7:B6 Amp[1:0] | B5:B1 Dur[4:0] | B0 Wait[4]
 *   HptDataL  B7:B4 Wait[3:0] | B3:B0 RPT[3:0]
 *
 * Note the amplitude is split across two bytes and the wait across two
 * bytes, in opposite directions. Get this wrong and patterns still play --
 * they just feel wrong, which is far harder to debug than a failure. The
 * readback verify in the load path catches transport errors but NOT a
 * wrong field order, so this mapping is the thing to re-check first if a
 * pattern feels nothing like its table entry.
 */

static void haptic_ram_pack(FAR const struct max20303_haptic_ram_sample_s *s,
                            FAR uint8_t *out)
{
  uint8_t amp  = s->amp & 0x7f;
  uint8_t sign = (s->amp & MAX20303_HAPTIC_AMP_REVERSE) ? 1 : 0;
  uint8_t dur  = s->dur  & 0x1f;
  uint8_t wait = s->wait & 0x1f;
  uint8_t rpt  = s->rpt  & 0x0f;
  uint8_t nls  = s->nls  & 0x03;

  out[0] = (uint8_t)((nls << 6) | (sign << 5) | (amp >> 2));
  out[1] = (uint8_t)(((amp & 0x03) << 6) | (dur << 1) | (wait >> 4));
  out[2] = (uint8_t)(((wait & 0x0f) << 4) | rpt);
}

/* End-of-pattern teardown, deferred.
 *
 * The datasheet is explicit: "Once vibration finishes, HptDrvMode[4:0]
 * must be set to 00000 before the haptic driver may be disabled via
 * HptDrvEn = 0".
 * Skipping it does not merely waste power -- the chip stays in a vibration
 * session, HptDrvTmo eventually decides that vibration has run too long,
 * aborts it and LATCHES HptLock. Every subsequent play then does nothing at
 * all, with no error anywhere, until something clears the lock.
 *
 * That failure is nastily time-shaped rather than amplitude-shaped: the
 * first plays after a boot work, and everything more than HptDrvTmo later is
 * silently dead. It reads like "the motor is too weak" and it is not.
 *
 * Doing this inline would mean blocking for the pattern duration, which is
 * exactly what RAMHP exists to avoid, so it runs on the low-priority work
 * queue once the pattern is due to have finished.
 */

static void haptic_teardown_work(FAR void *arg)
{
  FAR struct max20303_dev_s *dev = (FAR struct max20303_dev_s *)arg;

  nxmutex_lock(&dev->lock);
  reg_write1(dev, MAX20303_PMIC_ADDR, REG_HPT_DIRECT1, HPT_DRV_EN);
  reg_write1(dev, MAX20303_PMIC_ADDR, REG_HPT_DIRECT1, 0x00);
  nxmutex_unlock(&dev->lock);
}

/* Fire the stored pattern by pulsing HptExtTrig, and GUARANTEE it falls
 * again.
 *
 * The trigger is a PULSE, not a level, and this is easy to get
 * catastrophically wrong: HptExtTrig must go 0 -> 1 to start the sequence
 * and then BACK TO 0.
 *
 * Leaving it high does not simply "start and forget" -- the datasheet's ETRG
 * section says a trigger held longer than ETRGActDur makes the driver output
 * ETRGActAmp continuously "until a falling edge is detected", and the brake
 * phase runs ON that falling edge. So a stuck-high trigger means the motor
 * runs forever at the active amplitude and never brakes. It was written that
 * way once; the board had to be unplugged.
 *
 * HptDrvEn is a different thing and legitimately stays set -- that is what
 * avoids a blocking wait for the pattern to end.
 *
 * THE DE-ASSERT IS UNCONDITIONAL, and that is the whole point of this
 * function existing. De-asserting only when the preceding writes succeeded
 * would leave one reachable path -- an I2C error on the de-assert itself,
 * or an error after the trigger bit was already set -- that exits with the
 * motor running and merely returns an error code nobody can act on.
 * Clearing the bit is the one operation that must be attempted no matter
 * what already went wrong, so it is tried even if the asserts failed
 * (harmless: the bit was then never set) and retried once if it fails.
 *
 * `idle` is HPT_DRV_EN | <mode>, so the same helper serves both ETRGI and
 * RAMHPI. Do not open-code this sequence anywhere else.
 *
 * Caller must hold dev->lock. Returns the FIRST error seen.
 */

static int haptic_pulse_trigger(FAR struct max20303_dev_s *dev, uint8_t idle)
{
  int ret;
  int err;

  ret = reg_write1(dev, MAX20303_PMIC_ADDR, REG_HPT_DIRECT1, idle);
  if (ret >= 0)
    {
      ret = reg_write1(dev, MAX20303_PMIC_ADDR, REG_HPT_DIRECT1,
                       HPT_EXT_TRIG | idle);
    }

  /* The rising edge must be held "a few clock cycles of the driver" to
   * register. The BEMF timebase is 400 kHz, so single-digit microseconds
   * would do; 200 us is generously above that and still three orders of
   * magnitude below the envelope, so this remains a fire-and-forget call
   * rather than a wait for the pattern.
   */

  if (ret >= 0)
    {
      nxsig_usleep(200);
    }

  err = reg_write1(dev, MAX20303_PMIC_ADDR, REG_HPT_DIRECT1, idle);
  if (err < 0)
    {
      err = reg_write1(dev, MAX20303_PMIC_ADDR, REG_HPT_DIRECT1, idle);
      if (err < 0)
        {
          /* Both attempts failed: the motor may be running and we cannot
           * stop it from here. HptDrvTmo is the remaining backstop.
           */

          baterr("ERROR: MAX20303 haptic trigger stuck high: %d\n", err);
        }
    }

  return ret < 0 ? ret : err;
}

/* ETRG control.
 *
 * SETETRG loads the envelope; TRIGGER fires it and returns immediately,
 * because the PMIC sequences overdrive/active/brake on its own. That is
 * the whole point of this path: write() blocks the caller for the pattern
 * duration, which makes it unusable from any thread that must stay
 * responsive, whereas TRIGGER is two I2C writes and done.
 *
 * The driver is deliberately LEFT ENABLED in ETRGI mode after a trigger.
 * Disabling it would need a wait for the pattern to finish, which would
 * reintroduce exactly the blocking this mode exists to avoid. The idle
 * cost is the haptic block's quiescent current; the next trigger just
 * re-fires. Callers wanting the low-power state can issue SETETRG again
 * (harmless) or simply not care -- vibration is not a hot path.
 */

static int haptic_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
  FAR struct max20303_dev_s *dev = g_max20303;
  int ret;

  if (dev == NULL)
    {
      return -ENODEV;
    }

  switch (cmd)
    {
      case MAX20303_HAPTIC_IOC_SETETRG:
        {
          FAR const struct max20303_haptic_etrg_s *p =
              (FAR const struct max20303_haptic_etrg_s *)((uintptr_t)arg);
          uint8_t data[6];

          if (p == NULL)
            {
              return -EINVAL;
            }

          /* Field order matches the opcode's DataOut0..5 exactly; see
           * Table 165. Copying through a local array keeps that mapping
           * explicit rather than relying on struct layout.
           */

          data[0] = p->od_amp;
          data[1] = p->od_dur;
          data[2] = p->act_amp;
          data[3] = p->act_dur;
          data[4] = p->brk_amp;
          data[5] = p->brk_dur;

          nxmutex_lock(&dev->lock);
          ret = ap_command(dev, CMD_HPT_SETPATTERN, data, sizeof(data));
          nxmutex_unlock(&dev->lock);
          return ret;
        }

      case MAX20303_HAPTIC_IOC_TRIGGER:
        {
          nxmutex_lock(&dev->lock);
          ret = haptic_pulse_trigger(dev, HPT_DRV_EN | HPT_MODE_ETRGI);
          nxmutex_unlock(&dev->lock);
          return ret;
        }

      case MAX20303_HAPTIC_IOC_RAMLOAD:
        {
          FAR const struct max20303_haptic_ramload_s *ld =
              (FAR const struct max20303_haptic_ramload_s *)((uintptr_t)arg);
          unsigned i;

          if (ld == NULL || ld->samples == NULL || ld->count == 0)
            {
              return -EINVAL;
            }

          if ((unsigned)ld->addr + ld->count > MAX20303_HAPTIC_RAM_SAMPLES)
            {
              return -ERANGE;
            }

          nxmutex_lock(&dev->lock);

          /* RAM writes need the block enabled but the driver idle -- loading
           * while a pattern plays is not something the datasheet sanctions.
           */

          ret = reg_write1(dev, MAX20303_PMIC_ADDR, REG_HPT_DIRECT1,
                           HPT_RAM_EN);

          for (i = 0; i < ld->count && ret >= 0; i++)
            {
              uint8_t pk[3];
              uint8_t rb[3];

              haptic_ram_pack(&ld->samples[i], pk);

              ret = reg_write1(dev, MAX20303_PMIC_ADDR, REG_HPT_RAM_ADDR,
                               (uint8_t)(ld->addr + i));
              if (ret >= 0)
                {
                  ret = reg_write(dev, MAX20303_PMIC_ADDR, REG_HPT_RAM_DH,
                                  pk, 3);
                }

              /* Verify by readback. A silently half-loaded bank plays as a
               * wrong pattern rather than as an error, and the RAM is
               * volatile (lost on PMIC brownout, not just host reset), so
               * this is the only cheap way to know the vocabulary is
               * actually resident.
               */

              if (ret >= 0)
                {
                  ret = reg_write1(dev, MAX20303_PMIC_ADDR, REG_HPT_RAM_ADDR,
                                   (uint8_t)(ld->addr + i));
                }

              if (ret >= 0)
                {
                  ret = reg_read(dev, MAX20303_PMIC_ADDR, REG_HPT_RAM_DH,
                                 rb, 3);
                }

              if (ret >= 0 && memcmp(pk, rb, 3) != 0)
                {
                  baterr("ERROR: MAX20303 haptic RAM verify failed at %u: "
                         "wrote %02x%02x%02x read %02x%02x%02x\n",
                         (unsigned)(ld->addr + i),
                         pk[0], pk[1], pk[2], rb[0], rb[1], rb[2]);
                  ret = -EIO;
                }
            }

          /* Leave the RAM block disabled and the driver idle either way. */

          reg_write1(dev, MAX20303_PMIC_ADDR, REG_HPT_DIRECT1, 0x00);
          nxmutex_unlock(&dev->lock);
          return ret;
        }

      case MAX20303_HAPTIC_IOC_RAMPLAY:
        {
          FAR const struct max20303_haptic_ramplay_s *pl =
              (FAR const struct max20303_haptic_ramplay_s *)((uintptr_t)arg);

          if (pl == NULL || pl->addr >= MAX20303_HAPTIC_RAM_SAMPLES)
            {
              return -ERANGE;
            }

          nxmutex_lock(&dev->lock);

          /* Cancel any teardown still pending from a previous pattern, so a
           * new play cannot be shut down by the old one's timer.
           */

          work_cancel(LPWORK, &dev->hpt_work);

          ret = reg_write1(dev, MAX20303_PMIC_ADDR, REG_HPT_PATRAM_ADDR,
                           pl->addr);
          if (ret >= 0)
            {
              /* HPT_RAM_EN must stay SET for playback, not just for the
               * upload. It is the "Haptic RAM Block Enable" (0 = RAM
               * disabled), so with it clear the RAMHPI sequencer has no RAM
               * to read: every register write succeeds, the trigger pulses
               * correctly, the ioctl returns 0 -- and the motor does not
               * move. Nothing in the failure reports a fault, which is why
               * this cost a bench session to find.
               */

              ret = haptic_pulse_trigger(dev,
                                         HPT_RAM_EN | HPT_DRV_EN |
                                         HPT_MODE_RAMHPI);
            }

          /* Schedule the teardown for just after the pattern is due to
           * finish. +50 ms of slack absorbs the 5 ms quantisation and any
           * scheduling jitter without approaching HptDrvTmo.
           */

          if (ret >= 0)
            {
              work_queue(LPWORK, &dev->hpt_work, haptic_teardown_work, dev,
                         MSEC2TICK((unsigned)pl->duration_ms + 50));
            }

          nxmutex_unlock(&dev->lock);
          return ret;
        }

      case MAX20303_HAPTIC_IOC_SETTIMEOUT:
        {
          uint8_t v = (uint8_t)(arg > 63 ? 63 : (arg < 1 ? 1 : arg));

          nxmutex_lock(&dev->lock);
          ret = ap_command(dev, CMD_HPT_SETTIMEOUT, &v, 1);
          nxmutex_unlock(&dev->lock);
          return ret;
        }

      case MAX20303_HAPTIC_IOC_STOP:
        {
          nxmutex_lock(&dev->lock);

          /* Shutdown order the datasheet requires: mode to 0 while the
           * driver is still enabled, then disable.
           */

          ret = reg_write1(dev, MAX20303_PMIC_ADDR, REG_HPT_RTI2C_AMP, 0x00);
          reg_write1(dev, MAX20303_PMIC_ADDR, REG_HPT_DIRECT1, HPT_DRV_EN);
          reg_write1(dev, MAX20303_PMIC_ADDR, REG_HPT_DIRECT1, 0x00);
          nxmutex_unlock(&dev->lock);
          return ret;
        }

      case MAX20303_HAPTIC_IOC_UNLOCK:
        {
          uint8_t v = 0x00;

          nxmutex_lock(&dev->lock);
          ret = ap_command(dev, CMD_HPT_SETLOCK, &v, 1);
          nxmutex_unlock(&dev->lock);
          return ret;
        }

      case MAX20303_HAPTIC_IOC_RESONANCE:
        {
          FAR int *out = (FAR int *)((uintptr_t)arg);

          if (out == NULL)
            {
              return -EINVAL;
            }

          nxmutex_lock(&dev->lock);
          *out = haptic_measure_resonance(dev);
          nxmutex_unlock(&dev->lock);
          return OK;
        }

      default:
        return -ENOTTY;
    }
}

/****************************************************************************
 * Public Functions
 ***************************************************************************
 */

int max20303_register(FAR struct i2c_master_s *i2c,
                      FAR const char *batdev, FAR const char *hapticdev)
{
  FAR struct max20303_dev_s *dev;
  uint8_t id = 0;
  int ret;

  DEBUGASSERT(i2c != NULL && batdev != NULL);

  dev = kmm_zalloc(sizeof(struct max20303_dev_s));
  if (dev == NULL)
    {
      return -ENOMEM;
    }

  dev->i2c = i2c;
  dev->gauge.ops = &g_max20303_gaugeops;
  nxmutex_init(&dev->lock);

  ret = reg_read(dev, MAX20303_PMIC_ADDR, REG_HARDWARE_ID, &id, 1);
  if (ret < 0 || id != HW_ID_MAX20303)
    {
      baterr("ERROR: MAX20303 not found (ret %d, id 0x%02x)\n", ret, id);
      ret = -ENODEV;
      goto errout;
    }

  ret = charger_init(dev);
  if (ret < 0)
    {
      baterr("ERROR: MAX20303 charger init: %d\n", ret);
      goto errout;
    }

  ret = haptic_init(dev);
  if (ret < 0)
    {
      baterr("ERROR: MAX20303 haptic init: %d\n", ret);
      goto errout;
    }

#if CONFIG_MAX20303_LDO1_MV != 0 || CONFIG_MAX20303_LDO2_MV != 0
  /* Rail failure leaves the sensors dead but battery + haptic still
   * work -- log loudly (ldo_config already did) and keep registering.
   */

  ldo_init(dev);
#endif

  ret = battery_gauge_register(batdev, &dev->gauge);
  if (ret < 0)
    {
      baterr("ERROR: battery_gauge_register: %d\n", ret);
      goto errout;
    }

  if (hapticdev != NULL)
    {
      g_max20303 = dev;
      ret = register_driver(hapticdev, &g_haptic_fops, 0666, dev);
      if (ret < 0)
        {
          baterr("ERROR: haptic register_driver: %d\n", ret);
          g_max20303 = NULL;

          /* Battery still works; not fatal */
        }
    }

#if defined(CONFIG_MAX20303_HAPTIC_LRA) && \
    defined(CONFIG_MAX20303_HAPTIC_PROBE_RESONANCE)
  /* Bring-up feedback: what the motor ACTUALLY resonates at, versus what
   * we told the tracker to assume. A large disagreement means every short
   * alert is being played while the tracker is still converging -- which
   * shows up as a weak or oddly-shaped buzz and reports no error anywhere,
   * so it is worth one line of boot log and one short buzz until the
   * Kconfig has been confirmed against a real board.
   */

  {
    int hz;

    nxmutex_lock(&dev->lock);
    hz = haptic_measure_resonance(dev);
    nxmutex_unlock(&dev->lock);

    if (hz <= 0)
      {
        batwarn("MAX20303 haptic: resonance probe failed "
                "(no BEMF lock) -- is a motor fitted?\n");
      }
    else
      {
        int guess = CONFIG_MAX20303_HAPTIC_LRA_HZ;
        int err   = ((hz - guess) * 100) / guess;

        syslog(err > 10 || err < -10 ? LOG_WARNING : LOG_INFO,
               "MAX20303 haptic: resonance %d Hz measured, %d Hz assumed "
               "(%+d%%)%s\n", hz, guess, err,
               (err > 10 || err < -10)
                 ? " -- set CONFIG_MAX20303_HAPTIC_LRA_HZ to the measured "
                   "value" : "");
      }
  }
#endif

  batinfo("MAX20303 registered: %s%s%s\n", batdev,
          hapticdev ? " + " : "", hapticdev ? hapticdev : "");
  return OK;

errout:
  nxmutex_destroy(&dev->lock);
  kmm_free(dev);
  return ret;
}
