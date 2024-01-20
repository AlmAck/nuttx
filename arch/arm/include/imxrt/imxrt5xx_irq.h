/****************************************************************************
 * arch/arm/include/imxrt/imxrt5xx_irq.h
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

/* This file should never be included directly but, rather,
 * only indirectly through nuttx/irq.h
 */

#ifndef __ARCH_ARM_INCLUDE_IMXRT_IMXRT5XX_IRQ_H
#define __ARCH_ARM_INCLUDE_IMXRT_IMXRT5XX_IRQ_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

/****************************************************************************
 * Pre-processor Prototypes
 ****************************************************************************/

/* External interrupts (priority levels >= 90) ******************************/

#define IMXRT_IRQ_WDT0                    (IMXRT_IRQ_EXTINT + 0)   /* Watchdog timer interrupt */
#define IMXRT_IRQ_DMA0                    (IMXRT_IRQ_EXTINT + 1)   /* DMA interrupt */
#define IMXRT_IRQ_GPIO_INTA               (IMXRT_IRQ_EXTINT + 2)   /* GPIO Interrupt A */
#define IMXRT_IRQ_GPIO_INTB               (IMXRT_IRQ_EXTINT + 3)   /* GPIO Interrupt B */
#define IMXRT_IRQ_PIN_INT0                (IMXRT_IRQ_EXTINT + 4)   /* General Purpose Input/Output interrupt 0 */
#define IMXRT_IRQ_PIN_INT1                (IMXRT_IRQ_EXTINT + 5)   /* General Purpose Input/Output interrupt 1 */
#define IMXRT_IRQ_PIN_INT2                (IMXRT_IRQ_EXTINT + 6)   /* General Purpose Input/Output interrupt 2 */
#define IMXRT_IRQ_PIN_INT3                (IMXRT_IRQ_EXTINT + 7)   /* General Purpose Input/Output interrupt 3 */
#define IMXRT_IRQ_UTICK0                  (IMXRT_IRQ_EXTINT + 8)   /* Micro-tick Timer */
#define IMXRT_IRQ_MRT0                    (IMXRT_IRQ_EXTINT + 9)   /* Multi-Rate Timer */
#define IMXRT_IRQ_CTIMER0                 (IMXRT_IRQ_EXTINT + 10)  /* Standard counter/timer CTIMER0 */
#define IMXRT_IRQ_CTIMER1                 (IMXRT_IRQ_EXTINT + 11)  /* Standard counter/timer CTIMER1 */
#define IMXRT_IRQ_SCT0                    (IMXRT_IRQ_EXTINT + 12)  /* SCTimer/PWM */
#define IMXRT_IRQ_CTIMER3                 (IMXRT_IRQ_EXTINT + 13)  /* Standard counter/timer CTIMER3 */
#define IMXRT_IRQ_FLEXCOMM0               (IMXRT_IRQ_EXTINT + 14)  /* FlexComm interrupt */
#define IMXRT_IRQ_FLEXCOMM1               (IMXRT_IRQ_EXTINT + 15)  /* FlexComm interrupt */
#define IMXRT_IRQ_FLEXCOMM2               (IMXRT_IRQ_EXTINT + 16)  /* FlexComm interrupt */
#define IMXRT_IRQ_FLEXCOMM3               (IMXRT_IRQ_EXTINT + 17)  /* FlexComm interrupt */
#define IMXRT_IRQ_FLEXCOMM4               (IMXRT_IRQ_EXTINT + 18)  /* FlexComm interrupt */
#define IMXRT_IRQ_FLEXCOMM5               (IMXRT_IRQ_EXTINT + 19)  /* FlexComm interrupt */
#define IMXRT_IRQ_FLEXCOMM14              (IMXRT_IRQ_EXTINT + 20)  /* FlexComm interrupt. Standalone SPI */
#define IMXRT_IRQ_FLEXCOMM15              (IMXRT_IRQ_EXTINT + 21)  /* FlexComm interrupt. Standalone I2C */
#define IMXRT_IRQ_ADC0                    (IMXRT_IRQ_EXTINT + 22)  /* Analog-to-Digital Converter interrupt */
#define IMXRT_IRQ_Reserved39              (IMXRT_IRQ_EXTINT + 23)  /* Reserved interrupt */
#define IMXRT_IRQ_ACMP                    (IMXRT_IRQ_EXTINT + 24)  /* Analog comparator Interrupts */
#define IMXRT_IRQ_DMIC0                   (IMXRT_IRQ_EXTINT + 25)  /* Digital Microphone Interface interrupt */
#define IMXRT_IRQ_Reserved42              (IMXRT_IRQ_EXTINT + 26)  /* Reserved interrupt */
#define IMXRT_IRQ_HYPERVISOR              (IMXRT_IRQ_EXTINT + 27)  /* Hypervisor interrupt */
#define IMXRT_IRQ_SECURE_VIOLATION        (IMXRT_IRQ_EXTINT + 28)  /* Secure violation interrupt */
#define IMXRT_IRQ_HWVAD0                  (IMXRT_IRQ_EXTINT + 29)  /* Hardware Voice Activity Detector interrupt */
#define IMXRT_IRQ_Reserved46              (IMXRT_IRQ_EXTINT + 30)  /* Reserved interrupt */
#define IMXRT_IRQ_RNG                     (IMXRT_IRQ_EXTINT + 31)  /* Random Number Generator interrupt */
#define IMXRT_IRQ_RTC                     (IMXRT_IRQ_EXTINT + 32)  /* Real Time Clock Alarm interrupt OR Wakeup timer interrupt */
#define IMXRT_IRQ_DSP_TIE_EXPSTATE1       (IMXRT_IRQ_EXTINT + 33)  /* DSP interrupt */
#define IMXRT_IRQ_MU_A                    (IMXRT_IRQ_EXTINT + 34)  /* Messaging Unit - Side A */
#define IMXRT_IRQ_PIN_INT4                (IMXRT_IRQ_EXTINT + 35)  /* General Purpose Input/Output interrupt 4 */
#define IMXRT_IRQ_PIN_INT5                (IMXRT_IRQ_EXTINT + 36)  /* General Purpose Input/Output interrupt 5 */
#define IMXRT_IRQ_PIN_INT6                (IMXRT_IRQ_EXTINT + 37)  /* General Purpose Input/Output interrupt 6 */
#define IMXRT_IRQ_PIN_INT7                (IMXRT_IRQ_EXTINT + 38)  /* General Purpose Input/Output interrupt 7 */
#define IMXRT_IRQ_CTIMER2                 (IMXRT_IRQ_EXTINT + 39)  /* Standard counter/timer CTIMER2 */
#define IMXRT_IRQ_CTIMER4                 (IMXRT_IRQ_EXTINT + 40)  /* Standard counter/timer CTIMER4 */
#define IMXRT_IRQ_OS_EVENT                (IMXRT_IRQ_EXTINT + 41)  /* Event timer M33 Wakeup/interrupt */
#define IMXRT_IRQ_FLEXSPI0_FLEXSPI1       (IMXRT_IRQ_EXTINT + 42)  /* FlexSPI0_IRQ OR FlexSPI1_IRQ */
#define IMXRT_IRQ_FLEXCOMM6               (IMXRT_IRQ_EXTINT + 43)  /* FlexComm interrupt */
#define IMXRT_IRQ_FLEXCOMM7               (IMXRT_IRQ_EXTINT + 44)  /* FlexComm interrupt */
#define IMXRT_IRQ_USDHC0                  (IMXRT_IRQ_EXTINT + 45)  /* USDHC interrupt */
#define IMXRT_IRQ_USDHC1                  (IMXRT_IRQ_EXTINT + 46)  /* USDHC interrupt */
#define IMXRT_IRQ_SGPIO_INTA              (IMXRT_IRQ_EXTINT + 47)  /* Secure GPIO HS interrupt 0 */
#define IMXRT_IRQ_SGPIO_INTB              (IMXRT_IRQ_EXTINT + 48)  /* Secure GPIO HS interrupt 1 */
#define IMXRT_IRQ_I3C0                    (IMXRT_IRQ_EXTINT + 49)  /* Improved Inter Integrated Circuit 0 interrupt */
#define IMXRT_IRQ_USB0                    (IMXRT_IRQ_EXTINT + 50)  /* USB device */
#define IMXRT_IRQ_USB0_NEEDCLK            (IMXRT_IRQ_EXTINT + 51)  /* USB Activity Wake-up Interrupt */
#define IMXRT_IRQ_WDT1                    (IMXRT_IRQ_EXTINT + 52)  /* Watchdog timer 1 interrupt */
#define IMXRT_IRQ_USB_PHYDCD              (IMXRT_IRQ_EXTINT + 53)  /* USBPHY DCD interrupt */
#define IMXRT_IRQ_DMA1                    (IMXRT_IRQ_EXTINT + 54)  /* DMA interrupt */
#define IMXRT_IRQ_PUF                     (IMXRT_IRQ_EXTINT + 55)  /* QuidKey interrupt */
#define IMXRT_IRQ_POWERQUAD               (IMXRT_IRQ_EXTINT + 56)  /* Powerquad interrupt */
#define IMXRT_IRQ_CASPER                  (IMXRT_IRQ_EXTINT + 57)  /* Caspar interrupt */
#define IMXRT_IRQ_PMU_PMIC                (IMXRT_IRQ_EXTINT + 58)  /* Power Management Control interrupt */
#define IMXRT_IRQ_HASHCRYPT               (IMXRT_IRQ_EXTINT + 59)  /* SHA interrupt */
#define IMXRT_IRQ_FLEXCOMM8               (IMXRT_IRQ_EXTINT + 60)  /* FlexComm interrupt */
#define IMXRT_IRQ_FLEXCOMM9               (IMXRT_IRQ_EXTINT + 61)  /* FlexComm interrupt */
#define IMXRT_IRQ_FLEXCOMM10              (IMXRT_IRQ_EXTINT + 62)  /* FlexComm interrupt */
#define IMXRT_IRQ_FLEXCOMM11              (IMXRT_IRQ_EXTINT + 63)  /* FlexComm interrupt */
#define IMXRT_IRQ_FLEXCOMM12              (IMXRT_IRQ_EXTINT + 64)  /* FlexComm interrupt */
#define IMXRT_IRQ_FLEXCOMM13              (IMXRT_IRQ_EXTINT + 65)  /* FlexComm interrupt */
#define IMXRT_IRQ_FLEXCOMM16              (IMXRT_IRQ_EXTINT + 66)  /* FlexComm interrupt */
#define IMXRT_IRQ_I3C1                    (IMXRT_IRQ_EXTINT + 67)  /* Improved Inter Integrated Circuit 1 interrupt */
#define IMXRT_IRQ_FLEXIO                  (IMXRT_IRQ_EXTINT + 68)  /* Flexible I/O interrupt */
#define IMXRT_IRQ_LCDIF                   (IMXRT_IRQ_EXTINT + 69)  /* Liquid Crystal Display interface interrupt */
#define IMXRT_IRQ_GPU                     (IMXRT_IRQ_EXTINT + 70)  /* Graphics Processor Unit interrupt */
#define IMXRT_IRQ_MIPI                    (IMXRT_IRQ_EXTINT + 71)  /* MIPI interrupt */
#define IMXRT_IRQ_Reserved88              (IMXRT_IRQ_EXTINT + 72)  /* Reserve */
#define IMXRT_IRQ_SDMA                    (IMXRT_IRQ_EXTINT + 73)  /* Smart DMA Engine Controller interrupt */

#define IMXRT_IRQ_NEXTINT        (74)

/* GPIO second level interrupt **********************************************/

#define IMXRT_GPIO_IRQ_FIRST     (IMXRT_IRQ_EXTINT + IMXRT_IRQ_NEXTINT)
#define _IMXRT_GPIO1_0_15_BASE   IMXRT_GPIO_IRQ_FIRST

/* TO DO: Check if this really works for all GPIO banks on i.MX RT117X
 * Also, not all banks have 32 pins.
 * CM7 GPIO2 and GPIO3 are not added yet.
 */

#ifdef CONFIG_IMXRT_GPIO1_0_15_IRQ
#  define IMXRT_IRQ_GPIO1_0      (_IMXRT_GPIO1_0_15_BASE + 0)   /* GPIO1 pin 0 interrupt */
#  define IMXRT_IRQ_GPIO1_1      (_IMXRT_GPIO1_0_15_BASE + 1)   /* GPIO1 pin 1 interrupt */
#  define IMXRT_IRQ_GPIO1_2      (_IMXRT_GPIO1_0_15_BASE + 2)   /* GPIO1 pin 2 interrupt */
#  define IMXRT_IRQ_GPIO1_3      (_IMXRT_GPIO1_0_15_BASE + 3)   /* GPIO1 pin 3 interrupt */
#  define IMXRT_IRQ_GPIO1_4      (_IMXRT_GPIO1_0_15_BASE + 4)   /* GPIO1 pin 4 interrupt */
#  define IMXRT_IRQ_GPIO1_5      (_IMXRT_GPIO1_0_15_BASE + 5)   /* GPIO1 pin 5 interrupt */
#  define IMXRT_IRQ_GPIO1_6      (_IMXRT_GPIO1_0_15_BASE + 6)   /* GPIO1 pin 6 interrupt */
#  define IMXRT_IRQ_GPIO1_7      (_IMXRT_GPIO1_0_15_BASE + 7)   /* GPIO1 pin 7 interrupt */
#  define IMXRT_IRQ_GPIO1_8      (_IMXRT_GPIO1_0_15_BASE + 8)   /* GPIO1 pin 8 interrupt */
#  define IMXRT_IRQ_GPIO1_9      (_IMXRT_GPIO1_0_15_BASE + 9)   /* GPIO1 pin 9 interrupt */
#  define IMXRT_IRQ_GPIO1_10     (_IMXRT_GPIO1_0_15_BASE + 10)  /* GPIO1 pin 10 interrupt */
#  define IMXRT_IRQ_GPIO1_11     (_IMXRT_GPIO1_0_15_BASE + 11)  /* GPIO1 pin 11 interrupt */
#  define IMXRT_IRQ_GPIO1_12     (_IMXRT_GPIO1_0_15_BASE + 12)  /* GPIO1 pin 12 interrupt */
#  define IMXRT_IRQ_GPIO1_13     (_IMXRT_GPIO1_0_15_BASE + 13)  /* GPIO1 pin 13 interrupt */
#  define IMXRT_IRQ_GPIO1_14     (_IMXRT_GPIO1_0_15_BASE + 14)  /* GPIO1 pin 14 interrupt */
#  define IMXRT_IRQ_GPIO1_15     (_IMXRT_GPIO1_0_15_BASE + 15)  /* GPIO1 pin 15 interrupt */

#  define _IMXRT_GPIO1_0_15_NIRQS 16
#  define _IMXRT_GPIO1_16_31_BASE (_IMXRT_GPIO1_0_15_BASE + _IMXRT_GPIO1_0_15_NIRQS)
#else
#  define _IMXRT_GPIO1_0_15_NIRQS 0
#  define _IMXRT_GPIO1_16_31_BASE _IMXRT_GPIO1_0_15_BASE
#endif

#ifdef CONFIG_IMXRT_GPIO1_16_31_IRQ
#  define IMXRT_IRQ_GPIO1_16     (_IMXRT_GPIO1_16_31_BASE + 0)   /* GPIO1 pin 16 interrupt */
#  define IMXRT_IRQ_GPIO1_17     (_IMXRT_GPIO1_16_31_BASE + 1)   /* GPIO1 pin 17 interrupt */
#  define IMXRT_IRQ_GPIO1_18     (_IMXRT_GPIO1_16_31_BASE + 2)   /* GPIO1 pin 18 interrupt */
#  define IMXRT_IRQ_GPIO1_19     (_IMXRT_GPIO1_16_31_BASE + 3)   /* GPIO1 pin 19 interrupt */
#  define IMXRT_IRQ_GPIO1_20     (_IMXRT_GPIO1_16_31_BASE + 4)   /* GPIO1 pin 10 interrupt */
#  define IMXRT_IRQ_GPIO1_21     (_IMXRT_GPIO1_16_31_BASE + 5)   /* GPIO1 pin 21 interrupt */
#  define IMXRT_IRQ_GPIO1_22     (_IMXRT_GPIO1_16_31_BASE + 6)   /* GPIO1 pin 22 interrupt */
#  define IMXRT_IRQ_GPIO1_23     (_IMXRT_GPIO1_16_31_BASE + 7)   /* GPIO1 pin 23 interrupt */
#  define IMXRT_IRQ_GPIO1_24     (_IMXRT_GPIO1_16_31_BASE + 8)   /* GPIO1 pin 24 interrupt */
#  define IMXRT_IRQ_GPIO1_25     (_IMXRT_GPIO1_16_31_BASE + 9)   /* GPIO1 pin 25 interrupt */
#  define IMXRT_IRQ_GPIO1_26     (_IMXRT_GPIO1_16_31_BASE + 10)  /* GPIO1 pin 26 interrupt */
#  define IMXRT_IRQ_GPIO1_27     (_IMXRT_GPIO1_16_31_BASE + 11)  /* GPIO1 pin 27 interrupt */
#  define IMXRT_IRQ_GPIO1_28     (_IMXRT_GPIO1_16_31_BASE + 12)  /* GPIO1 pin 28 interrupt */
#  define IMXRT_IRQ_GPIO1_29     (_IMXRT_GPIO1_16_31_BASE + 13)  /* GPIO1 pin 29 interrupt */
#  define IMXRT_IRQ_GPIO1_30     (_IMXRT_GPIO1_16_31_BASE + 14)  /* GPIO1 pin 30 interrupt */
#  define IMXRT_IRQ_GPIO1_31     (_IMXRT_GPIO1_16_31_BASE + 15)  /* GPIO1 pin 31 interrupt */

#  define _IMXRT_GPIO1_16_31_NIRQS 16
#  define _IMXRT_GPIO2_0_15_BASE (_IMXRT_GPIO1_16_31_BASE + _IMXRT_GPIO1_16_31_NIRQS)
#  define IMXRT_GPIO1_NIRQS      (_IMXRT_GPIO1_0_15_NIRQS + _IMXRT_GPIO1_16_31_NIRQS)
#else
#  define _IMXRT_GPIO2_0_15_BASE _IMXRT_GPIO1_16_31_BASE
#  define IMXRT_GPIO1_NIRQS      _IMXRT_GPIO1_0_15_NIRQS
#endif

#ifdef CONFIG_IMXRT_GPIO2_0_15_IRQ
#  define IMXRT_IRQ_GPIO2_0      (_IMXRT_GPIO2_0_15_BASE + 0)   /* GPIO2 pin 0 interrupt */
#  define IMXRT_IRQ_GPIO2_1      (_IMXRT_GPIO2_0_15_BASE + 1)   /* GPIO2 pin 1 interrupt */
#  define IMXRT_IRQ_GPIO2_2      (_IMXRT_GPIO2_0_15_BASE + 2)   /* GPIO2 pin 2 interrupt */
#  define IMXRT_IRQ_GPIO2_3      (_IMXRT_GPIO2_0_15_BASE + 3)   /* GPIO2 pin 3 interrupt */
#  define IMXRT_IRQ_GPIO2_4      (_IMXRT_GPIO2_0_15_BASE + 4)   /* GPIO2 pin 4 interrupt */
#  define IMXRT_IRQ_GPIO2_5      (_IMXRT_GPIO2_0_15_BASE + 5)   /* GPIO2 pin 5 interrupt */
#  define IMXRT_IRQ_GPIO2_6      (_IMXRT_GPIO2_0_15_BASE + 6)   /* GPIO2 pin 6 interrupt */
#  define IMXRT_IRQ_GPIO2_7      (_IMXRT_GPIO2_0_15_BASE + 7)   /* GPIO2 pin 7 interrupt */
#  define IMXRT_IRQ_GPIO2_8      (_IMXRT_GPIO2_0_15_BASE + 8)   /* GPIO2 pin 8 interrupt */
#  define IMXRT_IRQ_GPIO2_9      (_IMXRT_GPIO2_0_15_BASE + 9)   /* GPIO2 pin 9 interrupt */
#  define IMXRT_IRQ_GPIO2_10     (_IMXRT_GPIO2_0_15_BASE + 10)  /* GPIO2 pin 10 interrupt */
#  define IMXRT_IRQ_GPIO2_11     (_IMXRT_GPIO2_0_15_BASE + 11)  /* GPIO2 pin 11 interrupt */
#  define IMXRT_IRQ_GPIO2_12     (_IMXRT_GPIO2_0_15_BASE + 12)  /* GPIO2 pin 12 interrupt */
#  define IMXRT_IRQ_GPIO2_13     (_IMXRT_GPIO2_0_15_BASE + 13)  /* GPIO2 pin 13 interrupt */
#  define IMXRT_IRQ_GPIO2_14     (_IMXRT_GPIO2_0_15_BASE + 14)  /* GPIO2 pin 14 interrupt */
#  define IMXRT_IRQ_GPIO2_15     (_IMXRT_GPIO2_0_15_BASE + 15)  /* GPIO2 pin 15 interrupt */

#  define _IMXRT_GPIO2_0_15_NIRQS 16
#  define _IMXRT_GPIO2_16_31_BASE (_IMXRT_GPIO2_0_15_BASE + _IMXRT_GPIO2_0_15_NIRQS)
#else
#  define _IMXRT_GPIO2_0_15_NIRQS 0
#  define _IMXRT_GPIO2_16_31_BASE _IMXRT_GPIO2_0_15_BASE
#endif

#ifdef CONFIG_IMXRT_GPIO2_16_31_IRQ
#  define IMXRT_IRQ_GPIO2_16     (_IMXRT_GPIO2_16_31_BASE + 0)   /* GPIO2 pin 16 interrupt */
#  define IMXRT_IRQ_GPIO2_17     (_IMXRT_GPIO2_16_31_BASE + 1)   /* GPIO2 pin 17 interrupt */
#  define IMXRT_IRQ_GPIO2_18     (_IMXRT_GPIO2_16_31_BASE + 2)   /* GPIO2 pin 18 interrupt */
#  define IMXRT_IRQ_GPIO2_19     (_IMXRT_GPIO2_16_31_BASE + 3)   /* GPIO2 pin 19 interrupt */
#  define IMXRT_IRQ_GPIO2_20     (_IMXRT_GPIO2_16_31_BASE + 4)   /* GPIO2 pin 20 interrupt */
#  define IMXRT_IRQ_GPIO2_21     (_IMXRT_GPIO2_16_31_BASE + 5)   /* GPIO2 pin 21 interrupt */
#  define IMXRT_IRQ_GPIO2_22     (_IMXRT_GPIO2_16_31_BASE + 6)   /* GPIO2 pin 22 interrupt */
#  define IMXRT_IRQ_GPIO2_23     (_IMXRT_GPIO2_16_31_BASE + 7)   /* GPIO2 pin 23 interrupt */
#  define IMXRT_IRQ_GPIO2_24     (_IMXRT_GPIO2_16_31_BASE + 8)   /* GPIO2 pin 24 interrupt */
#  define IMXRT_IRQ_GPIO2_25     (_IMXRT_GPIO2_16_31_BASE + 9)   /* GPIO2 pin 25 interrupt */
#  define IMXRT_IRQ_GPIO2_26     (_IMXRT_GPIO2_16_31_BASE + 10)  /* GPIO2 pin 26 interrupt */
#  define IMXRT_IRQ_GPIO2_27     (_IMXRT_GPIO2_16_31_BASE + 11)  /* GPIO2 pin 27 interrupt */
#  define IMXRT_IRQ_GPIO2_28     (_IMXRT_GPIO2_16_31_BASE + 12)  /* GPIO2 pin 28 interrupt */
#  define IMXRT_IRQ_GPIO2_29     (_IMXRT_GPIO2_16_31_BASE + 13)  /* GPIO2 pin 29 interrupt */
#  define IMXRT_IRQ_GPIO2_30     (_IMXRT_GPIO2_16_31_BASE + 14)  /* GPIO2 pin 30 interrupt */
#  define IMXRT_IRQ_GPIO2_31     (_IMXRT_GPIO2_16_31_BASE + 15)  /* GPIO2 pin 31 interrupt */

#  define _IMXRT_GPIO2_16_31_NIRQS 16
#  define _IMXRT_GPIO3_0_15_BASE (_IMXRT_GPIO2_16_31_BASE + _IMXRT_GPIO2_16_31_NIRQS)
#  define IMXRT_GPIO2_NIRQS      (_IMXRT_GPIO2_0_15_NIRQS + _IMXRT_GPIO2_16_31_NIRQS)
#else
#  define _IMXRT_GPIO3_0_15_BASE _IMXRT_GPIO2_16_31_BASE
#  define IMXRT_GPIO2_NIRQS      _IMXRT_GPIO2_0_15_NIRQS
#endif

#ifdef CONFIG_IMXRT_GPIO3_0_15_IRQ
#  define IMXRT_IRQ_GPIO3_0      (_IMXRT_GPIO3_0_15_BASE + 0)   /* GPIO3 pin 0 interrupt */
#  define IMXRT_IRQ_GPIO3_1      (_IMXRT_GPIO3_0_15_BASE + 1)   /* GPIO3 pin 1 interrupt */
#  define IMXRT_IRQ_GPIO3_2      (_IMXRT_GPIO3_0_15_BASE + 2)   /* GPIO3 pin 2 interrupt */
#  define IMXRT_IRQ_GPIO3_3      (_IMXRT_GPIO3_0_15_BASE + 3)   /* GPIO3 pin 3 interrupt */
#  define IMXRT_IRQ_GPIO3_4      (_IMXRT_GPIO3_0_15_BASE + 4)   /* GPIO3 pin 4 interrupt */
#  define IMXRT_IRQ_GPIO3_5      (_IMXRT_GPIO3_0_15_BASE + 5)   /* GPIO3 pin 5 interrupt */
#  define IMXRT_IRQ_GPIO3_6      (_IMXRT_GPIO3_0_15_BASE + 6)   /* GPIO3 pin 6 interrupt */
#  define IMXRT_IRQ_GPIO3_7      (_IMXRT_GPIO3_0_15_BASE + 7)   /* GPIO3 pin 7 interrupt */
#  define IMXRT_IRQ_GPIO3_8      (_IMXRT_GPIO3_0_15_BASE + 8)   /* GPIO3 pin 8 interrupt */
#  define IMXRT_IRQ_GPIO3_9      (_IMXRT_GPIO3_0_15_BASE + 9)   /* GPIO3 pin 9 interrupt */
#  define IMXRT_IRQ_GPIO3_10     (_IMXRT_GPIO3_0_15_BASE + 10)  /* GPIO3 pin 10 interrupt */
#  define IMXRT_IRQ_GPIO3_11     (_IMXRT_GPIO3_0_15_BASE + 11)  /* GPIO3 pin 11 interrupt */
#  define IMXRT_IRQ_GPIO3_12     (_IMXRT_GPIO3_0_15_BASE + 12)  /* GPIO3 pin 12 interrupt */
#  define IMXRT_IRQ_GPIO3_13     (_IMXRT_GPIO3_0_15_BASE + 13)  /* GPIO3 pin 13 interrupt */
#  define IMXRT_IRQ_GPIO3_14     (_IMXRT_GPIO3_0_15_BASE + 14)  /* GPIO3 pin 14 interrupt */
#  define IMXRT_IRQ_GPIO3_15     (_IMXRT_GPIO3_0_15_BASE + 15)  /* GPIO3 pin 15 interrupt */

#  define _IMXRT_GPIO3_0_15_NIRQS 16
#  define _IMXRT_GPIO3_16_31_BASE (_IMXRT_GPIO3_0_15_BASE + _IMXRT_GPIO3_0_15_NIRQS)
#else
#  define _IMXRT_GPIO3_0_15_NIRQS 0
#  define _IMXRT_GPIO3_16_31_BASE _IMXRT_GPIO3_0_15_BASE
#endif

#ifdef CONFIG_IMXRT_GPIO3_16_31_IRQ
#  define IMXRT_IRQ_GPIO3_16     (_IMXRT_GPIO3_16_31_BASE + 0)   /* GPIO3 pin 16 interrupt */
#  define IMXRT_IRQ_GPIO3_17     (_IMXRT_GPIO3_16_31_BASE + 1)   /* GPIO3 pin 17 interrupt */
#  define IMXRT_IRQ_GPIO3_18     (_IMXRT_GPIO3_16_31_BASE + 2)   /* GPIO3 pin 18 interrupt */
#  define IMXRT_IRQ_GPIO3_19     (_IMXRT_GPIO3_16_31_BASE + 3)   /* GPIO3 pin 19 interrupt */
#  define IMXRT_IRQ_GPIO3_20     (_IMXRT_GPIO3_16_31_BASE + 4)   /* GPIO3 pin 20 interrupt */
#  define IMXRT_IRQ_GPIO3_21     (_IMXRT_GPIO3_16_31_BASE + 5)   /* GPIO3 pin 21 interrupt */
#  define IMXRT_IRQ_GPIO3_22     (_IMXRT_GPIO3_16_31_BASE + 6)   /* GPIO3 pin 22 interrupt */
#  define IMXRT_IRQ_GPIO3_23     (_IMXRT_GPIO3_16_31_BASE + 7)   /* GPIO3 pin 23 interrupt */
#  define IMXRT_IRQ_GPIO3_24     (_IMXRT_GPIO3_16_31_BASE + 8)   /* GPIO3 pin 24 interrupt */
#  define IMXRT_IRQ_GPIO3_25     (_IMXRT_GPIO3_16_31_BASE + 9)   /* GPIO3 pin 25 interrupt */
#  define IMXRT_IRQ_GPIO3_26     (_IMXRT_GPIO3_16_31_BASE + 10)  /* GPIO3 pin 26 interrupt */
#  define IMXRT_IRQ_GPIO3_27     (_IMXRT_GPIO3_16_31_BASE + 11)  /* GPIO3 pin 27 interrupt */
#  define IMXRT_IRQ_GPIO3_28     (_IMXRT_GPIO3_16_31_BASE + 12)  /* GPIO3 pin 28 interrupt */
#  define IMXRT_IRQ_GPIO3_29     (_IMXRT_GPIO3_16_31_BASE + 13)  /* GPIO3 pin 29 interrupt */
#  define IMXRT_IRQ_GPIO3_30     (_IMXRT_GPIO3_16_31_BASE + 14)  /* GPIO3 pin 30 interrupt */
#  define IMXRT_IRQ_GPIO3_31     (_IMXRT_GPIO3_16_31_BASE + 15)  /* GPIO3 pin 31 interrupt */

#  define _IMXRT_GPIO3_16_31_NIRQS 16
#  define _IMXRT_GPIO4_0_15_BASE (_IMXRT_GPIO3_16_31_BASE + _IMXRT_GPIO3_16_31_NIRQS)
#  define IMXRT_GPIO3_NIRQS      (_IMXRT_GPIO3_0_15_NIRQS + _IMXRT_GPIO3_16_31_NIRQS)
#else
#  define _IMXRT_GPIO4_0_15_BASE _IMXRT_GPIO3_16_31_BASE
#  define IMXRT_GPIO3_NIRQS      _IMXRT_GPIO3_0_15_NIRQS
#endif

#ifdef CONFIG_IMXRT_GPIO4_0_15_IRQ
#  define IMXRT_IRQ_GPIO4_0      (_IMXRT_GPIO4_0_15_BASE + 0)   /* GPIO4 pin 0 interrupt */
#  define IMXRT_IRQ_GPIO4_1      (_IMXRT_GPIO4_0_15_BASE + 1)   /* GPIO4 pin 1 interrupt */
#  define IMXRT_IRQ_GPIO4_2      (_IMXRT_GPIO4_0_15_BASE + 2)   /* GPIO4 pin 2 interrupt */
#  define IMXRT_IRQ_GPIO4_3      (_IMXRT_GPIO4_0_15_BASE + 3)   /* GPIO4 pin 3 interrupt */
#  define IMXRT_IRQ_GPIO4_4      (_IMXRT_GPIO4_0_15_BASE + 4)   /* GPIO4 pin 4 interrupt */
#  define IMXRT_IRQ_GPIO4_5      (_IMXRT_GPIO4_0_15_BASE + 5)   /* GPIO4 pin 5 interrupt */
#  define IMXRT_IRQ_GPIO4_6      (_IMXRT_GPIO4_0_15_BASE + 6)   /* GPIO4 pin 6 interrupt */
#  define IMXRT_IRQ_GPIO4_7      (_IMXRT_GPIO4_0_15_BASE + 7)   /* GPIO4 pin 7 interrupt */
#  define IMXRT_IRQ_GPIO4_8      (_IMXRT_GPIO4_0_15_BASE + 8)   /* GPIO4 pin 8 interrupt */
#  define IMXRT_IRQ_GPIO4_9      (_IMXRT_GPIO4_0_15_BASE + 9)   /* GPIO4 pin 9 interrupt */
#  define IMXRT_IRQ_GPIO4_10     (_IMXRT_GPIO4_0_15_BASE + 10)  /* GPIO4 pin 10 interrupt */
#  define IMXRT_IRQ_GPIO4_11     (_IMXRT_GPIO4_0_15_BASE + 11)  /* GPIO4 pin 11 interrupt */
#  define IMXRT_IRQ_GPIO4_12     (_IMXRT_GPIO4_0_15_BASE + 12)  /* GPIO4 pin 12 interrupt */
#  define IMXRT_IRQ_GPIO4_13     (_IMXRT_GPIO4_0_15_BASE + 13)  /* GPIO4 pin 13 interrupt */
#  define IMXRT_IRQ_GPIO4_14     (_IMXRT_GPIO4_0_15_BASE + 14)  /* GPIO4 pin 14 interrupt */
#  define IMXRT_IRQ_GPIO4_15     (_IMXRT_GPIO4_0_15_BASE + 15)  /* GPIO4 pin 15 interrupt */

#  define _IMXRT_GPIO4_0_15_NIRQS 16
#  define _IMXRT_GPIO4_16_31_BASE (_IMXRT_GPIO4_0_15_BASE + _IMXRT_GPIO4_0_15_NIRQS)
#else
#  define _IMXRT_GPIO4_0_15_NIRQS 0
#  define _IMXRT_GPIO4_16_31_BASE _IMXRT_GPIO4_0_15_BASE
#endif

#ifdef CONFIG_IMXRT_GPIO4_16_31_IRQ
#  define IMXRT_IRQ_GPIO4_16     (_IMXRT_GPIO4_16_31_BASE + 0)   /* GPIO4 pin 16 interrupt */
#  define IMXRT_IRQ_GPIO4_17     (_IMXRT_GPIO4_16_31_BASE + 1)   /* GPIO4 pin 17 interrupt */
#  define IMXRT_IRQ_GPIO4_18     (_IMXRT_GPIO4_16_31_BASE + 2)   /* GPIO4 pin 18 interrupt */
#  define IMXRT_IRQ_GPIO4_19     (_IMXRT_GPIO4_16_31_BASE + 3)   /* GPIO4 pin 19 interrupt */
#  define IMXRT_IRQ_GPIO4_20     (_IMXRT_GPIO4_16_31_BASE + 4)   /* GPIO4 pin 20 interrupt */
#  define IMXRT_IRQ_GPIO4_21     (_IMXRT_GPIO4_16_31_BASE + 5)   /* GPIO4 pin 21 interrupt */
#  define IMXRT_IRQ_GPIO4_22     (_IMXRT_GPIO4_16_31_BASE + 6)   /* GPIO4 pin 22 interrupt */
#  define IMXRT_IRQ_GPIO4_23     (_IMXRT_GPIO4_16_31_BASE + 7)   /* GPIO4 pin 23 interrupt */
#  define IMXRT_IRQ_GPIO4_24     (_IMXRT_GPIO4_16_31_BASE + 8)   /* GPIO4 pin 24 interrupt */
#  define IMXRT_IRQ_GPIO4_25     (_IMXRT_GPIO4_16_31_BASE + 9)   /* GPIO4 pin 25 interrupt */
#  define IMXRT_IRQ_GPIO4_26     (_IMXRT_GPIO4_16_31_BASE + 10)  /* GPIO4 pin 26 interrupt */
#  define IMXRT_IRQ_GPIO4_27     (_IMXRT_GPIO4_16_31_BASE + 11)  /* GPIO4 pin 27 interrupt */
#  define IMXRT_IRQ_GPIO4_28     (_IMXRT_GPIO4_16_31_BASE + 12)  /* GPIO4 pin 28 interrupt */
#  define IMXRT_IRQ_GPIO4_29     (_IMXRT_GPIO4_16_31_BASE + 13)  /* GPIO4 pin 29 interrupt */
#  define IMXRT_IRQ_GPIO4_30     (_IMXRT_GPIO4_16_31_BASE + 14)  /* GPIO4 pin 30 interrupt */
#  define IMXRT_IRQ_GPIO4_31     (_IMXRT_GPIO4_16_31_BASE + 15)  /* GPIO4 pin 31 interrupt */

#  define _IMXRT_GPIO4_16_31_NIRQS 16
#  define _IMXRT_GPIO5_0_15_BASE (_IMXRT_GPIO4_16_31_BASE + _IMXRT_GPIO4_16_31_NIRQS)
#  define IMXRT_GPIO4_NIRQS      (_IMXRT_GPIO4_0_15_NIRQS + _IMXRT_GPIO4_16_31_NIRQS)
#else
#  define _IMXRT_GPIO5_0_15_BASE _IMXRT_GPIO4_16_31_BASE
#  define IMXRT_GPIO4_NIRQS      _IMXRT_GPIO4_0_15_NIRQS
#endif

#ifdef CONFIG_IMXRT_GPIO5_0_15_IRQ
#  define IMXRT_IRQ_GPIO5_0      (_IMXRT_GPIO5_0_15_BASE + 0)   /* GPIO5 pin 0 interrupt */
#  define IMXRT_IRQ_GPIO5_1      (_IMXRT_GPIO5_0_15_BASE + 1)   /* GPIO5 pin 1 interrupt */
#  define IMXRT_IRQ_GPIO5_2      (_IMXRT_GPIO5_0_15_BASE + 2)   /* GPIO5 pin 2 interrupt */
#  define IMXRT_IRQ_GPIO5_3      (_IMXRT_GPIO5_0_15_BASE + 3)   /* GPIO5 pin 3 interrupt */
#  define IMXRT_IRQ_GPIO5_4      (_IMXRT_GPIO5_0_15_BASE + 4)   /* GPIO5 pin 4 interrupt */
#  define IMXRT_IRQ_GPIO5_5      (_IMXRT_GPIO5_0_15_BASE + 5)   /* GPIO5 pin 5 interrupt */
#  define IMXRT_IRQ_GPIO5_6      (_IMXRT_GPIO5_0_15_BASE + 6)   /* GPIO5 pin 6 interrupt */
#  define IMXRT_IRQ_GPIO5_7      (_IMXRT_GPIO5_0_15_BASE + 7)   /* GPIO5 pin 7 interrupt */
#  define IMXRT_IRQ_GPIO5_8      (_IMXRT_GPIO5_0_15_BASE + 8)   /* GPIO5 pin 8 interrupt */
#  define IMXRT_IRQ_GPIO5_9      (_IMXRT_GPIO5_0_15_BASE + 9)   /* GPIO5 pin 9 interrupt */
#  define IMXRT_IRQ_GPIO5_10     (_IMXRT_GPIO5_0_15_BASE + 10)  /* GPIO5 pin 10 interrupt */
#  define IMXRT_IRQ_GPIO5_11     (_IMXRT_GPIO5_0_15_BASE + 11)  /* GPIO5 pin 11 interrupt */
#  define IMXRT_IRQ_GPIO5_12     (_IMXRT_GPIO5_0_15_BASE + 12)  /* GPIO5 pin 12 interrupt */
#  define IMXRT_IRQ_GPIO5_13     (_IMXRT_GPIO5_0_15_BASE + 13)  /* GPIO5 pin 13 interrupt */
#  define IMXRT_IRQ_GPIO5_14     (_IMXRT_GPIO5_0_15_BASE + 14)  /* GPIO5 pin 14 interrupt */
#  define IMXRT_IRQ_GPIO5_15     (_IMXRT_GPIO5_0_15_BASE + 15)  /* GPIO5 pin 15 interrupt */

#  define _IMXRT_GPIO5_0_15_NIRQS 16
#  define _IMXRT_GPIO5_16_31_BASE (_IMXRT_GPIO5_0_15_BASE + _IMXRT_GPIO5_0_15_NIRQS)
#else
#  define _IMXRT_GPIO5_0_15_NIRQS 0
#  define _IMXRT_GPIO5_16_31_BASE _IMXRT_GPIO5_0_15_BASE
#endif

#ifdef CONFIG_IMXRT_GPIO5_16_31_IRQ
#  define IMXRT_IRQ_GPIO5_16     (_IMXRT_GPIO5_16_31_BASE + 0)   /* GPIO5 pin 16 interrupt */
#  define IMXRT_IRQ_GPIO5_17     (_IMXRT_GPIO5_16_31_BASE + 1)   /* GPIO5 pin 17 interrupt */
#  define IMXRT_IRQ_GPIO5_18     (_IMXRT_GPIO5_16_31_BASE + 2)   /* GPIO5 pin 18 interrupt */
#  define IMXRT_IRQ_GPIO5_19     (_IMXRT_GPIO5_16_31_BASE + 3)   /* GPIO5 pin 19 interrupt */
#  define IMXRT_IRQ_GPIO5_20     (_IMXRT_GPIO5_16_31_BASE + 4)   /* GPIO5 pin 20 interrupt */
#  define IMXRT_IRQ_GPIO5_21     (_IMXRT_GPIO5_16_31_BASE + 5)   /* GPIO5 pin 21 interrupt */
#  define IMXRT_IRQ_GPIO5_22     (_IMXRT_GPIO5_16_31_BASE + 6)   /* GPIO5 pin 22 interrupt */
#  define IMXRT_IRQ_GPIO5_23     (_IMXRT_GPIO5_16_31_BASE + 7)   /* GPIO5 pin 23 interrupt */
#  define IMXRT_IRQ_GPIO5_24     (_IMXRT_GPIO5_16_31_BASE + 8)   /* GPIO5 pin 24 interrupt */
#  define IMXRT_IRQ_GPIO5_25     (_IMXRT_GPIO5_16_31_BASE + 9)   /* GPIO5 pin 25 interrupt */
#  define IMXRT_IRQ_GPIO5_26     (_IMXRT_GPIO5_16_31_BASE + 10)  /* GPIO5 pin 26 interrupt */
#  define IMXRT_IRQ_GPIO5_27     (_IMXRT_GPIO5_16_31_BASE + 11)  /* GPIO5 pin 27 interrupt */
#  define IMXRT_IRQ_GPIO5_28     (_IMXRT_GPIO5_16_31_BASE + 12)  /* GPIO5 pin 28 interrupt */
#  define IMXRT_IRQ_GPIO5_29     (_IMXRT_GPIO5_16_31_BASE + 13)  /* GPIO5 pin 29 interrupt */
#  define IMXRT_IRQ_GPIO5_30     (_IMXRT_GPIO5_16_31_BASE + 14)  /* GPIO5 pin 30 interrupt */
#  define IMXRT_IRQ_GPIO5_31     (_IMXRT_GPIO5_16_31_BASE + 15)  /* GPIO5 pin 31 interrupt */

#  define _IMXRT_GPIO5_16_31_NIRQS 16
#  define _IMXRT_GPIO6_0_15_BASE (_IMXRT_GPIO5_16_31_BASE + _IMXRT_GPIO5_16_31_NIRQS)
#  define IMXRT_GPIO5_NIRQS      (_IMXRT_GPIO5_0_15_NIRQS + _IMXRT_GPIO5_16_31_NIRQS)
#else
#  define _IMXRT_GPIO6_0_15_BASE _IMXRT_GPIO5_16_31_BASE
#  define IMXRT_GPIO5_NIRQS      _IMXRT_GPIO5_0_15_NIRQS
#endif

#ifdef CONFIG_IMXRT_GPIO6_0_15_IRQ
#  define IMXRT_IRQ_GPIO6_0      (_IMXRT_GPIO6_0_15_BASE + 0)   /* GPIO6 pin 0 interrupt */
#  define IMXRT_IRQ_GPIO6_1      (_IMXRT_GPIO6_0_15_BASE + 1)   /* GPIO6 pin 1 interrupt */
#  define IMXRT_IRQ_GPIO6_2      (_IMXRT_GPIO6_0_15_BASE + 2)   /* GPIO6 pin 2 interrupt */
#  define IMXRT_IRQ_GPIO6_3      (_IMXRT_GPIO6_0_15_BASE + 3)   /* GPIO6 pin 3 interrupt */
#  define IMXRT_IRQ_GPIO6_4      (_IMXRT_GPIO6_0_15_BASE + 4)   /* GPIO6 pin 4 interrupt */
#  define IMXRT_IRQ_GPIO6_5      (_IMXRT_GPIO6_0_15_BASE + 5)   /* GPIO6 pin 5 interrupt */
#  define IMXRT_IRQ_GPIO6_6      (_IMXRT_GPIO6_0_15_BASE + 6)   /* GPIO6 pin 6 interrupt */
#  define IMXRT_IRQ_GPIO6_7      (_IMXRT_GPIO6_0_15_BASE + 7)   /* GPIO6 pin 7 interrupt */
#  define IMXRT_IRQ_GPIO6_8      (_IMXRT_GPIO6_0_15_BASE + 8)   /* GPIO6 pin 8 interrupt */
#  define IMXRT_IRQ_GPIO6_9      (_IMXRT_GPIO6_0_15_BASE + 9)   /* GPIO6 pin 9 interrupt */
#  define IMXRT_IRQ_GPIO6_10     (_IMXRT_GPIO6_0_15_BASE + 10)  /* GPIO6 pin 10 interrupt */
#  define IMXRT_IRQ_GPIO6_11     (_IMXRT_GPIO6_0_15_BASE + 11)  /* GPIO6 pin 11 interrupt */
#  define IMXRT_IRQ_GPIO6_12     (_IMXRT_GPIO6_0_15_BASE + 12)  /* GPIO6 pin 12 interrupt */
#  define IMXRT_IRQ_GPIO6_13     (_IMXRT_GPIO6_0_15_BASE + 13)  /* GPIO6 pin 13 interrupt */
#  define IMXRT_IRQ_GPIO6_14     (_IMXRT_GPIO6_0_15_BASE + 14)  /* GPIO6 pin 14 interrupt */
#  define IMXRT_IRQ_GPIO6_15     (_IMXRT_GPIO6_0_15_BASE + 15)  /* GPIO6 pin 15 interrupt */

#  define _IMXRT_GPIO6_0_15_NIRQS 16
#  define _IMXRT_GPIO6_16_31_BASE (_IMXRT_GPIO6_0_15_BASE + _IMXRT_GPIO6_0_15_NIRQS)
#else
#  define _IMXRT_GPIO6_0_15_NIRQS 0
#  define _IMXRT_GPIO6_16_31_BASE _IMXRT_GPIO6_0_15_BASE
#endif

#ifdef CONFIG_IMXRT_GPIO6_16_31_IRQ
#  define IMXRT_IRQ_GPIO6_16     (_IMXRT_GPIO6_16_31_BASE + 0)   /* GPIO6 pin 16 interrupt */
#  define IMXRT_IRQ_GPIO6_17     (_IMXRT_GPIO6_16_31_BASE + 1)   /* GPIO6 pin 17 interrupt */
#  define IMXRT_IRQ_GPIO6_18     (_IMXRT_GPIO6_16_31_BASE + 2)   /* GPIO6 pin 18 interrupt */
#  define IMXRT_IRQ_GPIO6_19     (_IMXRT_GPIO6_16_31_BASE + 3)   /* GPIO6 pin 19 interrupt */
#  define IMXRT_IRQ_GPIO6_20     (_IMXRT_GPIO6_16_31_BASE + 4)   /* GPIO6 pin 20 interrupt */
#  define IMXRT_IRQ_GPIO6_21     (_IMXRT_GPIO6_16_31_BASE + 5)   /* GPIO6 pin 21 interrupt */
#  define IMXRT_IRQ_GPIO6_22     (_IMXRT_GPIO6_16_31_BASE + 6)   /* GPIO6 pin 22 interrupt */
#  define IMXRT_IRQ_GPIO6_23     (_IMXRT_GPIO6_16_31_BASE + 7)   /* GPIO6 pin 23 interrupt */
#  define IMXRT_IRQ_GPIO6_24     (_IMXRT_GPIO6_16_31_BASE + 8)   /* GPIO6 pin 24 interrupt */
#  define IMXRT_IRQ_GPIO6_25     (_IMXRT_GPIO6_16_31_BASE + 9)   /* GPIO6 pin 25 interrupt */
#  define IMXRT_IRQ_GPIO6_26     (_IMXRT_GPIO6_16_31_BASE + 10)  /* GPIO6 pin 26 interrupt */
#  define IMXRT_IRQ_GPIO6_27     (_IMXRT_GPIO6_16_31_BASE + 11)  /* GPIO6 pin 27 interrupt */
#  define IMXRT_IRQ_GPIO6_28     (_IMXRT_GPIO6_16_31_BASE + 12)  /* GPIO6 pin 28 interrupt */
#  define IMXRT_IRQ_GPIO6_29     (_IMXRT_GPIO6_16_31_BASE + 13)  /* GPIO6 pin 29 interrupt */
#  define IMXRT_IRQ_GPIO6_30     (_IMXRT_GPIO6_16_31_BASE + 14)  /* GPIO6 pin 30 interrupt */
#  define IMXRT_IRQ_GPIO6_31     (_IMXRT_GPIO6_16_31_BASE + 15)  /* GPIO6 pin 31 interrupt */

#  define IMXRT_GPIO6_NIRQS     32
#else
#  define IMXRT_GPIO6_NIRQS     0
#endif

#define IMXRT_GPIO_NIRQS         (IMXRT_GPIO1_NIRQS + IMXRT_GPIO2_NIRQS + \
                                  IMXRT_GPIO3_NIRQS + IMXRT_GPIO4_NIRQS + \
                                  IMXRT_GPIO5_NIRQS + IMXRT_GPIO6_NIRQS)
#define IMXRT_GPIO_IRQ_LAST      (_IMXRT_GPIO1_0_15_BASE + IMXRT_GPIO_NIRQS)

/* Total number of IRQ numbers **********************************************/

#define NR_IRQS                  (IMXRT_IRQ_EXTINT + IMXRT_IRQ_NEXTINT + IMXRT_GPIO_NIRQS)

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Inline functions
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifndef __ASSEMBLY__
#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif
#endif

#endif /* __ARCH_ARM_INCLUDE_IMXRT_IMXRT5XX_IRQ_H */
