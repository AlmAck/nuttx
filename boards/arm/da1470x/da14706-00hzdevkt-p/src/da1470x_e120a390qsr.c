/****************************************************************************
 * boards/arm/da1470x/da14706-00hzdevkt-p/src/da1470x_e120a390qsr.c
 *
 * Panel driver for the EverDisplay E120A390QSR (Raydium RM69091 driver
 * IC, 1.19" 390x390 round AMOLED, RGB565) mounted on the Renesas
 * "da1470x-sb-E120A390QSR" QSPI daughterboard.
 *
 * Pinmux and the RM69091 init sequence are transcribed from the
 * Renesas demo (da1470x_demo_vscode/gdi/inc/e120a390qsr.h +
 * config/peripheral_setup.h) — see project memory for the schematic
 * cross-reference.
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to you under the Apache License, Version
 * 2.0 (the "License"); you may not use this file except in compliance
 * with the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <debug.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>

#include <nuttx/arch.h>

#include <arch/board/board.h>

#include "da1470x_gpio.h"
#include "da1470x_lcdc.h"
#include "hardware/da1470x_lcdc.h"
#include "da1470x_e120a390qsr.h"

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* Framebuffer for Layer 0. 390x390 RGB565 = 304200 B. Aligned to 4 bytes
 * because LAYER0_BASEADDR / LAYER0_STRIDE must be word-aligned. Lives in
 * the retained-RAM BSS section (1 MB available); the LCDC DMA reads it
 * directly from SRAM each frame.
 */

static uint16_t g_e120a390_fb[E120A390_RESX * E120A390_RESY]
        aligned_data(4);

/****************************************************************************
 * Private helpers
 ****************************************************************************/

/* Compact helpers for one-shot batched command sequences. The RM69091
 * doesn't really need DMA-hold for short single-cmd transfers but it
 * mirrors the SDK pattern and keeps batches deterministic.
 */

static inline void e120a390_send_cmd(uint8_t cmd)
{
  da1470x_lcdc_qspi_send_cmd(E120A390_QSPI_WRITE_PREFIX, cmd);
}

static inline void e120a390_send_data(uint8_t data)
{
  da1470x_lcdc_qspi_send_data(data);
}

static void e120a390_send_cmd_p1(uint8_t cmd, uint8_t p0)
{
  da1470x_lcdc_set_hold(true);
  e120a390_send_cmd(cmd);
  e120a390_send_data(p0);
  da1470x_lcdc_set_hold(false);
}

static void e120a390_set_window(uint16_t x0, uint16_t y0,
                                uint16_t x1, uint16_t y1)
{
  da1470x_lcdc_set_hold(true);
  e120a390_send_cmd(E120A390_DCS_CASET);
  e120a390_send_data((x0 >> 8) & 0xFF);
  e120a390_send_data( x0       & 0xFF);
  e120a390_send_data((x1 >> 8) & 0xFF);
  e120a390_send_data( x1       & 0xFF);
  e120a390_send_cmd(E120A390_DCS_RASET);
  e120a390_send_data((y0 >> 8) & 0xFF);
  e120a390_send_data( y0       & 0xFF);
  e120a390_send_data((y1 >> 8) & 0xFF);
  e120a390_send_data( y1       & 0xFF);
  da1470x_lcdc_set_hold(false);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int da1470x_e120a390_initialize(void)
{
  /* 1. Pinmux. The six LCDC signal pins (SCLK / SD0 / SD1 / SD2 / SD3 /
   * CSX) get configured as plain GPIO outputs with no pull — the LCDC
   * peripheral itself will steal them once we flip its pad-mux below.
   * RST and DCDC_EN are pure GPIO outputs driven by this function.
   */

  da1470x_gpio_config(BOARD_LCDC_SCLK_PIN);
  da1470x_gpio_config(BOARD_LCDC_SD0_PIN);
  da1470x_gpio_config(BOARD_LCDC_SD1_PIN);
  da1470x_gpio_config(BOARD_LCDC_SD2_PIN);
  da1470x_gpio_config(BOARD_LCDC_SD3_PIN);
  da1470x_gpio_config(BOARD_LCDC_CSX_PIN);

  da1470x_gpio_config(BOARD_LCDC_RST_PIN);
  da1470x_gpio_config(BOARD_LCDC_DCDC_PIN);

  /* 2. Enable the on-board boost: NCP333FCT2G load switch on the VCI
   * rail, gated by P1.7 (DCDC_EN). Spec says no min hold-on time, but
   * give the LT3463 ~5 ms to bring up ELVDD/ELVSS.
   */

  da1470x_gpio_write(BOARD_LCDC_DCDC_PIN, true);
  up_mdelay(5);

  /* 3. RM69091 power-on reset sequence: hold RST low for >=50 us, then
   * wait >=120 ms after release. The Renesas demo uses 50 ms / 120 ms.
   */

  da1470x_gpio_write(BOARD_LCDC_RST_PIN, false);
  up_mdelay(50);
  da1470x_gpio_write(BOARD_LCDC_RST_PIN, true);
  up_mdelay(120);

  /* 4. LCDC takes over the pads (Serial mode, SI-on-SO shared) and we
   * program the DBIB_CFG for quad-SPI. 32 MHz DivN / 1 = 32 MHz SCLK,
   * within the RM69091's 50 MHz write limit.
   */

  da1470x_lcdc_qspi_configure(1);
  da1470x_lcdc_set_iface_serial(true);

  /* 5. RM69091 init sequence (transcribed from the demo's
   * screen_init_cmds[] in gdi/inc/e120a390qsr.h). The 0xFE writes
   * select the manufacturer command page; 0x6A/0xAB set ELVSS;
   * 0xC4 enables SPI->RAM writes; 0x53/0x51 set dimming + brightness.
   */

  e120a390_send_cmd_p1(0xFE, 0x01);                       /* CMD2 page 1 */
  e120a390_send_cmd_p1(0x04, 0xA0);
  e120a390_send_cmd_p1(0xFE, 0x01);
  e120a390_send_cmd_p1(0x6A, 0x00);                       /* ELVSS = -2.4V */
  e120a390_send_cmd_p1(0xAB, 0x00);                       /* HBM ELVSS = -2.4V */
  e120a390_send_cmd_p1(0xFE, 0x00);                       /* user CMD page */
  e120a390_send_cmd_p1(0xC4, 0x80);                       /* SPI write to RAM */
  e120a390_send_cmd_p1(0x53, 0x20);                       /* DCS Write Ctrl Display: dimming */
  e120a390_send_cmd_p1(0x51, 0xFF);                       /* DCS Set Brightness max */

  /* Window: [OFFSETX .. OFFSETX+RESX-1] x [OFFSETY .. OFFSETY+RESY-1] */

  e120a390_set_window(E120A390_OFFSETX,
                      E120A390_OFFSETY,
                      E120A390_OFFSETX + E120A390_RESX - 1,
                      E120A390_OFFSETY + E120A390_RESY - 1);

  /* Pixel format RGB565 (0x55) */

  e120a390_send_cmd_p1(E120A390_DCS_COLMOD, 0x55);

  /* Tear off — we aren't using the TE line in this pass */

  da1470x_lcdc_set_hold(true);
  e120a390_send_cmd(E120A390_DCS_TEAR_OFF);
  da1470x_lcdc_set_hold(false);

  /* Sleep out + 120 ms */

  da1470x_lcdc_set_hold(true);
  e120a390_send_cmd(E120A390_DCS_SLPOUT);
  da1470x_lcdc_set_hold(false);
  up_mdelay(120);

  /* Display on + 50 ms */

  da1470x_lcdc_set_hold(true);
  e120a390_send_cmd(E120A390_DCS_DISPON);
  da1470x_lcdc_set_hold(false);
  up_mdelay(50);

  return OK;
}

void da1470x_e120a390_fill(uint16_t rgb565)
{
  const size_t n = (size_t)E120A390_RESX * E120A390_RESY;

  for (size_t i = 0; i < n; i++)
    {
      g_e120a390_fb[i] = rgb565;
    }

  /* Reset the RM69091 window to the full panel before kicking off the
   * frame transfer. */

  e120a390_set_window(E120A390_OFFSETX,
                      E120A390_OFFSETY,
                      E120A390_OFFSETX + E120A390_RESX - 1,
                      E120A390_OFFSETY + E120A390_RESY - 1);

  /* Program the LCDC display timing and Layer 0 to fetch from the
   * framebuffer. Stride = RESX * 2 bytes/pixel (already 4-byte aligned
   * because RESX=390 is even).
   */

  da1470x_lcdc_set_resolution(E120A390_RESX, E120A390_RESY);
  da1470x_lcdc_set_layer0((uintptr_t)g_e120a390_fb,
                          E120A390_RESX, E120A390_RESY,
                          E120A390_RESX * 2,
                          LCDC_OCM_8RGB565);

  /* Send the SSQ-prefixed RAMWR (frame command). Under DMA_HOLD so the
   * cmd lands first; send_one_frame() releases hold, forces CSX low for
   * the whole frame, and triggers SFRAME_UPD.
   */

  da1470x_lcdc_set_hold(true);
  da1470x_lcdc_qspi_send_frame_cmd(0x32, E120A390_DCS_RAMWR);

  (void)da1470x_lcdc_send_one_frame();
}
