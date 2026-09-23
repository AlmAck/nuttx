/****************************************************************************
 * arch/arm/src/da1470x/da1470x_i2c.c
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

/* Interrupt driven master driver for the DesignWare I2C controllers of the
 * DA1470x.  Each i2c_msg_s is pushed through the 4-entry command FIFO from
 * the TX_EMPTY interrupt; reads are drained from the RX_FULL interrupt;
 * completion is signalled by STOP_DET, and NACKs by TX_ABRT.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <assert.h>
#include <debug.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include <nuttx/arch.h>
#include <nuttx/clock.h>
#include <nuttx/mutex.h>
#include <nuttx/semaphore.h>
#include <nuttx/i2c/i2c_master.h>

#include "arm_internal.h"
#include "hardware/da1470x_crg_snc.h"
#include "hardware/da1470x_i2c.h"
#include "hardware/da1470x_memorymap.h"
#include "da1470x_clockconfig.h"
#include "da1470x_gpio.h"
#include "da1470x_i2c.h"
#include "da1470x_pmu.h"

#ifdef CONFIG_DA1470X_I2C

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define I2C_TIMEOUT_MS          500
#define I2C_ENABLE_WAIT_LOOPS   10000

/* Interrupts used by the master state machine */

#define I2C_INTR_MASTER (I2C_INTR_MASK_M_TX_EMPTY | \
                         I2C_INTR_MASK_M_RX_FULL  | \
                         I2C_INTR_MASK_M_TX_ABRT  | \
                         I2C_INTR_MASK_M_STOP_DET)

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct da1470x_i2c_priv_s
{
  struct i2c_master_s i2cdev;   /* Externally visible part */
  uintptr_t   base;             /* Register base */
  uint8_t     bus;              /* Bus number (0..2) */
  uint8_t     irq;              /* NVIC interrupt number */
  uint32_t    enable_bit;       /* CRG_SNC clock enable bit */
  uint32_t    sel_bit;          /* CRG_SNC clock select bit */
  mutex_t     lock;             /* Bus exclusive lock */
  sem_t       wait;             /* Transfer completion */
  uint32_t    frequency;        /* Programmed bus frequency */
  bool        initialized;

  /* Current transfer */

  struct i2c_msg_s *msgs;       /* Message array */
  int         msgc;             /* Number of messages */
  int         msgi;             /* Index of the message being pushed */
  size_t      txndx;            /* Next byte to push in msgs[msgi] */
  int         rxmsg;            /* Index of the message being drained */
  size_t      rxndx;            /* Next byte to store in msgs[rxmsg] */
  int         result;           /* Transfer result */
  bool        done;             /* Completion flag */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int  i2c_transfer(struct i2c_master_s *dev, struct i2c_msg_s *msgs,
                         int count);
#ifdef CONFIG_I2C_RESET
static int  i2c_reset(struct i2c_master_s *dev);
#endif
static int  i2c_interrupt(int irq, void *context, void *arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct i2c_ops_s g_i2c_ops =
{
  .transfer = i2c_transfer,
#ifdef CONFIG_I2C_RESET
  .reset    = i2c_reset,
#endif
};

#ifdef CONFIG_DA1470X_I2C0
static struct da1470x_i2c_priv_s g_i2c0priv =
{
  .i2cdev     =
  {
    .ops = &g_i2c_ops
  },
  .base       = DA1470X_I2C0_BASE,
  .bus        = 0,
  .irq        = DA1470X_IRQ_I2C0,
  .enable_bit = CRG_SNC_CLK_SNC_I2C_ENABLE,
  .sel_bit    = CRG_SNC_CLK_SNC_I2C_CLK_SEL,
  .lock       = NXMUTEX_INITIALIZER,
  .wait       = SEM_INITIALIZER(0),
};
#endif

#ifdef CONFIG_DA1470X_I2C1
static struct da1470x_i2c_priv_s g_i2c1priv =
{
  .i2cdev     =
  {
    .ops = &g_i2c_ops
  },
  .base       = DA1470X_I2C1_BASE,
  .bus        = 1,
  .irq        = DA1470X_IRQ_I2C1,
  .enable_bit = CRG_SNC_CLK_SNC_I2C2_ENABLE,
  .sel_bit    = CRG_SNC_CLK_SNC_I2C2_CLK_SEL,
  .lock       = NXMUTEX_INITIALIZER,
  .wait       = SEM_INITIALIZER(0),
};
#endif

#ifdef CONFIG_DA1470X_I2C2
static struct da1470x_i2c_priv_s g_i2c2priv =
{
  .i2cdev     =
  {
    .ops = &g_i2c_ops
  },
  .base       = DA1470X_I2C2_BASE,
  .bus        = 2,
  .irq        = DA1470X_IRQ_I2C2,
  .enable_bit = CRG_SNC_CLK_SNC_I2C3_ENABLE,
  .sel_bit    = CRG_SNC_CLK_SNC_I2C3_CLK_SEL,
  .lock       = NXMUTEX_INITIALIZER,
  .wait       = SEM_INITIALIZER(0),
};
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: i2c_getreg / i2c_putreg
 ****************************************************************************/

static inline uint32_t i2c_getreg(struct da1470x_i2c_priv_s *priv,
                                  uint32_t offset)
{
  return getreg32(priv->base + offset);
}

static inline void i2c_putreg(struct da1470x_i2c_priv_s *priv,
                              uint32_t offset, uint32_t value)
{
  putreg32(value, priv->base + offset);
}

/****************************************************************************
 * Name: i2c_enable_block
 *
 * Description:
 *   Enable or disable the controller and wait for IC_ENABLE_STATUS to
 *   follow.
 *
 ****************************************************************************/

static void i2c_enable_block(struct da1470x_i2c_priv_s *priv, bool enable)
{
  uint32_t want = enable ? I2C_ENABLE_STATUS_IC_EN : 0;
  int i;

  i2c_putreg(priv, DA1470X_I2C_ENABLE_OFFSET, enable ? I2C_ENABLE_EN : 0);

  for (i = 0; i < I2C_ENABLE_WAIT_LOOPS; i++)
    {
      if ((i2c_getreg(priv, DA1470X_I2C_ENABLE_STATUS_OFFSET) &
           I2C_ENABLE_STATUS_IC_EN) == want)
        {
          return;
        }
    }

  i2cwarn("IC_ENABLE did not settle\n");
}

/****************************************************************************
 * Name: i2c_setfrequency
 *
 * Description:
 *   Program the SCL high/low counts for the requested bus frequency.  The
 *   controller is clocked from DIVN (32 MHz).  Counts follow the DesignWare
 *   rule: HCNT + LCNT + spike-filter + fixed overhead = clocks per bit.
 *
 ****************************************************************************/

static void i2c_setfrequency(struct da1470x_i2c_priv_s *priv,
                             uint32_t frequency)
{
  uint32_t clk = da1470x_get_divn_clk();
  uint32_t total;
  uint32_t hcnt;
  uint32_t lcnt;
  uint32_t con;

  if (frequency == priv->frequency || frequency == 0)
    {
      return;
    }

  total = clk / frequency;

  /* Low phase gets the larger share as required by the I2C timing spec
   * (tLOW >= 1.3 us at 400 kHz, >= 4.7 us at 100 kHz).
   */

  lcnt = (total * 6) / 10;
  hcnt = total - lcnt;

  /* Subtract the controller's internal overhead (7 clocks on high, 1 on
   * low) and keep the minimum legal values.
   */

  if (hcnt > 8)
    {
      hcnt -= 8;
    }
  else
    {
      hcnt = 6;
    }

  if (lcnt > 1)
    {
      lcnt -= 1;
    }
  else
    {
      lcnt = 8;
    }

  i2c_enable_block(priv, false);

  con  = i2c_getreg(priv, DA1470X_I2C_CON_OFFSET);
  con &= ~I2C_CON_SPEED_MASK;

  if (frequency <= 100000)
    {
      con |= I2C_CON_SPEED_STANDARD;
      i2c_putreg(priv, DA1470X_I2C_SS_SCL_HCNT_OFFSET, hcnt);
      i2c_putreg(priv, DA1470X_I2C_SS_SCL_LCNT_OFFSET, lcnt);
    }
  else
    {
      con |= I2C_CON_SPEED_FAST;
      i2c_putreg(priv, DA1470X_I2C_FS_SCL_HCNT_OFFSET, hcnt);
      i2c_putreg(priv, DA1470X_I2C_FS_SCL_LCNT_OFFSET, lcnt);
    }

  i2c_putreg(priv, DA1470X_I2C_CON_OFFSET, con);
  i2c_putreg(priv, DA1470X_I2C_IC_FS_SPKLEN_OFFSET, 1);

  i2c_enable_block(priv, true);
  priv->frequency = frequency;
}

/****************************************************************************
 * Name: i2c_settarget
 *
 * Description:
 *   Program the target address and addressing mode.  Requires the block to
 *   be disabled.
 *
 ****************************************************************************/

static void i2c_settarget(struct da1470x_i2c_priv_s *priv,
                          struct i2c_msg_s *msg)
{
  uint32_t con;

  i2c_enable_block(priv, false);

  con = i2c_getreg(priv, DA1470X_I2C_CON_OFFSET);

  if ((msg->flags & I2C_M_TEN) != 0)
    {
      con |= I2C_CON_10BITADDR_MASTER;
      i2c_putreg(priv, DA1470X_I2C_TAR_OFFSET, msg->addr & 0x3ff);
    }
  else
    {
      con &= ~I2C_CON_10BITADDR_MASTER;
      i2c_putreg(priv, DA1470X_I2C_TAR_OFFSET, msg->addr & 0x7f);
    }

  i2c_putreg(priv, DA1470X_I2C_CON_OFFSET, con);
  i2c_enable_block(priv, true);
}

/****************************************************************************
 * Name: i2c_abort_to_errno
 ****************************************************************************/

static int i2c_abort_to_errno(uint32_t source)
{
  if ((source & (I2C_TX_ABRT_SOURCE_ABRT_7B_ADDR_NOACK |
                 I2C_TX_ABRT_SOURCE_ABRT_10ADDR1_NOACK |
                 I2C_TX_ABRT_SOURCE_ABRT_10ADDR2_NOACK)) != 0)
    {
      return -ENXIO;
    }

  if ((source & I2C_TX_ABRT_SOURCE_ABRT_TXDATA_NOACK) != 0)
    {
      return -EIO;
    }

  if ((source & I2C_TX_ABRT_SOURCE_ARB_LOST) != 0)
    {
      return -EAGAIN;
    }

  return -EIO;
}

/****************************************************************************
 * Name: i2c_fill_txfifo
 *
 * Description:
 *   Push as many command entries as the FIFO accepts.  Returns true when
 *   every message has been fully queued.
 *
 ****************************************************************************/

static bool i2c_fill_txfifo(struct da1470x_i2c_priv_s *priv)
{
  while (priv->msgi < priv->msgc)
    {
      struct i2c_msg_s *msg = &priv->msgs[priv->msgi];
      uint32_t cmd;

      if (priv->txndx >= msg->length)
        {
          /* Message fully queued: advance.  A new address requires the
           * controller to be re-targeted, which can only happen once
           * the current traffic has drained; that case is handled by
           * splitting the transfer in i2c_transfer().
           */

          priv->msgi++;
          priv->txndx = 0;
          continue;
        }

      if ((i2c_getreg(priv, DA1470X_I2C_STATUS_OFFSET) &
           I2C_STATUS_TFNF) == 0)
        {
          return false;
        }

      if ((msg->flags & I2C_M_READ) != 0)
        {
          cmd = I2C_DATA_CMD_CMD;
        }
      else
        {
          cmd = msg->buffer[priv->txndx];
        }

      /* RESTART before the first byte of every message that follows a
       * message without STOP (unless the caller asked for no START).
       */

      if (priv->txndx == 0 && priv->msgi > 0 &&
          (msg->flags & I2C_M_NOSTART) == 0)
        {
          cmd |= I2C_DATA_CMD_RESTART;
        }

      /* STOP after the last byte of the last message, unless suppressed */

      if (priv->txndx == msg->length - 1 &&
          priv->msgi == priv->msgc - 1 &&
          (msg->flags & I2C_M_NOSTOP) == 0)
        {
          cmd |= I2C_DATA_CMD_STOP;
        }

      i2c_putreg(priv, DA1470X_I2C_DATA_CMD_OFFSET, cmd);
      priv->txndx++;
    }

  return true;
}

/****************************************************************************
 * Name: i2c_drain_rxfifo
 ****************************************************************************/

static void i2c_drain_rxfifo(struct da1470x_i2c_priv_s *priv)
{
  while ((i2c_getreg(priv, DA1470X_I2C_STATUS_OFFSET) &
          I2C_STATUS_RFNE) != 0)
    {
      uint8_t data = i2c_getreg(priv, DA1470X_I2C_DATA_CMD_OFFSET) & 0xff;

      /* Find the next read message with room */

      while (priv->rxmsg < priv->msgc &&
             ((priv->msgs[priv->rxmsg].flags & I2C_M_READ) == 0 ||
              priv->rxndx >= priv->msgs[priv->rxmsg].length))
        {
          priv->rxmsg++;
          priv->rxndx = 0;
        }

      if (priv->rxmsg >= priv->msgc)
        {
          i2cwarn("Unexpected RX byte\n");
          continue;
        }

      priv->msgs[priv->rxmsg].buffer[priv->rxndx++] = data;
    }
}

/****************************************************************************
 * Name: i2c_complete
 ****************************************************************************/

static void i2c_complete(struct da1470x_i2c_priv_s *priv, int result)
{
  if (!priv->done)
    {
      priv->result = result;
      priv->done   = true;
      i2c_putreg(priv, DA1470X_I2C_INTR_MASK_OFFSET, 0);
      nxsem_post(&priv->wait);
    }
}

/****************************************************************************
 * Name: i2c_interrupt
 ****************************************************************************/

static int i2c_interrupt(int irq, void *context, void *arg)
{
  struct da1470x_i2c_priv_s *priv = (struct da1470x_i2c_priv_s *)arg;
  uint32_t stat = i2c_getreg(priv, DA1470X_I2C_INTR_STAT_OFFSET);

  if ((stat & I2C_RAW_INTR_STAT_TX_ABRT) != 0)
    {
      uint32_t source = i2c_getreg(priv, DA1470X_I2C_TX_ABRT_SOURCE_OFFSET);

      i2c_getreg(priv, DA1470X_I2C_CLR_TX_ABRT_OFFSET);
      i2c_complete(priv, i2c_abort_to_errno(source));
      return OK;
    }

  if ((stat & I2C_RAW_INTR_STAT_RX_FULL) != 0)
    {
      i2c_drain_rxfifo(priv);
    }

  if ((stat & I2C_RAW_INTR_STAT_TX_EMPTY) != 0)
    {
      if (i2c_fill_txfifo(priv))
        {
          /* Everything queued: stop asking for TX_EMPTY */

          modifyreg32(priv->base + DA1470X_I2C_INTR_MASK_OFFSET,
                      I2C_INTR_MASK_M_TX_EMPTY, 0);
        }
    }

  if ((stat & I2C_RAW_INTR_STAT_STOP_DET) != 0)
    {
      i2c_getreg(priv, DA1470X_I2C_CLR_STOP_DET_OFFSET);
      i2c_drain_rxfifo(priv);

      if (priv->msgi >= priv->msgc)
        {
          i2c_complete(priv, OK);
        }
    }

  return OK;
}

/****************************************************************************
 * Name: i2c_run
 *
 * Description:
 *   Execute a group of messages that share the same target address and
 *   end with a STOP (or a caller-requested NOSTOP).
 *
 ****************************************************************************/

static int i2c_run(struct da1470x_i2c_priv_s *priv, struct i2c_msg_s *msgs,
                   int count)
{
  irqstate_t flags;
  int ret;

  priv->msgs   = msgs;
  priv->msgc   = count;
  priv->msgi   = 0;
  priv->txndx  = 0;
  priv->rxmsg  = 0;
  priv->rxndx  = 0;
  priv->result = OK;
  priv->done   = false;

  i2c_settarget(priv, &msgs[0]);

  /* Clear any stale status, then arm the interrupts.  The TX_EMPTY
   * interrupt fires immediately and starts pushing commands.
   */

  i2c_getreg(priv, DA1470X_I2C_CLR_INTR_OFFSET);
  i2c_putreg(priv, DA1470X_I2C_TX_TL_OFFSET, 0);
  i2c_putreg(priv, DA1470X_I2C_RX_TL_OFFSET, 0);

  flags = enter_critical_section();
  i2c_putreg(priv, DA1470X_I2C_INTR_MASK_OFFSET, I2C_INTR_MASTER);
  leave_critical_section(flags);

  ret = nxsem_tickwait_uninterruptible(&priv->wait,
                                       MSEC2TICK(I2C_TIMEOUT_MS));
  if (ret < 0)
    {
      flags = enter_critical_section();
      i2c_putreg(priv, DA1470X_I2C_INTR_MASK_OFFSET, 0);
      priv->done = true;
      leave_critical_section(flags);

      i2cerr("Timeout, STATUS=%08" PRIx32 " RAW=%08" PRIx32 "\n",
             i2c_getreg(priv, DA1470X_I2C_STATUS_OFFSET),
             i2c_getreg(priv, DA1470X_I2C_RAW_INTR_STAT_OFFSET));

      /* Abort and recover the controller */

      i2c_putreg(priv, DA1470X_I2C_ENABLE_OFFSET,
                 I2C_ENABLE_EN | I2C_ENABLE_ABORT);
      up_udelay(100);
      i2c_getreg(priv, DA1470X_I2C_CLR_TX_ABRT_OFFSET);
      i2c_enable_block(priv, false);
      i2c_enable_block(priv, true);
      return -ETIMEDOUT;
    }

  /* Make sure any trailing RX data is stored */

  i2c_drain_rxfifo(priv);
  i2c_getreg(priv, DA1470X_I2C_CLR_INTR_OFFSET);

  return priv->result;
}

/****************************************************************************
 * Name: i2c_transfer
 *
 * Description:
 *   Generic I2C transfer function.  Consecutive messages to the same
 *   address are executed in one hardware transaction; a change of address
 *   starts a new one.
 *
 ****************************************************************************/

static int i2c_transfer(struct i2c_master_s *dev, struct i2c_msg_s *msgs,
                        int count)
{
  struct da1470x_i2c_priv_s *priv = (struct da1470x_i2c_priv_s *)dev;
  int start = 0;
  int ret = OK;
  int i;

  if (msgs == NULL || count <= 0)
    {
      return -EINVAL;
    }

  ret = nxmutex_lock(&priv->lock);
  if (ret < 0)
    {
      return ret;
    }

  if (msgs[0].frequency != 0)
    {
      i2c_setfrequency(priv, msgs[0].frequency);
    }

  for (i = 1; i <= count; i++)
    {
      if (i == count || msgs[i].addr != msgs[start].addr)
        {
          ret = i2c_run(priv, &msgs[start], i - start);
          if (ret < 0)
            {
              break;
            }

          start = i;
        }
    }

  nxmutex_unlock(&priv->lock);
  return ret;
}

/****************************************************************************
 * Name: i2c_reset
 *
 * Description:
 *   Recover a stuck bus: disable the controller, clock SCL manually until
 *   SDA is released, issue a STOP and re-enable.
 *
 ****************************************************************************/

#ifdef CONFIG_I2C_RESET
static int i2c_reset(struct i2c_master_s *dev)
{
  struct da1470x_i2c_priv_s *priv = (struct da1470x_i2c_priv_s *)dev;

  nxmutex_lock(&priv->lock);
  i2c_enable_block(priv, false);
  i2c_getreg(priv, DA1470X_I2C_CLR_INTR_OFFSET);
  i2c_enable_block(priv, true);
  nxmutex_unlock(&priv->lock);
  return OK;
}
#endif

/****************************************************************************
 * Name: i2c_bus_initialize
 ****************************************************************************/

static void i2c_bus_initialize(struct da1470x_i2c_priv_s *priv)
{
  da1470x_pd_enable(DA1470X_PD_SNC);

  /* DIVN clock, then enable */

  putreg32(priv->sel_bit, DA1470X_CRG_SNC_RESET_CLK_SNC);
  putreg32(priv->enable_bit, DA1470X_CRG_SNC_SET_CLK_SNC);

  i2c_enable_block(priv, false);

  i2c_putreg(priv, DA1470X_I2C_CON_OFFSET,
             I2C_CON_MASTER_MODE | I2C_CON_SPEED_FAST |
             I2C_CON_RESTART_EN | I2C_CON_SLAVE_DISABLE |
             I2C_CON_TX_EMPTY_CTRL);

  i2c_putreg(priv, DA1470X_I2C_INTR_MASK_OFFSET, 0);
  i2c_putreg(priv, DA1470X_I2C_TX_TL_OFFSET, 0);
  i2c_putreg(priv, DA1470X_I2C_RX_TL_OFFSET, 0);
  i2c_putreg(priv, DA1470X_I2C_SDA_HOLD_OFFSET, 8);

  priv->frequency = 0;
  i2c_setfrequency(priv, 100000);

  irq_attach(priv->irq, i2c_interrupt, priv);
  up_enable_irq(priv->irq);

  i2c_enable_block(priv, true);
  priv->initialized = true;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_i2cbus_initialize
 ****************************************************************************/

struct i2c_master_s *da1470x_i2cbus_initialize(int bus)
{
  struct da1470x_i2c_priv_s *priv;

  switch (bus)
    {
#ifdef CONFIG_DA1470X_I2C0
      case 0:
        priv = &g_i2c0priv;
        break;
#endif
#ifdef CONFIG_DA1470X_I2C1
      case 1:
        priv = &g_i2c1priv;
        break;
#endif
#ifdef CONFIG_DA1470X_I2C2
      case 2:
        priv = &g_i2c2priv;
        break;
#endif
      default:
        i2cerr("I2C bus %d not supported\n", bus);
        return NULL;
    }

  if (!priv->initialized)
    {
      nxmutex_lock(&priv->lock);
      i2c_bus_initialize(priv);
      nxmutex_unlock(&priv->lock);
    }

  return &priv->i2cdev;
}

/****************************************************************************
 * Name: da1470x_i2cbus_uninitialize
 ****************************************************************************/

int da1470x_i2cbus_uninitialize(struct i2c_master_s *dev)
{
  struct da1470x_i2c_priv_s *priv = (struct da1470x_i2c_priv_s *)dev;

  if (priv == NULL || !priv->initialized)
    {
      return -EINVAL;
    }

  nxmutex_lock(&priv->lock);

  up_disable_irq(priv->irq);
  irq_detach(priv->irq);
  i2c_enable_block(priv, false);
  putreg32(priv->enable_bit, DA1470X_CRG_SNC_RESET_CLK_SNC);

  priv->initialized = false;
  nxmutex_unlock(&priv->lock);
  return OK;
}

#endif /* CONFIG_DA1470X_I2C */
