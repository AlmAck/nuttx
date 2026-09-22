/****************************************************************************
 * boards/arm/da1470x/da14706-00hzdevkt-p/src/da1470x_buttons.c
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

/* K1 and K2, the two buttons of the motherboard, behind the standard board
 * button interface so that ARCH_BUTTONS and the button upper half can use
 * them.
 *
 * Both are pulled up and shorted to ground when pressed, so the pin reads
 * low for a press and the bit this reports is the inverse of the pin.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <errno.h>

#include <nuttx/board.h>
#include <arch/board/board.h>

#include "da1470x_gpio.h"
#include "da14706-00hzdevkt-p.h"

#ifdef CONFIG_ARCH_BUTTONS

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const da1470x_pinset_t g_buttons[NUM_BUTTONS] =
{
  GPIO_BUTTON1,
  GPIO_BUTTON2,
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: board_button_initialize
 ****************************************************************************/

uint32_t board_button_initialize(void)
{
  int i;

  for (i = 0; i < NUM_BUTTONS; i++)
    {
      da1470x_gpio_config(g_buttons[i]);
    }

  return NUM_BUTTONS;
}

/****************************************************************************
 * Name: board_buttons
 ****************************************************************************/

uint32_t board_buttons(void)
{
  uint32_t ret = 0;
  int i;

  for (i = 0; i < NUM_BUTTONS; i++)
    {
      /* Pulled up and grounded when pressed, so a low pin is a press */

      if (!da1470x_gpio_read(g_buttons[i]))
        {
          ret |= 1 << i;
        }
    }

  return ret;
}

/****************************************************************************
 * Name: board_button_irq
 *
 * Description:
 *   Deliberately arms nothing and returns OK.
 *
 *   Arming a falling-edge interrupt on K1/K2 has been seen to wedge the
 *   system on the first press: black screen, silent console, and the core
 *   at 100% inside board_buttons() via the upper half's interrupt handler.
 *   The cause is not established -- da1470x_gpioirq_dispatch() does clear
 *   the wake-up latch on entry, so it is not a simple unacknowledged
 *   interrupt -- and until it is, the interrupt stays off.
 *
 *   Nothing is lost for a reader: read() and poll() with a timeout both
 *   sample board_buttons() directly.  What is lost is being woken by a
 *   press, so a consumer has to poll.  ARCH_IRQBUTTONS is still selected
 *   because INPUT_BUTTONS_LOWER depends on it, hence a stub rather than no
 *   symbol at all.
 *
 ****************************************************************************/

#ifdef CONFIG_ARCH_IRQBUTTONS
int board_button_irq(int id, xcpt_t irqhandler, void *arg)
{
  UNUSED(irqhandler);
  UNUSED(arg);

  if (id < 0 || id >= NUM_BUTTONS)
    {
      return -EINVAL;
    }

  return OK;
}
#endif /* CONFIG_ARCH_IRQBUTTONS */

#endif /* CONFIG_ARCH_BUTTONS */
