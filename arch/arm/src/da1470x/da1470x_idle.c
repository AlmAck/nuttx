/****************************************************************************
 * arch/arm/src/da1470x/da1470x_idle.c
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

#include <stdbool.h>
#include <stdint.h>
#include <debug.h>

#include <nuttx/arch.h>
#include <nuttx/clock.h>
#include <nuttx/power/pm.h>
#include <arch/irq.h>

#include <nuttx/board.h>
#include <arch/board/board.h>

// #include "da1470x_pm.h"
// #include "da1470x_rtc.h"
// #include "da1470x_exti.h"
// #include "da1470x_clock.h"
#include "da1470x_clk.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* LED_IDLE, if your board has an idle LED */
#if defined(CONFIG_ARCH_LEDS) && defined(LED_IDLE)
#  define BEGIN_IDLE() board_autoled_on(LED_IDLE)
#  define END_IDLE()   board_autoled_off(LED_IDLE)
#else
#  define BEGIN_IDLE()
#  define END_IDLE()
#endif

/* Default wake-up alarm if nothing else (15s) */
#ifndef CONFIG_PM_ALARM_SEC
#  define CONFIG_PM_ALARM_SEC 15
#endif
#ifndef CONFIG_PM_ALARM_NSEC
#  define CONFIG_PM_ALARM_NSEC 0
#endif

#ifdef CONFIG_PM_SLEEP_WAKEUP
#  ifndef CONFIG_RTC_ALARM
#    error "CONFIG_RTC_ALARM must be enabled for PM_SLEEP_WAKEUP"
#  endif
#  ifndef CONFIG_PM_SLEEP_WAKEUP_SEC
#    define CONFIG_PM_SLEEP_WAKEUP_SEC 10
#  endif
#  ifndef CONFIG_PM_SLEEP_WAKEUP_NSEC
#    define CONFIG_PM_SLEEP_WAKEUP_NSEC 0
#  endif
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

#if defined(CONFIG_PM) && defined(CONFIG_RTC_ALARM)
static volatile bool g_alarmwakeup;
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/* RTC alarm callback */
#if defined(CONFIG_PM) && defined(CONFIG_RTC_ALARM)
static void da1470x_alarmcb(void)
{
  g_alarmwakeup = true;
}

/* EXTI ISR for RTC alarm */
static int da1470x_alarm_exti(int irq, void *context, void *arg)
{
  da1470x_alarmcb();
  return OK;
}

/* Cancel the EXTI wakeup line */
static void da1470x_exti_cancel(void)
{
  da1470x_exti_alarm(false, false, false, NULL, NULL);
}

/* Program RTC alarm */
static int da1470x_rtc_setalarm_pm(time_t sec, time_t nsec, bool use_exti)
{
  struct timespec ts;
  int ret;

  if (use_exti)
    {
      /* attach EXTI line for RTC alarm */
      da1470x_exti_alarm(true, true, true, da1470x_alarm_exti, NULL);
    }

  /* compute wakeup time */
  up_rtc_gettime(&ts);
  ts.tv_sec  += sec;
  ts.tv_nsec += nsec;
  if (ts.tv_nsec >= NSEC_PER_SEC)
    {
      ts.tv_sec++;
      ts.tv_nsec -= NSEC_PER_SEC;
    }

  g_alarmwakeup = false;
  ret = da1470x_rtc_setalarm(&ts, da1470x_alarmcb);
  if (ret < 0)
    {
      serr("ERROR: RTC alarm already set\n");
    }
  return ret;
}
#endif

/****************************************************************************
 * Name: da1470x_idlepm
 *
 * Description:
 *   Perform power management in the IDLE loop.
 ****************************************************************************/

#ifdef CONFIG_PM
static void da1470x_idlepm(void)
{
  static enum pm_state_e oldstate = PM_NORMAL;
  enum pm_state_e newstate;
  int ret;

  /* If we just came out of STANDBY, pick SLEEP vs NORMAL */
  if (oldstate == PM_STANDBY)
    {
#ifdef CONFIG_RTC_ALARM
      newstate = g_alarmwakeup ? PM_SLEEP : PM_NORMAL;
#else
      newstate = PM_NORMAL;
#endif
    }
  else
    {
      /* Let the PM framework pick the next idle state */
      newstate = pm_checkstate(PM_IDLE_DOMAIN);
    }

  if (newstate != oldstate)
    {
      _info("PM: %d -> %d\n", oldstate, newstate);
      sched_lock();

      ret = pm_changestate(PM_IDLE_DOMAIN, newstate);
      if (ret < 0)
        {
          pm_changestate(PM_IDLE_DOMAIN, oldstate);
          goto out_unlock;
        }

      switch (newstate)
        {
          case PM_NORMAL:
            if (oldstate == PM_STANDBY)
              {
                /* Re-enable clocks */
                da1470x_clock_enable();
#ifdef CONFIG_RTC
                clock_synchronize(NULL);
#endif
              }
            break;

          case PM_IDLE:
            /* Nothing special; just return to idle loop */
            break;

          case PM_STANDBY:
#ifdef CONFIG_RTC_ALARM
            /* Wake us back into SLEEP when alarm fires */
            da1470x_rtc_setalarm_pm(CONFIG_PM_ALARM_SEC,
                                    CONFIG_PM_ALARM_NSEC,
                                    true);
#endif
            /* small delay for EXTI setup */
            up_mdelay(10);

            /* Enter STOP mode (low-power standby) */
            da1470x_pmstop(false);

#ifdef CONFIG_RTC_ALARM
            /* Cancel leftover alarm EXTI */
            da1470x_exti_cancel();
            ret = da1470x_rtc_cancelalarm();
            if (ret < 0)
              {
                swarn("WARNING: cancel alarm failed\n");
              }
#endif
            break;

          case PM_SLEEP:
#ifdef CONFIG_PM_SLEEP_WAKEUP
            /* Program RTC to reset after a delay */
            da1470x_rtc_setalarm_pm(CONFIG_PM_SLEEP_WAKEUP_SEC,
                                    CONFIG_PM_SLEEP_WAKEUP_NSEC,
                                    false);
#endif
            up_mdelay(10);

            /* Enter deep standby (reset on wake) */
            da1470x_pmstandby();
            break;

          default:
            break;
        }

      oldstate = newstate;

    out_unlock:
      sched_unlock();
    }
}
#else
#  define da1470x_idlepm()
#endif

/****************************************************************************
 * Name: up_idle
 *
 * Description:
 *   Called when NuttX idle task runs.
 ****************************************************************************/

void up_idle(void)
{
#if defined(CONFIG_SUPPRESS_INTERRUPTS) || defined(CONFIG_SUPPRESS_TIMER_INTS)
  nxsched_process_timer();
#else
  BEGIN_IDLE();
  da1470x_idlepm();
  END_IDLE();
#endif
}
