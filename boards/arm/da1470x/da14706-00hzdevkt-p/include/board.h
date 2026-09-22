/****************************************************************************
 * boards/arm/da1470x/da14706-00hzdevkt-p/include/board.h
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

#ifndef __BOARDS_ARM_DA1470X_DA14706_00HZDEVKT_P_INCLUDE_BOARD_H
#define __BOARDS_ARM_DA1470X_DA14706_00HZDEVKT_P_INCLUDE_BOARD_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdbool.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Clocking *****************************************************************/

/* The devkit carries a 32 MHz crystal (XTAL32M) and a 32.768 kHz crystal
 * (XTAL32K).  The system clock source is selected in Kconfig; the SysTick
 * frequency is read back from the clock controller at run time.
 */

#define BOARD_XTAL32M_FREQ          32000000
#define BOARD_XTAL32K_FREQ          32768

/* LED definitions **********************************************************/

/* A low output illuminates the LED.
 *
 * LED index values for use with board_userled()
 */

#define BOARD_LED1                  0
#define BOARD_LED2                  1
#define BOARD_LED3                  2
#ifdef CONFIG_DA14706_TOUCH_ZT2628
#  define BOARD_NLEDS               2  /* P1.1 is the touch reset line */
#else
#  define BOARD_NLEDS               3
#endif

/* LED bits for use with board_userled_all() */

#define BOARD_LED1_BIT              (1 << BOARD_LED1)
#define BOARD_LED2_BIT              (1 << BOARD_LED2)
#define BOARD_LED3_BIT              (1 << BOARD_LED3)

/* If CONFIG_ARCH_LEDS is defined, the LED will be controlled as follows
 * for NuttX debug functionality (where NC means "No Change").
 */

#define LED_STARTED                 0  /* OFF      */
#define LED_HEAPALLOCATE            0  /* OFF      */
#define LED_IRQSENABLED             0  /* OFF      */
#define LED_STACKCREATED            1  /* ON       */
#define LED_INIRQ                   2  /* NC       */
#define LED_SIGNAL                  2  /* NC       */
#define LED_ASSERTION               2  /* NC       */
#define LED_PANIC                   3  /* Flashing */

/* Button definitions *******************************************************/

/* K1 and K2 are the user push buttons on P1.22 and P1.23 (active low). */

#define BUTTON_BTN1                 0
#define BUTTON_BTN2                 1
#define NUM_BUTTONS                 2

#define BUTTON_BTN1_BIT             (1 << BUTTON_BTN1)
#define BUTTON_BTN2_BIT             (1 << BUTTON_BTN2)

/* UART Pins ****************************************************************/

/* UART0 is routed to the FT2232HL of the devkit: TX on P0.8, RX on P2.1 */

#define BOARD_UART0_RX_PIN  (GPIO_INPUT | GPIO_PULLUP | GPIO_FUNC_UART_RX | \
                             GPIO_PORT2 | GPIO_PIN(1))
#define BOARD_UART0_TX_PIN  (GPIO_OUTPUT | GPIO_FUNC_UART_TX | GPIO_PORT0 | \
                             GPIO_PIN(8))

/* UART1 (datasheet UART2) on the expansion header, with flow control */

#define BOARD_UART1_RX_PIN  (GPIO_INPUT | GPIO_PULLUP | GPIO_FUNC_UART2_RX | \
                             GPIO_PORT1 | GPIO_PIN(8))
#define BOARD_UART1_TX_PIN  (GPIO_OUTPUT | GPIO_FUNC_UART2_TX | GPIO_PORT1 | \
                             GPIO_PIN(9))
#define BOARD_UART1_RTS_PIN (GPIO_OUTPUT | GPIO_FUNC_UART2_RTSN | GPIO_PORT1 | \
                             GPIO_PIN(10))
#define BOARD_UART1_CTS_PIN (GPIO_INPUT | GPIO_PULLUP | GPIO_FUNC_UART2_CTSN | \
                             GPIO_PORT1 | GPIO_PIN(11))

/* SPI Pins *****************************************************************/

/* SPI0 on the display connector's spare serial pins (P1.13/14/16/17),
 * free when the E120A390QSR board is fitted -- but NOT free if the PSRAM
 * is in use.  P1.13 and P1.14 are also QSPIC2_D2 and QSPIC2_D1, which run
 * across the board connector to the APS6404 PSRAM (U2) on the
 * motherboard, so SPI0 here and the PSRAM cannot both be enabled.  The
 * other four QSPIC2 signals are P1.15 (D0), P1.19 (CLK), P1.20 (D3) and
 * P1.24 (CS).  Those pads are fixed at the 1.8 V V18P rail, which is what
 * the PSRAM runs at.
 */

#define BOARD_SPI0_SCLK_PIN (GPIO_OUTPUT | GPIO_FUNC_SPI_CLK | GPIO_PORT1 | \
                             GPIO_PIN(17))
#define BOARD_SPI0_MOSI_PIN (GPIO_OUTPUT | GPIO_FUNC_SPI_DO | GPIO_PORT1 | \
                             GPIO_PIN(13))
#define BOARD_SPI0_MISO_PIN (GPIO_INPUT | GPIO_FUNC_SPI_DI | GPIO_PORT1 | \
                             GPIO_PIN(16))
#define BOARD_SPI0_CS_PIN   (GPIO_OUTPUT | GPIO_VALUE_ONE | GPIO_FUNC_GPIO | \
                             GPIO_PORT1 | GPIO_PIN(14))

/* I2C Pins *****************************************************************/

/* I2C0 is the touch controller bus of the display connector (P1.12 SCL,
 * P1.11 SDA); its pull-ups are powered through BOARD_TOUCH_PWR_PIN.
 */

#define BOARD_I2C0_SCL_PIN  (GPIO_OUTPUT | GPIO_FUNC_I2C_SCL | GPIO_OPENDRAIN | \
                             GPIO_PORT1 | GPIO_PIN(12))
#define BOARD_I2C0_SDA_PIN  (GPIO_OUTPUT | GPIO_FUNC_I2C_SDA | GPIO_OPENDRAIN | \
                             GPIO_PORT1 | GPIO_PIN(11))

/* Touch controller (Zinitix ZT2628 on the E120A390QSR board) ***************/

#define BOARD_TOUCH_PWR_PIN (GPIO_OUTPUT | GPIO_FUNC_GPIO | GPIO_PORT0 | \
                             GPIO_PIN(28))

#define BOARD_TOUCH_RST_PIN (GPIO_OUTPUT | GPIO_VALUE_ONE | GPIO_FUNC_GPIO | \
                             GPIO_PORT1 | GPIO_PIN(1))
#define BOARD_TOUCH_INT_PIN (GPIO_INPUT | GPIO_PULLUP | GPIO_FUNC_GPIO | \
                             GPIO_PORT1 | GPIO_PIN(3))

/* QSPIC2 / PSRAM **********************************************************/

/* The PSRAM controller's signals have a fixed assignment to these six
 * pins (datasheet Table 188).  They are not reachable through the PID
 * multiplexer at all -- there is no QSPIC2 PID -- so they are set up as
 * plain GPIOs and the controller takes them over.  Clock and chip select
 * are outputs; the four data lines are left as inputs without a pull,
 * because the controller switches their direction itself between the
 * command and data phases.  The reset state's pull-down is dropped so it
 * does not fight the PSRAM's weak drive.
 *
 * Chip select is given an idle-high value, so that the part cannot be
 * selected in the moment between configuring the pin and the controller
 * taking it.
 */

/* The PSRAM's power AND its connection to the SoC are both switched by
 * RAM_PWRON on the motherboard (user manual section 4.9): high powers U2
 * through the load switch U3 and routes these six signals to the SoC; low
 * leaves U2 unpowered and routes the signals to the monitoring header J4
 * instead.  With J12 in its default 1-2 position RAM_PWRON is P1_00, which
 * comes out of reset as a pulled-down input -- so until something raises
 * it the PSRAM is both off and disconnected, and reads see a floating
 * line.  (J12 2-3 ties it on permanently.)
 *
 * P1_00 is also the panel's IM1 interface-select line, which the display
 * code drives high too.  Both want it high; it just has to be high before
 * the PSRAM is touched, not only once the panel is set up.
 */

#define BOARD_RAM_PWRON_PIN  (GPIO_OUTPUT | GPIO_VALUE_ONE | GPIO_FUNC_GPIO | \
                              GPIO_PORT1 | GPIO_PIN(0))

#define BOARD_QSPIC2_CLK_PIN (GPIO_OUTPUT | GPIO_FUNC_GPIO | GPIO_PORT1 | \
                              GPIO_PIN(19))
#define BOARD_QSPIC2_CS_PIN  (GPIO_OUTPUT | GPIO_VALUE_ONE | GPIO_FUNC_GPIO | \
                              GPIO_PORT1 | GPIO_PIN(24))
#define BOARD_QSPIC2_D0_PIN  (GPIO_INPUT | GPIO_FLOAT | GPIO_FUNC_GPIO | \
                              GPIO_PORT1 | GPIO_PIN(15))
#define BOARD_QSPIC2_D1_PIN  (GPIO_INPUT | GPIO_FLOAT | GPIO_FUNC_GPIO | \
                              GPIO_PORT1 | GPIO_PIN(14))
#define BOARD_QSPIC2_D2_PIN  (GPIO_INPUT | GPIO_FLOAT | GPIO_FUNC_GPIO | \
                              GPIO_PORT1 | GPIO_PIN(13))
#define BOARD_QSPIC2_D3_PIN  (GPIO_INPUT | GPIO_FLOAT | GPIO_FUNC_GPIO | \
                              GPIO_PORT1 | GPIO_PIN(20))

/* Timer PWM ***************************************************************/

/* A general purpose timer output, brought out on a pin that nothing else
 * on this kit uses.  A vibration motor would go here; with nothing fitted
 * it is also the one pin whose waveform can be checked, because the
 * converter can read the same pin back as ADC0 and its average follows
 * the duty cycle.
 */

#define BOARD_PWM_TIM3_PIN  (GPIO_OUTPUT | GPIO_FUNC_TIM3_PWM | \
                             GPIO_PORT0 | GPIO_PIN(5))


/* LCDC / E120A390QSR display pins ******************************************/

/* JDI parallel interface (CONFIG_DA14706_LCD_LPM012M134B, no such panel on
 * the devkit): the signals have fixed pads handed to the LCDC through
 * LCDC_MAP_CTRL; only the panel enable is a board choice.
 */

#define BOARD_JDI_VCK_PIN    (GPIO_OUTPUT | GPIO_FUNC_GPIO | GPIO_PORT0 | GPIO_PIN(9))
#define BOARD_JDI_HCK_PIN    (GPIO_OUTPUT | GPIO_FUNC_GPIO | GPIO_PORT0 | GPIO_PIN(14))
#define BOARD_JDI_HST_PIN    (GPIO_OUTPUT | GPIO_FUNC_GPIO | GPIO_PORT0 | GPIO_PIN(15))
#define BOARD_JDI_VST_PIN    (GPIO_OUTPUT | GPIO_FUNC_GPIO | GPIO_PORT0 | GPIO_PIN(16))
#define BOARD_JDI_RED0_PIN   (GPIO_OUTPUT | GPIO_FUNC_GPIO | GPIO_PORT0 | GPIO_PIN(17))
#define BOARD_JDI_ENB_PIN    (GPIO_OUTPUT | GPIO_FUNC_GPIO | GPIO_PORT0 | GPIO_PIN(18))
#define BOARD_JDI_VCOM_PIN   (GPIO_OUTPUT | GPIO_FUNC_GPIO | GPIO_PORT0 | GPIO_PIN(19))
#define BOARD_JDI_BLUE1_PIN  (GPIO_OUTPUT | GPIO_FUNC_GPIO | GPIO_PORT0 | GPIO_PIN(21))
#define BOARD_JDI_XRST_PIN   (GPIO_OUTPUT | GPIO_FUNC_GPIO | GPIO_PORT0 | GPIO_PIN(22))
#define BOARD_JDI_RED1_PIN   (GPIO_OUTPUT | GPIO_FUNC_GPIO | GPIO_PORT0 | GPIO_PIN(23))
#define BOARD_JDI_GREEN0_PIN (GPIO_OUTPUT | GPIO_FUNC_GPIO | GPIO_PORT0 | GPIO_PIN(24))
#define BOARD_JDI_GREEN1_PIN (GPIO_OUTPUT | GPIO_FUNC_GPIO | GPIO_PORT1 | GPIO_PIN(0))
#define BOARD_JDI_BLUE0_PIN  (GPIO_OUTPUT | GPIO_FUNC_GPIO | GPIO_PORT1 | GPIO_PIN(1))
#define BOARD_JDI_PEN_PIN    (GPIO_OUTPUT | GPIO_FUNC_GPIO | GPIO_PORT1 | GPIO_PIN(7))

/* Daughterboard "da1470x-sb-E120A390QSR" hard-straps the panel into QSPI
 * mode.  The LCDC pad mux is hard wired: SCLK/SD0..SD3/CSX are set as plain
 * GPIO outputs and the LCDC takes them over through LCDC_GPIO_REG.
 * RST and DCDC_EN are ordinary GPIO outputs driven by the panel driver.
 */

#define BOARD_LCDC_SCLK_PIN (GPIO_OUTPUT | GPIO_FUNC_GPIO | GPIO_PORT0 | \
                             GPIO_PIN(14))
#define BOARD_LCDC_SD0_PIN  (GPIO_OUTPUT | GPIO_FUNC_GPIO | GPIO_PORT0 | \
                             GPIO_PIN(15))
#define BOARD_LCDC_SD1_PIN  (GPIO_OUTPUT | GPIO_FUNC_GPIO | GPIO_PORT0 | \
                             GPIO_PIN(16))
#define BOARD_LCDC_SD3_PIN  (GPIO_OUTPUT | GPIO_FUNC_GPIO | GPIO_PORT0 | \
                             GPIO_PIN(17))
#define BOARD_LCDC_CSX_PIN  (GPIO_OUTPUT | GPIO_FUNC_GPIO | GPIO_PORT0 | \
                             GPIO_PIN(18))
#define BOARD_LCDC_SD2_PIN  (GPIO_OUTPUT | GPIO_FUNC_GPIO | GPIO_PORT0 | \
                             GPIO_PIN(22))
#define BOARD_LCDC_TE_PIN   (GPIO_INPUT | GPIO_FUNC_GPIO | GPIO_PORT0 | \
                             GPIO_PIN(10))
#define BOARD_LCDC_RST_PIN  (GPIO_OUTPUT | GPIO_FUNC_GPIO | GPIO_PORT0 | \
                             GPIO_PIN(23))
#define BOARD_LCDC_DCDC_PIN (GPIO_OUTPUT | GPIO_FUNC_GPIO | GPIO_PORT1 | \
                             GPIO_PIN(7))

/* Interface mode straps of the panel: IM[1:0] = 10 selects quad SPI */

#define BOARD_LCDC_IM0_PIN  (GPIO_OUTPUT | GPIO_FUNC_GPIO | GPIO_PORT0 | \
                             GPIO_PIN(24))
#define BOARD_LCDC_IM1_PIN  (GPIO_OUTPUT | GPIO_VALUE_ONE | GPIO_FUNC_GPIO | \
                             GPIO_PORT1 | GPIO_PIN(0))

/* Audio unit.  The kit carries an analogue microphone, a CMM-3729AT, fed
 * from P0.21 through a 100 ohm series resistor.  Its output reaches the
 * voice detector directly through a zero ohm link, and the amplifier
 * through a coupling capacitor.  Those inputs share their pads with P1.05 and P1.06, which come
 * out of reset as digital pins with a pull resistor, so both have to be
 * put in analogue mode or they load the microphone signal away.
 */

#define BOARD_MIC_PWR_PIN   (GPIO_OUTPUT | GPIO_FUNC_GPIO | GPIO_PORT0 | \
                             GPIO_PIN(21))
#define BOARD_PGA_P_PIN     (GPIO_INPUT | GPIO_FLOAT | GPIO_FUNC_ADC | \
                             GPIO_PORT1 | GPIO_PIN(5))
#define BOARD_PGA_N_PIN     (GPIO_INPUT | GPIO_FLOAT | GPIO_FUNC_ADC | \
                             GPIO_PORT1 | GPIO_PIN(6))

/* Pins of the two serial audio interfaces.  Nothing on this kit uses
 * them; they are a free choice that has to be matched to whatever gets
 * wired up, and P1.05 and P1.06 must not be among them.
 */

#define BOARD_PDM_DATA_PIN  (GPIO_INPUT | GPIO_FUNC_PDM_DATA | \
                             GPIO_PORT1 | GPIO_PIN(4))
#define BOARD_PDM_CLK_PIN   (GPIO_OUTPUT | GPIO_FUNC_PDM_CLK | \
                             GPIO_PORT1 | GPIO_PIN(5))

#define BOARD_PCM_DI_PIN    (GPIO_INPUT | GPIO_FUNC_PCM_DI | \
                             GPIO_PORT1 | GPIO_PIN(6))
#define BOARD_PCM_DO_PIN    (GPIO_OUTPUT | GPIO_FUNC_PCM_DO | \
                             GPIO_PORT1 | GPIO_PIN(9))
#define BOARD_PCM_FSC_PIN   (GPIO_OUTPUT | GPIO_FUNC_PCM_FSC | \
                             GPIO_PORT1 | GPIO_PIN(10))
#define BOARD_PCM_CLK_PIN   (GPIO_OUTPUT | GPIO_FUNC_PCM_CLK | \
                             GPIO_PORT1 | GPIO_PIN(13))

#endif /* __BOARDS_ARM_DA1470X_DA14706_00HZDEVKT_P_INCLUDE_BOARD_H */
