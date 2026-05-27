/****************************************************************************
 * arch/arm/src/da1470x/da1470x_i2c.c
 *
 * Polled I2C master driver for DA1470x. Implements NuttX i2c_ops_s for
 * I2C, I2C2, I2C3 — all DesignWare DW_apb_i2c controllers in PD_SNC.
 *
 * Initial scope: 7-bit-address master, Fast-mode (~400 kHz), polled.
 * Each transfer() processes the msg array sequentially, using repeated
 * START between messages unless I2C_M_NOSTART/I2C_M_NOSTOP say
 * otherwise. NACK is detected via IC_RAW_INTR_STAT.TX_ABRT.
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

#include <assert.h>
#include <debug.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include <nuttx/arch.h>
#include <nuttx/mutex.h>
#include <nuttx/i2c/i2c_master.h>

#include "arm_internal.h"
#include "hardware/da1470x_crg_snc.h"
#include "hardware/da1470x_i2c.h"
#include "hardware/da1470x_memorymap.h"
#include "da1470x_i2c.h"
#include "da1470x_pmu.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* I2C source clock = DivN = 32 MHz on our clock tree.
 * DW SCL_HCNT/LCNT register values are in source-clock periods.
 */

#define DA1470X_I2C_SRC_CLK              32000000

/* Bits we use that aren't already defined in hardware/da1470x_i2c.h. The
 * local header has offsets only; bit fields come from the SDK DA1470x-00.h.
 */

#define I2C_CON_MASTER_MODE              (1U << 0)
#define I2C_CON_SPEED_STANDARD           (1U << 1)
#define I2C_CON_SPEED_FAST               (2U << 1)
#define I2C_CON_10BITADDR_MASTER         (1U << 4)
#define I2C_CON_RESTART_EN               (1U << 5)
#define I2C_CON_SLAVE_DISABLE            (1U << 6)
#define I2C_CON_STOP_DET_IFADDRESSED     (1U << 7)
#define I2C_CON_TX_EMPTY_CTRL            (1U << 8)

#define I2C_DATA_CMD_DAT_MASK            0xFFU
#define I2C_DATA_CMD_CMD_READ            (1U << 8)
#define I2C_DATA_CMD_STOP                (1U << 9)
#define I2C_DATA_CMD_RESTART             (1U << 10)

#define I2C_ENABLE_EN                    (1U << 0)
#define I2C_ENABLE_ABORT                 (1U << 1)

#define I2C_STATUS_ACTIVITY              (1U << 0)
#define I2C_STATUS_TFNF                  (1U << 1)
#define I2C_STATUS_TFE                   (1U << 2)
#define I2C_STATUS_RFNE                  (1U << 3)
#define I2C_STATUS_RFF                   (1U << 4)
#define I2C_STATUS_MST_ACTIVITY          (1U << 5)

/* Bits in IC_RAW_INTR_STAT we care about. */

#define I2C_INTR_TX_ABRT                 (1U << 6)
#define I2C_INTR_STOP_DET                (1U << 9)

/* Poll deadline in source-clock cycles, ~50 ms at 32 MHz. */

#define DA1470X_I2C_POLL_LOOPS           1600000

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct da1470x_i2c_priv_s
{
  struct i2c_master_s i2cdev;    /* MUST be first */
  uintptr_t           base;
  mutex_t             lock;
  uint8_t             bus;
  uint16_t            cur_addr;  /* Currently programmed target address */
  bool                initialized;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int da1470x_i2c_transfer(struct i2c_master_s *dev,
                                struct i2c_msg_s *msgs, int count);
#ifdef CONFIG_I2C_RESET
static int da1470x_i2c_reset(struct i2c_master_s *dev);
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct i2c_ops_s g_i2c_ops =
{
  .transfer = da1470x_i2c_transfer,
#ifdef CONFIG_I2C_RESET
  .reset    = da1470x_i2c_reset,
#endif
};

static struct da1470x_i2c_priv_s g_i2c0_priv =
{
  .i2cdev = { .ops = &g_i2c_ops },
  .base   = DA1470X_I2C_BASE,
  .lock   = NXMUTEX_INITIALIZER,
  .bus    = DA1470X_I2C_BUS_I2C,
};

static struct da1470x_i2c_priv_s g_i2c1_priv =
{
  .i2cdev = { .ops = &g_i2c_ops },
  .base   = DA1470X_I2C2_BASE,
  .lock   = NXMUTEX_INITIALIZER,
  .bus    = DA1470X_I2C_BUS_I2C2,
};

static struct da1470x_i2c_priv_s g_i2c2_priv =
{
  .i2cdev = { .ops = &g_i2c_ops },
  .base   = DA1470X_I2C3_BASE,
  .lock   = NXMUTEX_INITIALIZER,
  .bus    = DA1470X_I2C_BUS_I2C3,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static inline void i2c_putreg(struct da1470x_i2c_priv_s *p,
                              uint32_t off, uint32_t val)
{
  putreg32(val, p->base + off);
}

static inline uint32_t i2c_getreg(struct da1470x_i2c_priv_s *p, uint32_t off)
{
  return getreg32(p->base + off);
}

static void i2c_clock_enable(struct da1470x_i2c_priv_s *p)
{
  uint32_t mask;
  uint32_t sel;

  switch (p->bus)
    {
      case DA1470X_I2C_BUS_I2C:
        mask = CRG_SNC_I2C_ENABLE;
        sel  = CRG_SNC_I2C_CLK_SEL;
        break;
      case DA1470X_I2C_BUS_I2C2:
        mask = CRG_SNC_I2C2_ENABLE;
        sel  = CRG_SNC_I2C2_CLK_SEL;
        break;
      case DA1470X_I2C_BUS_I2C3:
        mask = CRG_SNC_I2C3_ENABLE;
        sel  = CRG_SNC_I2C3_CLK_SEL;
        break;
      default:
        return;
    }

  /* DivN (32 MHz) -> clear CLK_SEL, then set ENABLE. */

  putreg32(sel, DA1470_CRG_SNC_RESET_CLK_SNC);
  putreg32(mask, DA1470_CRG_SNC_SET_CLK_SNC);
}

static void i2c_disable_block(struct da1470x_i2c_priv_s *p)
{
  /* Spec: writing 0 to IC_ENABLE asks the controller to stop; poll
   * IC_ENABLE_STATUS.IC_EN until it actually deasserts.
   */

  i2c_putreg(p, DA1470_I2C_ENABLE_OFFSET, 0);
  for (int i = 0; i < 1000; i++)
    {
      if ((i2c_getreg(p, DA1470_I2C_ENABLE_STATUS_OFFSET) & 0x1U) == 0)
        {
          return;
        }
    }
}

static void i2c_enable_block(struct da1470x_i2c_priv_s *p)
{
  i2c_putreg(p, DA1470_I2C_ENABLE_OFFSET, I2C_ENABLE_EN);
  for (int i = 0; i < 1000; i++)
    {
      if ((i2c_getreg(p, DA1470_I2C_ENABLE_STATUS_OFFSET) & 0x1U) != 0)
        {
          return;
        }
    }
}

static void i2c_set_target(struct da1470x_i2c_priv_s *p, uint16_t addr)
{
  if (addr == p->cur_addr)
    {
      return;
    }

  /* IC_TAR may only be written while the controller is disabled. */

  i2c_disable_block(p);
  i2c_putreg(p, DA1470_I2C_TAR_OFFSET, addr & 0x7FU);
  i2c_enable_block(p);
  p->cur_addr = addr;
}

/****************************************************************************
 * Master transfer (polled)
 *
 * For each message: enable RESTART when crossing message boundaries (or
 * when transitioning from write -> read). Push bytes via IC_DATA_CMD
 * with the CMD bit selecting direction; for the last byte of the last
 * message (unless I2C_M_NOSTOP), set the STOP bit so the controller
 * emits a STOP condition.
 *
 * NACK is detected by reading IC_RAW_INTR_STAT after the controller
 * raises an error; we clear it via IC_CLR_TX_ABRT and return -ENXIO so
 * the caller can distinguish "no device" from a bus hang (-ETIMEDOUT).
 ****************************************************************************/

static int i2c_xfer_msg(struct da1470x_i2c_priv_s *p,
                        struct i2c_msg_s *msg, bool need_restart, bool last)
{
  size_t i;
  uint8_t *buf = msg->buffer;
  size_t len   = msg->length;
  bool is_read = (msg->flags & I2C_M_READ) != 0;

  /* Make sure the controller is addressing this slave. */

  i2c_set_target(p, msg->addr);

  /* Walk the data buffer. For reads, push N "read command" entries and
   * pull responses; for writes, push N data bytes.
   */

  size_t rx_done = 0;
  for (i = 0; i < len; i++)
    {
      uint32_t cmd = is_read ? I2C_DATA_CMD_CMD_READ : (buf[i] & 0xFFU);

      if (need_restart && i == 0)
        {
          cmd |= I2C_DATA_CMD_RESTART;
        }

      if (last && i == len - 1 && (msg->flags & I2C_M_NOSTOP) == 0)
        {
          cmd |= I2C_DATA_CMD_STOP;
        }

      /* Wait for TX FIFO space. */

      int budget = DA1470X_I2C_POLL_LOOPS;
      while ((i2c_getreg(p, DA1470_I2C_STATUS_OFFSET) & I2C_STATUS_TFNF) == 0)
        {
          if (--budget <= 0) return -ETIMEDOUT;
        }

      i2c_putreg(p, DA1470_I2C_DATA_CMD_OFFSET, cmd);

      /* For reads, drain the RX FIFO as bytes arrive so it doesn't
       * stall. We don't need to wait per-byte here: the controller
       * keeps issuing reads up to TX FIFO depth, and we'll loop again.
       */

      if (is_read)
        {
          while (rx_done <= i &&
                 (i2c_getreg(p, DA1470_I2C_STATUS_OFFSET) & I2C_STATUS_RFNE)
                 != 0)
            {
              buf[rx_done++] = i2c_getreg(p, DA1470_I2C_DATA_CMD_OFFSET)
                               & I2C_DATA_CMD_DAT_MASK;
            }
        }

      /* Check for NACK / abort after each pushed entry. */

      if ((i2c_getreg(p, DA1470_I2C_RAW_INTR_STAT_OFFSET) & I2C_INTR_TX_ABRT)
          != 0)
        {
          (void)i2c_getreg(p, DA1470_I2C_CLR_TX_ABRT_OFFSET);
          return -ENXIO;
        }
    }

  /* For reads, drain whatever remains in the RX FIFO. */

  if (is_read)
    {
      int budget = DA1470X_I2C_POLL_LOOPS;
      while (rx_done < len)
        {
          if ((i2c_getreg(p, DA1470_I2C_RAW_INTR_STAT_OFFSET)
               & I2C_INTR_TX_ABRT) != 0)
            {
              (void)i2c_getreg(p, DA1470_I2C_CLR_TX_ABRT_OFFSET);
              return -ENXIO;
            }

          if ((i2c_getreg(p, DA1470_I2C_STATUS_OFFSET) & I2C_STATUS_RFNE)
              != 0)
            {
              buf[rx_done++] = i2c_getreg(p, DA1470_I2C_DATA_CMD_OFFSET)
                               & I2C_DATA_CMD_DAT_MASK;
            }
          else if (--budget <= 0)
            {
              return -ETIMEDOUT;
            }
        }
    }

  return OK;
}

static int da1470x_i2c_transfer(struct i2c_master_s *dev,
                                struct i2c_msg_s *msgs, int count)
{
  struct da1470x_i2c_priv_s *p = (struct da1470x_i2c_priv_s *)dev;
  int ret = OK;
  int i;

  if (count <= 0)
    {
      return -EINVAL;
    }

  nxmutex_lock(&p->lock);

  for (i = 0; i < count; i++)
    {
      bool need_restart = (i > 0) && (msgs[i].flags & I2C_M_NOSTART) == 0;
      bool last         = (i == count - 1);

      ret = i2c_xfer_msg(p, &msgs[i], need_restart, last);
      if (ret < 0)
        {
          break;
        }
    }

  /* Confirm the bus actually completed: either we saw STOP_DET (the
   * controller drove a STOP at the end) or TX_ABRT (NACK from slave).
   * MST_ACTIVITY alone is not enough — it can be 0 from the start if
   * the pins aren't muxed and no transaction ever began, which would
   * falsely indicate success.
   *
   * Only require STOP_DET when the last message asked for a STOP.
   */

  bool needed_stop = (count > 0) &&
                     (msgs[count - 1].flags & I2C_M_NOSTOP) == 0;

  if (ret == OK && needed_stop)
    {
      int budget = DA1470X_I2C_POLL_LOOPS;
      while (true)
        {
          uint32_t raw = i2c_getreg(p, DA1470_I2C_RAW_INTR_STAT_OFFSET);

          if ((raw & I2C_INTR_TX_ABRT) != 0)
            {
              (void)i2c_getreg(p, DA1470_I2C_CLR_TX_ABRT_OFFSET);
              ret = -ENXIO;
              break;
            }

          if ((raw & I2C_INTR_STOP_DET) != 0)
            {
              (void)i2c_getreg(p, DA1470_I2C_CLR_STOP_DET_OFFSET);
              break;
            }

          if (--budget <= 0)
            {
              ret = -ETIMEDOUT;
              break;
            }
        }
    }

  /* Drain any residual STOP_DET so it doesn't masquerade as completion
   * of a future transfer.
   */

  (void)i2c_getreg(p, DA1470_I2C_CLR_STOP_DET_OFFSET);

  nxmutex_unlock(&p->lock);
  return ret;
}

#ifdef CONFIG_I2C_RESET
static int da1470x_i2c_reset(struct i2c_master_s *dev)
{
  struct da1470x_i2c_priv_s *p = (struct da1470x_i2c_priv_s *)dev;
  nxmutex_lock(&p->lock);
  i2c_disable_block(p);
  i2c_enable_block(p);
  p->cur_addr = 0xFFFF;       /* force re-program */
  nxmutex_unlock(&p->lock);
  return OK;
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

struct i2c_master_s *da1470x_i2cbus_initialize(int bus)
{
  struct da1470x_i2c_priv_s *p;

  switch (bus)
    {
      case DA1470X_I2C_BUS_I2C:  p = &g_i2c0_priv; break;
      case DA1470X_I2C_BUS_I2C2: p = &g_i2c1_priv; break;
      case DA1470X_I2C_BUS_I2C3: p = &g_i2c2_priv; break;
      default:
        i2cerr("ERROR: I2C bus %d not supported\n", bus);
        return NULL;
    }

  if (p->initialized)
    {
      return &p->i2cdev;
    }

  da1470x_pd_enable(DA1470X_PD_SNC);
  i2c_clock_enable(p);

  /* Controller must be disabled to program timing / mode. */

  i2c_disable_block(p);

  /* Fast-mode master, 7-bit, slave-disabled, repeated-start enabled. */

  i2c_putreg(p, DA1470_I2C_CON_OFFSET,
             I2C_CON_MASTER_MODE | I2C_CON_SPEED_FAST |
             I2C_CON_RESTART_EN  | I2C_CON_SLAVE_DISABLE |
             I2C_CON_TX_EMPTY_CTRL);

  /* Timing for Fast-mode (400 kHz) @ 32 MHz source clock:
   *   bit time = 2.5 us; HCNT + LCNT ~= 80 cycles minimum.
   * Use HCNT = 26, LCNT = 50 -> ~76 cycles total (~ 421 kHz) which is
   * within Fast-mode spec margins. Spike-length filter = 1 cycle.
   */

  i2c_putreg(p, DA1470_I2C_FS_SCL_HCNT_OFFSET, 26);
  i2c_putreg(p, DA1470_I2C_FS_SCL_LCNT_OFFSET, 50);
  i2c_putreg(p, DA1470_I2C_IC_FS_SPKLEN_OFFSET, 1);

  /* Standard-mode timing (filled in case CON.SPEED is changed later). */

  i2c_putreg(p, DA1470_I2C_SS_SCL_HCNT_OFFSET, 130);
  i2c_putreg(p, DA1470_I2C_SS_SCL_LCNT_OFFSET, 150);

  /* TX/RX FIFO thresholds: interrupt on TX-empty when level <= 4, and
   * on RX-full when level >= 0 (i.e. at least one byte present). We're
   * polled so the thresholds only matter for future IRQ-mode work, but
   * keep them sensible.
   */

  i2c_putreg(p, DA1470_I2C_TX_TL_OFFSET, 0);
  i2c_putreg(p, DA1470_I2C_RX_TL_OFFSET, 0);

  /* Mask all interrupts (polled driver). */

  i2c_putreg(p, DA1470_I2C_INTR_MASK_OFFSET, 0);

  /* Re-enable the controller in master mode. */

  i2c_enable_block(p);

  p->cur_addr    = 0xFFFF;    /* force first transfer to program TAR */
  p->initialized = true;
  return &p->i2cdev;
}
