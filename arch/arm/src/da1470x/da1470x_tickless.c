/****************************************************************************
 * arch/arm/src/da1470x/da1470x_tickless.c
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

/* The system time comes from one of the general purpose timers running from
 * the low power clock instead of from SysTick.  SysTick is driven by the
 * processor clock, which stops whenever the core stops, so with it as the
 * time base the part can never sleep for longer than one tick and cannot
 * lose its clock at all.  The timer block lives in PD_TIM and keeps
 * counting while PD_SYS is off, which is what lets the idle logic ask for a
 * wake-up seconds away.
 *
 * The counter is 24 bits wide and free running: it wraps every 512 s at
 * 32768 Hz.  There is a single interrupt, raised when the counter equals
 * the value in SETTINGS.RELOAD, so the wrap itself is not reported.  The
 * driver therefore extends the counter in software, detecting a wrap by a
 * reading that went backwards, and always keeps the comparator armed no
 * further than half a period ahead.  That guarantees one reading per half
 * period, so no wrap can pass unseen, and costs at most one wake-up every
 * 256 s when nothing else is going on.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>
#include <debug.h>

#include <nuttx/arch.h>
#include <nuttx/irq.h>
#include <nuttx/clock.h>
#include <sys/param.h>

#include "arm_internal.h"
#include "hardware/da1470x_timer.h"
#include "da1470x_clockconfig.h"
#include "da1470x_pmu.h"
#include "da1470x_tickless.h"

#ifdef CONFIG_DA1470X_TICKLESS

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#if CONFIG_DA1470X_TICKLESS_TIMER == 1
#  define TL_BASE  DA1470X_TIMER_BASE
#  define TL_IRQ   DA1470X_IRQ_TIMER
#elif CONFIG_DA1470X_TICKLESS_TIMER == 2
#  define TL_BASE  DA1470X_TIMER2_BASE
#  define TL_IRQ   DA1470X_IRQ_TIMER2
#else
#  error "CONFIG_DA1470X_TICKLESS_TIMER must name a timer in PD_TIM (1 or 2)"
#endif

#define TL_CTRL       (TL_BASE + DA1470X_TIMER_CTRL_OFFSET)
#define TL_VAL        (TL_BASE + DA1470X_TIMER_TIMER_VAL_OFFSET)
#define TL_SETTINGS   (TL_BASE + DA1470X_TIMER_SETTINGS_OFFSET)
#define TL_CLEAR_IRQ  (TL_BASE + DA1470X_TIMER_CLEAR_IRQ_OFFSET)

/* Width of the hardware counter */

#define TL_MASK       0x00ffffffull
#define TL_PERIOD     0x01000000ull

/* Never arm the comparator further than half a period ahead, so that a
 * wrap is always seen by the interrupt that follows it.
 */

#define TL_WRAP_DELAY (TL_PERIOD / 2)

/* A configured limit shortens that, so the system is seen to be alive even
 * when nothing is due.
 */

#if CONFIG_DA1470X_TICKLESS_MAX_SLEEP_MS > 0
#  define TL_MAX_DELAY(freq) \
     MIN(TL_WRAP_DELAY, \
         ((uint64_t)(freq) * CONFIG_DA1470X_TICKLESS_MAX_SLEEP_MS) / 1000)
#else
#  define TL_MAX_DELAY(freq)  TL_WRAP_DELAY
#endif

/* The comparator value is resynchronised into the low power clock domain
 * and that takes up to two of its cycles, so a value less than three counts
 * ahead may be missed altogether -- and a missed comparison costs a whole
 * 512 s period.
 */

#define TL_MIN_DELAY  3

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct da1470x_tickless_s
{
  uint32_t freq;        /* Low power clock frequency in Hz */
  uint32_t last;        /* Last raw counter reading, for wrap detection */
  uint64_t base;        /* Counts accumulated by all wraps so far */
  uint64_t alarm;       /* Absolute count the alarm is due at */
  bool     alarm_set;   /* An alarm has been requested */
};

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct da1470x_tickless_s g_tickless;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: tl_getcount
 *
 * Description:
 *   Read the counter and extend it to 64 bits.  A reading below the
 *   previous one means the counter wrapped since that reading.  Must be
 *   called with interrupts disabled.
 *
 ****************************************************************************/

static uint64_t tl_getcount(void)
{
  uint32_t raw = getreg32(TL_VAL) & TL_MASK;

  if (raw < g_tickless.last)
    {
      g_tickless.base += TL_PERIOD;
    }

  g_tickless.last = raw;
  return g_tickless.base + raw;
}

/****************************************************************************
 * Name: tl_count2ts / tl_ts2count
 *
 * Description:
 *   Convert between counts of the low power clock and absolute time.  The
 *   seconds are split off before the fraction is scaled so that neither
 *   product can overflow.
 *
 ****************************************************************************/

static void tl_count2ts(uint64_t count, struct timespec *ts)
{
  uint64_t sec = count / g_tickless.freq;

  ts->tv_sec  = (time_t)sec;
  ts->tv_nsec = (long)(((count - sec * g_tickless.freq) * NSEC_PER_SEC) /
                       g_tickless.freq);
}

static uint64_t tl_ts2count(const struct timespec *ts)
{
  return (uint64_t)ts->tv_sec * g_tickless.freq +
         ((uint64_t)ts->tv_nsec * g_tickless.freq) / NSEC_PER_SEC;
}

/****************************************************************************
 * Name: tl_arm
 *
 * Description:
 *   Point the comparator at the next event.  The target is clamped into the
 *   window the hardware can actually hit: not so near that the write misses
 *   the comparison, and not so far that a wrap would pass unseen.  Must be
 *   called with interrupts disabled.
 *
 ****************************************************************************/

static void tl_arm(void)
{
  uint64_t now = tl_getcount();
  uint64_t target = now + TL_MAX_DELAY(g_tickless.freq);
  uint32_t regval;

  if (g_tickless.alarm_set && g_tickless.alarm < target)
    {
      target = g_tickless.alarm;
    }

  if (target < now + TL_MIN_DELAY)
    {
      target = now + TL_MIN_DELAY;
    }

  regval  = getreg32(TL_SETTINGS);
  regval &= ~TIMER_SETTINGS_TIM_RELOAD_MASK;
  regval |= TIMER_SETTINGS_TIM_RELOAD((uint32_t)(target & TL_MASK));
  putreg32(regval, TL_SETTINGS);
}

/****************************************************************************
 * Name: tl_try_xtal32k
 *
 * Description:
 *   Move the low power clock onto the watch crystal, but only once the
 *   counter in front of us proves the crystal is oscillating.  There is no
 *   ready flag for it and it takes a few hundred milliseconds of bias to
 *   start, so the counter is the only witness available.  If it never
 *   moves -- an unpopulated crystal, most likely -- the clock goes back to
 *   the RC oscillator and the system runs on that, a percent or so off but
 *   alive.
 *
 ****************************************************************************/

#ifdef CONFIG_DA1470X_CLOCK_LPCLK_SRC_XTAL32K
static void tl_try_xtal32k(void)
{
  uint32_t seen;
  int i;

  da1470x_set_lpclk(DA1470X_LPCLK_XTAL32K);

  seen = getreg32(TL_VAL) & TL_MASK;

  for (i = 0; i < CONFIG_DA1470X_XTAL32K_SETTLE_MS; i++)
    {
      up_udelay(1000);

      if ((getreg32(TL_VAL) & TL_MASK) != seen)
        {
          return;
        }
    }

  _warn("XTAL32K did not start; staying on the RC low power clock\n");
  da1470x_set_lpclk(DA1470X_LPCLK_RCLP);
}
#endif

/****************************************************************************
 * Name: tl_interrupt
 ****************************************************************************/

static int tl_interrupt(int irq, void *context, void *arg)
{
  irqstate_t flags = enter_critical_section();
  bool expired;

  putreg32(0, TL_CLEAR_IRQ);

  expired = g_tickless.alarm_set && tl_getcount() >= g_tickless.alarm;
  if (expired)
    {
      g_tickless.alarm_set = false;
    }

  tl_arm();
  leave_critical_section(flags);

  if (expired)
    {
      nxsched_process_timer();
    }

  return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_timer_initialize
 ****************************************************************************/

void up_timer_initialize(void)
{
  uint32_t regval;

  /* The timer block only answers once its power domain is up */

  da1470x_pd_enable(DA1470X_PD_TIM);

  /* Free running upward from the low power clock with no prescaling, the
   * interrupt enabled and the comparator parked out of the way until the
   * first arming.
   */

  putreg32(0, TL_CTRL);
  putreg32(TIMER_SETTINGS_TIM_RELOAD(TL_MASK), TL_SETTINGS);
  putreg32(0, TL_CLEAR_IRQ);

  regval = TIMER_CTRL_TIM_CLK_EN | TIMER_CTRL_TIM_FREE_RUN_MODE_EN |
           TIMER_CTRL_TIM_IRQ_EN | TIMER_CTRL_TIM_EN;
  putreg32(regval, TL_CTRL);

#ifdef CONFIG_DA1470X_CLOCK_LPCLK_SRC_XTAL32K
  /* Now that something is counting the low power clock, the crystal can be
   * put to the test.  Do it before the time base is latched so that the
   * counts spent waiting for it are not mistaken for elapsed time.
   */

  tl_try_xtal32k();
#endif

  g_tickless.freq = da1470x_get_lpclk();
  DEBUGASSERT(g_tickless.freq != 0);

  g_tickless.last = getreg32(TL_VAL) & TL_MASK;
  g_tickless.base = 0;

  irq_attach(TL_IRQ, tl_interrupt, NULL);
  up_enable_irq(TL_IRQ);

  tl_arm();
}

/****************************************************************************
 * Name: up_timer_gettime
 ****************************************************************************/

int up_timer_gettime(struct timespec *ts)
{
  irqstate_t flags = enter_critical_section();

  tl_count2ts(tl_getcount(), ts);

  leave_critical_section(flags);
  return OK;
}

/****************************************************************************
 * Name: up_alarm_start
 ****************************************************************************/

int up_alarm_start(const struct timespec *ts)
{
  irqstate_t flags = enter_critical_section();

  g_tickless.alarm     = tl_ts2count(ts);
  g_tickless.alarm_set = true;
  tl_arm();

  leave_critical_section(flags);
  return OK;
}

/****************************************************************************
 * Name: up_alarm_cancel
 ****************************************************************************/

int up_alarm_cancel(struct timespec *ts)
{
  irqstate_t flags = enter_critical_section();

  g_tickless.alarm_set = false;

  if (ts != NULL)
    {
      tl_count2ts(tl_getcount(), ts);
    }

  tl_arm();

  leave_critical_section(flags);
  return OK;
}

#endif /* CONFIG_DA1470X_TICKLESS */
