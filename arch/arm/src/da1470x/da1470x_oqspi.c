/****************************************************************************
 * arch/arm/src/da1470x/da1470x_oqspi.c
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

/* MTD driver for the boot flash behind the OQSPI controller.
 *
 * NuttX executes in place from this flash through the controller's auto
 * (memory-mapped) mode.  Erasing and programming need the controller in
 * manual mode, during which the XiP window is unavailable: every function
 * that runs in that window therefore lives in .ramfunc, touches registers
 * only through inline accessors, and runs with interrupts masked.  Reads
 * are plain memory copies from the XiP window.
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
#include <nuttx/mtd/mtd.h>

#include "arm_internal.h"
#include "hardware/da1470x_oqspif.h"
#include "hardware/da1470x_cache.h"
#include "hardware/da1470x_crg_top.h"
#include "da1470x_oqspi.h"

#ifdef CONFIG_DA1470X_OQSPI_MTD

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Flash opcodes (Macronix MX25U6432, quad mode already enabled by the
 * boot ROM configuration)
 */

#define FLASH_CMD_WREN            0x06  /* Write enable */
#define FLASH_CMD_RDSR            0x05  /* Read status register */
#define FLASH_CMD_PP4             0x38  /* Quad I/O page program */
#define FLASH_CMD_SE              0x20  /* 4 KiB sector erase */
#define FLASH_CMD_EXIT_CONT       0xffffffff

#define FLASH_SR_WIP              (1 << 0)
#define FLASH_SR_WEL              (1 << 1)

#define OQSPI_NPAGES              (DA1470X_OQSPI_FLASH_SIZE / \
                                   DA1470X_OQSPI_PAGE_SIZE)
#define OQSPI_NSECTORS            (DA1470X_OQSPI_FLASH_SIZE / \
                                   DA1470X_OQSPI_SECTOR_SIZE)

/* Bounded busy waits (loop counts).  A sector erase takes up to ~100 ms. */

#define OQSPI_BUSY_LOOPS          4000000

/* Byte-wide views of the data registers */

#define OQSPIF_WRITEDATA8         (DA1470X_OQSPIF_WRITEDATA)
#define OQSPIF_READDATA8          (DA1470X_OQSPIF_READDATA)

/* Code that must run from RAM while the XiP window is down */

#define RAMFUNC                   locate_code(".ramfunc") noinline_function

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct da1470x_oqspi_dev_s
{
  struct mtd_dev_s mtd;   /* MTD interface, must be first */
  mutex_t lock;           /* Serialises flash operations */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int     oqspi_erase(struct mtd_dev_s *dev, off_t startblock,
                           size_t nblocks);
static ssize_t oqspi_bread(struct mtd_dev_s *dev, off_t startblock,
                           size_t nblocks, uint8_t *buffer);
static ssize_t oqspi_bwrite(struct mtd_dev_s *dev, off_t startblock,
                            size_t nblocks, const uint8_t *buffer);
static ssize_t oqspi_read(struct mtd_dev_s *dev, off_t offset,
                          size_t nbytes, uint8_t *buffer);
#ifdef CONFIG_MTD_BYTE_WRITE
static ssize_t oqspi_write(struct mtd_dev_s *dev, off_t offset,
                           size_t nbytes, const uint8_t *buffer);
#endif
static int     oqspi_ioctl(struct mtd_dev_s *dev, int cmd,
                           unsigned long arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct da1470x_oqspi_dev_s g_oqspi =
{
  .mtd =
  {
    .erase  = oqspi_erase,
    .bread  = oqspi_bread,
    .bwrite = oqspi_bwrite,
    .read   = oqspi_read,
#ifdef CONFIG_MTD_BYTE_WRITE
    .write  = oqspi_write,
#endif
    .ioctl  = oqspi_ioctl,
    .name   = "mx25u6432"
  },
  .lock = NXMUTEX_INITIALIZER,
};

/****************************************************************************
 * Private Functions (RAM resident)
 ****************************************************************************/

/****************************************************************************
 * Name: oqspi_ram_setbus
 *
 * Description:
 *   Select single or quad lanes for the next manual transfer and drive
 *   IO2/IO3 accordingly (they are WP#/HOLD# in single mode).
 *
 ****************************************************************************/

static RAMFUNC void oqspi_ram_setbus(bool quad)
{
  uint32_t mode = getreg32(DA1470X_OQSPIF_CTRLMODE);

  if (quad)
    {
      putreg32(OQSPIF_CTRLBUS_OSPIC_SET_QUAD, DA1470X_OQSPIF_CTRLBUS);
      mode &= ~(OQSPIF_CTRLMODE_OSPIC_IO2_OEN | OQSPIF_CTRLMODE_OSPIC_IO2_DAT |
                OQSPIF_CTRLMODE_OSPIC_IO3_OEN | OQSPIF_CTRLMODE_OSPIC_IO3_DAT);
    }
  else
    {
      putreg32(OQSPIF_CTRLBUS_OSPIC_SET_SINGLE, DA1470X_OQSPIF_CTRLBUS);
      mode |= OQSPIF_CTRLMODE_OSPIC_IO2_OEN | OQSPIF_CTRLMODE_OSPIC_IO2_DAT |
              OQSPIF_CTRLMODE_OSPIC_IO3_OEN | OQSPIF_CTRLMODE_OSPIC_IO3_DAT;
    }

  mode &= ~OQSPIF_CTRLMODE_OSPIC_IO_UH_OEN;
  putreg32(mode, DA1470X_OQSPIF_CTRLMODE);
}

/****************************************************************************
 * Name: oqspi_ram_status
 *
 * Description:
 *   Read the flash status register in single-lane mode.
 *
 ****************************************************************************/

static RAMFUNC uint8_t oqspi_ram_status(void)
{
  uint8_t status;

  oqspi_ram_setbus(false);
  putreg32(OQSPIF_CTRLBUS_OSPIC_EN_CS, DA1470X_OQSPIF_CTRLBUS);
  putreg8(FLASH_CMD_RDSR, OQSPIF_WRITEDATA8);
  status = getreg8(OQSPIF_READDATA8);
  putreg32(OQSPIF_CTRLBUS_OSPIC_DIS_CS, DA1470X_OQSPIF_CTRLBUS);
  return status;
}

/****************************************************************************
 * Name: oqspi_ram_wait_ready
 *
 * Description:
 *   Poll WIP until the flash is idle.  Returns false on timeout.
 *
 ****************************************************************************/

static RAMFUNC bool oqspi_ram_wait_ready(void)
{
  int i;

  for (i = 0; i < OQSPI_BUSY_LOOPS; i++)
    {
      if ((oqspi_ram_status() & FLASH_SR_WIP) == 0)
        {
          return true;
        }
    }

  return false;
}

/****************************************************************************
 * Name: oqspi_ram_write_enable
 ****************************************************************************/

static RAMFUNC bool oqspi_ram_write_enable(void)
{
  int i;

  for (i = 0; i < 16; i++)
    {
      oqspi_ram_setbus(false);
      putreg32(OQSPIF_CTRLBUS_OSPIC_EN_CS, DA1470X_OQSPIF_CTRLBUS);
      putreg8(FLASH_CMD_WREN, OQSPIF_WRITEDATA8);
      putreg32(OQSPIF_CTRLBUS_OSPIC_DIS_CS, DA1470X_OQSPIF_CTRLBUS);

      if ((oqspi_ram_status() & FLASH_SR_WEL) != 0)
        {
          return true;
        }
    }

  return false;
}

/****************************************************************************
 * Name: oqspi_ram_enter_manual
 *
 * Description:
 *   Leave auto (XiP) mode and take the flash out of continuous read mode
 *   by clocking 0xFF on all four lanes where the read command expects
 *   the mode byte.
 *
 ****************************************************************************/

static RAMFUNC void oqspi_ram_enter_manual(void)
{
  uint32_t mode = getreg32(DA1470X_OQSPIF_CTRLMODE);

  putreg32(mode & ~OQSPIF_CTRLMODE_OSPIC_AUTO_MD, DA1470X_OQSPIF_CTRLMODE);

  oqspi_ram_setbus(true);
  putreg32(OQSPIF_CTRLBUS_OSPIC_EN_CS, DA1470X_OQSPIF_CTRLBUS);
  putreg32(FLASH_CMD_EXIT_CONT, DA1470X_OQSPIF_WRITEDATA);
  putreg32(OQSPIF_CTRLBUS_OSPIC_DIS_CS, DA1470X_OQSPIF_CTRLBUS);
}

/****************************************************************************
 * Name: oqspi_ram_enter_auto
 *
 * Description:
 *   Hand the pins back to the controller (quad lanes) and re-enable the
 *   memory-mapped auto mode.
 *
 ****************************************************************************/

static RAMFUNC void oqspi_ram_enter_auto(void)
{
  uint32_t mode;

  oqspi_ram_setbus(true);
  mode = getreg32(DA1470X_OQSPIF_CTRLMODE);
  putreg32(mode | OQSPIF_CTRLMODE_OSPIC_AUTO_MD, DA1470X_OQSPIF_CTRLMODE);
}

/****************************************************************************
 * Name: oqspi_ram_program_page
 *
 * Description:
 *   Program up to one page (never crossing a page boundary) with the quad
 *   I/O page program command: opcode on one lane, address and data on
 *   four.  Runs entirely from RAM with interrupts masked.
 *
 ****************************************************************************/

static RAMFUNC int oqspi_ram_program_page(uint32_t addr,
                                          const uint8_t *buf,
                                          size_t len)
{
  irqstate_t flags;
  size_t i;
  int ret = OK;

  flags = up_irq_save();

  oqspi_ram_enter_manual();

  if (!oqspi_ram_wait_ready() || !oqspi_ram_write_enable())
    {
      ret = -ETIMEDOUT;
      goto out;
    }

  oqspi_ram_setbus(false);
  putreg32(OQSPIF_CTRLBUS_OSPIC_EN_CS, DA1470X_OQSPIF_CTRLBUS);
  putreg8(FLASH_CMD_PP4, OQSPIF_WRITEDATA8);

  oqspi_ram_setbus(true);
  putreg8((addr >> 16) & 0xff, OQSPIF_WRITEDATA8);
  putreg8((addr >> 8) & 0xff, OQSPIF_WRITEDATA8);
  putreg8(addr & 0xff, OQSPIF_WRITEDATA8);

  for (i = 0; i < len; i++)
    {
      putreg8(buf[i], OQSPIF_WRITEDATA8);
    }

  putreg32(OQSPIF_CTRLBUS_OSPIC_DIS_CS, DA1470X_OQSPIF_CTRLBUS);

  if (!oqspi_ram_wait_ready())
    {
      ret = -ETIMEDOUT;
    }

out:
  oqspi_ram_enter_auto();
  up_irq_restore(flags);
  return ret;
}

/****************************************************************************
 * Name: oqspi_ram_erase_sector
 ****************************************************************************/

static RAMFUNC int oqspi_ram_erase_sector(uint32_t addr)
{
  irqstate_t flags;
  int ret = OK;

  flags = up_irq_save();

  oqspi_ram_enter_manual();

  if (!oqspi_ram_wait_ready() || !oqspi_ram_write_enable())
    {
      ret = -ETIMEDOUT;
      goto out;
    }

  oqspi_ram_setbus(false);
  putreg32(OQSPIF_CTRLBUS_OSPIC_EN_CS, DA1470X_OQSPIF_CTRLBUS);
  putreg8(FLASH_CMD_SE, OQSPIF_WRITEDATA8);
  putreg8((addr >> 16) & 0xff, OQSPIF_WRITEDATA8);
  putreg8((addr >> 8) & 0xff, OQSPIF_WRITEDATA8);
  putreg8(addr & 0xff, OQSPIF_WRITEDATA8);
  putreg32(OQSPIF_CTRLBUS_OSPIC_DIS_CS, DA1470X_OQSPIF_CTRLBUS);

  if (!oqspi_ram_wait_ready())
    {
      ret = -ETIMEDOUT;
    }

out:
  oqspi_ram_enter_auto();
  up_irq_restore(flags);
  return ret;
}

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: oqspi_cache_flush
 *
 * Description:
 *   Invalidate the instruction cache after the flash contents changed.
 *   The cache is flushed by toggling its RAM mux and waiting for FLUSHED.
 *
 ****************************************************************************/

static void oqspi_cache_flush(void)
{
  int i;

  if ((getreg32(DA1470X_CACHE_CTRL2) & CACHE_CTRL2_FLUSH_DISABLE) != 0)
    {
      return;
    }

  modifyreg32(DA1470X_CRG_TOP_SYS_CTRL, CRG_TOP_SYS_CTRL_CACHERAM_MUX, 0);
  modifyreg32(DA1470X_CRG_TOP_SYS_CTRL, 0, CRG_TOP_SYS_CTRL_CACHERAM_MUX);

  for (i = 0; i < 100000; i++)
    {
      if ((getreg32(DA1470X_CACHE_CTRL2) & CACHE_CTRL2_FLUSHED) != 0)
        {
          break;
        }
    }

  modifyreg32(DA1470X_CACHE_CTRL2, CACHE_CTRL2_FLUSHED, 0);
}

/****************************************************************************
 * Name: oqspi_erase
 ****************************************************************************/

static int oqspi_erase(struct mtd_dev_s *dev, off_t startblock,
                       size_t nblocks)
{
  struct da1470x_oqspi_dev_s *priv = (struct da1470x_oqspi_dev_s *)dev;
  size_t i;
  int ret;

  if (startblock + nblocks > OQSPI_NSECTORS)
    {
      return -EINVAL;
    }

  ret = nxmutex_lock(&priv->lock);
  if (ret < 0)
    {
      return ret;
    }

  for (i = 0; i < nblocks; i++)
    {
      ret = oqspi_ram_erase_sector((startblock + i) *
                                   DA1470X_OQSPI_SECTOR_SIZE);
      if (ret < 0)
        {
          ferr("Erase of sector %lu failed: %d\n",
               (unsigned long)(startblock + i), ret);
          break;
        }
    }

  oqspi_cache_flush();
  nxmutex_unlock(&priv->lock);
  return ret < 0 ? ret : (int)nblocks;
}

/****************************************************************************
 * Name: oqspi_read
 ****************************************************************************/

static ssize_t oqspi_read(struct mtd_dev_s *dev, off_t offset,
                          size_t nbytes, uint8_t *buffer)
{
  struct da1470x_oqspi_dev_s *priv = (struct da1470x_oqspi_dev_s *)dev;
  int ret;

  if (offset + nbytes > DA1470X_OQSPI_FLASH_SIZE)
    {
      return -EINVAL;
    }

  ret = nxmutex_lock(&priv->lock);
  if (ret < 0)
    {
      return ret;
    }

  memcpy(buffer, (const void *)(DA1470X_OQSPIF_MEM_BASE + offset), nbytes);

  nxmutex_unlock(&priv->lock);
  return nbytes;
}

/****************************************************************************
 * Name: oqspi_bread
 ****************************************************************************/

static ssize_t oqspi_bread(struct mtd_dev_s *dev, off_t startblock,
                           size_t nblocks, uint8_t *buffer)
{
  ssize_t nbytes = oqspi_read(dev, startblock * DA1470X_OQSPI_PAGE_SIZE,
                              nblocks * DA1470X_OQSPI_PAGE_SIZE, buffer);

  return nbytes < 0 ? nbytes : nbytes / DA1470X_OQSPI_PAGE_SIZE;
}

/****************************************************************************
 * Name: oqspi_write_locked
 *
 * Description:
 *   Program an arbitrary byte range, page by page.  The lock must be held.
 *
 ****************************************************************************/

static ssize_t oqspi_write_locked(off_t offset, size_t nbytes,
                                  const uint8_t *buffer)
{
  size_t remaining = nbytes;
  int ret = OK;

  while (remaining > 0)
    {
      size_t chunk = DA1470X_OQSPI_PAGE_SIZE -
                     (offset & (DA1470X_OQSPI_PAGE_SIZE - 1));

      if (chunk > remaining)
        {
          chunk = remaining;
        }

      ret = oqspi_ram_program_page(offset, buffer, chunk);
      if (ret < 0)
        {
          ferr("Program at %08lx failed: %d\n", (unsigned long)offset, ret);
          break;
        }

      offset    += chunk;
      buffer    += chunk;
      remaining -= chunk;
    }

  oqspi_cache_flush();
  return ret < 0 ? ret : (ssize_t)nbytes;
}

/****************************************************************************
 * Name: oqspi_bwrite
 ****************************************************************************/

static ssize_t oqspi_bwrite(struct mtd_dev_s *dev, off_t startblock,
                            size_t nblocks, const uint8_t *buffer)
{
  struct da1470x_oqspi_dev_s *priv = (struct da1470x_oqspi_dev_s *)dev;
  ssize_t nbytes;
  int ret;

  if (startblock + nblocks > OQSPI_NPAGES)
    {
      return -EINVAL;
    }

  ret = nxmutex_lock(&priv->lock);
  if (ret < 0)
    {
      return ret;
    }

  nbytes = oqspi_write_locked(startblock * DA1470X_OQSPI_PAGE_SIZE,
                              nblocks * DA1470X_OQSPI_PAGE_SIZE, buffer);

  nxmutex_unlock(&priv->lock);
  return nbytes < 0 ? nbytes : (ssize_t)nblocks;
}

/****************************************************************************
 * Name: oqspi_write
 ****************************************************************************/

#ifdef CONFIG_MTD_BYTE_WRITE
static ssize_t oqspi_write(struct mtd_dev_s *dev, off_t offset,
                           size_t nbytes, const uint8_t *buffer)
{
  struct da1470x_oqspi_dev_s *priv = (struct da1470x_oqspi_dev_s *)dev;
  ssize_t ret;

  if (offset + nbytes > DA1470X_OQSPI_FLASH_SIZE)
    {
      return -EINVAL;
    }

  ret = nxmutex_lock(&priv->lock);
  if (ret < 0)
    {
      return ret;
    }

  ret = oqspi_write_locked(offset, nbytes, buffer);

  nxmutex_unlock(&priv->lock);
  return ret;
}
#endif

/****************************************************************************
 * Name: oqspi_ioctl
 ****************************************************************************/

static int oqspi_ioctl(struct mtd_dev_s *dev, int cmd, unsigned long arg)
{
  int ret = -ENOTTY;

  switch (cmd)
    {
      case MTDIOC_GEOMETRY:
        {
          struct mtd_geometry_s *geo = (struct mtd_geometry_s *)arg;

          if (geo != NULL)
            {
              memset(geo, 0, sizeof(*geo));
              geo->blocksize    = DA1470X_OQSPI_PAGE_SIZE;
              geo->erasesize    = DA1470X_OQSPI_SECTOR_SIZE;
              geo->neraseblocks = OQSPI_NSECTORS;
              strlcpy(geo->model, "mx25u6432", sizeof(geo->model));
              ret = OK;
            }
        }
        break;

      case MTDIOC_ERASESTATE:
        {
          uint8_t *state = (uint8_t *)arg;

          if (state != NULL)
            {
              *state = 0xff;
              ret = OK;
            }
        }
        break;

      case MTDIOC_BULKERASE:

        /* Never erase the whole device: it holds the running image */

        ret = -EPERM;
        break;

      default:
        break;
    }

  return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_oqspi_initialize
 ****************************************************************************/

struct mtd_dev_s *da1470x_oqspi_initialize(void)
{
  return &g_oqspi.mtd;
}

#endif /* CONFIG_DA1470X_OQSPI_MTD */
