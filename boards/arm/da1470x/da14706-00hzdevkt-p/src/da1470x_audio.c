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
#include "da1470x_vad.h"
#include "da14706-00hzdevkt-p.h"

/****************************************************************************
 * Private Data
 ****************************************************************************/

#ifdef CONFIG_DA1470X_AUDIO

/* The microphone fitted to this kit is analogue.  It sits on the 3V rail
 * and runs all the time, feeding two places at once: the voice activity
 * detector on its own dedicated pin, which keeps listening while the rest
 * of the chip sleeps, and the programmable amplifier in front of the
 * sigma delta converter, which is the recording path below.
 *
 * Measured on the kit: with the amplifier in differential mode the output
 * sits at zero and follows its gain setting, while either single ended
 * mode parks at a rail, so the microphone is wired across both branches.
 */

static const struct da1470x_audio_config_s g_mic_config =
{
  .iface      = DA1470X_AUDIO_IF_ADC,
  .direction  = DA1470X_AUDIO_CAPTURE,
  .srcid      = DA1470X_AUDIO_SRC1,
  .nchannels  = 1,
  .bits       = 16,
  .samplerate = 16000,
  .pga_gain   = DA1470X_PGA_GAIN_18DB,
  .pga_mode   = DA1470X_PGA_MODE_DIFF,
  .pga_bias   = 4
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

  /* The analogue microphone needs no pin muxing: it lands on dedicated
   * analogue balls, not on general purpose pins.
   */

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
