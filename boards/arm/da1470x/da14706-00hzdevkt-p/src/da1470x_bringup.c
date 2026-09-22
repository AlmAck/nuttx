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

#include <nuttx/arch.h>
#include <nuttx/board.h>
#include <arch/board/board.h>
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
#include "da1470x_snc.h"
#include "da1470x_wdt.h"
#include <nuttx/timers/pwm.h>

#include "da1470x_charger.h"
#include "da1470x_gpadc.h"
#include "da1470x_pwm.h"
#include "da1470x_psram.h"
#include "da1470x_oqspi.h"
#include "da1470x_gpu.h"
#include "da1470x_bt.h"
#include "da14706-00hzdevkt-p.h"

/****************************************************************************
 * Name: da1470x_button_wakeup
 *
 * Description:
 *   Arm a button so that pressing it wakes the system.  The handler does
 *   nothing: the point of the interrupt is that arming it is what puts the
 *   pin into the wake-up block and registers its power domain controller
 *   entry.  Whoever cares about the press attaches over the top.
 *
 ****************************************************************************/

#if defined(CONFIG_DA1470X_PDC) && defined(CONFIG_DA1470X_GPIO_IRQ)
static int da1470x_button_isr(int irq, void *context, void *arg)
{
  return OK;
}

static void da1470x_button_wakeup(da1470x_pinset_t pinset)
{
  da1470x_gpio_config(pinset);
  da1470x_gpioirq_attach(pinset, DA1470X_GPIO_EDGE_FALLING,
                         da1470x_button_isr, NULL);
  da1470x_gpioirq_enable(pinset);
}
#endif

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

/* On this kit SPI0 is wired to P1.13 and P1.14, which are also two of the
 * PSRAM's data lines.  Both enabled would put two controllers on the same
 * wires, and the PSRAM would fail its line test at best.  Refuse at build
 * time rather than let it fail on the hardware.
 */

#if defined(CONFIG_DA1470X_PSRAM) && defined(CONFIG_DA1470X_SPI0)
#  error "DA1470X_PSRAM and DA1470X_SPI0 share P1.13/P1.14 on this board"
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

#ifdef CONFIG_DA1470X_GPADC
static const uint8_t g_adc_channels[] =
{
  DA1470X_ADC_VBAT,
  DA1470X_ADC_VSYS,
  DA1470X_ADC_VBUS,
  DA1470X_ADC_P0_5,
};
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
  /* Wake-up sources for the low-power states.  The RTC alarm is a
   * peripheral trigger and stands on its own; the buttons are pins, and a
   * pin only becomes a wake-up source once its interrupt is armed, which
   * is what registers the power domain controller entry alongside the
   * wake-up block configuration.  Arming them here means a press wakes the
   * board whether or not anything has opened /dev/gpio1; a driver that
   * later attaches to the pin simply replaces the handler.
   */

  da1470x_pdc_add(DA1470X_PDC_TRIG_PERIPHERAL, DA1470X_PDC_PERIPH_RTC_ALARM,
                  DA1470X_PDC_MASTER_CM33, DA1470X_PDC_FLAG_EN_XTAL);

#ifdef CONFIG_DA1470X_GPIO_IRQ
  da1470x_button_wakeup(GPIO_BUTTON1);
  da1470x_button_wakeup(GPIO_BUTTON2);
#endif
#endif

#ifdef CONFIG_DA1470X_CHARGER
  ret = da1470x_charger_initialize("/dev/charger0");
  if (ret < 0)
    {
      syslog(LOG_ERR, "Failed to register /dev/charger0: %d\n", ret);
    }
#endif

#ifdef CONFIG_DA1470X_PSRAM
  /* Early, before anything might want to allocate from it.  A missing or
   * faulty part is reported and skipped; the board is still usable.
   */

  /* Power the PSRAM and connect it: see BOARD_RAM_PWRON_PIN.  Then give
   * the load switch time to turn on and the part the 150 us it needs after
   * its supply is stable before the first command.
   */

  da1470x_gpio_config(BOARD_RAM_PWRON_PIN);
  up_mdelay(2);

  da1470x_gpio_config(BOARD_QSPIC2_CS_PIN);
  da1470x_gpio_config(BOARD_QSPIC2_CLK_PIN);
  da1470x_gpio_config(BOARD_QSPIC2_D0_PIN);
  da1470x_gpio_config(BOARD_QSPIC2_D1_PIN);
  da1470x_gpio_config(BOARD_QSPIC2_D2_PIN);
  da1470x_gpio_config(BOARD_QSPIC2_D3_PIN);

  ret = da1470x_psram_initialize();
  if (ret < 0)
    {
      syslog(LOG_WARNING, "PSRAM not available: %d\n", ret);
    }
#endif

#ifdef CONFIG_DA1470X_PWM
  {
    struct pwm_lowerhalf_s *pwm;

    pwm = da1470x_pwm_initialize(DA1470X_PWM_TIMER3, BOARD_PWM_TIM3_PIN);
    if (pwm == NULL)
      {
        syslog(LOG_ERR, "Failed to init TIMER3 PWM\n");
      }
    else
      {
        ret = pwm_register("/dev/pwm1", pwm);
        if (ret < 0)
          {
            syslog(LOG_ERR, "Failed to register /dev/pwm1: %d\n", ret);
          }
      }
  }
#endif

#ifdef CONFIG_DA1470X_GPADC
  /* The rails worth watching on a board with a battery in it.  The four
   * pins the converter can also reach are left out: putting a pin into its
   * analogue mode is a decision about what is wired to it, and nothing is.
   */

  ret = da1470x_gpadc_initialize("/dev/adc0", g_adc_channels,
                                 sizeof(g_adc_channels));
  if (ret < 0)
    {
      syslog(LOG_ERR, "Failed to register /dev/adc0: %d\n", ret);
    }
#endif

#ifdef CONFIG_DA1470X_WDT
  ret = da1470x_wdt_initialize("/dev/watchdog0");
  if (ret < 0)
    {
      syslog(LOG_ERR, "Failed to register /dev/watchdog0: %d\n", ret);
    }
#endif

#ifdef CONFIG_DA1470X_SNC_SMOKETEST
  ret = da1470x_snc_smoketest();
  if (ret < 0)
    {
      syslog(LOG_ERR, "The sensor node controller did not start: %d\n", ret);
    }
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

#if defined(CONFIG_DA14706_LCD_E120A390QSR) || \
    defined(CONFIG_DA14706_LCD_LPM012M134B)
#  ifdef CONFIG_DA14706_LCD_E120A390QSR
  ret = da1470x_e120a390_initialize();
#  else
  ret = da1470x_lpm012m134b_initialize();
#  endif
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

#ifdef CONFIG_DA14706_TOUCH_ZT2628
  ret = da1470x_zt2628_initialize();
  if (ret < 0)
    {
      syslog(LOG_ERR, "Failed to register the touch controller: %d\n",
             ret);
    }
#endif

#ifdef CONFIG_DA1470X_AUDIO
  ret = da1470x_audio_setup();
  if (ret < 0)
    {
      syslog(LOG_ERR, "ERROR: cannot set the audio unit up: %d\n", ret);
    }
#endif

#ifdef CONFIG_DA1470X_VAD
  ret = da1470x_vad_setup();
  if (ret < 0)
    {
      syslog(LOG_ERR, "ERROR: cannot set the voice detector up: %d\n",
             ret);
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
