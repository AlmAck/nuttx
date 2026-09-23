/****************************************************************************
 * arch/arm/include/da1470x/lcdc.h
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

#ifndef __ARCH_ARM_INCLUDE_DA1470X_LCDC_H
#define __ARCH_ARM_INCLUDE_DA1470X_LCDC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>

#include <nuttx/video/fb.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifdef CONFIG_DA1470X_LCDC_AOD

/* Always-on display, as /dev/fb0 ioctls.  The set mirrors the MIPLCDIO_*
 * ioctls of the MIP bridge, so a client drives both targets the same way;
 * here the display driver itself plays the bridge.
 *
 * In always-on mode the panel sits in its low power idle mode showing a
 * face -- the time drawn from an AODF v1 record -- and the driver redraws
 * the rows the digits occupy once a minute from a low priority work item.
 * Nothing above the driver has to run, and between redraws the display
 * no longer holds the system at PM_NORMAL.
 *
 * Any frame the client sends (FBIO_UPDATE, a pan) leaves always-on mode
 * first, as RESUME would.  Leaving does not repaint: the client owns
 * that, and the frame buffer it shows was overwritten by the face.
 */

#define DA1470X_FBIOC_AOD_IDLE        _FBIOC(0x0080)  /* Arg: const struct
                                                       * da1470x_lcdc_aod_time_s *
                                                       * Draw the face for this
                                                       * time and park the panel.
                                                       * -EBUSY during an upload */
#define DA1470X_FBIOC_AOD_RESUME      _FBIOC(0x0081)  /* Arg: none */
#define DA1470X_FBIOC_AOD_RENDERTIME  _FBIOC(0x0082)  /* Arg: const struct
                                                       * da1470x_lcdc_aod_time_s *
                                                       * Reset the clock, and
                                                       * redraw if parked */

/* Face upload, streamed: BEGIN, DATA for each chunk, COMMIT.  BEGIN makes
 * IDLE answer -EBUSY until COMMIT or ABORT; call ABORT on every error path
 * after a successful BEGIN.  An installed face lives in RAM only: after a
 * reset the built-in face is back, and FACE_STATUS reports sequence 0.
 */

#define DA1470X_FBIOC_AOD_FACE_BEGIN  _FBIOC(0x0083)  /* Arg: const struct
                                                       * da1470x_lcdc_face_begin_s * */
#define DA1470X_FBIOC_AOD_FACE_DATA   _FBIOC(0x0084)  /* Arg: const struct
                                                       * da1470x_lcdc_face_data_s * */
#define DA1470X_FBIOC_AOD_FACE_COMMIT _FBIOC(0x0085)  /* Arg: 0 install,
                                                       * 1 validate only */
#define DA1470X_FBIOC_AOD_FACE_CLEAR  _FBIOC(0x0086)  /* Arg: none; back to
                                                       * the built-in face */
#define DA1470X_FBIOC_AOD_FACE_ABORT  _FBIOC(0x0087)  /* Arg: none */
#define DA1470X_FBIOC_AOD_FACE_STATUS _FBIOC(0x0088)  /* Arg: struct
                                                       * da1470x_lcdc_face_status_s * */

#define DA1470X_LCDC_FACE_COMMIT_WRITE 0
#define DA1470X_LCDC_FACE_COMMIT_DRY   1

#endif /* CONFIG_DA1470X_LCDC_AOD */

/****************************************************************************
 * Public Types
 ****************************************************************************/

#ifdef CONFIG_DA1470X_LCDC_AOD

/* Wall clock time, as the MIP bridge takes it */

struct da1470x_lcdc_aod_time_s
{
  uint8_t hh;
  uint8_t mm;
  uint8_t ss;
};

struct da1470x_lcdc_face_begin_s
{
  uint16_t total_len;      /* Size of the whole AODF record */
  uint16_t seq;            /* Caller's version tag, reported back */
};

struct da1470x_lcdc_face_data_s
{
  uint16_t           off;
  uint16_t           len;
  FAR const uint8_t *data;
};

struct da1470x_lcdc_face_status_s
{
  uint8_t  uploading;      /* An upload is open */
  uint8_t  parked;         /* Always-on mode is active */
  uint16_t active_seq;     /* 0 = the built-in face (or none) */
};

#endif /* CONFIG_DA1470X_LCDC_AOD */

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

/****************************************************************************
 * Name: da1470x_lcdc_set_brightness
 *
 * Description:
 *   Set the panel's brightness.  Only panels that carry a brightness hook
 *   answer this; the rest return -ENOTSUP.
 *
 *   The level is remembered across power transitions.  Powering the panel
 *   on re-runs its init, which resets it to full, so the driver reapplies
 *   the stored level afterwards -- a level set while the panel is dark is
 *   kept rather than dropped, and callers need not order the two.
 *
 * Input Parameters:
 *   level - 0 (darkest) to 255 (full).  Note that 0 does not power the
 *           panel down: it stays on, and keeps the system at PM_NORMAL.
 *           Use FBIOSET_POWER to actually put it out.
 *
 * Returned Value:
 *   OK on success; -ENODEV if the LCDC is not up, -ENOTSUP if the panel
 *   has no brightness control.
 *
 ****************************************************************************/

int da1470x_lcdc_set_brightness(uint8_t level);

#ifdef __cplusplus
}
#endif

#endif /* __ARCH_ARM_INCLUDE_DA1470X_LCDC_H */
