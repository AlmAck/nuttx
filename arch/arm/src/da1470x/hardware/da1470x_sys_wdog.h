/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_sys_wdog.h
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

/* Generated from the Renesas CMSIS device header DA1470x-00.h (SDK
 * 10.2.6.49).  Do not edit by hand; regenerate with tools/genhdr.py.
 */

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_SYS_WDOG_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_SYS_WDOG_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470x_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define DA1470X_SYS_WDOG_WATCHDOG_OFFSET 0x0000      /* Watchdog timer register. */
#define DA1470X_SYS_WDOG_WATCHDOG_CTRL_OFFSET 0x0004 /* Watchdog control register. */

/* Register Addresses *******************************************************/

#define DA1470X_SYS_WDOG_WATCHDOG (DA1470X_SYS_WDOG_BASE + DA1470X_SYS_WDOG_WATCHDOG_OFFSET)
#define DA1470X_SYS_WDOG_WATCHDOG_CTRL (DA1470X_SYS_WDOG_BASE + DA1470X_SYS_WDOG_WATCHDOG_CTRL_OFFSET)

/* Register Bit-field Definitions *******************************************/

/* WATCHDOG register */

#define SYS_WDOG_WATCHDOG_WDOG_VAL_SHIFT (0)     /* Bits 0-12 */
#define SYS_WDOG_WATCHDOG_WDOG_VAL_MASK (0x1fff << SYS_WDOG_WATCHDOG_WDOG_VAL_SHIFT)
#  define SYS_WDOG_WATCHDOG_WDOG_VAL(n) ((uint32_t)(n) << SYS_WDOG_WATCHDOG_WDOG_VAL_SHIFT)
#define SYS_WDOG_WATCHDOG_WDOG_VAL_NEG (1 << 13) /* Bit 13 */
#define SYS_WDOG_WATCHDOG_WDOG_WEN_SHIFT (14)    /* Bits 14-31 */
#define SYS_WDOG_WATCHDOG_WDOG_WEN_MASK (0x3ffff << SYS_WDOG_WATCHDOG_WDOG_WEN_SHIFT)
#  define SYS_WDOG_WATCHDOG_WDOG_WEN(n) ((uint32_t)(n) << SYS_WDOG_WATCHDOG_WDOG_WEN_SHIFT)

/* WATCHDOG_CTRL register */

#define SYS_WDOG_WATCHDOG_CTRL_NMI_RST (1 << 0)        /* Bit 0 */
#define SYS_WDOG_WATCHDOG_CTRL_WDOG_FREEZE_EN (1 << 2) /* Bit 2 */
#define SYS_WDOG_WATCHDOG_CTRL_WRITE_BUSY (1 << 3)     /* Bit 3 */

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_SYS_WDOG_H */
