/****************************************************************************
 * arch/arm/src/da1470x/da1470x_rtc.c
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

/* The DA1470x RTC keeps a BCD time (hundredths, seconds, minutes, hours)
 * and calendar (weekday, month, day, year with century), clocked at 100 Hz
 * from the low-power clock through CLK_RTCDIV.  A single alarm compares
 * seconds, minutes, hours, day and month.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
#include <debug.h>

#include <nuttx/arch.h>
#include <nuttx/irq.h>
#include <nuttx/timers/rtc.h>

#include "arm_internal.h"
#include "hardware/da1470x_rtc.h"
#include "hardware/da1470x_crg_top.h"
#include "da1470x_clockconfig.h"
#include "da1470x_rtc.h"

#ifdef CONFIG_RTC

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* The arch RTC interface is implemented here; RTC_ARCH would build a
 * second one on top of the driver interface and clash at link time.
 */

#ifdef CONFIG_RTC_ARCH
#  error "CONFIG_DA1470X_RTC cannot be combined with CONFIG_RTC_ARCH"
#endif

/* Alarm compare mask: everything except hundredths */

#define RTC_ALARM_MASK  (RTC_ALARM_ENABLE_ALARM_SEC_EN  | \
                         RTC_ALARM_ENABLE_ALARM_MIN_EN  | \
                         RTC_ALARM_ENABLE_ALARM_HOUR_EN | \
                         RTC_ALARM_ENABLE_ALARM_DATE_EN | \
                         RTC_ALARM_ENABLE_ALARM_MNTH_EN)

#define RTC_TICK_HZ     100

/****************************************************************************
 * Public Data
 ****************************************************************************/

/* g_rtc_enabled is set true after the RTC has successfully initialized */

volatile bool g_rtc_enabled = false;

/****************************************************************************
 * Private Data
 ****************************************************************************/

#ifdef CONFIG_RTC_ALARM
static da1470x_alarmcb_t g_alarmcb;
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: rtc_bcd2bin / rtc_bin2bcd
 ****************************************************************************/

static inline unsigned int rtc_bcd2bin(uint32_t value, int shift,
                                       uint32_t umask, uint32_t tmask,
                                       int tshift)
{
  return ((value & umask) >> shift) + 10 * ((value & tmask) >> tshift);
}

static inline uint32_t rtc_bin2bcd(unsigned int value, int ushift,
                                   int tshift)
{
  return ((value % 10) << ushift) | ((value / 10) << tshift);
}

/****************************************************************************
 * Name: rtc_decode_time / rtc_decode_calendar
 *
 * Description:
 *   Convert the packed register values into a struct tm (UTC).
 *
 ****************************************************************************/

static void rtc_decode(uint32_t time, uint32_t cal, struct tm *tp)
{
  unsigned int century;

  tp->tm_sec  = rtc_bcd2bin(time, RTC_TIME_TIME_S_U_SHIFT,
                            RTC_TIME_TIME_S_U_MASK, RTC_TIME_TIME_S_T_MASK,
                            RTC_TIME_TIME_S_T_SHIFT);
  tp->tm_min  = rtc_bcd2bin(time, RTC_TIME_TIME_M_U_SHIFT,
                            RTC_TIME_TIME_M_U_MASK, RTC_TIME_TIME_M_T_MASK,
                            RTC_TIME_TIME_M_T_SHIFT);
  tp->tm_hour = rtc_bcd2bin(time, RTC_TIME_TIME_HR_U_SHIFT,
                            RTC_TIME_TIME_HR_U_MASK, RTC_TIME_TIME_HR_T_MASK,
                            RTC_TIME_TIME_HR_T_SHIFT);

  tp->tm_wday = (cal & RTC_CALENDAR_DAY_MASK) >> RTC_CALENDAR_DAY_SHIFT;
  tp->tm_wday = tp->tm_wday % 7;   /* Register uses 1..7 */

  tp->tm_mon  = ((cal & RTC_CALENDAR_CAL_M_U_MASK) >>
                 RTC_CALENDAR_CAL_M_U_SHIFT) +
                ((cal & RTC_CALENDAR_CAL_M_T) != 0 ? 10 : 0) - 1;
  tp->tm_mday = rtc_bcd2bin(cal, RTC_CALENDAR_CAL_D_U_SHIFT,
                            RTC_CALENDAR_CAL_D_U_MASK,
                            RTC_CALENDAR_CAL_D_T_MASK,
                            RTC_CALENDAR_CAL_D_T_SHIFT);
  century     = rtc_bcd2bin(cal, RTC_CALENDAR_CAL_C_U_SHIFT,
                            RTC_CALENDAR_CAL_C_U_MASK,
                            RTC_CALENDAR_CAL_C_T_MASK,
                            RTC_CALENDAR_CAL_C_T_SHIFT);
  tp->tm_year = century * 100 +
                rtc_bcd2bin(cal, RTC_CALENDAR_CAL_Y_U_SHIFT,
                            RTC_CALENDAR_CAL_Y_U_MASK,
                            RTC_CALENDAR_CAL_Y_T_MASK,
                            RTC_CALENDAR_CAL_Y_T_SHIFT) - 1900;
  tp->tm_yday = 0;
  tp->tm_isdst = 0;
}

/****************************************************************************
 * Name: rtc_encode_time / rtc_encode_calendar
 ****************************************************************************/

static uint32_t rtc_encode_time(const struct tm *tp)
{
  return rtc_bin2bcd(tp->tm_sec, RTC_TIME_TIME_S_U_SHIFT,
                     RTC_TIME_TIME_S_T_SHIFT) |
         rtc_bin2bcd(tp->tm_min, RTC_TIME_TIME_M_U_SHIFT,
                     RTC_TIME_TIME_M_T_SHIFT) |
         rtc_bin2bcd(tp->tm_hour, RTC_TIME_TIME_HR_U_SHIFT,
                     RTC_TIME_TIME_HR_T_SHIFT);
}

static uint32_t rtc_encode_calendar(const struct tm *tp)
{
  unsigned int year = tp->tm_year + 1900;
  unsigned int mon  = tp->tm_mon + 1;
  uint32_t cal;

  cal  = ((tp->tm_wday == 0 ? 7 : tp->tm_wday) << RTC_CALENDAR_DAY_SHIFT);
  cal |= ((mon % 10) << RTC_CALENDAR_CAL_M_U_SHIFT);
  if (mon >= 10)
    {
      cal |= RTC_CALENDAR_CAL_M_T;
    }

  cal |= rtc_bin2bcd(tp->tm_mday, RTC_CALENDAR_CAL_D_U_SHIFT,
                     RTC_CALENDAR_CAL_D_T_SHIFT);
  cal |= rtc_bin2bcd(year % 100, RTC_CALENDAR_CAL_Y_U_SHIFT,
                     RTC_CALENDAR_CAL_Y_T_SHIFT);
  cal |= rtc_bin2bcd(year / 100, RTC_CALENDAR_CAL_C_U_SHIFT,
                     RTC_CALENDAR_CAL_C_T_SHIFT);
  return cal;
}

/****************************************************************************
 * Name: rtc_clock_config
 *
 * Description:
 *   Derive the 100 Hz RTC clock from the selected low-power clock:
 *   div = lp_hz / 100 with a 1/1000 fractional part.
 *
 ****************************************************************************/

static void rtc_clock_config(void)
{
  uint32_t lp = da1470x_get_lpclk();
  uint32_t div_int  = lp / RTC_TICK_HZ;
  uint32_t div_frac = 10 * (lp - div_int * RTC_TICK_HZ);
  uint32_t regval;

  if (div_int > (CRG_TOP_CLK_RTCDIV_RTC_DIV_INT_MASK >>
                 CRG_TOP_CLK_RTCDIV_RTC_DIV_INT_SHIFT))
    {
      /* The 512 kHz RCLP mode cannot be divided down to 100 Hz */

      rtcerr("LP clock %" PRIu32 " Hz too fast for the RTC divider\n", lp);
      div_int  = CRG_TOP_CLK_RTCDIV_RTC_DIV_INT_MASK >>
                 CRG_TOP_CLK_RTCDIV_RTC_DIV_INT_SHIFT;
      div_frac = 0;
    }

  regval  = getreg32(DA1470X_CRG_TOP_CLK_RTCDIV);
  regval &= ~(CRG_TOP_CLK_RTCDIV_RTC_DIV_DENOM |
              CRG_TOP_CLK_RTCDIV_RTC_DIV_INT_MASK |
              CRG_TOP_CLK_RTCDIV_RTC_DIV_FRAC_MASK);
  regval |= CRG_TOP_CLK_RTCDIV_RTC_DIV_INT(div_int) |
            CRG_TOP_CLK_RTCDIV_RTC_DIV_FRAC(div_frac) |
            CRG_TOP_CLK_RTCDIV_RTC_DIV_ENABLE;
  putreg32(regval, DA1470X_CRG_TOP_CLK_RTCDIV);
}

/****************************************************************************
 * Name: rtc_interrupt
 ****************************************************************************/

#ifdef CONFIG_RTC_ALARM
static int rtc_interrupt(int irq, void *context, void *arg)
{
  uint32_t flags = getreg32(DA1470X_RTC_EVENT_FLAGS);  /* Clears on read */

  if ((flags & RTC_EVENT_FLAGS_EVENT_ALRM) != 0)
    {
      da1470x_alarmcb_t cb = g_alarmcb;

      putreg32(0, DA1470X_RTC_ALARM_ENABLE);
      putreg32(RTC_INTERRUPT_DISABLE_ALRM_INT_DIS,
               DA1470X_RTC_INTERRUPT_DISABLE);
      g_alarmcb = NULL;

      if (cb != NULL)
        {
          cb();
        }
    }

  return OK;
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_rtc_initialize
 *
 * Description:
 *   Initialize the hardware RTC per the selected configuration.  This
 *   function is called once during the OS initialization sequence
 *
 ****************************************************************************/

int up_rtc_initialize(void)
{
  uint32_t status;

  rtc_clock_config();

  /* Keep counting across software resets, 24-hour mode */

  putreg32(RTC_KEEP_RTC_KEEP, DA1470X_RTC_KEEP_RTC);
  putreg32(0, DA1470X_RTC_HOUR_MODE);

  /* If the counters do not hold a valid date (first power-up), load the
   * build-time default so that time is at least monotonic and sane.
   */

  status = getreg32(DA1470X_RTC_STATUS);
  if ((status & (RTC_STATUS_VALID_TIME | RTC_STATUS_VALID_CAL)) !=
      (RTC_STATUS_VALID_TIME | RTC_STATUS_VALID_CAL))
    {
      struct tm tp;

      tp.tm_sec  = 0;
      tp.tm_min  = 0;
      tp.tm_hour = 0;
      tp.tm_mday = CONFIG_START_DAY;
      tp.tm_mon  = CONFIG_START_MONTH - 1;
      tp.tm_year = CONFIG_START_YEAR - 1900;
      tp.tm_wday = 1;

      putreg32(RTC_CONTROL_TIME_DISABLE | RTC_CONTROL_CAL_DISABLE,
               DA1470X_RTC_CONTROL);
      putreg32(rtc_encode_time(&tp), DA1470X_RTC_TIME);
      putreg32(rtc_encode_calendar(&tp), DA1470X_RTC_CALENDAR);
    }

  /* No interrupts until an alarm is requested; start both counters */

  putreg32(0, DA1470X_RTC_ALARM_ENABLE);
  putreg32(0x7f, DA1470X_RTC_INTERRUPT_DISABLE);
  getreg32(DA1470X_RTC_EVENT_FLAGS);
  putreg32(0, DA1470X_RTC_CONTROL);

#ifdef CONFIG_RTC_ALARM
  irq_attach(DA1470X_IRQ_RTC, rtc_interrupt, NULL);
  up_enable_irq(DA1470X_IRQ_RTC);
#endif

  g_rtc_enabled = true;
  return OK;
}

/****************************************************************************
 * Name: up_rtc_getdatetime
 *
 * Description:
 *   Get the current date and time from the date/time RTC.  This interface
 *   is only supported by the date/time RTC hardware implementation.
 *
 ****************************************************************************/

int up_rtc_getdatetime(struct tm *tp)
{
  irqstate_t flags;
  uint32_t time;
  uint32_t cal;

  /* Stop the counters while reading so the two registers are coherent */

  flags = enter_critical_section();
  putreg32(RTC_CONTROL_TIME_DISABLE | RTC_CONTROL_CAL_DISABLE,
           DA1470X_RTC_CONTROL);
  time = getreg32(DA1470X_RTC_TIME);
  cal  = getreg32(DA1470X_RTC_CALENDAR);
  putreg32(0, DA1470X_RTC_CONTROL);
  leave_critical_section(flags);

  rtc_decode(time, cal, tp);
  return OK;
}

/****************************************************************************
 * Name: up_rtc_settime
 *
 * Description:
 *   Set the RTC to the provided time.  All RTC implementations must be able
 *   to set their time based on a standard timespec.
 *
 ****************************************************************************/

int up_rtc_settime(const struct timespec *tp)
{
  irqstate_t flags;
  struct tm newtime;
  uint32_t status;
  int ret = OK;

  gmtime_r(&tp->tv_sec, &newtime);

  flags = enter_critical_section();
  putreg32(RTC_CONTROL_TIME_DISABLE | RTC_CONTROL_CAL_DISABLE,
           DA1470X_RTC_CONTROL);
  putreg32(rtc_encode_time(&newtime), DA1470X_RTC_TIME);
  putreg32(rtc_encode_calendar(&newtime), DA1470X_RTC_CALENDAR);

  status = getreg32(DA1470X_RTC_STATUS);
  if ((status & (RTC_STATUS_VALID_TIME | RTC_STATUS_VALID_CAL)) !=
      (RTC_STATUS_VALID_TIME | RTC_STATUS_VALID_CAL))
    {
      rtcerr("RTC rejected the new time (status %02" PRIx32 ")\n", status);
      ret = -EINVAL;
    }

  putreg32(0, DA1470X_RTC_CONTROL);
  leave_critical_section(flags);
  return ret;
}

#ifdef CONFIG_RTC_ALARM

/****************************************************************************
 * Name: da1470x_rtc_setalarm
 ****************************************************************************/

int da1470x_rtc_setalarm(const struct timespec *tp, da1470x_alarmcb_t cb)
{
  irqstate_t flags;
  struct tm alarm;
  uint32_t cal;
  uint32_t status;
  unsigned int mon;

  if (tp == NULL || cb == NULL)
    {
      return -EINVAL;
    }

  gmtime_r(&tp->tv_sec, &alarm);
  mon = alarm.tm_mon + 1;

  cal  = (mon % 10) << RTC_CALENDAR_ALARM_CAL_M_U_SHIFT;
  if (mon >= 10)
    {
      cal |= RTC_CALENDAR_ALARM_CAL_M_T;
    }

  cal |= rtc_bin2bcd(alarm.tm_mday, RTC_CALENDAR_ALARM_CAL_D_U_SHIFT,
                     RTC_CALENDAR_ALARM_CAL_D_T_SHIFT);

  flags = enter_critical_section();

  if (g_alarmcb != NULL)
    {
      leave_critical_section(flags);
      return -EBUSY;
    }

  putreg32(0, DA1470X_RTC_ALARM_ENABLE);
  putreg32(rtc_encode_time(&alarm), DA1470X_RTC_TIME_ALARM);
  putreg32(cal, DA1470X_RTC_CALENDAR_ALARM);

  status = getreg32(DA1470X_RTC_STATUS);
  if ((status & (RTC_STATUS_VALID_TIME_ALM | RTC_STATUS_VALID_CAL_ALM)) !=
      (RTC_STATUS_VALID_TIME_ALM | RTC_STATUS_VALID_CAL_ALM))
    {
      leave_critical_section(flags);
      rtcerr("RTC rejected the alarm (status %02" PRIx32 ")\n", status);
      return -EINVAL;
    }

  g_alarmcb = cb;
  getreg32(DA1470X_RTC_EVENT_FLAGS);
  putreg32(RTC_ALARM_MASK, DA1470X_RTC_ALARM_ENABLE);
  putreg32(RTC_INTERRUPT_ENABLE_ALRM_INT_EN, DA1470X_RTC_INTERRUPT_ENABLE);

  leave_critical_section(flags);
  return OK;
}

/****************************************************************************
 * Name: da1470x_rtc_cancelalarm
 ****************************************************************************/

int da1470x_rtc_cancelalarm(void)
{
  irqstate_t flags;

  flags = enter_critical_section();
  putreg32(0, DA1470X_RTC_ALARM_ENABLE);
  putreg32(RTC_INTERRUPT_DISABLE_ALRM_INT_DIS,
           DA1470X_RTC_INTERRUPT_DISABLE);
  g_alarmcb = NULL;
  leave_critical_section(flags);
  return OK;
}

/****************************************************************************
 * Name: da1470x_rtc_rdalarm
 ****************************************************************************/

int da1470x_rtc_rdalarm(struct timespec *tp)
{
  struct tm alarm;
  uint32_t time;
  uint32_t cal;

  if (tp == NULL)
    {
      return -EINVAL;
    }

  /* The alarm holds no year: assume the current one */

  up_rtc_getdatetime(&alarm);
  time = getreg32(DA1470X_RTC_TIME_ALARM);
  cal  = getreg32(DA1470X_RTC_CALENDAR_ALARM);

  alarm.tm_sec  = rtc_bcd2bin(time, RTC_TIME_ALARM_TIME_S_U_SHIFT,
                              RTC_TIME_ALARM_TIME_S_U_MASK,
                              RTC_TIME_ALARM_TIME_S_T_MASK,
                              RTC_TIME_ALARM_TIME_S_T_SHIFT);
  alarm.tm_min  = rtc_bcd2bin(time, RTC_TIME_ALARM_TIME_M_U_SHIFT,
                              RTC_TIME_ALARM_TIME_M_U_MASK,
                              RTC_TIME_ALARM_TIME_M_T_MASK,
                              RTC_TIME_ALARM_TIME_M_T_SHIFT);
  alarm.tm_hour = rtc_bcd2bin(time, RTC_TIME_ALARM_TIME_HR_U_SHIFT,
                              RTC_TIME_ALARM_TIME_HR_U_MASK,
                              RTC_TIME_ALARM_TIME_HR_T_MASK,
                              RTC_TIME_ALARM_TIME_HR_T_SHIFT);
  alarm.tm_mon  = ((cal & RTC_CALENDAR_ALARM_CAL_M_U_MASK) >>
                   RTC_CALENDAR_ALARM_CAL_M_U_SHIFT) +
                  ((cal & RTC_CALENDAR_ALARM_CAL_M_T) != 0 ? 10 : 0) - 1;
  alarm.tm_mday = rtc_bcd2bin(cal, RTC_CALENDAR_ALARM_CAL_D_U_SHIFT,
                              RTC_CALENDAR_ALARM_CAL_D_U_MASK,
                              RTC_CALENDAR_ALARM_CAL_D_T_MASK,
                              RTC_CALENDAR_ALARM_CAL_D_T_SHIFT);

  tp->tv_sec  = timegm(&alarm);
  tp->tv_nsec = 0;
  return OK;
}

#endif /* CONFIG_RTC_ALARM */
#endif /* CONFIG_RTC */
