/****************************************************************************
 * drivers/input/zt2628.c
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

/* Zinitix ZT2628 capacitive touch controller.  The device speaks 16-bit
 * little-endian register addresses and values over I2C; a "command" is a
 * bare address write.  The controller reports one point per interrupt in
 * a fixed status block and expects the interrupt to be cleared after
 * every read.  The start-up sequence follows the vendor's reference.
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
#include <nuttx/signal.h>
#include <nuttx/wqueue.h>
#include <nuttx/clock.h>
#include <nuttx/i2c/i2c_master.h>
#include <nuttx/input/touchscreen.h>
#include <nuttx/input/zt2628.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Registers and commands */

#define ZT2628_CMD_SWRESET          0x0000
#define ZT2628_CMD_WAKEUP           0x0001
#define ZT2628_CMD_CLEAR_INT        0x0003
#define ZT2628_CMD_SLEEP            0x0005
#define ZT2628_REG_TOUCH_MODE       0x0010
#define ZT2628_REG_CHIP_REVISION    0x0011
#define ZT2628_REG_FIRMWARE_VERSION 0x0012
#define ZT2628_REG_X_RESOLUTION     0x00c0
#define ZT2628_REG_Y_RESOLUTION     0x00c1
#define ZT2628_REG_POINT_STATUS     0x0080
#define ZT2628_REG_INT_EN           0x00f0
#define ZT2628_REG_COVER_CONTROL    0x023e

#define ZT2628_CMD_POWER_ON         0xc000
#define ZT2628_CMD_NVM_INIT         0xc002
#define ZT2628_CMD_NVM_VPP          0xc004
#define ZT2628_CMD_NVM_WP           0xc001
#define ZT2628_REG_CHIP_CODE        0xcc00

#define ZT2628_CHIP_CODE            0xe628
#define ZT2628_COVER_OPEN           0x0200

/* Interrupt enable bits */

#define ZT2628_INT_PT_CNT_CHANGE    (1 << 0)
#define ZT2628_INT_DOWN             (1 << 1)
#define ZT2628_INT_MOVE             (1 << 2)
#define ZT2628_INT_UP               (1 << 3)
#define ZT2628_INT_MUST_ZERO        (1 << 13)

/* Point status bits */

#define ZT2628_POINT_EXIST          (1 << 0)
#define ZT2628_POINT_DOWN           (1 << 1)
#define ZT2628_POINT_MOVE           (1 << 2)
#define ZT2628_POINT_UP             (1 << 3)

#define ZT2628_STATUS_LEN           10

#ifndef CONFIG_INPUT_ZT2628_NPOLLWAITERS
#  define CONFIG_INPUT_ZT2628_NPOLLWAITERS 2
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct zt2628_dev_s
{
  struct touch_lowerhalf_s lower;             /* Must be first */
  FAR struct i2c_master_s *i2c;
  FAR const struct zt2628_config_s *config;
  struct work_s work;
  bool pressed;                               /* Last reported state */
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: zt2628_write
 *
 * Description:
 *   Write a register address, optionally followed by one 16-bit value.
 *
 ****************************************************************************/

static int zt2628_write(FAR struct zt2628_dev_s *priv, uint16_t reg,
                        FAR const uint16_t *value)
{
  struct i2c_msg_s msg;
  uint8_t buf[4];

  buf[0] = reg & 0xff;
  buf[1] = reg >> 8;
  if (value != NULL)
    {
      buf[2] = *value & 0xff;
      buf[3] = *value >> 8;
    }

  msg.frequency = priv->config->frequency;
  msg.addr      = priv->config->address;
  msg.flags     = 0;
  msg.buffer    = buf;
  msg.length    = value != NULL ? 4 : 2;

  return I2C_TRANSFER(priv->i2c, &msg, 1);
}

/****************************************************************************
 * Name: zt2628_read
 *
 * Description:
 *   Write a register address then read len bytes from it.
 *
 ****************************************************************************/

static int zt2628_read(FAR struct zt2628_dev_s *priv, uint16_t reg,
                       FAR uint8_t *data, size_t len)
{
  struct i2c_msg_s msg[2];
  uint8_t addr[2];

  addr[0] = reg & 0xff;
  addr[1] = reg >> 8;

  msg[0].frequency = priv->config->frequency;
  msg[0].addr      = priv->config->address;
  msg[0].flags     = 0;
  msg[0].buffer    = addr;
  msg[0].length    = 2;

  msg[1].frequency = priv->config->frequency;
  msg[1].addr      = priv->config->address;
  msg[1].flags     = I2C_M_READ;
  msg[1].buffer    = data;
  msg[1].length    = len;

  return I2C_TRANSFER(priv->i2c, msg, 2);
}

/****************************************************************************
 * Name: zt2628_read16
 ****************************************************************************/

static int zt2628_read16(FAR struct zt2628_dev_s *priv, uint16_t reg,
                         FAR uint16_t *value)
{
  uint8_t buf[2];
  int ret;

  ret = zt2628_read(priv, reg, buf, 2);
  if (ret >= 0)
    {
      *value = buf[0] | (buf[1] << 8);
    }

  return ret;
}

/****************************************************************************
 * Name: zt2628_init
 *
 * Description:
 *   Vendor start-up sequence: power the analogue front end, verify the
 *   chip code, release the firmware, then configure point mode and the
 *   interrupt sources.
 *
 ****************************************************************************/

static int zt2628_init(FAR struct zt2628_dev_s *priv)
{
  uint16_t value;
  uint16_t code;
  int ret;
  int i;

  value = 1;
  ret = zt2628_write(priv, ZT2628_CMD_POWER_ON, &value);
  if (ret < 0)
    {
      ierr("ZT2628 does not answer: %d\n", ret);
      return ret;
    }

  up_udelay(10);

  ret = zt2628_read16(priv, ZT2628_REG_CHIP_CODE, &code);
  if (ret < 0 || code != ZT2628_CHIP_CODE)
    {
      ierr("ZT2628 chip code %04x (%d)\n", code, ret);
      return ret < 0 ? ret : -ENODEV;
    }

  zt2628_write(priv, ZT2628_CMD_NVM_VPP, NULL);
  up_udelay(10);
  value = 1;
  zt2628_write(priv, ZT2628_CMD_NVM_INIT, &value);
  up_udelay(2000);
  value = 1;
  zt2628_write(priv, ZT2628_CMD_NVM_WP, &value);

  /* Firmware start-up and checksum */

  nxsig_usleep(150 * 1000);

  zt2628_write(priv, ZT2628_CMD_SWRESET, NULL);

  value = 0;                                   /* Point mode */
  zt2628_write(priv, ZT2628_REG_TOUCH_MODE, &value);
  value = ZT2628_COVER_OPEN;
  zt2628_write(priv, ZT2628_REG_COVER_CONTROL, &value);
  value = ZT2628_INT_PT_CNT_CHANGE | ZT2628_INT_DOWN |
          ZT2628_INT_MOVE | ZT2628_INT_UP;
  zt2628_write(priv, ZT2628_REG_INT_EN, &value);

  for (i = 0; i < 10; i++)
    {
      zt2628_write(priv, ZT2628_CMD_CLEAR_INT, NULL);
      up_udelay(10);
    }

  zt2628_read16(priv, ZT2628_REG_FIRMWARE_VERSION, &value);
  iinfo("ZT2628 firmware %04x\n", value);
  return OK;
}

/****************************************************************************
 * Name: zt2628_worker
 *
 * Description:
 *   Read the point status block after an interrupt and report it.
 *
 ****************************************************************************/

static void zt2628_worker(FAR void *arg)
{
  FAR struct zt2628_dev_s *priv = (FAR struct zt2628_dev_s *)arg;
  struct touch_sample_s sample;
  uint8_t buf[ZT2628_STATUS_LEN];
  uint16_t status;
  uint16_t x;
  uint16_t y;
  uint8_t pstatus;
  bool pressed;
  int ret;

  ret = zt2628_read(priv, ZT2628_REG_POINT_STATUS, buf, sizeof(buf));
  zt2628_write(priv, ZT2628_CMD_CLEAR_INT, NULL);
  priv->config->enable(priv->config, true);

  if (ret < 0)
    {
      ierr("Point read failed: %d\n", ret);
      return;
    }

  status  = buf[0] | (buf[1] << 8);
  x       = buf[4] | (buf[5] << 8);
  y       = buf[6] | (buf[7] << 8);
  pstatus = buf[9];

  if ((status & ZT2628_INT_MUST_ZERO) != 0)
    {
      ierr("Bad point status %04x\n", status);
      return;
    }

  pressed = (pstatus & ZT2628_POINT_EXIST) != 0 &&
            (pstatus & (ZT2628_POINT_DOWN | ZT2628_POINT_MOVE)) != 0;

  if (!pressed && !priv->pressed)
    {
      return;
    }

  if (x >= priv->config->xres)
    {
      x = priv->config->xres - 1;
    }

  if (y >= priv->config->yres)
    {
      y = priv->config->yres - 1;
    }

  memset(&sample, 0, sizeof(sample));
  sample.npoints           = 1;
  sample.point[0].id       = 0;
  sample.point[0].x        = x;
  sample.point[0].y        = y;
  sample.point[0].w        = buf[8];
  sample.point[0].h        = buf[8];
  sample.point[0].timestamp = clock_systime_ticks() * USEC_PER_TICK;
  sample.point[0].flags    = TOUCH_ID_VALID | TOUCH_POS_VALID;

  if (pressed)
    {
      sample.point[0].flags |= priv->pressed ? TOUCH_MOVE : TOUCH_DOWN;
    }
  else
    {
      sample.point[0].flags |= TOUCH_UP;
    }

  priv->pressed = pressed;
  touch_event(priv->lower.priv, &sample);
}

/****************************************************************************
 * Name: zt2628_interrupt
 ****************************************************************************/

static int zt2628_interrupt(int irq, FAR void *context, FAR void *arg)
{
  FAR struct zt2628_dev_s *priv = (FAR struct zt2628_dev_s *)arg;

  priv->config->enable(priv->config, false);
  if (work_available(&priv->work))
    {
      work_queue(LPWORK, &priv->work, zt2628_worker, priv, 0);
    }

  return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: zt2628_register
 ****************************************************************************/

int zt2628_register(FAR struct i2c_master_s *i2c,
                    FAR const struct zt2628_config_s *config, int minor)
{
  FAR struct zt2628_dev_s *priv;
  char path[16];
  int ret;

  if (i2c == NULL || config == NULL || config->attach == NULL ||
      config->enable == NULL)
    {
      return -EINVAL;
    }

  priv = kmm_zalloc(sizeof(struct zt2628_dev_s));
  if (priv == NULL)
    {
      return -ENOMEM;
    }

  priv->i2c            = i2c;
  priv->config         = config;
  priv->lower.maxpoint = 1;

  /* Supply and reset, then the firmware boot delay */

  if (config->power != NULL)
    {
      config->power(config, true);
      up_udelay(1500);
    }

  if (config->reset != NULL)
    {
      config->reset(config, true);
      up_udelay(1000);
      config->reset(config, false);
      nxsig_usleep(50 * 1000);
    }

  ret = zt2628_init(priv);
  if (ret < 0)
    {
      goto errout;
    }

  snprintf(path, sizeof(path), "/dev/input%d", minor);
  ret = touch_register(&priv->lower, path,
                       CONFIG_INPUT_ZT2628_NPOLLWAITERS);
  if (ret < 0)
    {
      goto errout;
    }

  ret = config->attach(config, zt2628_interrupt, priv);
  if (ret < 0)
    {
      touch_unregister(&priv->lower, path);
      goto errout;
    }

  config->enable(config, true);
  return OK;

errout:
  kmm_free(priv);
  return ret;
}
