/****************************************************************************
 * arch/arm/src/da1470x/da1470x_snc.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_SNC_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_SNC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include <arch/chip/snc.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* The sensor node controller is a Cortex-M0+ with its own view of the
 * system RAM:
 *
 *   RAM1+RAM2  64 KiB   code and data   M33 0x20000000   SNC 0x00000000
 *   RAM8      128 KiB   shared          M33 0x20110000   SNC 0x00030000
 *
 * It starts at its own address zero, so its image begins with its stack
 * pointer and reset vector.  The board's linker script keeps the 64 KiB
 * out of NuttX's way, and RAM8 is kept out of the heap while this driver
 * is configured.
 */

#define DA1470X_SNC_CODE_BASE   0x20000000  /* RAM1, as the M33 sees it */
#define DA1470X_SNC_CODE_SIZE   0x00010000  /* RAM1 and RAM2 */

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
 * Name: da1470x_snc_initialize
 *
 * Description:
 *   Load the configured firmware into the controller, start it, wait for
 *   it to report ready, and register the character device.
 *
 * Input Parameters:
 *   devpath - where to register the device, normally "/dev/snc0"
 *
 * Returned Value:
 *   Zero on success, a negated errno otherwise.  The device is registered
 *   even when the firmware fails to start, so that its state can be read.
 *
 ****************************************************************************/

int da1470x_snc_initialize(const char *devpath);

/****************************************************************************
 * Name: da1470x_snc_start / da1470x_snc_stop
 *
 * Description:
 *   (Re)load the firmware and run it, waiting for it to report ready; and
 *   hold the controller in reset without its clock.  Its power domain is
 *   left up: the M33's own console and serial drivers live there too.
 *
 ****************************************************************************/

int da1470x_snc_start(void);
void da1470x_snc_stop(void);

/****************************************************************************
 * Name: da1470x_snc_getstatus
 ****************************************************************************/

void da1470x_snc_getstatus(struct snc_status_s *status);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_SNC_H */
