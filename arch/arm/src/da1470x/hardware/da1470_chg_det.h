/****************************************************************************
 * ./tmp/da1470_chg_det.h
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

#ifndef __DA1470_CHG_DET_H
#define __DA1470_CHG_DET_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register offsets *********************************************************/

#define DA1470_CHG_DET_CHG_DET_ADC_CTRL_OFFSET    0x0010 /* Charge detection ADC control register */
#define DA1470_CHG_DET_CHG_DET_DCD_TIMER_OFFSET   0x0018 /* Charge detection DCD time-out timer register (used in the FSM) */
#define DA1470_CHG_DET_CHG_DET_FSM_CTRL_OFFSET    0x0000 /* Charge detection FSM control register */
#define DA1470_CHG_DET_CHG_DET_FSM_STATUS_OFFSET  0x0004 /* Charge detection FSM status register */
#define DA1470_CHG_DET_CHG_DET_IRQ_CLEAR_OFFSET   0x0024 /* Charge detection IRQ clear register */
#define DA1470_CHG_DET_CHG_DET_IRQ_MASK_OFFSET    0x001C /* Charge detection IRQ mask register */
#define DA1470_CHG_DET_CHG_DET_IRQ_STATUS_OFFSET  0x0020 /* Charge detection IRQ status register */
#define DA1470_CHG_DET_CHG_DET_STATUS_OFFSET      0x000C /* Charge detection status register holding the comparator outputs */
#define DA1470_CHG_DET_CHG_DET_SW_CTRL_OFFSET     0x0008 /* Charge detection manual (SW-based) mode control register */
#define DA1470_CHG_DET_CHG_DET_TIMER_OFFSET       0x0014 /* Charge detection timer register (used in the FSM) */

/* Register addresses *******************************************************/

#define DA1470_CHG_DET_BASE                       0x50040300 /* CHG_DET registers */

#define DA1470_CHG_DET_CHG_DET_ADC_CTRL           (DA1470_CHG_DET_BASE + DA1470_CHG_DET_CHG_DET_ADC_CTRL_OFFSET) /* Charge detection ADC control register */
#define DA1470_CHG_DET_CHG_DET_DCD_TIMER          (DA1470_CHG_DET_BASE + DA1470_CHG_DET_CHG_DET_DCD_TIMER_OFFSET) /* Charge detection DCD time-out timer register (used in the FSM) */
#define DA1470_CHG_DET_CHG_DET_FSM_CTRL           (DA1470_CHG_DET_BASE + DA1470_CHG_DET_CHG_DET_FSM_CTRL_OFFSET) /* Charge detection FSM control register */
#define DA1470_CHG_DET_CHG_DET_FSM_STATUS         (DA1470_CHG_DET_BASE + DA1470_CHG_DET_CHG_DET_FSM_STATUS_OFFSET) /* Charge detection FSM status register */
#define DA1470_CHG_DET_CHG_DET_IRQ_CLEAR          (DA1470_CHG_DET_BASE + DA1470_CHG_DET_CHG_DET_IRQ_CLEAR_OFFSET) /* Charge detection IRQ clear register */
#define DA1470_CHG_DET_CHG_DET_IRQ_MASK           (DA1470_CHG_DET_BASE + DA1470_CHG_DET_CHG_DET_IRQ_MASK_OFFSET) /* Charge detection IRQ mask register */
#define DA1470_CHG_DET_CHG_DET_IRQ_STATUS         (DA1470_CHG_DET_BASE + DA1470_CHG_DET_CHG_DET_IRQ_STATUS_OFFSET) /* Charge detection IRQ status register */
#define DA1470_CHG_DET_CHG_DET_STATUS             (DA1470_CHG_DET_BASE + DA1470_CHG_DET_CHG_DET_STATUS_OFFSET) /* Charge detection status register holding the comparator outputs */
#define DA1470_CHG_DET_CHG_DET_SW_CTRL            (DA1470_CHG_DET_BASE + DA1470_CHG_DET_CHG_DET_SW_CTRL_OFFSET) /* Charge detection manual (SW-based) mode control register */
#define DA1470_CHG_DET_CHG_DET_TIMER              (DA1470_CHG_DET_BASE + DA1470_CHG_DET_CHG_DET_TIMER_OFFSET) /* Charge detection timer register (used in the FSM) */

/* Register bit definitions *************************************************/


/* CHG_DET_ADC_CTRL Register */

#define CHG_DET_ADC_V30_SEL                       (1U << 2) /* This bit-field determines the mode to be used when the intention is to measure t */
#define CHG_DET_USB_DM_TO_ADC_EN                  (1U << 1) /* 0 = Disables the connection of USBm to the GP_ADC 1 = Enables the connection of  */
#define CHG_DET_USB_DP_TO_ADC_EN                  (1U << 0) /* 0 = Disables the connection of USBp to the GP_ADC 1 = Enables the connection of  */

/* CHG_DET_DCD_TIMER Register */

#define CHG_DET_DCD_TIMER_MASK                    (0x3FF << 16) /* This bit-field returns the current value of the DCD time-out timer, also ticking */
#define CHG_DET_DCD_TIMER_POS                     (16) /* This bit-field returns the current value of the DCD time-out timer, also ticking */
#define CHG_DET_DCD_TIMEOUT_THRES_MASK            (0x3FF << 0) /* This bit-field determines the value from which the Data Contact Detection (DCD)  */
#define CHG_DET_DCD_TIMEOUT_THRES_POS             (0) /* This bit-field determines the value from which the Data Contact Detection (DCD)  */

/* CHG_DET_FSM_CTRL Register */

#define CHG_DET_CHG_DET_EN                        (1U << 0) /* 0 = Charge detection FSM is disabled, analog part is controlled through the USB_ */

/* CHG_DET_FSM_STATUS Register */

#define CHG_DET_CHG_DET_STATE_MASK                (0xF << 9) /* This bit-field returns the current state of the charge detection block's FSM. Th */
#define CHG_DET_CHG_DET_STATE_POS                 (9) /* This bit-field returns the current state of the charge detection block's FSM. Th */
#define CHG_DET_NO_CONTACT_DETECTED               (1U << 8) /* 0 = Contact is sensed on the Dp/Dm (data) pins by the FSM, during the Data Conta */
#define CHG_DET_PORT_2P4AMP_DETECTED              (1U << 7) /* 0 = Port detection either not yet finished or finished without detecting a 2.4 A */
#define CHG_DET_PORT_2AMP_DETECTED                (1U << 6) /* 0 = Port detection either not yet finished or finished without detecting a 2 Amp */
#define CHG_DET_PORT_1AMP_DETECTED                (1U << 5) /* 0 = Port detection either not yet finished or finished without detecting a 1 Amp */
#define CHG_DET_PS2_PROP_PORT_DETECTED            (1U << 4) /* 0 = Port detection either not yet finished or finished without detecting a PS2 o */
#define CHG_DET_DCP_PORT_DETECTED                 (1U << 3) /* 0 = Port detection either not yet finished or finished without detecting a DCP p */
#define CHG_DET_CDP_PORT_DETECTED                 (1U << 2) /* 0 = Port detection either not yet finished or finished without detecting a CDP p */
#define CHG_DET_SDP_PORT_DETECTED                 (1U << 1) /* 0 = Port detection either not yet finished or finished without detecting an SDP  */
#define CHG_DET_DETECTION_COMPLETED               (1U << 0) /* 0 = Port detection not yet finished, FSM still active 1 = Port detection complet */

/* CHG_DET_IRQ_CLEAR Register */

#define CHG_DET_CHG_DET_IRQ_CLR                   (1U << 0) /* Writing any value to this register clears the charge detection IRQ, reading alwa */

/* CHG_DET_IRQ_MASK Register */

#define CHG_DET_CHG_DET_IRQ_EN                    (1U << 0) /* 0 = Charge detection block's IRQ generation is disabled, interrupts to Cortex-M3 */

/* CHG_DET_IRQ_STATUS Register */

#define CHG_DET_CHG_DET_IRQ                       (1U << 0) /* 0 = No new charge detection IRQ has been generated. 1 = A new charge detection I */

/* CHG_DET_STATUS Register */

#define CHG_DET_USB_DM_VAL2                       (1U << 5) /* 0: USBp < 2.3V 1: USBp > 2.5V */
#define CHG_DET_USB_DP_VAL2                       (1U << 4) /* 0: USBp < 2.3V 1: USBp > 2.5V */
#define CHG_DET_USB_DM_VAL                        (1U << 3) /* 0 = USBm < 0.8V 1 = USBm > 1.5V (PS2 or Proprietary Charger) */
#define CHG_DET_USB_DP_VAL                        (1U << 2) /* 0 = USBp < 0.8V 1 = USBp > 1.5V (PS2 or Proprietary Charger) */
#define CHG_DET_USB_CHG_DET                       (1U << 1) /* 0 = Standard Downstream Port (SDP) or no Dp/Dm contact detected (nothing connect */
#define CHG_DET_USB_DCP_DET                       (1U << 0) /* 0 = Charging downstream port is detected 1 = Dedicated charger is detected It is */

/* CHG_DET_SW_CTRL Register */

#define CHG_DET_IDM_SINK_ON                       (1U << 5) /* 0 = Disables the Idm_sink to USBm 1 = Enables the Idm_sink to USBm */
#define CHG_DET_IDP_SINK_ON                       (1U << 4) /* 0 = Disables the Idp_sink to USBp 1 = Enables the Idp_sink to USBp */
#define CHG_DET_VDM_SRC_ON                        (1U << 3) /* 0 = Disables the Vdm_src 1 = Enables the Vdm to USBm and also the USB_DCP_DET st */
#define CHG_DET_VDP_SRC_ON                        (1U << 2) /* 0 = Disables the Vdp_src 1 = Enables the Vdp_src and also the USB_CHG_DET status */
#define CHG_DET_IDP_SRC_ON                        (1U << 1) /* 0 = Disables the Idp_src and the Rdm_dwn 1 = Enables the Idp_src and the Rdm_dwn */
#define CHG_DET_USB_CHARGE_ON                     (1U << 0) /* 0 = Disables the charge detection analog circuit 1 = Enables the charge detectio */

/* CHG_DET_TIMER Register */

#define CHG_DET_CHG_DET_TIMER_MASK                (0xFF << 16) /* This bit-field returns the current value of the charge detection timer, which is */
#define CHG_DET_CHG_DET_TIMER_POS                 (16) /* This bit-field returns the current value of the charge detection timer, which is */
#define CHG_DET_CHG_DET_TIMER_THRES_MASK          (0xFF << 0) /* This bit-field determines the value from which the charge detection timer starts */
#define CHG_DET_CHG_DET_TIMER_THRES_POS           (0) /* This bit-field determines the value from which the charge detection timer starts */

#endif /* __DA1470_CHG_DET_H */
