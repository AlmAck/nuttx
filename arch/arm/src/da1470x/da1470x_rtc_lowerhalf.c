/****************************************************************************
 * arch/arm/src/da1470x/da1470x_rtc_lowerhalf.c
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
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <debug.h>

#include <nuttx/arch.h>
#include <nuttx/irq.h>
#include <nuttx/timers/rtc.h>

#include "da1470x_rtc.h"

#ifdef CONFIG_RTC_DRIVER

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* The DA1470x has a single alarm */

#define RTC_ALARMA  0

/****************************************************************************
 * Private Types
 ****************************************************************************/

#ifdef CONFIG_RTC_ALARM
struct da1470x_cbinfo_s
{
  volatile rtc_alarm_callback_t cb;  /* Callback when the alarm expires */
  volatile void *priv;               /* Private argument for the callback */
};
#endif

/* This is the private type for the RTC state.  It must be cast compatible
 * with struct rtc_lowerhalf_s.
 */

struct da1470x_lowerhalf_s
{
  const struct rtc_ops_s *ops;       /* Lower-half operations */
#ifdef CONFIG_RTC_ALARM
  struct da1470x_cbinfo_s cbinfo;    /* Alarm callback information */
#endif
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int da1470x_rdtime(struct rtc_lowerhalf_s *lower,
                          struct rtc_time *rtctime);
static int da1470x_settime(struct rtc_lowerhalf_s *lower,
                           const struct rtc_time *rtctime);
static bool da1470x_havesettime(struct rtc_lowerhalf_s *lower);
#ifdef CONFIG_RTC_ALARM
static int da1470x_setalarm(struct rtc_lowerhalf_s *lower,
                            const struct lower_setalarm_s *alarminfo);
static int da1470x_setrelative(struct rtc_lowerhalf_s *lower,
                               const struct lower_setrelative_s *alarminfo);
static int da1470x_cancelalarm(struct rtc_lowerhalf_s *lower, int alarmid);
static int da1470x_rdalarm(struct rtc_lowerhalf_s *lower,
                           struct lower_rdalarm_s *alarminfo);
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct rtc_ops_s g_rtc_ops =
{
  .rdtime      = da1470x_rdtime,
  .settime     = da1470x_settime,
  .havesettime = da1470x_havesettime,
#ifdef CONFIG_RTC_ALARM
  .setalarm    = da1470x_setalarm,
  .setrelative = da1470x_setrelative,
  .cancelalarm = da1470x_cancelalarm,
  .rdalarm     = da1470x_rdalarm,
#endif
};

static struct da1470x_lowerhalf_s g_rtc_lowerhalf =
{
  .ops = &g_rtc_ops,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_alarm_callback
 *
 * Description:
 *   Called from the RTC interrupt when the alarm fires.
 *
 ****************************************************************************/

#ifdef CONFIG_RTC_ALARM
static void da1470x_alarm_callback(void)
{
  struct da1470x_cbinfo_s *cbinfo = &g_rtc_lowerhalf.cbinfo;
  rtc_alarm_callback_t cb = (rtc_alarm_callback_t)cbinfo->cb;
  void *arg = (void *)cbinfo->priv;

  cbinfo->cb   = NULL;
  cbinfo->priv = NULL;

  if (cb != NULL)
    {
      cb(arg, 0);
    }
}
#endif

/****************************************************************************
 * Name: da1470x_rdtime
 ****************************************************************************/

static int da1470x_rdtime(struct rtc_lowerhalf_s *lower,
                          struct rtc_time *rtctime)
{
  return up_rtc_getdatetime((struct tm *)rtctime);
}

/****************************************************************************
 * Name: da1470x_settime
 ****************************************************************************/

static int da1470x_settime(struct rtc_lowerhalf_s *lower,
                           const struct rtc_time *rtctime)
{
  struct timespec ts;

  ts.tv_sec  = timegm((struct tm *)rtctime);
  ts.tv_nsec = 0;

  return up_rtc_settime(&ts);
}

/****************************************************************************
 * Name: da1470x_havesettime
 ****************************************************************************/

static bool da1470x_havesettime(struct rtc_lowerhalf_s *lower)
{
  struct tm tp;

  up_rtc_getdatetime(&tp);
  return (tp.tm_year + 1900) > CONFIG_START_YEAR;
}

#ifdef CONFIG_RTC_ALARM

/****************************************************************************
 * Name: da1470x_setalarm
 ****************************************************************************/

static int da1470x_setalarm(struct rtc_lowerhalf_s *lower,
                            const struct lower_setalarm_s *alarminfo)
{
  struct da1470x_lowerhalf_s *priv = (struct da1470x_lowerhalf_s *)lower;
  struct da1470x_cbinfo_s *cbinfo;
  struct timespec tp;
  int ret = -EINVAL;

  DEBUGASSERT(lower != NULL && alarminfo != NULL);

  if (alarminfo->id == RTC_ALARMA)
    {
      cbinfo       = &priv->cbinfo;
      cbinfo->cb   = alarminfo->cb;
      cbinfo->priv = alarminfo->priv;

      tp.tv_sec  = timegm((struct tm *)&alarminfo->time);
      tp.tv_nsec = 0;

      ret = da1470x_rtc_setalarm(&tp, da1470x_alarm_callback);
      if (ret < 0)
        {
          cbinfo->cb   = NULL;
          cbinfo->priv = NULL;
        }
    }

  return ret;
}

/****************************************************************************
 * Name: da1470x_setrelative
 ****************************************************************************/

static int da1470x_setrelative(struct rtc_lowerhalf_s *lower,
                               const struct lower_setrelative_s *alarminfo)
{
  struct lower_setalarm_s setalarm;
  struct tm time;
  time_t seconds;
  irqstate_t flags;
  int ret = -EINVAL;

  DEBUGASSERT(lower != NULL && alarminfo != NULL);

  if (alarminfo->id == RTC_ALARMA && alarminfo->reltime > 0)
    {
      flags = enter_critical_section();

      ret = up_rtc_getdatetime(&time);
      if (ret < 0)
        {
          leave_critical_section(flags);
          return ret;
        }

      seconds = timegm(&time) + alarminfo->reltime;
      gmtime_r(&seconds, (struct tm *)&setalarm.time);

      setalarm.id   = alarminfo->id;
      setalarm.cb   = alarminfo->cb;
      setalarm.priv = alarminfo->priv;

      ret = da1470x_setalarm(lower, &setalarm);
      leave_critical_section(flags);
    }

  return ret;
}

/****************************************************************************
 * Name: da1470x_cancelalarm
 ****************************************************************************/

static int da1470x_cancelalarm(struct rtc_lowerhalf_s *lower, int alarmid)
{
  struct da1470x_lowerhalf_s *priv = (struct da1470x_lowerhalf_s *)lower;
  int ret = -EINVAL;

  DEBUGASSERT(lower != NULL);

  if (alarmid == RTC_ALARMA)
    {
      priv->cbinfo.cb   = NULL;
      priv->cbinfo.priv = NULL;
      ret = da1470x_rtc_cancelalarm();
    }

  return ret;
}

/****************************************************************************
 * Name: da1470x_rdalarm
 ****************************************************************************/

static int da1470x_rdalarm(struct rtc_lowerhalf_s *lower,
                           struct lower_rdalarm_s *alarminfo)
{
  struct timespec ts;
  int ret = -EINVAL;

  DEBUGASSERT(lower != NULL && alarminfo != NULL &&
              alarminfo->time != NULL);

  if (alarminfo->id == RTC_ALARMA)
    {
      ret = da1470x_rtc_rdalarm(&ts);
      if (ret == OK)
        {
          gmtime_r(&ts.tv_sec, (struct tm *)alarminfo->time);
        }
    }

  return ret;
}

#endif /* CONFIG_RTC_ALARM */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_rtc_lowerhalf
 *
 * Description:
 *   Instantiate the RTC lower half driver for the DA1470x.  The board
 *   passes the returned structure to rtc_initialize().
 *
 ****************************************************************************/

struct rtc_lowerhalf_s *da1470x_rtc_lowerhalf(void)
{
  return (struct rtc_lowerhalf_s *)&g_rtc_lowerhalf;
}

#endif /* CONFIG_RTC_DRIVER */
