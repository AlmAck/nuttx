/****************************************************************************
 * arch/arm/src/da1470x/da1470x_lowputc.c
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <assert.h>
#include <stdbool.h>

#include "arm_internal.h"
#include "hardware/da1470x_crg_snc.h"
#include "hardware/da1470x_crg_top.h"
#include "hardware/da1470x_dma.h"
#include "hardware/da1470x_memorymap.h"
#include "hardware/da1470x_uart.h"
#include "hardware/da1470x_vad.h"

#include "da1470x_clockconfig.h"
#include "da1470x_config.h"
#include "da1470x_gpio.h"
#include "da1470x_irq.h"
#include "da1470x_lowputc.h"
#include "da1470x_pmu.h"
#include "nvic.h"

#include <arch/board/board.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifdef HAVE_UART_CONSOLE

#ifdef CONFIG_UART0_SERIAL_CONSOLE
#define CONSOLE_BASE DA1470X_UART0_BASE
#define CONSOLE_BAUD CONFIG_UART0_BAUD
#define CONSOLE_BITS CONFIG_UART0_BITS
#define CONSOLE_PARITY CONFIG_UART0_PARITY
#define CONSOLE_2STOP CONFIG_UART0_2STOP
#define CONSOLE_TX_PIN BOARD_UART0_TX_PIN
#define CONSOLE_RX_PIN BOARD_UART0_RX_PIN
#elif CONFIG_UART1_SERIAL_CONSOLE
#define CONSOLE_BASE DA1470X_UART1_BASE
#define CONSOLE_BAUD CONFIG_UART1_BAUD
#define CONSOLE_BITS CONFIG_UART1_BITS
#define CONSOLE_PARITY CONFIG_UART1_PARITY
#define CONSOLE_2STOP CONFIG_UART1_2STOP
#define CONSOLE_TX_PIN BOARD_UART1_TX_PIN
#define CONSOLE_RX_PIN BOARD_UART1_RX_PIN
#elif CONFIG_UART2_SERIAL_CONSOLE
#define CONSOLE_BASE DA1470X_UART2_BASE
#define CONSOLE_BAUD CONFIG_UART2_BAUD
#define CONSOLE_BITS CONFIG_UART2_BITS
#define CONSOLE_PARITY CONFIG_UART2_PARITY
#define CONSOLE_2STOP CONFIG_UART2_2STOP
#define CONSOLE_TX_PIN BOARD_UART2_TX_PIN
#define CONSOLE_RX_PIN BOARD_UART2_RX_PIN
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* UART console configuration */

static const struct uart_config_s g_console_config = {
    .baud = CONSOLE_BAUD,
    .parity = CONSOLE_PARITY,
    .bits = CONSOLE_BITS,
    .stopbits2 = CONSOLE_2STOP,
    .txpin = CONSOLE_TX_PIN,
    .rxpin = CONSOLE_RX_PIN,
};
#endif /* HAVE_UART_CONSOLE */

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/* Function to configure UART serial clock input
 * If sclk is false, the function uses DivN (a divided clock source).
 * If sclk is true, the function uses Div1 (an undivided clock source, meaning
 * the clock runs at full speed).
 */

void da1470x_uart_set_sclk(int uart, bool sclk) {
  uintptr_t reg;
  uint32_t mask;

  /* Select the appropriate register based on sclk */

  reg = sclk ? DA1470_CRG_SNC_SET_CLK_SNC : DA1470_CRG_SNC_RESET_CLK_SNC;

  /* Determine the appropriate mask for the UART */

  switch (uart) {
  case DA1470X_UART0_BASE:
    mask = CRG_SNC_UART0_CLK_SEL;
    break;
  case DA1470X_UART1_BASE:
    mask = CRG_SNC_UART1_CLK_SEL;
    break;
  case DA1470X_UART2_BASE:
    mask = CRG_SNC_UART2_CLK_SEL;
    break;
  default:
    assert(false); /* Invalid UART instance */
    return;
  }

  /* Write the mask to the selected register */

  putreg32(mask, reg);
}

/****************************************************************************
 * Name: da1470x_setbaud
 *
 * Description:
 *   Configure the UART BAUD.
 *
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_get_uart_clk
 *
 * Description:
 *   Get the UART clock frequency.
 *
 ****************************************************************************/

static uint32_t da1470x_get_uart_clk(void) {
  /* DEBUG: hardcode 32 MHz to validate the rest of the UART setup path
   * regardless of what the RCHS/PLL is actually doing. Revert once serial
   * is confirmed working.
   */
  return 32000000;

  sys_clk_is_t sys_clk = hw_clk_get_sysclk();
  uint32_t freq = 32000000; /* Default safe frequency */

  if (sys_clk == SYS_CLK_IS_RCHS) {
    rchs_speed_t mode = hw_clk_get_rchs_mode();
    if (mode == RCHS_96) {
      freq = 96000000;
    } else if (mode == RCHS_64) {
      freq = 64000000;
    } else {
      freq = 32000000;
    }
  } else if (sys_clk == SYS_CLK_IS_PLL) {
    freq = 160000000;
  } else if (sys_clk == SYS_CLK_IS_XTAL32M) {
    freq = 32000000;
  }

  /* Assuming Div1 is used for UART clock source.
   * If DivN is used, we would need to read the dividers.
   * For now, we force Div1 in setbaud.
   */
  return freq;
}

/****************************************************************************
 * Name: da1470x_setbaud
 *
 * Description:
 *   Configure the UART BAUD.
 *
 ****************************************************************************/

#ifdef HAVE_UART_DEVICE
static void da1470x_setbaud(uintptr_t base,
                            const struct uart_config_s *config) {
  uint32_t baud_rate = config->baud;
  uint32_t uart_clk;
  uint32_t divisor;
  uint32_t dlf;
  uint32_t cr;
  /* Use DivN clock (32 MHz on DA1470x regardless of sysclk speed) to match
   * the Renesas SDK default. Selecting DIV1 (= sysclk, e.g. 96 MHz on RCHS)
   * would require recomputing the divisor against that clock — but the
   * per-baud divisor constants documented in the datasheet (UART_BAUDRATE_*)
   * assume 32 MHz DivN.
   */
  bool sclk = false; /* DivN = 32 MHz */

  da1470x_uart_set_sclk(base, sclk);

  /* UART runs from DivN = 32 MHz */
  uart_clk = 32000000;

  /* Calculate Divisor
   * Divisor = UART_CLK / (16 * BaudRate)
   * We calculate it as integer + fractional part.
   * DA1470x has 16-bit Divisor and 4-bit Fractional part (DLF).
   */

  /* Integer part */
  divisor = uart_clk / (16 * baud_rate);

  /* Fractional part (4 bits)
   * Fraction = (Remainder * 16 + Half_Baud_Step) / (16 * BaudRate)
   * Simplified: DLF = ((uart_clk % (16 * baud)) * 16 + (16 * baud / 2)) / (16 *
   * baud)? Actually: remainder = uart_clk % (16 * baud_rate) dlf_val   =
   * (remainder * 16) / (16 * baud_rate) = remainder / baud_rate Rounding:
   * (remainder * 16 + (16 * baud / 2)) ... no. Standard way: val = (uart_clk *
   * 16) / (baud_rate * 16) -- no val_x16 = uart_clk / baud_rate divisor =
   * val_x16 >> 4 dlf = val_x16 & 0xF
   */

  /* Round to nearest to match the Renesas-published UART_BAUDRATE_* constants
   * (e.g. 115200 -> 0x1106 with DLL=0x11=17 and DLF=0x06, not 0x05 from
   * truncation).
   */
  uint32_t clk_per_bit = (uart_clk + (baud_rate / 2)) / baud_rate;
  divisor = clk_per_bit >> 4;
  dlf = clk_per_bit & 0xF;

  /* Check for validity */
  if (divisor == 0) {
    divisor = 1;
    dlf = 0;
  }

  /* Set Divisor Latch Access Bit in LCR register to access DLL & DLH registers
   */

  cr = getreg32(base + DA1470_UART_LCR_OFFSET);
  cr |= UART_LCR_DLAB;
  putreg32(cr, base + DA1470_UART_LCR_OFFSET);

  /* Set fraction byte of baud rate (DLF is a 4-bit register) */

  putreg32(dlf & 0xF, base + DA1470_UART_DLF_OFFSET);

  /* Set low byte of baud rate */

  cr = getreg32(base + DA1470_UART_RBR_THR_DLL_OFFSET);
  cr = 0xff & divisor;
  putreg32(cr, base + DA1470_UART_RBR_THR_DLL_OFFSET);

  /* Set high byte of baud rate */

  cr = getreg32(base + DA1470_UART_IER_DLH_OFFSET);
  cr = 0xff & (divisor >> 8);
  putreg32(cr, base + DA1470_UART_IER_DLH_OFFSET);

  /* Reset Divisor Latch Access Bit in LCR register */

  cr = getreg32(base + DA1470_UART_LCR_OFFSET);
  cr &= ~UART_LCR_DLAB;
  putreg32(cr, base + DA1470_UART_LCR_OFFSET);
}
#endif

/****************************************************************************
 * Name: da1470x_setparity
 ****************************************************************************/

static void da1470x_setparity(uintptr_t base,
                              const struct uart_config_s *config) {
  uint32_t regval = 0;

  regval = getreg32(base + DA1470_UART_LCR_OFFSET);

  if (config->parity == 1) /* Odd parity */
  {
    regval |= UART_LCR_PEN;
    regval &= ~UART_LCR_EPS;
  } else if (config->parity == 2) /* Even parity */
  {
    regval |= UART_LCR_PEN;
    regval |= UART_LCR_EPS;
  } else /* No parity */
  {
    regval &= ~UART_LCR_PEN;
    regval &= ~UART_LCR_EPS;
  }

  putreg32(regval, base + DA1470_UART_LCR_OFFSET);
}

/****************************************************************************
 * Name: da1470x_data_bits
 ****************************************************************************/

static void da1470x_data_bits(uintptr_t base,
                              const struct uart_config_s *config) {
  uint32_t regval = 0;
  uint32_t data_bits = 0;

  /* Read the current LCR register */

  regval = getreg32(base + DA1470_UART_LCR_OFFSET);

  switch (config->bits) {
  case 5:
    data_bits = 0b00;
    break;
  case 6:
    data_bits = 0b01;
    break;
  case 7:
    data_bits = 0b10;
    break;
  case 8:
  default:
    data_bits = 0b11;
    break;
  }

  /* Set Data Bits (DLS: bits [1:0])  */

  regval &= ~UART_LCR_WLS_MASK;              // Clear previous data bits setting
  regval |= (data_bits & UART_LCR_WLS_MASK); // Apply new data bits setting

  putreg32(regval, base + DA1470_UART_LCR_OFFSET);
}

/****************************************************************************
 * Name: da1470x_setstops
 ****************************************************************************/

#ifdef HAVE_UART_STOPBITS
static void da1470x_setstops(uintptr_t base,
                             const struct uart_config_s *config) {
  uint32_t regval = 0;

  regval = getreg32(base + DA1470_UART_LCR_OFFSET);

  if (config->stopbits2 == true) {
    regval |= UART_LCR_STOP;
  } else {
    regval &= ~UART_LCR_STOP;
  }

  putreg32(regval, base + DA1470_UART_LCR_OFFSET);
}
#endif

/****************************************************************************
 * Name: da1470x_sethwflow
 ****************************************************************************/
#ifdef CONFIG_SERIAL_AUTO_FLOW_CONTROL
static void da1470x_sethwflow(uintptr_t base,
                              const struct uart_config_s *config) {
  uint32_t regval = 0;

  // Configure Flow Control in MCR register
  regval = getreg32(base + DA1470_UART_MCR_OFFSET);

  // Set Auto Flow Control (AFCE: bit 5, RTS: bit 1)
  if (config->auto_flow_control) {
    regval |= (UART_MCR_AFCE | UART_MCR_RTS);
  } else {
    regval &= ~(UART_MCR_AFCE | UART_MCR_RTS);
  }

  // Write updated MCR register
  putreg32(regval, base + DA1470_UART_MCR_OFFSET);
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_lowsetup
 *
 * Description:
 *   Called at the very beginning of _start. Performs low level
 *   initialization including setup of the console UART.
 *   This UART initialization is done early so that the serial console is
 *   available for debugging very early in the boot sequence.
 *
 ****************************************************************************/

void da1470x_lowsetup(void) {
#ifdef HAVE_UART_DEVICE
#ifdef HAVE_UART_CONSOLE
  /* Configure the console UART (if any) */

  /* PD_SNC hosts UART/I2C/SPI on the DA1470x; bring it up before
   * touching any peripheral inside this domain.
   */

  da1470x_pd_enable(DA1470X_PD_SNC);

  da1470x_uart_configure(CONSOLE_BASE, &g_console_config);

#endif /* HAVE_UART_CONSOLE */
#endif /* HAVE_UART_DEVICE */

  /* 1. Disable VAD peripheral source and clear its pending interrupt.
   * This is a "belt and suspenders" guard to prevent IRQ 45 from firing
   * before the OS has fully initialized the interrupt system.
   *
   * Note: The VAD IRQ clearing mechanism requires toggling the sleep mode
   * to force-clear certain false triggers as per datasheet recommendation.
   */

  /* Put VAD in standby and sleep */

  putreg32(VAD_CTRL3_VAD_SB | VAD_CTRL3_VAD_SLEEP, DA1470X_VAD_CTRL3);

  /* Force clear IRQ by toggling SLEEP (1 -> 0 -> 1) */

  putreg32(VAD_CTRL3_VAD_SB, DA1470X_VAD_CTRL3);
  putreg32(VAD_CTRL3_VAD_SB | VAD_CTRL3_VAD_SLEEP, DA1470X_VAD_CTRL3);

  /* Clear the IRQ flag itself */

  putreg32(VAD_CTRL4_VAD_IRQ_FLAG, DA1470X_VAD_CTRL4);

  /* 2. Reset DMA Channel 5 (often associated with VAD or misused as IRQ 45
   * source) */

  putreg32(0, DA1470_DMA_DMA5_CTRL);

  /* 3. Explicitly disable and clear IRQ 45 (VAD) in NVIC.
   * This provides an early hammer before even leaving the low-level init.
   */

  putreg32(1 << (45 % 32), ARMV8M_NVIC_BASE + NVIC_IRQ_CLEAR_OFFSET(45));
  putreg32(1 << (45 % 32), ARMV8M_NVIC_BASE + NVIC_IRQ_CLRPEND_OFFSET(45));
}


/****************************************************************************
 * Name: da1470x_uart_configure
 *
 * Description:
 *   Configure a UART for non-interrupt driven operation
 *
 ****************************************************************************/

#ifdef HAVE_UART_DEVICE
void da1470x_uart_configure(uintptr_t base,
                            const struct uart_config_s *config) {
  /* Config GPIO pins for uart */

  /* Set specific PID function for the pins */
  /* Retrieve the generic config, ensure mode is correct, and apply function */
  /* Note: config->txpin/rxpin are expected to contain the PID encoded in them
   * via board.h definitions */

  da1470x_gpio_config(config->txpin);
  da1470x_gpio_config(config->rxpin);

  /* Enable the DA1470x UART Clock */

  da1470x_uart_enable(base);

  /* Software-reset the UART so we can safely (re)configure it even if
   * the controller was left in a BUSY state (e.g. from a prior boot-time
   * lowsetup). On the DesignWare UART, writes to LCR are silently dropped
   * while USR.BUSY is set, which would prevent DLAB from latching and
   * cause divisor writes to land in THR — producing wrong baud and
   * spurious TX bytes.
   */

  putreg32(UART_SRR_UR | UART_SRR_RFR | UART_SRR_XFR,
           base + DA1470_UART_SRR_OFFSET);

  /* Disable interrupts (IER) */
  putreg32(0, base + DA1470_UART_IER_DLH_OFFSET);

  /* Enable FIFO and Reset RX/TX FIFOs */
  /* Trigger level 1 character */
  putreg32(UART_FCR_FIFO_ENABLE | UART_FCR_RFR | UART_FCR_TFR |
               UART_FCR_RT_1CHAR,
           base + DA1470_UART_IIR_FCR_OFFSET);

  /* Set UART format (including baud rate) */

  da1470x_uart_setformat(base, config);
}

/****************************************************************************
 * Name: da1470x_uart_enable
 *
 * Description:
 *   Enable a UART.  it will be necessary to again call
 *   da1470x_uart_configure() in order to use this UART channel again.
 *
 ****************************************************************************/

void da1470x_uart_enable(uintptr_t base) {
  uint32_t regval = 0;

  /* Enable the UART clock in CRG_SNC */

  switch (base) {
#ifdef HAVE_UART0
  case DA1470X_UART0_BASE:
    regval = getreg32(DA1470_CRG_SNC_CLK_SNC);
    regval |= CRG_SNC_UART0_ENABLE;
    putreg32(regval, DA1470_CRG_SNC_CLK_SNC);
    break;
#endif
#ifdef HAVE_UART1
  case DA1470X_UART1_BASE:
    regval = getreg32(DA1470_CRG_SNC_CLK_SNC);
    regval |= CRG_SNC_UART1_ENABLE;
    putreg32(regval, DA1470_CRG_SNC_CLK_SNC);
    break;
#endif
#ifdef HAVE_UART2
  case DA1470X_UART2_BASE:
    regval = getreg32(DA1470_CRG_SNC_CLK_SNC);
    regval |= CRG_SNC_UART2_ENABLE;
    putreg32(regval, DA1470_CRG_SNC_CLK_SNC);
    break;
#endif
  default:
    break;
  }
}

/****************************************************************************
 * Name: da1470x_uart_disable
 *
 * Description:
 *   Disable a UART.
 *
 ****************************************************************************/

void da1470x_uart_disable(uintptr_t base) {
  uint32_t regval = 0;

  /* Disable the UART clock */

  switch (base) {
#ifdef HAVE_UART0
  case DA1470X_UART0_BASE:
    regval = getreg32(DA1470_CRG_SNC_CLK_SNC);
    regval &= ~CRG_SNC_UART0_ENABLE;
    putreg32(regval, DA1470_CRG_SNC_CLK_SNC);
    break;
#endif
#ifdef HAVE_UART1
  case DA1470X_UART1_BASE:
    regval = getreg32(DA1470_CRG_SNC_CLK_SNC);
    regval &= ~CRG_SNC_UART1_ENABLE;
    putreg32(regval, DA1470_CRG_SNC_CLK_SNC);
    break;
#endif
#ifdef HAVE_UART2
  case DA1470X_UART2_BASE:
    regval = getreg32(DA1470_CRG_SNC_CLK_SNC);
    regval &= ~CRG_SNC_UART2_ENABLE;
    putreg32(regval, DA1470_CRG_SNC_CLK_SNC);
    break;
#endif
  default:
    break;
  }
}

/****************************************************************************
 * Name: da1470x_uart_setformat
 *
 * Description:
 *   Set the USART line format and speed.
 *
 ****************************************************************************/

void da1470x_uart_setformat(uintptr_t base,
                            const struct uart_config_s *config) {
  /* Configure baud */

  da1470x_setbaud(base, config);

  /* Configure polarity */

  da1470x_setparity(base, config);

  /* Configure data bits */

  da1470x_data_bits(base, config);

#ifdef HAVE_UART_STOPBITS
  /* Configure STOP bits */

  da1470x_setstops(base, config);
#endif

  /* Configure hardware flow control */
#ifdef CONFIG_SERIAL_AUTO_FLOW_CONTROL
  da1470x_sethwflow(base, config);
#endif
}
#endif

/****************************************************************************
 * Name: arm_lowputc
 *
 * Description:
 *   Output one byte on the serial console
 *
 ****************************************************************************/

void arm_lowputc(char ch) {
#ifdef HAVE_UART_CONSOLE
  /* Wait until there is space in the TX FIFO. Use USR.TFNF rather than
   * LSR.THRE: when the buffered driver later enables IER.PTIME
   * (Programmable THRE Interrupt mode), the LSR.THRE bit can be cleared
   * by hardware as a side effect of IIR reads, which would make this
   * poll loop spin forever even though the FIFO has room.
   */

  while ((getreg32(CONSOLE_BASE + DA1470_UART_USR_OFFSET) & UART_USR_TFNF) == 0)
    ;

  putreg32((uint32_t)ch, CONSOLE_BASE + DA1470_UART_RBR_THR_DLL_OFFSET);
#endif
}
