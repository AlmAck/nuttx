/****************************************************************************
 * arch/arm/src/da1470x/da1470x_cmac.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_CMAC_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_CMAC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <sys/types.h>
#include <stdbool.h>
#include <stdint.h>

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* Called from the CMAC interrupt with every byte received from the
 * controller.  Returns nothing; the consumer buffers and frames them.
 */

typedef void (*da1470x_cmac_rx_t)(void *arg, const uint8_t *data,
                                  size_t len);

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
 * Name: da1470x_cmac_start
 *
 * Description:
 *   Load the controller firmware into RAM10, power the radio domain, run
 *   the boot handshake, fill the configuration tables with the given
 *   public address and bring up the mailbox.  The rx callback receives
 *   raw bytes from the controller.
 *
 ****************************************************************************/

int da1470x_cmac_start(const uint8_t bdaddr[6], da1470x_cmac_rx_t rx,
                       void *arg);

/****************************************************************************
 * Name: da1470x_cmac_stop
 *
 * Description:
 *   Hold the controller in reset and power the radio domain down.
 *
 ****************************************************************************/

void da1470x_cmac_stop(void);

/****************************************************************************
 * Name: da1470x_cmac_write
 *
 * Description:
 *   Queue bytes to the controller.  As much as fits goes into the shared
 *   ring immediately; the remainder is pushed from the interrupt as the
 *   controller drains it.  Returns OK or -EAGAIN if a write is already
 *   pending.
 *
 ****************************************************************************/

int da1470x_cmac_write(const uint8_t *data, size_t len);

/****************************************************************************
 * Name: da1470x_cmac_write_pending
 *
 * Description:
 *   True while part of the last write is still waiting for ring space.
 *
 ****************************************************************************/

bool da1470x_cmac_write_pending(void);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_CMAC_H */
