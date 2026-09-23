/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_rtc.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_RTC_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_RTC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470x_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define DA1470X_RTC_CONTROL_OFFSET 0x0000           /* RTC Control Register */
#define DA1470X_RTC_HOUR_MODE_OFFSET 0x0004         /* RTC Hour Mode Register */
#define DA1470X_RTC_TIME_OFFSET 0x0008              /* RTC Time Register */
#define DA1470X_RTC_CALENDAR_OFFSET 0x000c          /* RTC Calendar Register */
#define DA1470X_RTC_TIME_ALARM_OFFSET 0x0010        /* RTC Time Alarm Register */
#define DA1470X_RTC_CALENDAR_ALARM_OFFSET 0x0014    /* RTC Calendar Alram Register */
#define DA1470X_RTC_ALARM_ENABLE_OFFSET 0x0018      /* RTC Alarm Enable Register */
#define DA1470X_RTC_EVENT_FLAGS_OFFSET 0x001c       /* RTC Event Flags Register */
#define DA1470X_RTC_INTERRUPT_ENABLE_OFFSET 0x0020  /* RTC Interrupt Enable Register */
#define DA1470X_RTC_INTERRUPT_DISABLE_OFFSET 0x0024 /* RTC Interrupt Disable Register */
#define DA1470X_RTC_INTERRUPT_MASK_OFFSET 0x0028    /* RTC Interrupt Mask Register */
#define DA1470X_RTC_STATUS_OFFSET 0x002c            /* RTC Status Register */
#define DA1470X_RTC_KEEP_RTC_OFFSET 0x0030          /* RTC Keep RTC Register */
#define DA1470X_RTC_EVENT_CTRL_OFFSET 0x0080        /* RTC Event Control Register */
#define DA1470X_RTC_PDC_EVENT_PERIOD_OFFSET 0x0088  /* RTC PDC Event Period Register */
#define DA1470X_RTC_PDC_EVENT_CLEAR_OFFSET 0x008c   /* RTC PDC Event Clear Register */
#define DA1470X_RTC_PDC_EVENT_CNT_OFFSET 0x0094     /* RTC PDC Event Counter Register */

/* Register Addresses *******************************************************/

#define DA1470X_RTC_CONTROL (DA1470X_RTC_BASE + DA1470X_RTC_CONTROL_OFFSET)
#define DA1470X_RTC_HOUR_MODE (DA1470X_RTC_BASE + DA1470X_RTC_HOUR_MODE_OFFSET)
#define DA1470X_RTC_TIME (DA1470X_RTC_BASE + DA1470X_RTC_TIME_OFFSET)
#define DA1470X_RTC_CALENDAR (DA1470X_RTC_BASE + DA1470X_RTC_CALENDAR_OFFSET)
#define DA1470X_RTC_TIME_ALARM (DA1470X_RTC_BASE + DA1470X_RTC_TIME_ALARM_OFFSET)
#define DA1470X_RTC_CALENDAR_ALARM (DA1470X_RTC_BASE + DA1470X_RTC_CALENDAR_ALARM_OFFSET)
#define DA1470X_RTC_ALARM_ENABLE (DA1470X_RTC_BASE + DA1470X_RTC_ALARM_ENABLE_OFFSET)
#define DA1470X_RTC_EVENT_FLAGS (DA1470X_RTC_BASE + DA1470X_RTC_EVENT_FLAGS_OFFSET)
#define DA1470X_RTC_INTERRUPT_ENABLE (DA1470X_RTC_BASE + DA1470X_RTC_INTERRUPT_ENABLE_OFFSET)
#define DA1470X_RTC_INTERRUPT_DISABLE (DA1470X_RTC_BASE + DA1470X_RTC_INTERRUPT_DISABLE_OFFSET)
#define DA1470X_RTC_INTERRUPT_MASK (DA1470X_RTC_BASE + DA1470X_RTC_INTERRUPT_MASK_OFFSET)
#define DA1470X_RTC_STATUS (DA1470X_RTC_BASE + DA1470X_RTC_STATUS_OFFSET)
#define DA1470X_RTC_KEEP_RTC (DA1470X_RTC_BASE + DA1470X_RTC_KEEP_RTC_OFFSET)
#define DA1470X_RTC_EVENT_CTRL (DA1470X_RTC_BASE + DA1470X_RTC_EVENT_CTRL_OFFSET)
#define DA1470X_RTC_PDC_EVENT_PERIOD (DA1470X_RTC_BASE + DA1470X_RTC_PDC_EVENT_PERIOD_OFFSET)
#define DA1470X_RTC_PDC_EVENT_CLEAR (DA1470X_RTC_BASE + DA1470X_RTC_PDC_EVENT_CLEAR_OFFSET)
#define DA1470X_RTC_PDC_EVENT_CNT (DA1470X_RTC_BASE + DA1470X_RTC_PDC_EVENT_CNT_OFFSET)

/* Register Bit-field Definitions *******************************************/

/* CONTROL register */

#define RTC_CONTROL_TIME_DISABLE (1 << 0) /* Bit 0 */
#define RTC_CONTROL_CAL_DISABLE (1 << 1)  /* Bit 1 */

/* HOUR_MODE register */

#define RTC_HOUR_MODE_HMS (1 << 0) /* Bit 0 */

/* TIME register */

#define RTC_TIME_TIME_H_U_SHIFT (0)   /* Bits 0-3 */
#define RTC_TIME_TIME_H_U_MASK (0xf << RTC_TIME_TIME_H_U_SHIFT)
#  define RTC_TIME_TIME_H_U(n) ((uint32_t)(n) << RTC_TIME_TIME_H_U_SHIFT)
#define RTC_TIME_TIME_H_T_SHIFT (4)   /* Bits 4-7 */
#define RTC_TIME_TIME_H_T_MASK (0xf << RTC_TIME_TIME_H_T_SHIFT)
#  define RTC_TIME_TIME_H_T(n) ((uint32_t)(n) << RTC_TIME_TIME_H_T_SHIFT)
#define RTC_TIME_TIME_S_U_SHIFT (8)   /* Bits 8-11 */
#define RTC_TIME_TIME_S_U_MASK (0xf << RTC_TIME_TIME_S_U_SHIFT)
#  define RTC_TIME_TIME_S_U(n) ((uint32_t)(n) << RTC_TIME_TIME_S_U_SHIFT)
#define RTC_TIME_TIME_S_T_SHIFT (12)  /* Bits 12-14 */
#define RTC_TIME_TIME_S_T_MASK (0x7 << RTC_TIME_TIME_S_T_SHIFT)
#  define RTC_TIME_TIME_S_T(n) ((uint32_t)(n) << RTC_TIME_TIME_S_T_SHIFT)
#define RTC_TIME_TIME_M_U_SHIFT (16)  /* Bits 16-19 */
#define RTC_TIME_TIME_M_U_MASK (0xf << RTC_TIME_TIME_M_U_SHIFT)
#  define RTC_TIME_TIME_M_U(n) ((uint32_t)(n) << RTC_TIME_TIME_M_U_SHIFT)
#define RTC_TIME_TIME_M_T_SHIFT (20)  /* Bits 20-22 */
#define RTC_TIME_TIME_M_T_MASK (0x7 << RTC_TIME_TIME_M_T_SHIFT)
#  define RTC_TIME_TIME_M_T(n) ((uint32_t)(n) << RTC_TIME_TIME_M_T_SHIFT)
#define RTC_TIME_TIME_HR_U_SHIFT (24) /* Bits 24-27 */
#define RTC_TIME_TIME_HR_U_MASK (0xf << RTC_TIME_TIME_HR_U_SHIFT)
#  define RTC_TIME_TIME_HR_U(n) ((uint32_t)(n) << RTC_TIME_TIME_HR_U_SHIFT)
#define RTC_TIME_TIME_HR_T_SHIFT (28) /* Bits 28-29 */
#define RTC_TIME_TIME_HR_T_MASK (0x3 << RTC_TIME_TIME_HR_T_SHIFT)
#  define RTC_TIME_TIME_HR_T(n) ((uint32_t)(n) << RTC_TIME_TIME_HR_T_SHIFT)
#define RTC_TIME_TIME_PM (1 << 30)    /* Bit 30 */
#define RTC_TIME_TIME_CH (1 << 31)    /* Bit 31 */

/* CALENDAR register */

#define RTC_CALENDAR_DAY_SHIFT (0)      /* Bits 0-2 */
#define RTC_CALENDAR_DAY_MASK (0x7 << RTC_CALENDAR_DAY_SHIFT)
#  define RTC_CALENDAR_DAY(n) ((uint32_t)(n) << RTC_CALENDAR_DAY_SHIFT)
#define RTC_CALENDAR_CAL_M_U_SHIFT (3)  /* Bits 3-6 */
#define RTC_CALENDAR_CAL_M_U_MASK (0xf << RTC_CALENDAR_CAL_M_U_SHIFT)
#  define RTC_CALENDAR_CAL_M_U(n) ((uint32_t)(n) << RTC_CALENDAR_CAL_M_U_SHIFT)
#define RTC_CALENDAR_CAL_M_T (1 << 7)   /* Bit 7 */
#define RTC_CALENDAR_CAL_D_U_SHIFT (8)  /* Bits 8-11 */
#define RTC_CALENDAR_CAL_D_U_MASK (0xf << RTC_CALENDAR_CAL_D_U_SHIFT)
#  define RTC_CALENDAR_CAL_D_U(n) ((uint32_t)(n) << RTC_CALENDAR_CAL_D_U_SHIFT)
#define RTC_CALENDAR_CAL_D_T_SHIFT (12) /* Bits 12-13 */
#define RTC_CALENDAR_CAL_D_T_MASK (0x3 << RTC_CALENDAR_CAL_D_T_SHIFT)
#  define RTC_CALENDAR_CAL_D_T(n) ((uint32_t)(n) << RTC_CALENDAR_CAL_D_T_SHIFT)
#define RTC_CALENDAR_CAL_Y_U_SHIFT (16) /* Bits 16-19 */
#define RTC_CALENDAR_CAL_Y_U_MASK (0xf << RTC_CALENDAR_CAL_Y_U_SHIFT)
#  define RTC_CALENDAR_CAL_Y_U(n) ((uint32_t)(n) << RTC_CALENDAR_CAL_Y_U_SHIFT)
#define RTC_CALENDAR_CAL_Y_T_SHIFT (20) /* Bits 20-23 */
#define RTC_CALENDAR_CAL_Y_T_MASK (0xf << RTC_CALENDAR_CAL_Y_T_SHIFT)
#  define RTC_CALENDAR_CAL_Y_T(n) ((uint32_t)(n) << RTC_CALENDAR_CAL_Y_T_SHIFT)
#define RTC_CALENDAR_CAL_C_U_SHIFT (24) /* Bits 24-27 */
#define RTC_CALENDAR_CAL_C_U_MASK (0xf << RTC_CALENDAR_CAL_C_U_SHIFT)
#  define RTC_CALENDAR_CAL_C_U(n) ((uint32_t)(n) << RTC_CALENDAR_CAL_C_U_SHIFT)
#define RTC_CALENDAR_CAL_C_T_SHIFT (28) /* Bits 28-29 */
#define RTC_CALENDAR_CAL_C_T_MASK (0x3 << RTC_CALENDAR_CAL_C_T_SHIFT)
#  define RTC_CALENDAR_CAL_C_T(n) ((uint32_t)(n) << RTC_CALENDAR_CAL_C_T_SHIFT)
#define RTC_CALENDAR_CAL_CH (1 << 31)   /* Bit 31 */

/* TIME_ALARM register */

#define RTC_TIME_ALARM_TIME_H_U_SHIFT (0)   /* Bits 0-3 */
#define RTC_TIME_ALARM_TIME_H_U_MASK (0xf << RTC_TIME_ALARM_TIME_H_U_SHIFT)
#  define RTC_TIME_ALARM_TIME_H_U(n) ((uint32_t)(n) << RTC_TIME_ALARM_TIME_H_U_SHIFT)
#define RTC_TIME_ALARM_TIME_H_T_SHIFT (4)   /* Bits 4-7 */
#define RTC_TIME_ALARM_TIME_H_T_MASK (0xf << RTC_TIME_ALARM_TIME_H_T_SHIFT)
#  define RTC_TIME_ALARM_TIME_H_T(n) ((uint32_t)(n) << RTC_TIME_ALARM_TIME_H_T_SHIFT)
#define RTC_TIME_ALARM_TIME_S_U_SHIFT (8)   /* Bits 8-11 */
#define RTC_TIME_ALARM_TIME_S_U_MASK (0xf << RTC_TIME_ALARM_TIME_S_U_SHIFT)
#  define RTC_TIME_ALARM_TIME_S_U(n) ((uint32_t)(n) << RTC_TIME_ALARM_TIME_S_U_SHIFT)
#define RTC_TIME_ALARM_TIME_S_T_SHIFT (12)  /* Bits 12-14 */
#define RTC_TIME_ALARM_TIME_S_T_MASK (0x7 << RTC_TIME_ALARM_TIME_S_T_SHIFT)
#  define RTC_TIME_ALARM_TIME_S_T(n) ((uint32_t)(n) << RTC_TIME_ALARM_TIME_S_T_SHIFT)
#define RTC_TIME_ALARM_TIME_M_U_SHIFT (16)  /* Bits 16-19 */
#define RTC_TIME_ALARM_TIME_M_U_MASK (0xf << RTC_TIME_ALARM_TIME_M_U_SHIFT)
#  define RTC_TIME_ALARM_TIME_M_U(n) ((uint32_t)(n) << RTC_TIME_ALARM_TIME_M_U_SHIFT)
#define RTC_TIME_ALARM_TIME_M_T_SHIFT (20)  /* Bits 20-22 */
#define RTC_TIME_ALARM_TIME_M_T_MASK (0x7 << RTC_TIME_ALARM_TIME_M_T_SHIFT)
#  define RTC_TIME_ALARM_TIME_M_T(n) ((uint32_t)(n) << RTC_TIME_ALARM_TIME_M_T_SHIFT)
#define RTC_TIME_ALARM_TIME_HR_U_SHIFT (24) /* Bits 24-27 */
#define RTC_TIME_ALARM_TIME_HR_U_MASK (0xf << RTC_TIME_ALARM_TIME_HR_U_SHIFT)
#  define RTC_TIME_ALARM_TIME_HR_U(n) ((uint32_t)(n) << RTC_TIME_ALARM_TIME_HR_U_SHIFT)
#define RTC_TIME_ALARM_TIME_HR_T_SHIFT (28) /* Bits 28-29 */
#define RTC_TIME_ALARM_TIME_HR_T_MASK (0x3 << RTC_TIME_ALARM_TIME_HR_T_SHIFT)
#  define RTC_TIME_ALARM_TIME_HR_T(n) ((uint32_t)(n) << RTC_TIME_ALARM_TIME_HR_T_SHIFT)
#define RTC_TIME_ALARM_TIME_PM (1 << 30)    /* Bit 30 */

/* CALENDAR_ALARM register */

#define RTC_CALENDAR_ALARM_CAL_M_U_SHIFT (3)  /* Bits 3-6 */
#define RTC_CALENDAR_ALARM_CAL_M_U_MASK (0xf << RTC_CALENDAR_ALARM_CAL_M_U_SHIFT)
#  define RTC_CALENDAR_ALARM_CAL_M_U(n) ((uint32_t)(n) << RTC_CALENDAR_ALARM_CAL_M_U_SHIFT)
#define RTC_CALENDAR_ALARM_CAL_M_T (1 << 7)   /* Bit 7 */
#define RTC_CALENDAR_ALARM_CAL_D_U_SHIFT (8)  /* Bits 8-11 */
#define RTC_CALENDAR_ALARM_CAL_D_U_MASK (0xf << RTC_CALENDAR_ALARM_CAL_D_U_SHIFT)
#  define RTC_CALENDAR_ALARM_CAL_D_U(n) ((uint32_t)(n) << RTC_CALENDAR_ALARM_CAL_D_U_SHIFT)
#define RTC_CALENDAR_ALARM_CAL_D_T_SHIFT (12) /* Bits 12-13 */
#define RTC_CALENDAR_ALARM_CAL_D_T_MASK (0x3 << RTC_CALENDAR_ALARM_CAL_D_T_SHIFT)
#  define RTC_CALENDAR_ALARM_CAL_D_T(n) ((uint32_t)(n) << RTC_CALENDAR_ALARM_CAL_D_T_SHIFT)

/* ALARM_ENABLE register */

#define RTC_ALARM_ENABLE_ALARM_HOS_EN (1 << 0)  /* Bit 0 */
#define RTC_ALARM_ENABLE_ALARM_SEC_EN (1 << 1)  /* Bit 1 */
#define RTC_ALARM_ENABLE_ALARM_MIN_EN (1 << 2)  /* Bit 2 */
#define RTC_ALARM_ENABLE_ALARM_HOUR_EN (1 << 3) /* Bit 3 */
#define RTC_ALARM_ENABLE_ALARM_DATE_EN (1 << 4) /* Bit 4 */
#define RTC_ALARM_ENABLE_ALARM_MNTH_EN (1 << 5) /* Bit 5 */

/* EVENT_FLAGS register */

#define RTC_EVENT_FLAGS_EVENT_HOS (1 << 0)  /* Bit 0 */
#define RTC_EVENT_FLAGS_EVENT_SEC (1 << 1)  /* Bit 1 */
#define RTC_EVENT_FLAGS_EVENT_MIN (1 << 2)  /* Bit 2 */
#define RTC_EVENT_FLAGS_EVENT_HOUR (1 << 3) /* Bit 3 */
#define RTC_EVENT_FLAGS_EVENT_DATE (1 << 4) /* Bit 4 */
#define RTC_EVENT_FLAGS_EVENT_MNTH (1 << 5) /* Bit 5 */
#define RTC_EVENT_FLAGS_EVENT_ALRM (1 << 6) /* Bit 6 */

/* INTERRUPT_ENABLE register */

#define RTC_INTERRUPT_ENABLE_HOS_INT_EN (1 << 0)  /* Bit 0 */
#define RTC_INTERRUPT_ENABLE_SEC_INT_EN (1 << 1)  /* Bit 1 */
#define RTC_INTERRUPT_ENABLE_MIN_INT_EN (1 << 2)  /* Bit 2 */
#define RTC_INTERRUPT_ENABLE_HOUR_INT_EN (1 << 3) /* Bit 3 */
#define RTC_INTERRUPT_ENABLE_DATE_INT_EN (1 << 4) /* Bit 4 */
#define RTC_INTERRUPT_ENABLE_MNTH_INT_EN (1 << 5) /* Bit 5 */
#define RTC_INTERRUPT_ENABLE_ALRM_INT_EN (1 << 6) /* Bit 6 */

/* INTERRUPT_DISABLE register */

#define RTC_INTERRUPT_DISABLE_HOS_INT_DIS (1 << 0)  /* Bit 0 */
#define RTC_INTERRUPT_DISABLE_SEC_INT_DIS (1 << 1)  /* Bit 1 */
#define RTC_INTERRUPT_DISABLE_MIN_INT_DIS (1 << 2)  /* Bit 2 */
#define RTC_INTERRUPT_DISABLE_HOUR_INT_DIS (1 << 3) /* Bit 3 */
#define RTC_INTERRUPT_DISABLE_DATE_INT_DIS (1 << 4) /* Bit 4 */
#define RTC_INTERRUPT_DISABLE_MNTH_INT_DIS (1 << 5) /* Bit 5 */
#define RTC_INTERRUPT_DISABLE_ALRM_INT_DIS (1 << 6) /* Bit 6 */

/* INTERRUPT_MASK register */

#define RTC_INTERRUPT_MASK_HOS_INT_MSK (1 << 0)  /* Bit 0 */
#define RTC_INTERRUPT_MASK_SEC_INT_MSK (1 << 1)  /* Bit 1 */
#define RTC_INTERRUPT_MASK_MIN_INT_MSK (1 << 2)  /* Bit 2 */
#define RTC_INTERRUPT_MASK_HOUR_INT_MSK (1 << 3) /* Bit 3 */
#define RTC_INTERRUPT_MASK_DATE_INT_MSK (1 << 4) /* Bit 4 */
#define RTC_INTERRUPT_MASK_MNTH_INT_MSK (1 << 5) /* Bit 5 */
#define RTC_INTERRUPT_MASK_ALRM_INT_MSK (1 << 6) /* Bit 6 */

/* STATUS register */

#define RTC_STATUS_VALID_TIME (1 << 0)     /* Bit 0 */
#define RTC_STATUS_VALID_CAL (1 << 1)      /* Bit 1 */
#define RTC_STATUS_VALID_TIME_ALM (1 << 2) /* Bit 2 */
#define RTC_STATUS_VALID_CAL_ALM (1 << 3)  /* Bit 3 */

/* KEEP_RTC register */

#define RTC_KEEP_RTC_KEEP (1 << 0) /* Bit 0 */

/* EVENT_CTRL register */

#define RTC_EVENT_CTRL_PDC_EVENT_EN (1 << 1) /* Bit 1 */

/* PDC_EVENT_PERIOD register */

#define RTC_PDC_EVENT_PERIOD_PDC_EVENT_PERIOD_SHIFT (0) /* Bits 0-12 */
#define RTC_PDC_EVENT_PERIOD_PDC_EVENT_PERIOD_MASK (0x1fff << RTC_PDC_EVENT_PERIOD_PDC_EVENT_PERIOD_SHIFT)
#  define RTC_PDC_EVENT_PERIOD_PDC_EVENT_PERIOD(n) ((uint32_t)(n) << RTC_PDC_EVENT_PERIOD_PDC_EVENT_PERIOD_SHIFT)

/* PDC_EVENT_CLEAR register */

#define RTC_PDC_EVENT_CLEAR_PDC_EVENT_CLEAR (1 << 0) /* Bit 0 */

/* PDC_EVENT_CNT register */

#define RTC_PDC_EVENT_CNT_PDC_EVENT_CNT_SHIFT (0) /* Bits 0-12 */
#define RTC_PDC_EVENT_CNT_PDC_EVENT_CNT_MASK (0x1fff << RTC_PDC_EVENT_CNT_PDC_EVENT_CNT_SHIFT)
#  define RTC_PDC_EVENT_CNT_PDC_EVENT_CNT(n) ((uint32_t)(n) << RTC_PDC_EVENT_CNT_PDC_EVENT_CNT_SHIFT)

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_RTC_H */
