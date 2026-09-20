/****************************************************************************
 * arch/arm/src/da1470x/da1470x_wdt.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_WDT_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_WDT_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdbool.h>
#include <stdint.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* All three watchdogs on this part are the same circuit, decrementing once
 * every 10 ms from a 13-bit reload, so the longest any of them can be asked
 * to wait is a little under 82 seconds.
 */

#define DA1470X_WDT_TICK_MS     10
#define DA1470X_WDT_MAX_TICKS   0x1fff
#define DA1470X_WDT_MAX_TIMEOUT (DA1470X_WDT_MAX_TICKS * DA1470X_WDT_TICK_MS)

/* Which of them.  They differ in where they live and in who is expected to
 * feed them:
 *
 *   SYS   PD_SLP, fed by this processor.  The one exposed as a NuttX
 *         watchdog device.
 *   CMAC  PD_RAD, fed by the radio controller's own firmware.  Running
 *         whenever the radio domain is powered, whether or not anything
 *         here knows about it.
 *   SNC   PD_SNC, fed by whatever firmware the sensor node controller is
 *         running, and driving that processor's NMI rather than ours.
 */

enum da1470x_wdt_e
{
  DA1470X_WDT_SYS = 0,
  DA1470X_WDT_CMAC,
  DA1470X_WDT_SNC
};

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

/****************************************************************************
 * Name: da1470x_wdt_initialize
 *
 * Description:
 *   Register the system watchdog as a NuttX watchdog device.  It is
 *   registered stopped -- that is, frozen -- so that nothing resets the
 *   board until something opens the device and starts it deliberately.
 *
 *   A caution that matters on this part: freezing the watchdog does NOT
 *   survive sleep.  The datasheet is explicit that when the processor
 *   enters any sleep mode with PD_SYS switched off, the system watchdog
 *   resumes by itself.  So a system that sleeps for longer than the
 *   timeout will be reset while it sleeps, however thoroughly the
 *   watchdog was stopped beforehand.  Either keep the sleep shorter than
 *   the timeout, or reload the watchdog on the way down.
 *
 * Input Parameters:
 *   devpath - where to register it, conventionally "/dev/watchdog0"
 *
 * Returned Value:
 *   Zero on success, a negated errno otherwise.
 *
 ****************************************************************************/

int da1470x_wdt_initialize(const char *devpath);

/****************************************************************************
 * Name: da1470x_wdt_freeze
 *
 * Description:
 *   Stop or resume one of the three watchdogs.  All of them freeze by
 *   themselves while a debugger has the corresponding processor halted, so
 *   this is about what the software wants, not about debugging.
 *
 *   Freezing the CMAC watchdog is only possible while PD_SYS is up.
 *
 ****************************************************************************/

void da1470x_wdt_freeze(enum da1470x_wdt_e which, bool freeze);

/****************************************************************************
 * Name: da1470x_wdt_is_frozen
 ****************************************************************************/

bool da1470x_wdt_is_frozen(enum da1470x_wdt_e which);

/****************************************************************************
 * Name: da1470x_wdt_timeleft_ms
 *
 * Description:
 *   How long the system watchdog has left before it bites, in
 *   milliseconds.  Zero once it has run out.
 *
 ****************************************************************************/

uint32_t da1470x_wdt_timeleft_ms(void);

/****************************************************************************
 * Name: da1470x_wdt_snc_reload
 *
 * Description:
 *   Reload the sensor node controller's watchdog from this processor.  Its
 *   own firmware is the one that should be doing this; the path exists
 *   because the datasheet provides it and because a controller running
 *   something simple may not want a watchdog at all.
 *
 ****************************************************************************/

void da1470x_wdt_snc_reload(uint32_t ticks);

/****************************************************************************
 * Name: da1470x_wdt_reset_cause
 *
 * Description:
 *   Which watchdogs, if any, have reset the part.  Reads the sticky bits
 *   of RESET_STAT_REG, so it reports everything since the last time they
 *   were cleared rather than only the most recent reset.
 *
 * Returned Value:
 *   A mask of (1 << DA1470X_WDT_*).
 *
 *   There is no call to clear them.  A zero in that register is what the
 *   extended sleep path reads to tell a wake-up from a reset, so clearing
 *   it would make the following boot look like a resume.
 *
 ****************************************************************************/

uint32_t da1470x_wdt_reset_cause(void);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_WDT_H */
