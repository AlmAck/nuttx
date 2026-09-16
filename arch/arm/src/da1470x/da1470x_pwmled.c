/****************************************************************************
 * arch/arm/src/da1470x/da1470x_pwmled.c
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

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <debug.h>

#include <nuttx/arch.h>
#include <nuttx/timers/pwm.h>

#include "arm_internal.h"
#include "hardware/da1470x_pwmled.h"
#include "da1470x_clockconfig.h"
#include "da1470x_pmu.h"
#include "da1470x_pwmled.h"

#ifdef CONFIG_DA1470X_PWMLED

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* The block is clocked by the 32 MHz DIVN clock.  Period and prescaler are
 * 12 and 8 bits wide, which bounds the frequency to roughly 31 Hz..7.8 kHz
 * with a usable resolution.
 */

#define PWMLED_PERIOD_MAX   0xfff
#define PWMLED_PRESCALE_MAX 0xff
#define PWMLED_FREQ_MIN     31
#define PWMLED_FREQ_MAX     7800

#define PWMLED_ALL_LEDS     (PWMLED_LEDS_DRV_CTRL_LED1_EN | \
                             PWMLED_LEDS_DRV_CTRL_LED2_EN | \
                             PWMLED_LEDS_DRV_CTRL_LED3_EN)

/* Per-LED registers are equally spaced */

#define PWMLED_CONF(n)      (DA1470X_PWMLED_LED1_PWM_CONF + 4 * (n))

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct da1470x_pwmled_s
{
  const struct pwm_ops_s *ops;
  const struct da1470x_pwmled_config_s *config;
  uint32_t frequency;
  uint16_t period;               /* Programmed PWM_LEDS_PERIOD + 1 */
  bool started;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int pwmled_setup(struct pwm_lowerhalf_s *dev);
static int pwmled_shutdown(struct pwm_lowerhalf_s *dev);
static int pwmled_start(struct pwm_lowerhalf_s *dev,
                        const struct pwm_info_s *info);
static int pwmled_stop(struct pwm_lowerhalf_s *dev);
static int pwmled_ioctl(struct pwm_lowerhalf_s *dev, int cmd,
                        unsigned long arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct pwm_ops_s g_pwmled_ops =
{
  .setup    = pwmled_setup,
  .shutdown = pwmled_shutdown,
  .start    = pwmled_start,
  .stop     = pwmled_stop,
  .ioctl    = pwmled_ioctl,
};

static struct da1470x_pwmled_s g_pwmled =
{
  .ops = &g_pwmled_ops,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: pwmled_wait_idle
 *
 * Description:
 *   Configuration registers are double-buffered; wait until the previous
 *   update has been taken over by the PWM engine.
 *
 ****************************************************************************/

static void pwmled_wait_idle(uint32_t mask)
{
  int i;

  for (i = 0; i < 100000; i++)
    {
      if ((getreg32(DA1470X_PWMLED_LEDS_STATUS) & mask) == 0)
        {
          return;
        }
    }

  pwmwarn("PWMLED busy timeout (mask %08" PRIx32 ")\n", mask);
}

/****************************************************************************
 * Name: pwmled_set_frequency
 ****************************************************************************/

static int pwmled_set_frequency(struct da1470x_pwmled_s *priv,
                                uint32_t freq)
{
  uint32_t clk = da1470x_get_divn_clk();
  uint32_t div;
  uint32_t per;

  if (freq < PWMLED_FREQ_MIN || freq > PWMLED_FREQ_MAX)
    {
      return -EINVAL;
    }

  /* Smallest prescaler that keeps the period within 12 bits */

  div = 1 + clk / ((PWMLED_PERIOD_MAX + 1) * freq);
  if (div > PWMLED_PRESCALE_MAX + 1)
    {
      div = PWMLED_PRESCALE_MAX + 1;
    }

  per = clk / (div * freq);
  if (per > PWMLED_PERIOD_MAX + 1)
    {
      per = PWMLED_PERIOD_MAX + 1;
    }

  if (per == 0)
    {
      per = 1;
    }

  pwmled_wait_idle(PWMLED_LEDS_STATUS_LED_FREQUENCY_BUSY);
  putreg32(PWMLED_LEDS_FREQUENCY_PWM_LEDS_PRESCALE(div - 1) |
           PWMLED_LEDS_FREQUENCY_PWM_LEDS_PERIOD(per - 1),
           DA1470X_PWMLED_LEDS_FREQUENCY);

  priv->frequency = freq;
  priv->period    = per;

  pwminfo("freq %" PRIu32 " -> prescale %" PRIu32 " period %" PRIu32 "\n",
          freq, div, per);
  return OK;
}

/****************************************************************************
 * Name: pwmled_set_duty
 *
 * Description:
 *   Program the START/STOP cycle of one LED from a ub16 duty.  A full
 *   duty is expressed as start == stop == 0.
 *
 ****************************************************************************/

static void pwmled_set_duty(struct da1470x_pwmled_s *priv, int led,
                            ub16_t duty)
{
  uint32_t duration;
  uint32_t stop;

  if (duty >= 0xffff)
    {
      duration = priv->period;
    }
  else
    {
      duration = ((uint32_t)priv->period * duty + 0x8000) >> 16;
      if (duration == 0 && duty != 0)
        {
          duration = 1;
        }
    }

  if (duration >= priv->period)
    {
      stop = 0;
    }
  else
    {
      stop = duration;
    }

  pwmled_wait_idle(PWMLED_LEDS_STATUS_LED1_CONF_BUSY << led);
  putreg32(PWMLED_LED1_PWM_CONF_START_CYCLE(0) |
           PWMLED_LED1_PWM_CONF_STOP_CYCLE(stop), PWMLED_CONF(led));
}

/****************************************************************************
 * Name: pwmled_enable
 *
 * Description:
 *   Enable or disable the LED drivers and PWM engines in mask.
 *
 ****************************************************************************/

static void pwmled_enable(uint32_t mask, bool on)
{
  mask &= PWMLED_ALL_LEDS;

  if (on)
    {
      modifyreg32(DA1470X_PWMLED_LEDS_DRV_CTRL, 0, mask);
      modifyreg32(DA1470X_PWMLED_LEDS_PWM_CTRL, 0, mask);
    }
  else
    {
      modifyreg32(DA1470X_PWMLED_LEDS_PWM_CTRL, mask, 0);
      modifyreg32(DA1470X_PWMLED_LEDS_DRV_CTRL, mask, 0);
    }
}

/****************************************************************************
 * Name: pwmled_setup
 ****************************************************************************/

static int pwmled_setup(struct pwm_lowerhalf_s *dev)
{
  struct da1470x_pwmled_s *priv = (struct da1470x_pwmled_s *)dev;
  const struct da1470x_pwmled_config_s *cfg = priv->config;
  uint32_t load = 0;
  uint32_t trim = 0;
  int ret;
  int i;

  /* The block lives in PD_TIM */

  ret = da1470x_pd_enable(DA1470X_PD_TIM);
  if (ret < 0)
    {
      return ret;
    }

  pwmled_enable(PWMLED_ALL_LEDS, false);
  putreg32(0, DA1470X_PWMLED_LEDS_PWM_CTRL);

  for (i = 0; i < DA1470X_PWMLED_NCHANNELS; i++)
    {
      load |= (uint32_t)(cfg->load_sel[i] & DA1470X_PWMLED_LOAD_MAX) <<
              (PWMLED_LED_LOAD_SEL_LED1_LOAD_SEL_SHIFT + 3 * i);
      trim |= (uint32_t)(cfg->curr_trim[i] & 0xf) <<
              (PWMLED_LED_CURR_TRIM_LED1_CURR_TRIM_SHIFT + 4 * i);
    }

  putreg32(load, DA1470X_PWMLED_LED_LOAD_SEL);
  putreg32(trim, DA1470X_PWMLED_LED_CURR_TRIM);

  priv->started = false;
  return OK;
}

/****************************************************************************
 * Name: pwmled_shutdown
 ****************************************************************************/

static int pwmled_shutdown(struct pwm_lowerhalf_s *dev)
{
  struct da1470x_pwmled_s *priv = (struct da1470x_pwmled_s *)dev;

  pwmled_enable(PWMLED_ALL_LEDS, false);
  priv->started = false;
  return OK;
}

/****************************************************************************
 * Name: pwmled_start
 ****************************************************************************/

static int pwmled_start(struct pwm_lowerhalf_s *dev,
                        const struct pwm_info_s *info)
{
  struct da1470x_pwmled_s *priv = (struct da1470x_pwmled_s *)dev;
  uint32_t mask = 0;
  int ret;
  int i;

  ret = pwmled_set_frequency(priv, info->frequency);
  if (ret < 0)
    {
      return ret;
    }

#ifdef CONFIG_PWM_MULTICHAN
  for (i = 0; i < CONFIG_PWM_NCHANNELS; i++)
    {
      int ch = info->channels[i].channel;

      if (ch == 0)
        {
          continue;
        }

      if (ch < 1 || ch > DA1470X_PWMLED_NCHANNELS)
        {
          return -EINVAL;
        }

      pwmled_set_duty(priv, ch - 1, info->channels[i].duty);
      mask |= 1 << (ch - 1);
    }
#else
  for (i = 0; i < DA1470X_PWMLED_NCHANNELS; i++)
    {
      if ((priv->config->chmask & (1 << i)) != 0)
        {
          pwmled_set_duty(priv, i, info->duty);
          mask |= 1 << i;
        }
    }
#endif

  /* Channels not in this request are switched off */

  pwmled_enable(PWMLED_ALL_LEDS & ~mask, false);
  pwmled_enable(mask, true);

  priv->started = true;
  return OK;
}

/****************************************************************************
 * Name: pwmled_stop
 ****************************************************************************/

static int pwmled_stop(struct pwm_lowerhalf_s *dev)
{
  struct da1470x_pwmled_s *priv = (struct da1470x_pwmled_s *)dev;

  pwmled_enable(PWMLED_ALL_LEDS, false);
  priv->started = false;
  return OK;
}

/****************************************************************************
 * Name: pwmled_ioctl
 ****************************************************************************/

static int pwmled_ioctl(struct pwm_lowerhalf_s *dev, int cmd,
                        unsigned long arg)
{
  return -ENOTTY;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_pwmled_initialize
 ****************************************************************************/

struct pwm_lowerhalf_s *
da1470x_pwmled_initialize(const struct da1470x_pwmled_config_s *config)
{
  if (config == NULL)
    {
      return NULL;
    }

  g_pwmled.config = config;
  return (struct pwm_lowerhalf_s *)&g_pwmled;
}

#endif /* CONFIG_DA1470X_PWMLED */
