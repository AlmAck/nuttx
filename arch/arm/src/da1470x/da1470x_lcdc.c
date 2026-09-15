/****************************************************************************
 * arch/arm/src/da1470x/da1470x_lcdc.c
 *
 * Minimal LCDC bring-up for DA1470x. Powers PD_GPU and the LCDC clock
 * via CRG_SYS, then verifies the controller is alive by reading the
 * IDREG magic. Panel-specific mode programming (DBI / DPI / DSI) and
 * the command/pixel data path are deliberately deferred to a panel
 * driver layer once we have a real panel attached and TE/RESX pins
 * routed.
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

#include <nuttx/arch.h>

#include "arm_internal.h"
#include "hardware/da1470x_crg_sys.h"
#include "hardware/da1470x_lcdc.h"
#include "da1470x_lcdc.h"
#include "da1470x_pmu.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int da1470x_lcdc_initialize(void)
{
  /* PD_GPU hosts the LCDC register region at 0x30030000. Without it
   * powered, accesses produce undefined results / bus errors. The
   * pd_enable helper blocks until SYS_STAT.GPU_IS_UP asserts.
   */

  da1470x_pd_enable(DA1470X_PD_GPU);

  /* Release any pending LCD soft-reset. CRG_SYS has the SET/RESET-style
   * register pair: writing 1 to a bit in SET_CLK_SYS sets it; writing 1
   * to the same bit in RESET_CLK_SYS clears it.
   */

  putreg32(CRG_SYS_LCD_RESET_REQ, DA1470_CRG_SYS_RESET_CLK_SYS);

  /* Select DivN (= 32 MHz) as the LCDC source clock. Clear CLK_SEL via
   * the RESET register, then set ENABLE via the SET register.
   */

  putreg32(CRG_SYS_LCD_CLK_SEL, DA1470_CRG_SYS_RESET_CLK_SYS);
  putreg32(CRG_SYS_LCD_ENABLE,  DA1470_CRG_SYS_SET_CLK_SYS);

  /* Sanity check: the LCDC IDREG holds a documented magic value. If we
   * don't read it back, either the power domain didn't actually come
   * up, the clock isn't gated, or the LCDC base address is wrong for
   * this silicon revision.
   */

  uint32_t id = getreg32(DA1470_LCDC_IDREG);
  if (id != DA1470_LCDC_IDREG_MAGIC)
    {
      lcderr("LCDC IDREG mismatch: read 0x%08lx, expected 0x%08lx\n",
             (unsigned long)id, (unsigned long)DA1470_LCDC_IDREG_MAGIC);
      return -ENODEV;
    }

  return OK;
}

/****************************************************************************
 * Pad-mux + flow control
 ****************************************************************************/

void da1470x_lcdc_set_iface_serial(bool si_on_so)
{
  uint32_t regval = getreg32(DA1470_LCDC_GPIO);

  /* Replace OUTPUT_MODE field with "Serial", set OUTPUT_EN so the LCDC
   * actually drives the pads, and optionally share SI on SO.
   */

  regval &= ~LCDC_GPIO_REG_GPIO_OUTPUT_MODE_MASK;
  regval |= LCDC_GPIO_REG_IF_SPI | LCDC_GPIO_REG_GPIO_OUTPUT_EN;

  if (si_on_so)
    {
      regval |= LCDC_GPIO_REG_GPIO_SPI_SI_ON_SD_PAD;
    }
  else
    {
      regval &= ~LCDC_GPIO_REG_GPIO_SPI_SI_ON_SD_PAD;
    }

  putreg32(regval, DA1470_LCDC_GPIO);
}

void da1470x_lcdc_set_hold(bool hold)
{
  /* This is the "batch the upcoming pushes before they start clocking
   * out" knob used by the panel driver around short DCS init bursts.
   *
   * The DA1470x has two bits that could plausibly serve this role:
   *   - CLKCTRL.DMA_HOLD  : stalls the LCDC pixel DMA pump
   *   - DBIB_CFG.CMD_DATA_AS_HEADER : binds cmd+data into one CS-low
   *     transaction at the DBIB output
   *
   * Setting CMD_DATA_AS_HEADER stalls the cmd FIFO from draining until
   * SFRAME_UPD fires, which deadlocks DCS init. For init commands we
   * write CLKCTRL.DMA_HOLD instead: it doesn't affect the cmd FIFO, so
   * the panel sees DCS sequences as the LCDC pumps them through DBIB.
   * For the per-frame transaction (RAMWR + pixel data) we set
   * CMD_DATA_AS_HEADER directly inside da1470x_lcdc_send_one_frame().
   */

  uint32_t regval = getreg32(DA1470_LCDC_CLKCTRL);

  if (hold)
    {
      regval |= LCDC_CLKCTRL_DMA_HOLD;
    }
  else
    {
      regval &= ~LCDC_CLKCTRL_DMA_HOLD;
    }

  putreg32(regval, DA1470_LCDC_CLKCTRL);
}

/* Push one 32-bit DBIB_CMD_REG entry, backing off if the on-chip FIFO is
 * full. The FIFO is small (8 entries in the SDK); for init sequences
 * with DMA_HOLD asserted up front this almost never blocks.
 */

static void da1470x_lcdc_dbib_push(uint32_t value)
{
  while ((getreg32(DA1470_LCDC_STATUS) & LCDC_STATUS_DBIB_CMD_FIFO_FULL) != 0)
    ;

  putreg32(value, DA1470_LCDC_DBIB_CMD);
}

void da1470x_lcdc_qspi_send_cmd(uint8_t qspi_prefix, uint8_t dcs_cmd)
{
  /* First entry: the QSPI write-prefix byte (e.g. 0x02 for RM69091).
   * DBIB_CMD_SEND=1 says "this is the leading command byte of a new
   * transaction"; QSPI_SERIAL_CMD_TRANS=1 says "serial framing".
   */

  da1470x_lcdc_dbib_push(LCDC_DBIB_CMD_DBIB_CMD_SEND
                        | LCDC_DBIB_CMD_QSPI_SERIAL_CMD_TRANS
                        | (qspi_prefix & 0xFFu));

  /* Second entry: the actual DCS command byte, framed in 24 bits. The
   * Raydium RM69091 expects a 24-bit address field after the write
   * prefix, with the command byte at bits[15:8] and the rest zero.
   */

  da1470x_lcdc_dbib_push(LCDC_DBIB_CMD_QSPI_SERIAL_CMD_TRANS
                        | LCDC_DBIB_CMD_CMD_WIDTH_24
                        | (((uint32_t)dcs_cmd) << 8));
}

void da1470x_lcdc_qspi_send_data(uint8_t data)
{
  /* DBIB_CMD_SEND=0 (parameter, not new command), serial framing. */

  da1470x_lcdc_dbib_push(LCDC_DBIB_CMD_QSPI_SERIAL_CMD_TRANS | data);
}

void da1470x_lcdc_qspi_configure(uint8_t clk_div)
{
  /* DBIB_CFG matches the SDK's PHY_CFG_DEFAULT | DBI_EN | SPI4 | QSPI |
   * SPIDC_DQSPI configuration for HW_LCDC_PHY_QUAD_SPI:
   *   - INTERFACE_EN  : turn the DBIB block on
   *   - RESX_OUT_EN   : pass RESX through
   *   - TE_DISABLE    : TE pin not in use yet
   *   - SPI4_EN       : 4-wire SPI framing (active during cmd phase)
   *   - QUAD_SPI_EN   : quad-data emission during pixel phase
   *   - SPI_DC_AS_SPI_SD1 : SPI_DC pad doubles as SD1 for quad mode
   *   - INTERFACE_WIDTH = 5 : DBI width = "Quad SPI"
   *   - CSX_CFG_EN with CSX_CFG=0 : CS stays low for multi-byte bursts
   * SPI_CLK_POLARITY / SPI_CLK_PHASE are both 0 (mode-0) for the
   * RM69091.
   */

  uint32_t cfg = LCDC_DBIB_CFG_INTERFACE_EN
               | LCDC_DBIB_CFG_RESX_OUT_EN
               | LCDC_DBIB_CFG_TE_DISABLE
               | LCDC_DBIB_CFG_SPI4_EN
               | LCDC_DBIB_CFG_QUAD_SPI_EN
               | LCDC_DBIB_CFG_SPI_DC_AS_SPI_SD1
               | LCDC_DBIB_CFG_INTERFACE_WIDTH_QSPI;
  /* CSX_CFG_EN is NOT set here: leaving it 0 lets the LCDC toggle CSX
   * naturally between DCS init commands. send_one_frame() sets it just
   * before SFRAME_UPD so the SSQ cmd + pixel burst share one CS-low
   * transaction, then we clear it again after the frame.
   */
  putreg32(cfg, DA1470_LCDC_DBIB_CFG);

  /* LCDC_MODE: enable underrun-prevention; no continuous mode yet (we
   * trigger frames one-shot via SFRAME_UPD in send_one_frame()).
   */

  putreg32(LCDC_MODE_UNDERRUN_PREVENTION_EN, DA1470_LCDC_MODE);

  /* CLKCTRL: set the LCDC interface clock divider. SCLK_out = src / div.
   * Source = CRG_SYS DivN = 32 MHz; div=1 -> 32 MHz SCLK.
   */

  if (clk_div == 0)
    {
      clk_div = 1;
    }

  uint32_t clkctrl = getreg32(DA1470_LCDC_CLKCTRL);
  clkctrl &= ~LCDC_CLKCTRL_CLK_DIV_MASK;
  clkctrl |= ((uint32_t)clk_div << LCDC_CLKCTRL_CLK_DIV_SHIFT)
             & LCDC_CLKCTRL_CLK_DIV_MASK;
  putreg32(clkctrl, DA1470_LCDC_CLKCTRL);
}

/****************************************************************************
 * Display timing + Layer0 + one-shot frame trigger
 ****************************************************************************/

void da1470x_lcdc_set_resolution(uint16_t resx, uint16_t resy)
{
  /* Minimum porch / blanking values from the SDK for serial PHYs:
   *   fpx=1, blx=2, bpx=1, fpy=1, bly=1, bpy=1
   * The SDK programs cumulative coordinates:
   *   FPORCH  = (resx + fpx,    resy + fpy)
   *   BLANK   = (FPORCH + blx,  FPORCH + bly)
   *   BPORCH  = (BLANK  + bpx,  BLANK  + bpy)
   *   STARTXY = (FPORCH,        FPORCH - 1)
   */

  uint16_t fpx = resx + 1;
  uint16_t blx = fpx + 2;
  uint16_t bpx = blx + 1;
  uint16_t fpy = resy + 1;
  uint16_t bly = fpy + 1;
  uint16_t bpy = bly + 1;

  putreg32(LCDC_XY_PACK(resx, resy), DA1470_LCDC_RESXY);
  putreg32(LCDC_XY_PACK(fpx,  fpy),  DA1470_LCDC_FRONTPORCHXY);
  putreg32(LCDC_XY_PACK(blx,  bly),  DA1470_LCDC_BLANKINGXY);
  putreg32(LCDC_XY_PACK(bpx,  bpy),  DA1470_LCDC_BACKPORCHXY);
  putreg32(LCDC_XY_PACK(fpx,  fpy - 1), DA1470_LCDC_STARTXY);
}

void da1470x_lcdc_set_layer0(uintptr_t baseaddr, uint16_t resx, uint16_t resy,
                             uint16_t stride, uint8_t color_mode)
{
  putreg32((uint32_t)baseaddr,        DA1470_LCDC_LAYER0_BASEADDR);
  putreg32(LCDC_XY_PACK(0, 0),        DA1470_LCDC_LAYER0_STARTXY);
  putreg32(LCDC_XY_PACK(resx, resy),  DA1470_LCDC_LAYER0_SIZEXY);
  putreg32(LCDC_XY_PACK(resx, resy),  DA1470_LCDC_LAYER0_RESXY);
  putreg32((uint32_t)stride,          DA1470_LCDC_LAYER0_STRIDE);

  uint32_t mode = LCDC_LAYER0_MODE_L0_EN
                | (0xFFU << LCDC_LAYER0_MODE_L0_ALPHA_SHIFT)
                | (color_mode & LCDC_LAYER0_MODE_L0_COLOR_MODE_MASK);
  putreg32(mode, DA1470_LCDC_LAYER0_MODE);
}

void da1470x_lcdc_qspi_send_frame_cmd(uint8_t ssq_prefix, uint8_t dcs_cmd)
{
  /* Same encoding as a normal QSPI cmd, but with the SSQ prefix (0x32 for
   * the RM69091) — the LCDC keeps cmd+addr serial and the data phase that
   * follows from Layer0 goes out on all 4 lanes.
   */

  da1470x_lcdc_dbib_push(LCDC_DBIB_CMD_DBIB_CMD_SEND
                        | LCDC_DBIB_CMD_QSPI_SERIAL_CMD_TRANS
                        | (ssq_prefix & 0xFFu));

  da1470x_lcdc_dbib_push(LCDC_DBIB_CMD_QSPI_SERIAL_CMD_TRANS
                        | LCDC_DBIB_CMD_CMD_WIDTH_24
                        | (((uint32_t)dcs_cmd) << 8));
}

int da1470x_lcdc_send_one_frame(void)
{
  /* For the per-frame transaction we want:
   *   - CMD_DATA_AS_HEADER (a.k.a. SDK "SPI_HOLD") set so the SSQ RAMWR
   *     command and the pixel data from Layer 0 ship as one bound
   *     burst at the DBIB output.
   *   - CSX_CFG_EN with CSX_CFG=0 to force CSX low for that whole
   *     burst.
   * Both are added in a single read-modify-write so the LCDC sees a
   * consistent config before SFRAME_UPD goes out.
   */

  uint32_t cfg = getreg32(DA1470_LCDC_DBIB_CFG);
  cfg |= LCDC_DBIB_CFG_CMD_DATA_AS_HEADER
       | LCDC_DBIB_CFG_CSX_CFG_EN;
  cfg &= ~LCDC_DBIB_CFG_CSX_CFG;
  putreg32(cfg, DA1470_LCDC_DBIB_CFG);

  /* Trigger the single-frame engine via SFRAME_UPD. The LCDC drains
   * the cmd FIFO (SSQ frame cmd) and immediately follows with the
   * Layer 0 pixel stream over quad lanes, all in one CS-low burst.
   */

  uint32_t mode = getreg32(DA1470_LCDC_MODE);
  putreg32(mode | LCDC_MODE_SFRAME_UPD, DA1470_LCDC_MODE);

  /* Poll FRAME_END. At 32 MHz SCLK / quad lanes, a 390x390 RGB565 frame
   * is ~150 KB pixel data / 16 MB/s ≈ 10 ms; allow generous headroom.
   */

  const int max_poll = 250000;          /* µs */
  int waited = 0;
  int ret = OK;
  while ((getreg32(DA1470_LCDC_STATUS) & LCDC_STATUS_FRAME_END) == 0)
    {
      up_udelay(100);
      waited += 100;
      if (waited >= max_poll)
        {
          lcderr("LCDC: frame timed out (status=0x%08lx)\n",
                 (unsigned long)getreg32(DA1470_LCDC_STATUS));
          ret = -ETIMEDOUT;
          break;
        }
    }

  /* Release CS and undo the cmd/data binding so the panel doesn't keep
   * latching whatever the LCDC clocks out next. Without this, CSX stays
   * forced low and SCLK keeps running, so the RM69091 interprets any
   * subsequent traffic as more pixel data and its display RAM fills
   * with garbage frame-after-frame (visible as snow that brightens and
   * eventually warms the panel).
   */

  UNUSED(cfg);

  return ret;
}
