/****************************************************************************
 * arch/arm/src/da1470x/da1470x_lowputc.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_LOWPUTC_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_LOWPUTC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdbool.h>
#include <stdint.h>

#include "da1470x_config.h"
#include "da1470x_gpio.h"

/****************************************************************************
 * Public Types
 ****************************************************************************/

#ifdef HAVE_UART_DEVICE
/* This structure describes the configuration of an UART */

struct uart_config_s
{
  uint32_t baud;            /* Configured baud */
  uint8_t  parity;          /* 0=none, 1=odd, 2=even */
  uint8_t  bits;            /* Number of bits (5-8) */
  bool     stopbits2;       /* Configure with 2 stop bits instead of 1 */
  bool     iflow;           /* Input flow control (RTS) enabled */
  bool     oflow;           /* Output flow control (CTS) enabled */
  da1470x_pinset_t txpin;   /* TX pin */
  da1470x_pinset_t rxpin;   /* RX pin */
  da1470x_pinset_t rtspin;  /* RTS pin (0 if unused) */
  da1470x_pinset_t ctspin;  /* CTS pin (0 if unused) */
};
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_lowsetup
 *
 * Description:
 *   Called at the very beginning of _start. Performs low level
 *   initialization including setup of the console UART. This UART
 *   initialization is done early so that the serial console is available
 *   for debugging very early in the boot sequence.
 *
 ****************************************************************************/

void da1470x_lowsetup(void);

#ifdef HAVE_UART_DEVICE

/****************************************************************************
 * Name: da1470x_uart_configure
 *
 * Description:
 *   Configure a UART for non-interrupt driven operation
 *
 ****************************************************************************/

void da1470x_uart_configure(uintptr_t base,
                            const struct uart_config_s *config);

/****************************************************************************
 * Name: da1470x_uart_enable
 *
 * Description:
 *   Enable the peripheral clock of a UART.
 *
 ****************************************************************************/

void da1470x_uart_enable(uintptr_t base);

/****************************************************************************
 * Name: da1470x_uart_disable
 *
 * Description:
 *   Disable the peripheral clock of a UART.  It will be necessary to again
 *   call da1470x_uart_configure() in order to use this UART channel again.
 *
 ****************************************************************************/

void da1470x_uart_disable(uintptr_t base);

/****************************************************************************
 * Name: da1470x_uart_setformat
 *
 * Description:
 *   Set the UART line format and speed.
 *
 ****************************************************************************/

void da1470x_uart_setformat(uintptr_t base,
                            const struct uart_config_s *config);

#endif /* HAVE_UART_DEVICE */
#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_LOWPUTC_H */
