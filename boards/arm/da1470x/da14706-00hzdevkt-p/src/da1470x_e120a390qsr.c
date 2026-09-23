/****************************************************************************
 * boards/arm/da1470x/da14706-00hzdevkt-p/src/da1470x_e120a390qsr.c
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

/* EverDisplay E120A390QSR: 1.2" 390x390 round AMOLED with a Raydium
 * RM69091 controller, attached to the LCDC in quad-SPI mode on the
 * da1470x-sb-E120A390QSR daughterboard.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <stdbool.h>
#include <debug.h>
#include <errno.h>

#include <nuttx/arch.h>
#include <arch/board/board.h>

#include "arm_internal.h"
#include "hardware/da1470x_gpio.h"
#include "da1470x_gpio.h"
#include "da1470x_lcdc.h"
#include "da14706-00hzdevkt-p.h"

#ifdef CONFIG_DA14706_LCD_E120A390QSR

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define E120A390_RESX            390
#define E120A390_RESY            390
#define E120A390_OFFSETX         6      /* Column offset of the visible area */
#define E120A390_OFFSETY         0

/* DCS commands */

#define DCS_SLPOUT               0x11
#define DCS_DISPON               0x29
#define DCS_CASET                0x2a
#define DCS_RASET                0x2b
#define DCS_RAMWR                0x2c
#define DCS_TEOFF                0x34
#define DCS_TEON                 0x35
#define DCS_COLMOD               0x3a
#define DCS_WRCTRLD              0x53
#define DCS_WRDISBV              0x51

/* QSPI framing prefixes of the RM69091 */

#define RM69091_CMD_PREFIX       0x02   /* Single command write */
#define RM69091_FRAME_PREFIX     0x32   /* Quad pixel write */

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int  e120a390_init(const struct da1470x_lcdc_panel_s *panel);
static int  e120a390_window(const struct da1470x_lcdc_panel_s *panel,
                            uint16_t x0, uint16_t y0, uint16_t x1,
                            uint16_t y1);
static void e120a390_brightness(const struct da1470x_lcdc_panel_s *panel,
                                uint8_t level);
static void e120a390_power(const struct da1470x_lcdc_panel_s *panel,
                           bool on);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct da1470x_lcdc_panel_s g_e120a390_panel =
{
  .xres         = E120A390_RESX,
  .yres         = E120A390_RESY,
  .bpp          = 16,
  .sclk_max     = 48000000,     /* Panel limit 50 MHz */
  .cmd_prefix   = RM69091_CMD_PREFIX,
  .frame_prefix = RM69091_FRAME_PREFIX,
  .ramwr        = DCS_RAMWR,
#ifdef CONFIG_DA1470X_LCDC_TE
  .te           = true,
#else
  .te           = false,
#endif
  .init         = e120a390_init,
  .window       = e120a390_window,
  .power        = e120a390_power,
  .brightness   = e120a390_brightness,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: e120a390_cmd / e120a390_cmd1
 *
 * Description:
 *   Send a DCS command without or with one parameter as a single burst.
 *
 ****************************************************************************/

static void e120a390_cmd(uint8_t cmd)
{
  da1470x_lcdc_dcs_hold(true);
  da1470x_lcdc_dcs_cmd(cmd);
  da1470x_lcdc_dcs_hold(false);
}

static void e120a390_cmd1(uint8_t cmd, uint8_t param)
{
  da1470x_lcdc_dcs_hold(true);
  da1470x_lcdc_dcs_cmd(cmd);
  da1470x_lcdc_dcs_data(param);
  da1470x_lcdc_dcs_hold(false);
}

/****************************************************************************
 * Name: e120a390_brightness
 *
 * Description:
 *   Panel brightness, 0-255, via DCS WRDISBV. Called by
 *   da1470x_lcdc_set_brightness() with the driver lock held and the frame
 *   engine idle, so it may issue DCS traffic directly.
 *
 *   The mapping from this byte to luminance goes through the RM69091's
 *   DBV gamma table and is very unlikely to be perceptually linear, so a
 *   linear host ramp may not look linear. That is a tuning question for
 *   whoever is watching the panel.
 *
 ****************************************************************************/

static void e120a390_brightness(const struct da1470x_lcdc_panel_s *panel,
                                uint8_t level)
{
  UNUSED(panel);

  e120a390_cmd1(DCS_WRDISBV, level);
}

/****************************************************************************
 * Name: e120a390_window
 *
 * Description:
 *   Set the column and row address window (panel coordinates).
 *
 ****************************************************************************/

static int e120a390_window(const struct da1470x_lcdc_panel_s *panel,
                           uint16_t x0, uint16_t y0, uint16_t x1,
                           uint16_t y1)
{
  x0 += E120A390_OFFSETX;
  x1 += E120A390_OFFSETX;
  y0 += E120A390_OFFSETY;
  y1 += E120A390_OFFSETY;

  /* One chip-select cycle per command: the panel takes only the first
   * command of a burst.
   */

  da1470x_lcdc_dcs_hold(true);
  da1470x_lcdc_dcs_cmd(DCS_CASET);
  da1470x_lcdc_dcs_data(x0 >> 8);
  da1470x_lcdc_dcs_data(x0 & 0xff);
  da1470x_lcdc_dcs_data(x1 >> 8);
  da1470x_lcdc_dcs_data(x1 & 0xff);
  da1470x_lcdc_dcs_hold(false);

  da1470x_lcdc_dcs_hold(true);
  da1470x_lcdc_dcs_cmd(DCS_RASET);
  da1470x_lcdc_dcs_data(y0 >> 8);
  da1470x_lcdc_dcs_data(y0 & 0xff);
  da1470x_lcdc_dcs_data(y1 >> 8);
  da1470x_lcdc_dcs_data(y1 & 0xff);
  da1470x_lcdc_dcs_hold(false);
  return OK;
}

/****************************************************************************
 * Name: e120a390_power
 *
 * Description:
 *   Drive the panel DC/DC enable and reset lines.
 *
 ****************************************************************************/

static void e120a390_power(const struct da1470x_lcdc_panel_s *panel,
                           bool on)
{
  if (on)
    {
      da1470x_gpio_write(BOARD_LCDC_DCDC_PIN, true);
      up_mdelay(5);
      da1470x_gpio_write(BOARD_LCDC_RST_PIN, false);
      up_mdelay(50);
      da1470x_gpio_write(BOARD_LCDC_RST_PIN, true);
      up_mdelay(120);
    }
  else
    {
      da1470x_gpio_write(BOARD_LCDC_RST_PIN, false);
      da1470x_gpio_write(BOARD_LCDC_DCDC_PIN, false);
    }
}

/****************************************************************************
 * Name: e120a390_init
 *
 * Description:
 *   RM69091 start-up sequence (from the panel vendor's reference):
 *   manufacturer page settings, SPI-to-RAM write path, brightness, then
 *   the standard sleep-out and display-on.
 *
 ****************************************************************************/

static int e120a390_init(const struct da1470x_lcdc_panel_s *panel)
{
  e120a390_cmd1(0xfe, 0x01);            /* Manufacturer command page 1 */
  e120a390_cmd1(0x04, 0xa0);
  e120a390_cmd1(0xfe, 0x01);
  e120a390_cmd1(0x6a, 0x00);            /* ELVSS -2.4 V */
  e120a390_cmd1(0xab, 0x00);            /* HBM ELVSS -2.4 V */
  e120a390_cmd1(0xfe, 0x00);            /* User command page */
  e120a390_cmd1(0xc4, 0x80);            /* SPI write to RAM */
  /* WRCTRLD 0x20 = BCTRL, the brightness control block. NOT dimming:
   * the DCS smooth-transition bit is DD (0x08) and is not set, so the
   * panel does not interpolate between levels and the host's own ramp
   * is what produces the fade. (Read from the generic
   * DCS spec, not the RM69091 datasheet -- worth confirming against the
   * part if a fade ever looks stepped.)
   */

  e120a390_cmd1(DCS_WRCTRLD, 0x20);
  e120a390_cmd1(DCS_WRDISBV, 0xff);     /* Maximum brightness */

  e120a390_window(panel, 0, 0, E120A390_RESX - 1, E120A390_RESY - 1);

  e120a390_cmd1(DCS_COLMOD, 0x55);      /* RGB565 */

  if (panel->te)
    {
      e120a390_cmd1(DCS_TEON, 0x00);
    }
  else
    {
      e120a390_cmd(DCS_TEOFF);
    }

  e120a390_cmd(DCS_SLPOUT);
  up_mdelay(120);
  e120a390_cmd(DCS_DISPON);
  up_mdelay(50);

  return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_e120a390_initialize
 ****************************************************************************/

int da1470x_e120a390_initialize(void)
{
  /* The LCDC signals have fixed pads: each is handed to the controller
   * through LCDC_MAP_CTRL while the pad itself stays a GPIO output.
   * RST and DCDC_EN stay under GPIO control.
   */

  modifyreg32(DA1470X_GPIO_LCDC_MAP_CTRL, 0,
              GPIO_LCDC_MAP_CTRL_MAP_ON_P0_14_EN |
              GPIO_LCDC_MAP_CTRL_MAP_ON_P0_15_EN |
              GPIO_LCDC_MAP_CTRL_MAP_ON_P0_16_EN |
              GPIO_LCDC_MAP_CTRL_MAP_ON_P0_17_EN |
              GPIO_LCDC_MAP_CTRL_MAP_ON_P0_18_EN |
              GPIO_LCDC_MAP_CTRL_MAP_ON_P0_22_EN
#ifdef CONFIG_DA1470X_LCDC_TE
              | GPIO_LCDC_MAP_CTRL_MAP_ON_P0_10_EN
#endif
              );

  da1470x_gpio_config(BOARD_LCDC_SCLK_PIN);
  da1470x_gpio_config(BOARD_LCDC_SD0_PIN);
  da1470x_gpio_config(BOARD_LCDC_SD1_PIN);
  da1470x_gpio_config(BOARD_LCDC_SD2_PIN);
  da1470x_gpio_config(BOARD_LCDC_SD3_PIN);
  da1470x_gpio_config(BOARD_LCDC_CSX_PIN);
#ifdef CONFIG_DA1470X_LCDC_TE
  da1470x_gpio_config(BOARD_LCDC_TE_PIN);
#endif
  da1470x_gpio_config(BOARD_LCDC_RST_PIN);
  da1470x_gpio_config(BOARD_LCDC_DCDC_PIN);
  da1470x_gpio_config(BOARD_LCDC_IM0_PIN);
  da1470x_gpio_config(BOARD_LCDC_IM1_PIN);

  return da1470x_lcdc_register(&g_e120a390_panel);
}

#endif /* CONFIG_DA14706_LCD_E120A390QSR */
