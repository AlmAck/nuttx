/****************************************************************************
 * ./tmp/da1470_crg_xtal.h
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

#ifndef __DA1470_CRG_XTAL_H
#define __DA1470_CRG_XTAL_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register offsets *********************************************************/

#define DA1470_CRG_XTAL_PLL_SYS_CTRL1_OFFSET       0x0060 /* System PLL control register 1. */
#define DA1470_CRG_XTAL_PLL_SYS_CTRL2_OFFSET       0x0064 /* System PLL control register 2. */
#define DA1470_CRG_XTAL_PLL_SYS_CTRL3_OFFSET       0x0068 /* System PLL control register 3. */
#define DA1470_CRG_XTAL_PLL_SYS_STATUS_OFFSET      0x0070 /* System PLL status register. */
#define DA1470_CRG_XTAL_PLL_USB_CTRL1_OFFSET       0x0074 /* USB PLL control register 1. */
#define DA1470_CRG_XTAL_PLL_USB_CTRL2_OFFSET       0x0078 /* USB PLL control register 2. */
#define DA1470_CRG_XTAL_PLL_USB_CTRL3_OFFSET       0x007C /* USB PLL control register 3. */
#define DA1470_CRG_XTAL_PLL_USB_STATUS_OFFSET      0x0080 /* USB PLL status register. */
#define DA1470_CRG_XTAL_RESET_SYS_IRQ_CTRL_OFFSET  0x0098 /* System IRQ RESET register */
#define DA1470_CRG_XTAL_SET_SYS_IRQ_CTRL_OFFSET    0x0094 /* System IRQ SET register */
#define DA1470_CRG_XTAL_SYS_IRQ_CTRL_OFFSET        0x0090 /* System IRQ control register */
#define DA1470_CRG_XTAL_XTAL32M_CAP_MEAS_OFFSET    0x000C /* Capacitance measure circuit control */
#define DA1470_CRG_XTAL_XTAL32M_CTRL_OFFSET        0x0014 /* Xtal32m control register */
#define DA1470_CRG_XTAL_XTAL32M_FSM_OFFSET         0x0010 /* Startup state machine configuration */
#define DA1470_CRG_XTAL_XTAL32M_IRQ_CTRL_OFFSET    0x0018 /* Xtal32m Interrupt control register */
#define DA1470_CRG_XTAL_XTAL32M_IRQ_STAT_OFFSET    0x0028 /* XTAL32M IRQ status register */
#define DA1470_CRG_XTAL_XTAL32M_SETTLE_OFFSET      0x0004 /* Trim values for XTAL32M in SETTLE state of startup */
#define DA1470_CRG_XTAL_XTAL32M_START_OFFSET       0x0000 /* Trim values for XTAL32M in START state of startup */
#define DA1470_CRG_XTAL_XTAL32M_STAT0_OFFSET       0x0024 /* XTAL32M status register */
#define DA1470_CRG_XTAL_XTAL32M_TRIM_OFFSET        0x0008 /* Trim values for XTAL32M in RUNNING state */

/* Register addresses *******************************************************/

#define DA1470_CRG_XTAL_BASE                       0x50050400 /* CRG_XTAL registers */

#define DA1470_CRG_XTAL_PLL_SYS_CTRL1              (DA1470_CRG_XTAL_BASE + DA1470_CRG_XTAL_PLL_SYS_CTRL1_OFFSET) /* System PLL control register 1. */
#define DA1470_CRG_XTAL_PLL_SYS_CTRL2              (DA1470_CRG_XTAL_BASE + DA1470_CRG_XTAL_PLL_SYS_CTRL2_OFFSET) /* System PLL control register 2. */
#define DA1470_CRG_XTAL_PLL_SYS_CTRL3              (DA1470_CRG_XTAL_BASE + DA1470_CRG_XTAL_PLL_SYS_CTRL3_OFFSET) /* System PLL control register 3. */
#define DA1470_CRG_XTAL_PLL_SYS_STATUS             (DA1470_CRG_XTAL_BASE + DA1470_CRG_XTAL_PLL_SYS_STATUS_OFFSET) /* System PLL status register. */
#define DA1470_CRG_XTAL_PLL_USB_CTRL1              (DA1470_CRG_XTAL_BASE + DA1470_CRG_XTAL_PLL_USB_CTRL1_OFFSET) /* USB PLL control register 1. */
#define DA1470_CRG_XTAL_PLL_USB_CTRL2              (DA1470_CRG_XTAL_BASE + DA1470_CRG_XTAL_PLL_USB_CTRL2_OFFSET) /* USB PLL control register 2. */
#define DA1470_CRG_XTAL_PLL_USB_CTRL3              (DA1470_CRG_XTAL_BASE + DA1470_CRG_XTAL_PLL_USB_CTRL3_OFFSET) /* USB PLL control register 3. */
#define DA1470_CRG_XTAL_PLL_USB_STATUS             (DA1470_CRG_XTAL_BASE + DA1470_CRG_XTAL_PLL_USB_STATUS_OFFSET) /* USB PLL status register. */
#define DA1470_CRG_XTAL_RESET_SYS_IRQ_CTRL         (DA1470_CRG_XTAL_BASE + DA1470_CRG_XTAL_RESET_SYS_IRQ_CTRL_OFFSET) /* System IRQ RESET register */
#define DA1470_CRG_XTAL_SET_SYS_IRQ_CTRL           (DA1470_CRG_XTAL_BASE + DA1470_CRG_XTAL_SET_SYS_IRQ_CTRL_OFFSET) /* System IRQ SET register */
#define DA1470_CRG_XTAL_SYS_IRQ_CTRL               (DA1470_CRG_XTAL_BASE + DA1470_CRG_XTAL_SYS_IRQ_CTRL_OFFSET) /* System IRQ control register */
#define DA1470_CRG_XTAL_XTAL32M_CAP_MEAS           (DA1470_CRG_XTAL_BASE + DA1470_CRG_XTAL_XTAL32M_CAP_MEAS_OFFSET) /* Capacitance measure circuit control */
#define DA1470_CRG_XTAL_XTAL32M_CTRL               (DA1470_CRG_XTAL_BASE + DA1470_CRG_XTAL_XTAL32M_CTRL_OFFSET) /* Xtal32m control register */
#define DA1470_CRG_XTAL_XTAL32M_FSM                (DA1470_CRG_XTAL_BASE + DA1470_CRG_XTAL_XTAL32M_FSM_OFFSET) /* Startup state machine configuration */
#define DA1470_CRG_XTAL_XTAL32M_IRQ_CTRL           (DA1470_CRG_XTAL_BASE + DA1470_CRG_XTAL_XTAL32M_IRQ_CTRL_OFFSET) /* Xtal32m Interrupt control register */
#define DA1470_CRG_XTAL_XTAL32M_IRQ_STAT           (DA1470_CRG_XTAL_BASE + DA1470_CRG_XTAL_XTAL32M_IRQ_STAT_OFFSET) /* XTAL32M IRQ status register */
#define DA1470_CRG_XTAL_XTAL32M_SETTLE             (DA1470_CRG_XTAL_BASE + DA1470_CRG_XTAL_XTAL32M_SETTLE_OFFSET) /* Trim values for XTAL32M in SETTLE state of startup */
#define DA1470_CRG_XTAL_XTAL32M_START              (DA1470_CRG_XTAL_BASE + DA1470_CRG_XTAL_XTAL32M_START_OFFSET) /* Trim values for XTAL32M in START state of startup */
#define DA1470_CRG_XTAL_XTAL32M_STAT0              (DA1470_CRG_XTAL_BASE + DA1470_CRG_XTAL_XTAL32M_STAT0_OFFSET) /* XTAL32M status register */
#define DA1470_CRG_XTAL_XTAL32M_TRIM               (DA1470_CRG_XTAL_BASE + DA1470_CRG_XTAL_XTAL32M_TRIM_OFFSET) /* Trim values for XTAL32M in RUNNING state */

/* Register bit definitions *************************************************/


/* PLL_SYS_CTRL1 Register */

#define CRG_XTAL_PLL_OUT_DIV                       (1U << 15) /* 0: Output divider ON 1: Output divider OFF */
#define CRG_XTAL_PLL_SEL_MIN_CUR_INT               (1U << 14) /* 0: VCO current read from min_current <5:0>, 1: VCO current is internally determi */
#define CRG_XTAL_PLL_PRE_DIV                       (1U << 11) /* PLL input divider (1: Indicates divide by 2). */
#define CRG_XTAL_PLL_N_DIV_MASK                    (0x7F << 4) /* PLL loop divider N (x means divide by x, 0 means divide by 1) */
#define CRG_XTAL_PLL_N_DIV_POS                     (4) /* PLL loop divider N (x means divide by x, 0 means divide by 1) */
#define CRG_XTAL_LDO_PLL_VREF_HOLD                 (1U << 3) /* 0: Indicates that the reference input is tracked, 1: Indicates that the referenc */
#define CRG_XTAL_LDO_PLL_ENABLE                    (1U << 2) /* 0: LDO PLL off, 1: LDO PLL on. */
#define CRG_XTAL_PLL_RST_N                         (1U << 1) /* 0: PLL in Reset 1L PLL out of Reset */
#define CRG_XTAL_PLL_EN                            (1U << 0) /* 0: Power down 1: PLL on */

/* PLL_SYS_CTRL2 Register */

#define CRG_XTAL_PLL_RECALIB                       (1U << 15) /* Recalibrate */

/* PLL_SYS_CTRL3 Register */

#define CRG_XTAL_PLL_MIN_CURRENT_MASK              (0x3F << 1) /* VCO current trimming. */
#define CRG_XTAL_PLL_MIN_CURRENT_POS               (1) /* VCO current trimming. */

/* PLL_SYS_STATUS Register */

#define CRG_XTAL_LDO_PLL_OK                        (1U << 15) /* 1: Indicates that LDO PLL is in regulation. */
#define CRG_XTAL_PLL_CALIBRATION_END               (1U << 11) /* Indicates that calibration has finished. */
#define CRG_XTAL_PLL_BEST_MIN_CUR_MASK             (0x3F << 5) /* Calibrated VCO current. */
#define CRG_XTAL_PLL_BEST_MIN_CUR_POS              (5) /* Calibrated VCO current. */
#define CRG_XTAL_PLL_LOCK_FINE                     (1U << 0) /* 1: PLL locked */

/* PLL_USB_CTRL1 Register */

#define CRG_XTAL_PLL_OUT_DIV                       (1U << 15) /* 0: Output divider ON 1: Output divider OFF */
#define CRG_XTAL_PLL_SEL_MIN_CUR_INT               (1U << 14) /* 0: VCO current read from min_current <5:0>, 1: VCO current is internally determi */
#define CRG_XTAL_PLL_PRE_DIV                       (1U << 11) /* PLL input divider (1: Indicates divide by 2). */
#define CRG_XTAL_PLL_N_DIV_MASK                    (0x7F << 4) /* PLL loop divider N (x means divide by x, 0 means divide by 1) */
#define CRG_XTAL_PLL_N_DIV_POS                     (4) /* PLL loop divider N (x means divide by x, 0 means divide by 1) */
#define CRG_XTAL_LDO_PLL_VREF_HOLD                 (1U << 3) /* 0: Indicates that the reference input is tracked, 1: Indicates that the referenc */
#define CRG_XTAL_LDO_PLL_ENABLE                    (1U << 2) /* 0: LDO PLL off, 1: LDO PLL on. */
#define CRG_XTAL_PLL_RST_N                         (1U << 1) /* 0: PLL in Reset 1L PLL out of Reset */
#define CRG_XTAL_PLL_EN                            (1U << 0) /* 0: Power down 1: PLL on */

/* PLL_USB_CTRL2 Register */

#define CRG_XTAL_PLL_RECALIB                       (1U << 15) /* Recalibrate */

/* PLL_USB_CTRL3 Register */

#define CRG_XTAL_PLL_MIN_CURRENT_MASK              (0x3F << 1) /* VCO current trimming. */
#define CRG_XTAL_PLL_MIN_CURRENT_POS               (1) /* VCO current trimming. */

/* PLL_USB_STATUS Register */

#define CRG_XTAL_LDO_PLL_OK                        (1U << 15) /* 1: Indicates that LDO PLL is in regulation. */
#define CRG_XTAL_PLL_CALIBRATION_END               (1U << 11) /* Indicates that calibration has finished. */
#define CRG_XTAL_PLL_BEST_MIN_CUR_MASK             (0x3F << 5) /* Calibrated VCO current. */
#define CRG_XTAL_PLL_BEST_MIN_CUR_POS              (5) /* Calibrated VCO current. */
#define CRG_XTAL_PLL_LOCK_FINE                     (1U << 0) /* 1: PLL locked */

/* RESET_SYS_IRQ_CTRL Register */

#define CRG_XTAL_CMAC2SNC_IRQ_BIT                  (1U << 5) /* No description available. */
#define CRG_XTAL_CMAC2SYS_IRQ_BIT                  (1U << 4) /* No description available. */
#define CRG_XTAL_SNC2SYS_IRQ_BIT                   (1U << 3) /* No description available. */
#define CRG_XTAL_SNC2CMAC_IRQ_BIT                  (1U << 2) /* No description available. */
#define CRG_XTAL_SYS2SNC_IRQ_BIT                   (1U << 1) /* No description available. */
#define CRG_XTAL_SYS2CMAC_IRQ_BIT                  (1U << 0) /* No description available. */

/* SET_SYS_IRQ_CTRL Register */

#define CRG_XTAL_CMAC2SNC_IRQ_BIT                  (1U << 5) /* No description available. */
#define CRG_XTAL_CMAC2SYS_IRQ_BIT                  (1U << 4) /* No description available. */
#define CRG_XTAL_SNC2SYS_IRQ_BIT                   (1U << 3) /* No description available. */
#define CRG_XTAL_SNC2CMAC_IRQ_BIT                  (1U << 2) /* No description available. */
#define CRG_XTAL_SYS2SNC_IRQ_BIT                   (1U << 1) /* No description available. */
#define CRG_XTAL_SYS2CMAC_IRQ_BIT                  (1U << 0) /* No description available. */

/* SYS_IRQ_CTRL Register */

#define CRG_XTAL_CMAC2SNC_IRQ_BIT                  (1U << 5) /* No description available. */
#define CRG_XTAL_CMAC2SYS_IRQ_BIT                  (1U << 4) /* No description available. */
#define CRG_XTAL_SNC2SYS_IRQ_BIT                   (1U << 3) /* No description available. */
#define CRG_XTAL_SNC2CMAC_IRQ_BIT                  (1U << 2) /* No description available. */
#define CRG_XTAL_SYS2SNC_IRQ_BIT                   (1U << 1) /* No description available. */
#define CRG_XTAL_SYS2CMAC_IRQ_BIT                  (1U << 0) /* No description available. */

/* XTAL32M_CAP_MEAS Register */

#define CRG_XTAL_XTAL32M_MEAS_TIME_MASK            (0x7 << 6) /* Select measurement time (in DIVN clock-cycles) 0: 32 1: 64  6: 2048 7: 4096 */
#define CRG_XTAL_XTAL32M_MEAS_TIME_POS             (6) /* Select measurement time (in DIVN clock-cycles) 0: 32 1: 64  6: 2048 7: 4096 */
#define CRG_XTAL_XTAL32M_MEAS_START                (1U << 5) /* Starts capacitance measurement */
#define CRG_XTAL_XTAL32M_MEAS_CUR_MASK             (0x3 << 3) /* Select measurement current (minimum required capacitance) 0: 100nA (0.44pF) 1: 5 */
#define CRG_XTAL_XTAL32M_MEAS_CUR_POS              (3) /* Select measurement current (minimum required capacitance) 0: 100nA (0.44pF) 1: 5 */
#define CRG_XTAL_XTAL32M_CAP_SELECT_MASK           (0x7 << 0) /* Select measured capacitance 0: disabled 1: hold capacitance 2: xtal_p 3: xtal_n  */
#define CRG_XTAL_XTAL32M_CAP_SELECT_POS            (0) /* Select measured capacitance 0: disabled 1: hold capacitance 2: xtal_p 3: xtal_n  */

/* XTAL32M_CTRL Register */

#define CRG_XTAL_XTAL32M_DRIVE_CYCLES_MASK         (0x7 << 9) /* Number of drive clock-cycles 0x0: Drive disabled 0x1: 4 0x2: 8 0x3: 16 0x4: 32 0 */
#define CRG_XTAL_XTAL32M_DRIVE_CYCLES_POS          (9) /* Number of drive clock-cycles 0x0: Drive disabled 0x1: 4 0x2: 8 0x3: 16 0x4: 32 0 */
#define CRG_XTAL_XTAL32M_ENABLE                    (1U << 8) /* Enables xtal32m (testing purposes) */
#define CRG_XTAL_XTAL32M_BIASPROT_MASK             (0x3 << 6) /* Bias startup circuit 0: enable during startup 1: always enabled 2: always disabl */
#define CRG_XTAL_XTAL32M_BIASPROT_POS              (6) /* Bias startup circuit 0: enable during startup 1: always enabled 2: always disabl */
#define CRG_XTAL_XTAL32M_LDO_SAH_MASK              (0x3 << 4) /* Controls amplitude regulator sample-and-hold 2'b00: set to HOLD when IRQ fires 2 */
#define CRG_XTAL_XTAL32M_LDO_SAH_POS               (4) /* Controls amplitude regulator sample-and-hold 2'b00: set to HOLD when IRQ fires 2 */
#define CRG_XTAL_XTAL32M_AMPREG_SAH_MASK           (0x3 << 2) /* Controls amplitude regulator sample-and-hold 2'b00: set to HOLD when IRQ fires 2 */
#define CRG_XTAL_XTAL32M_AMPREG_SAH_POS            (2) /* Controls amplitude regulator sample-and-hold 2'b00: set to HOLD when IRQ fires 2 */
#define CRG_XTAL_XTAL32M_BIAS_SAH_MASK             (0x3 << 0) /* Controls bias sample-and-hold 2'b00: set to HOLD when IRQ fires 2'b01: always TR */
#define CRG_XTAL_XTAL32M_BIAS_SAH_POS              (0) /* Controls bias sample-and-hold 2'b00: set to HOLD when IRQ fires 2'b01: always TR */

/* XTAL32M_FSM Register */

#define CRG_XTAL_XTAL32M_BOOST_MODE                (1U << 5) /* Boost mode configuration 0: Only allow BOOST mode in START state 1: Allow BOOST  */
#define CRG_XTAL_XTAL32M_FSM_APPLY_CONFIG          (1U << 4) /* CUR_SET, AMPL_SET, CMP_LVL and TRIM from XTAL32M_TRIM_REG are 0: applied at next */
#define CRG_XTAL_XTAL32M_FSM_FORCE_IDLE            (1U << 3) /* Forces FSM in IDLE state, allows for software control */
#define CRG_XTAL_XTAL32M_CMP_MODE                  (1U << 2) /* Use following comparator trim settings in SETTLE state: 0: XTAL32M_TRIM_REG.CMP_ */
#define CRG_XTAL_XTAL32M_TRIM_MODE                 (1U << 1) /* Use following trimsetting in the SETTLE state 0: XTAL32M_TRIM_REG.TRIM 1: XTAL32 */
#define CRG_XTAL_XTAL32M_CUR_MODE                  (1U << 0) /* Use the following current setting in the SETTLE state 0: XTAL32M_START_REG.CUR_S */

/* XTAL32M_IRQ_CTRL Register */

#define CRG_XTAL_XTAL32M_IRQ_CAP_CTRL_MASK         (0x3 << 10) /* The IRQ counter is captured in the XTAL32M_IRQ_STATUS_REG.IRQ_COUNT_CAP when lea */
#define CRG_XTAL_XTAL32M_IRQ_CAP_CTRL_POS          (10) /* The IRQ counter is captured in the XTAL32M_IRQ_STATUS_REG.IRQ_COUNT_CAP when lea */
#define CRG_XTAL_XTAL32M_IRQ_ENABLE                (1U << 9) /* Enable xtal interrupt generation. */
#define CRG_XTAL_XTAL32M_IRQ_CLK                   (1U << 8) /* Clock divider for IRQ counter 0: 4us 1: 32us */
#define CRG_XTAL_XTAL32M_IRQ_CNT_MASK              (0xFF << 0) /* IRQ counter start value. */
#define CRG_XTAL_XTAL32M_IRQ_CNT_POS               (0) /* IRQ counter start value. */

/* XTAL32M_IRQ_STAT Register */

#define CRG_XTAL_XTAL32M_IRQ_COUNT_CAP_MASK        (0xFF << 8) /* Captured IRQ counter */
#define CRG_XTAL_XTAL32M_IRQ_COUNT_CAP_POS         (8) /* Captured IRQ counter */
#define CRG_XTAL_XTAL32M_IRQ_COUNT_STAT_MASK       (0xFF << 0) /* Current IRQ counter value */
#define CRG_XTAL_XTAL32M_IRQ_COUNT_STAT_POS        (0) /* Current IRQ counter value */

/* XTAL32M_SETTLE Register */

#define CRG_XTAL_XTAL32M_TIMEOUT_MASK              (0x7F << 22) /* Timeout 0: disabled 1: 4us 2: 8us  63: 252us 64: 268us ... 127: 1260us */
#define CRG_XTAL_XTAL32M_TIMEOUT_POS               (22) /* Timeout 0: disabled 1: 4us 2: 8us  63: 252us 64: 268us ... 127: 1260us */
#define CRG_XTAL_XTAL32M_CMP_BLANK_MASK            (0x7 << 19) /* Blanking time for comparator output 0: disabled 1: 4us 2: 8us 3: 16us 4: 32us 5: */
#define CRG_XTAL_XTAL32M_CMP_BLANK_POS             (19) /* Blanking time for comparator output 0: disabled 1: 4us 2: 8us 3: 16us 4: 32us 5: */
#define CRG_XTAL_XTAL32M_CMP_LVL_MASK              (0x3 << 17) /* Comparator triplevel 0: 30% 1: 35% 2: 45% 3: 60% */
#define CRG_XTAL_XTAL32M_CMP_LVL_POS               (17) /* Comparator triplevel 0: 30% 1: 35% 2: 45% 3: 60% */
#define CRG_XTAL_XTAL32M_AMPL_SET_MASK             (0x7 << 14) /* Amplitude Regulator input level setting (peak-peak) in SETTLE phase of startup 0 */
#define CRG_XTAL_XTAL32M_AMPL_SET_POS              (14) /* Amplitude Regulator input level setting (peak-peak) in SETTLE phase of startup 0 */
#define CRG_XTAL_XTAL32M_CUR_SET_MASK              (0xF << 10) /* Current setting (units of 16uA) in SETTLE phase of startup 0: OFF 1: 1x 2: 2x 3: */
#define CRG_XTAL_XTAL32M_CUR_SET_POS               (10) /* Current setting (units of 16uA) in SETTLE phase of startup 0: OFF 1: 1x 2: 2x 3: */
#define CRG_XTAL_XTAL32M_TRIM_MASK                 (0x3FF << 0) /* Capacitance bank seting in SETLLE phase of startup CL = 3.5pF + 50fF/LSB */
#define CRG_XTAL_XTAL32M_TRIM_POS                  (0) /* Capacitance bank seting in SETLLE phase of startup CL = 3.5pF + 50fF/LSB */

/* XTAL32M_START Register */

#define CRG_XTAL_XTAL32M_TIMEOUT_MASK              (0x7F << 22) /* Timeout 0: disabled 1: 4us 2: 8us  63: 252us 64: 268us ... 127: 1260us */
#define CRG_XTAL_XTAL32M_TIMEOUT_POS               (22) /* Timeout 0: disabled 1: 4us 2: 8us  63: 252us 64: 268us ... 127: 1260us */
#define CRG_XTAL_XTAL32M_CMP_BLANK_MASK            (0x7 << 19) /* Blanking time for comparator output 0: disabled 1: 4us 2: 8us 3: 16us 4: 32us 5: */
#define CRG_XTAL_XTAL32M_CMP_BLANK_POS             (19) /* Blanking time for comparator output 0: disabled 1: 4us 2: 8us 3: 16us 4: 32us 5: */
#define CRG_XTAL_XTAL32M_CMP_LVL_MASK              (0x3 << 17) /* Comparator triplevel 0: 30% 1: 35% 2: 45% 3: 60% */
#define CRG_XTAL_XTAL32M_CMP_LVL_POS               (17) /* Comparator triplevel 0: 30% 1: 35% 2: 45% 3: 60% */
#define CRG_XTAL_XTAL32M_AMPL_SET_MASK             (0x7 << 14) /* Amplitude Regulator input level setting (peak-peak) in START phase of startup 0: */
#define CRG_XTAL_XTAL32M_AMPL_SET_POS              (14) /* Amplitude Regulator input level setting (peak-peak) in START phase of startup 0: */
#define CRG_XTAL_XTAL32M_CUR_SET_MASK              (0xF << 10) /* Current setting (units of 16uA) in START phase of startup 0: OFF 1: 1x 2: 2x 3:  */
#define CRG_XTAL_XTAL32M_CUR_SET_POS               (10) /* Current setting (units of 16uA) in START phase of startup 0: OFF 1: 1x 2: 2x 3:  */
#define CRG_XTAL_XTAL32M_TRIM_MASK                 (0x3FF << 0) /* Capacitance bank seting in START phase of startup CL = 3.5pF + 50fF/LSB */
#define CRG_XTAL_XTAL32M_TRIM_POS                  (0) /* Capacitance bank seting in START phase of startup CL = 3.5pF + 50fF/LSB */

/* XTAL32M_STAT0 Register */

#define CRG_XTAL_XTAL32M_OVERLOAD                  (1U << 29) /* Indicates xtal is overloaded */
#define CRG_XTAL_XTAL32M_CMP_LVL_STAT_MASK         (0x3 << 27) /* Current value for amplitude regulator comparator setting */
#define CRG_XTAL_XTAL32M_CMP_LVL_STAT_POS          (27) /* Current value for amplitude regulator comparator setting */
#define CRG_XTAL_XTAL32M_AMPL_TRIM_MASK            (0x7 << 24) /* Current value for amplitude trim */
#define CRG_XTAL_XTAL32M_AMPL_TRIM_POS             (24) /* Current value for amplitude trim */
#define CRG_XTAL_XTAL32M_TRIM_VAL_MASK             (0x3FF << 14) /* Current value for oscillator trimming */
#define CRG_XTAL_XTAL32M_TRIM_VAL_POS              (14) /* Current value for oscillator trimming */
#define CRG_XTAL_XTAL32M_CUR_SET_STAT_MASK         (0xF << 10) /* Current value for cur_set */
#define CRG_XTAL_XTAL32M_CUR_SET_STAT_POS          (10) /* Current value for cur_set */
#define CRG_XTAL_XTAL32M_LDO_OK                    (1U << 9) /* Indicates LDO voltage level is ok */
#define CRG_XTAL_XTAL32M_CMP_OUT_MASK              (0x3 << 7) /* Amplitude regulator comparator output state */
#define CRG_XTAL_XTAL32M_CMP_OUT_POS               (7) /* Amplitude regulator comparator output state */
#define CRG_XTAL_XTAL32M_STATE_MASK                (0xF << 3) /* State of xtal startup FSM 0x0: IDLE 0x1: WAIT_LDO 0x2: WAIT_BIAS 0x3: XTAL_DRIVE */
#define CRG_XTAL_XTAL32M_STATE_POS                 (3) /* State of xtal startup FSM 0x0: IDLE 0x1: WAIT_LDO 0x2: WAIT_BIAS 0x3: XTAL_DRIVE */
#define CRG_XTAL_XTAL32M_CMP_OUT_HOLD_MASK         (0x3 << 1) /* Captured state of amplitude regulator comparators at IRQ fire. */
#define CRG_XTAL_XTAL32M_CMP_OUT_HOLD_POS          (1) /* Captured state of amplitude regulator comparators at IRQ fire. */
#define CRG_XTAL_XTAL32M_READY                     (1U << 0) /* Indicates xtal startup FSM has reached the RUNNIG state and is ready for use (sy */

/* XTAL32M_TRIM Register */

#define CRG_XTAL_XTAL32M_BOOST_TRIM_MASK           (0x3F << 19) /* Boost trimming, set accordingly to shunt capacitance. Sensitivity: 125fF/LSB 0x0 */
#define CRG_XTAL_XTAL32M_BOOST_TRIM_POS            (19) /* Boost trimming, set accordingly to shunt capacitance. Sensitivity: 125fF/LSB 0x0 */
#define CRG_XTAL_XTAL32M_CMP_LVL_MASK              (0x3 << 17) /* Comparator triplevel 0: 30% 1: 35% 2: 45% 3: 60% */
#define CRG_XTAL_XTAL32M_CMP_LVL_POS               (17) /* Comparator triplevel 0: 30% 1: 35% 2: 45% 3: 60% */
#define CRG_XTAL_XTAL32M_AMPL_SET_MASK             (0x7 << 14) /* Amplitude Regulator input level setting (peak-peak) in running phase 0: 300mV 1: */
#define CRG_XTAL_XTAL32M_AMPL_SET_POS              (14) /* Amplitude Regulator input level setting (peak-peak) in running phase 0: 300mV 1: */
#define CRG_XTAL_XTAL32M_CUR_SET_MASK              (0xF << 10) /* Current setting (units of 16uA) in running phase 0: OFF 1: 1x 2: 2x 3: 3x 4: 4x  */
#define CRG_XTAL_XTAL32M_CUR_SET_POS               (10) /* Current setting (units of 16uA) in running phase 0: OFF 1: 1x 2: 2x 3: 3x 4: 4x  */
#define CRG_XTAL_XTAL32M_TRIM_MASK                 (0x3FF << 0) /* Capacitance bank seting in running phase, use to trim the xtal32m output frequen */
#define CRG_XTAL_XTAL32M_TRIM_POS                  (0) /* Capacitance bank seting in running phase, use to trim the xtal32m output frequen */

#endif /* __DA1470_CRG_XTAL_H */
