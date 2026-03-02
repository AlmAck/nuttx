/****************************************************************************
 * arch/arm/src/da1470x/da1470x_irq.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_IRQ_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_IRQ_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Cortex-M33 External interrupts (vectors >= 16) */

#define DA1470X_IRQ_EXTINT                                                     \
  (16) /* Vector number of the first external interrupt */

/* M33 Interrupt Controller (Datasheet Table 97) */

#define DA1470X_IRQ_CMAC2SYS (DA1470X_IRQ_EXTINT + 0)
#define DA1470X_IRQ_SNC2SYS (DA1470X_IRQ_EXTINT + 1)
#define DA1470X_IRQ_M33CACHE_MRM (DA1470X_IRQ_EXTINT + 2)
#define DA1470X_IRQ_PDC_M33 (DA1470X_IRQ_EXTINT + 3)
#define DA1470X_IRQ_KEY_WKUP_GPIO (DA1470X_IRQ_EXTINT + 4)
#define DA1470X_IRQ_VBUS (DA1470X_IRQ_EXTINT + 5)
#define DA1470X_IRQ_CHARGER_STATE (DA1470X_IRQ_EXTINT + 6)
#define DA1470X_IRQ_CHARGER_ERROR (DA1470X_IRQ_EXTINT + 7)
#define DA1470X_IRQ_DCDC_BOOST (DA1470X_IRQ_EXTINT + 8)
#define DA1470X_IRQ_PLL48_LOCK (DA1470X_IRQ_EXTINT + 9)
#define DA1470X_IRQ_CRYPTO (DA1470X_IRQ_EXTINT + 10)
#define DA1470X_IRQ_PLL_LOCK (DA1470X_IRQ_EXTINT + 11)
#define DA1470X_IRQ_XTAL32MDRY (DA1470X_IRQ_EXTINT + 12)
#define DA1470X_IRQ_RFDIAG (DA1470X_IRQ_EXTINT + 13)
#define DA1470X_IRQ_GPIO_P0 (DA1470X_IRQ_EXTINT + 14)
#define DA1470X_IRQ_GPIO_P1 (DA1470X_IRQ_EXTINT + 15)
#define DA1470X_IRQ_GPIO_P2 (DA1470X_IRQ_EXTINT + 16)
#define DA1470X_IRQ_TIMER (DA1470X_IRQ_EXTINT + 17)
#define DA1470X_IRQ_TIMER2 (DA1470X_IRQ_EXTINT + 18)
#define DA1470X_IRQ_TIMER3 (DA1470X_IRQ_EXTINT + 19)
#define DA1470X_IRQ_TIMER4 (DA1470X_IRQ_EXTINT + 20)
#define DA1470X_IRQ_TIMER5 (DA1470X_IRQ_EXTINT + 21)
#define DA1470X_IRQ_TIMER6 (DA1470X_IRQ_EXTINT + 22)
#define DA1470X_IRQ_RTC (DA1470X_IRQ_EXTINT + 23)
#define DA1470X_IRQ_RTC_EVENT (DA1470X_IRQ_EXTINT + 24)
#define DA1470X_IRQ_CAPTIMER (DA1470X_IRQ_EXTINT + 25)
#define DA1470X_IRQ_ADC (DA1470X_IRQ_EXTINT + 26)
#define DA1470X_IRQ_ADC2 (DA1470X_IRQ_EXTINT + 27)
#define DA1470X_IRQ_DMA (DA1470X_IRQ_EXTINT + 28)
#define DA1470X_IRQ_UART (DA1470X_IRQ_EXTINT + 29)
#define DA1470X_IRQ_UART2 (DA1470X_IRQ_EXTINT + 30)
#define DA1470X_IRQ_UART3 (DA1470X_IRQ_EXTINT + 31)
#define DA1470X_IRQ_SPI (DA1470X_IRQ_EXTINT + 32)
#define DA1470X_IRQ_SPI2 (DA1470X_IRQ_EXTINT + 33)
#define DA1470X_IRQ_SPI3 (DA1470X_IRQ_EXTINT + 34)
#define DA1470X_IRQ_I2C (DA1470X_IRQ_EXTINT + 35)
#define DA1470X_IRQ_I2C2 (DA1470X_IRQ_EXTINT + 36)
#define DA1470X_IRQ_I2C3 (DA1470X_IRQ_EXTINT + 37)
#define DA1470X_IRQ_I3C (DA1470X_IRQ_EXTINT + 38)
#define DA1470X_IRQ_USB (DA1470X_IRQ_EXTINT + 39)
#define DA1470X_IRQ_PCM (DA1470X_IRQ_EXTINT + 40)
#define DA1470X_IRQ_SRC_IN (DA1470X_IRQ_EXTINT + 41)
#define DA1470X_IRQ_SRC_OUT (DA1470X_IRQ_EXTINT + 42)
#define DA1470X_IRQ_SRC2_IN (DA1470X_IRQ_EXTINT + 43)
#define DA1470X_IRQ_SRC2_OUT (DA1470X_IRQ_EXTINT + 44)
#define DA1470X_IRQ_VAD (DA1470X_IRQ_EXTINT + 45)
#define DA1470X_IRQ_EMMC (DA1470X_IRQ_EXTINT + 46)
// Reserved 47
#define DA1470X_IRQ_GPU (DA1470X_IRQ_EXTINT + 48)
#define DA1470X_IRQ_LCD (DA1470X_IRQ_EXTINT + 49)
// Reserved 50
#define DA1470X_IRQ_CHARGER_DET (DA1470X_IRQ_EXTINT + 51)
#define DA1470X_IRQ_DCACHE_MRM (DA1470X_IRQ_EXTINT + 52)
#define DA1470X_IRQ_CLK_CALIBRATION (DA1470X_IRQ_EXTINT + 53)
#define DA1470X_IRQ_VSYS_GEN (DA1470X_IRQ_EXTINT + 54)

#define DA1470X_IRQ_NEXTINT (55)

/* Mappings for consistency with logic drivers */
#define DA1470X_IRQ_UART0 DA1470X_IRQ_UART
#define DA1470X_IRQ_UART1 DA1470X_IRQ_UART2
#define DA1470X_IRQ_UART2 DA1470X_IRQ_UART3

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_clrpend
 *
 * Description:
 *   Clear a pending interrupt at the NVIC.  This does not seem to be
 *   required for most interrupts.
 *
 ****************************************************************************/

void da1470x_clrpend(int irq);

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_IRQ_H */
