/****************************************************************************
 * boards/arm/da1470x/da14706-00hzdevkt-p/src/da1470x_lpm012m134b.c
 *
 * SPDX-License-Identifier: Apache-2.0
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

/* JDI LPM012M134B: a 240x240 memory-in-pixel panel on the JDI parallel
 * interface, eight colours (two bits per component), refreshed by whole
 * frames and kept alive by the VCOM/FRP square wave.  This is the panel
 * the vendor SDK ships a configuration for; the devkit itself does not
 * carry it, so the pads below follow the chip's fixed JDI parallel
 * assignment and only the panel enable pin is a board choice.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <stdbool.h>

#include <nuttx/arch.h>
#include <arch/board/board.h>

#include "arm_internal.h"
#include "hardware/da1470x_gpio.h"
#include "da1470x_gpio.h"
#include "da1470x_lcdc.h"
#include "da14706-00hzdevkt-p.h"

#ifdef CONFIG_DA14706_LCD_LPM012M134B

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define LPM012M134B_RESX  240
#define LPM012M134B_RESY  240

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static void lpm012m134b_power(const struct da1470x_lcdc_panel_s *panel,
                              bool on);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct da1470x_lcdc_panel_s g_lpm012m134b_panel =
{
  .xres        = LPM012M134B_RESX,
  .yres        = LPM012M134B_RESY,
  .bpp         = 8,
  .iface       = DA1470X_LCDC_IF_JDI_PARALLEL,
  .sclk_max    = 1600000,             /* HCK, panel limit 2 MHz */
  .jdi         =
  {
    .fpx = 1,
    .blx = 2,
    .bpx = 1,
    .fpy = 6,
    .bly = 2,
    .bpy = 0,
  },
  .ext_clk_dhz = 625,                 /* VCOM/FRP at 62.5 Hz */
  .te          = false,
  .power       = lpm012m134b_power,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: lpm012m134b_power
 *
 * Description:
 *   The panel has one enable input; its supplies are on the board.
 *
 ****************************************************************************/

static void lpm012m134b_power(const struct da1470x_lcdc_panel_s *panel,
                              bool on)
{
  da1470x_gpio_write(BOARD_JDI_PEN_PIN, on);
  up_mdelay(on ? 10 : 1);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_lpm012m134b_initialize
 *
 * Description:
 *   Hand the JDI parallel pads to the LCDC and register the panel with
 *   the framebuffer driver.  The frame buffer is RGB332; the controller
 *   keeps the top two bits of each component.
 *
 ****************************************************************************/

int da1470x_lpm012m134b_initialize(void)
{
  modifyreg32(DA1470X_GPIO_LCDC_MAP_CTRL, 0,
              GPIO_LCDC_MAP_CTRL_MAP_ON_P0_09_EN |   /* VCK */
              GPIO_LCDC_MAP_CTRL_MAP_ON_P0_14_EN |   /* HCK */
              GPIO_LCDC_MAP_CTRL_MAP_ON_P0_15_EN |   /* HST */
              GPIO_LCDC_MAP_CTRL_MAP_ON_P0_16_EN |   /* VST */
              GPIO_LCDC_MAP_CTRL_MAP_ON_P0_17_EN |   /* RED0 */
              GPIO_LCDC_MAP_CTRL_MAP_ON_P0_18_EN |   /* ENB */
              GPIO_LCDC_MAP_CTRL_MAP_ON_P0_19_EN |   /* VCOM/FRP */
              GPIO_LCDC_MAP_CTRL_MAP_ON_P0_21_EN |   /* BLUE1 */
              GPIO_LCDC_MAP_CTRL_MAP_ON_P0_22_EN |   /* XRST */
              GPIO_LCDC_MAP_CTRL_MAP_ON_P0_23_EN |   /* RED1 */
              GPIO_LCDC_MAP_CTRL_MAP_ON_P0_24_EN |   /* GREEN0 */
              GPIO_LCDC_MAP_CTRL_MAP_ON_P1_00_EN |   /* GREEN1 */
              GPIO_LCDC_MAP_CTRL_MAP_ON_P1_01_EN);   /* BLUE0 */

  da1470x_gpio_config(BOARD_JDI_VCK_PIN);
  da1470x_gpio_config(BOARD_JDI_HCK_PIN);
  da1470x_gpio_config(BOARD_JDI_HST_PIN);
  da1470x_gpio_config(BOARD_JDI_VST_PIN);
  da1470x_gpio_config(BOARD_JDI_RED0_PIN);
  da1470x_gpio_config(BOARD_JDI_ENB_PIN);
  da1470x_gpio_config(BOARD_JDI_VCOM_PIN);
  da1470x_gpio_config(BOARD_JDI_BLUE1_PIN);
  da1470x_gpio_config(BOARD_JDI_XRST_PIN);
  da1470x_gpio_config(BOARD_JDI_RED1_PIN);
  da1470x_gpio_config(BOARD_JDI_GREEN0_PIN);
  da1470x_gpio_config(BOARD_JDI_GREEN1_PIN);
  da1470x_gpio_config(BOARD_JDI_BLUE0_PIN);
  da1470x_gpio_config(BOARD_JDI_PEN_PIN);

  return da1470x_lcdc_register(&g_lpm012m134b_panel);
}

#endif /* CONFIG_DA14706_LCD_LPM012M134B */
