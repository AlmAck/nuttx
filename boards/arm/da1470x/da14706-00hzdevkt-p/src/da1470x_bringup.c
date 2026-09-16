/****************************************************************************
 * boards/arm/da1470x/da14706-00hzdevkt-p/src/da1470x_bringup.c
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <sys/types.h>
#include <debug.h>
#include <errno.h>

#include <nuttx/board.h>
#include <nuttx/fs/fs.h>

#ifdef CONFIG_USERLED
#  include <nuttx/leds/userled.h>
#endif

#ifdef CONFIG_RTC_DRIVER
#  include <nuttx/timers/rtc.h>
#endif

#ifdef CONFIG_VIDEO_FB
#  include <nuttx/video/fb.h>
#endif

#ifdef CONFIG_DA1470X_OQSPI_MTD
#  include <nuttx/mtd/mtd.h>
#  ifdef CONFIG_FS_NXFFS
#    include <nuttx/fs/nxffs.h>
#  endif
#endif

#include "da1470x_gpio.h"
#include "da1470x_dma.h"
#include "da1470x_rtc.h"
#include "da1470x_pdc.h"
#include "da1470x_oqspi.h"
#include "da1470x_gpu.h"
#include "da1470x_bt.h"
#include "da14706-00hzdevkt-p.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_flash_initialize
 *
 * Description:
 *   Carve the data partition out of the boot flash and expose it as
 *   /dev/mtd0 (character), /dev/mtdblock0 (FTL) and, when NXFFS is
 *   enabled, mount it.
 *
 ****************************************************************************/

#ifdef CONFIG_DA1470X_OQSPI_MTD
static int da1470x_flash_initialize(void)
{
  struct mtd_dev_s *mtd;
  struct mtd_dev_s *part;
  off_t firstblock;
  off_t nblocks;
  int ret;

  mtd = da1470x_oqspi_initialize();
  if (mtd == NULL)
    {
      return -ENODEV;
    }

  firstblock = CONFIG_DA1470X_OQSPI_MTD_OFFSET / DA1470X_OQSPI_PAGE_SIZE;
  nblocks    = CONFIG_DA1470X_OQSPI_MTD_SIZE / DA1470X_OQSPI_PAGE_SIZE;

  part = mtd_partition(mtd, firstblock, nblocks);
  if (part == NULL)
    {
      return -ENOMEM;
    }

  ret = register_mtddriver("/dev/mtd0", part, 0755, NULL);
  if (ret < 0)
    {
      syslog(LOG_ERR, "Failed to register /dev/mtd0: %d\n", ret);
    }

  ret = ftl_initialize(0, part);
  if (ret < 0)
    {
      syslog(LOG_ERR, "Failed to initialize FTL: %d\n", ret);
      return ret;
    }

#ifdef CONFIG_FS_NXFFS
  ret = nxffs_initialize(part);
  if (ret < 0)
    {
      syslog(LOG_ERR, "NXFFS initialization failed: %d\n", ret);
      return ret;
    }

  ret = nx_mount(NULL, CONFIG_DA14706_OQSPI_MOUNTPOINT, "nxffs", 0, NULL);
  if (ret < 0)
    {
      syslog(LOG_ERR, "Failed to mount NXFFS at %s: %d\n",
             CONFIG_DA14706_OQSPI_MOUNTPOINT, ret);
      return ret;
    }
#endif

  return OK;
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_bringup
 *
 * Description:
 *   Bring up board features in dependency order and register the device
 *   nodes under /dev.
 *
 ****************************************************************************/

int da1470x_bringup(void)
{
  int ret = OK;

#ifdef CONFIG_FS_PROCFS
  ret = nx_mount(NULL, "/proc", "procfs", 0, NULL);
  if (ret < 0)
    {
      syslog(LOG_ERR, "Failed to mount procfs: %d\n", ret);
    }
#endif

#ifdef CONFIG_DA1470X_GPIO_IRQ
  da1470x_gpioirq_initialize();
#endif

#ifdef CONFIG_DA1470X_DMA
  da1470x_dma_initialize();
#endif

#ifdef CONFIG_DEV_GPIO
  ret = da1470x_gpio_initialize();
  if (ret < 0)
    {
      syslog(LOG_ERR, "Failed to initialize GPIO devices: %d\n", ret);
    }
#endif

#ifdef CONFIG_USERLED_LOWER
  ret = userled_lower_initialize("/dev/userleds");
  if (ret < 0)
    {
      syslog(LOG_ERR, "Failed to initialize user LEDs: %d\n", ret);
    }
#endif

#ifdef CONFIG_RTC_DRIVER
  ret = rtc_initialize(0, da1470x_rtc_lowerhalf());
  if (ret < 0)
    {
      syslog(LOG_ERR, "Failed to register /dev/rtc0: %d\n", ret);
    }
#endif

#ifdef CONFIG_DA1470X_PDC
  /* Wake-up sources for the low-power states: RTC alarm and button K1 */

  da1470x_pdc_add(DA1470X_PDC_TRIG_PERIPHERAL, DA1470X_PDC_PERIPH_RTC_ALARM,
                  DA1470X_PDC_MASTER_CM33, DA1470X_PDC_FLAG_EN_XTAL);
  da1470x_pdc_add(DA1470X_PDC_TRIG_P1_GPIO, GPIO_PIN_DECODE(GPIO_BUTTON1),
                  DA1470X_PDC_MASTER_CM33, DA1470X_PDC_FLAG_EN_XTAL);
#endif

#ifdef CONFIG_DA1470X_SPI
  ret = da1470x_spidev_initialize();
  if (ret < 0)
    {
      syslog(LOG_ERR, "Failed to initialize SPI: %d\n", ret);
    }
#endif

#ifdef CONFIG_DA1470X_I2C
  ret = da1470x_i2cdev_initialize();
  if (ret < 0)
    {
      syslog(LOG_ERR, "Failed to initialize I2C: %d\n", ret);
    }
#endif

#ifdef CONFIG_DA1470X_OQSPI_MTD
  ret = da1470x_flash_initialize();
  if (ret < 0)
    {
      syslog(LOG_ERR, "Failed to initialize the flash partition: %d\n", ret);
    }
#endif

#ifdef CONFIG_DA14706_LCD_E120A390QSR
  ret = da1470x_e120a390_initialize();
  if (ret < 0)
    {
      syslog(LOG_ERR, "Failed to initialize the display: %d\n", ret);
    }
  else
    {
      ret = fb_register(0, 0);
      if (ret < 0)
        {
          syslog(LOG_ERR, "Failed to register /dev/fb0: %d\n", ret);
        }
    }
#endif

#ifdef CONFIG_DA1470X_PWMLED
  ret = da1470x_pwmled_setup();
  if (ret < 0)
    {
      syslog(LOG_ERR, "Failed to register the PWM LED driver: %d\n", ret);
    }
#endif

#ifdef CONFIG_DA1470X_GPU
  ret = da1470x_gpu_register("/dev/gpu0");
  if (ret < 0)
    {
      syslog(LOG_ERR, "Failed to register /dev/gpu0: %d\n", ret);
    }
#endif

#ifdef CONFIG_DA1470X_BLE
  ret = da1470x_bt_initialize(NULL);
  if (ret < 0)
    {
      syslog(LOG_ERR, "Failed to register the Bluetooth driver: %d\n", ret);
    }
#endif

  UNUSED(ret);
  return OK;
}
