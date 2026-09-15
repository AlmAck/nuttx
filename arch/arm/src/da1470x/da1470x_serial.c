/****************************************************************************
 * arch/arm/src/da1470x/da1470x_serial.c
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
#include <debug.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <nuttx/arch.h>
#include <nuttx/fs/ioctl.h>
#include <nuttx/irq.h>
#include <nuttx/serial/serial.h>

#ifdef CONFIG_SERIAL_TERMIOS
#  include <termios.h>
#endif

#include <arch/board/board.h>

#include "arm_internal.h"
#include "chip.h"
#include "da1470x_config.h"
#include "da1470x_irq.h"
#include "da1470x_lowputc.h"
#include "da1470x_serial.h"
#include "hardware/da1470x_uart.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#if defined(HAVE_UART_DEVICE) && defined(USE_SERIALDRIVER)

/* Which UART with be tty0/console and which tty1-2? */

#if defined(CONFIG_UART0_SERIAL_CONSOLE)
#  define CONSOLE_DEV         g_uart0port
#  define TTYS0_DEV           g_uart0port
#  define UART0_ASSIGNED      1
#elif defined(CONFIG_UART1_SERIAL_CONSOLE)
#  define CONSOLE_DEV         g_uart1port
#  define TTYS0_DEV           g_uart1port
#  define UART1_ASSIGNED      1
#elif defined(CONFIG_UART2_SERIAL_CONSOLE)
#  define CONSOLE_DEV         g_uart2port
#  define TTYS0_DEV           g_uart2port
#  define UART2_ASSIGNED      1
#else
#  undef CONSOLE_DEV
#  if defined(HAVE_UART0)
#    define TTYS0_DEV         g_uart0port
#    define UART0_ASSIGNED    1
#  elif defined(HAVE_UART1)
#    define TTYS0_DEV         g_uart1port
#    define UART1_ASSIGNED    1
#  elif defined(HAVE_UART2)
#    define TTYS0_DEV         g_uart2port
#    define UART2_ASSIGNED    1
#  endif
#endif

/* Pick ttyS1 */

#if defined(HAVE_UART0) && !defined(UART0_ASSIGNED)
#  define TTYS1_DEV           g_uart0port
#  define UART0_ASSIGNED      1
#elif defined(HAVE_UART1) && !defined(UART1_ASSIGNED)
#  define TTYS1_DEV           g_uart1port
#  define UART1_ASSIGNED      1
#elif defined(HAVE_UART2) && !defined(UART2_ASSIGNED)
#  define TTYS1_DEV           g_uart2port
#  define UART2_ASSIGNED      1
#endif

/* Pick ttyS2 */

#if defined(HAVE_UART0) && !defined(UART0_ASSIGNED)
#  define TTYS2_DEV           g_uart0port
#  define UART0_ASSIGNED      1
#elif defined(HAVE_UART1) && !defined(UART1_ASSIGNED)
#  define TTYS2_DEV           g_uart1port
#  define UART1_ASSIGNED      1
#elif defined(HAVE_UART2) && !defined(UART2_ASSIGNED)
#  define TTYS2_DEV           g_uart2port
#  define UART2_ASSIGNED      1
#endif

#ifndef BOARD_UART1_RTS_PIN
#  define BOARD_UART1_RTS_PIN 0
#endif
#ifndef BOARD_UART1_CTS_PIN
#  define BOARD_UART1_CTS_PIN 0
#endif
#ifndef BOARD_UART2_RTS_PIN
#  define BOARD_UART2_RTS_PIN 0
#endif
#ifndef BOARD_UART2_CTS_PIN
#  define BOARD_UART2_CTS_PIN 0
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct da1470x_dev_s
{
  uintptr_t uartbase;             /* Base address of UART registers */
  uint8_t   irq;                  /* IRQ associated with this UART */
  struct uart_config_s config;    /* Line configuration */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int  da1470x_setup(struct uart_dev_s *dev);
static void da1470x_shutdown(struct uart_dev_s *dev);
static int  da1470x_attach(struct uart_dev_s *dev);
static void da1470x_detach(struct uart_dev_s *dev);
static int  da1470x_interrupt(int irq, void *context, void *arg);
static int  da1470x_ioctl(struct file *filep, int cmd, unsigned long arg);
static int  da1470x_receive(struct uart_dev_s *dev, unsigned int *status);
static void da1470x_rxint(struct uart_dev_s *dev, bool enable);
static bool da1470x_rxavailable(struct uart_dev_s *dev);
#ifdef CONFIG_SERIAL_IFLOWCONTROL
static bool da1470x_rxflowcontrol(struct uart_dev_s *dev,
                                  unsigned int nbuffered, bool upper);
#endif
static void da1470x_send(struct uart_dev_s *dev, int ch);
static void da1470x_txint(struct uart_dev_s *dev, bool enable);
static bool da1470x_txready(struct uart_dev_s *dev);
static bool da1470x_txempty(struct uart_dev_s *dev);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct uart_ops_s g_uart_ops =
{
  .setup          = da1470x_setup,
  .shutdown       = da1470x_shutdown,
  .attach         = da1470x_attach,
  .detach         = da1470x_detach,
  .ioctl          = da1470x_ioctl,
  .receive        = da1470x_receive,
  .rxint          = da1470x_rxint,
  .rxavailable    = da1470x_rxavailable,
#ifdef CONFIG_SERIAL_IFLOWCONTROL
  .rxflowcontrol  = da1470x_rxflowcontrol,
#endif
  .send           = da1470x_send,
  .txint          = da1470x_txint,
  .txready        = da1470x_txready,
  .txempty        = da1470x_txempty,
};

/* I/O buffers */

#ifdef HAVE_UART0
static char g_uart0rxbuffer[CONFIG_UART0_RXBUFSIZE];
static char g_uart0txbuffer[CONFIG_UART0_TXBUFSIZE];
#endif
#ifdef HAVE_UART1
static char g_uart1rxbuffer[CONFIG_UART1_RXBUFSIZE];
static char g_uart1txbuffer[CONFIG_UART1_TXBUFSIZE];
#endif
#ifdef HAVE_UART2
static char g_uart2rxbuffer[CONFIG_UART2_RXBUFSIZE];
static char g_uart2txbuffer[CONFIG_UART2_TXBUFSIZE];
#endif

/* This describes the state of the DA1470X UART0 port. */

#ifdef HAVE_UART0
static struct da1470x_dev_s g_uart0priv =
{
  .uartbase       = DA1470X_UART0_BASE,
  .irq            = DA1470X_IRQ_UART0,
  .config         =
  {
    .baud         = CONFIG_UART0_BAUD,
    .parity       = CONFIG_UART0_PARITY,
    .bits         = CONFIG_UART0_BITS,
    .stopbits2    = CONFIG_UART0_2STOP,
    .txpin        = BOARD_UART0_TX_PIN,
    .rxpin        = BOARD_UART0_RX_PIN,
  }
};

static uart_dev_t g_uart0port =
{
  .recv     =
  {
    .size   = CONFIG_UART0_RXBUFSIZE,
    .buffer = g_uart0rxbuffer,
  },
  .xmit     =
  {
    .size   = CONFIG_UART0_TXBUFSIZE,
    .buffer = g_uart0txbuffer,
  },
  .ops      = &g_uart_ops,
  .priv     = &g_uart0priv,
};
#endif

/* This describes the state of the DA1470X UART1 port. */

#ifdef HAVE_UART1
static struct da1470x_dev_s g_uart1priv =
{
  .uartbase       = DA1470X_UART1_BASE,
  .irq            = DA1470X_IRQ_UART1,
  .config         =
  {
    .baud         = CONFIG_UART1_BAUD,
    .parity       = CONFIG_UART1_PARITY,
    .bits         = CONFIG_UART1_BITS,
    .stopbits2    = CONFIG_UART1_2STOP,
#ifdef CONFIG_UART1_IFLOWCONTROL
    .iflow        = true,
#endif
#ifdef CONFIG_UART1_OFLOWCONTROL
    .oflow        = true,
#endif
    .txpin        = BOARD_UART1_TX_PIN,
    .rxpin        = BOARD_UART1_RX_PIN,
    .rtspin       = BOARD_UART1_RTS_PIN,
    .ctspin       = BOARD_UART1_CTS_PIN,
  }
};

static uart_dev_t g_uart1port =
{
  .recv     =
  {
    .size   = CONFIG_UART1_RXBUFSIZE,
    .buffer = g_uart1rxbuffer,
  },
  .xmit     =
  {
    .size   = CONFIG_UART1_TXBUFSIZE,
    .buffer = g_uart1txbuffer,
  },
  .ops      = &g_uart_ops,
  .priv     = &g_uart1priv,
};
#endif

/* This describes the state of the DA1470X UART2 port. */

#ifdef HAVE_UART2
static struct da1470x_dev_s g_uart2priv =
{
  .uartbase       = DA1470X_UART2_BASE,
  .irq            = DA1470X_IRQ_UART2,
  .config         =
  {
    .baud         = CONFIG_UART2_BAUD,
    .parity       = CONFIG_UART2_PARITY,
    .bits         = CONFIG_UART2_BITS,
    .stopbits2    = CONFIG_UART2_2STOP,
#ifdef CONFIG_UART2_IFLOWCONTROL
    .iflow        = true,
#endif
#ifdef CONFIG_UART2_OFLOWCONTROL
    .oflow        = true,
#endif
    .txpin        = BOARD_UART2_TX_PIN,
    .rxpin        = BOARD_UART2_RX_PIN,
    .rtspin       = BOARD_UART2_RTS_PIN,
    .ctspin       = BOARD_UART2_CTS_PIN,
  }
};

static uart_dev_t g_uart2port =
{
  .recv     =
  {
    .size   = CONFIG_UART2_RXBUFSIZE,
    .buffer = g_uart2rxbuffer,
  },
  .xmit     =
  {
    .size   = CONFIG_UART2_TXBUFSIZE,
    .buffer = g_uart2txbuffer,
  },
  .ops      = &g_uart_ops,
  .priv     = &g_uart2priv,
};
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_serialin / da1470x_serialout
 ****************************************************************************/

static inline uint32_t da1470x_serialin(struct da1470x_dev_s *priv,
                                        uint32_t offset)
{
  return getreg32(priv->uartbase + offset);
}

static inline void da1470x_serialout(struct da1470x_dev_s *priv,
                                     uint32_t offset, uint32_t value)
{
  putreg32(value, priv->uartbase + offset);
}

/****************************************************************************
 * Name: da1470x_setup
 *
 * Description:
 *   Configure the UART baud, bits, parity, etc. This method is called the
 *   first time that the serial port is opened.
 *
 ****************************************************************************/

static int da1470x_setup(struct uart_dev_s *dev)
{
#ifndef CONFIG_SUPPRESS_UART_CONFIG
  struct da1470x_dev_s *priv = (struct da1470x_dev_s *)dev->priv;

  /* The console was configured by da1470x_lowsetup(); reconfiguring it
   * here would corrupt boot output still in the FIFO.
   */

  if (!dev->isconsole)
    {
      da1470x_uart_configure(priv->uartbase, &priv->config);
    }
#endif

  return OK;
}

/****************************************************************************
 * Name: da1470x_shutdown
 *
 * Description:
 *   Disable the UART.  This method is called when the serial port is
 *   closed.
 *
 ****************************************************************************/

static void da1470x_shutdown(struct uart_dev_s *dev)
{
  struct da1470x_dev_s *priv = (struct da1470x_dev_s *)dev->priv;

  da1470x_serialout(priv, DA1470X_UART_IER_DLH_OFFSET, 0);
  da1470x_uart_disable(priv->uartbase);
}

/****************************************************************************
 * Name: da1470x_attach
 *
 * Description:
 *   Configure the UART to operation in interrupt driven mode.  This method
 *   is called when the serial port is opened.  Normally, this is just after
 *   the setup() method is called, however, the serial console may operate
 *   in a non-interrupt driven mode during the boot phase.
 *
 ****************************************************************************/

static int da1470x_attach(struct uart_dev_s *dev)
{
  struct da1470x_dev_s *priv = (struct da1470x_dev_s *)dev->priv;
  int ret;

  ret = irq_attach(priv->irq, da1470x_interrupt, dev);
  if (ret == OK)
    {
      up_enable_irq(priv->irq);
    }

  return ret;
}

/****************************************************************************
 * Name: da1470x_detach
 *
 * Description:
 *   Detach UART interrupts.  This method is called when the serial port is
 *   closed normally just before the shutdown method is called.
 *
 ****************************************************************************/

static void da1470x_detach(struct uart_dev_s *dev)
{
  struct da1470x_dev_s *priv = (struct da1470x_dev_s *)dev->priv;

  da1470x_serialout(priv, DA1470X_UART_IER_DLH_OFFSET, 0);
  up_disable_irq(priv->irq);
  irq_detach(priv->irq);
}

/****************************************************************************
 * Name: da1470x_interrupt
 *
 * Description:
 *   This is the UART interrupt handler.  It will be invoked when an
 *   interrupt is received on the 'irq'.  It should call uart_xmitchars or
 *   uart_recvchars to perform the appropriate data transfers.
 *
 ****************************************************************************/

static int da1470x_interrupt(int irq, void *context, void *arg)
{
  struct uart_dev_s *dev = (struct uart_dev_s *)arg;
  struct da1470x_dev_s *priv;
  uint32_t iir;
  int passes;

  DEBUGASSERT(dev != NULL && dev->priv != NULL);
  priv = (struct da1470x_dev_s *)dev->priv;

  /* Loop until there are no further pending interrupts, bounded so that a
   * stuck source cannot hang the system.
   */

  for (passes = 0; passes < 256; passes++)
    {
      iir = da1470x_serialin(priv, DA1470X_UART_IIR_FCR_OFFSET) &
            UART_IIR_IID_MASK;

      switch (iir)
        {
          case UART_IIR_IID_NONE:
            return OK;

          case UART_IIR_IID_RLS:

            /* Line status: cleared by reading LSR */

            da1470x_serialin(priv, DA1470X_UART_LSR_OFFSET);
            break;

          case UART_IIR_IID_RDA:
          case UART_IIR_IID_CTI:
            uart_recvchars(dev);
            break;

          case UART_IIR_IID_THRE:
            uart_xmitchars(dev);
            break;

          case UART_IIR_IID_BUSY:

            /* Busy detect: cleared by reading USR */

            da1470x_serialin(priv, DA1470X_UART_USR_OFFSET);
            break;

          case UART_IIR_IID_MODEM:
          default:

            /* Modem status: cleared by reading MSR (offset 0x18) */

            da1470x_serialin(priv, 0x18);
            break;
        }
    }

  return OK;
}

/****************************************************************************
 * Name: da1470x_ioctl
 *
 * Description:
 *   All ioctl calls will be routed through this method
 *
 ****************************************************************************/

static int da1470x_ioctl(struct file *filep, int cmd, unsigned long arg)
{
#ifdef CONFIG_SERIAL_TERMIOS
  struct inode *inode = filep->f_inode;
  struct uart_dev_s *dev = inode->i_private;
  struct da1470x_dev_s *priv = (struct da1470x_dev_s *)dev->priv;
  struct uart_config_s *config = &priv->config;
#endif
  int ret = OK;

  switch (cmd)
    {
#ifdef CONFIG_SERIAL_TERMIOS
      case TCGETS:
        {
          struct termios *termiosp = (struct termios *)arg;

          if (termiosp == NULL)
            {
              ret = -EINVAL;
              break;
            }

          termiosp->c_cflag = ((config->parity != 0) ? PARENB : 0) |
                              ((config->parity == 1) ? PARODD : 0) |
                              ((config->stopbits2) ? CSTOPB : 0) |
#ifdef CONFIG_SERIAL_OFLOWCONTROL
                              ((config->oflow) ? CCTS_OFLOW : 0) |
#endif
#ifdef CONFIG_SERIAL_IFLOWCONTROL
                              ((config->iflow) ? CRTS_IFLOW : 0) |
#endif
                              CS8;

          switch (config->bits)
            {
              case 5:
                termiosp->c_cflag = (termiosp->c_cflag & ~CSIZE) | CS5;
                break;

              case 6:
                termiosp->c_cflag = (termiosp->c_cflag & ~CSIZE) | CS6;
                break;

              case 7:
                termiosp->c_cflag = (termiosp->c_cflag & ~CSIZE) | CS7;
                break;

              default:
                break;
            }

          cfsetispeed(termiosp, config->baud);
        }
        break;

      case TCSETS:
        {
          struct termios *termiosp = (struct termios *)arg;

          if (termiosp == NULL)
            {
              ret = -EINVAL;
              break;
            }

          switch (termiosp->c_cflag & CSIZE)
            {
              case CS5:
                config->bits = 5;
                break;

              case CS6:
                config->bits = 6;
                break;

              case CS7:
                config->bits = 7;
                break;

              case CS8:
              default:
                config->bits = 8;
                break;
            }

          if ((termiosp->c_cflag & PARENB) != 0)
            {
              config->parity = (termiosp->c_cflag & PARODD) ? 1 : 2;
            }
          else
            {
              config->parity = 0;
            }

          config->stopbits2 = (termiosp->c_cflag & CSTOPB) != 0;

#ifdef CONFIG_SERIAL_OFLOWCONTROL
          config->oflow = (termiosp->c_cflag & CCTS_OFLOW) != 0;
#endif
#ifdef CONFIG_SERIAL_IFLOWCONTROL
          config->iflow = (termiosp->c_cflag & CRTS_IFLOW) != 0;
#endif

          config->baud = cfgetispeed(termiosp);

          da1470x_uart_setformat(priv->uartbase, config);
        }
        break;
#endif /* CONFIG_SERIAL_TERMIOS */

      default:
        ret = -ENOTTY;
        break;
    }

  return ret;
}

/****************************************************************************
 * Name: da1470x_receive
 *
 * Description:
 *   Called (usually) from the interrupt level to receive one
 *   character from the UART.  Error bits associated with the
 *   receipt are provided in the return 'status'.
 *
 ****************************************************************************/

static int da1470x_receive(struct uart_dev_s *dev, unsigned int *status)
{
  struct da1470x_dev_s *priv = (struct da1470x_dev_s *)dev->priv;

  if (status != NULL)
    {
      *status = da1470x_serialin(priv, DA1470X_UART_LSR_OFFSET);
    }

  return da1470x_serialin(priv, DA1470X_UART_RBR_THR_DLL_OFFSET) & 0xff;
}

/****************************************************************************
 * Name: da1470x_rxint
 *
 * Description:
 *   Call to enable or disable RX interrupts
 *
 ****************************************************************************/

static void da1470x_rxint(struct uart_dev_s *dev, bool enable)
{
  struct da1470x_dev_s *priv = (struct da1470x_dev_s *)dev->priv;
  irqstate_t flags;
  uint32_t ier;

  flags = enter_critical_section();
  ier   = da1470x_serialin(priv, DA1470X_UART_IER_DLH_OFFSET);

  if (enable)
    {
#ifndef CONFIG_SUPPRESS_SERIAL_INTS
      ier |= UART_IER_DLH_ERBFI_DLH0 | UART_IER_DLH_ELSI_DLH2;
#endif
    }
  else
    {
      ier &= ~(UART_IER_DLH_ERBFI_DLH0 | UART_IER_DLH_ELSI_DLH2);
    }

  da1470x_serialout(priv, DA1470X_UART_IER_DLH_OFFSET, ier);
  leave_critical_section(flags);
}

/****************************************************************************
 * Name: da1470x_rxavailable
 *
 * Description:
 *   Return true if the receive register is not empty
 *
 ****************************************************************************/

static bool da1470x_rxavailable(struct uart_dev_s *dev)
{
  struct da1470x_dev_s *priv = (struct da1470x_dev_s *)dev->priv;

  return (da1470x_serialin(priv, DA1470X_UART_USR_OFFSET) &
          UART_USR_RFNE) != 0;
}

/****************************************************************************
 * Name: da1470x_rxflowcontrol
 *
 * Description:
 *   Called when Rx buffer is full (or exceeds configured watermark levels
 *   if CONFIG_SERIAL_IFLOWCONTROL_WATERMARKS is defined).
 *   Return true if UART activated RX flow control to block more incoming
 *   data
 *
 ****************************************************************************/

#ifdef CONFIG_SERIAL_IFLOWCONTROL
static bool da1470x_rxflowcontrol(struct uart_dev_s *dev,
                                  unsigned int nbuffered, bool upper)
{
  struct da1470x_dev_s *priv = (struct da1470x_dev_s *)dev->priv;

  if (priv->config.iflow && priv->uartbase != DA1470X_UART0_BASE)
    {
      /* With auto flow control the hardware deasserts RTS when the RX
       * FIFO reaches its trigger level.  Emulate software control by
       * driving the RTS bit directly.
       */

      uint32_t mcr = da1470x_serialin(priv, DA1470X_UART_MCR_OFFSET);

      if (upper)
        {
          mcr &= ~UART_MCR_RTS;
        }
      else
        {
          mcr |= UART_MCR_RTS;
        }

      da1470x_serialout(priv, DA1470X_UART_MCR_OFFSET, mcr);
      return upper;
    }

  return false;
}
#endif

/****************************************************************************
 * Name: da1470x_send
 *
 * Description:
 *   This method will send one byte on the UART
 *
 ****************************************************************************/

static void da1470x_send(struct uart_dev_s *dev, int ch)
{
  struct da1470x_dev_s *priv = (struct da1470x_dev_s *)dev->priv;

  da1470x_serialout(priv, DA1470X_UART_RBR_THR_DLL_OFFSET, (uint32_t)ch);
}

/****************************************************************************
 * Name: da1470x_txint
 *
 * Description:
 *   Call to enable or disable TX interrupts
 *
 ****************************************************************************/

static void da1470x_txint(struct uart_dev_s *dev, bool enable)
{
  struct da1470x_dev_s *priv = (struct da1470x_dev_s *)dev->priv;
  irqstate_t flags;
  uint32_t ier;

  flags = enter_critical_section();
  ier   = da1470x_serialin(priv, DA1470X_UART_IER_DLH_OFFSET);

  if (enable)
    {
#ifndef CONFIG_SUPPRESS_SERIAL_INTS
      /* Plain ETBEI: fires once when the THR/FIFO becomes empty.  PTIME
       * would keep the interrupt asserted while the FIFO is empty and
       * storm the CPU.
       */

      ier |= UART_IER_DLH_ETBEI_DLH1;
      da1470x_serialout(priv, DA1470X_UART_IER_DLH_OFFSET, ier);

      /* Fake a TX interrupt here by just calling uart_xmitchars() with
       * interrupts disabled (note this may recurse).
       */

      uart_xmitchars(dev);
#endif
    }
  else
    {
      ier &= ~(UART_IER_DLH_ETBEI_DLH1 | UART_IER_DLH_PTIME_DLH7);
      da1470x_serialout(priv, DA1470X_UART_IER_DLH_OFFSET, ier);
    }

  leave_critical_section(flags);
}

/****************************************************************************
 * Name: da1470x_txready
 *
 * Description:
 *   Return true if the transmit FIFO is not full
 *
 ****************************************************************************/

static bool da1470x_txready(struct uart_dev_s *dev)
{
  struct da1470x_dev_s *priv = (struct da1470x_dev_s *)dev->priv;

  return (da1470x_serialin(priv, DA1470X_UART_USR_OFFSET) &
          UART_USR_TFNF) != 0;
}

/****************************************************************************
 * Name: da1470x_txempty
 *
 * Description:
 *   Return true if the transmit FIFO is empty
 *
 ****************************************************************************/

static bool da1470x_txempty(struct uart_dev_s *dev)
{
  struct da1470x_dev_s *priv = (struct da1470x_dev_s *)dev->priv;

  return (da1470x_serialin(priv, DA1470X_UART_USR_OFFSET) &
          UART_USR_TFE) != 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_earlyserialinit
 *
 * Description:
 *   Performs the low level UART initialization early in debug so that the
 *   serial console will be available during boot up.  This must be called
 *   before arm_serialinit.
 *
 ****************************************************************************/

#ifdef USE_EARLYSERIALINIT
void da1470x_earlyserialinit(void)
{
#ifdef CONSOLE_DEV
  CONSOLE_DEV.isconsole = true;
  da1470x_setup(&CONSOLE_DEV);
#endif
}
#endif

/****************************************************************************
 * Name: arm_serialinit
 *
 * Description:
 *   Register serial console and serial ports.  This assumes
 *   that da1470x_earlyserialinit was called previously.
 *
 ****************************************************************************/

void arm_serialinit(void)
{
#ifdef CONSOLE_DEV
  uart_register("/dev/console", &CONSOLE_DEV);
#endif
#ifdef TTYS0_DEV
  uart_register("/dev/ttyS0", &TTYS0_DEV);
#endif
#ifdef TTYS1_DEV
  uart_register("/dev/ttyS1", &TTYS1_DEV);
#endif
#ifdef TTYS2_DEV
  uart_register("/dev/ttyS2", &TTYS2_DEV);
#endif
}

/****************************************************************************
 * Name: up_putc
 *
 * Description:
 *   Provide priority, low-level access to support OS debug  writes
 *
 ****************************************************************************/

void up_putc(int ch)
{
#ifdef CONSOLE_DEV
  if (ch == '\n')
    {
      arm_lowputc('\r');
    }

  arm_lowputc(ch);
#endif
}

#else /* HAVE_UART_DEVICE && USE_SERIALDRIVER */

/****************************************************************************
 * Name: up_putc
 ****************************************************************************/

void up_putc(int ch)
{
#ifdef HAVE_UART_CONSOLE
  if (ch == '\n')
    {
      arm_lowputc('\r');
    }

  arm_lowputc(ch);
#endif
}

#endif /* HAVE_UART_DEVICE && USE_SERIALDRIVER */
