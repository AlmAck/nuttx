/****************************************************************************
 * arch/arm/src/da1470x/da1470x_lcdc.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_LCDC_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_LCDC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdbool.h>
#include <stdint.h>

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* Description of a panel attached to the LCDC in quad-SPI DBI mode.  The
 * board provides one of these to da1470x_lcdc_register(); the init
 * callback is invoked after the controller is clocked and configured so it
 * can send the panel's DCS start-up sequence with the helpers below.
 */

struct da1470x_lcdc_panel_s
{
  uint16_t xres;          /* Visible columns */
  uint16_t yres;          /* Visible rows */
  uint8_t  bpp;           /* Bits per pixel: 16 (RGB565) or 32 (RGBA8888) */
  uint32_t sclk_max;      /* Highest serial clock the panel accepts (Hz) */
  uint8_t  cmd_prefix;    /* QSPI write-command prefix byte (e.g. 0x02) */
  uint8_t  frame_prefix;  /* QSPI prefix for the pixel stream (e.g. 0x32) */
  uint8_t  ramwr;         /* DCS memory write command (0x2c) */
  bool     te;            /* Panel drives a tearing-effect line */

  int  (*init)(const struct da1470x_lcdc_panel_s *panel);
  int  (*window)(const struct da1470x_lcdc_panel_s *panel, uint16_t x0,
                 uint16_t y0, uint16_t x1, uint16_t y1);
  void (*power)(const struct da1470x_lcdc_panel_s *panel, bool on);
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Name: da1470x_lcdc_register
 *
 * Description:
 *   Bring up the LCD controller for the given panel, allocate the frame
 *   buffer and make display 0 available to fb_register().
 *
 ****************************************************************************/

int da1470x_lcdc_register(const struct da1470x_lcdc_panel_s *panel);

/****************************************************************************
 * Name: da1470x_lcdc_dcs_cmd / da1470x_lcdc_dcs_data
 *
 * Description:
 *   Queue a DCS command (with the panel's write prefix) or a parameter
 *   byte into the DBI command FIFO.  For use by panel init sequences.
 *
 ****************************************************************************/

void da1470x_lcdc_dcs_cmd(uint8_t cmd);
void da1470x_lcdc_dcs_data(uint8_t data);

/****************************************************************************
 * Name: da1470x_lcdc_dcs_hold
 *
 * Description:
 *   Hold (true) or release (false) the command FIFO so that a command and
 *   its parameters are clocked out as one burst.
 *
 ****************************************************************************/

void da1470x_lcdc_dcs_hold(bool hold);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_LCDC_H */
