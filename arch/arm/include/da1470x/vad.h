/****************************************************************************
 * arch/arm/include/da1470x/vad.h
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

#ifndef __ARCH_ARM_INCLUDE_DA1470X_VAD_H
#define __ARCH_ARM_INCLUDE_DA1470X_VAD_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdint.h>
#include <stdbool.h>

#include <nuttx/fs/ioctl.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* The voice activity detector listens on its own analogue front end while
 * the rest of the chip sleeps, and raises an interrupt when it hears
 * something louder than the background noise it has been tracking.
 *
 * Reading /dev/vad0 returns one struct vad_event_s per detection and
 * blocks until there is one; poll() works as expected.
 *
 * A detection puts the block to sleep and it stays there: listening
 * again is a mode change the reader makes when it is ready.  A sound
 * that stays above the threshold would otherwise be reported for as
 * long as it lasts, which is no use to anything waiting for one sound
 * to act on.
 */

/* Operating mode */

#define VAD_MODE_STANDBY     0  /* Off, analogue front end powered down */
#define VAD_MODE_SLEEP       1  /* Front end alive, not listening */
#define VAD_MODE_LISTENING   2  /* Listening */

/* Clock source */

#define VAD_CLK_RCLP32K      0  /* Low power oscillator, normalised */
#define VAD_CLK_XTAL32K      1  /* Crystal, steadier and more accurate */

/* How much louder than the tracked noise floor a sound has to be before
 * it counts, in decibels.
 */

#define VAD_SENSITIVITY_2DB  0
#define VAD_SENSITIVITY_3DB  1
#define VAD_SENSITIVITY_4DB  2
#define VAD_SENSITIVITY_5DB  3
#define VAD_SENSITIVITY_6DB  4
#define VAD_SENSITIVITY_8DB  5
#define VAD_SENSITIVITY_10DB 6
#define VAD_SENSITIVITY_16DB 7

/* How long a sound has to last to count, in clock cycles */

#define VAD_MINEVENT_1       0
#define VAD_MINEVENT_16      1
#define VAD_MINEVENT_32      2
#define VAD_MINEVENT_64      3
#define VAD_MINEVENT_128     4
#define VAD_MINEVENT_256     5
#define VAD_MINEVENT_512     6
#define VAD_MINEVENT_1024    7

/* How long the block waits after powering up before it trusts what it
 * hears, in clock cycles.
 */

#define VAD_MINDELAY_768     0
#define VAD_MINDELAY_1536    1
#define VAD_MINDELAY_3584    2
#define VAD_MINDELAY_9632    3

/* Divider from the clock source to the block's own clock */

#define VAD_MCLK_DIV_1       0
#define VAD_MCLK_DIV_2       1
#define VAD_MCLK_DIV_4       2
#define VAD_MCLK_DIV_8       3
#define VAD_MCLK_DIV_16      4
#define VAD_MCLK_DIV_24      5
#define VAD_MCLK_DIV_48      6

/* Input/output control */

#define VADIOC_SETCONFIG  _DIOC(0x90)  /* IN:  struct vad_config_s */
#define VADIOC_GETCONFIG  _DIOC(0x91)  /* OUT: struct vad_config_s */
#define VADIOC_SETMODE    _DIOC(0x92)  /* IN:  int, VAD_MODE_* */
#define VADIOC_GETMODE    _DIOC(0x93)  /* OUT: int, VAD_MODE_* */
#define VADIOC_GETNOISE   _DIOC(0x94)  /* OUT: uint32_t, noise floor */
#define VADIOC_SETWAKEUP  _DIOC(0x96)  /* IN:  int, non-zero to allow the
                                        *      detector to wake the system
                                        *      while it is listening */
#define VADIOC_GETWAKEUP  _DIOC(0x97)  /* OUT: int, non-zero if it would */
#define VADIOC_FLUSH      _DIOC(0x95)  /* Drop undelivered detections */

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* How the detector is tuned.  The defaults the driver starts from are the
 * ones the block itself resets to, which are a reasonable middle ground.
 */

struct vad_config_s
{
  uint8_t clksrc;       /* VAD_CLK_* */
  uint8_t mclk_div;     /* VAD_MCLK_DIV_* */
  uint8_t sensitivity;  /* VAD_SENSITIVITY_* */
  uint8_t minevent;     /* VAD_MINEVENT_* */
  uint8_t mindelay;     /* VAD_MINDELAY_* */
  uint8_t noise_track;  /* How fast the noise floor follows, 0..7 */
  uint8_t voice_track;  /* How fast the voice level follows, 0..3 */
  uint8_t nfi_thres;    /* Noise floor above which nothing is reported,
                         * 0..63 */
};

/* One detection */

struct vad_event_s
{
  uint32_t timestamp;   /* Clock tick the detection arrived on */
  uint8_t  noise;       /* Noise floor at that moment, 0..63 */
  uint8_t  overrun;     /* Detections dropped before this one */
};

#endif /* __ARCH_ARM_INCLUDE_DA1470X_VAD_H */
