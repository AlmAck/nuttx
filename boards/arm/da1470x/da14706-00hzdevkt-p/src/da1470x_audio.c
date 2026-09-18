/****************************************************************************
 * boards/arm/da1470x/da14706-00hzdevkt-p/src/da1470x_audio.c
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

#include <errno.h>
#include <debug.h>

#include <nuttx/audio/audio.h>
#include <arch/board/board.h>

#include "da1470x_audio.h"
#include "da1470x_gpio.h"
#include "da1470x_vad.h"
#include "da14706-00hzdevkt-p.h"

/****************************************************************************
 * Private Data
 ****************************************************************************/

#ifdef CONFIG_DA1470X_AUDIO

/* One microphone on the pulse density interface, sampled at the rate voice
 * work usually wants.  The bit clock is the lowest the converter accepts
 * for this rate, which keeps the interface quiet.
 */

static const struct da1470x_audio_config_s g_mic_config =
{
  .iface      = DA1470X_AUDIO_IF_PDM,
  .direction  = DA1470X_AUDIO_CAPTURE,
  .srcid      = DA1470X_AUDIO_SRC1,
  .nchannels  = 1,
  .bits       = 16,
  .samplerate = 16000,
  .pdm_rate   = 1024000,
  .pdm_delay  = DA1470X_PDM_DELAY_0NS,
  .pdm_master = true,
  .pdm_swap   = false
};

#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_audio_setup
 *
 * Description:
 *   Mux the pins of the pulse density interface and register the
 *   microphone as an audio device.
 *
 ****************************************************************************/

#ifdef CONFIG_DA1470X_AUDIO
int da1470x_audio_setup(void)
{
  struct audio_lowerhalf_s *lower;
  int ret;

  da1470x_gpio_config(BOARD_PDM_DATA_PIN);
  da1470x_gpio_config(BOARD_PDM_CLK_PIN);

  lower = da1470x_audio_initialize(&g_mic_config);
  if (lower == NULL)
    {
      auderr("ERROR: cannot create the microphone path\n");
      return -ENODEV;
    }

  ret = audio_register("pcm0", lower);
  if (ret < 0)
    {
      auderr("ERROR: cannot register the microphone: %d\n", ret);
      return ret;
    }

  return OK;
}
#endif

/****************************************************************************
 * Name: da1470x_vad_setup
 *
 * Description:
 *   Register the voice activity detector.  It is left standing by; a mode
 *   change through its device starts it listening.
 *
 ****************************************************************************/

#ifdef CONFIG_DA1470X_VAD
int da1470x_vad_setup(void)
{
  return da1470x_vad_register("/dev/vad0", NULL);
}
#endif
