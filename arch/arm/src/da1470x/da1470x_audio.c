/****************************************************************************
 * arch/arm/src/da1470x/da1470x_audio.c
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

/* The chip has no single audio peripheral.  A sample rate converter sits in
 * the middle of every path; the pulse density interface for digital
 * microphones and the pulse code serial port both hang off it, and memory
 * is reached by the general purpose DMA.  So an audio path is built from
 * four pieces: the audio power domain, the converter and its clock, the
 * serial interface and its clock, and a DMA channel.
 *
 * Every path here runs through the converter, including the pulse code
 * ones that the hardware could route straight to memory.  It costs one
 * clock and keeps a single code path.
 *
 * Only single channel paths are implemented.  The two channels of the
 * converter appear at two separate data registers, so stereo needs two DMA
 * channels filling two separate buffers, which is not what the audio
 * buffer model here describes.  Single channel also lets the converter
 * FIFO be used, which the hardware forbids in stereo.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <debug.h>

#include <nuttx/arch.h>
#include <nuttx/kmalloc.h>
#include <nuttx/mutex.h>
#include <nuttx/queue.h>
#include <nuttx/wqueue.h>
#include <nuttx/audio/audio.h>

#include "arm_internal.h"
#include "da1470x_audio.h"
#include "da1470x_clockconfig.h"
#include "da1470x_dma.h"
#include "da1470x_pmu.h"
#include "hardware/da1470x_crg_aud.h"
#include "hardware/da1470x_pcm.h"
#include "hardware/da1470x_sdadc.h"
#include "hardware/da1470x_src.h"

#ifdef CONFIG_DA1470X_AUDIO

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* The converter is specified to run at exactly 32MHz, so the divider off
 * the 32MHz reference is always one.
 */

#define AUDIO_SRC_CLOCK        32000000

/* Bounds of the pulse density bit clock */

#define AUDIO_PDM_MIN_RATE     62500
#define AUDIO_PDM_MAX_RATE     4000000

/* The converter decimates by at least this much, so the bit clock cannot
 * be closer than this to the sample rate.
 */

#define AUDIO_PDM_OVERSAMPLE   64

/* How long to wait for the converter to report both sides settled.  The
 * wait is a few sample periods; the count only has to cover that.
 */

#define AUDIO_SRC_OK_RETRIES   100000

/* How the converter presents a sample: a plain signed 16 bit value */

#define AUDIO_SDADC_RESULT_NORMAL 2

/* How long to wait for the converter's own regulator */

#define AUDIO_SDADC_LDO_RETRIES   100000

/* Byte offset added to a data register so that a narrow transfer lands on
 * the right lane of the 32 bit word.
 */

#define AUDIO_LANE_OFFSET(b)   ((b) == 16 ? 2 : 0)

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct da1470x_audio_dev_s
{
  struct audio_lowerhalf_s dev;      /* The lower half, first for casts */
  const struct da1470x_audio_config_s *cfg;

  uintptr_t  srcbase;                /* Converter register block */
  DMA_HANDLE dma;                    /* Channel moving the samples */
  uint8_t    dmachan;                /* Its number, for the pair select */

  mutex_t    lock;                   /* Guards the queue and the state */
  struct dq_queue_s pending;         /* Buffers waiting for the hardware */
  struct ap_buffer_s *active;        /* Buffer the DMA is working on */

  struct work_s work;                /* Hands finished buffers back */
  struct ap_buffer_s *done;          /* Buffer waiting to be handed back */
  int        result;                 /* How its transfer ended */

  bool       reserved;               /* Someone holds the device */
  bool       running;                /* Samples are moving */
  bool       paused;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int  audio_getcaps(struct audio_lowerhalf_s *dev, int type,
                          struct audio_caps_s *caps);
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int  audio_configure(struct audio_lowerhalf_s *dev, void *session,
                            const struct audio_caps_s *caps);
static int  audio_start(struct audio_lowerhalf_s *dev, void *session);
static int  audio_stop(struct audio_lowerhalf_s *dev, void *session);
static int  audio_pause(struct audio_lowerhalf_s *dev, void *session);
static int  audio_resume(struct audio_lowerhalf_s *dev, void *session);
static int  audio_reserve(struct audio_lowerhalf_s *dev, void **session);
static int  audio_release(struct audio_lowerhalf_s *dev, void *session);
#else
static int  audio_configure(struct audio_lowerhalf_s *dev,
                            const struct audio_caps_s *caps);
static int  audio_start(struct audio_lowerhalf_s *dev);
static int  audio_stop(struct audio_lowerhalf_s *dev);
static int  audio_pause(struct audio_lowerhalf_s *dev);
static int  audio_resume(struct audio_lowerhalf_s *dev);
static int  audio_reserve(struct audio_lowerhalf_s *dev);
static int  audio_release(struct audio_lowerhalf_s *dev);
#endif
static int  audio_shutdown(struct audio_lowerhalf_s *dev);
static int  audio_enqueue(struct audio_lowerhalf_s *dev,
                          struct ap_buffer_s *apb);
static int  audio_cancel(struct audio_lowerhalf_s *dev,
                         struct ap_buffer_s *apb);
static int  audio_ioctl(struct audio_lowerhalf_s *dev, int cmd,
                        unsigned long arg);
static int  audio_dma_next(struct da1470x_audio_dev_s *priv);
static void audio_worker(void *arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct audio_ops_s g_audio_ops =
{
  audio_getcaps,
  audio_configure,
  audio_shutdown,
  audio_start,
  audio_stop,
  audio_pause,
  audio_resume,
  NULL,                 /* allocbuffer, the upper half default will do */
  NULL,                 /* freebuffer */
  audio_enqueue,
  audio_cancel,
  audio_ioctl,
  NULL,                 /* read */
  NULL,                 /* write */
  audio_reserve,
  audio_release
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: audio_src_reg
 *
 * Description:
 *   Address of one converter register for this device.
 *
 ****************************************************************************/

static inline uintptr_t audio_src_reg(struct da1470x_audio_dev_s *priv,
                                      unsigned int offset)
{
  return priv->srcbase + offset;
}

/****************************************************************************
 * Name: audio_clocks_on
 *
 * Description:
 *   Bring the audio domain up and start the clocks the path needs.  The
 *   converter clock has to be running before any of its registers are
 *   touched, and the pulse density divider has to be programmed no later
 *   than its gate is opened.
 *
 ****************************************************************************/

static int audio_clocks_on(struct da1470x_audio_dev_s *priv)
{
  const struct da1470x_audio_config_s *cfg = priv->cfg;
  uint32_t regval;
  int ret;

  ret = da1470x_pd_enable(DA1470X_PD_AUD);
  if (ret < 0)
    {
      auderr("ERROR: the audio domain did not come up: %d\n", ret);
      return ret;
    }

  /* Converter clock: the reference divided by one */

  regval = getreg32(DA1470X_CRG_AUD_SRC_DIV);
  if (cfg->srcid == DA1470X_AUDIO_SRC2)
    {
      regval &= ~CRG_AUD_SRC_DIV_SRC2_DIV_MASK;
      regval |= CRG_AUD_SRC_DIV_SRC2_DIV(1) | CRG_AUD_SRC_DIV_CLK_SRC2_EN;
    }
  else
    {
      regval &= ~CRG_AUD_SRC_DIV_SRC_DIV_MASK;
      regval |= CRG_AUD_SRC_DIV_SRC_DIV(1) | CRG_AUD_SRC_DIV_CLK_SRC_EN;
    }

  putreg32(regval, DA1470X_CRG_AUD_SRC_DIV);
  return OK;
}

/****************************************************************************
 * Name: audio_clocks_off
 ****************************************************************************/

static void audio_clocks_off(struct da1470x_audio_dev_s *priv)
{
  uint32_t regval;

  regval  = getreg32(DA1470X_CRG_AUD_PDM_DIV);
  regval &= ~CRG_AUD_PDM_DIV_CLK_PDM_EN;
  putreg32(regval, DA1470X_CRG_AUD_PDM_DIV);

  regval  = getreg32(DA1470X_CRG_AUD_PCM_DIV);
  regval &= ~CRG_AUD_PCM_DIV_CLK_PCM_EN;
  putreg32(regval, DA1470X_CRG_AUD_PCM_DIV);

  regval  = getreg32(DA1470X_CRG_AUD_SRC_DIV);
  if (priv->cfg->srcid == DA1470X_AUDIO_SRC2)
    {
      regval &= ~CRG_AUD_SRC_DIV_CLK_SRC2_EN;
    }
  else
    {
      regval &= ~CRG_AUD_SRC_DIV_CLK_SRC_EN;
    }

  putreg32(regval, DA1470X_CRG_AUD_SRC_DIV);

  da1470x_pd_disable(DA1470X_PD_AUD);
}

/****************************************************************************
 * Name: audio_src_fs
 *
 * Description:
 *   Turn a sample rate into the converter's frequency word, and report the
 *   filter setting that goes with it.  The converter counts in units of
 *   4096 periods of its own clock per hundred samples; above 48kHz it
 *   works on a fraction of the rate and interpolates the rest.
 *
 ****************************************************************************/

static uint32_t audio_src_fs(uint32_t samplerate, uint8_t *filter)
{
  uint64_t fs;

  if (samplerate > 96000)
    {
      *filter = 3;
    }
  else if (samplerate > 48000)
    {
      *filter = 1;
    }
  else
    {
      *filter = 0;
    }

  fs = (uint64_t)4096 * (samplerate / (*filter + 1));
  return (uint32_t)(fs / 100) & 0xffffff;
}

/****************************************************************************
 * Name: audio_src_clear_flow
 *
 * Description:
 *   Clear the over and underflow flags of one side of the converter.
 *   Reconfiguring it always trips them, so they are masked until both
 *   sides report settled and only then released.
 *
 ****************************************************************************/

static void audio_src_clear_flow(struct da1470x_audio_dev_s *priv,
                                 bool input)
{
  uintptr_t ctrl = audio_src_reg(priv, DA1470X_SRC_CTRL_OFFSET);
  uint32_t over  = input ? SRC_CTRL_SRC_IN_OVFLOW : SRC_CTRL_SRC_OUT_OVFLOW;
  uint32_t under = input ? SRC_CTRL_SRC_IN_UNFLOW : SRC_CTRL_SRC_OUT_UNFLOW;
  uint32_t clr   = input ? SRC_CTRL_SRC_IN_FLOWCLR :
                           SRC_CTRL_SRC_OUT_FLOWCLR;
  int retries;

  for (retries = AUDIO_SRC_OK_RETRIES; retries > 0; retries--)
    {
      if ((getreg32(ctrl) & (over | under)) == 0)
        {
          break;
        }

      modifyreg32(ctrl, 0, clr);
    }

  modifyreg32(ctrl, clr, 0);
}

/****************************************************************************
 * Name: audio_src_configure
 *
 * Description:
 *   Set the converter up for this path: where its input comes from, the
 *   rates of both sides, which side is driven by the hardware and which
 *   by the DMA, and the small FIFO that smooths the memory side.
 *
 ****************************************************************************/

static void audio_src_configure(struct da1470x_audio_dev_s *priv)
{
  const struct da1470x_audio_config_s *cfg = priv->cfg;
  bool capture = cfg->direction == DA1470X_AUDIO_CAPTURE;
  uint32_t ctrl;
  uint32_t mux;
  uint8_t filter;
  uint32_t fs;

  /* Where the converter takes its input from.  Capturing from a
   * microphone takes it from the pulse density pads; everything else
   * comes either from the pulse code port or from the data registers the
   * DMA writes.
   */

  mux = getreg32(audio_src_reg(priv, DA1470X_SRC_MUX_OFFSET));
  mux &= ~(SRC_MUX_MUX_IN_MASK | SRC_MUX_PDM1_MUX_IN);

  if (capture && cfg->iface == DA1470X_AUDIO_IF_PDM)
    {
      mux |= SRC_MUX_PDM1_MUX_IN;
    }
  else if (capture && cfg->iface == DA1470X_AUDIO_IF_ADC)
    {
      mux |= SRC_MUX_MUX_IN(3);          /* From the analogue converter */
    }
  else if (capture)
    {
      mux |= SRC_MUX_MUX_IN(1);          /* From the pulse code port */
    }
  else
    {
      mux |= SRC_MUX_MUX_IN(2);          /* From the data registers */
    }

  putreg32(mux, audio_src_reg(priv, DA1470X_SRC_MUX_OFFSET));

  ctrl = getreg32(audio_src_reg(priv, DA1470X_SRC_CTRL_OFFSET));
  ctrl &= ~(SRC_CTRL_SRC_IN_DS_MASK | SRC_CTRL_SRC_OUT_US_MASK |
            SRC_CTRL_SRC_IN_AMODE | SRC_CTRL_SRC_OUT_AMODE |
            SRC_CTRL_SRC_FIFO_ENABLE | SRC_CTRL_SRC_FIFO_DIRECTION);

  /* Rates.  A pulse density stream has no rate of its own: the converter
   * derives it from the bit clock, so only the memory side is programmed.
   */

  if (capture)
    {
      fs = audio_src_fs(cfg->samplerate, &filter);
      putreg32(fs, audio_src_reg(priv, DA1470X_SRC_OUT_FS_OFFSET));
      ctrl |= SRC_CTRL_SRC_OUT_US(filter);

      if (cfg->iface != DA1470X_AUDIO_IF_PDM)
        {
          fs = audio_src_fs(cfg->samplerate, &filter);
          putreg32(fs, audio_src_reg(priv, DA1470X_SRC_IN_FS_OFFSET));
          ctrl |= SRC_CTRL_SRC_IN_DS(filter);
        }

      /* The serial side runs itself, the memory side is fed by the DMA */

      ctrl |= SRC_CTRL_SRC_IN_AMODE;
      ctrl |= SRC_CTRL_SRC_FIFO_DIRECTION | SRC_CTRL_SRC_FIFO_ENABLE;
    }
  else
    {
      fs = audio_src_fs(cfg->samplerate, &filter);
      putreg32(fs, audio_src_reg(priv, DA1470X_SRC_IN_FS_OFFSET));
      ctrl |= SRC_CTRL_SRC_IN_DS(filter);

      fs = audio_src_fs(cfg->samplerate, &filter);
      putreg32(fs, audio_src_reg(priv, DA1470X_SRC_OUT_FS_OFFSET));
      ctrl |= SRC_CTRL_SRC_OUT_US(filter);

      ctrl |= SRC_CTRL_SRC_OUT_AMODE;
      ctrl |= SRC_CTRL_SRC_FIFO_ENABLE;
    }

  putreg32(ctrl, audio_src_reg(priv, DA1470X_SRC_CTRL_OFFSET));

  /* Start from empty data registers and no stale flow errors */

  putreg32(0, audio_src_reg(priv, DA1470X_SRC_IN1_OFFSET));
  putreg32(0, audio_src_reg(priv, DA1470X_SRC_IN2_OFFSET));

  audio_src_clear_flow(priv, true);
  audio_src_clear_flow(priv, false);
}

/****************************************************************************
 * Name: audio_src_enable
 *
 * Description:
 *   Start the converter and wait until both of its sides report settled,
 *   keeping the flow errors that the start itself causes masked.
 *
 ****************************************************************************/

static int audio_src_enable(struct da1470x_audio_dev_s *priv)
{
  uintptr_t ctrl = audio_src_reg(priv, DA1470X_SRC_CTRL_OFFSET);
  int retries;

  modifyreg32(ctrl, 0, SRC_CTRL_SRC_IN_FLOWCLR |
                       SRC_CTRL_SRC_OUT_FLOWCLR | SRC_CTRL_SRC_EN);

  for (retries = AUDIO_SRC_OK_RETRIES; retries > 0; retries--)
    {
      uint32_t regval = getreg32(ctrl);

      if ((regval & SRC_CTRL_SRC_IN_OK) != 0 &&
          (regval & SRC_CTRL_SRC_OUT_OK) != 0)
        {
          break;
        }
    }

  modifyreg32(ctrl, SRC_CTRL_SRC_IN_FLOWCLR | SRC_CTRL_SRC_OUT_FLOWCLR, 0);

  if (retries == 0)
    {
      auderr("ERROR: the converter never settled\n");
      return -ETIMEDOUT;
    }

  return OK;
}

/****************************************************************************
 * Name: audio_src_disable
 ****************************************************************************/

static void audio_src_disable(struct da1470x_audio_dev_s *priv)
{
  modifyreg32(audio_src_reg(priv, DA1470X_SRC_CTRL_OFFSET),
              SRC_CTRL_SRC_EN, 0);
}

/****************************************************************************
 * Name: audio_adc_configure
 *
 * Description:
 *   Set the amplifier and the sigma delta converter up for an analogue
 *   microphone.  The microphone sits on the 3V rail and is always
 *   running, which is what lets the voice detector listen to the same
 *   signal while this path is powered down.
 *
 ****************************************************************************/

static int audio_adc_configure(struct da1470x_audio_dev_s *priv)
{
  const struct da1470x_audio_config_s *cfg = priv->cfg;
  uint32_t branches;
  int retries;

  if (cfg->direction != DA1470X_AUDIO_CAPTURE)
    {
      auderr("ERROR: the analogue path only captures\n");
      return -EINVAL;
    }

  /* A single ended microphone only needs the branch it is wired to */

  switch (cfg->pga_mode)
    {
      case DA1470X_PGA_MODE_SE_P:
        branches = 1;
        break;

      case DA1470X_PGA_MODE_SE_N:
        branches = 2;
        break;

      default:
        branches = 3;
        break;
    }

  putreg32(SDADC_PGA_CTRL_PGA_EN(branches) |
           SDADC_PGA_CTRL_PGA_MODE(cfg->pga_mode) |
           SDADC_PGA_CTRL_PGA_GAIN(cfg->pga_gain) |
           SDADC_PGA_CTRL_PGA_BIAS(cfg->pga_bias),
           DA1470X_SDADC_PGA_CTRL);

  /* No offset correction on the decimation filter to start from */

  putreg32(0, DA1470X_SDADC_AUDIO_FILT);

  /* Power the converter up and wait for its own regulator, then put the
   * decimation filter in the path.  The filter has to be enabled before
   * the conversion is started, not with it.
   */

  putreg32(SDADC_CTRL_EN, DA1470X_SDADC_CTRL);

  for (retries = AUDIO_SDADC_LDO_RETRIES; retries > 0; retries--)
    {
      if ((getreg32(DA1470X_SDADC_CTRL) & SDADC_CTRL_LDO_OK) != 0)
        {
          break;
        }
    }

  if (retries == 0)
    {
      auderr("ERROR: the converter regulator never came up\n");
      return -ETIMEDOUT;
    }

  putreg32(SDADC_CTRL_EN |
           SDADC_CTRL_RESULT_MODE(AUDIO_SDADC_RESULT_NORMAL) |
           SDADC_CTRL_AUDIO_FILTER_EN | SDADC_CTRL_DMA_EN |
           SDADC_CTRL_MINT, DA1470X_SDADC_CTRL);

  return OK;
}

/****************************************************************************
 * Name: audio_adc_enable
 ****************************************************************************/

static void audio_adc_enable(struct da1470x_audio_dev_s *priv, bool on)
{
  if (on)
    {
      modifyreg32(DA1470X_SDADC_CTRL, 0, SDADC_CTRL_START);
    }
  else
    {
      modifyreg32(DA1470X_SDADC_CTRL,
                  SDADC_CTRL_EN | SDADC_CTRL_START, 0);
      putreg32(0, DA1470X_SDADC_PGA_CTRL);
    }

  UNUSED(priv);
}

/****************************************************************************
 * Name: audio_pdm_configure
 *
 * Description:
 *   Set the pulse density interface up.  The divider has to be programmed
 *   no later than the moment its gate opens, so both are written here and
 *   the gate is opened again when the path starts.
 *
 ****************************************************************************/

static int audio_pdm_configure(struct da1470x_audio_dev_s *priv)
{
  const struct da1470x_audio_config_s *cfg = priv->cfg;
  uint32_t divider;
  uint32_t ctrl;
  uint32_t regval;

  if (cfg->pdm_rate < AUDIO_PDM_MIN_RATE ||
      cfg->pdm_rate > AUDIO_PDM_MAX_RATE)
    {
      auderr("ERROR: bit clock %" PRIu32 " Hz is out of range\n",
             cfg->pdm_rate);
      return -EINVAL;
    }

  if (cfg->pdm_rate < cfg->samplerate * AUDIO_PDM_OVERSAMPLE)
    {
      auderr("ERROR: bit clock %" PRIu32 " Hz is too slow for %" PRIu32
             " samples per second\n", cfg->pdm_rate, cfg->samplerate);
      return -EINVAL;
    }

  divider = AUDIO_SRC_CLOCK / cfg->pdm_rate;

  /* Data line delay and channel selection live in the converter */

  ctrl = getreg32(audio_src_reg(priv, DA1470X_SRC_CTRL_OFFSET));
  ctrl &= ~(SRC_CTRL_SRC_PDM_DI_DEL_MASK | SRC_CTRL_SRC_PDM_DO_DEL_MASK |
            SRC_CTRL_SRC_PDM_IN_INV | SRC_CTRL_SRC_PDM_OUT_INV |
            SRC_CTRL_SRC_PDM_MODE_MASK);

  if (cfg->direction == DA1470X_AUDIO_CAPTURE)
    {
      ctrl |= SRC_CTRL_SRC_PDM_DI_DEL(cfg->pdm_delay);

      /* The input path takes the opposite sense of the output path: the
       * bit selects the channel that is NOT swapped.
       */

      if (!cfg->pdm_swap)
        {
          ctrl |= SRC_CTRL_SRC_PDM_IN_INV;
        }
    }
  else
    {
      ctrl |= SRC_CTRL_SRC_PDM_DO_DEL(cfg->pdm_delay);

      if (cfg->pdm_swap)
        {
          ctrl |= SRC_CTRL_SRC_PDM_OUT_INV;
        }
    }

  putreg32(ctrl, audio_src_reg(priv, DA1470X_SRC_CTRL_OFFSET));

  regval  = getreg32(DA1470X_CRG_AUD_PDM_DIV);
  regval &= ~(CRG_AUD_PDM_DIV_PDM_DIV_MASK |
              CRG_AUD_PDM_DIV_PDM_MASTER_MODE);
  regval |= CRG_AUD_PDM_DIV_PDM_DIV(divider);

  if (cfg->pdm_master)
    {
      regval |= CRG_AUD_PDM_DIV_PDM_MASTER_MODE;
    }

  putreg32(regval, DA1470X_CRG_AUD_PDM_DIV);
  return OK;
}

/****************************************************************************
 * Name: audio_pcm_configure
 *
 * Description:
 *   Set the pulse code port up.  Only whole dividers are supported: if the
 *   chosen clock does not divide down to the bit clock exactly the path is
 *   refused rather than run at the wrong rate.
 *
 ****************************************************************************/

static int audio_pcm_configure(struct da1470x_audio_dev_s *priv)
{
  const struct da1470x_audio_config_s *cfg = priv->cfg;
  uint32_t source;
  uint32_t bitclock;
  uint32_t divider;
  uint32_t fscdiv;
  uint32_t ctrl;
  uint32_t regval;

  /* Bits carried in one frame, and from that the bit clock */

  fscdiv = (uint32_t)cfg->bits * cfg->nchannels;
  if (cfg->pcm_format == DA1470X_PCM_FMT_TDM)
    {
      fscdiv += (uint32_t)cfg->pcm_chdelay * 8;
    }

  if (cfg->pcm_format == DA1470X_PCM_FMT_IOM2)
    {
      fscdiv *= 2;                       /* Two clocks for every bit */
    }

  if (fscdiv == 0 || fscdiv > 0xfff)
    {
      return -EINVAL;
    }

  source   = cfg->pcm_sysclk ? da1470x_get_sysclk() : AUDIO_SRC_CLOCK;
  bitclock = cfg->samplerate * fscdiv;

  if (bitclock == 0 || source % bitclock != 0)
    {
      auderr("ERROR: %" PRIu32 " Hz does not divide to a %" PRIu32
             " Hz bit clock\n", source, bitclock);
      return -EINVAL;
    }

  divider = source / bitclock;
  if (divider == 0 || divider > 0xfff)
    {
      return -EINVAL;
    }

  /* Stop the port and force the unused channels to silence before it is
   * reconfigured.
   */

  putreg32(0, DA1470X_PCM1_CTRL);
  putreg32(0, DA1470X_PCM1_OUT1);
  putreg32(0, DA1470X_PCM1_OUT2);

  regval  = getreg32(DA1470X_CRG_AUD_PCM_DIV);
  regval &= ~(CRG_AUD_PCM_DIV_PCM_DIV_MASK | CRG_AUD_PCM_DIV_PCM_SRC_SEL);
  regval |= CRG_AUD_PCM_DIV_PCM_DIV(divider);

  if (cfg->pcm_sysclk)
    {
      regval |= CRG_AUD_PCM_DIV_PCM_SRC_SEL;
    }

  putreg32(regval, DA1470X_CRG_AUD_PCM_DIV);
  putreg32(0, DA1470X_CRG_AUD_PCM_FDIV);

  ctrl = PCM_CTRL_PCM_FSC_DIV(fscdiv);

  if (cfg->pcm_master)
    {
      ctrl |= PCM_CTRL_PCM_MASTER;
    }

  switch (cfg->pcm_format)
    {
      case DA1470X_PCM_FMT_I2S:

        /* Frame sync on both edges, one cycle of it before the first bit,
         * and the bit clock inverted, which is what the format asks for.
         */

        ctrl |= PCM_CTRL_PCM_FSC_EDGE | PCM_CTRL_PCM_FSCDEL |
                PCM_CTRL_PCM_CLKINV |
                PCM_CTRL_PCM_FSCLEN(cfg->bits / 8);
        break;

      case DA1470X_PCM_FMT_TDM:
        ctrl |= PCM_CTRL_PCM_FSC_EDGE |
                PCM_CTRL_PCM_CH_DEL(cfg->pcm_chdelay) |
                PCM_CTRL_PCM_FSCLEN(cfg->bits / 8 + cfg->pcm_chdelay);
        break;

      case DA1470X_PCM_FMT_IOM2:

        /* Two clock cycles per bit, which needs an even frame */

        if ((fscdiv & 1) != 0)
          {
            return -EINVAL;
          }

        ctrl |= PCM_CTRL_PCM_CLK_BIT;
        break;

      case DA1470X_PCM_FMT_PCM:
      default:
        ctrl |= PCM_CTRL_PCM_CH_DEL(cfg->pcm_chdelay) |
                PCM_CTRL_PCM_FSCLEN(cfg->bits / 8);
        break;
    }

  putreg32(ctrl, DA1470X_PCM1_CTRL);

  /* On playback the port takes its samples from this converter */

  if (cfg->direction == DA1470X_AUDIO_PLAYBACK)
    {
      regval  = getreg32(DA1470X_SRC1_MUX);
      regval &= ~SRC_MUX_PCM1_MUX_IN_MASK;
      regval |= SRC_MUX_PCM1_MUX_IN(
                  cfg->srcid == DA1470X_AUDIO_SRC2 ? 3 : 1);
      putreg32(regval, DA1470X_SRC1_MUX);
    }

  return OK;
}

/****************************************************************************
 * Name: audio_dma_alloc
 *
 * Description:
 *   Take a DMA channel of the parity the hardware insists on: samples
 *   travelling from a peripheral into memory use an even channel, samples
 *   going the other way an odd one.
 *
 ****************************************************************************/

static int audio_dma_alloc(struct da1470x_audio_dev_s *priv)
{
  int first = priv->cfg->direction == DA1470X_AUDIO_CAPTURE ? 0 : 1;
  int chan;

  for (chan = first; chan < 8; chan += 2)
    {
      priv->dma = da1470x_dmach_alloc(chan);
      if (priv->dma != NULL)
        {
          priv->dmachan = chan;
          return OK;
        }
    }

  return -EBUSY;
}

/****************************************************************************
 * Name: audio_dma_callback
 *
 * Description:
 *   End of one buffer.  The work is handed to a worker: the upper half
 *   may take locks, which must not happen in an interrupt.
 *
 ****************************************************************************/

static void audio_dma_callback(DMA_HANDLE handle, void *arg, int result)
{
  struct da1470x_audio_dev_s *priv = (struct da1470x_audio_dev_s *)arg;

  priv->done   = priv->active;
  priv->active = NULL;
  priv->result = result;

  if (priv->done != NULL)
    {
      work_queue(LPWORK, &priv->work, audio_worker, priv, 0);
    }
}

/****************************************************************************
 * Name: audio_dma_next
 *
 * Description:
 *   Point the DMA at the buffer at the head of the queue and start it.
 *   Called with the lock held.
 *
 ****************************************************************************/

static int audio_dma_next(struct da1470x_audio_dev_s *priv)
{
  const struct da1470x_audio_config_s *cfg = priv->cfg;
  struct da1470x_dma_config_s dmacfg;
  struct ap_buffer_s *apb;
  uintptr_t datareg;
  uint32_t bytes;

  if (priv->active != NULL || !priv->running || priv->paused)
    {
      return OK;
    }

  apb = (struct ap_buffer_s *)dq_remfirst(&priv->pending);
  if (apb == NULL)
    {
      return OK;
    }

  bytes = cfg->direction == DA1470X_AUDIO_CAPTURE ?
          apb->nmaxbytes : apb->nbytes;

  memset(&dmacfg, 0, sizeof(dmacfg));
  dmacfg.bw     = cfg->bits == 16 ? DA1470X_DMA_BW_16 : DA1470X_DMA_BW_32;
  dmacfg.len    = bytes / (cfg->bits / 8);
  dmacfg.prio   = DA1470X_DMA_PRIO_HIGHEST;
  dmacfg.dreq   = true;
  dmacfg.idle   = true;
  dmacfg.burst  = DA1470X_DMA_BURST_1;
  dmacfg.peripheral = cfg->srcid == DA1470X_AUDIO_SRC2 ?
                      DA1470X_DMA_PERIPH_SRC2 : DA1470X_DMA_PERIPH_SRC;

  if (cfg->direction == DA1470X_AUDIO_CAPTURE)
    {
      datareg = audio_src_reg(priv, DA1470X_SRC_OUT1_OFFSET) +
                AUDIO_LANE_OFFSET(cfg->bits);
      dmacfg.src  = datareg;
      dmacfg.dest = (uintptr_t)apb->samp;
      dmacfg.ainc = false;
      dmacfg.binc = true;
    }
  else
    {
      datareg = audio_src_reg(priv, DA1470X_SRC_IN1_OFFSET) +
                AUDIO_LANE_OFFSET(cfg->bits);
      dmacfg.src  = (uintptr_t)apb->samp;
      dmacfg.dest = datareg;
      dmacfg.ainc = true;
      dmacfg.binc = false;
    }

  dmacfg.int_len = dmacfg.len;

  priv->active = apb;

  da1470x_dma_setup(priv->dma, &dmacfg, audio_dma_callback, priv);
  return da1470x_dma_start(priv->dma);
}

/****************************************************************************
 * Name: audio_worker
 *
 * Description:
 *   Give one finished buffer back to the upper half and start the next.
 *
 ****************************************************************************/

static void audio_worker(void *arg)
{
  struct da1470x_audio_dev_s *priv = (struct da1470x_audio_dev_s *)arg;
  struct ap_buffer_s *apb;
  int result;

  nxmutex_lock(&priv->lock);

  apb        = priv->done;
  result     = priv->result;
  priv->done = NULL;

  if (apb != NULL && priv->cfg->direction == DA1470X_AUDIO_CAPTURE)
    {
      apb->nbytes = result < 0 ? 0 : apb->nmaxbytes;
    }

  audio_dma_next(priv);
  nxmutex_unlock(&priv->lock);

  if (apb != NULL)
    {
      priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_DEQUEUE, apb, result);
    }
}

/****************************************************************************
 * Name: audio_getcaps
 ****************************************************************************/

static int audio_getcaps(struct audio_lowerhalf_s *dev, int type,
                         struct audio_caps_s *caps)
{
  struct da1470x_audio_dev_s *priv = (struct da1470x_audio_dev_s *)dev;
  bool capture = priv->cfg->direction == DA1470X_AUDIO_CAPTURE;

  caps->ac_format.hw  = 0;
  caps->ac_controls.w = 0;

  switch (caps->ac_type)
    {
      case AUDIO_TYPE_QUERY:

        /* Say which direction this path runs in */

        caps->ac_channels = priv->cfg->nchannels;

        if (caps->ac_subtype == AUDIO_TYPE_QUERY)
          {
            caps->ac_controls.b[0] = capture ? AUDIO_TYPE_INPUT :
                                               AUDIO_TYPE_OUTPUT;
          }
        break;

      case AUDIO_TYPE_INPUT:
      case AUDIO_TYPE_OUTPUT:
        caps->ac_channels = priv->cfg->nchannels;

        if (caps->ac_subtype == AUDIO_TYPE_QUERY)
          {
            /* The converter is programmed for one rate, so only that one
             * is offered.
             */

            caps->ac_controls.hw[0] = priv->cfg->samplerate & 0xffff;
            caps->ac_controls.hw[1] = priv->cfg->samplerate >> 16;
            caps->ac_controls.b[3]  = priv->cfg->bits;
          }
        break;

      default:
        caps->ac_subtype = 0;
        caps->ac_channels = 0;
        break;
    }

  return caps->ac_len;
}

/****************************************************************************
 * Name: audio_configure
 *
 * Description:
 *   The converter and the serial port are set up once for the rate the
 *   board asked for, so a request that does not match it is refused
 *   rather than silently ignored.
 *
 ****************************************************************************/

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int audio_configure(struct audio_lowerhalf_s *dev, void *session,
                           const struct audio_caps_s *caps)
#else
static int audio_configure(struct audio_lowerhalf_s *dev,
                           const struct audio_caps_s *caps)
#endif
{
  struct da1470x_audio_dev_s *priv = (struct da1470x_audio_dev_s *)dev;
  const struct da1470x_audio_config_s *cfg = priv->cfg;
  uint32_t samplerate;

  switch (caps->ac_type)
    {
      case AUDIO_TYPE_INPUT:
      case AUDIO_TYPE_OUTPUT:
        samplerate = caps->ac_controls.hw[0] |
                     ((uint32_t)caps->ac_controls.hw[1] << 16);

        if (samplerate != 0 && samplerate != cfg->samplerate)
          {
            auderr("ERROR: only %" PRIu32 " samples per second\n",
                   cfg->samplerate);
            return -EINVAL;
          }

        if (caps->ac_channels != 0 && caps->ac_channels != cfg->nchannels)
          {
            return -EINVAL;
          }

        if (caps->ac_controls.b[2] != 0 &&
            caps->ac_controls.b[2] != cfg->bits)
          {
            return -EINVAL;
          }
        break;

      default:
        break;
    }

  return OK;
}

/****************************************************************************
 * Name: audio_shutdown
 ****************************************************************************/

static int audio_shutdown(struct audio_lowerhalf_s *dev)
{
  struct da1470x_audio_dev_s *priv = (struct da1470x_audio_dev_s *)dev;

  nxmutex_lock(&priv->lock);

  if (priv->running)
    {
      da1470x_dma_stop(priv->dma);
      audio_src_disable(priv);
      priv->running = false;
    }

  nxmutex_unlock(&priv->lock);
  return OK;
}

/****************************************************************************
 * Name: audio_start
 *
 * Description:
 *   Build the path and let the samples move.  The serial interface is
 *   started before the converter, which the hardware asks for.
 *
 ****************************************************************************/

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int audio_start(struct audio_lowerhalf_s *dev, void *session)
#else
static int audio_start(struct audio_lowerhalf_s *dev)
#endif
{
  struct da1470x_audio_dev_s *priv = (struct da1470x_audio_dev_s *)dev;
  const struct da1470x_audio_config_s *cfg = priv->cfg;
  uint32_t regval;
  int ret;

  nxmutex_lock(&priv->lock);

  if (priv->running)
    {
      nxmutex_unlock(&priv->lock);
      return OK;
    }

  audio_src_configure(priv);

  if (cfg->iface == DA1470X_AUDIO_IF_PDM)
    {
      ret = audio_pdm_configure(priv);
    }
  else if (cfg->iface == DA1470X_AUDIO_IF_ADC)
    {
      ret = audio_adc_configure(priv);
    }
  else
    {
      ret = audio_pcm_configure(priv);
    }

  if (ret < 0)
    {
      nxmutex_unlock(&priv->lock);
      return ret;
    }

  /* Open the interface clock gate, then the first transfer, and only then
   * the converter.
   */

  if (cfg->iface == DA1470X_AUDIO_IF_PDM)
    {
      if (cfg->pdm_master)
        {
          regval = getreg32(DA1470X_CRG_AUD_PDM_DIV);
          putreg32(regval | CRG_AUD_PDM_DIV_CLK_PDM_EN,
                   DA1470X_CRG_AUD_PDM_DIV);
        }
    }
  else if (cfg->iface == DA1470X_AUDIO_IF_ADC)
    {
      audio_adc_enable(priv, true);
    }
  else
    {
      regval = getreg32(DA1470X_CRG_AUD_PCM_DIV);
      putreg32(regval | CRG_AUD_PCM_DIV_CLK_PCM_EN,
               DA1470X_CRG_AUD_PCM_DIV);
      modifyreg32(DA1470X_PCM1_CTRL, 0, PCM_CTRL_PCM_EN);
    }

  priv->running = true;
  priv->paused  = false;

  ret = audio_dma_next(priv);
  if (ret >= 0)
    {
      ret = audio_src_enable(priv);
    }

  if (ret < 0)
    {
      priv->running = false;
    }

  nxmutex_unlock(&priv->lock);
  return ret;
}

/****************************************************************************
 * Name: audio_stop
 ****************************************************************************/

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int audio_stop(struct audio_lowerhalf_s *dev, void *session)
#else
static int audio_stop(struct audio_lowerhalf_s *dev)
#endif
{
  struct da1470x_audio_dev_s *priv = (struct da1470x_audio_dev_s *)dev;
  struct ap_buffer_s *apb;

  nxmutex_lock(&priv->lock);

  da1470x_dma_stop(priv->dma);
  audio_src_disable(priv);

  if (priv->cfg->iface == DA1470X_AUDIO_IF_PCM)
    {
      modifyreg32(DA1470X_PCM1_CTRL, PCM_CTRL_PCM_EN, 0);
    }
  else if (priv->cfg->iface == DA1470X_AUDIO_IF_ADC)
    {
      audio_adc_enable(priv, false);
    }

  priv->running = false;
  priv->paused  = false;
  priv->active  = NULL;

  /* Give every queued buffer back so the upper half is not left waiting */

  while ((apb = (struct ap_buffer_s *)dq_remfirst(&priv->pending)) != NULL)
    {
      nxmutex_unlock(&priv->lock);
      priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_DEQUEUE, apb, OK);
      nxmutex_lock(&priv->lock);
    }

  nxmutex_unlock(&priv->lock);

  priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_COMPLETE, NULL, OK);
  return OK;
}

/****************************************************************************
 * Name: audio_pause
 ****************************************************************************/

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int audio_pause(struct audio_lowerhalf_s *dev, void *session)
#else
static int audio_pause(struct audio_lowerhalf_s *dev)
#endif
{
  struct da1470x_audio_dev_s *priv = (struct da1470x_audio_dev_s *)dev;

  nxmutex_lock(&priv->lock);

  if (priv->running && !priv->paused)
    {
      da1470x_dma_stop(priv->dma);
      audio_src_disable(priv);
      priv->paused = true;
    }

  nxmutex_unlock(&priv->lock);
  return OK;
}

/****************************************************************************
 * Name: audio_resume
 ****************************************************************************/

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int audio_resume(struct audio_lowerhalf_s *dev, void *session)
#else
static int audio_resume(struct audio_lowerhalf_s *dev)
#endif
{
  struct da1470x_audio_dev_s *priv = (struct da1470x_audio_dev_s *)dev;
  int ret = OK;

  nxmutex_lock(&priv->lock);

  if (priv->running && priv->paused)
    {
      priv->paused = false;
      priv->active = NULL;

      ret = audio_dma_next(priv);
      if (ret >= 0)
        {
          ret = audio_src_enable(priv);
        }
    }

  nxmutex_unlock(&priv->lock);
  return ret;
}

/****************************************************************************
 * Name: audio_enqueue
 ****************************************************************************/

static int audio_enqueue(struct audio_lowerhalf_s *dev,
                         struct ap_buffer_s *apb)
{
  struct da1470x_audio_dev_s *priv = (struct da1470x_audio_dev_s *)dev;
  int ret;

  nxmutex_lock(&priv->lock);
  dq_addlast(&apb->dq_entry, &priv->pending);
  ret = audio_dma_next(priv);
  nxmutex_unlock(&priv->lock);

  return ret;
}

/****************************************************************************
 * Name: audio_cancel
 ****************************************************************************/

static int audio_cancel(struct audio_lowerhalf_s *dev,
                        struct ap_buffer_s *apb)
{
  struct da1470x_audio_dev_s *priv = (struct da1470x_audio_dev_s *)dev;

  nxmutex_lock(&priv->lock);

  /* A buffer the hardware is already working on cannot be taken back */

  if (priv->active != apb)
    {
      dq_rem(&apb->dq_entry, &priv->pending);
    }

  nxmutex_unlock(&priv->lock);
  return OK;
}

/****************************************************************************
 * Name: audio_ioctl
 ****************************************************************************/

static int audio_ioctl(struct audio_lowerhalf_s *dev, int cmd,
                       unsigned long arg)
{
  struct da1470x_audio_dev_s *priv = (struct da1470x_audio_dev_s *)dev;
  struct ap_buffer_info_s *info;

  switch (cmd)
    {
      case AUDIOIOC_GETBUFFERINFO:

        /* Two buffers are the fewest that keep the hardware fed while one
         * of them is being handled.
         */

        info = (struct ap_buffer_info_s *)arg;
        info->buffer_size = CONFIG_DA1470X_AUDIO_BUFSIZE;
        info->nbuffers    = CONFIG_DA1470X_AUDIO_NBUFFERS;
        return OK;

      default:
        break;
    }

  UNUSED(priv);
  return -ENOTTY;
}

/****************************************************************************
 * Name: audio_reserve
 *
 * Description:
 *   Claim the device.  The power domain and the clocks come up here and
 *   go away again on release, so an idle path costs nothing.
 *
 ****************************************************************************/

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int audio_reserve(struct audio_lowerhalf_s *dev, void **session)
#else
static int audio_reserve(struct audio_lowerhalf_s *dev)
#endif
{
  struct da1470x_audio_dev_s *priv = (struct da1470x_audio_dev_s *)dev;
  int ret;

  nxmutex_lock(&priv->lock);

  if (priv->reserved)
    {
      nxmutex_unlock(&priv->lock);
      return -EBUSY;
    }

  ret = audio_dma_alloc(priv);
  if (ret < 0)
    {
      auderr("ERROR: no DMA channel of the right parity\n");
      nxmutex_unlock(&priv->lock);
      return ret;
    }

  ret = audio_clocks_on(priv);
  if (ret < 0)
    {
      da1470x_dmach_free(priv->dma);
      priv->dma = NULL;
      nxmutex_unlock(&priv->lock);
      return ret;
    }

  priv->reserved = true;

#ifdef CONFIG_AUDIO_MULTI_SESSION
  *session = NULL;
#endif

  nxmutex_unlock(&priv->lock);
  return OK;
}

/****************************************************************************
 * Name: audio_release
 ****************************************************************************/

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int audio_release(struct audio_lowerhalf_s *dev, void *session)
#else
static int audio_release(struct audio_lowerhalf_s *dev)
#endif
{
  struct da1470x_audio_dev_s *priv = (struct da1470x_audio_dev_s *)dev;

  nxmutex_lock(&priv->lock);

  if (priv->reserved)
    {
      audio_clocks_off(priv);

      if (priv->dma != NULL)
        {
          da1470x_dmach_free(priv->dma);
          priv->dma = NULL;
        }

      priv->reserved = false;
    }

  nxmutex_unlock(&priv->lock);
  return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_audio_initialize
 ****************************************************************************/

struct audio_lowerhalf_s *
da1470x_audio_initialize(const struct da1470x_audio_config_s *config)
{
  struct da1470x_audio_dev_s *priv;

  DEBUGASSERT(config != NULL);

  if (config->nchannels != 1)
    {
      auderr("ERROR: only one channel per path is implemented\n");
      return NULL;
    }

  if (config->bits != 16 && config->bits != 32)
    {
      auderr("ERROR: %u bits per sample is not supported\n", config->bits);
      return NULL;
    }

  if (config->srcid != DA1470X_AUDIO_SRC1 &&
      config->srcid != DA1470X_AUDIO_SRC2)
    {
      return NULL;
    }

  if (config->samplerate == 0)
    {
      return NULL;
    }

  if (config->iface == DA1470X_AUDIO_IF_ADC &&
      config->direction != DA1470X_AUDIO_CAPTURE)
    {
      auderr("ERROR: the analogue path only captures\n");
      return NULL;
    }

  priv = kmm_zalloc(sizeof(struct da1470x_audio_dev_s));
  if (priv == NULL)
    {
      return NULL;
    }

  priv->dev.ops = &g_audio_ops;
  priv->cfg     = config;
  priv->srcbase = config->srcid == DA1470X_AUDIO_SRC2 ?
                  DA1470X_SRC2_BASE : DA1470X_SRC1_BASE;

  nxmutex_init(&priv->lock);
  dq_init(&priv->pending);

  audinfo("Audio path on converter %u, %s, %" PRIu32 " Hz\n",
          config->srcid,
          config->direction == DA1470X_AUDIO_CAPTURE ? "capture" :
                                                       "playback",
          config->samplerate);

  return &priv->dev;
}

#endif /* CONFIG_DA1470X_AUDIO */
