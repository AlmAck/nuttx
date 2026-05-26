/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_wkup.h
 *
 * WKUP (wake-up) block register definitions for DA1470x.
 *
 * The WKUP block routes per-pin GPIO edge events to:
 *   - the debounced KEY_WKUP_GPIO interrupt (NVIC line 4, "any key" IRQ
 *     after a configurable debounce period), and
 *   - the per-port GPIO_P0/P1/P2 toggle interrupts (NVIC lines 14/15/16),
 *     which fire on any selected pin's edge without debounce.
 *
 * The SELECT/POL/STATUS/CLEAR per-port registers are shared by both
 * paths; CTRL_REG.WKUP_ENABLE_IRQ gates the debounced path independently.
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.
 *
 ****************************************************************************/

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_WKUP_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_WKUP_H

#include <nuttx/config.h>

/* Register Offsets *********************************************************/

#define DA1470_WKUP_CTRL_OFFSET           0x0000
#define DA1470_WKUP_RESET_IRQ_OFFSET      0x0004
#define DA1470_WKUP_SELECT_P0_OFFSET      0x0008
#define DA1470_WKUP_SELECT_P1_OFFSET      0x000C
#define DA1470_WKUP_SELECT_P2_OFFSET      0x0010
#define DA1470_WKUP_POL_P0_OFFSET         0x0014
#define DA1470_WKUP_POL_P1_OFFSET         0x0018
#define DA1470_WKUP_POL_P2_OFFSET         0x001C
#define DA1470_WKUP_STATUS_P0_OFFSET      0x0020
#define DA1470_WKUP_STATUS_P1_OFFSET      0x0024
#define DA1470_WKUP_STATUS_P2_OFFSET      0x0028
#define DA1470_WKUP_CLEAR_P0_OFFSET       0x002C
#define DA1470_WKUP_CLEAR_P1_OFFSET       0x0030
#define DA1470_WKUP_CLEAR_P2_OFFSET       0x0034

#define DA1470_WKUP_BASE                  0x50000900

#define DA1470_WKUP_CTRL                  (DA1470_WKUP_BASE + DA1470_WKUP_CTRL_OFFSET)
#define DA1470_WKUP_RESET_IRQ             (DA1470_WKUP_BASE + DA1470_WKUP_RESET_IRQ_OFFSET)
#define DA1470_WKUP_SELECT_P(p)           (DA1470_WKUP_BASE + DA1470_WKUP_SELECT_P0_OFFSET + ((p) * 4))
#define DA1470_WKUP_POL_P(p)              (DA1470_WKUP_BASE + DA1470_WKUP_POL_P0_OFFSET    + ((p) * 4))
#define DA1470_WKUP_STATUS_P(p)           (DA1470_WKUP_BASE + DA1470_WKUP_STATUS_P0_OFFSET + ((p) * 4))
#define DA1470_WKUP_CLEAR_P(p)            (DA1470_WKUP_BASE + DA1470_WKUP_CLEAR_P0_OFFSET  + ((p) * 4))

/* Polarity encoding (in WKUP_POL_Px): 0 = active high (rising edge),
 * 1 = active low (falling edge). One bit per pin.
 */

#define WKUP_POL_RISING   0
#define WKUP_POL_FALLING  1

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_WKUP_H */
