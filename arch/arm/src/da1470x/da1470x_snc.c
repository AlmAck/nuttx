/****************************************************************************
 * arch/arm/src/da1470x/da1470x_snc.c
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

/* The sensor node controller is the third processor on this part: a
 * Cortex-M0+ at up to 32 MHz that reaches every peripheral and keeps
 * running while the M33 sleeps.  What it is for is the work that would
 * otherwise wake the M33 for no good reason: a wake of the M33 was
 * measured on this board at about 39 uC, so polling at a hundred hertz from
 * the M33 costs milliamps before anything is even read.
 *
 * This driver loads the controller's firmware -- built from
 * arch/arm/src/da1470x/snc/ as part of the NuttX build and embedded in the
 * image -- starts it, and carries messages both ways through RAM8 (see
 * da1470x_snc_ipc.h).  The controller rings the SNC2SYS doorbell when it
 * has queued something; the interrupt hands the ring to the low priority
 * work queue, which gives each message to the kernel handler registered for
 * its type, or queues it for readers of the character device.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <debug.h>

#include <nuttx/arch.h>
#include <nuttx/irq.h>
#include <nuttx/fs/fs.h>
#include <nuttx/mutex.h>
#include <nuttx/semaphore.h>
#include <nuttx/wqueue.h>

#include <arch/chip/snc.h>

#include "arm_internal.h"
#include "hardware/da1470x_crg_top.h"
#include "hardware/da1470x_crg_xtal.h"
#include "hardware/da1470x_gpreg.h"
#include "da1470x_clockconfig.h"
#include "da1470x_pmu.h"
#include "da1470x_snc.h"
#include "da1470x_snc_ipc.h"

#ifdef CONFIG_DA1470X_PDC
#  include "da1470x_pdc.h"
#endif

#ifdef CONFIG_DA1470X_SNC

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define DA1470X_SNC_STATUS      (DA1470X_SNC_BASE + 0x0000)

#define SNC_BOOT_TIMEOUT_MS     100
#define SNC_NHANDLERS           8
#define SNC_NPOLLWAITERS        2
#define SNC_RXQ_LEN             CONFIG_DA1470X_SNC_RXQUEUE

#define g_shared ((struct snc_shared_s *)SNC_SHARED_M33_BASE)

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct snc_handler_s
{
  uint16_t       type;
  snc_handler_t  handler;
  void          *arg;
};

struct snc_dev_s
{
  mutex_t              lock;
  sem_t                rxsem;       /* Readers waiting for a message */
  int                  nwaiters;
  struct work_s        work;        /* Drains the event ring */
  bool                 running;
  bool                 fault_reported;
  uint32_t             dropped_to_snc;
  bool                 rx_blocked;  /* Queue full, events left in RAM8 */
  uint8_t              rxhead;
  uint8_t              rxcount;
  struct snc_msg_s     rxq[SNC_RXQ_LEN];
  struct snc_handler_s handlers[SNC_NHANDLERS];
  FAR struct pollfd   *fds[SNC_NPOLLWAITERS];
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static ssize_t snc_read(FAR struct file *filep, FAR char *buffer,
                        size_t buflen);
static ssize_t snc_write(FAR struct file *filep, FAR const char *buffer,
                         size_t buflen);
static int     snc_ioctl(FAR struct file *filep, int cmd,
                         unsigned long arg);
static int     snc_poll(FAR struct file *filep, FAR struct pollfd *fds,
                        bool setup);

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* The firmware image, from da1470x_snc_image.S */

extern const uint8_t g_da1470x_snc_image[];
extern const uint8_t g_da1470x_snc_image_end[];

static struct snc_dev_s g_snc =
{
  .lock  = NXMUTEX_INITIALIZER,
  .rxsem = SEM_INITIALIZER(0),
};

static const struct file_operations g_snc_fops =
{
  NULL,           /* open */
  NULL,           /* close */
  snc_read,       /* read */
  snc_write,      /* write */
  NULL,           /* seek */
  snc_ioctl,      /* ioctl */
  NULL,           /* mmap */
  NULL,           /* truncate */
  snc_poll        /* poll */
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void snc_hold_reset(void)
{
  modifyreg32(DA1470X_CRG_TOP_CLK_SNC_CTRL, 0,
              CRG_TOP_CLK_SNC_CTRL_SNC_RESET_REQ);
}

static void snc_doorbell(void)
{
  putreg32(CRG_XTAL_SET_SYS_IRQ_CTRL_SYS2SNC_IRQ_BIT,
           DA1470X_CRG_XTAL_SET_SYS_IRQ_CTRL);
}

/* Readers and pollers learn about a new message.  The lock is held. */

static void snc_notify(FAR struct snc_dev_s *priv)
{
  poll_notify(priv->fds, SNC_NPOLLWAITERS, POLLIN);

  while (priv->nwaiters > 0)
    {
      priv->nwaiters--;
      nxsem_post(&priv->rxsem);
    }
}

static snc_handler_t snc_find_handler(FAR struct snc_dev_s *priv,
                                      uint16_t type, FAR void **arg)
{
  int i;

  for (i = 0; i < SNC_NHANDLERS; i++)
    {
      if (priv->handlers[i].handler != NULL &&
          priv->handlers[i].type == type)
        {
          *arg = priv->handlers[i].arg;
          return priv->handlers[i].handler;
        }
    }

  return NULL;
}

/****************************************************************************
 * Name: snc_worker
 *
 * Description:
 *   Drain the controller's event ring: each message goes to its handler,
 *   or into the queue for /dev/snc0.  Nothing is dropped here: when that
 *   queue is full the rest stays in the ring, where the controller sees
 *   the ring fill and holds back, and draining resumes once a reader has
 *   made room.  The controller is told whenever space was freed.
 *
 ****************************************************************************/

static void snc_worker(FAR void *arg)
{
  FAR struct snc_dev_s *priv = arg;
  struct snc_msg_s msg;
  snc_handler_t handler;
  FAR void *harg;
  bool consumed = false;
  int n;

  if (g_shared->state == SNC_STATE_FAULT && !priv->fault_reported)
    {
      priv->fault_reported = true;
      snerr("SNC fault: PC %08" PRIx32 " LR %08" PRIx32 "\n",
            g_shared->fault_pc, g_shared->fault_lr);
    }

  for (; ; )
    {
      nxmutex_lock(&priv->lock);

      if (snc_ring_peek(&g_shared->to_m33, &msg.type) < 0)
        {
          nxmutex_unlock(&priv->lock);
          break;
        }

      handler = snc_find_handler(priv, msg.type, &harg);
      if (handler == NULL && priv->rxcount >= SNC_RXQ_LEN)
        {
          priv->rx_blocked = true;
          nxmutex_unlock(&priv->lock);
          break;
        }

      n = snc_ring_get(&g_shared->to_m33, &msg.type, msg.payload,
                       sizeof(msg.payload));
      msg.len = n;
      consumed = true;

      if (handler == NULL)
        {
          int slot = (priv->rxhead + priv->rxcount) % SNC_RXQ_LEN;

          memcpy(&priv->rxq[slot], &msg, sizeof(msg));
          priv->rxcount++;
          snc_notify(priv);
        }

      nxmutex_unlock(&priv->lock);

      if (handler != NULL)
        {
          handler(msg.type, msg.payload, msg.len, harg);
        }
    }

  if (consumed && priv->running)
    {
      snc_doorbell();
    }
}

/****************************************************************************
 * Name: snc_interrupt
 ****************************************************************************/

static int snc_interrupt(int irq, FAR void *context, FAR void *arg)
{
  FAR struct snc_dev_s *priv = arg;

  putreg32(CRG_XTAL_RESET_SYS_IRQ_CTRL_SNC2SYS_IRQ_BIT,
           DA1470X_CRG_XTAL_RESET_SYS_IRQ_CTRL);

  if (work_available(&priv->work))
    {
      work_queue(LPWORK, &priv->work, snc_worker, priv, 0);
    }

  return OK;
}

/****************************************************************************
 * Name: snc_read
 ****************************************************************************/

static ssize_t snc_read(FAR struct file *filep, FAR char *buffer,
                        size_t buflen)
{
  FAR struct snc_dev_s *priv = &g_snc;
  FAR struct snc_msg_s *msg;
  size_t n;
  int ret;

  if (buflen < sizeof(struct snc_msg_hdr_s))
    {
      return -EINVAL;
    }

  for (; ; )
    {
      ret = nxmutex_lock(&priv->lock);
      if (ret < 0)
        {
          return ret;
        }

      if (priv->rxcount > 0)
        {
          break;
        }

      if (filep->f_oflags & O_NONBLOCK)
        {
          nxmutex_unlock(&priv->lock);
          return -EAGAIN;
        }

      priv->nwaiters++;
      nxmutex_unlock(&priv->lock);

      ret = nxsem_wait(&priv->rxsem);
      if (ret < 0)
        {
          nxmutex_lock(&priv->lock);
          if (priv->nwaiters > 0)
            {
              priv->nwaiters--;
            }

          nxmutex_unlock(&priv->lock);
          return ret;
        }
    }

  /* One message per read; a short buffer truncates its payload */

  msg = &priv->rxq[priv->rxhead];
  n = sizeof(struct snc_msg_hdr_s) + msg->len;
  if (n > buflen)
    {
      n = buflen;
    }

  memcpy(buffer, msg, n);
  priv->rxhead = (priv->rxhead + 1) % SNC_RXQ_LEN;
  priv->rxcount--;

  /* Room again: go back for what was left in the ring */

  if (priv->rx_blocked)
    {
      priv->rx_blocked = false;
      if (work_available(&priv->work))
        {
          work_queue(LPWORK, &priv->work, snc_worker, priv, 0);
        }
    }

  nxmutex_unlock(&priv->lock);
  return n;
}

/****************************************************************************
 * Name: snc_write
 ****************************************************************************/

static ssize_t snc_write(FAR struct file *filep, FAR const char *buffer,
                         size_t buflen)
{
  FAR const struct snc_msg_s *msg = (FAR const struct snc_msg_s *)buffer;
  int ret;

  if (buflen < sizeof(struct snc_msg_hdr_s) ||
      buflen - sizeof(struct snc_msg_hdr_s) < msg->len)
    {
      return -EINVAL;
    }

  ret = da1470x_snc_send(msg->type, msg->payload, msg->len);
  return ret < 0 ? ret : (ssize_t)(sizeof(struct snc_msg_hdr_s) + msg->len);
}

/****************************************************************************
 * Name: snc_ioctl
 ****************************************************************************/

static int snc_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
  switch (cmd)
    {
      case SNCIOC_STATUS:
        {
          FAR struct snc_status_s *status =
            (FAR struct snc_status_s *)((uintptr_t)arg);

          if (status == NULL)
            {
              return -EINVAL;
            }

          da1470x_snc_getstatus(status);
          return OK;
        }

      case SNCIOC_STOP:
        da1470x_snc_stop();
        return OK;

      case SNCIOC_START:
        return da1470x_snc_start();

      default:
        return -ENOTTY;
    }
}

/****************************************************************************
 * Name: snc_poll
 ****************************************************************************/

static int snc_poll(FAR struct file *filep, FAR struct pollfd *fds,
                    bool setup)
{
  FAR struct snc_dev_s *priv = &g_snc;
  pollevent_t events = POLLOUT;
  int ret;
  int i;

  ret = nxmutex_lock(&priv->lock);
  if (ret < 0)
    {
      return ret;
    }

  if (setup)
    {
      for (i = 0; i < SNC_NPOLLWAITERS; i++)
        {
          if (priv->fds[i] == NULL)
            {
              priv->fds[i] = fds;
              fds->priv = &priv->fds[i];
              break;
            }
        }

      if (i >= SNC_NPOLLWAITERS)
        {
          ret = -EBUSY;
        }
      else
        {
          if (priv->rxcount > 0)
            {
              events |= POLLIN;
            }

          poll_notify(&fds, 1, events);
        }
    }
  else if (fds->priv != NULL)
    {
      *(FAR struct pollfd **)fds->priv = NULL;
      fds->priv = NULL;
    }

  nxmutex_unlock(&priv->lock);
  return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_snc_start
 ****************************************************************************/

int da1470x_snc_start(void)
{
  FAR struct snc_dev_s *priv = &g_snc;
  FAR const struct snc_image_hdr_s *hdr;
  size_t len = g_da1470x_snc_image_end - g_da1470x_snc_image;
  irqstate_t flags;
  int ret;
  int i;

  /* Check the image before anything is touched */

  hdr = (FAR const struct snc_image_hdr_s *)
        (g_da1470x_snc_image + SNC_IMAGE_HDR_OFFSET);

  if (len < SNC_IMAGE_HDR_OFFSET + sizeof(*hdr) ||
      hdr->magic != SNC_IPC_MAGIC ||
      hdr->ipc_version != SNC_IPC_VERSION ||
      hdr->size > len || hdr->size > DA1470X_SNC_CODE_SIZE)
    {
      snerr("SNC image rejected (%zu bytes)\n", len);
      return -ENOEXEC;
    }

  /* The processor lives in PD_SNC, its timer in PD_TIM */

  ret = da1470x_pd_enable(DA1470X_PD_SNC);
  if (ret >= 0)
    {
      ret = da1470x_pd_enable(DA1470X_PD_TIM);
    }

  if (ret < 0)
    {
      snerr("SNC power domains did not come up: %d\n", ret);
      return ret;
    }

  nxmutex_lock(&priv->lock);
  priv->running = false;
  priv->fault_reported = false;

  flags = enter_critical_section();

  /* Load with the processor held still: this is also the path that
   * replaces the firmware of a controller that is already running.
   */

  snc_hold_reset();

  /* Its watchdog is a separate timer that drives its NMI.  Freeze it: a
   * firmware that wants it can unfreeze it itself.
   */

  putreg32(GPREG_SET_FREEZE_FRZ_SNC_WDOG, DA1470X_GPREG_SET_FREEZE);

  /* A fresh shared area, then the image */

  memset(g_shared, 0, sizeof(*g_shared));
  g_shared->magic    = SNC_IPC_MAGIC;
  g_shared->version  = SNC_IPC_VERSION;
  g_shared->lpclk_hz = da1470x_get_lpclk();
  g_shared->state    = SNC_STATE_OFF;

  memcpy((FAR void *)DA1470X_SNC_CODE_BASE, g_da1470x_snc_image,
         hdr->size);

  /* The processor reads through a different port than we wrote, so make
   * sure the writes have landed before it is allowed to fetch.
   */

  __asm__ __volatile__ ("dsb" : : : "memory");

  putreg32(CRG_XTAL_RESET_SYS_IRQ_CTRL_SNC2SYS_IRQ_BIT |
           CRG_XTAL_RESET_SYS_IRQ_CTRL_SYS2SNC_IRQ_BIT,
           DA1470X_CRG_XTAL_RESET_SYS_IRQ_CTRL);

  modifyreg32(DA1470X_CRG_TOP_CLK_SNC_CTRL, 0,
              CRG_TOP_CLK_SNC_CTRL_SNC_CLK_ENABLE);
  modifyreg32(DA1470X_CRG_TOP_CLK_SNC_CTRL,
              CRG_TOP_CLK_SNC_CTRL_SNC_RESET_REQ, 0);

  leave_critical_section(flags);

  /* Wait for it to say it is ready, or that it fell over */

  for (i = 0; i < SNC_BOOT_TIMEOUT_MS; i++)
    {
      if (g_shared->state == SNC_STATE_READY ||
          g_shared->state == SNC_STATE_FAULT)
        {
          break;
        }

      up_mdelay(1);
    }

  if (g_shared->state != SNC_STATE_READY)
    {
      snerr("SNC did not start: state %" PRIu32 ", PC %08" PRIx32 "\n",
            g_shared->state, g_shared->fault_pc);
      snc_hold_reset();
      nxmutex_unlock(&priv->lock);
      return -EIO;
    }

  priv->running = true;
  nxmutex_unlock(&priv->lock);

  sninfo("SNC running, %" PRIu32 " byte image, ready in %d ms\n",
         hdr->size, i);

  /* Anything it said while starting */

  if (work_available(&priv->work))
    {
      work_queue(LPWORK, &priv->work, snc_worker, priv, 0);
    }

  return OK;
}

/****************************************************************************
 * Name: da1470x_snc_stop
 ****************************************************************************/

void da1470x_snc_stop(void)
{
  FAR struct snc_dev_s *priv = &g_snc;
  irqstate_t flags;

  nxmutex_lock(&priv->lock);

  flags = enter_critical_section();
  snc_hold_reset();
  modifyreg32(DA1470X_CRG_TOP_CLK_SNC_CTRL,
              CRG_TOP_CLK_SNC_CTRL_SNC_CLK_ENABLE, 0);
  g_shared->state = SNC_STATE_OFF;
  leave_critical_section(flags);

  priv->running = false;
  nxmutex_unlock(&priv->lock);
}

/****************************************************************************
 * Name: da1470x_snc_getstatus
 ****************************************************************************/

void da1470x_snc_getstatus(FAR struct snc_status_s *status)
{
  FAR struct snc_dev_s *priv = &g_snc;

  status->state          = g_shared->state;
  status->heartbeat      = g_shared->heartbeat;
  status->fault_pc       = g_shared->fault_pc;
  status->fault_lr       = g_shared->fault_lr;
  status->dropped_to_m33 = g_shared->dropped;
  status->dropped_to_snc = priv->dropped_to_snc;
  status->hwstatus       = getreg32(DA1470X_SNC_STATUS);
}

/****************************************************************************
 * Name: da1470x_snc_send
 ****************************************************************************/

int da1470x_snc_send(uint16_t type, FAR const void *payload, uint16_t len)
{
  FAR struct snc_dev_s *priv = &g_snc;
  int ret;

  if (len > SNC_MSG_MAX_PAYLOAD || (len > 0 && payload == NULL))
    {
      return -EINVAL;
    }

  ret = nxmutex_lock(&priv->lock);
  if (ret < 0)
    {
      return ret;
    }

  if (!priv->running)
    {
      ret = -ENODEV;
    }
  else if (g_shared->state != SNC_STATE_READY)
    {
      /* It faulted: nothing will read the ring until it is restarted */

      ret = -EIO;
    }
  else if (snc_ring_put(&g_shared->to_snc, type, payload, len) < 0)
    {
      priv->dropped_to_snc++;
      ret = -EAGAIN;
    }
  else
    {
      snc_doorbell();
    }

  nxmutex_unlock(&priv->lock);
  return ret;
}

/****************************************************************************
 * Name: da1470x_snc_register_handler
 ****************************************************************************/

int da1470x_snc_register_handler(uint16_t type, snc_handler_t handler,
                                 FAR void *arg)
{
  FAR struct snc_dev_s *priv = &g_snc;
  int free_slot = -1;
  int ret;
  int i;

  ret = nxmutex_lock(&priv->lock);
  if (ret < 0)
    {
      return ret;
    }

  for (i = 0; i < SNC_NHANDLERS; i++)
    {
      if (priv->handlers[i].handler != NULL &&
          priv->handlers[i].type == type)
        {
          break;
        }

      if (priv->handlers[i].handler == NULL && free_slot < 0)
        {
          free_slot = i;
        }
    }

  if (i >= SNC_NHANDLERS)
    {
      i = free_slot;
    }

  if (i < 0)
    {
      ret = handler == NULL ? OK : -ENOMEM;
    }
  else
    {
      priv->handlers[i].type    = type;
      priv->handlers[i].handler = handler;
      priv->handlers[i].arg     = arg;
    }

  nxmutex_unlock(&priv->lock);
  return ret;
}

/****************************************************************************
 * Name: da1470x_snc_initialize
 ****************************************************************************/

int da1470x_snc_initialize(FAR const char *devpath)
{
  int ret;

  irq_attach(DA1470X_IRQ_SNC2SYS, snc_interrupt, &g_snc);
  up_enable_irq(DA1470X_IRQ_SNC2SYS);

#ifdef CONFIG_DA1470X_PDC
  /* Let the controller's doorbell wake the M33 from its sleep states */

  da1470x_pdc_add(DA1470X_PDC_TRIG_PERIPHERAL, DA1470X_PDC_PERIPH_SNC2SYS,
                  DA1470X_PDC_MASTER_CM33, DA1470X_PDC_FLAG_EN_XTAL);
#endif

  ret = da1470x_snc_start();

  if (register_driver(devpath, &g_snc_fops, 0666, &g_snc) < 0)
    {
      snerr("Failed to register %s\n", devpath);
    }

  return ret;
}

#endif /* CONFIG_DA1470X_SNC */
