/****************************************************************************
 * arch/arm/src/da1470x/da1470x_rtc.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_RTC_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_RTC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdbool.h>
#include <time.h>

#include <nuttx/timers/rtc.h>

/****************************************************************************
 * Public Types
 ****************************************************************************/

#ifdef CONFIG_RTC_ALARM
typedef void (*da1470x_alarmcb_t)(void);
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

#ifdef CONFIG_RTC_ALARM

/****************************************************************************
 * Name: da1470x_rtc_setalarm
 *
 * Description:
 *   Set an alarm at the given absolute time (UTC seconds).  The callback
 *   runs from interrupt context.  Only one alarm is supported.
 *
 ****************************************************************************/

int da1470x_rtc_setalarm(const struct timespec *tp, da1470x_alarmcb_t cb);

/****************************************************************************
 * Name: da1470x_rtc_cancelalarm
 ****************************************************************************/

int da1470x_rtc_cancelalarm(void);

/****************************************************************************
 * Name: da1470x_rtc_rdalarm
 *
 * Description:
 *   Read back the programmed alarm time.
 *
 ****************************************************************************/

int da1470x_rtc_rdalarm(struct timespec *tp);

#endif /* CONFIG_RTC_ALARM */

/****************************************************************************
 * Name: da1470x_rtc_lowerhalf
 *
 * Description:
 *   Return the RTC lower-half structure for rtc_initialize().
 *
 ****************************************************************************/

#ifdef CONFIG_RTC_DRIVER
struct rtc_lowerhalf_s *da1470x_rtc_lowerhalf(void);
#endif

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_RTC_H */
