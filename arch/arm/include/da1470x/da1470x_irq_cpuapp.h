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

#ifndef __ARCH_ARM_INCLUDE_DA1470X_DA1470X_IRQ_CPUAPP_H
#define __ARCH_ARM_INCLUDE_DA1470X_DA1470X_IRQ_CPUAPP_H

/****************************************************************************
 * Pre-processor Prototypes
 ****************************************************************************/

/* Cortex-M33 External interrupts (vectors >= 16) */

/* Application core */

/* External interrupts (vectors >= 16) for DA1470x, per Renesas IRQn_Type enum */
#define DA1470X_IRQ_CMAC2SYS         (DA1470X_IRQ_EXTINT+0)   /* CMAC to System interrupt */
#define DA1470X_IRQ_SNC2SYS          (DA1470X_IRQ_EXTINT+1)   /* SNC M0+ interrupt to Cortex M33 */
#define DA1470X_IRQ_CACHE_MRM        (DA1470X_IRQ_EXTINT+2)   /* Cache MRM interrupt */
#define DA1470X_IRQ_PDC              (DA1470X_IRQ_EXTINT+3)   /* Peripheral domain controller interrupt */
#define DA1470X_IRQ_KEY_WKUP         (DA1470X_IRQ_EXTINT+4)   /* Key wake-up GPIO interrupt */
#define DA1470X_IRQ_VBUS             (DA1470X_IRQ_EXTINT+5)   /* VBUS presence interrupt */
#define DA1470X_IRQ_CHARGER_STATE    (DA1470X_IRQ_EXTINT+6)   /* Charger state change interrupt */
#define DA1470X_IRQ_CHARGER_ERROR    (DA1470X_IRQ_EXTINT+7)   /* Charger error interrupt */
#define DA1470X_IRQ_DCDC_BOOST       (DA1470X_IRQ_EXTINT+8)   /* DCDC boost controller interrupt */
#define DA1470X_IRQ_PLL48_LOCK       (DA1470X_IRQ_EXTINT+9)   /* PLL48 locked at 48 MHz */
#define DA1470X_IRQ_CRYPTO           (DA1470X_IRQ_EXTINT+10)  /* Crypto function interrupt */
#define DA1470X_IRQ_PLL_LOCK         (DA1470X_IRQ_EXTINT+11)  /* PLL locked at 160 MHz */
#define DA1470X_IRQ_XTAL32M_READY    (DA1470X_IRQ_EXTINT+12)  /* XTAL32M oscillator ready */
#define DA1470X_IRQ_RFDIAG           (DA1470X_IRQ_EXTINT+13)  /* Radio/baseband diagnostics interrupt */
#define DA1470X_IRQ_GPIO_P0          (DA1470X_IRQ_EXTINT+14)  /* GPIO Port 0 interrupt */
#define DA1470X_IRQ_GPIO_P1          (DA1470X_IRQ_EXTINT+15)  /* GPIO Port 1 interrupt */
#define DA1470X_IRQ_GPIO_P2          (DA1470X_IRQ_EXTINT+16)  /* GPIO Port 2 interrupt */
#define DA1470X_IRQ_TIMER            (DA1470X_IRQ_EXTINT+17)  /* TIMER interrupt */
#define DA1470X_IRQ_TIMER2           (DA1470X_IRQ_EXTINT+18)  /* TIMER2 interrupt */
#define DA1470X_IRQ_TIMER3           (DA1470X_IRQ_EXTINT+19)  /* TIMER3 interrupt */
#define DA1470X_IRQ_TIMER4           (DA1470X_IRQ_EXTINT+20)  /* TIMER4 interrupt */
#define DA1470X_IRQ_TIMER5           (DA1470X_IRQ_EXTINT+21)  /* TIMER5 interrupt */
#define DA1470X_IRQ_TIMER6           (DA1470X_IRQ_EXTINT+22)  /* TIMER6 interrupt */
#define DA1470X_IRQ_RTC              (DA1470X_IRQ_EXTINT+23)  /* Real-Time Clock interrupt */
#define DA1470X_IRQ_RTC_EVENT        (DA1470X_IRQ_EXTINT+24)  /* RTC event interrupt */
#define DA1470X_IRQ_CAPTIMER         (DA1470X_IRQ_EXTINT+25)  /* Capture timer interrupt */
#define DA1470X_IRQ_ADC              (DA1470X_IRQ_EXTINT+26)  /* General-purpose ADC interrupt */
#define DA1470X_IRQ_ADC2             (DA1470X_IRQ_EXTINT+27)  /* Application ADC interrupt */
#define DA1470X_IRQ_DMA              (DA1470X_IRQ_EXTINT+28)  /* DMA interrupt */
#define DA1470X_IRQ_UART0            (DA1470X_IRQ_EXTINT+29)  /* UART0 interrupt */
#define DA1470X_IRQ_UART1            (DA1470X_IRQ_EXTINT+30)  /* UART1 interrupt */
#define DA1470X_IRQ_UART2            (DA1470X_IRQ_EXTINT+31)  /* UART2 interrupt */
#define DA1470X_IRQ_SPI              (DA1470X_IRQ_EXTINT+32)  /* SPI interrupt */
#define DA1470X_IRQ_SPI2             (DA1470X_IRQ_EXTINT+33)  /* SPI2 interrupt */
#define DA1470X_IRQ_SPI3             (DA1470X_IRQ_EXTINT+34)  /* SPI3 interrupt */
#define DA1470X_IRQ_I2C              (DA1470X_IRQ_EXTINT+35)  /* I2C interrupt */
#define DA1470X_IRQ_I2C2             (DA1470X_IRQ_EXTINT+36)  /* I2C2 interrupt */
#define DA1470X_IRQ_I2C3             (DA1470X_IRQ_EXTINT+37)  /* I2C3 interrupt */
#define DA1470X_IRQ_I3C              (DA1470X_IRQ_EXTINT+38)  /* I3C interrupt */
#define DA1470X_IRQ_USB              (DA1470X_IRQ_EXTINT+39)  /* USB interrupt */
#define DA1470X_IRQ_PCM              (DA1470X_IRQ_EXTINT+40)  /* PCM interrupt */
#define DA1470X_IRQ_SRC_IN           (DA1470X_IRQ_EXTINT+41)  /* SRC input interrupt */
#define DA1470X_IRQ_SRC_OUT          (DA1470X_IRQ_EXTINT+42)  /* SRC output interrupt */
#define DA1470X_IRQ_SRC2_IN          (DA1470X_IRQ_EXTINT+43)  /* SRC2 input interrupt */
#define DA1470X_IRQ_SRC2_OUT         (DA1470X_IRQ_EXTINT+44)  /* SRC2 output interrupt */
#define DA1470X_IRQ_VAD              (DA1470X_IRQ_EXTINT+45)  /* Voice Activity Detection interrupt */
#define DA1470X_IRQ_EMMC             (DA1470X_IRQ_EXTINT+46)  /* eMMC controller interrupt */
#define DA1470X_IRQ_SDIO             (DA1470X_IRQ_EXTINT+47)  /* SDIO controller interrupt */
#define DA1470X_IRQ_GPU              (DA1470X_IRQ_EXTINT+48)  /* GPU interrupt */
#define DA1470X_IRQ_LCD              (DA1470X_IRQ_EXTINT+49)  /* Display controller interrupt */
#define DA1470X_IRQ_DSI              (DA1470X_IRQ_EXTINT+50)  /* DSI/D-PHY interrupt */
#define DA1470X_IRQ_CHARGER_DET      (DA1470X_IRQ_EXTINT+51)  /* Charger detection interrupt */
#define DA1470X_IRQ_DCACHE_MRM       (DA1470X_IRQ_EXTINT+52)  /* Data cache MRM interrupt */
#define DA1470X_IRQ_CLK_CALIBRATION  (DA1470X_IRQ_EXTINT+53)  /* Clock calibration interrupt */
#define DA1470X_IRQ_VSYS_GEN         (DA1470X_IRQ_EXTINT+54)  /* VSYS generator interrupt */

#define DA1470X_IRQ_NEXTINT          (55)                    /* Total number of defined external interrupts */

#define DA1470X_IRQ_NIRQS            (DA1470X_IRQ_EXTINT+DA1470X_IRQ_NEXTINT)

/* Total number of IRQ numbers */

#define NR_IRQS                 DA1470X_IRQ_NIRQS

#endif /* __ARCH_ARM_INCLUDE_NDA1470X_DA1470X_IRQ_CPUAPP_H */
