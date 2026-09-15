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

#include "da1470x_gpio.h"
#include "da1470x_dma.h"
#include "da1470x_rtc.h"
#include "da1470x_pdc.h"
#include "da14706-00hzdevkt-p.h"

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

#ifdef CONFIG_DA14706_LCD_E120A390QSR
  ret = da1470x_e120a390_initialize();
  if (ret < 0)
    {
      syslog(LOG_ERR, "Failed to initialize the display: %d\n", ret);
    }
#endif

  UNUSED(ret);
  return OK;
}
