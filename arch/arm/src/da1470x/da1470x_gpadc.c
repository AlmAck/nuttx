/****************************************************************************
 * arch/arm/src/da1470x/da1470x_gpadc.c
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

/* The general purpose converter.  It is the one that can see the battery:
 * the sigma-delta converter on this part is the audio one, with a
 * programmable gain amplifier in front of it and no route to any supply.
 *
 * What makes this converter worth having is the set of rails wired to it
 * internally -- the battery, the system supply, the charger input and the
 * regulated rails -- each behind a scaler of its own.  That last detail is
 * why this driver reports millivolts: a count from the battery channel and
 * a count from the system channel are not the same quantity, and handing
 * both to an application as bare numbers invites exactly the mistake the
 * scalers exist to prevent.
 *
 * Conversions are done by waiting rather than by interrupt.  With the
 * oversampling asked for here one takes well under a millisecond, which is
 * shorter than the handshake to arrange being woken up, and it lets the
 * same code answer a kernel caller during board bring-up when there is
 * nothing to yield to yet.
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
#include <nuttx/mutex.h>
#include <nuttx/analog/adc.h>
#include <nuttx/analog/ioctl.h>

#include "arm_internal.h"
#include "hardware/da1470x_gpadc.h"
#include "da1470x_gpadc.h"

#ifdef CONFIG_DA1470X_GPADC

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* How long to wait for a conversion before giving up.  Generous: the point
 * is to notice a converter that never answers, not to time one.
 */

#define GPADC_TIMEOUT_US 20000

/* Oversampling.  Six means sixty-four samples, which costs a few hundred
 * microseconds and buys a reading steady enough to watch a battery with.
 */

#define GPADC_OVERSAMPLING 6

/* Sample time, in the converter's own units */

#define GPADC_SAMPLE_TIME 2

/* What GP_ADC_SEL_P has to say for each kind of input.  Pointing the
 * multiplexer at a rail is only half of it: the positive input has to be
 * told to take the multiplexer's output, and leaving it at its reset value
 * measures the ADC0 pin instead -- which, with nothing wired to it, gives
 * a plausible-looking few tens of millivolts of noise.
 */

#define GPADC_SELP_ADC0     0
#define GPADC_SELP_ADC1     1
#define GPADC_SELP_ADC2     2
#define GPADC_SELP_ADC3     3
#define GPADC_SELP_MUX1     4
#define GPADC_SELP_DIFFTEMP 5
#define GPADC_SELP_MUX2     6
#define GPADC_SELP_DIETEMP  7

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* What a channel needs: where to point the input multiplexers, what the
 * scaler in front of it divides by, and how much attenuation the result
 * needs to stay inside the converter's range.
 */

struct gpadc_chan_s
{
  uint8_t  mux1;      /* GPADC_MUX1_*, or NONE */
  uint8_t  mux2;      /* GPADC_MUX2_*, or NONE */
  uint8_t  selp;      /* Positive input, for the external channels */
  uint8_t  attn;      /* 0: 0.9 V, 1: 1.8 V, 2: 2.7 V, 3: 3.6 V */
  uint16_t num;       /* Undo the internal scaler: multiply by num ... */
  uint16_t den;       /* ... and divide by den */
};

struct da1470x_gpadc_s
{
  const struct adc_callback_s *cb;
  const uint8_t *chanlist;
  int      nchan;
  mutex_t  lock;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int  gpadc_bind(struct adc_dev_s *dev,
                       const struct adc_callback_s *callback);
static void gpadc_reset(struct adc_dev_s *dev);
static int  gpadc_setup(struct adc_dev_s *dev);
static void gpadc_shutdown(struct adc_dev_s *dev);
static void gpadc_rxint(struct adc_dev_s *dev, bool enable);
static int  gpadc_ioctl(struct adc_dev_s *dev, int cmd, unsigned long arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* The scalers come from the part's own documentation: 0.157 on the system
 * supply, 0.164 on the charger input, 0.189 on the battery.  They are
 * written here as the fractions that undo them.
 */

static const struct gpadc_chan_s g_channels[DA1470X_ADC_NCHANNELS] =
{
  /* mux1, mux2, selp, attn, num, den */

  { GPADC_MUX1_NONE, GPADC_MUX2_VBAT, GPADC_SELP_MUX2, 0,  37,  7 }, /* VBAT 0.189 */
  { GPADC_MUX1_NONE, GPADC_MUX2_VSYS, GPADC_SELP_MUX2, 0,  51,  8 }, /* VSYS 0.157 */
  { GPADC_MUX1_NONE, GPADC_MUX2_VBUS, GPADC_SELP_MUX2, 0, 214, 35 }, /* VBUS 0.164 */
  { GPADC_MUX1_V30,  GPADC_MUX2_NONE, GPADC_SELP_MUX1, 3,   1,  1 }, /* V30 */
  { GPADC_MUX1_V18F, GPADC_MUX2_NONE, GPADC_SELP_MUX1, 2,   1,  1 }, /* V18F */
  { GPADC_MUX1_NONE, GPADC_MUX2_V18P, GPADC_SELP_MUX2, 2,   1,  1 }, /* V18P */
  { GPADC_MUX1_NONE, GPADC_MUX2_V18,  GPADC_SELP_MUX2, 2,   1,  1 }, /* V18 */
  { GPADC_MUX1_NONE, GPADC_MUX2_V14,  GPADC_SELP_MUX2, 1,   1,  1 }, /* V14 */
  { GPADC_MUX1_NONE, GPADC_MUX2_V12,  GPADC_SELP_MUX2, 1,   1,  1 }, /* V12 */
  { GPADC_MUX1_NONE, GPADC_MUX2_NONE, GPADC_SELP_ADC0, 3,   1,  1 }, /* P0[5] */
  { GPADC_MUX1_NONE, GPADC_MUX2_NONE, GPADC_SELP_ADC1, 3,   1,  1 }, /* P0[6] */
  { GPADC_MUX1_NONE, GPADC_MUX2_NONE, GPADC_SELP_ADC2, 3,   1,  1 }, /* P0[27] */
  { GPADC_MUX1_NONE, GPADC_MUX2_NONE, GPADC_SELP_ADC3, 3,   1,  1 }, /* P0[30] */
};

static const struct adc_ops_s g_gpadc_ops =
{
  .ao_bind     = gpadc_bind,
  .ao_reset    = gpadc_reset,
  .ao_setup    = gpadc_setup,
  .ao_shutdown = gpadc_shutdown,
  .ao_rxint    = gpadc_rxint,
  .ao_ioctl    = gpadc_ioctl,
};

static struct da1470x_gpadc_s g_gpadc_priv =
{
  .lock = NXMUTEX_INITIALIZER,
};

static struct adc_dev_s g_gpadc_dev =
{
  .ad_ops  = &g_gpadc_ops,
  .ad_priv = &g_gpadc_priv,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: gpadc_convert
 *
 * Description:
 *   Run one conversion on one channel and return the raw, left-aligned
 *   sixteen bit result.
 *
 ****************************************************************************/

static int gpadc_convert(const struct gpadc_chan_s *chan, uint16_t *raw)
{
  irqstate_t flags;
  int i;

  flags = enter_critical_section();

  /* Turn the converter's regulator on and let it settle.  EN_DEL is in
   * the converter's own units and the reset value is a sane one, so it is
   * left alone.
   */

  putreg32(GPADC_CTRL_EN, DA1470X_GPADC_CTRL);

  /* Single ended, chopped, oversampled, result left-aligned.
   *
   * Chopping is the one that matters for accuracy here.  It takes each
   * sample twice with the input and output polarity swapped, so the
   * converter's own offset cancels instead of having to be measured.
   * This driver does not read the calibration values the factory left in
   * one-time programmable memory, and with chopping on it does not need
   * them for a reading of this kind.
   */

  putreg32(GPADC_CTRL_EN | GPADC_CTRL_SE | GPADC_CTRL_CHOP |
           GPADC_CTRL_RESULT_MODE(0),
           DA1470X_GPADC_CTRL);

  putreg32(GPADC_CTRL2_ATTN(chan->attn) |
           GPADC_CTRL2_CONV_NRS(GPADC_OVERSAMPLING) |
           GPADC_CTRL2_SMPL_TIME(GPADC_SAMPLE_TIME),
           DA1470X_GPADC_CTRL2);

  putreg32(GPADC_SEL_MUX1(chan->mux1) | GPADC_SEL_MUX2(chan->mux2) |
           GPADC_SEL_P(chan->selp),
           DA1470X_GPADC_SEL);

  /* Uncalibrated mid-scale.  See the note about chopping above. */

  putreg32(GPADC_OFF(GPADC_OFF_DEFAULT), DA1470X_GPADC_OFFP);
  putreg32(GPADC_OFF(GPADC_OFF_DEFAULT), DA1470X_GPADC_OFFN);

  /* Clear anything left over, then go */

  putreg32(1, DA1470X_GPADC_CLEAR_INT);
  modifyreg32(DA1470X_GPADC_CTRL, 0, GPADC_CTRL_START);

  leave_critical_section(flags);

  /* START clears itself when the conversion is done, and INT is raised at
   * the same time.  Watching START is enough and does not need the
   * interrupt to be unmasked.
   */

  for (i = 0; i < GPADC_TIMEOUT_US; i++)
    {
      if ((getreg32(DA1470X_GPADC_CTRL) & GPADC_CTRL_START) == 0)
        {
          *raw = getreg32(DA1470X_GPADC_RESULT) & GPADC_RESULT_VAL_MASK;

          putreg32(1, DA1470X_GPADC_CLEAR_INT);
          putreg32(0, DA1470X_GPADC_CTRL);
          return OK;
        }

      up_udelay(1);
    }

  putreg32(0, DA1470X_GPADC_CTRL);
  aerr("conversion did not finish\n");
  return -ETIMEDOUT;
}

/****************************************************************************
 * Name: gpadc_to_mv
 *
 * Description:
 *   Turn a raw result into millivolts at the pin or rail, undoing both the
 *   attenuator and whatever scaler stands in front of the channel.
 *
 ****************************************************************************/

static int gpadc_to_mv(const struct gpadc_chan_s *chan, uint16_t raw)
{
  /* Sixty-four bits on the way through: the widest channel multiplies a
   * full scale count by 214 and then by 3600, which does not fit in
   * thirty-two.
   */

  uint64_t v = (uint64_t)raw * chan->num;

  v *= (uint64_t)(chan->attn + 1) * GPADC_VREF_MV;
  v /= (uint64_t)chan->den * UINT16_MAX;

  return (int)v;
}

/****************************************************************************
 * Name: gpadc_sample_locked
 ****************************************************************************/

static int gpadc_sample_locked(uint8_t channel, int *mv)
{
  const struct gpadc_chan_s *chan;
  uint16_t raw;
  int ret;

  if (channel >= DA1470X_ADC_NCHANNELS)
    {
      return -EINVAL;
    }

  chan = &g_channels[channel];

  ret = gpadc_convert(chan, &raw);
  if (ret < 0)
    {
      return ret;
    }

  *mv = gpadc_to_mv(chan, raw);
  return OK;
}

/****************************************************************************
 * Name: gpadc_bind
 ****************************************************************************/

static int gpadc_bind(struct adc_dev_s *dev,
                      const struct adc_callback_s *callback)
{
  struct da1470x_gpadc_s *priv = dev->ad_priv;

  priv->cb = callback;
  return OK;
}

/****************************************************************************
 * Name: gpadc_reset
 ****************************************************************************/

static void gpadc_reset(struct adc_dev_s *dev)
{
  putreg32(0, DA1470X_GPADC_CTRL);
}

/****************************************************************************
 * Name: gpadc_setup
 ****************************************************************************/

static int gpadc_setup(struct adc_dev_s *dev)
{
  return OK;
}

/****************************************************************************
 * Name: gpadc_shutdown
 ****************************************************************************/

static void gpadc_shutdown(struct adc_dev_s *dev)
{
  putreg32(0, DA1470X_GPADC_CTRL);
}

/****************************************************************************
 * Name: gpadc_rxint
 *
 * Description:
 *   Nothing to do.  Conversions here are run to completion by whoever asks
 *   for one, so there is no stream of samples arriving on its own that
 *   would need to be turned off.
 *
 ****************************************************************************/

static void gpadc_rxint(struct adc_dev_s *dev, bool enable)
{
}

/****************************************************************************
 * Name: gpadc_ioctl
 ****************************************************************************/

static int gpadc_ioctl(struct adc_dev_s *dev, int cmd, unsigned long arg)
{
  struct da1470x_gpadc_s *priv = dev->ad_priv;
  int ret;
  int i;

  if (cmd != ANIOC_TRIGGER)
    {
      return -ENOTTY;
    }

  ret = nxmutex_lock(&priv->lock);
  if (ret < 0)
    {
      return ret;
    }

  for (i = 0; i < priv->nchan; i++)
    {
      int mv;

      ret = gpadc_sample_locked(priv->chanlist[i], &mv);
      if (ret < 0)
        {
          break;
        }

      if (priv->cb != NULL && priv->cb->au_receive != NULL)
        {
          priv->cb->au_receive(dev, priv->chanlist[i], mv);
        }
    }

  nxmutex_unlock(&priv->lock);
  return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_gpadc_sample_mv
 ****************************************************************************/

int da1470x_gpadc_sample_mv(uint8_t channel, int *mv)
{
  struct da1470x_gpadc_s *priv = &g_gpadc_priv;
  int ret;

  if (mv == NULL)
    {
      return -EINVAL;
    }

  ret = nxmutex_lock(&priv->lock);
  if (ret < 0)
    {
      return ret;
    }

  ret = gpadc_sample_locked(channel, mv);
  nxmutex_unlock(&priv->lock);
  return ret;
}

/****************************************************************************
 * Name: da1470x_gpadc_initialize
 ****************************************************************************/

int da1470x_gpadc_initialize(const char *devpath,
                             const uint8_t *chanlist, int nchan)
{
  struct da1470x_gpadc_s *priv = &g_gpadc_priv;
  int i;

  if (chanlist == NULL || nchan <= 0)
    {
      return -EINVAL;
    }

  for (i = 0; i < nchan; i++)
    {
      if (chanlist[i] >= DA1470X_ADC_NCHANNELS)
        {
          return -EINVAL;
        }
    }

  priv->chanlist = chanlist;
  priv->nchan    = nchan;

  putreg32(0, DA1470X_GPADC_CTRL);

  return adc_register(devpath, &g_gpadc_dev);
}

#endif /* CONFIG_DA1470X_GPADC */
