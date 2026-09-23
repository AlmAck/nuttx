/****************************************************************************
 * arch/arm/src/da1470x/da1470x_pwm.c
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

/* Pulse width modulation on the general purpose timers.
 *
 * Each of TIMER3 to TIMER6 has one output, and the pair of sixteen bit
 * fields in its PWM_CTRL register says what that output does: one is the
 * period in timer ticks less one, the other the number of those ticks the
 * output is high.  A five bit prescaler in front of them is what makes the
 * slow end reachable.
 *
 * The reason this exists is a vibration motor, but nothing here assumes
 * one.  What is on the far end of the pin is the board's business.
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
#include <nuttx/timers/pwm.h>

#include "arm_internal.h"
#include "hardware/da1470x_timer.h"
#include "hardware/da1470x_memorymap.h"
#include "da1470x_clockconfig.h"
#include "da1470x_gpio.h"
#include "da1470x_pwm.h"

#ifdef CONFIG_DA1470X_PWM

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Both fields of PWM_CTRL are sixteen bits, so a period is at most this
 * many ticks and the prescaler is what reaches anything slower.
 */

#define PWM_PERIOD_MAX  0x10000
#define PWM_PRESCALE_MAX 0x1f

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct da1470x_pwm_s
{
  const struct pwm_ops_s *ops;   /* Must be first: the upper half casts */
  uint32_t         base;         /* Of the timer */
  da1470x_pinset_t pinset;       /* The output */
  bool             started;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int pwm_setup(struct pwm_lowerhalf_s *dev);
static int pwm_shutdown(struct pwm_lowerhalf_s *dev);
static int pwm_start(struct pwm_lowerhalf_s *dev,
                     const struct pwm_info_s *info);
static int pwm_stop(struct pwm_lowerhalf_s *dev);
static int pwm_ioctl(struct pwm_lowerhalf_s *dev, int cmd,
                     unsigned long arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct pwm_ops_s g_pwm_ops =
{
  .setup    = pwm_setup,
  .shutdown = pwm_shutdown,
  .start    = pwm_start,
  .stop     = pwm_stop,
  .ioctl    = pwm_ioctl,
};

static struct da1470x_pwm_s g_pwm[4] =
{
  { .ops = &g_pwm_ops, .base = DA1470X_TIMER3_BASE },
  { .ops = &g_pwm_ops, .base = DA1470X_TIMER4_BASE },
  { .ops = &g_pwm_ops, .base = DA1470X_TIMER5_BASE },
  { .ops = &g_pwm_ops, .base = DA1470X_TIMER6_BASE },
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: pwm_putreg / pwm_getreg
 ****************************************************************************/

static inline void pwm_putreg(struct da1470x_pwm_s *priv, unsigned int off,
                              uint32_t value)
{
  putreg32(value, priv->base + off);
}

static inline uint32_t pwm_getreg(struct da1470x_pwm_s *priv,
                                  unsigned int off)
{
  return getreg32(priv->base + off);
}

/****************************************************************************
 * Name: pwm_configure
 *
 * Description:
 *   Work out a prescaler and a period that land on the asked-for frequency,
 *   and set the output high for the asked-for fraction of it.
 *
 *   The two sixteen bit fields cannot express a long period on their own,
 *   so the prescaler takes whatever multiple of 65536 ticks the period
 *   needs and the period field takes the rest.  Picking the smallest
 *   prescaler that works keeps as many ticks in the period as possible,
 *   which is what decides how finely the duty cycle can be set.
 *
 ****************************************************************************/

static int pwm_configure(struct da1470x_pwm_s *priv, uint32_t frequency,
                         ub16_t duty)
{
  uint32_t source = da1470x_get_divn_clk();
  uint32_t prescale;
  uint32_t ticks;
  uint32_t period;
  uint32_t high;

  if (frequency == 0 || frequency > source / 2)
    {
      pwmerr("%" PRIu32 " Hz is not reachable from %" PRIu32 " Hz\n",
             frequency, source);
      return -ERANGE;
    }

  ticks = source / frequency;
  if (ticks == 0)
    {
      return -ERANGE;
    }

  prescale = (ticks + PWM_PERIOD_MAX - 1) / PWM_PERIOD_MAX;
  if (prescale > 0)
    {
      prescale--;
    }

  if (prescale > PWM_PRESCALE_MAX)
    {
      pwmerr("%" PRIu32 " Hz is slower than this timer reaches\n",
             frequency);
      return -ERANGE;
    }

  period = ticks / (prescale + 1);
  if (period == 0)
    {
      period = 1;
    }

  /* duty is a 16.16 fraction of the period, so the top sixteen bits are
   * the fraction of 65536 the output should be high for.
   */

  high = ((uint64_t)period * duty) >> 16;
  if (high > period)
    {
      high = period;
    }

  /* Both fields count from zero, so the register holds one less than the
   * number of ticks.
   */

  pwm_putreg(priv, DA1470X_TIMER_SETTINGS_OFFSET,
             TIMER_SETTINGS_TIM_PRESCALER(prescale));

  pwm_putreg(priv, DA1470X_TIMER_PWM_CTRL_OFFSET,
             TIMER_PWM_CTRL_TIM_PWM_FREQ(period - 1) |
             (high << TIMER_PWM_CTRL_TIM_PWM_DC_SHIFT));

  pwminfo("%" PRIu32 " Hz: prescaler %" PRIu32 ", period %" PRIu32
          ", high %" PRIu32 "\n", frequency, prescale, period, high);
  return OK;
}

/****************************************************************************
 * Name: pwm_setup
 ****************************************************************************/

static int pwm_setup(struct pwm_lowerhalf_s *dev)
{
  struct da1470x_pwm_s *priv = (struct da1470x_pwm_s *)dev;

  return da1470x_gpio_config(priv->pinset);
}

/****************************************************************************
 * Name: pwm_shutdown
 ****************************************************************************/

static int pwm_shutdown(struct pwm_lowerhalf_s *dev)
{
  struct da1470x_pwm_s *priv = (struct da1470x_pwm_s *)dev;

  pwm_stop(dev);
  return da1470x_gpio_unconfig(priv->pinset);
}

/****************************************************************************
 * Name: pwm_start
 ****************************************************************************/

static int pwm_start(struct pwm_lowerhalf_s *dev,
                     const struct pwm_info_s *info)
{
  struct da1470x_pwm_s *priv = (struct da1470x_pwm_s *)dev;
  int ret;

  /* One output to a timer, so the first channel is the one that gets
   * driven.  Anything the caller asks for beyond it cannot be honoured.
   *
   * Refusing outright was the first instinct, and it is wrong here:
   * CONFIG_PWM_NCHANNELS is three on this board because the LED block
   * needs three, so every caller that fills the generic structure fills
   * three of them, and a one-output timer that rejects those is a device
   * nothing can drive.  Say so instead, once, and carry on with the one
   * output that exists.
   */

#if CONFIG_PWM_NCHANNELS > 1
  int i;

  for (i = 1; i < CONFIG_PWM_NCHANNELS; i++)
    {
      if (info->channels[i].duty != 0)
        {
          pwmwarn("this timer has one output, ignoring channel %d\n",
                  info->channels[i].channel);
        }
    }
#endif

  ret = pwm_configure(priv, info->frequency, info->channels[0].duty);
  if (ret < 0)
    {
      return ret;
    }

  /* Clock the timer from the divided system clock, let it run and let the
   * modulator out.
   */

  pwm_putreg(priv, DA1470X_TIMER_CTRL_OFFSET,
             TIMER_CTRL_TIM_SYS_CLK_EN | TIMER_CTRL_TIM_CLK_EN |
             TIMER_CTRL_TIM_FREE_RUN_MODE_EN | TIMER_CTRL_TIM_EN);

  priv->started = true;
  return OK;
}

/****************************************************************************
 * Name: pwm_stop
 ****************************************************************************/

static int pwm_stop(struct pwm_lowerhalf_s *dev)
{
  struct da1470x_pwm_s *priv = (struct da1470x_pwm_s *)dev;

  /* Ask for nothing before stopping the counter.  Stopping it alone leaves
   * the output wherever the last tick left it, and a motor held on by a
   * stopped timer is a worse failure than one that never started.
   */

  pwm_putreg(priv, DA1470X_TIMER_PWM_CTRL_OFFSET, 0);
  pwm_putreg(priv, DA1470X_TIMER_CTRL_OFFSET, 0);

  priv->started = false;
  return OK;
}

/****************************************************************************
 * Name: pwm_ioctl
 ****************************************************************************/

static int pwm_ioctl(struct pwm_lowerhalf_s *dev, int cmd,
                     unsigned long arg)
{
  return -ENOTTY;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_pwm_initialize
 ****************************************************************************/

struct pwm_lowerhalf_s *da1470x_pwm_initialize(int timer,
                                               da1470x_pinset_t pinset)
{
  struct da1470x_pwm_s *priv;

  if (timer < DA1470X_PWM_TIMER3 || timer > DA1470X_PWM_TIMER6)
    {
      pwmerr("no such timer: %d\n", timer);
      return NULL;
    }

#ifdef CONFIG_DA1470X_SNC
  /* TIMER6 is the sensor node controller's own timer */

  if (timer == DA1470X_PWM_TIMER6)
    {
      pwmerr("TIMER6 belongs to the sensor node controller\n");
      return NULL;
    }
#endif

  priv = &g_pwm[timer - DA1470X_PWM_TIMER3];
  priv->pinset = pinset;

  /* Leave it stopped.  Registering a device should not start a motor. */

  pwm_putreg(priv, DA1470X_TIMER_CTRL_OFFSET, 0);
  pwm_putreg(priv, DA1470X_TIMER_PWM_CTRL_OFFSET, 0);

  return (struct pwm_lowerhalf_s *)priv;
}

#endif /* CONFIG_DA1470X_PWM */
