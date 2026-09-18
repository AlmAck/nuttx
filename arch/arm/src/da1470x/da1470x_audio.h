/****************************************************************************
 * arch/arm/src/da1470x/da1470x_audio.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_AUDIO_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_AUDIO_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <stdbool.h>

#include <nuttx/audio/audio.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Which serial interface carries the samples.
 *
 * The chip has no separate audio peripheral: a sample rate converter sits
 * in the middle of every path and both interfaces hang off it.  A pulse
 * density stream from a digital microphone always has to pass through a
 * converter, which is what turns it into pulse code samples.
 */

#define DA1470X_AUDIO_IF_PDM       0  /* Pulse density, digital microphone */
#define DA1470X_AUDIO_IF_PCM       1  /* Pulse code serial port */

/* Frame format of the pulse code port */

#define DA1470X_PCM_FMT_PCM        0  /* Plain frame sync */
#define DA1470X_PCM_FMT_I2S        1  /* Inter-IC sound */
#define DA1470X_PCM_FMT_TDM        2  /* Time division multiplex */
#define DA1470X_PCM_FMT_IOM2       3  /* Two clocks per bit */

/* Which of the two converters a path uses */

#define DA1470X_AUDIO_SRC1         1
#define DA1470X_AUDIO_SRC2         2

/* Direction of the path */

#define DA1470X_AUDIO_CAPTURE      0  /* Interface to memory */
#define DA1470X_AUDIO_PLAYBACK     1  /* Memory to interface */

/* Delay of the pulse density data line against its clock, in nanoseconds.
 * The hardware offers four fixed steps.
 */

#define DA1470X_PDM_DELAY_0NS      0
#define DA1470X_PDM_DELAY_4NS      1
#define DA1470X_PDM_DELAY_8NS      2
#define DA1470X_PDM_DELAY_12NS     3

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* How one audio path is wired up.  The board fills this in and hands it to
 * da1470x_audio_initialize().
 */

struct da1470x_audio_config_s
{
  uint8_t  iface;        /* DA1470X_AUDIO_IF_* */
  uint8_t  direction;    /* DA1470X_AUDIO_CAPTURE or _PLAYBACK */
  uint8_t  srcid;        /* Converter to use, 1 or 2 */
  uint8_t  nchannels;    /* 1 or 2 */
  uint8_t  bits;         /* Sample width in memory: 16 or 32 */
  uint32_t samplerate;   /* Sample rate on the memory side, Hz */

  /* Pulse density interface */

  uint32_t pdm_rate;     /* Bit clock, Hz; at least 64 times the sample
                          * rate and between 62500 and 4000000 */
  uint8_t  pdm_delay;    /* DA1470X_PDM_DELAY_* */
  bool     pdm_master;   /* Drive the clock rather than follow it */
  bool     pdm_swap;     /* Take the other clock edge for this channel */

  /* Pulse code interface */

  uint8_t  pcm_format;   /* DA1470X_PCM_FMT_* */
  uint8_t  pcm_chdelay;  /* Channel delay in slots, format dependent */
  bool     pcm_master;   /* Generate the clock and frame sync */
  bool     pcm_sysclk;   /* Clock the port from the system clock rather
                          * than from the 32MHz divider output */
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_audio_initialize
 *
 * Description:
 *   Create an audio device for one path through the converter.  The
 *   returned lower half is handed to audio_register().
 *
 *   The power domain stays down and no clock runs until the device is
 *   reserved, so an unused path costs nothing.
 *
 * Input Parameters:
 *   config - How the path is wired.  The structure is not copied and has
 *            to stay valid for as long as the device exists.
 *
 * Returned Value:
 *   The lower half on success, NULL if the configuration cannot be met.
 *
 ****************************************************************************/

struct audio_lowerhalf_s *
da1470x_audio_initialize(const struct da1470x_audio_config_s *config);

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_AUDIO_H */
