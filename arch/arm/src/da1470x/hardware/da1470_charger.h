/****************************************************************************
 * ./tmp/da1470_charger.h
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

#ifndef __DA1470_CHARGER_H
#define __DA1470_CHARGER_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register offsets *********************************************************/

#define DA1470_CHARGER_CHARGER_CC_CHARGE_TIMER_OFFSET     0x0028 /* Maximum CC-charge time limit register */
#define DA1470_CHARGER_CHARGER_CTRL_OFFSET                0x0000 /* Charger main control register */
#define DA1470_CHARGER_CHARGER_CURRENT_PARAM_OFFSET       0x0010 /* Charger current settings register */
#define DA1470_CHARGER_CHARGER_CURRENT_STATUS_OFFSET      0x0018 /* Charger current status register */
#define DA1470_CHARGER_CHARGER_CV_CHARGE_TIMER_OFFSET     0x002C /* Maximum CV-charge time limit register */
#define DA1470_CHARGER_CHARGER_ERROR_IRQ_CLR_OFFSET       0x007C /* Interrupt clear register of Charger Error IRQs */
#define DA1470_CHARGER_CHARGER_ERROR_IRQ_MASK_OFFSET      0x006C /* Mask register of Charger Error IRQs */
#define DA1470_CHARGER_CHARGER_ERROR_IRQ_STATUS_OFFSET    0x0074 /* Status register of Charger Error IRQs */
#define DA1470_CHARGER_CHARGER_JEITA_CURRENT2_OFFSET      0x0048 /* JEITA-compliant current settings register for "Cooler" and "Warmer" zones */
#define DA1470_CHARGER_CHARGER_JEITA_CURRENT_OFFSET       0x0044 /* JEITA-compliant current settings register */
#define DA1470_CHARGER_CHARGER_JEITA_V_CHARGE_OFFSET      0x0034 /* JEITA-compliant Charge voltage settings register */
#define DA1470_CHARGER_CHARGER_JEITA_V_OVP_OFFSET         0x0040 /* JEITA-compliant OVP settings register */
#define DA1470_CHARGER_CHARGER_JEITA_V_PRECHARGE_OFFSET   0x0038 /* JEITA-compliant Pre-Charge voltage settings register */
#define DA1470_CHARGER_CHARGER_JEITA_V_REPLENISH_OFFSET   0x003C /* JEITA-compliant Replenish settings register */
#define DA1470_CHARGER_CHARGER_LOCK_OFFSET                0x0080 /* Charger HW lock register */
#define DA1470_CHARGER_CHARGER_PRE_CHARGE_TIMER_OFFSET    0x0024 /* Maximum pre-charge time limit register */
#define DA1470_CHARGER_CHARGER_PWR_UP_TIMER_OFFSET        0x0064 /* Charger power-up settling timer register */
#define DA1470_CHARGER_CHARGER_STATE_IRQ_CLR_OFFSET       0x0078 /* Interrupt clear register of Charger FSM IRQs */
#define DA1470_CHARGER_CHARGER_STATE_IRQ_MASK_OFFSET      0x0068 /* Mask register of Charger FSM IRQs */
#define DA1470_CHARGER_CHARGER_STATE_IRQ_STATUS_OFFSET    0x0070 /* Status register of Charger FSM IRQs */
#define DA1470_CHARGER_CHARGER_STATUS_OFFSET              0x0008 /* Charger main status register */
#define DA1470_CHARGER_CHARGER_SWLOCK_OFFSET              0x0084 /* Charger SW lock register */
#define DA1470_CHARGER_CHARGER_TBAT_COMP_TIMER_OFFSET     0x005C /* Battery temperature (main) comparator timer register */
#define DA1470_CHARGER_CHARGER_TBAT_MON_TIMER_OFFSET      0x0058 /* Battery temperature monitor interval timer register */
#define DA1470_CHARGER_CHARGER_TDIE_COMP_TIMER_OFFSET     0x0054 /* Die temperature comparator timer register */
#define DA1470_CHARGER_CHARGER_TEMPSET2_PARAM_OFFSET      0x0020 /* Charger battery temperature settings register (Cooler, Warmer). */
#define DA1470_CHARGER_CHARGER_TEMPSET_PARAM_OFFSET       0x001C /* Charger battery temperature settings register */
#define DA1470_CHARGER_CHARGER_THOT_COMP_TIMER_OFFSET     0x0060 /* Battery temperature comparator timer register for "Hot" zone */
#define DA1470_CHARGER_CHARGER_TOTAL_CHARGE_TIMER_OFFSET  0x0030 /* Maximum total charge time limit register */
#define DA1470_CHARGER_CHARGER_VBAT_COMP_TIMER_OFFSET     0x004C /* Main Vbat comparator timer register */
#define DA1470_CHARGER_CHARGER_VOLTAGE_PARAM_OFFSET       0x000C /* Charger voltage settings register */
#define DA1470_CHARGER_CHARGER_VOLTAGE_STATUS_OFFSET      0x0014 /* Charger voltage status register */
#define DA1470_CHARGER_CHARGER_VOVP_COMP_TIMER_OFFSET     0x0050 /* Vbat OVP comparator timer register */

/* Register addresses *******************************************************/

#define DA1470_CHARGER_BASE                               0x51000600 /* CHARGER registers */

#define DA1470_CHARGER_CHARGER_CC_CHARGE_TIMER            (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_CC_CHARGE_TIMER_OFFSET) /* Maximum CC-charge time limit register */
#define DA1470_CHARGER_CHARGER_CTRL                       (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_CTRL_OFFSET) /* Charger main control register */
#define DA1470_CHARGER_CHARGER_CURRENT_PARAM              (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_CURRENT_PARAM_OFFSET) /* Charger current settings register */
#define DA1470_CHARGER_CHARGER_CURRENT_STATUS             (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_CURRENT_STATUS_OFFSET) /* Charger current status register */
#define DA1470_CHARGER_CHARGER_CV_CHARGE_TIMER            (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_CV_CHARGE_TIMER_OFFSET) /* Maximum CV-charge time limit register */
#define DA1470_CHARGER_CHARGER_ERROR_IRQ_CLR              (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_ERROR_IRQ_CLR_OFFSET) /* Interrupt clear register of Charger Error IRQs */
#define DA1470_CHARGER_CHARGER_ERROR_IRQ_MASK             (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_ERROR_IRQ_MASK_OFFSET) /* Mask register of Charger Error IRQs */
#define DA1470_CHARGER_CHARGER_ERROR_IRQ_STATUS           (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_ERROR_IRQ_STATUS_OFFSET) /* Status register of Charger Error IRQs */
#define DA1470_CHARGER_CHARGER_JEITA_CURRENT2             (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_JEITA_CURRENT2_OFFSET) /* JEITA-compliant current settings register for "Cooler" and "Warmer" zones */
#define DA1470_CHARGER_CHARGER_JEITA_CURRENT              (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_JEITA_CURRENT_OFFSET) /* JEITA-compliant current settings register */
#define DA1470_CHARGER_CHARGER_JEITA_V_CHARGE             (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_JEITA_V_CHARGE_OFFSET) /* JEITA-compliant Charge voltage settings register */
#define DA1470_CHARGER_CHARGER_JEITA_V_OVP                (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_JEITA_V_OVP_OFFSET) /* JEITA-compliant OVP settings register */
#define DA1470_CHARGER_CHARGER_JEITA_V_PRECHARGE          (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_JEITA_V_PRECHARGE_OFFSET) /* JEITA-compliant Pre-Charge voltage settings register */
#define DA1470_CHARGER_CHARGER_JEITA_V_REPLENISH          (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_JEITA_V_REPLENISH_OFFSET) /* JEITA-compliant Replenish settings register */
#define DA1470_CHARGER_CHARGER_LOCK                       (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_LOCK_OFFSET) /* Charger HW lock register */
#define DA1470_CHARGER_CHARGER_PRE_CHARGE_TIMER           (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_PRE_CHARGE_TIMER_OFFSET) /* Maximum pre-charge time limit register */
#define DA1470_CHARGER_CHARGER_PWR_UP_TIMER               (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_PWR_UP_TIMER_OFFSET) /* Charger power-up settling timer register */
#define DA1470_CHARGER_CHARGER_STATE_IRQ_CLR              (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_STATE_IRQ_CLR_OFFSET) /* Interrupt clear register of Charger FSM IRQs */
#define DA1470_CHARGER_CHARGER_STATE_IRQ_MASK             (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_STATE_IRQ_MASK_OFFSET) /* Mask register of Charger FSM IRQs */
#define DA1470_CHARGER_CHARGER_STATE_IRQ_STATUS           (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_STATE_IRQ_STATUS_OFFSET) /* Status register of Charger FSM IRQs */
#define DA1470_CHARGER_CHARGER_STATUS                     (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_STATUS_OFFSET) /* Charger main status register */
#define DA1470_CHARGER_CHARGER_SWLOCK                     (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_SWLOCK_OFFSET) /* Charger SW lock register */
#define DA1470_CHARGER_CHARGER_TBAT_COMP_TIMER            (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_TBAT_COMP_TIMER_OFFSET) /* Battery temperature (main) comparator timer register */
#define DA1470_CHARGER_CHARGER_TBAT_MON_TIMER             (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_TBAT_MON_TIMER_OFFSET) /* Battery temperature monitor interval timer register */
#define DA1470_CHARGER_CHARGER_TDIE_COMP_TIMER            (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_TDIE_COMP_TIMER_OFFSET) /* Die temperature comparator timer register */
#define DA1470_CHARGER_CHARGER_TEMPSET2_PARAM             (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_TEMPSET2_PARAM_OFFSET) /* Charger battery temperature settings register (Cooler, Warmer). */
#define DA1470_CHARGER_CHARGER_TEMPSET_PARAM              (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_TEMPSET_PARAM_OFFSET) /* Charger battery temperature settings register */
#define DA1470_CHARGER_CHARGER_THOT_COMP_TIMER            (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_THOT_COMP_TIMER_OFFSET) /* Battery temperature comparator timer register for "Hot" zone */
#define DA1470_CHARGER_CHARGER_TOTAL_CHARGE_TIMER         (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_TOTAL_CHARGE_TIMER_OFFSET) /* Maximum total charge time limit register */
#define DA1470_CHARGER_CHARGER_VBAT_COMP_TIMER            (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_VBAT_COMP_TIMER_OFFSET) /* Main Vbat comparator timer register */
#define DA1470_CHARGER_CHARGER_VOLTAGE_PARAM              (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_VOLTAGE_PARAM_OFFSET) /* Charger voltage settings register */
#define DA1470_CHARGER_CHARGER_VOLTAGE_STATUS             (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_VOLTAGE_STATUS_OFFSET) /* Charger voltage status register */
#define DA1470_CHARGER_CHARGER_VOVP_COMP_TIMER            (DA1470_CHARGER_BASE + DA1470_CHARGER_CHARGER_VOVP_COMP_TIMER_OFFSET) /* Vbat OVP comparator timer register */

/* Register bit definitions *************************************************/


/* CHARGER_CC_CHARGE_TIMER Register */

#define CHARGER_CC_CHARGE_TIMER_MASK                      (0x7FFF << 16) /* Returns the current value of the CC-Charge timeout counter, running at a 1Hz clo */
#define CHARGER_CC_CHARGE_TIMER_POS                       (16) /* Returns the current value of the CC-Charge timeout counter, running at a 1Hz clo */
#define CHARGER_MAX_CC_CHARGE_TIME_MASK                   (0x7FFF << 0) /* This bit-field determines the maximum time (measured in ticks of the Charger's 1 */
#define CHARGER_MAX_CC_CHARGE_TIME_POS                    (0) /* This bit-field determines the maximum time (measured in ticks of the Charger's 1 */

/* CHARGER_CTRL Register */

#define CHARGER_EOC_TRIGGER                               (1U << 28) /* 0 = Edge-sensitive End-of-Charge detection support is enabled (default). The Cha */
#define CHARGER_EOC_INTERVAL_CHECK_TIMER_MASK             (0x3F << 22) /* The specific bit-field determines the current state of the timer used to periodi */
#define CHARGER_EOC_INTERVAL_CHECK_TIMER_POS              (22) /* The specific bit-field determines the current state of the timer used to periodi */
#define CHARGER_EOC_INTERVAL_CHECK_THRES_MASK             (0x3F << 16) /* This bit-field determines the periodic interval of checking the End-of-Charge si */
#define CHARGER_EOC_INTERVAL_CHECK_THRES_POS              (16) /* This bit-field determines the periodic interval of checking the End-of-Charge si */
#define CHARGER_REPLENISH_MODE                            (1U << 15) /* When this bit-field is set and the Charger's FSM is in the BYPASSED state (thus, */
#define CHARGER_PRE_CHARGE_MODE                           (1U << 14) /* When set, this bit-field enables a signal of the same name with the bit-field, d */
#define CHARGER_CHARGE_LOOP_HOLD                          (1U << 13) /* When set, this bit-field disables charging, provided that the Charger's FSM has  */
#define CHARGER_JEITA_SUPPORT_DISABLED                    (1U << 12) /* 0 = Charger's JEITA FSM monitoring the battery temperature checks also if batter */
#define CHARGER_TBAT_MONITOR_MODE_MASK                    (0x3 << 10) /* Battery temperature pack monitoring modes, according to the following encoding:  */
#define CHARGER_TBAT_MONITOR_MODE_POS                     (10) /* Battery temperature pack monitoring modes, according to the following encoding:  */
#define CHARGER_CHARGE_TIMERS_HALT_ENABLE                 (1U << 9) /* 0 = Charge timeout timers continue running when charging is disabled because of  */
#define CHARGER_STOP_CHARGE_TIMERS                        (1U << 8) /* 0 = Charger's Pre-Charge, CC, CV and Total charge timeout timers are ticking pro */
#define CHARGER_NTC_LOW_DISABLE                           (1U << 7) /* 0 = Charging is disabled when the battery temperature is found to have reached t */
#define CHARGER_TBAT_PROT_ENABLE                          (1U << 6) /* 0 = Battery temperature protection disabled 1 = Battery temperature protection e */
#define CHARGER_TDIE_ERROR_RESUME                         (1U << 5) /* 0 = FSM will not resume from a Die temperature error. Consequently, its state wi */
#define CHARGER_TDIE_PROT_ENABLE                          (1U << 4) /* 0 = Die temperature protection is disabled, thus charging will not be disabled b */
#define CHARGER_CHARGER_RESUME                            (1U << 3) /* 0 = Charger's FSM is not enable to resume from a charge timeout error or a Vbat  */
#define CHARGER_CHARGER_BYPASS                            (1U << 2) /* 0 = Charger's FSM is active and running, notifying SW upon switching between its */
#define CHARGER_CHARGE_START                              (1U << 1) /* 0 = Charger's FSM is disabled, FSM stays at "DISABLED" state 1 = Charger's FSM i */
#define CHARGER_CHARGER_ENABLE                            (1U << 0) /* 0 = Charger's analogue circuitry is powered-down 1 = Charger's analogue circuitr */

/* CHARGER_CURRENT_PARAM Register */

#define CHARGER_I_EOC_DOUBLE_RANGE                        (1U << 15) /* When set, the specific bit-field enables an increase of the (%) range of End-of- */
#define CHARGER_I_END_OF_CHARGE_MASK                      (0x7 << 12) /* End-of-Charge current setting, ranging from 6%("000") to 20% ("111") of the char */
#define CHARGER_I_END_OF_CHARGE_POS                       (12) /* End-of-Charge current setting, ranging from 6%("000") to 20% ("111") of the char */
#define CHARGER_I_PRECHARGE_MASK                          (0x3F << 6) /* This bit-field determines the Pre-Charge current, in mA, ranging from 0.5 to 72m */
#define CHARGER_I_PRECHARGE_POS                           (6) /* This bit-field determines the Pre-Charge current, in mA, ranging from 0.5 to 72m */
#define CHARGER_I_CHARGE_MASK                             (0x3F << 0) /* This bit-field determines the charge current range, in mA. The range is from 5mA */
#define CHARGER_I_CHARGE_POS                              (0) /* This bit-field determines the charge current range, in mA. The range is from 5mA */

/* CHARGER_CURRENT_STATUS Register */

#define CHARGER_I_PRECHARGE_SET_MASK                      (0x3F << 6) /* This bit-field returns the applied Pre-Charge current setting. In JEITA charging */
#define CHARGER_I_PRECHARGE_SET_POS                       (6) /* This bit-field returns the applied Pre-Charge current setting. In JEITA charging */
#define CHARGER_I_CHARGE_SET_MASK                         (0x3F << 0) /* This bit-field returns the applied CC current setting. In JEITA charging, it con */
#define CHARGER_I_CHARGE_SET_POS                          (0) /* This bit-field returns the applied CC current setting. In JEITA charging, it con */

/* CHARGER_CV_CHARGE_TIMER Register */

#define CHARGER_CV_CHARGE_TIMER_MASK                      (0x7FFF << 16) /* Returns the current value of the CV-Charge timeout counter, running at a 1Hz clo */
#define CHARGER_CV_CHARGE_TIMER_POS                       (16) /* Returns the current value of the CV-Charge timeout counter, running at a 1Hz clo */
#define CHARGER_MAX_CV_CHARGE_TIME_MASK                   (0x7FFF << 0) /* This bit-field determines the maximum time (measured in ticks of the Charger's 1 */
#define CHARGER_MAX_CV_CHARGE_TIME_POS                    (0) /* This bit-field determines the maximum time (measured in ticks of the Charger's 1 */

/* CHARGER_ERROR_IRQ_CLR Register */

#define CHARGER_TBAT_ERROR_IRQ_CLR                        (1U << 6) /* Writing a 1 will reset the respective Charger's Error IRQ status bit ; writing a */
#define CHARGER_TDIE_ERROR_IRQ_CLR                        (1U << 5) /* Writing a 1 will reset the respective Charger's Error IRQ status bit ; writing a */
#define CHARGER_VBAT_OVP_ERROR_IRQ_CLR                    (1U << 4) /* Writing a 1 will reset the respective Charger's Error IRQ status bit ; writing a */
#define CHARGER_TOTAL_CHARGE_TIMEOUT_IRQ_CLR              (1U << 3) /* Writing a 1 will reset the respective Charger's Error IRQ status bit ; writing a */
#define CHARGER_CV_CHARGE_TIMEOUT_IRQ_CLR                 (1U << 2) /* Writing a 1 will reset the respective Charger's Error IRQ status bit ; writing a */
#define CHARGER_CC_CHARGE_TIMEOUT_IRQ_CLR                 (1U << 1) /* Writing a 1 will reset the respective Charger's Error IRQ status bit ; writing a */
#define CHARGER_PRECHARGE_TIMEOUT_IRQ_CLR                 (1U << 0) /* Writing a 1 will reset the respective Charger's Error IRQ status bit ; writing a */

/* CHARGER_ERROR_IRQ_MASK Register */

#define CHARGER_TBAT_ERROR_IRQ_EN                         (1U << 6) /* When set, it enables the generation of Battery temperature IRQs.The IRQ is gener */
#define CHARGER_TDIE_ERROR_IRQ_EN                         (1U << 5) /* When set, it enables the generation of Die temperature error IRQs. The IRQ is ge */
#define CHARGER_VBAT_OVP_ERROR_IRQ_EN                     (1U << 4) /* When set, it enables the generation of VBAT_OVP IRQs. The IRQ is generated as so */
#define CHARGER_TOTAL_CHARGE_TIMEOUT_IRQ_EN               (1U << 3) /* When set, it enables the total charge timeout IRQs. The IRQ is generated as soon */
#define CHARGER_CV_CHARGE_TIMEOUT_IRQ_EN                  (1U << 2) /* When set, it enables the CV charge timeout IRQs. The IRQ is generated as soon as */
#define CHARGER_CC_CHARGE_TIMEOUT_IRQ_EN                  (1U << 1) /* When set, it enables the CC charge timeout IRQs. The IRQ is generated as soon as */
#define CHARGER_PRECHARGE_TIMEOUT_IRQ_EN                  (1U << 0) /* When set, it enables the Pre-Charge timeout IRQs. The IRQ is generated as soon a */

/* CHARGER_ERROR_IRQ_STATUS Register */

#define CHARGER_TBAT_ERROR_IRQ                            (1U << 6) /* 0 = No Battery temperature error IRQ event is captured, so charging may continue */
#define CHARGER_TDIE_ERROR_IRQ                            (1U << 5) /* 0 = No Die temperature error IRQ events have been captured, so charging may cont */
#define CHARGER_VBAT_OVP_ERROR_IRQ                        (1U << 4) /* 0 = Vbat has not exceeded the Over-Voltage Protection (OVP) level, so charging m */
#define CHARGER_TOTAL_CHARGE_TIMEOUT_IRQ                  (1U << 3) /* 0 = Total charge time counter has not yet reached the maximum charge time (set i */
#define CHARGER_CV_CHARGE_TIMEOUT_IRQ                     (1U << 2) /* 0 = State charge time counter has not yet reached the maximum CV charge time (se */
#define CHARGER_CC_CHARGE_TIMEOUT_IRQ                     (1U << 1) /* 0 = State charge time counter has not yet reached the maximum CC charge time (se */
#define CHARGER_PRECHARGE_TIMEOUT_IRQ                     (1U << 0) /* 0 = State charge time counter has not yet reached the maximum Pre-charge time (s */

/* CHARGER_JEITA_CURRENT2 Register */

#define CHARGER_I_PRECHARGE_TWARMER_MASK                  (0x3F << 18) /* Pre-charge current settings bit-field for the "WARMER" battery temperature pack  */
#define CHARGER_I_PRECHARGE_TWARMER_POS                   (18) /* Pre-charge current settings bit-field for the "WARMER" battery temperature pack  */
#define CHARGER_I_PRECHARGE_TCOOLER_MASK                  (0x3F << 12) /* Pre-charge current settings bit-field for the "COOLER" battery temperature pack  */
#define CHARGER_I_PRECHARGE_TCOOLER_POS                   (12) /* Pre-charge current settings bit-field for the "COOLER" battery temperature pack  */
#define CHARGER_I_CHARGE_TWARMER_MASK                     (0x3F << 6) /* Charge current settings bit-field for the "WARMER" battery temperature pack zone */
#define CHARGER_I_CHARGE_TWARMER_POS                      (6) /* Charge current settings bit-field for the "WARMER" battery temperature pack zone */
#define CHARGER_I_CHARGE_TCOOLER_MASK                     (0x3F << 0) /* Charge current settings bit-field for the "COOLER" battery temperature pack zone */
#define CHARGER_I_CHARGE_TCOOLER_POS                      (0) /* Charge current settings bit-field for the "COOLER" battery temperature pack zone */

/* CHARGER_JEITA_CURRENT Register */

#define CHARGER_I_PRECHARGE_TWARM_MASK                    (0x3F << 18) /* Pre-Charge current setting for the Warm battery temperature zone. Regarding the  */
#define CHARGER_I_PRECHARGE_TWARM_POS                     (18) /* Pre-Charge current setting for the Warm battery temperature zone. Regarding the  */
#define CHARGER_I_PRECHARGE_TCOOL_MASK                    (0x3F << 12) /* Pre-Charge current setting for the Cool battery temperature zone. Regarding the  */
#define CHARGER_I_PRECHARGE_TCOOL_POS                     (12) /* Pre-Charge current setting for the Cool battery temperature zone. Regarding the  */
#define CHARGER_I_CHARGE_TWARM_MASK                       (0x3F << 6) /* Charge current setting for the Warm battery temperature pack zone. Regarding the */
#define CHARGER_I_CHARGE_TWARM_POS                        (6) /* Charge current setting for the Warm battery temperature pack zone. Regarding the */
#define CHARGER_I_CHARGE_TCOOL_MASK                       (0x3F << 0) /* Charge current setting for the "COOL" battery temperature level. Regarding the r */
#define CHARGER_I_CHARGE_TCOOL_POS                        (0) /* Charge current setting for the "COOL" battery temperature level. Regarding the r */

/* CHARGER_JEITA_V_CHARGE Register */

#define CHARGER_V_CHARGE_TWARMER_MASK                     (0x3F << 18) /* Charge voltage setting for the Warmer battery temperature zone. Regarding the ra */
#define CHARGER_V_CHARGE_TWARMER_POS                      (18) /* Charge voltage setting for the Warmer battery temperature zone. Regarding the ra */
#define CHARGER_V_CHARGE_TCOOLER_MASK                     (0x3F << 12) /* Charge voltage setting for the Cooler battery temperature zone. Regarding the ra */
#define CHARGER_V_CHARGE_TCOOLER_POS                      (12) /* Charge voltage setting for the Cooler battery temperature zone. Regarding the ra */
#define CHARGER_V_CHARGE_TWARM_MASK                       (0x3F << 6) /* Charge voltage setting for the Warm battery temperature zone. Regarding the rang */
#define CHARGER_V_CHARGE_TWARM_POS                        (6) /* Charge voltage setting for the Warm battery temperature zone. Regarding the rang */
#define CHARGER_V_CHARGE_TCOOL_MASK                       (0x3F << 0) /* Charge voltage setting for the Cool battery temperature zone. Regarding the rang */
#define CHARGER_V_CHARGE_TCOOL_POS                        (0) /* Charge voltage setting for the Cool battery temperature zone. Regarding the rang */

/* CHARGER_JEITA_V_OVP Register */

#define CHARGER_V_OVP_TWARMER_MASK                        (0x3F << 18) /* VBAT Over-voltage Protection (OVP) setting for the Warmer battery temperature zo */
#define CHARGER_V_OVP_TWARMER_POS                         (18) /* VBAT Over-voltage Protection (OVP) setting for the Warmer battery temperature zo */
#define CHARGER_V_OVP_TCOOLER_MASK                        (0x3F << 12) /* VBAT Over-voltage Protection (OVP) setting for the Cooler battery temperature zo */
#define CHARGER_V_OVP_TCOOLER_POS                         (12) /* VBAT Over-voltage Protection (OVP) setting for the Cooler battery temperature zo */
#define CHARGER_V_OVP_TWARM_MASK                          (0x3F << 6) /* VBAT Over-voltage Protection (OVP) setting for the Warm battery temperature zone */
#define CHARGER_V_OVP_TWARM_POS                           (6) /* VBAT Over-voltage Protection (OVP) setting for the Warm battery temperature zone */
#define CHARGER_V_OVP_TCOOL_MASK                          (0x3F << 0) /* VBAT Over-voltage Protection (OVP) setting for the Cool battery temperature zone */
#define CHARGER_V_OVP_TCOOL_POS                           (0) /* VBAT Over-voltage Protection (OVP) setting for the Cool battery temperature zone */

/* CHARGER_JEITA_V_PRECHARGE Register */

#define CHARGER_V_PRECHARGE_TWARMER_MASK                  (0x3F << 18) /* Pre-Charge voltage setting for the Warmer battery temperature zone. Regarding th */
#define CHARGER_V_PRECHARGE_TWARMER_POS                   (18) /* Pre-Charge voltage setting for the Warmer battery temperature zone. Regarding th */
#define CHARGER_V_PRECHARGE_TCOOLER_MASK                  (0x3F << 12) /* Pre-Charge voltage setting for the Cooler battery temperature zone. Regarding th */
#define CHARGER_V_PRECHARGE_TCOOLER_POS                   (12) /* Pre-Charge voltage setting for the Cooler battery temperature zone. Regarding th */
#define CHARGER_V_PRECHARGE_TWARM_MASK                    (0x3F << 6) /* Pre-Charge voltage setting for the Warm battery temperature zone. Regarding the  */
#define CHARGER_V_PRECHARGE_TWARM_POS                     (6) /* Pre-Charge voltage setting for the Warm battery temperature zone. Regarding the  */
#define CHARGER_V_PRECHARGE_TCOOL_MASK                    (0x3F << 0) /* Pre-Charge current setting for the Cool battery temperature zone. Regarding the  */
#define CHARGER_V_PRECHARGE_TCOOL_POS                     (0) /* Pre-Charge current setting for the Cool battery temperature zone. Regarding the  */

/* CHARGER_JEITA_V_REPLENISH Register */

#define CHARGER_V_REPLENISH_TWARMER_MASK                  (0x3F << 18) /* Replenish voltage setting for the Warmer battery temperature zone. Regarding the */
#define CHARGER_V_REPLENISH_TWARMER_POS                   (18) /* Replenish voltage setting for the Warmer battery temperature zone. Regarding the */
#define CHARGER_V_REPLENISH_TCOOLER_MASK                  (0x3F << 12) /* Replenish voltage setting for the Cooler battery temperature zone. Regarding the */
#define CHARGER_V_REPLENISH_TCOOLER_POS                   (12) /* Replenish voltage setting for the Cooler battery temperature zone. Regarding the */
#define CHARGER_V_REPLENISH_TWARM_MASK                    (0x3F << 6) /* Replenish voltage setting for the Warm battery temperature zone. Regarding the r */
#define CHARGER_V_REPLENISH_TWARM_POS                     (6) /* Replenish voltage setting for the Warm battery temperature zone. Regarding the r */
#define CHARGER_V_REPLENISH_TCOOL_MASK                    (0x3F << 0) /* Replenish voltage setting for the Cool battery temperature zone. Regarding the r */
#define CHARGER_V_REPLENISH_TCOOL_POS                     (0) /* Replenish voltage setting for the Cool battery temperature zone. Regarding the r */

/* CHARGER_LOCK Register */

#define CHARGER_CHARGER_SWLOCK_EN                         (1U << 15) /* 0 = SW-based protection of Charger registers is disabled, protection is dependen */
#define CHARGER_JEITA_I_CHARGE2_LOCK                      (1U << 14) /* 0 = CHARGER_JEITA_CURRENT2_REG is un-locked, register writes are allowed 1 = CHA */
#define CHARGER_JEITA_I_CHARGE_LOCK                       (1U << 13) /* 0 = CHARGER_JEITA_CURRENT_REG is un-locked, register writes are allowed 1 = CHAR */
#define CHARGER_JEITA_V_OVP_LOCK                          (1U << 12) /* 0 = CHARGER_JEITA_V_OVP_REG is un-locked, register writes are allowed 1 = CHARGE */
#define CHARGER_JEITA_V_PRECHARGE_LOCK                    (1U << 11) /* 0 = CHARGER_JEITA_V_PRECHARGE_REG is un-locked, register writes are allowed 1 =  */
#define CHARGER_JEITA_V_CHARGE_LOCK                       (1U << 10) /* 0 = CHARGER_JEITA_V_CHARGE_REG is un-locked, register writes are allowed 1 = CHA */
#define CHARGER_TOTAL_CHARGE_TIMEOUT_LOCK                 (1U << 9) /* 0 = CHARGER_TOTAL_CHARGE_TIMER_REG is un-locked, register writes are allowed 1 = */
#define CHARGER_CV_CHARGE_TIMEOUT_LOCK                    (1U << 8) /* 0 = CHARGER_CV_CHARGE_TIMER_REG is un-locked, register writes are allowed 1 = CH */
#define CHARGER_CC_CHARGE_TIMEOUT_LOCK                    (1U << 7) /* 0 = CHARGER_CC_CHARGE_TIMER_REG is un-locked, register writes are allowed 1 = CH */
#define CHARGER_PRECHARGE_TIMEOUT_LOCK                    (1U << 6) /* 0 = CHARGER_PRE_CHARGE_TIMER_REG is un-locked, register writes are allowed 1 = C */
#define CHARGER_TEMPSET2_PARAM_LOCK                       (1U << 5) /* 0 = CHARGER_TEMPSET2_PARAM_REG is un-locked, register writes are allowed 1 = CHA */
#define CHARGER_TEMPSET_PARAM_LOCK                        (1U << 4) /* 0 = CHARGER_TEMPSET_PARAM_REG is un-locked, register writes are allowed 1 = CHAR */
#define CHARGER_CURRENT_PARAM_LOCK                        (1U << 3) /* 0 = CHARGER_CURRENT_PARAM_REG is un-locked, writes are allowed 1 = All bit-field */
#define CHARGER_VOLTAGE_PARAM_LOCK                        (1U << 2) /* 0 = CHARGER_VOLTAGE_PARAM_REG is un-locked, register writes are allowed 1 = All  */
#define CHARGER_CHARGER_TEST_CTRL_LOCK                    (1U << 1) /* 0 = CHARGER_TEST_CTRL_REG is un-locked, register writes are allowed 1 = CHARGER_ */
#define CHARGER_CHARGER_CTRL_LOCK                         (1U << 0) /* 0 = Protected bit-fields of CHARGER_CTRL_REG are un-locked, register writes are  */

/* CHARGER_PRE_CHARGE_TIMER Register */

#define CHARGER_PRE_CHARGE_TIMER_MASK                     (0x7FFF << 16) /* Returns the current value of the Pre-Charge timeout counter, running at a 1Hz cl */
#define CHARGER_PRE_CHARGE_TIMER_POS                      (16) /* Returns the current value of the Pre-Charge timeout counter, running at a 1Hz cl */
#define CHARGER_MAX_PRE_CHARGE_TIME_MASK                  (0x7FFF << 0) /* This bit-field determines the maximum time (measured in ticks of the Charger's 1 */
#define CHARGER_MAX_PRE_CHARGE_TIME_POS                   (0) /* This bit-field determines the maximum time (measured in ticks of the Charger's 1 */

/* CHARGER_PWR_UP_TIMER Register */

#define CHARGER_CHARGER_PWR_UP_TIMER_MASK                 (0x1F << 16) /* Returns the current value of the charger's power-up timer, running with the 1Khz */
#define CHARGER_CHARGER_PWR_UP_TIMER_POS                  (16) /* Returns the current value of the charger's power-up timer, running with the 1Khz */
#define CHARGER_CHARGER_PWR_UP_SETTLING_MASK              (0x1F << 0) /* This bit-field determines the charger's power-up (settling) time, required for t */
#define CHARGER_CHARGER_PWR_UP_SETTLING_POS               (0) /* This bit-field determines the charger's power-up (settling) time, required for t */

/* CHARGER_STATE_IRQ_CLR Register */

#define CHARGER_CV_TO_PRECHARGE_IRQ_CLR                   (1U << 11) /* Writing a 1 will reset the respective Charger's State IRQ status bit ; writing a */
#define CHARGER_CC_TO_PRECHARGE_IRQ_CLR                   (1U << 10) /* Writing a 1 will reset the respective Charger's State IRQ status bit ; writing a */
#define CHARGER_CV_TO_CC_IRQ_CLR                          (1U << 9) /* Writing a 1 will reset the respective Charger's State IRQ status bit ; writing a */
#define CHARGER_TBAT_STATUS_UPDATE_IRQ_CLR                (1U << 8) /* Writing a 1 will reset the Battery temperature status update IRQ status bit ; wr */
#define CHARGER_TBAT_PROT_TO_PRECHARGE_IRQ_CLR            (1U << 7) /* Writing a 1 will reset the respective Charger's State IRQ status bit ; writing a */
#define CHARGER_TDIE_PROT_TO_PRECHARGE_IRQ_CLR            (1U << 6) /* Writing a 1 will reset the respective Charger's State IRQ status bit ; writing a */
#define CHARGER_EOC_TO_PRECHARGE_IRQ_CLR                  (1U << 5) /* Writing a 1 will reset the respective Charger's State IRQ status bit ; writing a */
#define CHARGER_CV_TO_EOC_IRQ_CLR                         (1U << 4) /* Writing a 1 will reset the respective Charger's State IRQ status bit ; writing a */
#define CHARGER_CC_TO_EOC_IRQ_CLR                         (1U << 3) /* Writing a 1 will reset the respective Charger's State IRQ status bit ; writing a */
#define CHARGER_CC_TO_CV_IRQ_CLR                          (1U << 2) /* Writing a 1 will reset the respective Charger's State IRQ status bit ; writing a */
#define CHARGER_PRECHARGE_TO_CC_IRQ_CLR                   (1U << 1) /* Writing a 1 will reset the respective Charger's State IRQ status bit ; writing a */
#define CHARGER_DISABLED_TO_PRECHARGE_IRQ_CLR             (1U << 0) /* Writing a 1 will reset the respective Charger's State IRQ status bit ; writing a */

/* CHARGER_STATE_IRQ_MASK Register */

#define CHARGER_CV_TO_PRECHARGE_IRQ_EN                    (1U << 11) /* When set, this bit-field enables the IRQ generation as soon as the Charger's FSM */
#define CHARGER_CC_TO_PRECHARGE_IRQ_EN                    (1U << 10) /* When set, this bit-field enables the IRQ generation as soon as the Charger's FSM */
#define CHARGER_CV_TO_CC_IRQ_EN                           (1U << 9) /* When set, this bit-field enables the IRQ generation as soon as the Charger's FSM */
#define CHARGER_TBAT_STATUS_UPDATE_IRQ_EN                 (1U << 8) /* When set, this bit-field enables the generation of the Charger's state IRQ as so */
#define CHARGER_TBAT_PROT_TO_PRECHARGE_IRQ_EN             (1U << 7) /* When set, this bit-field enables the Charger's state IRQ generation as soon as t */
#define CHARGER_TDIE_PROT_TO_PRECHARGE_IRQ_EN             (1U << 6) /* When set, this bit-field enables the Charger's state IRQ generation as soon as t */
#define CHARGER_EOC_TO_PRECHARGE_IRQ_EN                   (1U << 5) /* When set, this bit-field enables the Charger's State IRQ generation as soon as t */
#define CHARGER_CV_TO_EOC_IRQ_EN                          (1U << 4) /* When set, this bit-field enables the IRQ generation as soon as the Charger's FSM */
#define CHARGER_CC_TO_EOC_IRQ_EN                          (1U << 3) /* When set, this bit-field enables the IRQ generation as soon as the Charger's FSM */
#define CHARGER_CC_TO_CV_IRQ_EN                           (1U << 2) /* When set, this bit-field enables the IRQ generation as soon as the Charger's FSM */
#define CHARGER_PRECHARGE_TO_CC_IRQ_EN                    (1U << 1) /* When set, this bit-field enables the IRQ generation as soon as the Charger's FSM */
#define CHARGER_DISABLED_TO_PRECHARGE_IRQ_EN              (1U << 0) /* When set, this bit-field enables the IRQ generation as soon as the Charger's FSM */

/* CHARGER_STATE_IRQ_STATUS Register */

#define CHARGER_CV_TO_PRECHARGE_IRQ                       (1U << 11) /* 0 = No transition of the Charger's FSM from CV_CHARGE to PRE_CHARGE state has be */
#define CHARGER_CC_TO_PRECHARGE_IRQ                       (1U << 10) /* 0 = No transition of the Charger's FSM from CC_CHARGE to PRE_CHARGE state has be */
#define CHARGER_CV_TO_CC_IRQ                              (1U << 9) /* 0 = No transition of the Charger's FSM from CV_CHARGE to CC_CHARGE state has bee */
#define CHARGER_TBAT_STATUS_UPDATE_IRQ                    (1U << 8) /* 0 = No battery temperature status update event has been captured 1 = Battery tem */
#define CHARGER_TBAT_PROT_TO_PRECHARGE_IRQ                (1U << 7) /* 0 = No transition of the Charger's FSM from TBAT_PROT to PRE_CHARGE state has be */
#define CHARGER_TDIE_PROT_TO_PRECHARGE_IRQ                (1U << 6) /* 0 = No transition of the Charger's FSM from TDIE_PROT to PRE_CHARGE state has be */
#define CHARGER_EOC_TO_PRECHARGE_IRQ                      (1U << 5) /* 0 = No transition of the Charger's FSM from END_OF_CHARGE to PRE_CHARGE state ha */
#define CHARGER_CV_TO_EOC_IRQ                             (1U << 4) /* 0 = No transition of the Charger's FSM from CV_CHARGE to END_OF_CHARGE state has */
#define CHARGER_CC_TO_EOC_IRQ                             (1U << 3) /* 0 = No transition of the Charger's FSM from CC_CHARGE to END_OF_CHARGE state has */
#define CHARGER_CC_TO_CV_IRQ                              (1U << 2) /* 0 = No transition of the Charger's FSM from CC_CHARGE to CV_CHARGE state has bee */
#define CHARGER_PRECHARGE_TO_CC_IRQ                       (1U << 1) /* 0 = No transition of the Charger's FSM from PRE_CHARGE to CC_CHARGE state has be */
#define CHARGER_DISABLED_TO_PRECHARGE_IRQ                 (1U << 0) /* 0 = No transition of the Charger's FSM from DISABLED to PRE_CHARGE state has bee */

/* CHARGER_STATUS Register */

#define CHARGER_OVP_EVENTS_DEBOUNCE_CNT_MASK              (0x7 << 26) /* The specific bit-field returns the consecutive number of times Vbat has exceeded */
#define CHARGER_OVP_EVENTS_DEBOUNCE_CNT_POS               (26) /* The specific bit-field returns the consecutive number of times Vbat has exceeded */
#define CHARGER_EOC_EVENTS_DEBOUNCE_CNT_MASK              (0x7 << 23) /* The specific bit-field returns the number of times the End-of-Charge signal has  */
#define CHARGER_EOC_EVENTS_DEBOUNCE_CNT_POS               (23) /* The specific bit-field returns the number of times the End-of-Charge signal has  */
#define CHARGER_TDIE_ERROR_DEBOUNCE_CNT_MASK              (0x7 << 20) /* The specific bit-field returns the consecutive number of times the Die temperatu */
#define CHARGER_TDIE_ERROR_DEBOUNCE_CNT_POS               (20) /* The specific bit-field returns the consecutive number of times the Die temperatu */
#define CHARGER_CHARGER_JEITA_STATE_MASK                  (0xF << 16) /* Returns the state of the Charger's JEITA FSM. This FSM is used to update the sta */
#define CHARGER_CHARGER_JEITA_STATE_POS                   (16) /* Returns the state of the Charger's JEITA FSM. This FSM is used to update the sta */
#define CHARGER_CHARGER_STATE_MASK                        (0xF << 12) /* Indicating the state of the Charger's main FSM, based on the following encoding: */
#define CHARGER_CHARGER_STATE_POS                         (12) /* Indicating the state of the Charger's main FSM, based on the following encoding: */
#define CHARGER_TBAT_STATUS_MASK                          (0x7 << 9) /* Battery pack temperature status, according to the following encoding: 0x0 : Batt */
#define CHARGER_TBAT_STATUS_POS                           (9) /* Battery pack temperature status, according to the following encoding: 0x0 : Batt */
#define CHARGER_MAIN_TBAT_COMP_OUT                        (1U << 8) /* Returns the status of the main battery temperature comparator. This comparator b */
#define CHARGER_TBAT_HOT_COMP_OUT                         (1U << 7) /* Returns the status of the battery temperature comparator dedicated to the Hot te */
#define CHARGER_TDIE_COMP_OUT                             (1U << 6) /* 0 = Die temperature is found to be below the programmed level, set in CHARGER_TE */
#define CHARGER_VBAT_OVP_COMP_OUT                         (1U << 5) /* 0 = Vbat has not exceeded the Over-Voltage Protection (OVP) voltage limit, accor */
#define CHARGER_MAIN_VBAT_COMP_OUT                        (1U << 4) /* This bit-field reflects the status of the main Vbat comparator residing in the a */
#define CHARGER_END_OF_CHARGE                             (1U << 3) /* 0 = Actual charge current is above the current level programmed in I_END_OF_CHAR */
#define CHARGER_CHARGER_CV_MODE                           (1U << 2) /* 0 = Charger's voltage loop not in regulation (or Charger is off) 1 = Charger's C */
#define CHARGER_CHARGER_CC_MODE                           (1U << 1) /* 0 = Charger's Current loop not in regulation (or Charger is off) 1 = Charger's C */
#define CHARGER_CHARGER_IS_POWERED_UP                     (1U << 0) /* 0 = Charger is either off or it is being powered-on but the analogue ciruitry is */

/* CHARGER_SWLOCK Register */

#define CHARGER_SWLOCK_FSM_STATE_MASK                     (0x3 << 1) /* Returns the state of the FSM detecting the sequence of writes that either enable */
#define CHARGER_SWLOCK_FSM_STATE_POS                      (1) /* Returns the state of the FSM detecting the sequence of writes that either enable */
#define CHARGER_SWLOCK_STATUS                             (1U << 0) /* 0 = SW locking is not active, writes to the protected Charger registers are allo */

/* CHARGER_TBAT_COMP_TIMER Register */

#define CHARGER_TBAT_COMP_TIMER_MASK                      (0x3FF << 16) /* Returns the main battery temperature comparator's timer, used for the latching o */
#define CHARGER_TBAT_COMP_TIMER_POS                       (16) /* Returns the main battery temperature comparator's timer, used for the latching o */
#define CHARGER_TBAT_COMP_SETTLING_MASK                   (0x3FF << 0) /* Settling time (specified in us) for the main battery temperature comparator, che */
#define CHARGER_TBAT_COMP_SETTLING_POS                    (0) /* Settling time (specified in us) for the main battery temperature comparator, che */

/* CHARGER_TBAT_MON_TIMER Register */

#define CHARGER_TBAT_MON_TIMER_MASK                       (0x3FF << 16) /* This is the battery temperature monitoring timer, counting with the Charger's 1K */
#define CHARGER_TBAT_MON_TIMER_POS                        (16) /* This is the battery temperature monitoring timer, counting with the Charger's 1K */
#define CHARGER_TBAT_MON_INTERVAL_MASK                    (0x3FF << 0) /* Timing interval (in ms) for the Battery temperature monitoring. This interval de */
#define CHARGER_TBAT_MON_INTERVAL_POS                     (0) /* Timing interval (in ms) for the Battery temperature monitoring. This interval de */

/* CHARGER_TDIE_COMP_TIMER Register */

#define CHARGER_TDIE_COMP_TIMER_MASK                      (0x3FF << 16) /* Returns the current value of the timer used to determine when the Die temperatur */
#define CHARGER_TDIE_COMP_TIMER_POS                       (16) /* Returns the current value of the timer used to determine when the Die temperatur */
#define CHARGER_TDIE_COMP_SETTLING_MASK                   (0x3FF << 0) /* Settling time threshold (in us) for the Die temperature comparator. */
#define CHARGER_TDIE_COMP_SETTLING_POS                    (0) /* Settling time threshold (in us) for the Die temperature comparator. */

/* CHARGER_TEMPSET2_PARAM Register */

#define CHARGER_TBAT_WARMER_MASK                          (0x3F << 18) /* This bit-field determines the battery temperature threshold above which the "WAR */
#define CHARGER_TBAT_WARMER_POS                           (18) /* This bit-field determines the battery temperature threshold above which the "WAR */
#define CHARGER_TBAT_WARM_MASK                            (0x3F << 12) /* This bit-field determines the battery temperature above which the charge current */
#define CHARGER_TBAT_WARM_POS                             (12) /* This bit-field determines the battery temperature above which the charge current */
#define CHARGER_TBAT_COOL_MASK                            (0x3F << 6) /* This bit-field determines the battery temperature below which the charge current */
#define CHARGER_TBAT_COOL_POS                             (6) /* This bit-field determines the battery temperature below which the charge current */
#define CHARGER_TBAT_COOLER_MASK                          (0x3F << 0) /* This bit-field determines the battery temperature below which the charge current */
#define CHARGER_TBAT_COOLER_POS                           (0) /* This bit-field determines the battery temperature below which the charge current */

/* CHARGER_TEMPSET_PARAM Register */

#define CHARGER_TDIE_MAX_MASK                             (0x7 << 12) /* This bit-field determines the maximum Die temperature level limit, ranging from  */
#define CHARGER_TDIE_MAX_POS                              (12) /* This bit-field determines the maximum Die temperature level limit, ranging from  */
#define CHARGER_TBAT_HOT_MASK                             (0x3F << 6) /* This bit-field determines the battery temperature above which the charge current */
#define CHARGER_TBAT_HOT_POS                              (6) /* This bit-field determines the battery temperature above which the charge current */
#define CHARGER_TBAT_COLD_MASK                            (0x3F << 0) /* This bit-field determines the battery temperature below which the charge current */
#define CHARGER_TBAT_COLD_POS                             (0) /* This bit-field determines the battery temperature below which the charge current */

/* CHARGER_THOT_COMP_TIMER Register */

#define CHARGER_THOT_COMP_TIMER_MASK                      (0x3FF << 16) /* Returns the battery temperature comparator's timer dedicated for the "Hot" level */
#define CHARGER_THOT_COMP_TIMER_POS                       (16) /* Returns the battery temperature comparator's timer dedicated for the "Hot" level */
#define CHARGER_THOT_COMP_SETTLING_MASK                   (0x3FF << 0) /* Charger's battery temperature comparator settling time (specified in us), specif */
#define CHARGER_THOT_COMP_SETTLING_POS                    (0) /* Charger's battery temperature comparator settling time (specified in us), specif */

/* CHARGER_TOTAL_CHARGE_TIMER Register */

#define CHARGER_TOTAL_CHARGE_TIMER_MASK                   (0xFFFF << 16) /* Returns the current value of the overall charge timeout counter, running at a 1H */
#define CHARGER_TOTAL_CHARGE_TIMER_POS                    (16) /* Returns the current value of the overall charge timeout counter, running at a 1H */
#define CHARGER_MAX_TOTAL_CHARGE_TIME_MASK                (0xFFFF << 0) /* This bit-field determines the maximum overall charging time allowed (measured in */
#define CHARGER_MAX_TOTAL_CHARGE_TIME_POS                 (0) /* This bit-field determines the maximum overall charging time allowed (measured in */

/* CHARGER_VBAT_COMP_TIMER Register */

#define CHARGER_VBAT_COMP_TIMER_MASK                      (0x3FF << 16) /* Returns the current value of the timer used to determine when the output of the  */
#define CHARGER_VBAT_COMP_TIMER_POS                       (16) /* Returns the current value of the timer used to determine when the output of the  */
#define CHARGER_VBAT_COMP_SETTLING_MASK                   (0x3FF << 0) /* Settling time threshold (in us) for the Vbat comparator checking Vbat vs the pro */
#define CHARGER_VBAT_COMP_SETTLING_POS                    (0) /* Settling time threshold (in us) for the Vbat comparator checking Vbat vs the pro */

/* CHARGER_VOLTAGE_PARAM Register */

#define CHARGER_V_OVP_MASK                                (0x3F << 18) /* This bit-field determines the VBAT Over-voltage protection limit. This Over-volt */
#define CHARGER_V_OVP_POS                                 (18) /* This bit-field determines the VBAT Over-voltage protection limit. This Over-volt */
#define CHARGER_V_REPLENISH_MASK                          (0x3F << 12) /* This bit-field determines the absolute value (in V) of the Replenish voltage thr */
#define CHARGER_V_REPLENISH_POS                           (12) /* This bit-field determines the absolute value (in V) of the Replenish voltage thr */
#define CHARGER_V_PRECHARGE_MASK                          (0x3F << 6) /* This bit-field determines the voltage level at which the battery is considered a */
#define CHARGER_V_PRECHARGE_POS                           (6) /* This bit-field determines the voltage level at which the battery is considered a */
#define CHARGER_V_CHARGE_MASK                             (0x3F << 0) /* This bit-field determines the charge voltage levels supported. The supported lev */
#define CHARGER_V_CHARGE_POS                              (0) /* This bit-field determines the charge voltage levels supported. The supported lev */

/* CHARGER_VOLTAGE_STATUS Register */

#define CHARGER_V_OVP_SET_MASK                            (0x3F << 18) /* This bit-field returns the applied V_OVP setting. In JEITA charging, it contains */
#define CHARGER_V_OVP_SET_POS                             (18) /* This bit-field returns the applied V_OVP setting. In JEITA charging, it contains */
#define CHARGER_V_REPLENISH_SET_MASK                      (0x3F << 12) /* This bit-field returns the applied V_REPLENISH setting. In JEITA charging, it co */
#define CHARGER_V_REPLENISH_SET_POS                       (12) /* This bit-field returns the applied V_REPLENISH setting. In JEITA charging, it co */
#define CHARGER_V_PRECHARGE_SET_MASK                      (0x3F << 6) /* This bit-field returns the applied V_PRECHARGE setting. In JEITA charging, it co */
#define CHARGER_V_PRECHARGE_SET_POS                       (6) /* This bit-field returns the applied V_PRECHARGE setting. In JEITA charging, it co */
#define CHARGER_V_CHARGE_SET_MASK                         (0x3F << 0) /* This bit-field returns the applied V_CHARGE setting. In JEITA charging, it conta */
#define CHARGER_V_CHARGE_SET_POS                          (0) /* This bit-field returns the applied V_CHARGE setting. In JEITA charging, it conta */

/* CHARGER_VOVP_COMP_TIMER Register */

#define CHARGER_OVP_INTERVAL_CHECK_TIMER_MASK             (0x3F << 26) /* The specific bit-field determines the current state of the timer used to periodi */
#define CHARGER_OVP_INTERVAL_CHECK_TIMER_POS              (26) /* The specific bit-field determines the current state of the timer used to periodi */
#define CHARGER_VBAT_OVP_COMP_TIMER_MASK                  (0x3FF << 16) /* Returns the current value of the timer used to determine when the Vbat Over-Volt */
#define CHARGER_VBAT_OVP_COMP_TIMER_POS                   (16) /* Returns the current value of the timer used to determine when the Vbat Over-Volt */
#define CHARGER_OVP_INTERVAL_CHECK_THRES_MASK             (0x3F << 10) /* This bit-field determines the periodic interval of checking the dedicated Vbat O */
#define CHARGER_OVP_INTERVAL_CHECK_THRES_POS              (10) /* This bit-field determines the periodic interval of checking the dedicated Vbat O */
#define CHARGER_VBAT_OVP_COMP_SETTLING_MASK               (0x3FF << 0) /* Settling time threshold (in us) for the Vbat comparator checking Vbat vs the pro */
#define CHARGER_VBAT_OVP_COMP_SETTLING_POS                (0) /* Settling time threshold (in us) for the Vbat comparator checking Vbat vs the pro */

#endif /* __DA1470_CHARGER_H */
