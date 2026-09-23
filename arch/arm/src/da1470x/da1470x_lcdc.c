/****************************************************************************
 * arch/arm/src/da1470x/da1470x_lcdc.c
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

/* Framebuffer driver for the DA1470x display controller driving a panel
 * over the DBI block in quad-SPI mode.  The LCDC fetches Layer 0 from the
 * frame buffer by DMA.  Each update sets the panel window to the dirty
 * rectangle, points the layer at that rectangle inside the frame buffer,
 * sends the RAMWR command bound to the pixel stream as one chip-select
 * burst and waits for FRAME_END.  The frame buffer holds one or two
 * screens (CONFIG_DA1470X_LCDC_NBUFFERS); a client selects the second
 * one by updating rows at yres..2*yres-1 and panning to it.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <debug.h>

#include <nuttx/arch.h>
#include <nuttx/irq.h>
#include <nuttx/kmalloc.h>
#include <nuttx/kthread.h>
#include <nuttx/power/pm.h>
#include <nuttx/mutex.h>
#include <nuttx/semaphore.h>
#include <nuttx/wqueue.h>
#include <nuttx/video/fb.h>

#include "arm_internal.h"
#include "hardware/da1470x_crg_sys.h"
#include "hardware/da1470x_crg_top.h"
#include "hardware/da1470x_lcdc.h"
#include "da1470x_lcdc.h"
#include "da1470x_pmu.h"
#include "da1470x_clockconfig.h"

#ifdef CONFIG_DA1470X_LCDC

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define LCDC_FIFO_WAIT_LOOPS    100000
#define LCDC_FRAME_TIMEOUT_MS   500
#define LCDC_TE_TIMEOUT_MS      50

#ifndef CONFIG_DA1470X_LCDC_NBUFFERS
#  define CONFIG_DA1470X_LCDC_NBUFFERS 1
#endif

#define LCDC_NBUFFERS           CONFIG_DA1470X_LCDC_NBUFFERS

#ifdef CONFIG_DA1470X_LCDC_ASYNC
#  define LCDC_THREAD_PRIORITY  CONFIG_DA1470X_LCDC_ASYNC_PRIORITY
#  define LCDC_THREAD_STACKSIZE 2048
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct da1470x_lcdc_s
{
  struct fb_vtable_s vtable;                  /* Framebuffer interface */
  const struct da1470x_lcdc_panel_s *panel;   /* Attached panel */
  uint8_t *fbmem;                             /* Frame buffers */
  size_t   fblen;                             /* Size of all buffers */
  size_t   buflen;                            /* Size of one buffer */
  uint16_t stride;                            /* Bytes per line */
  mutex_t  lock;                              /* Serialises updates */
  sem_t    frame;                             /* FRAME_END / TE wake-up */
  struct work_s panwork;                      /* Releases pan requests */
  struct fb_area_s pending;                   /* Last area sent */
  int      lastbuf;                           /* Buffer it was sent from */
  int      shown;                             /* Buffer the panel shows */
#ifdef CONFIG_DA1470X_LCDC_ASYNC
  sem_t    request;                           /* Wakes the transfer thread */
  sem_t    idle;                              /* No transfer in flight */
  struct fb_area_s req;                       /* Queued transfer: area */
  int      reqbuf;                            /* Queued transfer: buffer */
#endif
  bool     initialized;
  bool     poweron;                            /* Panel is lit and holding
                                               * the system at PM_NORMAL */
  uint8_t  brightness;                        /* Last level asked for.  Kept
                                               * across power transitions:
                                               * panel->init resets the panel
                                               * to full, so setpower() puts
                                               * this back afterwards and a
                                               * level set while the panel is
                                               * dark is not lost. */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int lcdc_getvideoinfo(struct fb_vtable_s *vtable,
                             struct fb_videoinfo_s *vinfo);
static int lcdc_getplaneinfo(struct fb_vtable_s *vtable, int planeno,
                             struct fb_planeinfo_s *pinfo);
static int lcdc_updatearea(struct fb_vtable_s *vtable,
                           const struct fb_area_s *area);
static int lcdc_pandisplay(struct fb_vtable_s *vtable,
                           struct fb_planeinfo_s *pinfo);
static int lcdc_getpower(struct fb_vtable_s *vtable);
static int lcdc_setpower(struct fb_vtable_s *vtable, int power);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct da1470x_lcdc_s g_lcdc =
{
  .vtable =
  {
    .getvideoinfo = lcdc_getvideoinfo,
    .getplaneinfo = lcdc_getplaneinfo,
    .updatearea   = lcdc_updatearea,
    .pandisplay   = lcdc_pandisplay,
    .getpower     = lcdc_getpower,
    .setpower     = lcdc_setpower,
  },
  .lock  = NXMUTEX_INITIALIZER,
  .frame = SEM_INITIALIZER(0),

  /* Full brightness until someone asks for less: the panel's own reset
   * state, so the first power-on reapplies what init already did.
   */

  .brightness = 0xff,
#ifdef CONFIG_DA1470X_LCDC_ASYNC
  .request = SEM_INITIALIZER(0),
  .idle    = SEM_INITIALIZER(1),
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: lcdc_fifo_push
 *
 * Description:
 *   Push one entry into the DBI command FIFO, waiting for room.
 *
 ****************************************************************************/

static int lcdc_fifo_push(uint32_t value)
{
  int i;

  for (i = 0; i < LCDC_FIFO_WAIT_LOOPS; i++)
    {
      if ((getreg32(DA1470X_LCDC_STATUS) &
           LCDC_STATUS_DBIB_CMD_FIFO_FULL) == 0)
        {
          putreg32(value, DA1470X_LCDC_DBIB_CMD);
          return OK;
        }
    }

  lcderr("DBI command FIFO stuck full\n");
  return -ETIMEDOUT;
}

/****************************************************************************
 * Name: lcdc_clock_select
 *
 * Description:
 *   Feed the controller from whichever of the fixed 32 MHz peripheral
 *   clock and the system clock gives the highest serial clock that stays
 *   within the panel's limit, and program the divider accordingly.
 *
 ****************************************************************************/

static void lcdc_clock_select(const struct da1470x_lcdc_panel_s *panel)
{
  uint32_t limit = panel->sclk_max ? panel->sclk_max : 32000000;
  uint32_t divn  = da1470x_get_divn_clk();
  uint32_t sys   = da1470x_get_sysclk();
  uint32_t divn_div;
  uint32_t sys_div;
  uint32_t div;
  bool use_sys;

  divn_div = (divn + limit - 1) / limit;
  sys_div  = (sys + limit - 1) / limit;
  if (divn_div == 0)
    {
      divn_div = 1;
    }

  if (sys_div == 0)
    {
      sys_div = 1;
    }

  use_sys = (sys / sys_div) > (divn / divn_div);
  div     = use_sys ? sys_div : divn_div;

  if (use_sys)
    {
      putreg32(CRG_SYS_CLK_SYS_LCD_CLK_SEL, DA1470X_CRG_SYS_SET_CLK_SYS);
    }
  else
    {
      putreg32(CRG_SYS_CLK_SYS_LCD_CLK_SEL, DA1470X_CRG_SYS_RESET_CLK_SYS);
    }

  modifyreg32(DA1470X_LCDC_CLKCTRL, LCDC_CLKCTRL_CLK_DIV_MASK,
              LCDC_CLKCTRL_CLK_DIV(div));

  lcdinfo("Serial clock %lu Hz from %s\n",
          (unsigned long)((use_sys ? sys : divn) / div),
          use_sys ? "the system clock" : "DIVN");
}

/****************************************************************************
 * Name: lcdc_qspi_configure
 *
 * Description:
 *   Configure the DBI block for a quad-SPI panel: 4-wire command phase,
 *   quad data phase, SPI mode 0, and the interface clock divider.
 *
 ****************************************************************************/

static void lcdc_qspi_configure(const struct da1470x_lcdc_panel_s *panel)
{
  uint32_t cfg;

  /* Output colour format on the serial interface: 8-bit RGB888 or the
   * two-byte RGB565 packing, matching the panel's COLMOD setting.  TE
   * detection stays disabled until a frame waits for it.
   */

  cfg = LCDC_DBIB_CFG_DBIB_INTERFACE_EN |
        LCDC_DBIB_CFG_DBIB_RESX_OUT_EN |
        LCDC_DBIB_CFG_SPI4_EN |
        LCDC_DBIB_CFG_QUAD_SPI_EN |
        LCDC_DBIB_CFG_SPI_DC_AS_SPI_SD1 |
        LCDC_DBIB_CFG_INTERFACE_WIDTH_QSPI |
        LCDC_DBIB_CFG_DBIB_TE_DISABLE |
        LCDC_DBIB_CFG_DBIB_COLOR_FMT(panel->bpp == 32 ?
                                     LCDC_OCM_8RGB888 : LCDC_OCM_8RGB565);

  putreg32(cfg, DA1470X_LCDC_DBIB_CFG);

  /* One-shot frames only; guard against FIFO underrun */

  putreg32(LCDC_MODE_UNDERRUN_PREVENTION_EN, DA1470X_LCDC_MODE);

  lcdc_clock_select(panel);

  /* Route the serial interface to the pads and let the LCDC drive them.
   * The panel's tearing-effect line is active high.
   */

  modifyreg32(DA1470X_LCDC_GPIO, LCDC_GPIO_GPIO_OUTPUT_MODE_MASK,
              LCDC_GPIO_OUTPUT_MODE_SPI | LCDC_GPIO_GPIO_OUTPUT_EN |
              LCDC_GPIO_GPIO_SPI_SI_ON_SD_PAD | LCDC_GPIO_TE_INV);
}

/****************************************************************************
 * Name: lcdc_jdi_configure
 *
 * Description:
 *   JDI memory-in-pixel parallel interface: the controller generates
 *   XRST, VST, VCK, HST, HCK and ENB itself and shifts two bits per
 *   colour on six data lines, two rows per VCK (double scan).  The
 *   pulse widths and offsets are derived from the line length the way
 *   the vendor driver does; the horizontal values count HCK quarters,
 *   the vertical ones VCK halves.  There are no commands: a frame is
 *   whole-screen, and the panel keeps it.
 *
 ****************************************************************************/

static void lcdc_jdi_configure(const struct da1470x_lcdc_panel_s *panel)
{
  const struct da1470x_lcdc_jdi_s *jdi = &panel->jdi;
  uint32_t line;
  uint32_t hck_width;
  uint32_t vck_width;
  uint32_t xrst_width;
  uint32_t regval;

  putreg32(LCDC_DBIB_CFG_DBIB_RESX_OUT_EN |
           LCDC_DBIB_CFG_DBIB_TE_DISABLE |
           LCDC_DBIB_CFG_DBIB_COLOR_FMT(LCDC_OCM_RGB222),
           DA1470X_LCDC_DBIB_CFG);

  putreg32(LCDC_MODE_OUT_MODE_JDIMIP | LCDC_MODE_DSCAN |
           LCDC_MODE_UNDERRUN_PREVENTION_EN, DA1470X_LCDC_MODE);

  line       = (panel->xres + jdi->fpx + jdi->blx + jdi->bpx) / 2;
  hck_width  = 2;
  vck_width  = line * hck_width;
  xrst_width = panel->yres * 2 + jdi->fpy + jdi->bly + jdi->bpy - 2;

  regval = LCDC_FMTCTRL_JDIP_HST_WIDTH(hck_width) |
           LCDC_FMTCTRL_JDIP_HST_OFFSET(hck_width) |
           LCDC_FMTCTRL_JDIP_VST_WIDTH(vck_width) |
           LCDC_FMTCTRL_JDIP_VST_OFFSET(vck_width / 2 + 2);
  putreg32(regval, DA1470X_LCDC_FMTCTRL);

  regval = LCDC_FMTCTRL_2_JDIP_ENB_WIDTH(vck_width / 2) |
           LCDC_FMTCTRL_2_JDIP_ENB_OFFSET(vck_width / 4 + 3) |
           LCDC_FMTCTRL_2_JDIP_XRST_OFFSET(vck_width / 4 + 2);
  putreg32(regval, DA1470X_LCDC_FMTCTRL_2);

  putreg32(LCDC_FMTCTRL_3_XRST_HIGH_STATE(xrst_width),
           DA1470X_LCDC_FMTCTRL_3);

  lcdc_clock_select(panel);

  modifyreg32(DA1470X_LCDC_GPIO, LCDC_GPIO_GPIO_OUTPUT_MODE_MASK |
              LCDC_GPIO_GPIO_SPI_SI_ON_SD_PAD | LCDC_GPIO_TE_INV,
              LCDC_GPIO_OUTPUT_MODE_JDI | LCDC_GPIO_GPIO_OUTPUT_EN);

  /* VCOM/FRP: a square wave from the 32 kHz sleep clock divided by
   * 32 * (reload + 1), kept running by CRG_TOP so it survives sleep.
   */

  if (panel->ext_clk_dhz != 0)
    {
      uint32_t reload = (32000 * 10) / ((uint32_t)panel->ext_clk_dhz * 32);

      if (reload > 0)
        {
          reload--;
        }

      if (reload > (CRG_TOP_LCD_EXT_CTRL_LCD_EXT_CNT_RELOAD_MASK >>
                    CRG_TOP_LCD_EXT_CTRL_LCD_EXT_CNT_RELOAD_SHIFT))
        {
          reload = CRG_TOP_LCD_EXT_CTRL_LCD_EXT_CNT_RELOAD_MASK >>
                   CRG_TOP_LCD_EXT_CTRL_LCD_EXT_CNT_RELOAD_SHIFT;
        }

      putreg32(CRG_TOP_LCD_EXT_CTRL_LCD_EXT_CLK_EN |
               CRG_TOP_LCD_EXT_CTRL_LCD_EXT_CNT_RELOAD(reload),
               DA1470X_CRG_TOP_LCD_EXT_CTRL);
    }
}

/****************************************************************************
 * Name: lcdc_set_timing
 *
 * Description:
 *   Program the resolution of the next transfer and the porch/blanking
 *   values: the minimal ones for serial panels (fpx=1, blx=2, bpx=1,
 *   fpy=1, bly=1, bpy=1), the panel's own for JDI parallel.
 *
 ****************************************************************************/

static void lcdc_set_timing(const struct da1470x_lcdc_panel_s *panel,
                            uint16_t resx, uint16_t resy)
{
  uint16_t fpx;
  uint16_t blx;
  uint16_t bpx;
  uint16_t fpy;
  uint16_t bly;
  uint16_t bpy;

  if (panel->iface == DA1470X_LCDC_IF_JDI_PARALLEL)
    {
      /* Two rows per VCK, and the panel's own porches */

      resy *= 2;
      fpx = resx + panel->jdi.fpx;
      blx = fpx + panel->jdi.blx;
      bpx = blx + panel->jdi.bpx;
      fpy = resy + panel->jdi.fpy;
      bly = fpy + panel->jdi.bly;
      bpy = bly + panel->jdi.bpy;
    }
  else
    {
      fpx = resx + 1;
      blx = fpx + 2;
      bpx = blx + 1;
      fpy = resy + 1;
      bly = fpy + 1;
      bpy = bly + 1;
    }

  putreg32(LCDC_XY_PACK(resx, resy),   DA1470X_LCDC_RESXY);
  putreg32(LCDC_XY_PACK(fpx, fpy),     DA1470X_LCDC_FRONTPORCHXY);
  putreg32(LCDC_XY_PACK(blx, bly),     DA1470X_LCDC_BLANKINGXY);
  putreg32(LCDC_XY_PACK(bpx, bpy),     DA1470X_LCDC_BACKPORCHXY);
  putreg32(LCDC_XY_PACK(fpx, fpy - 1), DA1470X_LCDC_STARTXY);
}

/****************************************************************************
 * Name: lcdc_set_layer0
 *
 * Description:
 *   Point Layer 0 at a rectangle of one frame buffer.  The layer keeps
 *   the full line stride so any window of the buffer can be sent.
 *
 ****************************************************************************/

static void lcdc_set_layer0(struct da1470x_lcdc_s *priv, uint8_t *base,
                            uint16_t w, uint16_t h)
{
  const struct da1470x_lcdc_panel_s *panel = priv->panel;
  uint32_t mode;

  uint32_t cm;

  if (panel->iface == DA1470X_LCDC_IF_JDI_PARALLEL)
    {
      h *= 2;                       /* Double scan */
    }

  putreg32((uint32_t)(uintptr_t)base, DA1470X_LCDC_LAYER0_BASEADDR);
  putreg32(LCDC_XY_PACK(0, 0), DA1470X_LCDC_LAYER0_STARTXY);
  putreg32(LCDC_XY_PACK(w, h), DA1470X_LCDC_LAYER0_SIZEXY);
  putreg32(LCDC_XY_PACK(w, h), DA1470X_LCDC_LAYER0_RESXY);
  putreg32(priv->stride, DA1470X_LCDC_LAYER0_STRIDE);

  /* Opaque copy: source factor one, destination factor zero */

  cm = panel->bpp == 32 ? LCDC_LCM_RGBA8888 :
       panel->bpp == 16 ? LCDC_LCM_RGB565 : LCDC_LCM_RGB332;

  mode = LCDC_LAYER0_MODE_L0_EN | LCDC_LAYER0_MODE_L0_ALPHA(0xff) |
         LCDC_LAYER0_MODE_L0_SRC_BLEND(LCDC_BF_ONE) |
         LCDC_LAYER0_MODE_L0_DST_BLEND(LCDC_BF_ZERO) |
         LCDC_LAYER0_MODE_L0_COLOR_MODE(cm);
  putreg32(mode, DA1470X_LCDC_LAYER0_MODE);
}

/****************************************************************************
 * Name: lcdc_interrupt
 *
 * Description:
 *   Either the tearing-effect edge the updater armed, or FRAME_END.  On
 *   FRAME_END release the command/data binding and the forced chip
 *   select so the panel does not latch subsequent traffic as pixels.
 *   In both cases wake the updater.
 *
 ****************************************************************************/

static int lcdc_interrupt(int irq, void *context, void *arg)
{
  struct da1470x_lcdc_s *priv = (struct da1470x_lcdc_s *)arg;
  uint32_t enabled = getreg32(DA1470X_LCDC_INTERRUPT);

  if ((enabled & LCDC_INTERRUPT_TE_IRQ_EN) != 0)
    {
      modifyreg32(DA1470X_LCDC_INTERRUPT, LCDC_INTERRUPT_TE_IRQ_EN, 0);
    }
  else
    {
      modifyreg32(DA1470X_LCDC_INTERRUPT, LCDC_INTERRUPT_FE_IRQ_EN, 0);
      modifyreg32(DA1470X_LCDC_DBIB_CFG,
                  LCDC_DBIB_CFG_CMD_DATA_AS_HEADER |
                  LCDC_DBIB_CFG_DBIB_CSX_CFG_EN, 0);
    }

  nxsem_post(&priv->frame);
  return OK;
}

/****************************************************************************
 * Name: lcdc_wait_te
 *
 * Description:
 *   Enable tearing-effect detection, wait for the next edge, disable it
 *   again.  A missing edge only costs the timeout.
 *
 ****************************************************************************/

#ifdef CONFIG_DA1470X_LCDC_TE
static void lcdc_wait_te(struct da1470x_lcdc_s *priv)
{
  int ret;

  modifyreg32(DA1470X_LCDC_DBIB_CFG, LCDC_DBIB_CFG_DBIB_TE_DISABLE, 0);
  modifyreg32(DA1470X_LCDC_INTERRUPT, 0, LCDC_INTERRUPT_TE_IRQ_EN);

  ret = nxsem_tickwait_uninterruptible(&priv->frame,
                                       MSEC2TICK(LCDC_TE_TIMEOUT_MS));
  if (ret < 0)
    {
      lcdwarn("No tearing-effect edge\n");
      modifyreg32(DA1470X_LCDC_INTERRUPT, LCDC_INTERRUPT_TE_IRQ_EN, 0);
    }

  modifyreg32(DA1470X_LCDC_DBIB_CFG, 0, LCDC_DBIB_CFG_DBIB_TE_DISABLE);
}
#endif

/****************************************************************************
 * Name: lcdc_send_region
 *
 * Description:
 *   Transfer the rectangle (x, y, w, h) of frame buffer 'buf' to the same
 *   position of the panel: set the panel window, size the transfer and
 *   the layer, queue the RAMWR command bound to the pixel stream with
 *   chip select held low, trigger a single frame and wait for it.
 *
 ****************************************************************************/

static int lcdc_send_region(struct da1470x_lcdc_s *priv, int buf,
                            uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  const struct da1470x_lcdc_panel_s *panel = priv->panel;
  uint8_t *base;
  uint32_t cfg;
  int ret;

  if (panel->window != NULL)
    {
      panel->window(panel, x, y, x + w - 1, y + h - 1);
    }

  base = priv->fbmem + (size_t)buf * priv->buflen +
         (size_t)y * priv->stride + (size_t)x * (panel->bpp / 8);

  lcdc_set_timing(panel, w, h);
  lcdc_set_layer0(priv, base, w, h);

  if (panel->iface == DA1470X_LCDC_IF_JDI_PARALLEL)
    {
      /* No command header, no chip select: fire the frame */

#ifdef CONFIG_PM
      pm_stay(PM_IDLE_DOMAIN, PM_NORMAL);
#endif
      modifyreg32(DA1470X_LCDC_INTERRUPT, 0, LCDC_INTERRUPT_FE_IRQ_EN);
      modifyreg32(DA1470X_LCDC_MODE, 0, LCDC_MODE_SFRAME_UPD);
      ret = nxsem_tickwait_uninterruptible(&priv->frame,
                                           MSEC2TICK(LCDC_FRAME_TIMEOUT_MS));
#ifdef CONFIG_PM
      pm_relax(PM_IDLE_DOMAIN, PM_NORMAL);
#endif
      if (ret < 0)
        {
          lcderr("Frame timed out (status %08" PRIx32 ")\n",
                 getreg32(DA1470X_LCDC_STATUS));
          modifyreg32(DA1470X_LCDC_INTERRUPT, LCDC_INTERRUPT_FE_IRQ_EN, 0);
        }

      return ret;
    }

#ifdef CONFIG_DA1470X_LCDC_TE
  if (panel->te)
    {
      lcdc_wait_te(priv);
    }
#endif

  /* Hold the write-memory command so it goes out as the header of the
   * pixel burst, in one chip-select transaction.
   */

  da1470x_lcdc_dcs_hold(true);

  lcdc_fifo_push(LCDC_DBIB_CMD_DBIB_CMD_SEND |
                 LCDC_DBIB_CMD_QSPI_SERIAL_CMD_TRANS |
                 panel->frame_prefix);
  lcdc_fifo_push(LCDC_DBIB_CMD_DBIB_CMD_SEND |
                 LCDC_DBIB_CMD_QSPI_SERIAL_CMD_TRANS |
                 LCDC_DBIB_CMD_CMD_WIDTH_24 |
                 ((uint32_t)panel->ramwr << 8));

  /* Chip select forced low for the whole frame */

  cfg  = getreg32(DA1470X_LCDC_DBIB_CFG);
  cfg |= LCDC_DBIB_CFG_DBIB_CSX_CFG_EN;
  cfg &= ~LCDC_DBIB_CFG_DBIB_CSX_CFG;
  putreg32(cfg, DA1470X_LCDC_DBIB_CFG);

  /* Arm FRAME_END and fire the frame.  The system clock, and with it
   * the serial clock, must not change while the frame is on the wire.
   */

#ifdef CONFIG_PM
  pm_stay(PM_IDLE_DOMAIN, PM_NORMAL);
#endif

  modifyreg32(DA1470X_LCDC_INTERRUPT, 0, LCDC_INTERRUPT_FE_IRQ_EN);
  modifyreg32(DA1470X_LCDC_MODE, 0, LCDC_MODE_SFRAME_UPD);

  ret = nxsem_tickwait_uninterruptible(&priv->frame,
                                       MSEC2TICK(LCDC_FRAME_TIMEOUT_MS));

#ifdef CONFIG_PM
  pm_relax(PM_IDLE_DOMAIN, PM_NORMAL);
#endif
  if (ret < 0)
    {
      lcderr("Frame timed out (status %08" PRIx32 ")\n",
             getreg32(DA1470X_LCDC_STATUS));

      /* Recover the interface as the interrupt would have */

      modifyreg32(DA1470X_LCDC_INTERRUPT, LCDC_INTERRUPT_FE_IRQ_EN, 0);
      modifyreg32(DA1470X_LCDC_DBIB_CFG,
                  LCDC_DBIB_CFG_CMD_DATA_AS_HEADER |
                  LCDC_DBIB_CFG_DBIB_CSX_CFG_EN, 0);
    }

  return ret;
}

/****************************************************************************
 * Name: lcdc_thread
 *
 * Description:
 *   Sends the queued transfer and reports the controller idle again.
 *   With two frame buffers the client that alternates between them can
 *   draw its next frame while this thread waits for the tearing-effect
 *   edge and for the end of the frame.
 *
 ****************************************************************************/

#ifdef CONFIG_DA1470X_LCDC_ASYNC
static int lcdc_thread(int argc, char *argv[])
{
  struct da1470x_lcdc_s *priv = &g_lcdc;
  int ret;

  for (; ; )
    {
      ret = nxsem_wait_uninterruptible(&priv->request);
      if (ret < 0)
        {
          continue;
        }

      ret = lcdc_send_region(priv, priv->reqbuf, priv->req.x, priv->req.y,
                             priv->req.w, priv->req.h);
      if (ret < 0)
        {
          lcderr("Queued transfer failed: %d\n", ret);
        }

      nxsem_post(&priv->idle);
    }

  return 0;
}
#endif

/****************************************************************************
 * Name: lcdc_transfer
 *
 * Description:
 *   Wait for the transfer in flight, if any, then send a region either
 *   from the driver thread (returning at once) or from the caller.  The
 *   lock must be held.
 *
 ****************************************************************************/

static int lcdc_transfer(struct da1470x_lcdc_s *priv, int buf, uint16_t x,
                         uint16_t y, uint16_t w, uint16_t h, bool async)
{
#ifdef CONFIG_DA1470X_LCDC_ASYNC
  int ret;

  ret = nxsem_wait_uninterruptible(&priv->idle);
  if (ret < 0)
    {
      return ret;
    }

  if (async)
    {
      priv->reqbuf = buf;
      priv->req.x  = x;
      priv->req.y  = y;
      priv->req.w  = w;
      priv->req.h  = h;
      nxsem_post(&priv->request);
      return OK;
    }

  ret = lcdc_send_region(priv, buf, x, y, w, h);
  nxsem_post(&priv->idle);
  return ret;
#else
  UNUSED(async);
  return lcdc_send_region(priv, buf, x, y, w, h);
#endif
}

/****************************************************************************
 * Name: lcdc_panwork
 *
 * Description:
 *   Every update is transferred synchronously, so a pan request queued by
 *   the upper half is complete as soon as it is recorded.  Release the
 *   queue from the work queue, after the ioctl that queued it returned.
 *
 ****************************************************************************/

static void lcdc_panwork(void *arg)
{
  struct da1470x_lcdc_s *priv = (struct da1470x_lcdc_s *)arg;

  while (fb_remove_paninfo(&priv->vtable, FB_NO_OVERLAY) == OK);
}

/****************************************************************************
 * Name: lcdc_getvideoinfo
 ****************************************************************************/

static int lcdc_getvideoinfo(struct fb_vtable_s *vtable,
                             struct fb_videoinfo_s *vinfo)
{
  struct da1470x_lcdc_s *priv = (struct da1470x_lcdc_s *)vtable;

  if (vinfo == NULL || !priv->initialized)
    {
      return -EINVAL;
    }

  memset(vinfo, 0, sizeof(*vinfo));
  vinfo->fmt     = priv->panel->bpp == 32 ? FB_FMT_RGBA32 :
                   priv->panel->bpp == 16 ? FB_FMT_RGB16_565 :
                   FB_FMT_RGB8_332;
  vinfo->xres    = priv->panel->xres;
  vinfo->yres    = priv->panel->yres;
  vinfo->nplanes = 1;
  return OK;
}

/****************************************************************************
 * Name: lcdc_getplaneinfo
 ****************************************************************************/

static int lcdc_getplaneinfo(struct fb_vtable_s *vtable, int planeno,
                             struct fb_planeinfo_s *pinfo)
{
  struct da1470x_lcdc_s *priv = (struct da1470x_lcdc_s *)vtable;

  if (pinfo == NULL || planeno != 0 || !priv->initialized)
    {
      return -EINVAL;
    }

  memset(pinfo, 0, sizeof(*pinfo));
  pinfo->fbmem        = priv->fbmem;
  pinfo->fblen        = priv->fblen;
  pinfo->stride       = priv->stride;
  pinfo->display      = 0;
  pinfo->bpp          = priv->panel->bpp;
  pinfo->xres_virtual = priv->panel->xres;
  pinfo->yres_virtual = priv->panel->yres * LCDC_NBUFFERS;
  return OK;
}

/****************************************************************************
 * Name: lcdc_updatearea
 *
 * Description:
 *   Send the dirty rectangle.  Rows beyond the visible height address
 *   the second buffer.  The RM69091-class controllers need the window
 *   to start on even coordinates with even sizes, so the rectangle is
 *   widened to satisfy that and clamped to the panel.
 *
 ****************************************************************************/

static int lcdc_updatearea(struct fb_vtable_s *vtable,
                           const struct fb_area_s *area)
{
  struct da1470x_lcdc_s *priv = (struct da1470x_lcdc_s *)vtable;
  const struct da1470x_lcdc_panel_s *panel = priv->panel;
  uint16_t x0;
  uint16_t y0;
  uint16_t x1;
  uint16_t y1;
  int buf;
  int ret;

  if (!priv->initialized)
    {
      return -ENODEV;
    }

  if (!priv->poweron)
    {
      /* A dark panel takes no pixels, but a client that keeps drawing is
       * not in error either.
       */

      return OK;
    }

  /* The row range names the buffer: rows beyond the visible height are
   * the second buffer.  Clients that pan (LVGL, the fb example) always
   * update with the offset of the buffer they drew into, so the area is
   * taken from that buffer and the pan that follows does not send it
   * again.
   */

  buf = area->y / panel->yres;
  if (buf >= LCDC_NBUFFERS)
    {
      return -EINVAL;
    }

  x0 = area->x;
  y0 = area->y % panel->yres;
  x1 = x0 + area->w;
  y1 = y0 + area->h;

  if (panel->iface == DA1470X_LCDC_IF_JDI_PARALLEL)
    {
      /* The parallel interface addresses no window: whole frames only */

      x0 = 0;
      y0 = 0;
      x1 = panel->xres;
      y1 = panel->yres;
    }

  x0 &= ~1;
  y0 &= ~1;
  x1  = (x1 + 1) & ~1;
  y1  = (y1 + 1) & ~1;

  if (x1 > panel->xres)
    {
      x1 = panel->xres;
    }

  if (y1 > panel->yres)
    {
      y1 = panel->yres;
    }

  if (x0 >= x1 || y0 >= y1)
    {
      return -EINVAL;
    }

  ret = nxmutex_lock(&priv->lock);
  if (ret < 0)
    {
      return ret;
    }

  /* A client that hands over another buffer than last time is double
   * buffering: it will not touch this one until the next swap, so the
   * frame can go out while it draws.  Anybody else may redraw the same
   * buffer as soon as this returns and gets a finished transfer.
   */

  ret = lcdc_transfer(priv, buf, x0, y0, x1 - x0, y1 - y0,
                      buf != priv->lastbuf);

  priv->pending.x = x0;
  priv->pending.y = y0;
  priv->pending.w = x1 - x0;
  priv->pending.h = y1 - y0;
  priv->lastbuf   = buf;

  nxmutex_unlock(&priv->lock);
  return ret;
}

/****************************************************************************
 * Name: lcdc_pandisplay
 *
 * Description:
 *   The client selects the buffer to show through yoffset.  The content
 *   has already been transferred by the preceding update, so nothing is
 *   sent here; the pan queue entry is released from the work queue.
 *
 ****************************************************************************/

static int lcdc_pandisplay(struct fb_vtable_s *vtable,
                           struct fb_planeinfo_s *pinfo)
{
  struct da1470x_lcdc_s *priv = (struct da1470x_lcdc_s *)vtable;
  int buf;
  int ret;

  if (!priv->initialized || pinfo == NULL ||
      pinfo->yoffset >= priv->panel->yres * LCDC_NBUFFERS)
    {
      return -EINVAL;
    }

  buf = pinfo->yoffset / priv->panel->yres;

  ret = nxmutex_lock(&priv->lock);
  if (ret < 0)
    {
      return ret;
    }

  /* The last update was taken from another buffer than the one now
   * selected: the client drew into the new buffer before panning, so
   * transfer the same area from it.
   */

  if (priv->poweron && buf != priv->lastbuf && priv->pending.w != 0)
    {
      lcdc_transfer(priv, buf, priv->pending.x, priv->pending.y,
                    priv->pending.w, priv->pending.h, false);
      priv->lastbuf = buf;
    }

  priv->shown = buf;
  nxmutex_unlock(&priv->lock);

  if (work_available(&priv->panwork))
    {
      work_queue(LPWORK, &priv->panwork, lcdc_panwork, priv, 0);
    }

  return OK;
}

/****************************************************************************
 * Name: lcdc_getpower
 ****************************************************************************/

static int lcdc_getpower(struct fb_vtable_s *vtable)
{
  struct da1470x_lcdc_s *priv = (struct da1470x_lcdc_s *)vtable;

  return priv->poweron ? 1 : 0;
}

/****************************************************************************
 * Name: lcdc_setpower
 *
 * Description:
 *   Light the panel or put it out.  This is also where the display hands
 *   the system back to the power manager: while the panel is lit the
 *   driver holds PM_NORMAL, because the pixel clock is derived from the
 *   system clock and a frame drawn at a quarter of the speed is a frame
 *   that misses its refresh.  With the panel dark nothing here needs the
 *   fast clock, the hold is released and the part is free to descend as
 *   far as the rest of the system allows.
 *
 ****************************************************************************/

static int lcdc_setpower(struct fb_vtable_s *vtable, int power)
{
  struct da1470x_lcdc_s *priv = (struct da1470x_lcdc_s *)vtable;
  const struct da1470x_lcdc_panel_s *panel;
  int ret = OK;

  if (!priv->initialized)
    {
      return -ENODEV;
    }

  ret = nxmutex_lock(&priv->lock);
  if (ret < 0)
    {
      return ret;
    }

  panel = priv->panel;

  if (power > 0 && !priv->poweron)
    {
#ifdef CONFIG_PM
      pm_stay(PM_IDLE_DOMAIN, PM_NORMAL);
#endif
      putreg32(CRG_SYS_CLK_SYS_LCD_ENABLE, DA1470X_CRG_SYS_SET_CLK_SYS);

      if (panel->iface == DA1470X_LCDC_IF_JDI_PARALLEL)
        {
          lcdc_jdi_configure(panel);
        }
      else
        {
          lcdc_qspi_configure(panel);
        }

      lcdc_set_timing(panel, panel->xres, panel->yres);
      lcdc_set_layer0(priv, priv->fbmem + (size_t)priv->shown * priv->buflen,
                      panel->xres, panel->yres);

      if (panel->power != NULL)
        {
          panel->power(panel, true);
        }

      if (panel->init != NULL)
        {
          ret = panel->init(panel);
        }

      /* init leaves the panel at full brightness, so restore the level the
       * caller last asked for -- otherwise powering on flashes full bright
       * before the next fade step brings it down.
       */

      if (panel->brightness != NULL)
        {
          panel->brightness(panel, priv->brightness);
        }

      priv->poweron = true;
    }
  else if (power == 0 && priv->poweron)
    {
      /* Nothing may be on the wire when the panel loses its supply */

#ifdef CONFIG_DA1470X_LCDC_ASYNC
      nxsem_wait_uninterruptible(&priv->idle);
      nxsem_post(&priv->idle);
#endif

      if (panel->power != NULL)
        {
          panel->power(panel, false);
        }

      putreg32(0, DA1470X_LCDC_INTERRUPT);
      putreg32(CRG_SYS_CLK_SYS_LCD_ENABLE, DA1470X_CRG_SYS_RESET_CLK_SYS);

      priv->poweron = false;
      priv->pending.w = 0;
#ifdef CONFIG_PM
      pm_relax(PM_IDLE_DOMAIN, PM_NORMAL);
#endif
    }

  nxmutex_unlock(&priv->lock);
  return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_lcdc_set_brightness
 *
 * Description:
 *   Set the panel's brightness, serialised against the frame engine.
 *
 *   Raw DCS traffic from outside the driver is not safe while frames are
 *   moving: lcdc_send_region() holds CS low across the whole transfer and
 *   sets CMD_DATA_AS_HEADER, so an injected command's prefix and bytes
 *   land inside the pixel stream -- a corrupt frame at best, a wedged
 *   interface at worst.
 *
 *   The mutex alone does not make it safe either. With
 *   CONFIG_DA1470X_LCDC_ASYNC, updatearea() queues the frame to the
 *   driver thread and releases the lock while it is still on the wire, so
 *   we must also wait for the frame engine to go idle. Worst case this
 *   blocks for one frame.
 *
 *   Note brightness 0 does NOT power the panel down: it stays on and the
 *   LCDC keeps its pm_stay, so the system will not sleep. Use
 *   FBIOSET_POWER 0 for that. Coming back, setpower(1) re-runs the panel
 *   init, which restores full brightness -- so a fade-in has to write its
 *   level after powering on, not before.
 *
 ****************************************************************************/

int da1470x_lcdc_set_brightness(uint8_t level)
{
  struct da1470x_lcdc_s *priv = &g_lcdc;
  const struct da1470x_lcdc_panel_s *panel;
  int ret;

  if (!priv->initialized)
    {
      return -ENODEV;
    }

  if (priv->panel->brightness == NULL)
    {
      return -ENOTSUP;
    }

  ret = nxmutex_lock(&priv->lock);
  if (ret < 0)
    {
      return ret;
    }

  if (!priv->poweron)
    {
      /* A dark panel takes no commands, and the init on the way back up
       * sets the brightness anyway.
       */

      nxmutex_unlock(&priv->lock);
      return OK;
    }

  panel = priv->panel;

#ifdef CONFIG_DA1470X_LCDC_ASYNC
  nxsem_wait_uninterruptible(&priv->idle);
#endif

  panel->brightness(panel, level);

#ifdef CONFIG_DA1470X_LCDC_ASYNC
  nxsem_post(&priv->idle);
#endif

  nxmutex_unlock(&priv->lock);
  return OK;
}

/****************************************************************************
 * Name: da1470x_lcdc_dcs_cmd
 ****************************************************************************/

void da1470x_lcdc_dcs_cmd(uint8_t cmd)
{
  uint8_t prefix = g_lcdc.panel ? g_lcdc.panel->cmd_prefix : 0x02;

  /* Prefix byte opens a new transaction; the DCS command follows in a
   * 24-bit field with the command at bits 15:8.
   */

  lcdc_fifo_push(LCDC_DBIB_CMD_DBIB_CMD_SEND |
                 LCDC_DBIB_CMD_QSPI_SERIAL_CMD_TRANS | prefix);
  lcdc_fifo_push(LCDC_DBIB_CMD_DBIB_CMD_SEND |
                 LCDC_DBIB_CMD_QSPI_SERIAL_CMD_TRANS |
                 LCDC_DBIB_CMD_CMD_WIDTH_24 | ((uint32_t)cmd << 8));
}

/****************************************************************************
 * Name: da1470x_lcdc_dcs_data
 ****************************************************************************/

void da1470x_lcdc_dcs_data(uint8_t data)
{
  lcdc_fifo_push(LCDC_DBIB_CMD_QSPI_SERIAL_CMD_TRANS | data);
}

/****************************************************************************
 * Name: da1470x_lcdc_dcs_hold
 ****************************************************************************/

void da1470x_lcdc_dcs_hold(bool hold)
{
  int i;

  /* The configuration register must not change while the command FIFO
   * is being drained.
   */

  for (i = 0; i < LCDC_FIFO_WAIT_LOOPS; i++)
    {
      if ((getreg32(DA1470X_LCDC_STATUS) &
           LCDC_STATUS_DBIB_CMD_PENDING) == 0)
        {
          break;
        }
    }

  if (hold)
    {
      modifyreg32(DA1470X_LCDC_DBIB_CFG, 0,
                  LCDC_DBIB_CFG_CMD_DATA_AS_HEADER);
    }
  else
    {
      modifyreg32(DA1470X_LCDC_DBIB_CFG,
                  LCDC_DBIB_CFG_CMD_DATA_AS_HEADER, 0);
    }
}

/****************************************************************************
 * Name: da1470x_lcdc_register
 ****************************************************************************/

int da1470x_lcdc_register(const struct da1470x_lcdc_panel_s *panel)
{
  struct da1470x_lcdc_s *priv = &g_lcdc;
  uint32_t id;
  int ret;

  if (panel == NULL || panel->xres == 0 || panel->yres == 0 ||
      (panel->bpp != 16 && panel->bpp != 32 &&
       !(panel->bpp == 8 && panel->iface == DA1470X_LCDC_IF_JDI_PARALLEL)))
    {
      return -EINVAL;
    }

  if (priv->initialized)
    {
      return OK;
    }

  /* The LCDC lives in PD_GPU and is clocked from DIVN through CRG_SYS */

  ret = da1470x_pd_enable(DA1470X_PD_GPU);
  if (ret < 0)
    {
      return ret;
    }

  putreg32(CRG_SYS_CLK_SYS_LCD_RESET_REQ, DA1470X_CRG_SYS_RESET_CLK_SYS);
  putreg32(CRG_SYS_CLK_SYS_LCD_CLK_SEL, DA1470X_CRG_SYS_RESET_CLK_SYS);
  putreg32(CRG_SYS_CLK_SYS_LCD_ENABLE, DA1470X_CRG_SYS_SET_CLK_SYS);

  id = getreg32(DA1470X_LCDC_IDREG);
  if (id != DA1470X_LCDC_IDREG_MAGIC)
    {
      lcderr("LCDC ID mismatch: %08" PRIx32 "\n", id);
      return -ENODEV;
    }

  /* Frame buffers */

  priv->panel  = panel;
  priv->stride = panel->xres * (panel->bpp / 8);
  priv->buflen = (size_t)priv->stride * panel->yres;
  priv->fblen  = priv->buflen * LCDC_NBUFFERS;
  priv->fbmem  = kmm_memalign(32, priv->fblen);
  if (priv->fbmem == NULL)
    {
      return -ENOMEM;
    }

  memset(priv->fbmem, 0, priv->fblen);

  /* Controller configuration, then the panel start-up sequence */

  if (panel->iface == DA1470X_LCDC_IF_JDI_PARALLEL)
    {
      lcdc_jdi_configure(panel);
    }
  else
    {
      lcdc_qspi_configure(panel);
    }

  lcdc_set_timing(panel, panel->xres, panel->yres);
  lcdc_set_layer0(priv, priv->fbmem, panel->xres, panel->yres);

  putreg32(0, DA1470X_LCDC_INTERRUPT);
  irq_attach(DA1470X_IRQ_LCD, lcdc_interrupt, priv);
  up_enable_irq(DA1470X_IRQ_LCD);

  if (panel->power != NULL)
    {
      panel->power(panel, true);
    }

  if (panel->init != NULL)
    {
      ret = panel->init(panel);
      if (ret < 0)
        {
          lcderr("Panel init failed: %d\n", ret);
          kmm_free(priv->fbmem);
          priv->fbmem = NULL;
          return ret;
        }
    }

  /* The panel is lit from here on, and holds the system at its full clock
   * until something turns the display off through FBIOSET_POWER.
   */

  priv->poweron = true;
#ifdef CONFIG_PM
  pm_stay(PM_IDLE_DOMAIN, PM_NORMAL);
#endif

#ifdef CONFIG_DA1470X_LCDC_ASYNC
  ret = kthread_create("lcdc", LCDC_THREAD_PRIORITY, LCDC_THREAD_STACKSIZE,
                       lcdc_thread, NULL);
  if (ret < 0)
    {
      lcderr("Transfer thread failed: %d\n", ret);
      kmm_free(priv->fbmem);
      priv->fbmem = NULL;
      return ret;
    }
#endif

  priv->initialized = true;

  /* Push the cleared frame buffer so the panel does not show whatever
   * its own memory held at power-up.
   */

  ret = lcdc_send_region(priv, 0, 0, 0, panel->xres, panel->yres);
  if (ret < 0)
    {
      lcdwarn("Initial frame failed: %d\n", ret);
    }

  return OK;
}

/****************************************************************************
 * Name: up_fbinitialize
 *
 * Description:
 *   Initialize the framebuffer video hardware associated with the display.
 *   The controller is set up by da1470x_lcdc_register() from the board;
 *   here we only check that it happened.
 *
 ****************************************************************************/

int up_fbinitialize(int display)
{
  if (display != 0)
    {
      return -EINVAL;
    }

  return g_lcdc.initialized ? OK : -ENODEV;
}

/****************************************************************************
 * Name: up_fbgetvplane
 *
 * Description:
 *   Return a reference to the framebuffer object for the specified video
 *   plane of the specified plane.
 *
 ****************************************************************************/

struct fb_vtable_s *up_fbgetvplane(int display, int vplane)
{
  if (display != 0 || vplane != 0 || !g_lcdc.initialized)
    {
      return NULL;
    }

  return &g_lcdc.vtable;
}

/****************************************************************************
 * Name: up_fbuninitialize
 *
 * Description:
 *   Uninitialize the framebuffer support for the specified display.
 *
 ****************************************************************************/

void up_fbuninitialize(int display)
{
  struct da1470x_lcdc_s *priv = &g_lcdc;

  if (display != 0 || !priv->initialized)
    {
      return;
    }

  up_disable_irq(DA1470X_IRQ_LCD);
  irq_detach(DA1470X_IRQ_LCD);

  if (priv->poweron)
    {
      if (priv->panel->power != NULL)
        {
          priv->panel->power(priv->panel, false);
        }

      priv->poweron = false;
#ifdef CONFIG_PM
      pm_relax(PM_IDLE_DOMAIN, PM_NORMAL);
#endif
    }

  putreg32(0, DA1470X_LCDC_LAYER0_MODE);
  putreg32(0, DA1470X_LCDC_DBIB_CFG);
  putreg32(CRG_SYS_CLK_SYS_LCD_ENABLE, DA1470X_CRG_SYS_RESET_CLK_SYS);

  kmm_free(priv->fbmem);
  priv->fbmem       = NULL;
  priv->initialized = false;
}

#endif /* CONFIG_DA1470X_LCDC */
