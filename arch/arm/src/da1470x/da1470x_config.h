/****************************************************************************
 * arch/arm/src/da1470x/da1470x_config.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_CONFIG_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_CONFIG_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <arch/board/board.h>

#include "chip.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Map logical UART names */

#undef HAVE_UART0
#undef HAVE_UART1
#undef HAVE_UART2

#ifdef CONFIG_DA1470X_UART0
#  define HAVE_UART0 1
#endif

#ifdef CONFIG_DA1470X_UART1
#  define HAVE_UART1 1
#endif

#ifdef CONFIG_DA1470X_UART2
#  define HAVE_UART2 1
#endif

/* Check if we have a UART device */

#undef HAVE_UART_DEVICE

#if defined(HAVE_UART0) || defined(HAVE_UART1) || defined(HAVE_UART2)
#  define HAVE_UART_DEVICE 1
#endif

/* Is there a serial console? There should be at most one defined. */

#undef HAVE_UART_CONSOLE

#if defined(CONFIG_UART0_SERIAL_CONSOLE) && defined(HAVE_UART0)
#  undef CONFIG_UART1_SERIAL_CONSOLE
#  undef CONFIG_UART2_SERIAL_CONSOLE
#  define HAVE_UART_CONSOLE 1
#elif defined(CONFIG_UART1_SERIAL_CONSOLE) && defined(HAVE_UART1)
#  undef CONFIG_UART0_SERIAL_CONSOLE
#  undef CONFIG_UART2_SERIAL_CONSOLE
#  define HAVE_UART_CONSOLE 1
#elif defined(CONFIG_UART2_SERIAL_CONSOLE) && defined(HAVE_UART2)
#  undef CONFIG_UART0_SERIAL_CONSOLE
#  undef CONFIG_UART1_SERIAL_CONSOLE
#  define HAVE_UART_CONSOLE 1
#else
#  undef CONFIG_UART0_SERIAL_CONSOLE
#  undef CONFIG_UART1_SERIAL_CONSOLE
#  undef CONFIG_UART2_SERIAL_CONSOLE
#endif

/* Hardware flow control is only wired on UART1 and UART2 (datasheet UART2
 * and UART3).
 */

#undef HAVE_UART_FLOWCONTROL

#if defined(CONFIG_SERIAL_IFLOWCONTROL) || defined(CONFIG_SERIAL_OFLOWCONTROL)
#  define HAVE_UART_FLOWCONTROL 1
#endif

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_CONFIG_H */
