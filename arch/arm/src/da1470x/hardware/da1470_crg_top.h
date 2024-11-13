/****************************************************************************
 * ./tmp/da1470_crg_top.h
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

#ifndef __DA1470_CRG_TOP_H
#define __DA1470_CRG_TOP_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register offsets *********************************************************/

#define DA1470_CRG_TOP_ANA_STATUS_OFFSET          0x00EC /* Analog Signals Status Register */
#define DA1470_CRG_TOP_BANDGAP_OFFSET             0x0050 /* bandgap trimming */
#define DA1470_CRG_TOP_BIAS_VREF_SEL_OFFSET       0x00E8 /* No description available. */
#define DA1470_CRG_TOP_BOD_CTRL_OFFSET            0x0060 /* Brown Out Detection control register */
#define DA1470_CRG_TOP_BOD_STATUS_OFFSET          0x0064 /* No description available. */
#define DA1470_CRG_TOP_CLK_AMBA_OFFSET            0x0000 /* HCLK, PCLK, divider and clock gates */
#define DA1470_CRG_TOP_CLK_CMAC_SWITCH_OFFSET     0x0004 /* Clock switching register for CMAC clock domain */
#define DA1470_CRG_TOP_CLK_CTRL_OFFSET            0x0014 /* Clock control register */
#define DA1470_CRG_TOP_CLK_RADIO_OFFSET           0x0010 /* Radio PLL control register */
#define DA1470_CRG_TOP_CLK_RCHS_OFFSET            0x0044 /* Fast RC control register */
#define DA1470_CRG_TOP_CLK_RCLP_OFFSET            0x003C /* 32/512 kHz RC oscillator register */
#define DA1470_CRG_TOP_CLK_RCX_OFFSET             0x0048 /* RCX-oscillator control register */
#define DA1470_CRG_TOP_CLK_RTCDIV_OFFSET          0x004C /* Divisor for RTC 100Hz clock */
#define DA1470_CRG_TOP_CLK_SNC_CTRL_OFFSET        0x002C /* No description available. */
#define DA1470_CRG_TOP_CLK_SWITCH2XTAL_OFFSET     0x001C /* Switches clock from RC32M to XTAL32M */
#define DA1470_CRG_TOP_CLK_TMR_OFFSET             0x0018 /* Clock control for the timers */
#define DA1470_CRG_TOP_CLK_XTAL32K_OFFSET         0x0040 /* 32 kHz XTAL oscillator register */
#define DA1470_CRG_TOP_DISCHARGE_RAIL_OFFSET      0x00D4 /* Immediate rail resetting. There is no LDO/DCDC gating */
#define DA1470_CRG_TOP_LCD_EXT_CTRL_OFFSET        0x0034 /* No description available. */
#define DA1470_CRG_TOP_P0_PAD_LATCH_OFFSET        0x0070 /* Control the state retention of the GPIO ports */
#define DA1470_CRG_TOP_P0_RESET_PAD_LATCH_OFFSET  0x0078 /* Control the state retention of the GPIO ports */
#define DA1470_CRG_TOP_P0_SET_PAD_LATCH_OFFSET    0x0074 /* Control the state retention of the GPIO ports */
#define DA1470_CRG_TOP_P1_PAD_LATCH_OFFSET        0x007C /* Control the state retention of the GPIO ports */
#define DA1470_CRG_TOP_P1_RESET_PAD_LATCH_OFFSET  0x0084 /* Control the state retention of the GPIO ports */
#define DA1470_CRG_TOP_P1_SET_PAD_LATCH_OFFSET    0x0080 /* Control the state retention of the GPIO ports */
#define DA1470_CRG_TOP_P2_PAD_LATCH_OFFSET        0x0088 /* Control the state retention of the GPIO ports */
#define DA1470_CRG_TOP_P2_RESET_PAD_LATCH_OFFSET  0x0090 /* Control the state retention of the GPIO ports */
#define DA1470_CRG_TOP_P2_SET_PAD_LATCH_OFFSET    0x008C /* Control the state retention of the GPIO ports */
#define DA1470_CRG_TOP_PMU_CTRL_OFFSET            0x0020 /* Power Management Unit control register */
#define DA1470_CRG_TOP_PMU_SLEEP_OFFSET           0x00F4 /* Configures the sleep/wakeup strategy */
#define DA1470_CRG_TOP_PMU_TRIM_OFFSET            0x00D0 /* No description available. */
#define DA1470_CRG_TOP_POR_CTRL_OFFSET            0x0094 /* Controls the POR on VBAT */
#define DA1470_CRG_TOP_POR_PIN_OFFSET             0x0098 /* Selects a GPIO pin for POR generation */
#define DA1470_CRG_TOP_POR_TIMER_OFFSET           0x009C /* Time for POR to happen */
#define DA1470_CRG_TOP_POWER_CTRL_OFFSET          0x00F0 /* Power control register */
#define DA1470_CRG_TOP_POWER_LVL_OFFSET           0x00F8 /* No description available. */
#define DA1470_CRG_TOP_RAM_PWR_CTRL_OFFSET        0x00C0 /* Control power state of System RAMS */
#define DA1470_CRG_TOP_RESET_STAT_OFFSET          0x00BC /* Reset status register */
#define DA1470_CRG_TOP_RST_CTRL_OFFSET            0x000C /* Reset control register */
#define DA1470_CRG_TOP_SECURE_BOOT_OFFSET         0x00CC /* Controls secure booting */
#define DA1470_CRG_TOP_SLP_MAP_OFFSET             0x0030 /* Map signals on GPIOs during sleep */
#define DA1470_CRG_TOP_SW_V18F_OFFSET             0x00E4 /* No description available. */
#define DA1470_CRG_TOP_SYS_CTRL_OFFSET            0x0024 /* System Control register */
#define DA1470_CRG_TOP_SYS_STAT_OFFSET            0x0028 /* System status register */
#define DA1470_CRG_TOP_VBUS_IRQ_CLEAR_OFFSET      0x0058 /* Clear pending IRQ register */
#define DA1470_CRG_TOP_VBUS_IRQ_MASK_OFFSET       0x0054 /* IRQ masking */
#define DA1470_CRG_TOP_WAKEUP_HIBERN_OFFSET       0x00E0 /* No description available. */

/* Register addresses *******************************************************/

#define DA1470_CRG_TOP_BASE                       0x50000000 /* CRG_TOP registers */

#define DA1470_CRG_TOP_ANA_STATUS                 (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_ANA_STATUS_OFFSET) /* Analog Signals Status Register */
#define DA1470_CRG_TOP_BANDGAP                    (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_BANDGAP_OFFSET) /* bandgap trimming */
#define DA1470_CRG_TOP_BIAS_VREF_SEL              (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_BIAS_VREF_SEL_OFFSET) /* No description available. */
#define DA1470_CRG_TOP_BOD_CTRL                   (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_BOD_CTRL_OFFSET) /* Brown Out Detection control register */
#define DA1470_CRG_TOP_BOD_STATUS                 (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_BOD_STATUS_OFFSET) /* No description available. */
#define DA1470_CRG_TOP_CLK_AMBA                   (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_CLK_AMBA_OFFSET) /* HCLK, PCLK, divider and clock gates */
#define DA1470_CRG_TOP_CLK_CMAC_SWITCH            (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_CLK_CMAC_SWITCH_OFFSET) /* Clock switching register for CMAC clock domain */
#define DA1470_CRG_TOP_CLK_CTRL                   (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_CLK_CTRL_OFFSET) /* Clock control register */
#define DA1470_CRG_TOP_CLK_RADIO                  (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_CLK_RADIO_OFFSET) /* Radio PLL control register */
#define DA1470_CRG_TOP_CLK_RCHS                   (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_CLK_RCHS_OFFSET) /* Fast RC control register */
#define DA1470_CRG_TOP_CLK_RCLP                   (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_CLK_RCLP_OFFSET) /* 32/512 kHz RC oscillator register */
#define DA1470_CRG_TOP_CLK_RCX                    (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_CLK_RCX_OFFSET) /* RCX-oscillator control register */
#define DA1470_CRG_TOP_CLK_RTCDIV                 (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_CLK_RTCDIV_OFFSET) /* Divisor for RTC 100Hz clock */
#define DA1470_CRG_TOP_CLK_SNC_CTRL               (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_CLK_SNC_CTRL_OFFSET) /* No description available. */
#define DA1470_CRG_TOP_CLK_SWITCH2XTAL            (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_CLK_SWITCH2XTAL_OFFSET) /* Switches clock from RC32M to XTAL32M */
#define DA1470_CRG_TOP_CLK_TMR                    (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_CLK_TMR_OFFSET) /* Clock control for the timers */
#define DA1470_CRG_TOP_CLK_XTAL32K                (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_CLK_XTAL32K_OFFSET) /* 32 kHz XTAL oscillator register */
#define DA1470_CRG_TOP_DISCHARGE_RAIL             (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_DISCHARGE_RAIL_OFFSET) /* Immediate rail resetting. There is no LDO/DCDC gating */
#define DA1470_CRG_TOP_LCD_EXT_CTRL               (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_LCD_EXT_CTRL_OFFSET) /* No description available. */
#define DA1470_CRG_TOP_P0_PAD_LATCH               (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_P0_PAD_LATCH_OFFSET) /* Control the state retention of the GPIO ports */
#define DA1470_CRG_TOP_P0_RESET_PAD_LATCH         (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_P0_RESET_PAD_LATCH_OFFSET) /* Control the state retention of the GPIO ports */
#define DA1470_CRG_TOP_P0_SET_PAD_LATCH           (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_P0_SET_PAD_LATCH_OFFSET) /* Control the state retention of the GPIO ports */
#define DA1470_CRG_TOP_P1_PAD_LATCH               (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_P1_PAD_LATCH_OFFSET) /* Control the state retention of the GPIO ports */
#define DA1470_CRG_TOP_P1_RESET_PAD_LATCH         (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_P1_RESET_PAD_LATCH_OFFSET) /* Control the state retention of the GPIO ports */
#define DA1470_CRG_TOP_P1_SET_PAD_LATCH           (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_P1_SET_PAD_LATCH_OFFSET) /* Control the state retention of the GPIO ports */
#define DA1470_CRG_TOP_P2_PAD_LATCH               (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_P2_PAD_LATCH_OFFSET) /* Control the state retention of the GPIO ports */
#define DA1470_CRG_TOP_P2_RESET_PAD_LATCH         (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_P2_RESET_PAD_LATCH_OFFSET) /* Control the state retention of the GPIO ports */
#define DA1470_CRG_TOP_P2_SET_PAD_LATCH           (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_P2_SET_PAD_LATCH_OFFSET) /* Control the state retention of the GPIO ports */
#define DA1470_CRG_TOP_PMU_CTRL                   (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_PMU_CTRL_OFFSET) /* Power Management Unit control register */
#define DA1470_CRG_TOP_PMU_SLEEP                  (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_PMU_SLEEP_OFFSET) /* Configures the sleep/wakeup strategy */
#define DA1470_CRG_TOP_PMU_TRIM                   (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_PMU_TRIM_OFFSET) /* No description available. */
#define DA1470_CRG_TOP_POR_CTRL                   (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_POR_CTRL_OFFSET) /* Controls the POR on VBAT */
#define DA1470_CRG_TOP_POR_PIN                    (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_POR_PIN_OFFSET) /* Selects a GPIO pin for POR generation */
#define DA1470_CRG_TOP_POR_TIMER                  (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_POR_TIMER_OFFSET) /* Time for POR to happen */
#define DA1470_CRG_TOP_POWER_CTRL                 (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_POWER_CTRL_OFFSET) /* Power control register */
#define DA1470_CRG_TOP_POWER_LVL                  (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_POWER_LVL_OFFSET) /* No description available. */
#define DA1470_CRG_TOP_RAM_PWR_CTRL               (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_RAM_PWR_CTRL_OFFSET) /* Control power state of System RAMS */
#define DA1470_CRG_TOP_RESET_STAT                 (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_RESET_STAT_OFFSET) /* Reset status register */
#define DA1470_CRG_TOP_RST_CTRL                   (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_RST_CTRL_OFFSET) /* Reset control register */
#define DA1470_CRG_TOP_SECURE_BOOT                (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_SECURE_BOOT_OFFSET) /* Controls secure booting */
#define DA1470_CRG_TOP_SLP_MAP                    (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_SLP_MAP_OFFSET) /* Map signals on GPIOs during sleep */
#define DA1470_CRG_TOP_SW_V18F                    (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_SW_V18F_OFFSET) /* No description available. */
#define DA1470_CRG_TOP_SYS_CTRL                   (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_SYS_CTRL_OFFSET) /* System Control register */
#define DA1470_CRG_TOP_SYS_STAT                   (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_SYS_STAT_OFFSET) /* System status register */
#define DA1470_CRG_TOP_VBUS_IRQ_CLEAR             (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_VBUS_IRQ_CLEAR_OFFSET) /* Clear pending IRQ register */
#define DA1470_CRG_TOP_VBUS_IRQ_MASK              (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_VBUS_IRQ_MASK_OFFSET) /* IRQ masking */
#define DA1470_CRG_TOP_WAKEUP_HIBERN              (DA1470_CRG_TOP_BASE + DA1470_CRG_TOP_WAKEUP_HIBERN_OFFSET) /* No description available. */

/* Register bit definitions *************************************************/


/* ANA_STATUS Register */

#define CRG_TOP_FLAG_LDO_V30_COMBINED_OK          (1U << 29) /* No description available. */
#define CRG_TOP_XOR_DOUT_WAKEUP_PADS              (1U << 28) /* Will be the result of XOR operation of the hibernation wakeup pads outputs combi */
#define CRG_TOP_VBUS_AVAILABLE                    (1U << 27) /* High when VBUS > ( VBAT + 150 mV). Hysteresis is approx. 40 mV */
#define CRG_TOP_FLAG_ADC_LDO_OK                   (1U << 26) /* When high, ldo_adc is active */
#define CRG_TOP_FLAG_IBIAS_TRIM                   (1U << 25) /* 10nA Iref trimming, high when on-chip current is larger than reference current */
#define CRG_TOP_BOD_VIN_NOK                       (1U << 24) /* General output of the BOD to indicate that one of the monitored inputs is below  */
#define CRG_TOP_BG_OK                             (1U << 23) /* When high bandgap is active */
#define CRG_TOP_BOOST_DCDC_VLED_OK                (1U << 22) /* When high, boost dcdc vled is active */
#define CRG_TOP_LDO_VSYS_HIGH_TEMP                (1U << 21) /* If 1 indicates that temperature of LDO_VSYS is above operating conditions */
#define CRG_TOP_VBAT_VSYS_STATE_MASK              (0x3 << 19) /* 0x0 : Hibernation mode or VBUS-only ( sysbat switch opened). 0x1 : Test mode ( s */
#define CRG_TOP_VBAT_VSYS_STATE_POS               (19) /* 0x0 : Hibernation mode or VBUS-only ( sysbat switch opened). 0x1 : Test mode ( s */
#define CRG_TOP_LDO_VSYS_HEAD_LIM                 (1U << 18) /* When high, the headroom loop is controlling VSYS */
#define CRG_TOP_LDO_VSYS_CURR_LIM                 (1U << 17) /* When high, the current limiter is controlling VSYS */
#define CRG_TOP_LDO_VSYS_LIM                      (1U << 16) /* When high, the voltage loop is controlling VSYS */
#define CRG_TOP_LDO_VSYS_OK                       (1U << 15) /* When high, LDO_VSYS is in regulating mode */
#define CRG_TOP_LDO_MIPI_OK                       (1U << 14) /* When high, ldo_mipi is active */
#define CRG_TOP_LDO_V30_OK                        (1U << 13) /* When high, ldo_v30 is active */
#define CRG_TOP_SWITCH_V18F_OK                    (1U << 12) /* V18F switch completely closed */
#define CRG_TOP_BUCK_DCDC_V18P_OK                 (1U << 11) /* V18P Rail ok based on DCDC V18P programmed level */
#define CRG_TOP_BUCK_DCDC_V18_OK                  (1U << 10) /* V18 Rail ok based on DCDC V18 programmed level */
#define CRG_TOP_BUCK_DCDC_V14_OK                  (1U << 9) /* V14 Rail ok based on DCDC V14 programmed level */
#define CRG_TOP_BUCK_DCDC_V12_OK                  (1U << 8) /* V12 Rail ok based on DCDC V12 programmed level */
#define CRG_TOP_COMP_VBUS_PLUGIN                  (1U << 7) /* VBUS is connected (VBUS > 2.5V) */
#define CRG_TOP_COMP_VSYS_NEAR_VLED               (1U << 6) /* BOOST_VLED_SEL=0x0 : VSYS>4.3 BOOST_VLED_SEL=0x1 : VSYS>4.55 BOOST_VLED_SEL=0x2  */
#define CRG_TOP_COMP_VBUS_ABOVE_VSYS              (1U << 5) /* VBUS>VSYS+0.05V */
#define CRG_TOP_COMP_VSYS_OK                      (1U << 4) /* VSYS> 2.45V */
#define CRG_TOP_COMP_VBAT_OK                      (1U << 3) /* VBAT> 2.7V */
#define CRG_TOP_COMP_VBUS_OK                      (1U << 2) /* 1: VBUS> 4.1V 0: VBUS<3.4V */
#define CRG_TOP_POR_VSYS_OK                       (1U << 1) /* No description available. */
#define CRG_TOP_POR_V30_OK                        (1U << 0) /* No description available. */

/* BANDGAP Register */

#define CRG_TOP_EN_BGR_TCCOMP                     (1U << 15) /* Enable Temperature compensation in the reference current for Charger and LED mod */
#define CRG_TOP_BANDGAP_ENABLE_CLAMP              (1U << 12) /* Enables a supply clamp inside the bandgap that improves PSRR. Should be enabled  */
#define CRG_TOP_BGR_TRIM_MASK                     (0x3F << 6) /* Trim register for bandgap */
#define CRG_TOP_BGR_TRIM_POS                      (6) /* Trim register for bandgap */
#define CRG_TOP_SYSRAM_LPMX                       (1U << 5) /* RAM Transparent Light Sleep (TLS) Core Enable for System RAMs and Cache RAM. Ass */
#define CRG_TOP_BGR_ITRIM_MASK                    (0x1F << 0) /* Current trimming for bias */
#define CRG_TOP_BGR_ITRIM_POS                     (0) /* Current trimming for bias */

/* BIAS_VREF_SEL Register */

#define CRG_TOP_BIAS_VREF_RF2_SEL_MASK            (0xF << 4) /* same coding as BIAS_VREF_RF1_SEL. */
#define CRG_TOP_BIAS_VREF_RF2_SEL_POS             (4) /* same coding as BIAS_VREF_RF1_SEL. */
#define CRG_TOP_BIAS_VREF_RF1_SEL_MASK            (0xF << 0) /* Vref_code | Vref_Voltage (mV) 0:900 1:930 2:960 3:990 4:1020 5:1050 6:1080 7:111 */
#define CRG_TOP_BIAS_VREF_RF1_SEL_POS             (0) /* Vref_code | Vref_Voltage (mV) 0:900 1:930 2:960 3:990 4:1020 5:1050 6:1080 7:111 */

/* BOD_CTRL Register */

#define CRG_TOP_BOD_VBUS_RST_EN                   (1U << 18) /* If set, generate power-on reset on channel VBUS */
#define CRG_TOP_BOD_VBAT_RST_EN                   (1U << 17) /* If set, generate power-on reset on channel VBAT. */
#define CRG_TOP_BOD_MIPI_RST_EN                   (1U << 16) /* If set, generate power-on reset on channel VMIPI */
#define CRG_TOP_BOD_VSYS_RST_EN                   (1U << 15) /* If set, generate power-on reset on channel VSYS. Bear in mind that there is an a */
#define CRG_TOP_BOD_V18F_RST_EN                   (1U << 14) /* If set, generate power-on reset on channel V18F */
#define CRG_TOP_BOD_V18P_RST_EN                   (1U << 13) /* If set, generate power-on reset on channel V18P */
#define CRG_TOP_BOD_V18_RST_EN                    (1U << 12) /* If set, generate power-on reset on channel V18 */
#define CRG_TOP_BOD_V14_RST_EN                    (1U << 11) /* If set, generate power-on reset on channel V14 */
#define CRG_TOP_BOD_V12_RST_EN                    (1U << 10) /* If set, generate power-on reset on channel V12 */
#define CRG_TOP_BOD_VBUS_EN                       (1U << 9) /* Enable brown-out detection for channel VBUS */
#define CRG_TOP_BOD_VBAT_EN                       (1U << 8) /* Enable brown-out detection for channel VBAT. */
#define CRG_TOP_BOD_MIPI_EN                       (1U << 7) /* Enable brown-out detection for channel VMIPI */
#define CRG_TOP_BOD_VSYS_EN                       (1U << 6) /* Enable brown-out detection for channel VSYS. Bear in mind that there is an addit */
#define CRG_TOP_BOD_V18F_EN                       (1U << 5) /* Enable brown-out detection for channel V18F */
#define CRG_TOP_BOD_V18P_EN                       (1U << 4) /* Enable brown-out detection for channel V18P */
#define CRG_TOP_BOD_V18_EN                        (1U << 3) /* Enable brown-out detection for channel V18 */
#define CRG_TOP_BOD_V14_EN                        (1U << 2) /* Enable brown-out detection for channel V14 */
#define CRG_TOP_BOD_V12_EN                        (1U << 1) /* Enable brown-out detection for channel VDD (V12) */
#define CRG_TOP_BOD_STATUS_CLEAR                  (1U << 0) /* Clears BOD_STATUS_REG when this bit is 1 for more than 2us. It must be 0 to regi */

/* BOD_STATUS Register */

#define CRG_TOP_BOD_VBUS                          (1U << 8) /* 1: below trigger level (BOD event) 0: above trigger level */
#define CRG_TOP_BOD_VBAT                          (1U << 7) /* 1: below trigger level (BOD event) 0: above trigger level */
#define CRG_TOP_BOD_VMIPI                         (1U << 6) /* 1: below trigger level (BOD event) 0: above trigger level */
#define CRG_TOP_BOD_VSYS                          (1U << 5) /* 1: below trigger level (BOD event) 0: above trigger level */
#define CRG_TOP_BOD_V18F                          (1U << 4) /* 1: below trigger level (BOD event) 0: above trigger level */
#define CRG_TOP_BOD_V18P                          (1U << 3) /* 1: below trigger level (BOD event) 0: above trigger level */
#define CRG_TOP_BOD_V18                           (1U << 2) /* 1: below trigger level (BOD event) 0: above trigger level */
#define CRG_TOP_BOD_V14                           (1U << 1) /* 1: below trigger level (BOD event) 0: above trigger level */
#define CRG_TOP_BOD_V12                           (1U << 0) /* 1: below trigger level (BOD event) 0: above trigger level */

/* CLK_AMBA Register */

#define CRG_TOP_OQSPI_PULLUP_ENABLE               (1U << 20) /* Selects pull value when OQSPIF_D* pads are not output. 0: The pads are pull-down */
#define CRG_TOP_OQSPI_GPIO_MODE                   (1U << 19) /* If this bit is set, the upper 4 pins of the OQSPIF controller can be used as GPI */
#define CRG_TOP_QSPIC2_ENABLE                     (1U << 18) /* Clock enable for QSPI RAM controller */
#define CRG_TOP_QSPIC2_DIV_MASK                   (0x3 << 16) /* Clock divider setting. 0b00: divide by 1 0b01: divide by 2 0b10: divide by 4 0b1 */
#define CRG_TOP_QSPIC2_DIV_POS                    (16) /* Clock divider setting. 0b00: divide by 1 0b01: divide by 2 0b10: divide by 4 0b1 */
#define CRG_TOP_QSPIC_ENABLE                      (1U << 15) /* Clock enable for QSPI FLASH2 controller */
#define CRG_TOP_QSPIC_DIV_MASK                    (0x3 << 13) /* Clock divider setting. 0b00: divide by 1 0b01: divide by 2 0b10: divide by 4 0b1 */
#define CRG_TOP_QSPIC_DIV_POS                     (13) /* Clock divider setting. 0b00: divide by 1 0b01: divide by 2 0b10: divide by 4 0b1 */
#define CRG_TOP_OQSPIF_ENABLE                     (1U << 12) /* Clock enable for Octal SPI controller */
#define CRG_TOP_OQSPIF_DIV_MASK                   (0x3 << 10) /* Clock divider setting. 0b00: divide by 1 0b01: divide by 2 0b10: divide by 4 0b1 */
#define CRG_TOP_OQSPIF_DIV_POS                    (10) /* Clock divider setting. 0b00: divide by 1 0b01: divide by 2 0b10: divide by 4 0b1 */
#define CRG_TOP_OTP_ENABLE                        (1U << 9) /* Clock enable for OTP controller */
#define CRG_TOP_AES_CLK_ENABLE                    (1U << 8) /* Clock enable for AES crypto block */
#define CRG_TOP_SLOW_PCLK_DIV_MASK                (0x7 << 5) /* Slow-APB interface clock, derived from DIVN_CLK: 0b000: divide divn_clk by 1 0b0 */
#define CRG_TOP_SLOW_PCLK_DIV_POS                 (5) /* Slow-APB interface clock, derived from DIVN_CLK: 0b000: divide divn_clk by 1 0b0 */
#define CRG_TOP_PCLK_DIV_MASK                     (0x3 << 3) /* Fast-APB interface clock, Cascaded with HCLK: 0b00: divide hclk by 1 0b01: divid */
#define CRG_TOP_PCLK_DIV_POS                      (3) /* Fast-APB interface clock, Cascaded with HCLK: 0b00: divide hclk by 1 0b01: divid */
#define CRG_TOP_HCLK_DIV_MASK                     (0x7 << 0) /* AHB interface and microprocessor clock. Source clock divided by: 0b000: divide c */
#define CRG_TOP_HCLK_DIV_POS                      (0) /* AHB interface and microprocessor clock. Source clock divided by: 0b000: divide c */

/* CLK_CMAC_SWITCH Register */

#define CRG_TOP_CMAC_RUNNING_ON_XTAL              (1U << 2) /* This bit is '1' when the CMAC_CLK is enabled, and the switch is set in the XTAL3 */
#define CRG_TOP_CMAC_RUNNING_ON_DIVN              (1U << 1) /* This bit is '1' when the CMAC_CLK is enabled, and the switch is set in the DIVN_ */
#define CRG_TOP_CMAC_CLK_SEL                      (1U << 0) /* Selects the clock source of the CMAC_CLK. 0: DIVN_CLK is selected 1: XTAL32M is  */

/* CLK_CTRL Register */

#define CRG_TOP_RUNNING_AT_PLL                    (1U << 15) /* Indicates that the PLL clock is used as clock, and may not be switched off */
#define CRG_TOP_RUNNING_AT_XTAL32M                (1U << 14) /* Indicates that the XTAL32M clock is used as clock, and may not be switched off */
#define CRG_TOP_RUNNING_AT_RCHS                   (1U << 13) /* Indicates that the RCHS clock is used as clock */
#define CRG_TOP_RUNNING_AT_RCLP                   (1U << 12) /* Indicates that the RCLP_CLK is being used as clock */
#define CRG_TOP_VAD_CLK_SEL                       (1U << 6) /* Selects the clock for the VAD. 0: Select RCLP clock (normalized for ~32KHz) 1: S */
#define CRG_TOP_LP_CLK_SEL_MASK                   (0x3 << 2) /* Sets the clock source of the LowerPower clock 0x0: RC32K 0x1: RCX 0x2: XTAL32K t */
#define CRG_TOP_LP_CLK_SEL_POS                    (2) /* Sets the clock source of the LowerPower clock 0x0: RC32K 0x1: RCX 0x2: XTAL32K t */
#define CRG_TOP_SYS_CLK_SEL_MASK                  (0x3 << 0) /* Selects the clock source. 0x0 : XTAL32M 0x1 : RCHS 0x2 : The Low Power clock is  */
#define CRG_TOP_SYS_CLK_SEL_POS                   (0) /* Selects the clock source. 0x0 : XTAL32M 0x1 : RCHS 0x2 : The Low Power clock is  */

/* CLK_RADIO Register */

#define CRG_TOP_RAD_REG_RESET_REQ                 (1U << 6) /* Reset request for registers of the radio PHY. */
#define CRG_TOP_RFCU_ENABLE                       (1U << 5) /* Enable the RF control Unit clock */
#define CRG_TOP_CMAC_SYNCH_RESET                  (1U << 4) /* Force synchronous reset to CMAC core and Sleep Timer. Its effective only when bo */
#define CRG_TOP_CMAC_CLK_ENABLE                   (1U << 2) /* Enables the clock */

/* CLK_RCHS Register */

#define CRG_TOP_RCHS_SPEED_MASK                   (0x3 << 22) /* Selects speed of RCHS output 0b00: 32MHz, by dividing 96 / 3. 0b01: 96MHz, by di */
#define CRG_TOP_RCHS_SPEED_POS                    (22) /* Selects speed of RCHS output 0b00: 32MHz, by dividing 96 / 3. 0b01: 96MHz, by di */
#define CRG_TOP_RCHS_INIT_RANGE_MASK              (0x3 << 20) /* Course frequency adjustment */
#define CRG_TOP_RCHS_INIT_RANGE_POS               (20) /* Course frequency adjustment */
#define CRG_TOP_RCHS_INIT_DEL_MASK                (0xFF << 12) /* Fine frequency adjustment */
#define CRG_TOP_RCHS_INIT_DEL_POS                 (12) /* Fine frequency adjustment */
#define CRG_TOP_RCHS_INIT_DTCF_MASK               (0x7 << 9) /* Fine duty-cycle adjustment. 0x0: minimum 0x2: default 0x4: maximum 0x5 until 0x7 */
#define CRG_TOP_RCHS_INIT_DTCF_POS                (9) /* Fine duty-cycle adjustment. 0x0: minimum 0x2: default 0x4: maximum 0x5 until 0x7 */
#define CRG_TOP_RCHS_INIT_DTC_MASK                (0xF << 5) /* Course duty-cycle adjustment. 0x0: minimum 0x5: default 0xA: maximum 0xB until 0 */
#define CRG_TOP_RCHS_INIT_DTC_POS                 (5) /* Course duty-cycle adjustment. 0x0: minimum 0x5: default 0xA: maximum 0xB until 0 */
#define CRG_TOP_RCHS_BIAS_MASK                    (0xF << 1) /* Bias adjustment */
#define CRG_TOP_RCHS_BIAS_POS                     (1) /* Bias adjustment */
#define CRG_TOP_RCHS_ENABLE                       (1U << 0) /* Enables the HighSpeed RC oscillator */

/* CLK_RCLP Register */

#define CRG_TOP_RCLP_LOW_SPEED_FORCE              (1U << 5) /* Forces RCLP in 32kHz mode. If this bit is 0 then RCLP frequency is 512KHz */
#define CRG_TOP_RCLP_TRIM_MASK                    (0xF << 1) /* 0000 = lowest frequency 0111 = default 1111 = highest frequency */
#define CRG_TOP_RCLP_TRIM_POS                     (1) /* 0000 = lowest frequency 0111 = default 1111 = highest frequency */
#define CRG_TOP_RCLP_ENABLE                       (1U << 0) /* Enables the 32kHz RC oscillator. This bit is gated to '0' automatically when sle */

/* CLK_RCX Register */

#define CRG_TOP_RCX_BIAS_MASK                     (0xF << 8) /* LDO bias current. 0x0: minimum 0xF: maximum */
#define CRG_TOP_RCX_BIAS_POS                      (8) /* LDO bias current. 0x0: minimum 0xF: maximum */
#define CRG_TOP_RCX_C0                            (1U << 7) /* Add unit capacitance to RC-time delay. */
#define CRG_TOP_RCX_CADJUST_MASK                  (0x1F << 2) /* Adjust capacitance part of RC-time delay. 0x00: minimum capacitance 0x1F: maximu */
#define CRG_TOP_RCX_CADJUST_POS                   (2) /* Adjust capacitance part of RC-time delay. 0x00: minimum capacitance 0x1F: maximu */
#define CRG_TOP_RCX_ENABLE                        (1U << 0) /* 0: Disable the RCX oscillator (watchdog runs at RCLP) 1: Enable the RCX oscillat */

/* CLK_RTCDIV Register */

#define CRG_TOP_RTC_RESET_REQ                     (1U << 21) /* Reset request for the RTC module */
#define CRG_TOP_RTC_DIV_ENABLE                    (1U << 20) /* Enable for the 100 Hz generation for the RTC block */
#define CRG_TOP_RTC_DIV_DENOM                     (1U << 19) /* Selects the denominator for the fractional division: 0b0: 1000 0b1: 1024 */
#define CRG_TOP_RTC_DIV_INT_MASK                  (0x1FF << 10) /* Integer divisor part for RTC 100Hz generation */
#define CRG_TOP_RTC_DIV_INT_POS                   (10) /* Integer divisor part for RTC 100Hz generation */
#define CRG_TOP_RTC_DIV_FRAC_MASK                 (0x3FF << 0) /* Fractional divisor part for RTC 100Hz generation. if RTC_DIV_DENOM=1, <RTC_DIV_F */
#define CRG_TOP_RTC_DIV_FRAC_POS                  (0) /* Fractional divisor part for RTC 100Hz generation. if RTC_DIV_DENOM=1, <RTC_DIV_F */

/* CLK_SNC_CTRL Register */

#define CRG_TOP_SNC_STATE_RETAINED                (1U << 2) /* A flag which can be used from FW to indicate that the CPU state has been retaine */
#define CRG_TOP_SNC_CLK_ENABLE                    (1U << 1) /* Clock-enable for the CM0plus in the SNC. */
#define CRG_TOP_SNC_RESET_REQ                     (1U << 0) /* Force the SNC microprocessor in reset. */

/* CLK_SWITCH2XTAL Register */

#define CRG_TOP_SWITCH2XTAL                       (1U << 0) /* When writing to this register, the clock switch will happen from RC32M to XTAL32 */

/* CLK_TMR Register */

#define CRG_TOP_TMR2_PWM_AON_MODE                 (1U << 2) /* Maps Timer2_pwm onto P1_17. This state is preserved during deep sleep, to allow  */
#define CRG_TOP_TMR_PWM_AON_MODE                  (1U << 1) /* Maps Timer1_pwm onto P1_00 This state is preserved during deep sleep, to allow P */
#define CRG_TOP_WAKEUPCT_ENABLE                   (1U << 0) /* Enables the clock */

/* CLK_XTAL32K Register */

#define CRG_TOP_XTAL32K_DISABLE_OUTPUT            (1U << 9) /* Disables output buffer, test only */
#define CRG_TOP_XTAL32K_DISABLE_AMPREG            (1U << 7) /* Setting this bit disables the amplitude regulation of the XTAL32kHz oscillator.  */
#define CRG_TOP_XTAL32K_CUR_MASK                  (0xF << 3) /* Bias current for the 32kHz XTAL oscillator. 0000 is minimum, 1111 is maximum, 00 */
#define CRG_TOP_XTAL32K_CUR_POS                   (3) /* Bias current for the 32kHz XTAL oscillator. 0000 is minimum, 1111 is maximum, 00 */
#define CRG_TOP_XTAL32K_RBIAS_MASK                (0x3 << 1) /* Setting for the bias resistor. 00 is maximum, 11 is minimum. Prefered setting wi */
#define CRG_TOP_XTAL32K_RBIAS_POS                 (1) /* Setting for the bias resistor. 00 is maximum, 11 is minimum. Prefered setting wi */
#define CRG_TOP_XTAL32K_ENABLE                    (1U << 0) /* Enables the 32kHz XTAL oscillator */

/* DISCHARGE_RAIL Register */

#define CRG_TOP_RESET_V30                         (1U << 4) /* 1: Enables immediate discharging of the V30 rail. Note that the source is not di */
#define CRG_TOP_RESET_VPOD                        (1U << 3) /* 1: Enables immediate discharging of the VPOD rail. Note that the source is not d */
#define CRG_TOP_RESET_V18P                        (1U << 2) /* 1: Enables immediate discharging of the V18P rail. Note that the source is not d */
#define CRG_TOP_RESET_V18                         (1U << 1) /* 1: Enables immediate discharging of the V18 rail. Note that the source is not di */
#define CRG_TOP_RESET_V14                         (1U << 0) /* 1: Enables immediate discharging of the V14 rail. Note that the source is not di */

/* LCD_EXT_CTRL Register */

#define CRG_TOP_LCD_EXT_CLK_EN                    (1U << 10) /* No description available. */
#define CRG_TOP_LCD_EXT_CNT_RELOAD_MASK           (0x3FF << 0) /* Reload value for LCD_EXT_CLK generation. When the counter hits 0x0, it is reload */
#define CRG_TOP_LCD_EXT_CNT_RELOAD_POS            (0) /* Reload value for LCD_EXT_CLK generation. When the counter hits 0x0, it is reload */

/* P0_PAD_LATCH Register */

#define CRG_TOP_P0_LATCH_EN_MASK                  (0xFFFFFFFF << 0) /* Direct write to the individual pad latching signals. Latches the control signals */
#define CRG_TOP_P0_LATCH_EN_POS                   (0) /* Direct write to the individual pad latching signals. Latches the control signals */

/* P0_RESET_PAD_LATCH Register */

#define CRG_TOP_P0_RESET_LATCH_EN_MASK            (0xFFFFFFFF << 0) /* Direct Reset of the marked bits. Reading returns 0x0. */
#define CRG_TOP_P0_RESET_LATCH_EN_POS             (0) /* Direct Reset of the marked bits. Reading returns 0x0. */

/* P0_SET_PAD_LATCH Register */

#define CRG_TOP_P0_SET_LATCH_EN_MASK              (0xFFFFFFFF << 0) /* Direct Set of the marked bits. Reading returns 0x0. */
#define CRG_TOP_P0_SET_LATCH_EN_POS               (0) /* Direct Set of the marked bits. Reading returns 0x0. */

/* P1_PAD_LATCH Register */

#define CRG_TOP_P1_LATCH_EN_MASK                  (0xFFFFFFFF << 0) /* Direct write to the individual pad latching signals. Latches the control signals */
#define CRG_TOP_P1_LATCH_EN_POS                   (0) /* Direct write to the individual pad latching signals. Latches the control signals */

/* P1_RESET_PAD_LATCH Register */

#define CRG_TOP_P1_RESET_LATCH_EN_MASK            (0xFFFFFFFF << 0) /* Direct Reset of the marked bits. Reading returns 0x0. */
#define CRG_TOP_P1_RESET_LATCH_EN_POS             (0) /* Direct Reset of the marked bits. Reading returns 0x0. */

/* P1_SET_PAD_LATCH Register */

#define CRG_TOP_P1_SET_LATCH_EN_MASK              (0xFFFFFFFF << 0) /* Direct Set of the marked bits. Reading returns 0x0. */
#define CRG_TOP_P1_SET_LATCH_EN_POS               (0) /* Direct Set of the marked bits. Reading returns 0x0. */

/* P2_PAD_LATCH Register */

#define CRG_TOP_P2_LATCH_EN_MASK                  (0x7FFF << 0) /* Direct write to the individual pad latching signals. Latches the control signals */
#define CRG_TOP_P2_LATCH_EN_POS                   (0) /* Direct write to the individual pad latching signals. Latches the control signals */

/* P2_RESET_PAD_LATCH Register */

#define CRG_TOP_P2_RESET_LATCH_EN_MASK            (0x7FFF << 0) /* Direct Reset of the marked bits. Reading returns 0x0. */
#define CRG_TOP_P2_RESET_LATCH_EN_POS             (0) /* Direct Reset of the marked bits. Reading returns 0x0. */

/* P2_SET_PAD_LATCH Register */

#define CRG_TOP_P2_SET_LATCH_EN_MASK              (0x7FFF << 0) /* Direct Set of the marked bits. Reading returns 0x0. */
#define CRG_TOP_P2_SET_LATCH_EN_POS               (0) /* Direct Set of the marked bits. Reading returns 0x0. */

/* PMU_CTRL Register */

#define CRG_TOP_RETAIN_RGP_RAM                    (1U << 13) /* Retain the R-G-B RAMs inside the LCD display controller */
#define CRG_TOP_RETAIN_GPU_CLUT                   (1U << 12) /* Retain the GPU CLUT memory */
#define CRG_TOP_RETAIN_DCACHE                     (1U << 11) /* Selects the retainability of the dcache block while PD_CTRL is off. '1' is retai */
#define CRG_TOP_GPU_SLEEP                         (1U << 10) /* Put the GPU power domain (PD_GPU) in powerdown */
#define CRG_TOP_CTRL_SLEEP                        (1U << 9) /* Put the Controller power domain (PD_CTRL) in powerdown */
#define CRG_TOP_ENABLE_CLKLESS                    (1U << 8) /* Selects the clockless sleep mode. Wakeup is done asynchronously. When set to '1' */
#define CRG_TOP_RETAIN_CACHE                      (1U << 7) /* Selects the retainability of the cache block during deep sleep. '1' is retainabl */
#define CRG_TOP_SYS_SLEEP                         (1U << 6) /* Put the System powerdomain (PD_SYS) in powerdown. If this bit is '1', and there  */
#define CRG_TOP_RESET_ON_WAKEUP                   (1U << 5) /* Perform a Hardware Reset after waking up. Booter will be started. */
#define CRG_TOP_SNC_SLEEP                         (1U << 3) /* Put the Communications powerdomain (PD_SNC) in powerdown */
#define CRG_TOP_TIM_SLEEP                         (1U << 2) /* Put the Timers Powerdomain (PD_TIM) in powerdown. */
#define CRG_TOP_RADIO_SLEEP                       (1U << 1) /* Put the digital part of the radio, including CMAC (PD_RAD) in powerdown */
#define CRG_TOP_AUD_SLEEP                         (1U << 0) /* Put the audio power domain (PD_AUD) in powerdown */

/* PMU_SLEEP Register */

#define CRG_TOP_ULTRA_FAST_WAKEUP                 (1U << 31) /* Allows the core to start running on the RC32M while the PMU is still waiting for */
#define CRG_TOP_ENABLE_FAST_SWITCH                (1U << 30) /* Enables early clock switching upon event detection to speed up wakeup time */
#define CRG_TOP_VLED_BYPASS_REFRESH_TIME_MASK     (0x7F << 23) /* This setting determines how long the VLED bypass switch is closed when VSYS near */
#define CRG_TOP_VLED_BYPASS_REFRESH_TIME_POS      (23) /* This setting determines how long the VLED bypass switch is closed when VSYS near */
#define CRG_TOP_BOD_SLEEP_INTERVAL_MASK           (0xF << 19) /* "This is the interval at which the BOD comparators/POR will be checked during sl */
#define CRG_TOP_BOD_SLEEP_INTERVAL_POS            (19) /* "This is the interval at which the BOD comparators/POR will be checked during sl */
#define CRG_TOP_RAILS_REFRESH_INTERVAL_MASK       (0x3FFF << 5) /* This is the interval at which the power rails reference voltage will be refreshe */
#define CRG_TOP_RAILS_REFRESH_INTERVAL_POS        (5) /* This is the interval at which the power rails reference voltage will be refreshe */
#define CRG_TOP_BASE_REFRESH_INTERVAL_MASK        (0x1F << 0) /* This sets the base time for calculating the intervals at which PMU refreshes dur */
#define CRG_TOP_BASE_REFRESH_INTERVAL_POS         (0) /* This sets the base time for calculating the intervals at which PMU refreshes dur */

/* PMU_TRIM Register */

#define CRG_TOP_IBIAS_10N_TRIM_MASK               (0x7 << 8) /* No description available. */
#define CRG_TOP_IBIAS_10N_TRIM_POS                (8) /* No description available. */
#define CRG_TOP_LDO_RET_V30_TRIM_MASK             (0xF << 4) /* No description available. */
#define CRG_TOP_LDO_RET_V30_TRIM_POS              (4) /* No description available. */
#define CRG_TOP_LDO_V30_TRIM_MASK                 (0xF << 0) /* No description available. */
#define CRG_TOP_LDO_V30_TRIM_POS                  (0) /* No description available. */

/* POR_CTRL Register */

#define CRG_TOP_POR_VSYS_SLEEP_CYCLE_EN           (1U << 11) /* Enables POR_VSYS during BOD check cycles in sleep (If POR_VSYS_DISABLE = 0) */
#define CRG_TOP_POR_VSYS_HYST_SEL                 (1U << 10) /* "Selects POR_VSYS threshold level, when hysteresis is disabled (see POR_VSYS_DIS */
#define CRG_TOP_POR_VSYS_HYST_DISABLE             (1U << 9) /* Disable POR_VSYS hysteresis. */
#define CRG_TOP_POR_VSYS_FORCE_ON                 (1U << 8) /* FORCE POR_VSYS to be ON (also in SLEEP) */
#define CRG_TOP_POR_VSYS_MASK                     (1U << 7) /* Mask POR on VSYS */
#define CRG_TOP_POR_VSYS_DISABLE                  (1U << 6) /* Disable POR_VSYS */
#define CRG_TOP_POR_V30_SLEEP_CYCLE_EN            (1U << 5) /* Enables POR_V30 during BOD check cycles in sleep (If POR_V30_DISABLE = 0) */
#define CRG_TOP_POR_V30_HYST_SEL                  (1U << 4) /* Selects POR_VDDA_3V0 threshold level, when hysteresis is disabled (see POR_VDDA_ */
#define CRG_TOP_POR_V30_HYST_DISABLE              (1U << 3) /* Disable POR_VDDA_3V0 hysteresis; select level with POR_VDDA_3V0_HYST_SEL */
#define CRG_TOP_POR_V30_FORCE_ON                  (1U << 2) /* Force POR_VDDA_3V0 always ON, (also in SLEEP) */
#define CRG_TOP_POR_V30_MASK                      (1U << 1) /* Mask POR_VDDA_3V0 */
#define CRG_TOP_POR_V30_DISABLE                   (1U << 0) /* Disable POR_VDDA_3V0 */

/* POR_PIN Register */

#define CRG_TOP_POR_PIN_POLARITY                  (1U << 7) /* 0: Active Low 1: Active High Note: This applies only for the GPIO pin. Reset pad */
#define CRG_TOP_POR_PIN_SELECT_MASK               (0x7F << 0) /* 0x00: P0_00 ... 0x1f: P0_31 0x20: P1_00 ... 0x3F: P1_31 0x40: P2_00 ... 0x4E: P2 */
#define CRG_TOP_POR_PIN_SELECT_POS                (0) /* 0x00: P0_00 ... 0x1f: P0_31 0x20: P1_00 ... 0x3F: P1_31 0x40: P2_00 ... 0x4E: P2 */

/* POR_TIMER Register */

#define CRG_TOP_POR_TIME_MASK                     (0x7F << 0) /* Time for the POReset to happen. Formula: Time = POR_TIME x 4096 x RC32 clock per */
#define CRG_TOP_POR_TIME_POS                      (0) /* Time for the POReset to happen. Formula: Time = POR_TIME x 4096 x RC32 clock per */

/* POWER_CTRL Register */

#define CRG_TOP_SW_V18F_SLEEP_ON                  (1U << 19) /* Closes the V18F switch in sleep when DCDC_V18P_SLEEP_EN is 1. See SW_V18F regist */
#define CRG_TOP_SW_V18F_ON                        (1U << 18) /* Closes the V18F switch when DCDC_V18P_EN is 1. See SW_V18F register for more set */
#define CRG_TOP_DCDC_VLED_SLEEP_EN                (1U << 17) /* Enables boost dcdc led rail in sleep mode */
#define CRG_TOP_DCDC_VLED_EN                      (1U << 16) /* Enables boost dcdc led rail in active mode */
#define CRG_TOP_DCDC_V18P_SLEEP_EN                (1U << 15) /* Enables buck dcdc V18p rail in sleep mode */
#define CRG_TOP_DCDC_V18P_EN                      (1U << 14) /* Enables buck dcdc V18p rail in active mode */
#define CRG_TOP_DCDC_V18_SLEEP_EN                 (1U << 13) /* Enables buck dcdc V18 rail in sleep mode */
#define CRG_TOP_DCDC_V18_EN                       (1U << 12) /* Enables buck dcdc V18 rail in active mode */
#define CRG_TOP_DCDC_V14_SLEEP_EN                 (1U << 11) /* Enables buck dcdc V14 rail in sleep mode */
#define CRG_TOP_DCDC_V14_EN                       (1U << 10) /* Enables buck dcdc V14 rail in active mode */
#define CRG_TOP_DCDC_V12_SLEEP_EN                 (1U << 9) /* Enables buck dcdc V12 rail in sleep mode */
#define CRG_TOP_DCDC_V12_EN                       (1U << 8) /* Enables buck dcdc V12 rail in active mode */
#define CRG_TOP_CLAMP_V12_DIS                     (1U << 7) /* Disables V12 clamp. During Hibernation, V12 clamp will be enabled regardless of  */
#define CRG_TOP_CLAMP_V30_EN                      (1U << 6) /* Enables V30 clamp. */
#define CRG_TOP_LDO_MIPI_EN                       (1U << 5) /* Enables ldo MIPI when DCDC_V18P_EN is 1 */
#define CRG_TOP_LDO_RET_V30_SLEEP_EN              (1U << 4) /* Enables ldo V30 ret in sleep mode */
#define CRG_TOP_LDO_RET_V30_EN                    (1U << 3) /* Enables ldo V30 ret in active mode */
#define CRG_TOP_LDO_V30_SLEEP_EN                  (1U << 2) /* Enables ldo V30 in sleep mode */
#define CRG_TOP_LDO_V30_EN                        (1U << 1) /* Enables ldo V30 in active mode */
#define CRG_TOP_LDO_START_EN                      (1U << 0) /* Enables ldo start. */

/* POWER_LVL Register */

#define CRG_TOP_VSYS_LEVEL_MASK                   (0x3 << 17) /* Level setting for VSYS rail when ldo_vsys is enabled (COMP_VBUS_OK & COMP_VBUS_A */
#define CRG_TOP_VSYS_LEVEL_POS                    (17) /* Level setting for VSYS rail when ldo_vsys is enabled (COMP_VBUS_OK & COMP_VBUS_A */
#define CRG_TOP_V18_LEVEL                         (1U << 16) /* Level setting for V18 rail: 0=1.8V 1=1.2V */
#define CRG_TOP_V14_LEVEL_MASK                    (0x3 << 14) /* Level setting for V14 rail: 0=1.2V 1=1.3V 2=1.4V 3=1.5V */
#define CRG_TOP_V14_LEVEL_POS                     (14) /* Level setting for V14 rail: 0=1.2V 1=1.3V 2=1.4V 3=1.5V */
#define CRG_TOP_V12_SLEEP_LEVEL_MASK              (0x3 << 12) /* Level setting for V12 rail in sleep: 0=0.75V 1=0.9V 2=1.2V 3=reserved */
#define CRG_TOP_V12_SLEEP_LEVEL_POS               (12) /* Level setting for V12 rail in sleep: 0=0.75V 1=0.9V 2=1.2V 3=reserved */
#define CRG_TOP_V12_LEVEL_MASK                    (0x3 << 10) /* Level setting for V12 rail: 0=0.75V 1=0.9V 2=1.2V 3=reserved */
#define CRG_TOP_V12_LEVEL_POS                     (10) /* Level setting for V12 rail: 0=0.75V 1=0.9V 2=1.2V 3=reserved */
#define CRG_TOP_V30_SLEEP_LEVEL_MASK              (0x3 << 8) /* Level setting for V30 in sleep: 0x0: 3.0V 0x1: reserved 0x2: 3.3V 0x3: 3.3V */
#define CRG_TOP_V30_SLEEP_LEVEL_POS               (8) /* Level setting for V30 in sleep: 0x0: 3.0V 0x1: reserved 0x2: 3.3V 0x3: 3.3V */
#define CRG_TOP_V30_LEVEL_MASK                    (0x3 << 6) /* Level setting for V30: 0x0: 3.0V 0x1: reserved 0x2: 3.3V 0x3: 3.3V */
#define CRG_TOP_V30_LEVEL_POS                     (6) /* Level setting for V30: 0x0: 3.0V 0x1: reserved 0x2: 3.3V 0x3: 3.3V */
#define CRG_TOP_VMIPI_LEVEL_MASK                  (0x7 << 3) /* LDO MIPI level: 0.9V+0.05V*LDO_MIPI_LEVEL. Default=1.2V */
#define CRG_TOP_VMIPI_LEVEL_POS                   (3) /* LDO MIPI level: 0.9V+0.05V*LDO_MIPI_LEVEL. Default=1.2V */
#define CRG_TOP_CLAMP_V12_LEVEL_MASK              (0x7 << 0) /* Level setting for V12 clamp, retained in hibernation (only V12 source in Hiberna */
#define CRG_TOP_CLAMP_V12_LEVEL_POS               (0) /* Level setting for V12 clamp, retained in hibernation (only V12 source in Hiberna */

/* RAM_PWR_CTRL Register */

#define CRG_TOP_RAM13_PWR_CTRL_MASK               (0x3 << 26) /* See description of RAM0_PWR_CTRL. */
#define CRG_TOP_RAM13_PWR_CTRL_POS                (26) /* See description of RAM0_PWR_CTRL. */
#define CRG_TOP_RAM12_PWR_CTRL_MASK               (0x3 << 24) /* See description of RAM0_PWR_CTRL. */
#define CRG_TOP_RAM12_PWR_CTRL_POS                (24) /* See description of RAM0_PWR_CTRL. */
#define CRG_TOP_RAM11_PWR_CTRL_MASK               (0x3 << 22) /* See description of RAM0_PWR_CTRL. */
#define CRG_TOP_RAM11_PWR_CTRL_POS                (22) /* See description of RAM0_PWR_CTRL. */
#define CRG_TOP_RAM10_PWR_CTRL_MASK               (0x3 << 20) /* See description of RAM0_PWR_CTRL. */
#define CRG_TOP_RAM10_PWR_CTRL_POS                (20) /* See description of RAM0_PWR_CTRL. */
#define CRG_TOP_RAM9_PWR_CTRL_MASK                (0x3 << 18) /* See description of RAM0_PWR_CTRL. */
#define CRG_TOP_RAM9_PWR_CTRL_POS                 (18) /* See description of RAM0_PWR_CTRL. */
#define CRG_TOP_RAM8_PWR_CTRL_MASK                (0x3 << 16) /* See description of RAM0_PWR_CTRL. */
#define CRG_TOP_RAM8_PWR_CTRL_POS                 (16) /* See description of RAM0_PWR_CTRL. */
#define CRG_TOP_RAM7_PWR_CTRL_MASK                (0x3 << 14) /* See description of RAM0_PWR_CTRL. */
#define CRG_TOP_RAM7_PWR_CTRL_POS                 (14) /* See description of RAM0_PWR_CTRL. */
#define CRG_TOP_RAM6_PWR_CTRL_MASK                (0x3 << 12) /* See description of RAM0_PWR_CTRL. */
#define CRG_TOP_RAM6_PWR_CTRL_POS                 (12) /* See description of RAM0_PWR_CTRL. */
#define CRG_TOP_RAM5_PWR_CTRL_MASK                (0x3 << 10) /* See description of RAM0_PWR_CTRL. */
#define CRG_TOP_RAM5_PWR_CTRL_POS                 (10) /* See description of RAM0_PWR_CTRL. */
#define CRG_TOP_RAM4_PWR_CTRL_MASK                (0x3 << 8) /* See description of RAM0_PWR_CTRL. */
#define CRG_TOP_RAM4_PWR_CTRL_POS                 (8) /* See description of RAM0_PWR_CTRL. */
#define CRG_TOP_RAM3_PWR_CTRL_MASK                (0x3 << 6) /* See description of RAM0_PWR_CTRL. */
#define CRG_TOP_RAM3_PWR_CTRL_POS                 (6) /* See description of RAM0_PWR_CTRL. */
#define CRG_TOP_RAM2_PWR_CTRL_MASK                (0x3 << 4) /* See description of RAM0_PWR_CTRL. */
#define CRG_TOP_RAM2_PWR_CTRL_POS                 (4) /* See description of RAM0_PWR_CTRL. */
#define CRG_TOP_RAM1_PWR_CTRL_MASK                (0x3 << 2) /* See description of RAM0_PWR_CTRL. */
#define CRG_TOP_RAM1_PWR_CTRL_POS                 (2) /* See description of RAM0_PWR_CTRL. */
#define CRG_TOP_RAM0_PWR_CTRL_MASK                (0x3 << 0) /* Power state control of the individual RAMs. May only change when the memory isn' */
#define CRG_TOP_RAM0_PWR_CTRL_POS                 (0) /* Power state control of the individual RAMs. May only change when the memory isn' */

/* RESET_STAT Register */

#define CRG_TOP_SNC_WDOGRESET_STAT                (1U << 6) /* Indicates that a SNC-Watchdog timeout has happened. Note that it is also set whe */
#define CRG_TOP_CMAC_WDOGRESET_STAT               (1U << 5) /* Indicates that a CMAC-Watchdog timeout has happened. Note that it is also set wh */
#define CRG_TOP_SWD_HWRESET_STAT                  (1U << 4) /* Indicates that a write to SWD_RESET_REG has happened. Note that it is also set w */
#define CRG_TOP_WDOGRESET_STAT                    (1U << 3) /* Indicates that a Watchdog timeout has happened. Note that it is also set when a  */
#define CRG_TOP_SWRESET_STAT                      (1U << 2) /* Indicates that a SW Reset has happened */
#define CRG_TOP_HWRESET_STAT                      (1U << 1) /* Indicates that a HW Reset has happened */
#define CRG_TOP_PORESET_STAT                      (1U << 0) /* Indicates that a PowerOn Reset has happened. All bitfields of RESET_STAT_REG sho */

/* RST_CTRL Register */

#define CRG_TOP_SYS_CACHE_FLUSH_WITH_SW_RESET     (1U << 0) /* 0: Flush the System Cache memory only at HW reset. 1: Flush the System Cache mem */

/* SECURE_BOOT Register */

#define CRG_TOP_PROT_OTP_CS_WRITE                 (1U << 9) /* This bit will permanentlly disable any write action to the CS inside the OTP */
#define CRG_TOP_FORCE_SNC_DEBUGGER_OFF            (1U << 8) /* This bit will permanently disable the SNC debugger */
#define CRG_TOP_PROT_OQSPIF_KEY_READ              (1U << 7) /* This bit will permanently disable CPU read capability at OTP offset 0x00000B00 a */
#define CRG_TOP_PROT_OQSPIF_KEY_WRITE             (1U << 6) /* This bit will permanently disable ANY write capability at OTP offset 0x00000B00  */
#define CRG_TOP_PROT_AES_KEY_READ                 (1U << 5) /* This bit will permanently disable CPU read capability at OTP offset 0x00000A00 a */
#define CRG_TOP_PROT_AES_KEY_WRITE                (1U << 4) /* This bit will permanently disable ANY write capability at OTP offset 0x00000A00  */
#define CRG_TOP_PROT_SIG_KEY_WRITE                (1U << 3) /* This bit will permanently disable ANY write capability at OTP offset 0x000008C0  */
#define CRG_TOP_FORCE_CMAC_DEBUGGER_OFF           (1U << 2) /* This bit will permanently disable the CMAC debugger */
#define CRG_TOP_FORCE_DEBUGGER_OFF                (1U << 1) /* Follows the respective OTP flag value. Its value is updated by the BootROM code. */
#define CRG_TOP_SECURE_BOOT                       (1U << 0) /* Follows the respective OTP flag value. Its value is updated by the BootROM code. */

/* SLP_MAP Register */

#define CRG_TOP_LCD_INV_EXT_CLK_SLP_MAP           (1U << 8) /* Maps inverted LCD_EXT_CLK on P0_10, for LCD XFRP function This state is preserve */
#define CRG_TOP_LCD_EXT_CLK_SLP_MAP               (1U << 7) /* Maps LCD_EXT_CLK on P0_19, for LCD VCOM/FRP/EXTCOMIN function This state is pres */
#define CRG_TOP_BANDGAP_SLP_MAP                   (1U << 6) /* Setting this bit will:  -map bandgap_enable to P0_13 -map (wokenup OR cmac_slp_t */
#define CRG_TOP_RCLP_SLP_MAP                      (1U << 5) /* Maps RCLP onto P1_23. This state is preserved during deep sleep, to allow pin ou */
#define CRG_TOP_XTAL32K_SLP_MAP                   (1U << 4) /* Maps XTA32k onto P0_31. This state is preserved during deep sleep, to allow pin  */
#define CRG_TOP_RCX_SLP_MAP                       (1U << 3) /* Maps RCX onto P1_22. This state is preserved during deep sleep, to allow pin out */
#define CRG_TOP_TMR4_PWM_SLP_MAP                  (1U << 2) /* Maps Timer4_pwm onto P1_31 This state is preserved during deep sleep, to allow p */
#define CRG_TOP_TMR3_PWM_SLP_MAP                  (1U << 1) /* Maps Timer3_pwm onto P1_30 This state is preserved during deep sleep, to allow p */
#define CRG_TOP_TMR_PWM_SLP_MAP                   (1U << 0) /* Maps Timer1_pwm onto P0_30 This state is preserved during deep sleep, to allow p */

/* SW_V18F Register */

#define CRG_TOP_DELAY_TRIM_MASK                   (0x3 << 2) /* Soft start delay trim */
#define CRG_TOP_DELAY_TRIM_POS                    (2) /* Soft start delay trim */
#define CRG_TOP_SKIP_SOFT_START                   (1U << 1) /* Skip soft start routine */
#define CRG_TOP_FORCE_SW_ON                       (1U << 0) /* Forces closing sw_V18f, independent of v18p state */

/* SYS_CTRL Register */

#define CRG_TOP_SW_RESET                          (1U << 15) /* Writing a '1' to this bit will generate a SW_RESET. */
#define CRG_TOP_CACHERAM_MUX                      (1U << 10) /* Controls accessiblity of Cache RAM: 0: the cache controller is bypassed, the cac */
#define CRG_TOP_TIMEOUT_DISABLE                   (1U << 9) /* Disables timeout in Power statemachine. By default, the statemachine continues i */
#define CRG_TOP_DEBUGGER_ENABLE                   (1U << 7) /* Enable the debugger. This bit is set by the booter according to the OTP header.  */
#define CRG_TOP_SNC_DEBUGGER_ENABLE               (1U << 5) /* Enable the CMAC debugger. If not set, the SWDIO and SW_CLK can be used as gpio p */
#define CRG_TOP_REMAP_INTVECT                     (1U << 3) /* 0: normal operation 1: If ARM is in address range 0 to 0x1FF then the address is */
#define CRG_TOP_REMAP_ADR0_MASK                   (0x7 << 0) /* Controls which memory is located at address 0x0000 for execution. 0x0: ROM 0x1:  */
#define CRG_TOP_REMAP_ADR0_POS                    (0) /* Controls which memory is located at address 0x0000 for execution. 0x0: ROM 0x1:  */

/* SYS_STAT Register */

#define CRG_TOP_GPU_IS_UP                         (1U << 17) /* Indicates that PD_GPU is functional */
#define CRG_TOP_GPU_IS_DOWN                       (1U << 16) /* Indicates that PD_GPU is in power down */
#define CRG_TOP_CTRL_IS_UP                        (1U << 15) /* Indicates that PD_CTRL is functional */
#define CRG_TOP_CTRL_IS_DOWN                      (1U << 14) /* Indicates that PD_CTRL is in power down */
#define CRG_TOP_POWER_IS_UP                       (1U << 13) /* Indicates that the Startup statemachine is finished, and all power regulation is */
#define CRG_TOP_DBG_IS_ACTIVE                     (1U << 12) /* Indicates that a debugger is attached. */
#define CRG_TOP_SNC_IS_UP                         (1U << 11) /* Indicates that PD_SNC is functional */
#define CRG_TOP_SNC_IS_DOWN                       (1U << 10) /* Indicates that PD_SNC is in power down */
#define CRG_TOP_TIM_IS_UP                         (1U << 9) /* Indicates that PD_TIM is functional */
#define CRG_TOP_TIM_IS_DOWN                       (1U << 8) /* Indicates that PD_TIM is in power down */
#define CRG_TOP_MEM_IS_UP                         (1U << 7) /* Indicates that PD_MEM is functional */
#define CRG_TOP_MEM_IS_DOWN                       (1U << 6) /* Indicates that PD_MEM is in power down */
#define CRG_TOP_SYS_IS_UP                         (1U << 5) /* Indicates that PD_SYS is functional */
#define CRG_TOP_SYS_IS_DOWN                       (1U << 4) /* Indicates that PD_SYS is in power down */
#define CRG_TOP_AUD_IS_UP                         (1U << 3) /* Indicates that PD_AUD is functional */
#define CRG_TOP_AUD_IS_DOWN                       (1U << 2) /* Indicates that PD_AUD is in power down */
#define CRG_TOP_RAD_IS_UP                         (1U << 1) /* Indicates that PD_RAD is functional */
#define CRG_TOP_RAD_IS_DOWN                       (1U << 0) /* Indicates that PD_RAD is in power down */

/* VBUS_IRQ_CLEAR Register */

#define CRG_TOP_VBUS_IRQ_CLEAR_MASK               (0xFFFF << 0) /* Writing any value to this register will reset the VBUS_IRQ line */
#define CRG_TOP_VBUS_IRQ_CLEAR_POS                (0) /* Writing any value to this register will reset the VBUS_IRQ line */

/* VBUS_IRQ_MASK Register */

#define CRG_TOP_VBUS_WAKEUP_CLKLESS               (1U << 2) /* Enables waking up from CLKLESS mode when VBUS becomes available. For the system  */
#define CRG_TOP_VBUS_IRQ_EN_RISE                  (1U << 1) /* Setting this bit to '1' enables VBUS_IRQ generation when the VBUS starts to ramp */
#define CRG_TOP_VBUS_IRQ_EN_FALL                  (1U << 0) /* Setting this bit to '1' enables VBUS_IRQ generation when the VBUS starts to fall */

/* WAKEUP_HIBERN Register */

#define CRG_TOP_HIBERNATION_ENABLE                (1U << 12) /* Enable hibernation mode */
#define CRG_TOP_WAKEUP_PD_EN_MASK                 (0xF << 6) /* Enables pulldown for GPIO[n] during hibernation Bit 0: P0_20 Bit 1: P0_29 Bit 2: */
#define CRG_TOP_WAKEUP_PD_EN_POS                  (6) /* Enables pulldown for GPIO[n] during hibernation Bit 0: P0_20 Bit 1: P0_29 Bit 2: */
#define CRG_TOP_WAKEUP_EN_MASK                    (0xF << 0) /* Enables GPIO[n] to wake up from hibernation Bit 0: P0_20 Bit 1: P0_29 Bit 2: P1_ */
#define CRG_TOP_WAKEUP_EN_POS                     (0) /* Enables GPIO[n] to wake up from hibernation Bit 0: P0_20 Bit 1: P0_29 Bit 2: P1_ */

#endif /* __DA1470_CRG_TOP_H */
