/****************************************************************************
 * boards/arm/da1470x/da14706-00hzdevkt-p/src/da1470x_e120a390qsr.h
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to you under the Apache License, Version
 * 2.0 (the "License"); you may not use this file except in compliance
 * with the License.
 *
 ****************************************************************************/

#ifndef __BOARDS_ARM_DA1470X_DA14706_00HZDEVKT_P_SRC_DA1470X_E120A390QSR_H
#define __BOARDS_ARM_DA1470X_DA14706_00HZDEVKT_P_SRC_DA1470X_E120A390QSR_H

#include <nuttx/config.h>
#include <stdint.h>

/* Panel geometry (E120A390QSR, 1.19" 390x390 round AMOLED, RM69091
 * driver IC, QSPI mode strapped on the Renesas daughterboard).
 */

#define E120A390_RESX            390
#define E120A390_RESY            390
#define E120A390_OFFSETX         6
#define E120A390_OFFSETY         0
#define E120A390_BPP             16    /* RGB565 */

/* RM69091 / MIPI DCS commands the driver uses */

#define E120A390_DCS_SLPOUT      0x11  /* Sleep out */
#define E120A390_DCS_DISPON      0x29  /* Display on */
#define E120A390_DCS_CASET       0x2A  /* Column address set */
#define E120A390_DCS_RASET       0x2B  /* Row address set */
#define E120A390_DCS_RAMWR       0x2C  /* Memory write (start pixel stream) */
#define E120A390_DCS_TEAR_OFF    0x34  /* Tearing-effect OFF */
#define E120A390_DCS_COLMOD      0x3A  /* Pixel format (0x55 = RGB565) */

/* QSPI write prefix expected by the RM69091. */

#define E120A390_QSPI_WRITE_PREFIX 0x02

/****************************************************************************
 * Name: da1470x_e120a390_initialize
 *
 * Description:
 *   Bring up the daughterboard panel:
 *     1. Configure GPIO pinmux for SCLK/SD0..SD3/CSX (plain GPIO, no pull),
 *        RST and DCDC_EN as plain outputs.
 *     2. Assert DCDC_EN to enable the on-board boost converter that
 *        generates ELVDD/ELVSS; wait for it to settle.
 *     3. Pulse RST low (50 ms) then high (120 ms) — the RM69091
 *        recommended power-on sequence.
 *     4. Switch the LCDC pad-mux to serial mode and program QSPI cfg.
 *     5. Push the RM69091 init command sequence (page-select, ELVSS,
 *        brightness, CASET/RASET window, COLMOD=RGB565, TEAR_OFF,
 *        SLPOUT, DISPON).
 *
 * Returned Value:
 *   OK on success; negative errno on failure (currently always OK —
 *   error paths get added once we have a panel that can NACK).
 ****************************************************************************/

int da1470x_e120a390_initialize(void);

/****************************************************************************
 * Name: da1470x_e120a390_fill
 *
 * Description:
 *   Write a solid RGB565 color to the entire panel via QSPI/RAMWR.
 *   Polled PIO — pushes ~152k 16-bit pixels through DBIB_CMD_REG. At
 *   32 MHz QSPI this takes around 200 ms; it's deliberately not using
 *   the Layer0 framebuffer path yet, that's a follow-up.
 ****************************************************************************/

void da1470x_e120a390_fill(uint16_t rgb565);

#endif /* __BOARDS_ARM_DA1470X_DA14706_00HZDEVKT_P_SRC_DA1470X_E120A390QSR_H */
