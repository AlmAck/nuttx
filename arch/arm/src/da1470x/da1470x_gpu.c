/****************************************************************************
 * arch/arm/src/da1470x/da1470x_gpu.c
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

/* Native driver for the DA1470x 2D GPU (a TES D/AVE 2D core) operated in
 * register mode: the CPU programs the geometry, colour and frame buffer
 * registers and the write to D2_ORIGIN starts the render.  Completion is
 * signalled by the "enumeration finished" interrupt.  Two primitives are
 * offered: an axis-aligned solid fill (no limiters, the bounding box is
 * the rectangle) and a rectangle copy through the texture unit.
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
#include <nuttx/mutex.h>
#include <nuttx/semaphore.h>
#include <nuttx/clock.h>
#include <nuttx/fs/fs.h>

#include "arm_internal.h"
#include "hardware/da1470x_gpu_core.h"
#include "hardware/da1470x_gpu_reg.h"
#include "hardware/da1470x_crg_gpu.h"
#include "da1470x_gpu.h"
#include "da1470x_pmu.h"

#ifdef CONFIG_DA1470X_GPU

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define GPU_TIMEOUT_MS          100

/* The core is an AHB master without the CPU's address remapping and
 * without the flash cache in its path: the boot flash, which the CPU
 * sees at address 0, is reached through the controller's second window.
 */

#define GPU_FLASH_CPU_WINDOW    0x08000000
#define GPU_FLASH_BUS_BASE      0x38000000
#define GPU_IDLE_LOOPS          1000000

#define GPU_BUSY_MASK           (GPU_CORE_D2_STATUS_D2C_BUSY_ENUM | \
                                 GPU_CORE_D2_STATUS_D2C_BUSY_WRITE | \
                                 GPU_CORE_D2_STATUS_D2C_DLISTACTIVE)

#define GPU_IRQ_ENABLE          (GPU_CORE_D2_IRQCTL_D2IRQCTL_ENABLE_FINISH_ENUM | \
                                 GPU_CORE_D2_IRQCTL_D2IRQCTL_ENABLE_BUS_ERROR)
#define GPU_IRQ_CLEAR           (GPU_CORE_D2_IRQCTL_D2IRQCTL_CLR_FINISH_ENUM | \
                                 GPU_CORE_D2_IRQCTL_D2IRQCTL_CLR_BUS_ERROR)

/* 16.16 fixed point one */

#define GPU_FIX16_ONE           0x00010000

/* Burst length limit: 8 words on every master interface */

#define GPU_CONTROL3_BURST8     (GPU_CORE_D2_CONTROL3_BURSTLENGTH_MFBR(3) | \
                                 GPU_CORE_D2_CONTROL3_BURSTLENGTH_MFBW(3) | \
                                 GPU_CORE_D2_CONTROL3_BURSTLENGTH_MTX(3)  | \
                                 GPU_CORE_D2_CONTROL3_BURSTLENGTH_MDL(3))

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct da1470x_gpu_s
{
  mutex_t lock;          /* One render at a time */
  sem_t   done;          /* Render completion */
  bool    initialized;
  bool    buserror;      /* Bus error reported by the last render */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int gpu_ioctl(struct file *filep, int cmd, unsigned long arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct da1470x_gpu_s g_gpu =
{
  .lock = NXMUTEX_INITIALIZER,
  .done = SEM_INITIALIZER(0),
};

static const struct file_operations g_gpu_fops =
{
  NULL,        /* open */
  NULL,        /* close */
  NULL,        /* read */
  NULL,        /* write */
  NULL,        /* seek */
  gpu_ioctl,   /* ioctl */
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: gpu_putreg
 ****************************************************************************/

static inline void gpu_putreg(uint32_t offset, uint32_t value)
{
  putreg32(value, DA1470X_GPU_CORE_BASE + offset);
}

/****************************************************************************
 * Name: gpu_bpp
 *
 * Description:
 *   Bytes per pixel of a GPU pixel format, 0 if unsupported.
 *
 ****************************************************************************/

static unsigned int gpu_bpp(uint8_t format)
{
  switch (format)
    {
      case DA1470X_GPU_FMT_ALPHA8:
        return 1;

      case DA1470X_GPU_FMT_RGB565:
      case DA1470X_GPU_FMT_ARGB4444:
      case DA1470X_GPU_FMT_ARGB1555:
      case DA1470X_GPU_FMT_RGBA4444:
        return 2;

      case DA1470X_GPU_FMT_ARGB8888:
      case DA1470X_GPU_FMT_RGBA8888:
        return 4;

      default:
        return 0;
    }
}

/****************************************************************************
 * Name: gpu_writeformat / gpu_readformat
 *
 * Description:
 *   Encode a format into the scattered D2_CONTROL2 descriptor bits.
 *
 ****************************************************************************/

static uint32_t gpu_writeformat(uint8_t format)
{
  uint32_t bits = 0;

  if ((format & 1) != 0)
    {
      bits |= GPU_CORE_D2_CONTROL2_D2C_WRITEFORMAT1;
    }

  if ((format & 2) != 0)
    {
      bits |= GPU_CORE_D2_CONTROL2_D2C_WRITEFORMAT2;
    }

  if ((format & 4) != 0)
    {
      bits |= GPU_CORE_D2_CONTROL2_D2C_WRITEFORMAT3;
    }

  return bits;
}

static uint32_t gpu_readformat(uint8_t format)
{
  uint32_t bits = 0;

  if ((format & 1) != 0)
    {
      bits |= GPU_CORE_D2_CONTROL2_D2C_READFORMAT1;
    }

  if ((format & 2) != 0)
    {
      bits |= GPU_CORE_D2_CONTROL2_D2C_READFORMAT2;
    }

  if ((format & 4) != 0)
    {
      bits |= GPU_CORE_D2_CONTROL2_D2C_READFORMAT3;
    }

  if ((format & 8) != 0)
    {
      bits |= GPU_CORE_D2_CONTROL2_D2C_READFORMAT4;
    }

  return bits;
}

/****************************************************************************
 * Name: gpu_check_rect
 *
 * Description:
 *   Validate that a rectangle lies inside a surface.
 *
 ****************************************************************************/

static uintptr_t gpu_bus_addr(uintptr_t addr)
{
  if (addr < GPU_FLASH_CPU_WINDOW)
    {
      return addr + GPU_FLASH_BUS_BASE;
    }

  return addr;
}

static bool gpu_check_rect(const struct da1470x_gpu_surface_s *s,
                           uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  if (s->addr == 0 || gpu_bpp(s->format) == 0 || s->pitch < s->width)
    {
      return false;
    }

  if (w == 0 || h == 0 || (uint32_t)x + w > s->width ||
      (uint32_t)y + h > s->height)
    {
      return false;
    }

  return true;
}

/****************************************************************************
 * Name: gpu_interrupt
 ****************************************************************************/

static int gpu_interrupt(int irq, void *context, void *arg)
{
  struct da1470x_gpu_s *priv = (struct da1470x_gpu_s *)arg;
  uint32_t status = getreg32(DA1470X_GPU_CORE_D2_STATUS);

  if ((status & GPU_CORE_D2_STATUS_D2C_IRQ_BUS_ERROR) != 0)
    {
      priv->buserror = true;
    }

  if ((status & (GPU_CORE_D2_STATUS_D2C_IRQ_ENUM |
                 GPU_CORE_D2_STATUS_D2C_IRQ_BUS_ERROR)) != 0)
    {
      /* Clear and keep enabled */

      gpu_putreg(DA1470X_GPU_CORE_D2_IRQCTL_OFFSET,
                 GPU_IRQ_ENABLE | GPU_IRQ_CLEAR);
      nxsem_post(&priv->done);
    }

  return OK;
}

/****************************************************************************
 * Name: gpu_wait_idle
 *
 * Description:
 *   Spin until the enumeration and write-back units are idle.
 *
 ****************************************************************************/

static int gpu_wait_idle(void)
{
  int i;

  for (i = 0; i < GPU_IDLE_LOOPS; i++)
    {
      if ((getreg32(DA1470X_GPU_CORE_D2_STATUS) & GPU_BUSY_MASK) == 0)
        {
          return OK;
        }
    }

  return -ETIMEDOUT;
}

/****************************************************************************
 * Name: gpu_setup
 *
 * Description:
 *   Static core configuration: burst limits, no caches (frame buffers
 *   are read by the LCDC right after a render and sources are rewritten
 *   by the CPU between renders), interrupts on enumeration end and bus
 *   error.
 *
 ****************************************************************************/

static void gpu_setup(void)
{
  gpu_putreg(DA1470X_GPU_CORE_D2_CONTROL3_OFFSET, GPU_CONTROL3_BURST8);
  gpu_putreg(DA1470X_GPU_CORE_D2_CACHECTL_OFFSET, 0);
  gpu_putreg(DA1470X_GPU_CORE_D2_IRQCTL_OFFSET,
             GPU_IRQ_ENABLE | GPU_IRQ_CLEAR);
}

/****************************************************************************
 * Name: gpu_reset
 *
 * Description:
 *   A bus error or a hung enumeration leaves the core busy for good.
 *   Cycle its enable to start over.
 *
 ****************************************************************************/

static void gpu_reset(void)
{
  modifyreg32(DA1470X_GPU_REG_GPU_CTRL, GPU_REG_GPU_CTRL_GPU_EN, 0);
  up_udelay(10);
  modifyreg32(DA1470X_GPU_REG_GPU_CTRL, 0, GPU_REG_GPU_CTRL_GPU_EN);
  gpu_setup();
}

/****************************************************************************
 * Name: gpu_render
 *
 * Description:
 *   Program the frame buffer target, kick the render by writing the
 *   origin and wait for the enumeration to finish.  The lock must be held.
 *
 ****************************************************************************/

static int gpu_render(struct da1470x_gpu_s *priv,
                      const struct da1470x_gpu_surface_s *dst,
                      uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  uintptr_t origin;
  int ret;

  ret = gpu_wait_idle();
  if (ret < 0)
    {
      gerr("GPU busy before render, resetting\n");
      gpu_reset();
      return ret;
    }

  origin = gpu_bus_addr(dst->addr) +
           ((uintptr_t)y * dst->pitch + x) * gpu_bpp(dst->format);


  priv->buserror = false;

  /* Drain any stale completion */

  while (nxsem_trywait(&priv->done) == OK)
    {
    }

  gpu_putreg(DA1470X_GPU_CORE_D2_PITCH_OFFSET, dst->pitch);
  gpu_putreg(DA1470X_GPU_CORE_D2_SIZE_OFFSET, ((uint32_t)h << 16) | w);
  gpu_putreg(DA1470X_GPU_CORE_D2_ORIGIN_OFFSET, (uint32_t)origin);

  ret = nxsem_tickwait_uninterruptible(&priv->done,
                                       MSEC2TICK(GPU_TIMEOUT_MS));
  if (ret < 0)
    {
      gerr("GPU render timed out, status %08" PRIx32 "\n",
           getreg32(DA1470X_GPU_CORE_D2_STATUS));
      gpu_reset();
      return ret;
    }

  if (priv->buserror)
    {
      gerr("GPU bus error\n");
      gpu_reset();
      return -EIO;
    }

  ret = gpu_wait_idle();
  if (ret < 0)
    {
      gpu_reset();
    }

  return ret;
}

/****************************************************************************
 * Name: gpu_ioctl
 ****************************************************************************/

static int gpu_ioctl(struct file *filep, int cmd, unsigned long arg)
{
  int ret;

  switch (cmd)
    {
      case GPUIOC_REVISION:
        {
          uint32_t *rev = (uint32_t *)arg;

          if (rev == NULL)
            {
              return -EINVAL;
            }

          *rev = da1470x_gpu_revision();
          ret  = OK;
        }
        break;

      case GPUIOC_FILL:
        ret = da1470x_gpu_fill((const struct da1470x_gpu_fill_s *)arg);
        break;

      case GPUIOC_BLIT:
        ret = da1470x_gpu_blit((const struct da1470x_gpu_blit_s *)arg);
        break;

      case GPUIOC_WAIT:
        ret = nxmutex_lock(&g_gpu.lock);
        if (ret == OK)
          {
            ret = gpu_wait_idle();
            nxmutex_unlock(&g_gpu.lock);
          }
        break;

      default:
        ret = -ENOTTY;
        break;
    }

  return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_gpu_initialize
 ****************************************************************************/

int da1470x_gpu_initialize(void)
{
  struct da1470x_gpu_s *priv = &g_gpu;
  int ret;

  if (priv->initialized)
    {
      return OK;
    }

  ret = da1470x_pd_enable(DA1470X_PD_GPU);
  if (ret < 0)
    {
      return ret;
    }

  modifyreg32(DA1470X_CRG_GPU_CLK_GPU, 0, CRG_GPU_CLK_GPU_GPU_ENABLE);
  modifyreg32(DA1470X_GPU_REG_GPU_CTRL, 0, GPU_REG_GPU_CTRL_GPU_EN);

  if (da1470x_gpu_revision() == 0 || da1470x_gpu_revision() == 0xffffffff)
    {
      gerr("GPU does not answer\n");
      return -ENODEV;
    }

  irq_attach(DA1470X_IRQ_GPU, gpu_interrupt, priv);
  up_enable_irq(DA1470X_IRQ_GPU);
  gpu_setup();

  priv->initialized = true;
  return OK;
}

/****************************************************************************
 * Name: da1470x_gpu_revision
 ****************************************************************************/

uint32_t da1470x_gpu_revision(void)
{
  return getreg32(DA1470X_GPU_CORE_D2_HWREVISION);
}

/****************************************************************************
 * Name: da1470x_gpu_fill
 *
 * Description:
 *   Solid rectangle: no limiters (every pixel of the bounding box is
 *   inside), colour from D2_COLOR1, source factor one and destination
 *   factor zero so the pixel is replaced.
 *
 ****************************************************************************/

int da1470x_gpu_fill(const struct da1470x_gpu_fill_s *op)
{
  struct da1470x_gpu_s *priv = &g_gpu;
  uint32_t control2;
  int ret;

  if (op == NULL || !priv->initialized ||
      !gpu_check_rect(&op->dst, op->x, op->y, op->w, op->h))
    {
      return -EINVAL;
    }

  ret = nxmutex_lock(&priv->lock);
  if (ret < 0)
    {
      return ret;
    }

  control2 = gpu_writeformat(op->dst.format) |
             GPU_CORE_D2_CONTROL2_D2C_WRITEALPHA1 |
             GPU_CORE_D2_CONTROL2_D2C_BDI;

  gpu_putreg(DA1470X_GPU_CORE_D2_CONTROL_OFFSET, 0);
  gpu_putreg(DA1470X_GPU_CORE_D2_CONTROL2_OFFSET, control2);
  gpu_putreg(DA1470X_GPU_CORE_D2_COLOR1_OFFSET, op->color);
  gpu_putreg(DA1470X_GPU_CORE_D2_COLOR2_OFFSET, op->color);

  ret = gpu_render(priv, &op->dst, op->x, op->y, op->w, op->h);

  nxmutex_unlock(&priv->lock);
  return ret;
}

/****************************************************************************
 * Name: da1470x_gpu_blit
 *
 * Description:
 *   Rectangle copy: the texture unit samples the source with the U and V
 *   limiters stepping one texel per pixel, clamped to the rectangle, and
 *   COLOR1 = 0 / COLOR2 = 1 leave the texel colour unchanged.
 *
 ****************************************************************************/

int da1470x_gpu_blit(const struct da1470x_gpu_blit_s *op)
{
  struct da1470x_gpu_s *priv = &g_gpu;
  uintptr_t texorigin;
  uint32_t control2;
  int ret;

  if (op == NULL || !priv->initialized ||
      !gpu_check_rect(&op->src, op->sx, op->sy, op->w, op->h) ||
      !gpu_check_rect(&op->dst, op->dx, op->dy, op->w, op->h))
    {
      return -EINVAL;
    }

  ret = nxmutex_lock(&priv->lock);
  if (ret < 0)
    {
      return ret;
    }

  texorigin = gpu_bus_addr(op->src.addr) +
              ((uintptr_t)op->sy * op->src.pitch + op->sx) *
              gpu_bpp(op->src.format);

  control2 = gpu_writeformat(op->dst.format) |
             gpu_readformat(op->src.format) |
             GPU_CORE_D2_CONTROL2_D2C_TEXTUREENABLE |
             GPU_CORE_D2_CONTROL2_D2C_TEXTURECLAMPX |
             GPU_CORE_D2_CONTROL2_D2C_TEXTURECLAMPY |
             GPU_CORE_D2_CONTROL2_D2C_WRITEALPHA1 |
             GPU_CORE_D2_CONTROL2_D2C_BDI;

  /* The colour part of COLOR1 is added to the texel and must stay zero.
   * Its alpha is ignored for textured boxes: this core has no texture
   * operation unit, so a constant opacity cannot be applied on top of
   * the source alpha.
   */

  gpu_putreg(DA1470X_GPU_CORE_D2_CONTROL_OFFSET, 0);
  gpu_putreg(DA1470X_GPU_CORE_D2_CONTROL2_OFFSET, control2);
  gpu_putreg(DA1470X_GPU_CORE_D2_COLOR1_OFFSET, 0x00000000);
  gpu_putreg(DA1470X_GPU_CORE_D2_COLOR2_OFFSET, 0xffffffff);

  /* Texture: base, pitch, and clamp limits */

  gpu_putreg(DA1470X_GPU_CORE_D2_TEXORIGIN_OFFSET, (uint32_t)texorigin);
  gpu_putreg(DA1470X_GPU_CORE_D2_TEXPITCH_OFFSET, op->src.pitch);
  gpu_putreg(DA1470X_GPU_CORE_D2_TEXMASK_OFFSET,
             ((uint32_t)(op->h - 1) << 11) | ((op->w - 1) & 0x7ff));

  /* U runs along x, V along y, one texel per pixel */

  gpu_putreg(DA1470X_GPU_CORE_D2_LUSTART_OFFSET, 0);
  gpu_putreg(DA1470X_GPU_CORE_D2_LUXADD_OFFSET, GPU_FIX16_ONE);
  gpu_putreg(DA1470X_GPU_CORE_D2_LUYADD_OFFSET, 0);
  gpu_putreg(DA1470X_GPU_CORE_D2_LVSTARTI_OFFSET, 0);
  gpu_putreg(DA1470X_GPU_CORE_D2_LVSTARTF_OFFSET, 0);
  gpu_putreg(DA1470X_GPU_CORE_D2_LVXADDI_OFFSET, 0);
  gpu_putreg(DA1470X_GPU_CORE_D2_LVYADDI_OFFSET, 1);
  gpu_putreg(DA1470X_GPU_CORE_D2_LVYXADDF_OFFSET, 0);

  ret = gpu_render(priv, &op->dst, op->dx, op->dy, op->w, op->h);

  nxmutex_unlock(&priv->lock);
  return ret;
}

/****************************************************************************
 * Name: da1470x_gpu_register
 ****************************************************************************/

int da1470x_gpu_register(const char *devpath)
{
  int ret = da1470x_gpu_initialize();

  if (ret < 0)
    {
      return ret;
    }

  return register_driver(devpath, &g_gpu_fops, 0666, &g_gpu);
}

#endif /* CONFIG_DA1470X_GPU */
