/****************************************************************************
 * arch/arm/src/da1470x/da1470x_charger.c
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

/* The charger is a state machine in hardware.  Told the charge voltage,
 * the currents and the temperature limits, it walks pre-charge, constant
 * current, constant voltage and end of charge on its own, reports where it
 * is in a status register, and raises one interrupt when the state changes
 * and another when something is wrong.  Nothing here supervises a charge:
 * this driver configures it, starts and stops it, and translates what it
 * reports into the terms NuttX's battery charger interface uses.
 *
 * Two encodings are worth stating in one place, because both are irregular
 * and the datasheet gives them as sixty-four line tables:
 *
 *   voltage   codes 0..20 are 2800 mV in 50 mV steps, 21..60 carry on from
 *             3800 mV in 20 mV steps -- 4200 mV, the usual lithium charge
 *             voltage, is code 40 -- and 61 and 62 are 4700 and 4800 mV.
 *   current   codes 0..15 are 500 uA in 500 uA steps, 16..31 are 9 mA in
 *             1 mA steps, 32..47 are 27 mA in 3 mA steps, and anything
 *             above that is the 72 mA maximum.
 *
 * The charger is left disabled when it is registered.  Pushing current
 * into whatever happens to be connected, before anything has said what it
 * is, is not a reasonable default.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <errno.h>
#include <debug.h>

#include <nuttx/arch.h>
#include <nuttx/irq.h>
#include <nuttx/kmalloc.h>
#include <nuttx/power/battery_charger.h>
#include <nuttx/power/battery_ioctl.h>

#include "arm_internal.h"
#include "hardware/da1470x_charger.h"
#include "hardware/da1470x_crg_top.h"
#include "da1470x_charger.h"

#ifdef CONFIG_DA1470X_CHARGER

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* The main state machine, as the status register reports it */

#define CHG_FSM_POWER_UP       0
#define CHG_FSM_INIT           1
#define CHG_FSM_DISABLED       2
#define CHG_FSM_PRE_CHARGE     3
#define CHG_FSM_CC_CHARGE      4
#define CHG_FSM_CV_CHARGE      5
#define CHG_FSM_END_OF_CHARGE  6
#define CHG_FSM_TDIE_PROT      7
#define CHG_FSM_TBAT_PROT      8
#define CHG_FSM_BYPASSED       9
#define CHG_FSM_ERROR          10

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct da1470x_charger_s
{
  struct battery_charger_dev_s dev;  /* Upper half sees this */
  int      voltage;                  /* Charge voltage, mV */
  int      current;                  /* Charge current, uA */
  bool     enabled;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int chg_state(struct battery_charger_dev_s *dev, int *status);
static int chg_health(struct battery_charger_dev_s *dev, int *health);
static int chg_online(struct battery_charger_dev_s *dev, bool *status);
static int chg_voltage(struct battery_charger_dev_s *dev, int value);
static int chg_current(struct battery_charger_dev_s *dev, int value);
static int chg_input_current(struct battery_charger_dev_s *dev, int value);
static int chg_operate(struct battery_charger_dev_s *dev, uintptr_t param);
static int chg_chipid(struct battery_charger_dev_s *dev,
                      unsigned int *value);
static int chg_get_voltage(struct battery_charger_dev_s *dev, int *value);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct battery_charger_operations_s g_chg_ops =
{
  .state         = chg_state,
  .health        = chg_health,
  .online        = chg_online,
  .voltage       = chg_voltage,
  .current       = chg_current,
  .input_current = chg_input_current,
  .operate       = chg_operate,
  .chipid        = chg_chipid,
  .get_voltage   = chg_get_voltage,
};

static struct da1470x_charger_s g_charger;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: chg_mv_to_code / chg_code_to_mv
 ****************************************************************************/

static uint32_t chg_mv_to_code(int mv)
{
  if (mv <= 2800)
    {
      return 0;
    }

  if (mv <= 3800)
    {
      return (uint32_t)((mv - 2800) / 50);
    }

  if (mv <= 4600)
    {
      return (uint32_t)(20 + (mv - 3800) / 20);
    }

  return mv < 4800 ? 61 : 62;
}

static int chg_code_to_mv(uint32_t code)
{
  if (code <= 20)
    {
      return 2800 + (int)code * 50;
    }

  if (code <= 60)
    {
      return 3800 + ((int)code - 20) * 20;
    }

  return code == 61 ? 4700 : 4800;
}

/****************************************************************************
 * Name: chg_ua_to_code
 ****************************************************************************/

static uint32_t chg_ua_to_code(int ua)
{
  if (ua <= 500)
    {
      return 0;
    }

  if (ua <= 8000)
    {
      return (uint32_t)((ua - 500) / 500);
    }

  if (ua <= 24000)
    {
      return (uint32_t)(16 + (ua - 9000) / 1000);
    }

  if (ua < 72000)
    {
      return (uint32_t)(32 + (ua - 27000) / 3000);
    }

  return 47;
}

/****************************************************************************
 * Name: chg_fsm_state
 ****************************************************************************/

static uint32_t chg_fsm_state(void)
{
  return (getreg32(DA1470X_CHARGER_STATUS) &
          CHARGER_STATUS_STATE_MASK) >> CHARGER_STATUS_STATE_SHIFT;
}

/****************************************************************************
 * Name: chg_enable
 ****************************************************************************/

static void chg_enable(struct da1470x_charger_s *priv, bool enable)
{
  irqstate_t flags = enter_critical_section();

  if (enable)
    {
      modifyreg32(DA1470X_CHARGER_CTRL, 0,
                  CHARGER_CTRL_ENABLE | CHARGER_CTRL_CHARGE_START);
    }
  else
    {
      modifyreg32(DA1470X_CHARGER_CTRL,
                  CHARGER_CTRL_ENABLE | CHARGER_CTRL_CHARGE_START, 0);
    }

  priv->enabled = enable;
  leave_critical_section(flags);
}

/****************************************************************************
 * Name: chg_interrupt
 *
 * Description:
 *   The state and error interrupts share this.  The charger needs nothing
 *   from us to carry on, so all that happens here is acknowledging what
 *   was raised and telling anyone waiting on the device that something
 *   changed.
 *
 ****************************************************************************/

static int chg_interrupt(int irq, void *context, void *arg)
{
  struct da1470x_charger_s *priv = (struct da1470x_charger_s *)arg;
  uint32_t state = getreg32(DA1470X_CHARGER_STATE_IRQ_STATUS);
  uint32_t error = getreg32(DA1470X_CHARGER_ERROR_IRQ_STATUS);

  if (state != 0)
    {
      putreg32(state, DA1470X_CHARGER_STATE_IRQ_CLR);
    }

  if (error != 0)
    {
      putreg32(error, DA1470X_CHARGER_ERROR_IRQ_CLR);
      baterr("charger error %08" PRIx32 ", state %" PRIu32 "\n",
             error, chg_fsm_state());
    }

  battery_charger_changed(&priv->dev, BATTERY_STATE_CHANGED);
  return OK;
}

/****************************************************************************
 * Name: chg_state
 ****************************************************************************/

static int chg_state(struct battery_charger_dev_s *dev, int *status)
{
  switch (chg_fsm_state())
    {
      case CHG_FSM_PRE_CHARGE:
      case CHG_FSM_CC_CHARGE:
      case CHG_FSM_CV_CHARGE:
        *status = BATTERY_CHARGING;
        break;

      case CHG_FSM_END_OF_CHARGE:
        *status = BATTERY_FULL;
        break;

      case CHG_FSM_ERROR:
        *status = BATTERY_FAULT;
        break;

      case CHG_FSM_TDIE_PROT:
      case CHG_FSM_TBAT_PROT:

        /* Charging has been suspended by a temperature limit, not
         * finished and not faulted: it resumes on its own once the
         * temperature comes back.
         */

        *status = BATTERY_IDLE;
        break;

      default:
        *status = da1470x_charger_vbus_present() ? BATTERY_IDLE :
                                                   BATTERY_DISCHARGING;
        break;
    }

  return OK;
}

/****************************************************************************
 * Name: chg_health
 ****************************************************************************/

static int chg_health(struct battery_charger_dev_s *dev, int *health)
{
  uint32_t status = getreg32(DA1470X_CHARGER_STATUS);

  if ((status & CHARGER_STATUS_VBAT_OVP_COMP_OUT) != 0)
    {
      *health = BATTERY_HEALTH_OVERVOLTAGE;
    }
  else if ((status & CHARGER_STATUS_TBAT_HOT_COMP_OUT) != 0 ||
           chg_fsm_state() == CHG_FSM_TBAT_PROT)
    {
      *health = BATTERY_HEALTH_OVERHEAT;
    }
  else if ((status & CHARGER_STATUS_TDIE_COMP_OUT) != 0 ||
           chg_fsm_state() == CHG_FSM_TDIE_PROT)
    {
      /* The die, not the cell.  There is no separate report for it, and
       * saying the battery is too hot is closer than saying it is well.
       */

      *health = BATTERY_HEALTH_OVERHEAT;
    }
  else if (chg_fsm_state() == CHG_FSM_ERROR)
    {
      *health = BATTERY_HEALTH_UNSPEC_FAIL;
    }
  else
    {
      *health = BATTERY_HEALTH_GOOD;
    }

  return OK;
}

/****************************************************************************
 * Name: chg_online
 ****************************************************************************/

static int chg_online(struct battery_charger_dev_s *dev, bool *status)
{
  *status = da1470x_charger_vbus_present();
  return OK;
}

/****************************************************************************
 * Name: chg_voltage
 ****************************************************************************/

static int chg_voltage(struct battery_charger_dev_s *dev, int value)
{
  struct da1470x_charger_s *priv = (struct da1470x_charger_s *)dev;
  uint32_t code;

  if (value < 2800 || value > 4800)
    {
      return -ERANGE;
    }

  code = chg_mv_to_code(value);

  modifyreg32(DA1470X_CHARGER_VOLTAGE_PARAM,
              CHARGER_VOLTAGE_PARAM_V_CHARGE_MASK,
              CHARGER_VOLTAGE_PARAM_V_CHARGE(code));

  priv->voltage = chg_code_to_mv(code);
  batinfo("charge voltage %d mV (code %" PRIu32 ")\n", priv->voltage, code);
  return OK;
}

/****************************************************************************
 * Name: chg_current
 ****************************************************************************/

static int chg_current(struct battery_charger_dev_s *dev, int value)
{
  struct da1470x_charger_s *priv = (struct da1470x_charger_s *)dev;
  uint32_t code;

  if (value < 500 || value > 72000)
    {
      return -ERANGE;
    }

  code = chg_ua_to_code(value);

  modifyreg32(DA1470X_CHARGER_CURRENT_PARAM,
              CHARGER_CURRENT_PARAM_I_CHARGE_MASK,
              CHARGER_CURRENT_PARAM_I_CHARGE(code));

  priv->current = value;
  batinfo("charge current %d uA (code %" PRIu32 ")\n", value, code);
  return OK;
}

/****************************************************************************
 * Name: chg_input_current
 *
 * Description:
 *   There is no input current limit to set: what this charger draws is
 *   governed by the charge current it has been given.
 *
 ****************************************************************************/

static int chg_input_current(struct battery_charger_dev_s *dev, int value)
{
  return -ENOSYS;
}

/****************************************************************************
 * Name: chg_operate
 ****************************************************************************/

static int chg_operate(struct battery_charger_dev_s *dev, uintptr_t param)
{
  struct da1470x_charger_s *priv = (struct da1470x_charger_s *)dev;
  struct batio_operate_msg_s *msg = (struct batio_operate_msg_s *)param;

  if (msg == NULL)
    {
      return -EINVAL;
    }

  switch (msg->operate_type)
    {
      case BATIO_OPRTN_CHARGE:
        chg_enable(priv, true);
        break;

      case BATIO_OPRTN_EN_TERM:
        chg_enable(priv, false);
        break;

      default:
        return -EINVAL;
    }

  return OK;
}

/****************************************************************************
 * Name: chg_chipid
 ****************************************************************************/

static int chg_chipid(struct battery_charger_dev_s *dev,
                      unsigned int *value)
{
  *value = getreg32(DA1470X_CHIP_VERSION_BASE);
  return OK;
}

/****************************************************************************
 * Name: chg_get_voltage
 *
 * Description:
 *   The charge voltage the charger has been configured for.  This is not a
 *   measurement of the cell: the charger has no converter of its own, and
 *   reading the battery needs the sigma-delta converter.
 *
 ****************************************************************************/

static int chg_get_voltage(struct battery_charger_dev_s *dev, int *value)
{
  uint32_t code = (getreg32(DA1470X_CHARGER_VOLTAGE_PARAM) &
                   CHARGER_VOLTAGE_PARAM_V_CHARGE_MASK) >>
                  CHARGER_VOLTAGE_PARAM_V_CHARGE_SHIFT;

  *value = chg_code_to_mv(code);
  return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_charger_vbus_present
 ****************************************************************************/

bool da1470x_charger_vbus_present(void)
{
  return (getreg32(DA1470X_CRG_TOP_ANA_STATUS) &
          CRG_TOP_ANA_STATUS_COMP_VBUS_PLUGIN) != 0;
}

/****************************************************************************
 * Name: da1470x_charger_initialize
 ****************************************************************************/

int da1470x_charger_initialize(const char *devpath)
{
  struct da1470x_charger_s *priv = &g_charger;
  int ret;

  priv->dev.ops = &g_chg_ops;
  nxmutex_init(&priv->dev.batlock);
  list_initialize(&priv->dev.flist);

  /* Anything already running stops while the parameters are written */

  chg_enable(priv, false);

  chg_voltage(&priv->dev, CONFIG_DA1470X_CHARGER_VOLTAGE);
  chg_current(&priv->dev, CONFIG_DA1470X_CHARGER_CURRENT);

  /* Pre-charge below CONFIG_DA1470X_CHARGER_PRECHARGE_VOLTAGE, and treat
   * the charge as finished when the current has fallen to the configured
   * fraction of the charge current.
   */

  modifyreg32(DA1470X_CHARGER_VOLTAGE_PARAM,
              CHARGER_VOLTAGE_PARAM_V_PRECHARGE_MASK,
              CHARGER_VOLTAGE_PARAM_V_PRECHARGE(
                chg_mv_to_code(CONFIG_DA1470X_CHARGER_PRECHARGE_VOLTAGE)));

  modifyreg32(DA1470X_CHARGER_CURRENT_PARAM,
              CHARGER_CURRENT_PARAM_I_PRECHARGE_MASK,
              CHARGER_CURRENT_PARAM_I_PRECHARGE(
                chg_ua_to_code(CONFIG_DA1470X_CHARGER_PRECHARGE_CURRENT)));

  /* Both temperature protections on.  They are the difference between a
   * charger and a hazard, and the state machine handles them itself.
   */

  modifyreg32(DA1470X_CHARGER_CTRL, 0,
              CHARGER_CTRL_TDIE_PROT_ENABLE |
              CHARGER_CTRL_TDIE_ERROR_RESUME |
              CHARGER_CTRL_TBAT_PROT_ENABLE);

  /* Clear anything stale, then let both interrupts through */

  putreg32(0xffffffff, DA1470X_CHARGER_STATE_IRQ_CLR);
  putreg32(0xffffffff, DA1470X_CHARGER_ERROR_IRQ_CLR);

  ret = irq_attach(DA1470X_IRQ_CHARGER_STATE, chg_interrupt, priv);
  if (ret == OK)
    {
      ret = irq_attach(DA1470X_IRQ_CHARGER_ERROR, chg_interrupt, priv);
    }

  if (ret < 0)
    {
      baterr("cannot attach the charger interrupts: %d\n", ret);
      return ret;
    }

  up_enable_irq(DA1470X_IRQ_CHARGER_STATE);
  up_enable_irq(DA1470X_IRQ_CHARGER_ERROR);

  ret = battery_charger_register(devpath, &priv->dev);
  if (ret < 0)
    {
      baterr("cannot register %s: %d\n", devpath, ret);
      up_disable_irq(DA1470X_IRQ_CHARGER_STATE);
      up_disable_irq(DA1470X_IRQ_CHARGER_ERROR);
      return ret;
    }

  batinfo("charger at %s, %d mV / %d uA, disabled, VBUS %s\n",
          devpath, priv->voltage, priv->current,
          da1470x_charger_vbus_present() ? "present" : "absent");

  return OK;
}

#endif /* CONFIG_DA1470X_CHARGER */
