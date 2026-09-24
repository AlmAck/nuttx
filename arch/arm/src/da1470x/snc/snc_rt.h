/****************************************************************************
 * arch/arm/src/da1470x/snc/snc_rt.h
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

/* The small runtime every sensor node controller firmware links against:
 * the controller's half of the mailbox, the doorbell, the NVIC, and a
 * periodic timer.  A firmware application provides main(), which calls
 * snc_rt_init(), declares itself ready and then loops on snc_rt_wait().
 */

#ifndef __ARCH_ARM_SRC_DA1470X_SNC_SNC_RT_H
#define __ARCH_ARM_SRC_DA1470X_SNC_SNC_RT_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdbool.h>
#include <stdint.h>

#include "../da1470x_snc_ipc.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define getreg32(a)       (*(volatile uint32_t *)(a))
#define putreg32(v, a)    (*(volatile uint32_t *)(a) = (v))

/* The controller's interrupt numbers used by the runtime */

#define SNC_IRQ_SYS2SNC   0
#define SNC_IRQ_TIMER6    11

/****************************************************************************
 * Public Data
 ****************************************************************************/

/* RAM8, seen from the controller */

#define g_snc_shared ((struct snc_shared_s *)SNC_SHARED_SNC_BASE)

/* Set by the interrupt handlers, cleared by the application */

extern volatile bool g_snc_cmd_pending;
extern volatile uint32_t g_snc_timer_ticks;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/* Enable the doorbell interrupt and report BOOTING */

void snc_rt_init(void);

/* Report READY; the M33's loader waits for this */

void snc_rt_ready(void);

/* Send a message to the M33 and ring its doorbell.  Returns 0, or -1 when
 * the ring is full, in which case the drop is counted in the shared area.
 */

int snc_send(uint16_t type, const void *payload, uint16_t len);

/* Take the next command, as snc_ring_get() */

int snc_recv(uint16_t *type, void *payload, uint16_t size);

/* Sleep until an interrupt has something for the application: a command,
 * or a timer tick.  Returns at once if one is already pending.
 */

void snc_rt_wait(void);

/* Run TIMER6 from the low power clock, advancing g_snc_timer_ticks every
 * period_ms milliseconds; 0 stops it.  The clock's frequency comes from
 * the M33, through the shared area.
 */

void snc_timer_start(uint32_t period_ms);

/* NVIC */

void snc_irq_enable(int irq);
void snc_irq_disable(int irq);

#endif /* __ARCH_ARM_SRC_DA1470X_SNC_SNC_RT_H */
