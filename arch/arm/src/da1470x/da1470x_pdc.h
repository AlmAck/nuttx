/****************************************************************************
 * arch/arm/src/da1470x/da1470x_pdc.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_PDC_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_PDC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdbool.h>
#include <stdint.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define DA1470X_PDC_LUT_SIZE        16
#define DA1470X_PDC_INVALID_ENTRY   (-1)

/* Trigger source select */

#define DA1470X_PDC_TRIG_P0_GPIO    0
#define DA1470X_PDC_TRIG_P1_GPIO    1
#define DA1470X_PDC_TRIG_P2_GPIO    2
#define DA1470X_PDC_TRIG_PERIPHERAL 3

/* Peripheral trigger identifiers (TRIG_SELECT = PERIPHERAL) */

#define DA1470X_PDC_PERIPH_TIMER      0x00
#define DA1470X_PDC_PERIPH_TIMER2     0x01
#define DA1470X_PDC_PERIPH_TIMER3     0x02
#define DA1470X_PDC_PERIPH_TIMER4     0x03
#define DA1470X_PDC_PERIPH_TIMER5     0x04
#define DA1470X_PDC_PERIPH_TIMER6     0x05
#define DA1470X_PDC_PERIPH_RTC_ALARM  0x06
#define DA1470X_PDC_PERIPH_RTC_TIMER  0x07
#define DA1470X_PDC_PERIPH_MAC_TIMER  0x08
#define DA1470X_PDC_PERIPH_VAD        0x09
#define DA1470X_PDC_PERIPH_XTAL32MRDY 0x0a
#define DA1470X_PDC_PERIPH_RFDIAG     0x0b
#define DA1470X_PDC_PERIPH_COMBO      0x0c  /* VBUS, JTAG, CMAC2SYS ... */
#define DA1470X_PDC_PERIPH_CMAC2SYS   0x0d
#define DA1470X_PDC_PERIPH_SNC2SYS    0x0e
#define DA1470X_PDC_PERIPH_MASTERONLY 0x0f
#define DA1470X_PDC_PERIPH_GPIO_P0    0x10
#define DA1470X_PDC_PERIPH_GPIO_P1    0x11
#define DA1470X_PDC_PERIPH_GPIO_P2    0x12
#define DA1470X_PDC_PERIPH_CMAC2SNC   0x13
#define DA1470X_PDC_PERIPH_SNC2CMAC   0x14
#define DA1470X_PDC_PERIPH_SYS2CMAC   0x15
#define DA1470X_PDC_PERIPH_SYS2SNC    0x16

/* Wake-up master */

#define DA1470X_PDC_MASTER_INVALID  0
#define DA1470X_PDC_MASTER_CM33     1
#define DA1470X_PDC_MASTER_CMAC     2
#define DA1470X_PDC_MASTER_SNC      3

/* Entry flags */

#define DA1470X_PDC_FLAG_EN_XTAL    (1 << 0)  /* Start XTAL32M on wake */
#define DA1470X_PDC_FLAG_EN_TMR     (1 << 1)  /* Power PD_TMR on wake */
#define DA1470X_PDC_FLAG_EN_SNC     (1 << 2)  /* Power PD_SNC on wake */

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
 * Name: da1470x_pdc_initialize
 *
 * Description:
 *   Clear the lookup table and attach the CM33 wake-up interrupt.
 *
 ****************************************************************************/

void da1470x_pdc_initialize(void);

/****************************************************************************
 * Name: da1470x_pdc_add
 *
 * Description:
 *   Add a lookup-table entry.  Returns the entry index or a negated errno
 *   if the table is full.  An identical existing entry is reused.
 *
 ****************************************************************************/

int da1470x_pdc_add(int trig_select, int trig_id, int master,
                    uint32_t flags);

/****************************************************************************
 * Name: da1470x_pdc_remove
 ****************************************************************************/

void da1470x_pdc_remove(int index);

/****************************************************************************
 * Name: da1470x_pdc_set_pending / da1470x_pdc_acknowledge
 *
 * Description:
 *   Software-trigger an entry, or clear its pending bit after the wake-up
 *   has been handled.
 *
 ****************************************************************************/

void da1470x_pdc_set_pending(int index);
void da1470x_pdc_acknowledge(int index);

/****************************************************************************
 * Name: da1470x_pdc_is_pending
 ****************************************************************************/

bool da1470x_pdc_is_pending(int index);

/****************************************************************************
 * Name: da1470x_pdc_ack_all_cm33
 *
 * Description:
 *   Acknowledge every entry pending for the CM33.  Must be called before
 *   entering sleep, otherwise the PMU wakes the core immediately.
 *
 ****************************************************************************/

void da1470x_pdc_ack_all_cm33(void);

/****************************************************************************
 * Name: da1470x_pdc_find
 *
 * Description:
 *   Return the index of the entry matching the given fields, or
 *   DA1470X_PDC_INVALID_ENTRY.
 *
 ****************************************************************************/

int da1470x_pdc_find(int trig_select, int trig_id, int master);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_PDC_H */
