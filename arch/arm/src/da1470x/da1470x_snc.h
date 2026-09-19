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

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* The sensor node controller is a Cortex-M0+ with its own view of the
 * system RAM.  Two cells are wired to it and the datasheet names them for
 * what they are meant to hold:
 *
 *   RAM1   32 KiB   "SNC code"   M33 0x20000000   SNC 0x00000000
 *   RAM2   32 KiB   "SNC data"   M33 0x20008000   SNC 0x00008000
 *   RAM8  128 KiB   shared       M33 0x20110000   SNC 0x00030000
 *
 * The processor starts at its own address zero, so the first two words of
 * RAM1 are its stack pointer and reset vector.  The board's linker script
 * keeps the whole 64 KiB out of the way of NuttX.
 */

#define DA1470X_SNC_CODE_BASE   0x20000000  /* RAM1, as the M33 sees it */
#define DA1470X_SNC_CODE_SIZE   0x00008000
#define DA1470X_SNC_DATA_BASE   0x20008000  /* RAM2, as the M33 sees it */
#define DA1470X_SNC_DATA_SIZE   0x00008000

/* The same two cells in the sensor node controller's own addresses, which
 * is what its firmware has to be linked against.
 */

#define DA1470X_SNC_CODE_ORIGIN 0x00000000
#define DA1470X_SNC_DATA_ORIGIN 0x00008000

/* Translate an address in one view into the other */

#define DA1470X_SNC_TO_M33(a)   ((a) + DA1470X_SNC_CODE_BASE)
#define DA1470X_M33_TO_SNC(a)   ((a) - DA1470X_SNC_CODE_BASE)

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
 * Name: da1470x_snc_start
 *
 * Description:
 *   Load a firmware image into the sensor node controller's code RAM and
 *   let it run.  The image is linked against the controller's own
 *   addresses, so it begins with the stack pointer and the reset vector.
 *
 *   The controller keeps running while the rest of the system sleeps,
 *   which is the point of it: work that would otherwise wake the M33 --
 *   polling a sensor, waiting for a threshold -- costs nothing here.
 *
 * Input Parameters:
 *   fw  - the image
 *   len - its length in bytes, at most DA1470X_SNC_CODE_SIZE
 *
 * Returned Value:
 *   Zero on success, a negated errno otherwise.
 *
 ****************************************************************************/

int da1470x_snc_start(const void *fw, size_t len);

/****************************************************************************
 * Name: da1470x_snc_stop
 *
 * Description:
 *   Hold the controller in reset and take its clock away.  Its RAM keeps
 *   whatever it held, so a stopped controller can be inspected.
 *
 ****************************************************************************/

void da1470x_snc_stop(void);

/****************************************************************************
 * Name: da1470x_snc_running
 ****************************************************************************/

bool da1470x_snc_running(void);

/****************************************************************************
 * Name: da1470x_snc_status
 *
 * Description:
 *   The controller's status register: whether it is idle, locked up, or
 *   has been bitten by its watchdog.
 *
 ****************************************************************************/

uint32_t da1470x_snc_status(void);

#ifdef CONFIG_DA1470X_SNC_SMOKETEST

/****************************************************************************
 * Name: da1470x_snc_smoketest
 *
 * Description:
 *   Run a firmware that signs its own RAM and then counts, and report
 *   whether the signature arrived and the count moved.  Proof that the
 *   second processor is executing, before anything is asked of it.
 *
 ****************************************************************************/

int da1470x_snc_smoketest(void);
#endif

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_SNC_H */
