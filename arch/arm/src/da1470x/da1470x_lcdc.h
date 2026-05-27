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

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_LCDC_H */
