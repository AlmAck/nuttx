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

#include <arch/board/board.h>

#include "arm_internal.h"
#include "hardware/da1470x_crg_snc.h"
#include "hardware/da1470x_uart.h"
#include "hardware/da1470x_vad.h"

#include "da1470x_clockconfig.h"
#include "da1470x_config.h"
#include "da1470x_gpio.h"
#include "da1470x_lowputc.h"
#include "da1470x_pmu.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifdef HAVE_UART_CONSOLE
#  if defined(CONFIG_UART0_SERIAL_CONSOLE)
#    define CONSOLE_BASE     DA1470X_UART0_BASE
#    define CONSOLE_BAUD     CONFIG_UART0_BAUD
#    define CONSOLE_BITS     CONFIG_UART0_BITS
#    define CONSOLE_PARITY   CONFIG_UART0_PARITY
#    define CONSOLE_2STOP    CONFIG_UART0_2STOP
#    define CONSOLE_TX_PIN   BOARD_UART0_TX_PIN
#    define CONSOLE_RX_PIN   BOARD_UART0_RX_PIN
#  elif defined(CONFIG_UART1_SERIAL_CONSOLE)
#    define CONSOLE_BASE     DA1470X_UART1_BASE
#    define CONSOLE_BAUD     CONFIG_UART1_BAUD
#    define CONSOLE_BITS     CONFIG_UART1_BITS
#    define CONSOLE_PARITY   CONFIG_UART1_PARITY
#    define CONSOLE_2STOP    CONFIG_UART1_2STOP
#    define CONSOLE_TX_PIN   BOARD_UART1_TX_PIN
#    define CONSOLE_RX_PIN   BOARD_UART1_RX_PIN
#  elif defined(CONFIG_UART2_SERIAL_CONSOLE)
#    define CONSOLE_BASE     DA1470X_UART2_BASE
#    define CONSOLE_BAUD     CONFIG_UART2_BAUD
#    define CONSOLE_BITS     CONFIG_UART2_BITS
#    define CONSOLE_PARITY   CONFIG_UART2_PARITY
#    define CONSOLE_2STOP    CONFIG_UART2_2STOP
#    define CONSOLE_TX_PIN   BOARD_UART2_TX_PIN
#    define CONSOLE_RX_PIN   BOARD_UART2_RX_PIN
#  endif
#endif

/* The DesignWare UART ignores LCR writes while USR.BUSY is set.  Bound the
 * wait for the transmitter to drain before touching the divisors.
 */

#define UART_BUSY_WAIT_LOOPS   100000

/****************************************************************************
 * Private Data
 ****************************************************************************/

#ifdef HAVE_UART_CONSOLE
static const struct uart_config_s g_console_config =
{
  .baud      = CONSOLE_BAUD,
  .parity    = CONSOLE_PARITY,
  .bits      = CONSOLE_BITS,
  .stopbits2 = CONSOLE_2STOP,
  .txpin     = CONSOLE_TX_PIN,
  .rxpin     = CONSOLE_RX_PIN,
};
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#ifdef HAVE_UART_DEVICE

/****************************************************************************
 * Name: da1470x_uart_clkbits
 *
 * Description:
 *   Return the CLK_SNC ENABLE and CLK_SEL bits of a UART instance.
 *
 ****************************************************************************/

static void da1470x_uart_clkbits(uintptr_t base, uint32_t *enable,
                                 uint32_t *sel)
{
  switch (base)
    {
      case DA1470X_UART1_BASE:
        *enable = CRG_SNC_CLK_SNC_UART2_ENABLE;
        *sel    = CRG_SNC_CLK_SNC_UART2_CLK_SEL;
        break;

      case DA1470X_UART2_BASE:
        *enable = CRG_SNC_CLK_SNC_UART3_ENABLE;
        *sel    = CRG_SNC_CLK_SNC_UART3_CLK_SEL;
        break;

      case DA1470X_UART0_BASE:
      default:
        *enable = CRG_SNC_CLK_SNC_UART_ENABLE;
        *sel    = CRG_SNC_CLK_SNC_UART_CLK_SEL;
        break;
    }
}

/****************************************************************************
 * Name: da1470x_uart_waitidle
 *
 * Description:
 *   Wait until the UART is not busy so that LCR/DLAB writes take effect.
 *   If it stays busy (for instance a stuck receiver) perform a soft reset.
 *
 ****************************************************************************/

static void da1470x_uart_waitidle(uintptr_t base)
{
  int i;

  for (i = 0; i < UART_BUSY_WAIT_LOOPS; i++)
    {
      if ((getreg32(base + DA1470X_UART_USR_OFFSET) & UART_USR_BUSY) == 0)
        {
          return;
        }
    }

  putreg32(UART_SRR_UR | UART_SRR_RFR | UART_SRR_XFR,
           base + DA1470X_UART_SRR_OFFSET);
}

/****************************************************************************
 * Name: da1470x_setbaud
 *
 * Description:
 *   Program the integer and fractional (4-bit) divisors for the requested
 *   baud rate.  The UART is clocked from DIVN (32 MHz) so that the divisor
 *   does not depend on the system clock selection.
 *
 *     divisor * 16 + dlf = round(16 * clk / (16 * baud))
 *
 ****************************************************************************/

static void da1470x_setbaud(uintptr_t base,
                            const struct uart_config_s *config)
{
  uint32_t clk = da1470x_get_divn_clk();
  uint32_t baud = config->baud;
  uint32_t div16;
  uint32_t divisor;
  uint32_t dlf;
  uint32_t lcr;

  if (baud == 0)
    {
      baud = 115200;
    }

  /* Number of clocks per bit in 1/16 units, rounded to nearest */

  div16   = (clk + (baud / 2)) / baud;
  divisor = div16 >> 4;
  dlf     = div16 & 0xf;

  if (divisor == 0)
    {
      divisor = 1;
      dlf     = 0;
    }

  da1470x_uart_waitidle(base);

  lcr = getreg32(base + DA1470X_UART_LCR_OFFSET);
  putreg32(lcr | UART_LCR_DLAB, base + DA1470X_UART_LCR_OFFSET);

  putreg32(divisor & 0xff, base + DA1470X_UART_RBR_THR_DLL_OFFSET);
  putreg32((divisor >> 8) & 0xff, base + DA1470X_UART_IER_DLH_OFFSET);
  putreg32(dlf, base + DA1470X_UART_DLF_OFFSET);

  putreg32(lcr & ~UART_LCR_DLAB, base + DA1470X_UART_LCR_OFFSET);
}

/****************************************************************************
 * Name: da1470x_setlcr
 *
 * Description:
 *   Program data bits, parity and stop bits.
 *
 ****************************************************************************/

static void da1470x_setlcr(uintptr_t base,
                           const struct uart_config_s *config)
{
  uint32_t lcr;

  da1470x_uart_waitidle(base);

  lcr  = getreg32(base + DA1470X_UART_LCR_OFFSET);
  lcr &= ~(UART_LCR_DLS_MASK | UART_LCR_STOP | UART_LCR_PEN | UART_LCR_EPS);

  switch (config->bits)
    {
      case 5:
        lcr |= UART_LCR_DLS_5BITS;
        break;

      case 6:
        lcr |= UART_LCR_DLS_6BITS;
        break;

      case 7:
        lcr |= UART_LCR_DLS_7BITS;
        break;

      case 8:
      default:
        lcr |= UART_LCR_DLS_8BITS;
        break;
    }

  if (config->parity == 1)
    {
      lcr |= UART_LCR_PEN;             /* Odd */
    }
  else if (config->parity == 2)
    {
      lcr |= UART_LCR_PEN | UART_LCR_EPS; /* Even */
    }

  if (config->stopbits2)
    {
      lcr |= UART_LCR_STOP;
    }

  putreg32(lcr, base + DA1470X_UART_LCR_OFFSET);
}

/****************************************************************************
 * Name: da1470x_sethwflow
 *
 * Description:
 *   Enable automatic RTS/CTS handling where the hardware supports it.
 *
 ****************************************************************************/

static void da1470x_sethwflow(uintptr_t base,
                              const struct uart_config_s *config)
{
  uint32_t mcr = getreg32(base + DA1470X_UART_MCR_OFFSET);

  if (base != DA1470X_UART0_BASE && (config->iflow || config->oflow))
    {
      mcr |= UART_MCR_AFCE | UART_MCR_RTS;
    }
  else
    {
      mcr &= ~(UART_MCR_AFCE | UART_MCR_RTS);
    }

  putreg32(mcr, base + DA1470X_UART_MCR_OFFSET);
}

#endif /* HAVE_UART_DEVICE */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_lowsetup
 ****************************************************************************/

void da1470x_lowsetup(void)
{
  /* PD_SNC hosts UART/I2C/SPI/DMA: bring it up before any of them */

  da1470x_pd_enable(DA1470X_PD_SNC);

#ifdef HAVE_UART_CONSOLE
  da1470x_uart_configure(CONSOLE_BASE, &g_console_config);
#endif

  /* Put the voice activity detector to sleep and clear its interrupt flag.
   * The block can come out of reset with a pending request that would
   * otherwise fire IRQ 45 before the OS is ready for it.
   */

  putreg32(VAD_CTRL3_SB | VAD_CTRL3_SLEEP, DA1470X_VAD_CTRL3);
  putreg32(VAD_CTRL3_SB, DA1470X_VAD_CTRL3);
  putreg32(VAD_CTRL3_SB | VAD_CTRL3_SLEEP, DA1470X_VAD_CTRL3);
  putreg32(VAD_CTRL4_IRQ_FLAG, DA1470X_VAD_CTRL4);
}

#ifdef HAVE_UART_DEVICE

/****************************************************************************
 * Name: da1470x_uart_configure
 ****************************************************************************/

void da1470x_uart_configure(uintptr_t base,
                            const struct uart_config_s *config)
{
  /* Pin multiplexing */

  da1470x_gpio_config(config->txpin);
  da1470x_gpio_config(config->rxpin);

  if (config->rtspin != 0)
    {
      da1470x_gpio_config(config->rtspin);
    }

  if (config->ctspin != 0)
    {
      da1470x_gpio_config(config->ctspin);
    }

  /* Clock the block, then soft-reset it so that we start from a known
   * state even if a previous boot stage left the transmitter busy.
   */

  da1470x_uart_enable(base);

  putreg32(UART_SRR_UR | UART_SRR_RFR | UART_SRR_XFR,
           base + DA1470X_UART_SRR_OFFSET);

  /* No interrupts, FIFOs enabled and flushed, RX trigger 1 char, TX
   * trigger when the FIFO is half empty.
   */

  putreg32(0, base + DA1470X_UART_IER_DLH_OFFSET);
  putreg32(UART_FCR_FIFOE | UART_FCR_RFIFOR | UART_FCR_XFIFOR |
           UART_FCR_RT_1CHAR | UART_FCR_TET_HALF,
           base + DA1470X_UART_IIR_FCR_OFFSET);

  da1470x_uart_setformat(base, config);
}

/****************************************************************************
 * Name: da1470x_uart_enable
 ****************************************************************************/

void da1470x_uart_enable(uintptr_t base)
{
  uint32_t enable;
  uint32_t sel;

  da1470x_uart_clkbits(base, &enable, &sel);

  /* CLK_SEL = 0 selects DIVN (32 MHz); then enable the clock.  The
   * SET/RESET shadow registers make this atomic.
   */

  putreg32(sel, DA1470X_CRG_SNC_RESET_CLK_SNC);
  putreg32(enable, DA1470X_CRG_SNC_SET_CLK_SNC);
}

/****************************************************************************
 * Name: da1470x_uart_disable
 ****************************************************************************/

void da1470x_uart_disable(uintptr_t base)
{
  uint32_t enable;
  uint32_t sel;

  da1470x_uart_clkbits(base, &enable, &sel);
  putreg32(enable, DA1470X_CRG_SNC_RESET_CLK_SNC);
}

/****************************************************************************
 * Name: da1470x_uart_setformat
 ****************************************************************************/

void da1470x_uart_setformat(uintptr_t base,
                            const struct uart_config_s *config)
{
  da1470x_setbaud(base, config);
  da1470x_setlcr(base, config);
  da1470x_sethwflow(base, config);
}

#endif /* HAVE_UART_DEVICE */

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
  /* Wait for space in the TX FIFO.  USR.TFNF is used rather than LSR.THRE
   * because the latter is affected by the PTIME interrupt mode.
   */

  while ((getreg32(CONSOLE_BASE + DA1470X_UART_USR_OFFSET) &
          UART_USR_TFNF) == 0)
    {
    }

  putreg32((uint32_t)ch, CONSOLE_BASE + DA1470X_UART_RBR_THR_DLL_OFFSET);
#endif
}
