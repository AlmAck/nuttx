/****************************************************************************
 * arch/arm/include/nrf53/nrf5340_irq_cpuapp.h
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

#ifndef __ARCH_ARM_INCLUDE_NRF53_NRF5340_IRQ_CPUAPP_H
#define __ARCH_ARM_INCLUDE_NRF53_NRF5340_IRQ_CPUAPP_H

/****************************************************************************
 * Pre-processor Prototypes
 ****************************************************************************/

/* Cortex-M33 External interrupts (vectors >= 16) */

/* Application core */

#define DA1470X_IRQ_CMAC2SYS         (DA1470X_IRQ_EXTINT+0) /* CMAC to System interrupt */
#define DA1470X_IRQ_SNC2SYS          (17) /* SNC M0+ interrupt to Cortex M33 */
#define DA1470X_IRQ_CACHE_MRM        (18) /* Cache MRM interrupt */
#define DA1470X_IRQ_PDC              (19) /* Peripheral domain controller interrupt */
#define DA1470X_IRQ_KEY_WKUP         (20) /* Key wake-up GPIO interrupt */
#define DA1470X_IRQ_VBUS             (21) /* VBUS presence interrupt */
#define DA1470X_IRQ_CHARGER_STATE    (22) /* Charger state change interrupt */
#define DA1470X_IRQ_CHARGER_ERROR    (23) /* Charger error interrupt */
#define DA1470X_IRQ_DCDC_BOOST       (24) /* DCDC Boost timeout interrupt */
#define DA1470X_IRQ_PLL48_LOCK       (25) /* PLL48 locked at 48 MHz */
#define DA1470X_IRQ_CRYPTO           (26) /* Crypto function interrupt */
#define DA1470X_IRQ_PLL_LOCK         (27) /* PLL locked at 160 MHz */
#define DA1470X_IRQ_XTAL32M_READY    (28) /* XTAL32M oscillator ready */
#define DA1470X_IRQ_RFDIAG           (29) /* Radio and baseband diagnostics interrupt */
#define DA1470X_IRQ_GPIO_P0          (30) /* GPIO Port 0 interrupt */
#define DA1470X_IRQ_GPIO_P1          (31) /* GPIO Port 1 interrupt */
#define DA1470X_IRQ_GPIO_P2          (32) /* GPIO Port 2 interrupt */
#define DA1470X_IRQ_TIMER            (33) /* Timer interrupt */
#define DA1470X_IRQ_TIMER2           (34) /* Timer2 interrupt */
#define DA1470X_IRQ_TIMER3           (35) /* Timer3 interrupt */
#define DA1470X_IRQ_TIMER4           (36) /* Timer4 interrupt */
#define DA1470X_IRQ_TIMER5           (37) /* Timer5 interrupt */
#define DA1470X_IRQ_TIMER6           (38) /* Timer6 interrupt */
#define DA1470X_IRQ_RTC              (39) /* Real-Time Clock interrupt */
#define DA1470X_IRQ_RTC_EVENT        (40) /* RTC event interrupt */
#define DA1470X_IRQ_CAPTIMER         (41) /* Capture timer interrupt */
#define DA1470X_IRQ_ADC              (42) /* General-purpose ADC interrupt */
#define DA1470X_IRQ_ADC2             (43) /* Application ADC interrupt */
#define DA1470X_IRQ_DMA              (44) /* DMA interrupt */
#define DA1470X_IRQ_UART             (45) /* UART interrupt */
#define DA1470X_IRQ_UART2            (46) /* UART2 interrupt */
#define DA1470X_IRQ_UART3            (47) /* UART3 interrupt */
#define DA1470X_IRQ_SPI              (48) /* SPI interrupt */
#define DA1470X_IRQ_SPI2             (49) /* SPI2 interrupt */
#define DA1470X_IRQ_SPI3             (50) /* SPI3 interrupt */
#define DA1470X_IRQ_I2C              (51) /* I2C interrupt */
#define DA1470X_IRQ_I2C2             (52) /* I2C2 interrupt */
#define DA1470X_IRQ_I2C3             (53) /* I2C3 interrupt */
#define DA1470X_IRQ_I3C              (54) /* I3C interrupt */
#define DA1470X_IRQ_USB              (55) /* USB interrupt */
#define DA1470X_IRQ_PCM              (56) /* PCM interrupt */
#define DA1470X_IRQ_SRC_IN           (57) /* SRC input interrupt */
#define DA1470X_IRQ_SRC_OUT          (58) /* SRC output interrupt */
#define DA1470X_IRQ_SRC2_IN          (59) /* SRC2 input interrupt */
#define DA1470X_IRQ_SRC2_OUT         (60) /* SRC2 output interrupt */
#define DA1470X_IRQ_VAD              (61) /* Voice Activity Detection (VAD) interrupt */
#define DA1470X_IRQ_EMMC             (62) /* eMMC controller interrupt */
#define DA1470X_IRQ_GPU              (63) /* GPU interrupt */
#define DA1470X_IRQ_LCD              (64) /* Display controller interrupt */
#define DA1470X_IRQ_CHARGER_DET      (65) /* Charger detection interrupt */
#define DA1470X_IRQ_DCACHE_MRM       (66) /* Data cache MRM interrupt */
#define DA1470X_IRQ_CLK_CALIBRATION  (67) /* Clock calibration interrupt */
#define DA1470X_IRQ_VSYS_GEN         (68) /* VSYS generator interrupt */

#define DA1470X_IRQ_NEXTINT          (69) /* Total number of defined interrupts */

#define DA1470X_IRQ_NIRQS         (DA1470X_IRQ_EXTINT+DA1470X_IRQ_NEXTINT)

/* Total number of IRQ numbers */

#define NR_IRQS                 DA1470X_IRQ_NIRQS

#endif /* __ARCH_ARM_INCLUDE_NRF53_NRF5340_IRQ_CPUAPP_H */
