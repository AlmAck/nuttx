/****************************************************************************
 * arch/arm/src/da1470x/da1470x_lcdc.h
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to you under the Apache License, Version
 * 2.0 (the "License"); you may not use this file except in compliance
 * with the License.
 *
 ****************************************************************************/

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_LCDC_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_LCDC_H

#include <nuttx/config.h>
#include <stdbool.h>
#include <stdint.h>

/****************************************************************************
 * Name: da1470x_lcdc_initialize
 *
 * Description:
 *   Bring up the LCDC block:
 *     1. Power up PD_GPU so the LCDC register region at 0x30030000 is
 *        actually accessible (reads return bus errors otherwise).
 *     2. Enable the LCDC clock in CRG_SYS (LCD_ENABLE, LCD_CLK_SEL=DivN).
 *     3. Release the LCD_RESET_REQ if it was asserted.
 *     4. Verify the controller is alive by reading LCDC_IDREG and
 *        comparing against the documented magic 0x87452365.
 *
 *   No panel/PHY-specific config is done here — the caller (panel driver)
 *   is responsible for configuring LCDC_MODE / DBIB_CFG / timings.
 *
 * Returned Value:
 *   OK on success; -ENODEV if the IDREG read-back doesn't match.
 *
 ****************************************************************************/

int da1470x_lcdc_initialize(void);

/****************************************************************************
 * Name: da1470x_lcdc_set_iface_serial
 *
 * Description:
 *   Switch the LCDC pad-mux to serial mode (SPI / Dual-SPI / Quad-SPI) by
 *   writing LCDC_GPIO_REG. This is what actually takes over the hardwired
 *   LCDC pins (P0.14 SCLK, P0.15 SD0, P0.16 SD1, P0.17 SD3, P0.18 CSX,
 *   P0.22 SD2) — the chip-level GPIO function table has no LCD PID, the
 *   LCDC block does its own pad takeover via this register. The GPIO
 *   controller must already be programmed for these pins as plain GPIO
 *   (PID 0) with no pull so it doesn't fight the LCDC.
 *
 *   si_on_so=true mirrors the SDK's "si_on_so" setting for QSPI panels
 *   (E120A390QSR and similar) — shares SI on the SO pad.
 ****************************************************************************/

void da1470x_lcdc_set_iface_serial(bool si_on_so);

/****************************************************************************
 * Name: da1470x_lcdc_set_hold
 *
 * Description:
 *   Stall (true) or release (false) the LCDC's outgoing transmission via
 *   LCDC_CLKCTRL_REG.DMA_HOLD. Used to batch-load the DBIB command FIFO
 *   without the controller starting to clock data out half-way through.
 ****************************************************************************/

void da1470x_lcdc_set_hold(bool hold);

/****************************************************************************
 * Name: da1470x_lcdc_qspi_send_cmd
 *
 * Description:
 *   In QSPI mode, send a single MIPI DCS command byte. The DA1470x LCDC
 *   wraps each DCS byte with the panel-specific QSPI write prefix (e.g.
 *   0x02 for the Raydium RM69091) and a 24-bit command frame, so this
 *   takes two register pushes:
 *     1) prefix byte with QSPI_SERIAL_CMD_TRANS=1 and CMD_SEND=1
 *     2) (dcs_cmd << 8) with QSPI_SERIAL_CMD_TRANS=1 and CMD_WIDTH=24-bit
 *   Each push polls LCDC_STATUS.DBIB_CMD_FIFO_FULL to back off if needed.
 *
 *   Caller is expected to da1470x_lcdc_set_hold(true) before a batch and
 *   da1470x_lcdc_set_hold(false) after the last command/data byte.
 ****************************************************************************/

void da1470x_lcdc_qspi_send_cmd(uint8_t qspi_prefix, uint8_t dcs_cmd);

/****************************************************************************
 * Name: da1470x_lcdc_qspi_send_data
 *
 * Description:
 *   Send a single DCS parameter byte in QSPI mode. Must follow a
 *   matching da1470x_lcdc_qspi_send_cmd() in the same DMA-hold batch.
 ****************************************************************************/

void da1470x_lcdc_qspi_send_data(uint8_t data);

/****************************************************************************
 * Name: da1470x_lcdc_qspi_configure
 *
 * Description:
 *   One-time LCDC configuration for a quad-SPI panel:
 *     - LCDC_DBIB_CFG: QUAD_SPI_EN=1, DBIB_INTERFACE_EN=1, CSX behavior,
 *       SPI clock polarity/phase
 *     - LCDC_CLKCTRL_REG: set CLK_DIV to clk_div (1 = full source clock,
 *       2 = half, etc.). The LCDC source clock on DA1470x DivN is 32 MHz;
 *       div=1 -> 32 MHz SCLK, which is comfortably under the RM69091's
 *       50 MHz max.
 *   The caller is responsible for setting up GPIO pinmux first and for
 *   calling da1470x_lcdc_set_iface_serial() to flip the pad-mux.
 ****************************************************************************/

void da1470x_lcdc_qspi_configure(uint8_t clk_div);

/****************************************************************************
 * Name: da1470x_lcdc_set_resolution
 *
 * Description:
 *   Program the LCDC display-timing registers (RESXY / FRONTPORCHXY /
 *   BLANKINGXY / BACKPORCHXY / STARTXY) for a serial-output panel with
 *   no real timing requirements. Mirrors hw_lcdc_set_lcd_timing() in the
 *   SDK using the documented MIN_* values (fpx=1, blx=2, bpx=1, fpy=1,
 *   bly=1, bpy=1). Active area is full-screen, origin (0,0).
 ****************************************************************************/
void da1470x_lcdc_set_resolution(uint16_t resx, uint16_t resy);

/****************************************************************************
 * Name: da1470x_lcdc_set_layer0
 *
 * Description:
 *   Program Layer 0 (the only layer the panel driver uses) to fetch
 *   pixels from a linear framebuffer:
 *     - LAYER0_BASEADDR = baseaddr (must be 4-byte aligned)
 *     - LAYER0_STARTXY  = (0, 0)
 *     - LAYER0_SIZEXY   = (resx, resy)
 *     - LAYER0_RESXY    = (resx, resy)
 *     - LAYER0_STRIDE   = stride (bytes/line, 4-byte aligned)
 *     - LAYER0_MODE     = L0_EN | alpha=0xFF | color_mode
 *
 *   color_mode is one of LCDC_OCM_8RGB565 / LCDC_OCM_8RGB888.
 ****************************************************************************/
void da1470x_lcdc_set_layer0(uintptr_t baseaddr, uint16_t resx, uint16_t resy,
                             uint16_t stride, uint8_t color_mode);

/****************************************************************************
 * Name: da1470x_lcdc_qspi_send_frame_cmd
 *
 * Description:
 *   Send the QSPI write-memory framing command (SSQ prefix + DCS cmd) that
 *   tells the LCDC the next data phase will be pixel data streamed from
 *   Layer 0 over quad lanes. For the RM69091 the SSQ prefix is 0x32 and
 *   the DCS command is HW_LCDC_MIPI_DCS_WRITE_MEMORY_START (0x2C).
 *
 *   Caller is expected to da1470x_lcdc_set_hold(true) before, and to
 *   trigger da1470x_lcdc_send_one_frame() afterwards.
 ****************************************************************************/
void da1470x_lcdc_qspi_send_frame_cmd(uint8_t ssq_prefix, uint8_t dcs_cmd);

/****************************************************************************
 * Name: da1470x_lcdc_send_one_frame
 *
 * Description:
 *   Trigger a single LCDC frame: release DMA_HOLD, force CSX low so the
 *   SSQ frame command and pixel data stay in one CSn transaction, then
 *   set LCDC_MODE.SFRAME_UPD which kicks off the DMA fetch from Layer 0
 *   and the QSPI quad-data emission. Polls LCDC_STATUS.FRAME_END for up
 *   to ~250 ms and returns OK on completion, -ETIMEDOUT otherwise.
 ****************************************************************************/
int da1470x_lcdc_send_one_frame(void);

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_LCDC_H */
