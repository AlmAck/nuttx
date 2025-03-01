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

#include <stdbool.h>
#include <assert.h>

#include "arm_internal.h"
#include "hardware/da1470x_memorymap.h"
#include "hardware/da1470x_uart.h"
#include "hardware/da1470x_crg_snc.h"
#include "hardware/da1470x_crg_top.h"

#include "da1470x_config.h"
#include "da1470x_clockconfig.h"
#include "da1470x_gpio.h"
#include "da1470x_lowputc.h"

#include <arch/board/board.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifdef HAVE_UART_CONSOLE

#ifdef CONFIG_UART0_SERIAL_CONSOLE
#  define CONSOLE_BASE     DA1470X_UART0_BASE
#  define CONSOLE_BAUD     CONFIG_UART0_BAUD
#  define CONSOLE_BITS     CONFIG_UART0_BITS
#  define CONSOLE_PARITY   CONFIG_UART0_PARITY
#  define CONSOLE_2STOP    CONFIG_UART0_2STOP
#  define CONSOLE_TX_PIN   BOARD_UART0_TX_PIN
#  define CONSOLE_RX_PIN   BOARD_UART0_RX_PIN
#elif CONFIG_UART1_SERIAL_CONSOLE
#  define CONSOLE_BASE     DA1470X_UART1_BASE
#  define CONSOLE_BAUD     CONFIG_UART1_BAUD
#  define CONSOLE_BITS     CONFIG_UART1_BITS
#  define CONSOLE_PARITY   CONFIG_UART1_PARITY
#  define CONSOLE_2STOP    CONFIG_UART1_2STOP
#  define CONSOLE_TX_PIN   BOARD_UART1_TX_PIN
#  define CONSOLE_RX_PIN   BOARD_UART1_RX_PIN
#elif CONFIG_UART2_SERIAL_CONSOLE
#  define CONSOLE_BASE     DA1470X_UART2_BASE
#  define CONSOLE_BAUD     CONFIG_UART2_BAUD
#  define CONSOLE_BITS     CONFIG_UART2_BITS
#  define CONSOLE_PARITY   CONFIG_UART2_PARITY
#  define CONSOLE_2STOP    CONFIG_UART2_2STOP
#  define CONSOLE_TX_PIN   BOARD_UART2_TX_PIN
#  define CONSOLE_RX_PIN   BOARD_UART2_RX_PIN
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* UART console configuration */

static const struct uart_config_s g_console_config =
{
  .baud      = CONSOLE_BAUD,
  .parity    = CONSOLE_PARITY,
  .bits      = CONSOLE_BITS,
  .stopbits2 = CONSOLE_2STOP,
  .txpin     = CONSOLE_TX_PIN,
  .rxpin     = CONSOLE_RX_PIN,
};
#endif /* HAVE_UART_CONSOLE */

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/* Function to configure UART serial clock input
 * sclk is false to use DivN
 * sclk id true to use Div1
 */

void da1470x_uart_set_sclk(int uart, bool sclk)
{
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

#ifdef HAVE_UART_DEVICE
static void da1470x_setbaud(uintptr_t base, const struct uart_config_s *config)
{
  uint32_t br = 0;
  uint32_t cr;

  switch (config->baud)
    {
      case 1200:
        {
          br = UART_BAUDRATE_1200;
          break;
        }

      case 2400:
        {
          br = UART_BAUDRATE_2400;
          break;
        }

      case 4800:
        {
          br = UART_BAUDRATE_4800;
          break;
        }

      case 9600:
        {
          br = UART_BAUDRATE_9600;
          break;
        }

      case 14400:
        {
          br = UART_BAUDRATE_14400;
          break;
        }

      case 19200:
        {
          br = UART_BAUDRATE_19200;
          break;
        }

      case 28800:
        {
          br = UART_BAUDRATE_28800;
          break;
        }

#ifdef UART_BAUDRATE_31250
      case 31250:
        {
          br = UART_BAUDRATE_31250;
          break;
        }
#endif

      case 38400:
        {
          br = UART_BAUDRATE_38400;
          break;
        }

#ifdef UART_BAUDRATE_56000
      case 56000:
        {
          br = UART_BAUDRATE_56000;
          break;
        }
#endif

      case 57600:
        {
          br = UART_BAUDRATE_57600;
          break;
        }

      case 115200:
        {
          br = UART_BAUDRATE_115200;
          break;
        }

      case 230400:
        {
          br = UART_BAUDRATE_230400;
          break;
        }

      case 460800:
        {
          br = UART_BAUDRATE_460800;
          break;
        }

      case 921600:
        {
          br = UART_BAUDRATE_921600;
          break;
        }

      case 1000000:
        {
          br = UART_BAUDRATE_1000000;
          break;
        }

      default:
        {
          DEBUGPANIC();
          break;
        }
    }

  da1470x_uart_set_sclk(base, true);

// TODO
//if (baud_rate < 0x100) { /* HW_UART_BAUDRATE_2000000 = 0x100*/
//  manage special cases with higher frequency and calculate divisor
// }
//else{
  uint32_t divisor = br;
//}

  /* Set Divisor Latch Access Bit in LCR register to access DLL & DLH registers */

  cr  = getreg32(base + DA1470_UART_LCR_OFFSET);
  cr |= UART_LCR_UART_DLAB;
  putreg32(cr, base + DA1470_UART_LCR_OFFSET);

  /* Set fraction byte of baud rate */

  cr  = getreg32(base + DA1470_UART_DLF_OFFSET);
  cr  = 0xff & divisor;
  putreg32(cr, base + DA1470_UART_DLF_OFFSET);

  /* Set low byte of baud rate */

  cr  = getreg32(base + DA1470_UART_RBR_THR_DLL_OFFSET);
  cr  = 0xff & (divisor >> 8);
  putreg32(cr, base + DA1470_UART_RBR_THR_DLL_OFFSET);

  /* Set high byte of baud rare */

  cr  = getreg32(base + DA1470_UART_IER_DLH_OFFSET);
  cr  = 0xff & (divisor >> 16);
  putreg32(cr, base + DA1470_UART_IER_DLH_OFFSET);

  /* Reset Divisor Latch Access Bit in LCR register */

  cr  = getreg32(base + DA1470_UART_LCR_OFFSET);
  cr &= UART_LCR_UART_DLAB;
  putreg32(cr, base + DA1470_UART_LCR_OFFSET);
}

/****************************************************************************
 * Name: da1470x_setparity
 ****************************************************************************/

static void da1470x_setparity(uintptr_t base,
                            const struct uart_config_s *config)
{
  uint32_t regval = 0;

  regval = getreg32(base + DA1470_UART_LCR_OFFSET);

  // TODO missin UART_LCR_UART_PEN_MASK enable parity

  if (config->parity == 2)
    {
      /* Include even parity */

      regval |= UART_LCR_UART_EPS;
    }
  else
    {
      /* Exclude parity */

      regval &= ~UART_LCR_UART_EPS;
    }

  putreg32(regval, base + DA1470_UART_LCR_OFFSET);
}

/****************************************************************************
 * Name: da1470x_setstops
 ****************************************************************************/

#ifdef HAVE_UART_STOPBITS
static void da1470x_setstops(uintptr_t base,
                           const struct uart_config_s *config)
{
  uint32_t regval = 0;

  regval = getreg32(base + DA1470_UART_LCR_OFFSET);

  if (config->stopbits2 == true)
    {
      regval |= UART_LCR_UART_STOP;
    }
  else
    {
      regval &= ~UART_LCR_UART_STOP;
    }

  putreg32(regval, base + DA1470_UART_LCR_OFFSET);
}
#endif

/****************************************************************************
 * Name: da1470x_sethwflow
 ****************************************************************************/
#ifdef CONFIG_SERIAL_AUTO_FLOW_CONTROL
static void da1470x_sethwflow(uintptr_t base,
                            const struct uart_config_s *config)
{
  uint32_t regval = 0;

  // Configure Flow Control in MCR register
  regval = getreg32(base + DA1470_UART_MCR_OFFSET);

  /* TODO */
  // // Set Auto flow control
  //       HW_UART_REG_SETF(uart, MCR, UART_AFCE, uart_init->auto_flow_control);
  //       HW_UART_REG_SETF(uart, MCR, UART_RTS, uart_init->auto_flow_control);

    // Configure Flow Control in MCR register
    regval = getreg32(base + DA1470_UART_MCR_OFFSET);

    // Set Auto Flow Control (AFCE: bit 5, RTS: bit 1)
    if (config->auto_flow_control)
    {
        regval |= (UART_MCR_AFCE | UART_MCR_RTS);
    }
    else
    {
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

void da1470x_lowsetup(void)
{
#ifdef HAVE_UART_DEVICE
#ifdef HAVE_UART_CONSOLE
  /* Configure the console UART (if any) */

  // Enable PowerDomain snc to use the uart TODO create a proper "class" pm separately
  da1470x_enable_snc();

  da1470x_uart_configure(CONSOLE_BASE, &g_console_config);

#endif /* HAVE_UART_CONSOLE */
#endif /* HAVE_UART_DEVICE */
}

void da1470x_enable_snc(void)
{
    irqstate_t flags;

    // // Disable global interrupts
    // flags = irqsave();

    // Clear the CRG_TOP_SNC_SLEEP bit in the DA1470_CRG_TOP_PMU_CTRL
    putreg32(getreg32(DA1470_CRG_TOP_PMU_CTRL) & ~CRG_TOP_SNC_SLEEP, DA1470_CRG_TOP_PMU_CTRL);

    // // Restore global interrupts
    // irqrestore(flags);

    // Wait until the SNC_IS_UP bit in SYS_STAT_REG is set
    while ((getreg32(DA1470_CRG_TOP_SYS_STAT) & CRG_TOP_SNC_IS_UP) == 0);
}

/****************************************************************************
 * Name: da1470x_uart_configure
 *
 * Description:
 *   Configure a UART for non-interrupt driven operation
 *
 ****************************************************************************/
// 1. Set up the GPIOs to be used for the UART interface (Px_yy_MODE_REG[PID]).
// 2. Select the UART clock (CLK_SNC_REG[UARTx_CLK_SEL]).
// 3. Enable the selected UART by setting the CLK_SNC_REG [UARTx_ENABLE] bit.
// 4. Enable access to Divisor Latch Registers (DLL and DLH) by setting the
//    UARTx_LCR_REG[UART_DLAB] bit.
// 5. Set the desired baud rate. To calculate the registers values for the desired baud rate, use the
//    formula: Divisor = UART CLK / (16 x Baud rate).
//    a. UARTx_IER_DLH_REG: High byte of the Divisor integer part.
//    b. UARTx_RBR_THR_DLL_REG: Low byte of the Divisor integer part.
//    c. UARTx_DLF_REG: The fractional part of the Divisor.
// 6. Configure the brake control bit, parity, number of stop bits, and data length (UARTx_LCR_REG).
// 7. Enable and configure the FIFO (UARTx_IIR_FCR_REG).
// 8. Configure the generated interrupts, if needed (UARTx_IER_DLH_REG).
// 9. Send a byte:
//    a. Check if Transmit Hold Register (THR) is empty (UARTx_LSR_REG[UART_THRE]).
//    b. Load the byte to THR (UARTx_RBR_THR_DLL_REG).
//    c. Check if the byte was transmitted (UARTx_LSR_REG[UART_TEMT]).
// 10. Receive a byte:
//     a. Wait until serial data is ready (UARTx_LSR_REG[UART_DR]).
//     b. Read the incoming byte from the THR (UARTx_RBR_THR_DLL_REG).


#ifdef HAVE_UART_DEVICE
void da1470x_uart_configure(uintptr_t base,
                           const struct uart_config_s *config)
{
  uint32_t pin    = 0;
  uint32_t port   = 0;
  uint32_t regval = 0;

  /* Config GPIO pins for uart */

  da1470x_gpio_config(config->txpin);
  da1470x_gpio_config(config->rxpin);

  /* Enable the DA1470x UART Clock */

  da1470x_uart_enable();

  /* Set UART format */

  da1470x_uart_setformat(base, config);

  /* Enable and configure the FIFO */

  /* Configure the generated interrupts */
  //TODO
}

/****************************************************************************
 * Name: da1470x_uart_enable
 *
 * Description:
 *   Enable a UART.  it will be necessary to again call
 *   da1470x_uart_configure() in order to use this UART channel again.
 *
 ****************************************************************************/

void da1470x_uart_enable()
{
  uint32_t regval = 0;

  /* Enable the UART clock */

  #ifdef CONFIG_UART0_SERIAL_CONSOLE
    regval  = getreg32(DA1470_CRG_SNC_CLK_SNC);
    regval |= CRG_SNC_UART0_ENABLE;
    // regval |= CRG_SNC_UART0_CLK_SEL;
    putreg32(regval, DA1470_CRG_SNC_CLK_SNC);
  #elif CONFIG_UART1_SERIAL_CONSOLE
    regval  = getreg32(DA1470_CRG_SNC_CLK_SNC);
    regval |= CRG_SNC_UART1_ENABLE;
    // regval |= CRG_SNC_UART1_CLK_SEL;
    putreg32(regval, DA1470_CRG_SNC_CLK_SNC);
  #elif CONFIG_UART2_SERIAL_CONSOLEb
    regval  = getreg32(DA1470_CRG_SNC_CLK_SNC);
    regval |= CRG_SNC_UART2_ENABLE;
    // regval |= CRG_SNC_UART2_CLK_SEL;
    putreg32(regval, DA1470_CRG_SNC_CLK_SNC);
  #endif

  /* Enable interrupts */

  /* Enable the UART */
}

/****************************************************************************
 * Name: da1470x_uart_disable
 *
 * Description:
 *   Disable a UART.
 *
 ****************************************************************************/

void da1470x_uart_disable()
{
  uint32_t regval = 0;

  /* Disable the UART clock */

  #ifdef CONFIG_UART0_SERIAL_CONSOLE
    regval  = getreg32(DA1470_CRG_SNC_CLK_SNC);
    regval &= ~CRG_SNC_UART1_ENABLE;
    putreg32(regval, DA1470_CRG_SNC_CLK_SNC);
  #elif CONFIG_UART1_SERIAL_CONSOLE
    regval  = getreg32(DA1470_CRG_SNC_CLK_SNC);
    regval &= ~CRG_SNC_UART2_ENABLE;
    putreg32(regval, DA1470_CRG_SNC_CLK_SNC);
  #elif CONFIG_UART2_SERIAL_CONSOLE
    regval  = getreg32(DA1470_CRG_SNC_CLK_SNC);
    regval &= ~CRG_SNC_UART3_ENABLE;
    putreg32(regval, DA1470_CRG_SNC_CLK_SNC);
  #endif

  /* Disable interrupts */

  /* Disable the UART */

  /* Unconfigure GPIO */

  //TODO
  // da1470x_gpio_unconfig(config->rxpin);
  // da1470x_gpio_unconfig(config->txpin);
}

/****************************************************************************
 * Name: da1470x_uart_setformat
 *
 * Description:
 *   Set the USART line format and speed.
 *
 ****************************************************************************/

void da1470x_uart_setformat(uintptr_t base,
                           const struct uart_config_s *config)
{
  /* Configure baud */

  da1470x_setbaud(base, config);

  /* Configure polarity */

  da1470x_setparity(base, config);

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

void arm_lowputc(char ch)
{
#ifdef HAVE_UART_CONSOLE
  /* Wait until the TX data register is empty */

  while ((getreg32(CONSOLE_BASE + DA1470_UART_LSR_OFFSET) &
                   UART_LSR_UART_TEMT) == 0);

  /* Then send the character */

  putreg32((uint32_t)ch, CONSOLE_BASE + DA1470_UART_RBR_THR_DLL_OFFSET);

#endif
}
