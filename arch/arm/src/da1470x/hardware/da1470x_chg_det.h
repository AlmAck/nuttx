/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_chg_det.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_CHG_DET_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_CHG_DET_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470x_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define DA1470X_CHG_DET_FSM_CTRL_OFFSET 0x0000   /* Charge detection FSM control register */
#define DA1470X_CHG_DET_FSM_STATUS_OFFSET 0x0004 /* Charge detection FSM status register */
#define DA1470X_CHG_DET_SW_CTRL_OFFSET 0x0008    /* Charge detection manual (SW-based) mode */
#define DA1470X_CHG_DET_STATUS_OFFSET 0x000c     /* Charge detection status register holding */
#define DA1470X_CHG_DET_ADC_CTRL_OFFSET 0x0010   /* Charge detection ADC control register */
#define DA1470X_CHG_DET_TIMER_OFFSET 0x0014      /* Charge detection timer register (used in */
#define DA1470X_CHG_DET_DCD_TIMER_OFFSET 0x0018  /* Charge detection DCD time-out timer regi */
#define DA1470X_CHG_DET_IRQ_MASK_OFFSET 0x001c   /* Charge detection IRQ mask register */
#define DA1470X_CHG_DET_IRQ_STATUS_OFFSET 0x0020 /* Charge detection IRQ status register */
#define DA1470X_CHG_DET_IRQ_CLEAR_OFFSET 0x0024  /* Charge detection IRQ clear register */

/* Register Addresses *******************************************************/

#define DA1470X_CHG_DET_FSM_CTRL (DA1470X_CHG_DET_BASE + DA1470X_CHG_DET_FSM_CTRL_OFFSET)
#define DA1470X_CHG_DET_FSM_STATUS (DA1470X_CHG_DET_BASE + DA1470X_CHG_DET_FSM_STATUS_OFFSET)
#define DA1470X_CHG_DET_SW_CTRL (DA1470X_CHG_DET_BASE + DA1470X_CHG_DET_SW_CTRL_OFFSET)
#define DA1470X_CHG_DET_STATUS (DA1470X_CHG_DET_BASE + DA1470X_CHG_DET_STATUS_OFFSET)
#define DA1470X_CHG_DET_ADC_CTRL (DA1470X_CHG_DET_BASE + DA1470X_CHG_DET_ADC_CTRL_OFFSET)
#define DA1470X_CHG_DET_TIMER (DA1470X_CHG_DET_BASE + DA1470X_CHG_DET_TIMER_OFFSET)
#define DA1470X_CHG_DET_DCD_TIMER (DA1470X_CHG_DET_BASE + DA1470X_CHG_DET_DCD_TIMER_OFFSET)
#define DA1470X_CHG_DET_IRQ_MASK (DA1470X_CHG_DET_BASE + DA1470X_CHG_DET_IRQ_MASK_OFFSET)
#define DA1470X_CHG_DET_IRQ_STATUS (DA1470X_CHG_DET_BASE + DA1470X_CHG_DET_IRQ_STATUS_OFFSET)
#define DA1470X_CHG_DET_IRQ_CLEAR (DA1470X_CHG_DET_BASE + DA1470X_CHG_DET_IRQ_CLEAR_OFFSET)

/* Register Bit-field Definitions *******************************************/

/* FSM_CTRL register */

#define CHG_DET_FSM_CTRL_EN (1 << 0) /* Bit 0 */

/* FSM_STATUS register */

#define CHG_DET_FSM_STATUS_DETECTION_COMPLETED (1 << 0)    /* Bit 0 */
#define CHG_DET_FSM_STATUS_SDP_PORT_DETECTED (1 << 1)      /* Bit 1 */
#define CHG_DET_FSM_STATUS_CDP_PORT_DETECTED (1 << 2)      /* Bit 2 */
#define CHG_DET_FSM_STATUS_DCP_PORT_DETECTED (1 << 3)      /* Bit 3 */
#define CHG_DET_FSM_STATUS_PS2_PROP_PORT_DETECTED (1 << 4) /* Bit 4 */
#define CHG_DET_FSM_STATUS_PORT_1AMP_DETECTED (1 << 5)     /* Bit 5 */
#define CHG_DET_FSM_STATUS_PORT_2AMP_DETECTED (1 << 6)     /* Bit 6 */
#define CHG_DET_FSM_STATUS_PORT_2P4AMP_DETECTED (1 << 7)   /* Bit 7 */
#define CHG_DET_FSM_STATUS_NO_CONTACT_DETECTED (1 << 8)    /* Bit 8 */
#define CHG_DET_FSM_STATUS_STATE_SHIFT (9)                 /* Bits 9-12 */
#define CHG_DET_FSM_STATUS_STATE_MASK (0xf << CHG_DET_FSM_STATUS_STATE_SHIFT)
#  define CHG_DET_FSM_STATUS_STATE(n) ((uint32_t)(n) << CHG_DET_FSM_STATUS_STATE_SHIFT)

/* SW_CTRL register */

#define CHG_DET_SW_CTRL_USB_CHARGE_ON (1 << 0) /* Bit 0 */
#define CHG_DET_SW_CTRL_IDP_SRC_ON (1 << 1)    /* Bit 1 */
#define CHG_DET_SW_CTRL_VDP_SRC_ON (1 << 2)    /* Bit 2 */
#define CHG_DET_SW_CTRL_VDM_SRC_ON (1 << 3)    /* Bit 3 */
#define CHG_DET_SW_CTRL_IDP_SINK_ON (1 << 4)   /* Bit 4 */
#define CHG_DET_SW_CTRL_IDM_SINK_ON (1 << 5)   /* Bit 5 */

/* STATUS register */

#define CHG_DET_STATUS_USB_DCP_DET (1 << 0) /* Bit 0 */
#define CHG_DET_STATUS_USB_CHG_DET (1 << 1) /* Bit 1 */
#define CHG_DET_STATUS_USB_DP_VAL (1 << 2)  /* Bit 2 */
#define CHG_DET_STATUS_USB_DM_VAL (1 << 3)  /* Bit 3 */
#define CHG_DET_STATUS_USB_DP_VAL2 (1 << 4) /* Bit 4 */
#define CHG_DET_STATUS_USB_DM_VAL2 (1 << 5) /* Bit 5 */

/* ADC_CTRL register */

#define CHG_DET_ADC_CTRL_USB_DP_TO_ADC_EN (1 << 0) /* Bit 0 */
#define CHG_DET_ADC_CTRL_USB_DM_TO_ADC_EN (1 << 1) /* Bit 1 */

/* TIMER register */

#define CHG_DET_TIMER_TIMER_THRES_SHIFT (0) /* Bits 0-7 */
#define CHG_DET_TIMER_TIMER_THRES_MASK (0xff << CHG_DET_TIMER_TIMER_THRES_SHIFT)
#  define CHG_DET_TIMER_TIMER_THRES(n) ((uint32_t)(n) << CHG_DET_TIMER_TIMER_THRES_SHIFT)
#define CHG_DET_TIMER_TIMER_SHIFT (16)      /* Bits 16-23 */
#define CHG_DET_TIMER_TIMER_MASK (0xff << CHG_DET_TIMER_TIMER_SHIFT)
#  define CHG_DET_TIMER_TIMER(n) ((uint32_t)(n) << CHG_DET_TIMER_TIMER_SHIFT)

/* DCD_TIMER register */

#define CHG_DET_DCD_TIMER_DCD_TIMEOUT_THRES_SHIFT (0) /* Bits 0-9 */
#define CHG_DET_DCD_TIMER_DCD_TIMEOUT_THRES_MASK (0x3ff << CHG_DET_DCD_TIMER_DCD_TIMEOUT_THRES_SHIFT)
#  define CHG_DET_DCD_TIMER_DCD_TIMEOUT_THRES(n) ((uint32_t)(n) << CHG_DET_DCD_TIMER_DCD_TIMEOUT_THRES_SHIFT)
#define CHG_DET_DCD_TIMER_DCD_TIMER_SHIFT (16)        /* Bits 16-25 */
#define CHG_DET_DCD_TIMER_DCD_TIMER_MASK (0x3ff << CHG_DET_DCD_TIMER_DCD_TIMER_SHIFT)
#  define CHG_DET_DCD_TIMER_DCD_TIMER(n) ((uint32_t)(n) << CHG_DET_DCD_TIMER_DCD_TIMER_SHIFT)

/* IRQ_MASK register */

#define CHG_DET_IRQ_MASK_IRQ_EN (1 << 0) /* Bit 0 */

/* IRQ_STATUS register */

#define CHG_DET_IRQ_STATUS_IRQ (1 << 0) /* Bit 0 */

/* IRQ_CLEAR register */

#define CHG_DET_IRQ_CLEAR_IRQ_CLR (1 << 0) /* Bit 0 */

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_CHG_DET_H */
