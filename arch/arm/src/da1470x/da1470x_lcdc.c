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
 * frame buffer by DMA; each update sends the RAMWR command followed by the
 * whole frame as one chip-select burst and waits for FRAME_END.
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
#include <nuttx/mutex.h>
#include <nuttx/semaphore.h>
#include <nuttx/video/fb.h>

#include "arm_internal.h"
#include "hardware/da1470x_crg_sys.h"
#include "hardware/da1470x_lcdc.h"
#include "da1470x_lcdc.h"
#include "da1470x_pmu.h"

#ifdef CONFIG_DA1470X_LCDC

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define LCDC_FIFO_WAIT_LOOPS    100000
#define LCDC_FRAME_TIMEOUT_MS   500

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct da1470x_lcdc_s
{
  struct fb_vtable_s vtable;                  /* Framebuffer interface */
  const struct da1470x_lcdc_panel_s *panel;   /* Attached panel */
  uint8_t *fbmem;                             /* Frame buffer */
  size_t   fblen;                             /* Frame buffer size */
  uint16_t stride;                            /* Bytes per line */
  mutex_t  lock;                              /* Serialises updates */
  sem_t    frame;                             /* FRAME_END completion */
  bool     initialized;
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
  },
  .lock  = NXMUTEX_INITIALIZER,
  .frame = SEM_INITIALIZER(0),
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
  uint32_t clkdiv = panel->clkdiv ? panel->clkdiv : 1;

  /* Output colour format on the serial interface: 8-bit RGB888 or the
   * two-byte RGB565 packing, matching the panel's COLMOD setting.
   */

  cfg = LCDC_DBIB_CFG_DBIB_INTERFACE_EN |
        LCDC_DBIB_CFG_DBIB_RESX_OUT_EN |
        LCDC_DBIB_CFG_SPI4_EN |
        LCDC_DBIB_CFG_QUAD_SPI_EN |
        LCDC_DBIB_CFG_SPI_DC_AS_SPI_SD1 |
        LCDC_DBIB_CFG_INTERFACE_WIDTH_QSPI |
        LCDC_DBIB_CFG_DBIB_COLOR_FMT(panel->bpp == 32 ?
                                     LCDC_OCM_8RGB888 : LCDC_OCM_8RGB565);

  if (!panel->te)
    {
      cfg |= LCDC_DBIB_CFG_DBIB_TE_DISABLE;
    }

  putreg32(cfg, DA1470X_LCDC_DBIB_CFG);

  /* One-shot frames only; guard against FIFO underrun */

  putreg32(LCDC_MODE_UNDERRUN_PREVENTION_EN, DA1470X_LCDC_MODE);

  modifyreg32(DA1470X_LCDC_CLKCTRL, LCDC_CLKCTRL_CLK_DIV_MASK,
              LCDC_CLKCTRL_CLK_DIV(clkdiv));

  /* Route the serial interface to the pads and let the LCDC drive them */

  modifyreg32(DA1470X_LCDC_GPIO, LCDC_GPIO_GPIO_OUTPUT_MODE_MASK,
              LCDC_GPIO_OUTPUT_MODE_SPI | LCDC_GPIO_GPIO_OUTPUT_EN |
              LCDC_GPIO_GPIO_SPI_SI_ON_SD_PAD);
}

/****************************************************************************
 * Name: lcdc_set_timing
 *
 * Description:
 *   Program the resolution and the minimal porch/blanking values used for
 *   serial panels (fpx=1, blx=2, bpx=1, fpy=1, bly=1, bpy=1).
 *
 ****************************************************************************/

static void lcdc_set_timing(uint16_t resx, uint16_t resy)
{
  uint16_t fpx = resx + 1;
  uint16_t blx = fpx + 2;
  uint16_t bpx = blx + 1;
  uint16_t fpy = resy + 1;
  uint16_t bly = fpy + 1;
  uint16_t bpy = bly + 1;

  putreg32(LCDC_XY_PACK(resx, resy),   DA1470X_LCDC_RESXY);
  putreg32(LCDC_XY_PACK(fpx, fpy),     DA1470X_LCDC_FRONTPORCHXY);
  putreg32(LCDC_XY_PACK(blx, bly),     DA1470X_LCDC_BLANKINGXY);
  putreg32(LCDC_XY_PACK(bpx, bpy),     DA1470X_LCDC_BACKPORCHXY);
  putreg32(LCDC_XY_PACK(fpx, fpy - 1), DA1470X_LCDC_STARTXY);
}

/****************************************************************************
 * Name: lcdc_set_layer0
 ****************************************************************************/

static void lcdc_set_layer0(struct da1470x_lcdc_s *priv)
{
  const struct da1470x_lcdc_panel_s *panel = priv->panel;
  uint32_t mode;

  putreg32((uint32_t)(uintptr_t)priv->fbmem, DA1470X_LCDC_LAYER0_BASEADDR);
  putreg32(LCDC_XY_PACK(0, 0), DA1470X_LCDC_LAYER0_STARTXY);
  putreg32(LCDC_XY_PACK(panel->xres, panel->yres),
           DA1470X_LCDC_LAYER0_SIZEXY);
  putreg32(LCDC_XY_PACK(panel->xres, panel->yres),
           DA1470X_LCDC_LAYER0_RESXY);
  putreg32(priv->stride, DA1470X_LCDC_LAYER0_STRIDE);

  /* Opaque copy: source factor one, destination factor zero */

  mode = LCDC_LAYER0_MODE_L0_EN | LCDC_LAYER0_MODE_L0_ALPHA(0xff) |
         LCDC_LAYER0_MODE_L0_SRC_BLEND(LCDC_BF_ONE) |
         LCDC_LAYER0_MODE_L0_DST_BLEND(LCDC_BF_ZERO) |
         LCDC_LAYER0_MODE_L0_COLOR_MODE(panel->bpp == 32 ?
                                        LCDC_LCM_RGBA8888 :
                                        LCDC_LCM_RGB565);
  putreg32(mode, DA1470X_LCDC_LAYER0_MODE);
}

/****************************************************************************
 * Name: lcdc_interrupt
 *
 * Description:
 *   FRAME_END: release the command/data binding and the forced chip
 *   select so the panel does not latch subsequent traffic as pixels,
 *   then wake the updater.
 *
 ****************************************************************************/

static int lcdc_interrupt(int irq, void *context, void *arg)
{
  struct da1470x_lcdc_s *priv = (struct da1470x_lcdc_s *)arg;

  modifyreg32(DA1470X_LCDC_INTERRUPT, LCDC_INTERRUPT_FE_IRQ_EN, 0);

  modifyreg32(DA1470X_LCDC_DBIB_CFG,
              LCDC_DBIB_CFG_CMD_DATA_AS_HEADER |
              LCDC_DBIB_CFG_DBIB_CSX_CFG_EN, 0);

  nxsem_post(&priv->frame);
  return OK;
}

/****************************************************************************
 * Name: lcdc_send_frame
 *
 * Description:
 *   Queue the RAMWR command with the frame prefix, bind it to the pixel
 *   stream with chip select held low, trigger a single frame and wait for
 *   it to complete.
 *
 ****************************************************************************/

static int lcdc_send_frame(struct da1470x_lcdc_s *priv)
{
  const struct da1470x_lcdc_panel_s *panel = priv->panel;
  uint32_t cfg;
  int ret;

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

  /* Arm FRAME_END and fire the frame */

  modifyreg32(DA1470X_LCDC_INTERRUPT, 0, LCDC_INTERRUPT_FE_IRQ_EN);
  modifyreg32(DA1470X_LCDC_MODE, 0, LCDC_MODE_SFRAME_UPD);

  ret = nxsem_tickwait_uninterruptible(&priv->frame,
                                       MSEC2TICK(LCDC_FRAME_TIMEOUT_MS));
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
  vinfo->fmt     = priv->panel->bpp == 32 ? FB_FMT_RGBA32 : FB_FMT_RGB16_565;
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
  pinfo->yres_virtual = priv->panel->yres;
  return OK;
}

/****************************************************************************
 * Name: lcdc_updatearea
 *
 * Description:
 *   The panel is refreshed as a whole: the RM69091-class controllers take
 *   the full frame after RAMWR, so partial windows are folded into a full
 *   update.
 *
 ****************************************************************************/

static int lcdc_updatearea(struct fb_vtable_s *vtable,
                           const struct fb_area_s *area)
{
  struct da1470x_lcdc_s *priv = (struct da1470x_lcdc_s *)vtable;
  const struct da1470x_lcdc_panel_s *panel = priv->panel;
  int ret;

  if (!priv->initialized)
    {
      return -ENODEV;
    }

  ret = nxmutex_lock(&priv->lock);
  if (ret < 0)
    {
      return ret;
    }

  if (panel->window != NULL)
    {
      panel->window(panel, 0, 0, panel->xres - 1, panel->yres - 1);
    }

  ret = lcdc_send_frame(priv);
  nxmutex_unlock(&priv->lock);
  return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

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
      (panel->bpp != 16 && panel->bpp != 32))
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

  /* Frame buffer */

  priv->panel  = panel;
  priv->stride = panel->xres * (panel->bpp / 8);
  priv->fblen  = (size_t)priv->stride * panel->yres;
  priv->fbmem  = kmm_memalign(32, priv->fblen);
  if (priv->fbmem == NULL)
    {
      return -ENOMEM;
    }

  memset(priv->fbmem, 0, priv->fblen);

  /* Controller configuration, then the panel start-up sequence */

  lcdc_qspi_configure(panel);
  lcdc_set_timing(panel->xres, panel->yres);
  lcdc_set_layer0(priv);

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

  priv->initialized = true;

  /* Push the cleared frame buffer so the panel does not show whatever
   * its own memory held at power-up.
   */

  ret = lcdc_send_frame(priv);
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

  if (priv->panel->power != NULL)
    {
      priv->panel->power(priv->panel, false);
    }

  putreg32(0, DA1470X_LCDC_LAYER0_MODE);
  putreg32(0, DA1470X_LCDC_DBIB_CFG);
  putreg32(CRG_SYS_CLK_SYS_LCD_ENABLE, DA1470X_CRG_SYS_RESET_CLK_SYS);

  kmm_free(priv->fbmem);
  priv->fbmem       = NULL;
  priv->initialized = false;
}

#endif /* CONFIG_DA1470X_LCDC */
