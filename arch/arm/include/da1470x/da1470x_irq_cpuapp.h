/****************************************************************************
 * arch/arm/include/da1470x/da1470x_irq_cpuapp.h
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

/* This file should never be included directly but, rather, only indirectly
 * through arch/irq.h
 */

#ifndef __ARCH_ARM_INCLUDE_DA1470X_DA1470X_IRQ_CPUAPP_H
#define __ARCH_ARM_INCLUDE_DA1470X_DA1470X_IRQ_CPUAPP_H

/****************************************************************************
 * Pre-processor Prototypes
 ****************************************************************************/

/* Cortex-M33 external interrupts (vectors >= 16), application core.
 * Numbering follows the Renesas IRQn_Type enumeration.
 */

#define DA1470X_IRQ_CMAC2SYS        (DA1470X_IRQ_EXTINT + 0)  /* CMAC to M33 */
#define DA1470X_IRQ_SNC2SYS         (DA1470X_IRQ_EXTINT + 1)  /* SNC to M33 */
#define DA1470X_IRQ_CACHE_MRM       (DA1470X_IRQ_EXTINT + 2)  /* Cache MRM */
#define DA1470X_IRQ_PDC             (DA1470X_IRQ_EXTINT + 3)  /* PDC wake-up */
#define DA1470X_IRQ_KEY_WKUP        (DA1470X_IRQ_EXTINT + 4)  /* Debounced key wake-up */
#define DA1470X_IRQ_VBUS            (DA1470X_IRQ_EXTINT + 5)  /* VBUS present */
#define DA1470X_IRQ_CHARGER_STATE   (DA1470X_IRQ_EXTINT + 6)  /* Charger state */
#define DA1470X_IRQ_CHARGER_ERROR   (DA1470X_IRQ_EXTINT + 7)  /* Charger error */
#define DA1470X_IRQ_DCDC_BOOST      (DA1470X_IRQ_EXTINT + 8)  /* Boost converter */
#define DA1470X_IRQ_PLL48_LOCK      (DA1470X_IRQ_EXTINT + 9)  /* USB PLL lock */
#define DA1470X_IRQ_CRYPTO          (DA1470X_IRQ_EXTINT + 10) /* AES/HASH */
#define DA1470X_IRQ_PLL_LOCK        (DA1470X_IRQ_EXTINT + 11) /* System PLL lock */
#define DA1470X_IRQ_XTAL32M_READY   (DA1470X_IRQ_EXTINT + 12) /* XTAL32M settled */
#define DA1470X_IRQ_RFDIAG          (DA1470X_IRQ_EXTINT + 13) /* Radio diagnostics */
#define DA1470X_IRQ_GPIO_P0         (DA1470X_IRQ_EXTINT + 14) /* GPIO port 0 */
#define DA1470X_IRQ_GPIO_P1         (DA1470X_IRQ_EXTINT + 15) /* GPIO port 1 */
#define DA1470X_IRQ_GPIO_P2         (DA1470X_IRQ_EXTINT + 16) /* GPIO port 2 */
#define DA1470X_IRQ_TIMER           (DA1470X_IRQ_EXTINT + 17) /* TIMER */
#define DA1470X_IRQ_TIMER2          (DA1470X_IRQ_EXTINT + 18) /* TIMER2 */
#define DA1470X_IRQ_TIMER3          (DA1470X_IRQ_EXTINT + 19) /* TIMER3 */
#define DA1470X_IRQ_TIMER4          (DA1470X_IRQ_EXTINT + 20) /* TIMER4 */
#define DA1470X_IRQ_TIMER5          (DA1470X_IRQ_EXTINT + 21) /* TIMER5 */
#define DA1470X_IRQ_TIMER6          (DA1470X_IRQ_EXTINT + 22) /* TIMER6 */
#define DA1470X_IRQ_RTC             (DA1470X_IRQ_EXTINT + 23) /* RTC alarm/roll */
#define DA1470X_IRQ_RTC_EVENT       (DA1470X_IRQ_EXTINT + 24) /* RTC event */
#define DA1470X_IRQ_CAPTIMER        (DA1470X_IRQ_EXTINT + 25) /* Capture timer */
#define DA1470X_IRQ_ADC             (DA1470X_IRQ_EXTINT + 26) /* GP ADC */
#define DA1470X_IRQ_ADC2            (DA1470X_IRQ_EXTINT + 27) /* SD ADC */
#define DA1470X_IRQ_DMA             (DA1470X_IRQ_EXTINT + 28) /* DMA */
#define DA1470X_IRQ_UART0           (DA1470X_IRQ_EXTINT + 29) /* UART */
#define DA1470X_IRQ_UART1           (DA1470X_IRQ_EXTINT + 30) /* UART2 */
#define DA1470X_IRQ_UART2           (DA1470X_IRQ_EXTINT + 31) /* UART3 */
#define DA1470X_IRQ_SPI0            (DA1470X_IRQ_EXTINT + 32) /* SPI */
#define DA1470X_IRQ_SPI1            (DA1470X_IRQ_EXTINT + 33) /* SPI2 */
#define DA1470X_IRQ_SPI2            (DA1470X_IRQ_EXTINT + 34) /* SPI3 */
#define DA1470X_IRQ_I2C0            (DA1470X_IRQ_EXTINT + 35) /* I2C */
#define DA1470X_IRQ_I2C1            (DA1470X_IRQ_EXTINT + 36) /* I2C2 */
#define DA1470X_IRQ_I2C2            (DA1470X_IRQ_EXTINT + 37) /* I2C3 */
#define DA1470X_IRQ_I3C             (DA1470X_IRQ_EXTINT + 38) /* I3C */
#define DA1470X_IRQ_USB             (DA1470X_IRQ_EXTINT + 39) /* USB */
#define DA1470X_IRQ_PCM             (DA1470X_IRQ_EXTINT + 40) /* PCM */
#define DA1470X_IRQ_SRC_IN          (DA1470X_IRQ_EXTINT + 41) /* SRC1 input */
#define DA1470X_IRQ_SRC_OUT         (DA1470X_IRQ_EXTINT + 42) /* SRC1 output */
#define DA1470X_IRQ_SRC2_IN         (DA1470X_IRQ_EXTINT + 43) /* SRC2 input */
#define DA1470X_IRQ_SRC2_OUT        (DA1470X_IRQ_EXTINT + 44) /* SRC2 output */
#define DA1470X_IRQ_VAD             (DA1470X_IRQ_EXTINT + 45) /* Voice activity */
#define DA1470X_IRQ_EMMC            (DA1470X_IRQ_EXTINT + 46) /* eMMC */
#define DA1470X_IRQ_SDIO            (DA1470X_IRQ_EXTINT + 47) /* SDIO */
#define DA1470X_IRQ_GPU             (DA1470X_IRQ_EXTINT + 48) /* GPU */
#define DA1470X_IRQ_LCD             (DA1470X_IRQ_EXTINT + 49) /* LCD controller */
#define DA1470X_IRQ_DSI             (DA1470X_IRQ_EXTINT + 50) /* MIPI DSI */
#define DA1470X_IRQ_CHARGER_DET     (DA1470X_IRQ_EXTINT + 51) /* Charger detect */
#define DA1470X_IRQ_DCACHE_MRM      (DA1470X_IRQ_EXTINT + 52) /* Data cache MRM */
#define DA1470X_IRQ_CLK_CALIBRATION (DA1470X_IRQ_EXTINT + 53) /* Clock calibration */
#define DA1470X_IRQ_VSYS_GEN        (DA1470X_IRQ_EXTINT + 54) /* VSYS generator */
#define DA1470X_IRQ_SWIRQ           (DA1470X_IRQ_EXTINT + 55) /* Software IRQ */

#define DA1470X_IRQ_NEXTINT         (56)
#define DA1470X_IRQ_NIRQS           (DA1470X_IRQ_EXTINT + DA1470X_IRQ_NEXTINT)

/* Total number of IRQ numbers */

#define NR_IRQS                     DA1470X_IRQ_NIRQS

#endif /* __ARCH_ARM_INCLUDE_DA1470X_DA1470X_IRQ_CPUAPP_H */
