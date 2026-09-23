/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_gpreg.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_GPREG_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_GPREG_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470x_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define DA1470X_GPREG_SET_FREEZE_OFFSET 0x0000   /* Controls freezing of various timers/coun */
#define DA1470X_GPREG_RESET_FREEZE_OFFSET 0x0004 /* Controls unfreezing of various timers/co */
#define DA1470X_GPREG_DEBUG_OFFSET 0x0008        /* Various debug information register. */
#define DA1470X_GPREG_GP_STATUS_OFFSET 0x000c    /* General purpose system status register. */
#define DA1470X_GPREG_USBPAD_OFFSET 0x0018       /* USB pads control register */

/* Register Addresses *******************************************************/

#define DA1470X_GPREG_SET_FREEZE (DA1470X_GPREG_BASE + DA1470X_GPREG_SET_FREEZE_OFFSET)
#define DA1470X_GPREG_RESET_FREEZE (DA1470X_GPREG_BASE + DA1470X_GPREG_RESET_FREEZE_OFFSET)
#define DA1470X_GPREG_DEBUG (DA1470X_GPREG_BASE + DA1470X_GPREG_DEBUG_OFFSET)
#define DA1470X_GPREG_GP_STATUS (DA1470X_GPREG_BASE + DA1470X_GPREG_GP_STATUS_OFFSET)
#define DA1470X_GPREG_USBPAD (DA1470X_GPREG_BASE + DA1470X_GPREG_USBPAD_OFFSET)

/* Register Bit-field Definitions *******************************************/

/* SET_FREEZE register */

#define GPREG_SET_FREEZE_FRZ_WKUPTIM (1 << 0)    /* Bit 0 */
#define GPREG_SET_FREEZE_FRZ_SWTIM (1 << 1)      /* Bit 1 */
#define GPREG_SET_FREEZE_FRZ_RESERVED (1 << 2)   /* Bit 2 */
#define GPREG_SET_FREEZE_FRZ_SYS_WDOG (1 << 3)   /* Bit 3 */
#define GPREG_SET_FREEZE_FRZ_USB (1 << 4)        /* Bit 4 */
#define GPREG_SET_FREEZE_FRZ_DMA (1 << 5)        /* Bit 5 */
#define GPREG_SET_FREEZE_FRZ_SWTIM2 (1 << 6)     /* Bit 6 */
#define GPREG_SET_FREEZE_FRZ_PWMLED (1 << 7)     /* Bit 7 */
#define GPREG_SET_FREEZE_FRZ_SWTIM3 (1 << 8)     /* Bit 8 */
#define GPREG_SET_FREEZE_FRZ_SWTIM4 (1 << 9)     /* Bit 9 */
#define GPREG_SET_FREEZE_FRZ_CMAC_WDOG (1 << 10) /* Bit 10 */
#define GPREG_SET_FREEZE_FRZ_SNC_WDOG (1 << 11)  /* Bit 11 */
#define GPREG_SET_FREEZE_FRZ_SWTIM5 (1 << 12)    /* Bit 12 */
#define GPREG_SET_FREEZE_FRZ_SWTIM6 (1 << 13)    /* Bit 13 */

/* RESET_FREEZE register */

#define GPREG_RESET_FREEZE_FRZ_WKUPTIM (1 << 0)    /* Bit 0 */
#define GPREG_RESET_FREEZE_FRZ_SWTIM (1 << 1)      /* Bit 1 */
#define GPREG_RESET_FREEZE_FRZ_RESERVED (1 << 2)   /* Bit 2 */
#define GPREG_RESET_FREEZE_FRZ_SYS_WDOG (1 << 3)   /* Bit 3 */
#define GPREG_RESET_FREEZE_FRZ_USB (1 << 4)        /* Bit 4 */
#define GPREG_RESET_FREEZE_FRZ_DMA (1 << 5)        /* Bit 5 */
#define GPREG_RESET_FREEZE_FRZ_SWTIM2 (1 << 6)     /* Bit 6 */
#define GPREG_RESET_FREEZE_FRZ_PWMLED (1 << 7)     /* Bit 7 */
#define GPREG_RESET_FREEZE_FRZ_SWTIM3 (1 << 8)     /* Bit 8 */
#define GPREG_RESET_FREEZE_FRZ_SWTIM4 (1 << 9)     /* Bit 9 */
#define GPREG_RESET_FREEZE_FRZ_CMAC_WDOG (1 << 10) /* Bit 10 */
#define GPREG_RESET_FREEZE_FRZ_SNC_WDOG (1 << 11)  /* Bit 11 */
#define GPREG_RESET_FREEZE_FRZ_SWTIM5 (1 << 12)    /* Bit 12 */
#define GPREG_RESET_FREEZE_FRZ_SWTIM6 (1 << 13)    /* Bit 13 */

/* DEBUG register */

#define GPREG_DEBUG_SYS_CPU_FREEZE_EN (1 << 0)          /* Bit 0 */
#define GPREG_DEBUG_CMAC_CPU_FREEZE_EN (1 << 1)         /* Bit 1 */
#define GPREG_DEBUG_HALT_CMAC_CPU_EN (1 << 2)           /* Bit 2 */
#define GPREG_DEBUG_HALT_SYS_CPU_EN (1 << 3)            /* Bit 3 */
#define GPREG_DEBUG_SYS_CPU_IS_HALTED (1 << 4)          /* Bit 4 */
#define GPREG_DEBUG_CMAC_CPU_IS_HALTED (1 << 5)         /* Bit 5 */
#define GPREG_DEBUG_SYS_CPUWAIT (1 << 6)                /* Bit 6 */
#define GPREG_DEBUG_SYS_CPUWAIT_ON_JTAG (1 << 7)        /* Bit 7 */
#define GPREG_DEBUG_CROSS_CPU_HALT_SENSITIVITY (1 << 8) /* Bit 8 */
#define GPREG_DEBUG_SNC_CPU_FREEZE_EN (1 << 9)          /* Bit 9 */
#define GPREG_DEBUG_HALT_SNC_CPU_EN (1 << 10)           /* Bit 10 */
#define GPREG_DEBUG_SNC_CPU_IS_HALTED (1 << 11)         /* Bit 11 */
#define GPREG_DEBUG_ETM_TRACE_MAP_ON_PINS_EN (1 << 12)  /* Bit 12 */

/* GP_STATUS register */

#define GPREG_GP_STATUS_CAL_PHASE (1 << 0) /* Bit 0 */

/* USBPAD register */

#define GPREG_USBPAD_USBPAD_EN (1 << 0)            /* Bit 0 */
#define GPREG_USBPAD_USBPHY_FORCE_SW1_OFF (1 << 1) /* Bit 1 */
#define GPREG_USBPAD_USBPHY_FORCE_SW2_ON (1 << 2)  /* Bit 2 */

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_GPREG_H */
