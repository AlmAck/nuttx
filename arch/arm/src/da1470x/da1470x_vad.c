/****************************************************************************
 * arch/arm/src/da1470x/da1470x_vad.c
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

/* The voice activity detector is a small always-on block with its own
 * analogue front end.  It tracks the background noise and raises an
 * interrupt when it hears something enough decibels above it for long
 * enough.  It is not part of the audio power domain and needs no system
 * clock, which is what lets it listen while everything else sleeps.
 *
 * Two things about the hardware shape this driver.  Its settings may only
 * be changed while it is standing by or asleep, never while it listens;
 * and a detection puts it to sleep by itself, so it has to be told to
 * listen again afterwards.  The interrupt handler does that, which means a
 * reader can miss detections but the block never stops listening.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <poll.h>
#include <errno.h>
#include <debug.h>

#include <nuttx/arch.h>
#include <nuttx/clock.h>
#include <nuttx/irq.h>
#include <nuttx/kmalloc.h>
#include <nuttx/mutex.h>
#include <nuttx/semaphore.h>
#include <nuttx/fs/fs.h>

#include "arm_internal.h"
#include "da1470x_vad.h"
#include "da1470x_pdc.h"
#include "hardware/da1470x_crg_top.h"
#include "hardware/da1470x_vad.h"

#ifdef CONFIG_DA1470X_VAD

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define VAD_NEVENTS  CONFIG_DA1470X_VAD_NEVENTS

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct da1470x_vad_dev_s
{
  mutex_t  lock;                    /* Guards everything below */
  sem_t    wait;                    /* Readers wait here */

  struct vad_config_s cfg;          /* Current tuning */
  uint8_t  mode;                    /* VAD_MODE_* */
  int      pdc_entry;               /* Wake-up entry, or invalid */
  bool     wakeup;                  /* Listening should wake the system */

  struct vad_event_s ring[VAD_NEVENTS];
  uint8_t  head;                    /* Where the next detection goes */
  uint8_t  tail;                    /* Where the next read comes from */
  uint8_t  count;                   /* Detections not yet read */
  uint8_t  overrun;                 /* Dropped since the last delivery */

  bool     opened;
  struct pollfd *fds[CONFIG_DA1470X_VAD_NPOLLWAITERS];
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int     vad_open(struct file *filep);
static int     vad_close(struct file *filep);
static ssize_t vad_read(struct file *filep, char *buffer, size_t buflen);
static int     vad_ioctl(struct file *filep, int cmd, unsigned long arg);
static int     vad_poll(struct file *filep, struct pollfd *fds, bool setup);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations g_vad_fops =
{
  vad_open,      /* open */
  vad_close,     /* close */
  vad_read,      /* read */
  NULL,          /* write */
  NULL,          /* seek */
  vad_ioctl,     /* ioctl */
  NULL,          /* mmap */
  NULL,          /* truncate */
  vad_poll       /* poll */
};

static struct da1470x_vad_dev_s g_vad =
{
  .lock      = NXMUTEX_INITIALIZER,
  .pdc_entry = -1,
  .wakeup    = true
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: vad_update_wakeup
 *
 * Description:
 *   Keep the power domain controller entry in step with what the detector
 *   is actually doing.  A block that is listening should be able to bring
 *   the system back, and one that is not should not be holding an entry:
 *   the table has sixteen of them, shared with the other two processors.
 *
 *   Registering the wake-up next to the thing that arms it is deliberate.
 *   The two halves of a wake-up source kept in separate places is exactly
 *   how the buttons came to have an entry with nothing behind it and the
 *   touch controller to have the reverse.
 *
 *   No crystal is asked for.  The detector runs from the low power clock,
 *   so the wake-up does not need the 32 MHz oscillator started, and not
 *   asking for it is what makes this a cheap way to be woken.
 *
 *   The lock must be held.
 *
 ****************************************************************************/

static void vad_update_wakeup(struct da1470x_vad_dev_s *priv)
{
  bool want = priv->wakeup && priv->mode == VAD_MODE_LISTENING;

  if (want && priv->pdc_entry < 0)
    {
      int entry = da1470x_pdc_add(DA1470X_PDC_TRIG_PERIPHERAL,
                                  DA1470X_PDC_PERIPH_VAD,
                                  DA1470X_PDC_MASTER_CM33, 0);
      if (entry < 0)
        {
          awarn("no PDC entry: the detector will not wake the system\n");
        }
      else
        {
          priv->pdc_entry = entry;
        }
    }
  else if (!want && priv->pdc_entry >= 0)
    {
      da1470x_pdc_remove(priv->pdc_entry);
      priv->pdc_entry = -1;
    }
}

/****************************************************************************
 * Name: vad_write_mode
 *
 * Description:
 *   Put the block in one of its three modes.  Both bits live in the same
 *   register and are written together, as the hardware expects.
 *
 ****************************************************************************/

static void vad_write_mode(struct da1470x_vad_dev_s *priv, uint8_t mode)
{
  uint32_t regval = getreg32(DA1470X_VAD_CTRL3);

  regval &= ~(VAD_CTRL3_SB | VAD_CTRL3_SLEEP);

  switch (mode)
    {
      case VAD_MODE_LISTENING:
        break;                        /* Both bits clear */

      case VAD_MODE_SLEEP:
        regval |= VAD_CTRL3_SLEEP;
        break;

      case VAD_MODE_STANDBY:
      default:
        regval |= VAD_CTRL3_SB;
        mode = VAD_MODE_STANDBY;
        break;
    }

  putreg32(regval, DA1470X_VAD_CTRL3);
  priv->mode = mode;

  vad_update_wakeup(priv);
}

/****************************************************************************
 * Name: vad_apply
 *
 * Description:
 *   Write the tuning into the block.  The caller must have put it in
 *   standby or sleep first: the hardware ignores these registers, or
 *   misbehaves, while it is listening.
 *
 ****************************************************************************/

static void vad_apply(struct da1470x_vad_dev_s *priv)
{
  const struct vad_config_s *cfg = &priv->cfg;
  uint32_t regval;

  /* Clock source lives with the rest of the clock tree, not in the block */

  regval = getreg32(DA1470X_CRG_TOP_CLK_CTRL);
  if (cfg->clksrc == VAD_CLK_XTAL32K)
    {
      regval |= CRG_TOP_CLK_CTRL_VAD_CLK_SEL;
    }
  else
    {
      regval &= ~CRG_TOP_CLK_CTRL_VAD_CLK_SEL;
    }

  putreg32(regval, DA1470X_CRG_TOP_CLK_CTRL);

  regval  = getreg32(DA1470X_VAD_CTRL3);
  regval &= ~VAD_CTRL3_MCLK_DIV_MASK;
  regval |= VAD_CTRL3_MCLK_DIV(cfg->mclk_div);
  putreg32(regval, DA1470X_VAD_CTRL3);

  /* A level interrupt would keep firing until the block is quiet again;
   * take the pulse instead, one per detection.
   */

  regval  = getreg32(DA1470X_VAD_CTRL4);
  regval |= VAD_CTRL4_IRQ_MODE;
  putreg32(regval, DA1470X_VAD_CTRL4);

  putreg32(VAD_CTRL0_VTRACK(cfg->voice_track) |
           VAD_CTRL0_NTRACK(cfg->noise_track) |
           VAD_CTRL0_PWR_LVL_SNSTVTY(cfg->sensitivity),
           DA1470X_VAD_CTRL0);

  putreg32(VAD_CTRL1_MINDELAY(cfg->mindelay) |
           VAD_CTRL1_MINEVENT(cfg->minevent),
           DA1470X_VAD_CTRL1);

  putreg32(VAD_CTRL2_NFI_DET(cfg->nfi_thres), DA1470X_VAD_CTRL2);
}

/****************************************************************************
 * Name: vad_notify
 *
 * Description:
 *   Wake a blocked reader and anyone polling.
 *
 ****************************************************************************/

static void vad_notify(struct da1470x_vad_dev_s *priv)
{
  int i;

  for (i = 0; i < CONFIG_DA1470X_VAD_NPOLLWAITERS; i++)
    {
      if (priv->fds[i] != NULL)
        {
          poll_notify(&priv->fds[i], 1, POLLIN);
        }
    }

  nxsem_post(&priv->wait);
}

/****************************************************************************
 * Name: vad_interrupt
 *
 * Description:
 *   A detection.  The block has put itself to sleep, so record what
 *   happened, clear the flag and set it listening again.
 *
 ****************************************************************************/

static int vad_interrupt(int irq, void *context, void *arg)
{
  struct da1470x_vad_dev_s *priv = (struct da1470x_vad_dev_s *)arg;
  struct vad_event_s *evt;

  putreg32(getreg32(DA1470X_VAD_CTRL4) | VAD_CTRL4_IRQ_FLAG,
           DA1470X_VAD_CTRL4);

  if (priv->count < VAD_NEVENTS)
    {
      evt = &priv->ring[priv->head];
      evt->timestamp = (uint32_t)clock_systime_ticks();
      evt->noise     = getreg32(DA1470X_VAD_STATUS) &
                       VAD_STATUS_NFI_RD_MASK;
      evt->overrun   = priv->overrun;

      priv->overrun  = 0;
      priv->head     = (priv->head + 1) % VAD_NEVENTS;
      priv->count++;
    }
  else if (priv->overrun < UINT8_MAX)
    {
      priv->overrun++;
    }

  /* The detection has put the block to sleep and it stays there.  Who
   * listens again, and when, is the caller's decision: a sound that
   * stays above the threshold would otherwise be reported over and over
   * for as long as it lasts, which is no use to anything waiting for
   * one sound to act on.  Record the state the hardware is now in.
   */

  priv->mode = VAD_MODE_SLEEP;

  vad_notify(priv);
  return OK;
}

/****************************************************************************
 * Name: vad_open
 ****************************************************************************/

static int vad_open(struct file *filep)
{
  struct da1470x_vad_dev_s *priv = filep->f_inode->i_private;

  nxmutex_lock(&priv->lock);

  if (priv->opened)
    {
      nxmutex_unlock(&priv->lock);
      return -EBUSY;
    }

  priv->opened = true;
  nxmutex_unlock(&priv->lock);
  return OK;
}

/****************************************************************************
 * Name: vad_close
 ****************************************************************************/

static int vad_close(struct file *filep)
{
  struct da1470x_vad_dev_s *priv = filep->f_inode->i_private;
  irqstate_t flags;

  nxmutex_lock(&priv->lock);

  flags = enter_critical_section();
  vad_write_mode(priv, VAD_MODE_STANDBY);
  priv->head    = 0;
  priv->tail    = 0;
  priv->count   = 0;
  priv->overrun = 0;
  leave_critical_section(flags);

  priv->opened  = false;

  nxmutex_unlock(&priv->lock);
  return OK;
}

/****************************************************************************
 * Name: vad_read
 *
 * Description:
 *   Hand back whole detections, waiting for one if the caller allows it.
 *
 ****************************************************************************/

static ssize_t vad_read(struct file *filep, char *buffer, size_t buflen)
{
  struct da1470x_vad_dev_s *priv = filep->f_inode->i_private;
  irqstate_t flags;
  size_t nread = 0;
  int ret;

  if (buflen < sizeof(struct vad_event_s))
    {
      return -EINVAL;
    }

  nxmutex_lock(&priv->lock);

  while (priv->count == 0)
    {
      nxmutex_unlock(&priv->lock);

      if ((filep->f_oflags & O_NONBLOCK) != 0)
        {
          return -EAGAIN;
        }

      ret = nxsem_wait(&priv->wait);
      if (ret < 0)
        {
          return ret;
        }

      nxmutex_lock(&priv->lock);
    }

  flags = enter_critical_section();

  while (priv->count > 0 && buflen - nread >= sizeof(struct vad_event_s))
    {
      memcpy(buffer + nread, &priv->ring[priv->tail],
             sizeof(struct vad_event_s));

      priv->tail = (priv->tail + 1) % VAD_NEVENTS;
      priv->count--;
      nread += sizeof(struct vad_event_s);
    }

  leave_critical_section(flags);
  nxmutex_unlock(&priv->lock);
  return nread;
}

/****************************************************************************
 * Name: vad_ioctl
 ****************************************************************************/

static int vad_ioctl(struct file *filep, int cmd, unsigned long arg)
{
  struct da1470x_vad_dev_s *priv = filep->f_inode->i_private;
  int ret = OK;

  nxmutex_lock(&priv->lock);

  switch (cmd)
    {
      case VADIOC_SETCONFIG:
        {
          const struct vad_config_s *cfg =
            (const struct vad_config_s *)arg;
          uint8_t mode = priv->mode;

          if (cfg == NULL)
            {
              ret = -EINVAL;
              break;
            }

          /* Settings only take while the block is not listening */

          vad_write_mode(priv, VAD_MODE_STANDBY);
          priv->cfg = *cfg;
          vad_apply(priv);
          vad_write_mode(priv, mode);
        }
        break;

      case VADIOC_GETCONFIG:
        {
          struct vad_config_s *cfg = (struct vad_config_s *)arg;

          if (cfg == NULL)
            {
              ret = -EINVAL;
              break;
            }

          *cfg = priv->cfg;
        }
        break;

      case VADIOC_SETMODE:
        if (arg > VAD_MODE_LISTENING)
          {
            ret = -EINVAL;
            break;
          }

        vad_write_mode(priv, (uint8_t)arg);
        break;

      case VADIOC_GETMODE:
        if (arg == 0)
          {
            ret = -EINVAL;
            break;
          }

        *(int *)arg = priv->mode;
        break;

      case VADIOC_SETWAKEUP:
        priv->wakeup = (arg != 0);
        vad_update_wakeup(priv);
        break;

      case VADIOC_GETWAKEUP:
        if (arg == 0)
          {
            ret = -EINVAL;
            break;
          }

        *(int *)arg = priv->pdc_entry >= 0;
        break;

      case VADIOC_GETNOISE:
        if (arg == 0)
          {
            ret = -EINVAL;
            break;
          }

        *(uint32_t *)arg = getreg32(DA1470X_VAD_STATUS) &
                           VAD_STATUS_NFI_RD_MASK;
        break;

      case VADIOC_FLUSH:
        {
          irqstate_t flags = enter_critical_section();

          priv->head    = 0;
          priv->tail    = 0;
          priv->count   = 0;
          priv->overrun = 0;
          leave_critical_section(flags);
        }
        break;

      default:
        ret = -ENOTTY;
        break;
    }

  nxmutex_unlock(&priv->lock);
  return ret;
}

/****************************************************************************
 * Name: vad_poll
 ****************************************************************************/

static int vad_poll(struct file *filep, struct pollfd *fds, bool setup)
{
  struct da1470x_vad_dev_s *priv = filep->f_inode->i_private;
  int ret = OK;
  int i;

  nxmutex_lock(&priv->lock);

  if (!setup)
    {
      if (fds->priv != NULL)
        {
          *((struct pollfd **)fds->priv) = NULL;
          fds->priv = NULL;
        }

      nxmutex_unlock(&priv->lock);
      return OK;
    }

  for (i = 0; i < CONFIG_DA1470X_VAD_NPOLLWAITERS; i++)
    {
      if (priv->fds[i] == NULL)
        {
          priv->fds[i] = fds;
          fds->priv    = &priv->fds[i];
          break;
        }
    }

  if (i >= CONFIG_DA1470X_VAD_NPOLLWAITERS)
    {
      fds->priv = NULL;
      ret = -EBUSY;
    }
  else
    {
      irqstate_t flags = enter_critical_section();
      bool ready = priv->count > 0;

      leave_critical_section(flags);

      if (ready)
        {
          poll_notify(&fds, 1, POLLIN);
        }
    }

  nxmutex_unlock(&priv->lock);
  return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_vad_register
 ****************************************************************************/

int da1470x_vad_register(const char *devpath,
                         const struct vad_config_s *config)
{
  struct da1470x_vad_dev_s *priv = &g_vad;
  int ret;

  DEBUGASSERT(devpath != NULL);

  nxsem_init(&priv->wait, 0, 0);

  if (config != NULL)
    {
      priv->cfg = *config;
    }
  else
    {
      /* What the block itself resets to: the low power oscillator
       * undivided, a moderate threshold, and a short settling time.
       */

      priv->cfg.clksrc      = VAD_CLK_RCLP32K;
      priv->cfg.mclk_div    = VAD_MCLK_DIV_1;
      priv->cfg.sensitivity = VAD_SENSITIVITY_6DB;
      priv->cfg.minevent    = VAD_MINEVENT_32;
      priv->cfg.mindelay    = VAD_MINDELAY_1536;
      priv->cfg.noise_track = 4;
      priv->cfg.voice_track = 1;
      priv->cfg.nfi_thres   = 0x27;
    }

  vad_write_mode(priv, VAD_MODE_STANDBY);
  vad_apply(priv);

  ret = irq_attach(DA1470X_IRQ_VAD, vad_interrupt, priv);
  if (ret < 0)
    {
      aerr("ERROR: cannot attach the detector interrupt: %d\n", ret);
      return ret;
    }

  up_enable_irq(DA1470X_IRQ_VAD);

  ret = register_driver(devpath, &g_vad_fops, 0666, priv);
  if (ret < 0)
    {
      up_disable_irq(DA1470X_IRQ_VAD);
      irq_detach(DA1470X_IRQ_VAD);
      aerr("ERROR: cannot create %s: %d\n", devpath, ret);
      return ret;
    }

  ainfo("Voice activity detector at %s\n", devpath);
  return OK;
}

/****************************************************************************
 * Name: da1470x_vad_wakeup_enable
 ****************************************************************************/

int da1470x_vad_wakeup_enable(bool enable)
{
  struct da1470x_vad_dev_s *priv = &g_vad;

  nxmutex_lock(&priv->lock);

  priv->wakeup = enable;
  vad_update_wakeup(priv);

  nxmutex_unlock(&priv->lock);
  return OK;
}

#endif /* CONFIG_DA1470X_VAD */
