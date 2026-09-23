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

#ifdef CONFIG_DA1470X_SNC_FW_INPUT
#  include <arch/chip/snc.h>
#endif

#ifdef CONFIG_ARCH_BUTTONS

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const da1470x_pinset_t g_buttons[NUM_BUTTONS] =
{
  GPIO_BUTTON1,
  GPIO_BUTTON2,
};

#if defined(CONFIG_ARCH_IRQBUTTONS) && defined(CONFIG_DA1470X_SNC_FW_INPUT)
static xcpt_t g_btn_handler[NUM_BUTTONS];
static void  *g_btn_arg[NUM_BUTTONS];
static bool   g_btn_watched;
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#if defined(CONFIG_ARCH_IRQBUTTONS) && defined(CONFIG_DA1470X_SNC_FW_INPUT)

/****************************************************************************
 * Name: btn_snc_event
 *
 * Description:
 *   A settled change reported by the sensor node controller.  Called from
 *   the low priority work queue; the button upper half's handler samples
 *   board_buttons() and wakes its readers, which is as good from here as
 *   from an interrupt.
 *
 ****************************************************************************/

static void btn_snc_event(uint16_t type, const void *payload, uint16_t len,
                          void *arg)
{
  const struct snc_input_event_s *ev = payload;
  int i;

  if (len < sizeof(*ev))
    {
      return;
    }

  for (i = 0; i < NUM_BUTTONS; i++)
    {
      if (GPIO_PORT_DECODE(g_buttons[i]) == ev->port &&
          (ev->changed & (1u << GPIO_PIN_DECODE(g_buttons[i]))) != 0 &&
          g_btn_handler[i] != NULL)
        {
          g_btn_handler[i](0, NULL, g_btn_arg[i]);
        }
    }
}

/****************************************************************************
 * Name: btn_snc_watch
 *
 * Description:
 *   Have the controller watch every button pin, whatever handlers are
 *   attached: it costs the M33 nothing until something changes.
 *
 ****************************************************************************/

static int btn_snc_watch(void)
{
  struct snc_input_config_s cfg;
  uint32_t pins[SNC_INPUT_NPORTS] =
    {
      0
    };

  int ret;
  int i;

  ret = da1470x_snc_register_handler(SNC_INPUT_EVENT, btn_snc_event, NULL);
  if (ret < 0)
    {
      return ret;
    }

  for (i = 0; i < NUM_BUTTONS; i++)
    {
      pins[GPIO_PORT_DECODE(g_buttons[i])] |=
        1u << GPIO_PIN_DECODE(g_buttons[i]);
    }

  for (i = 0; i < SNC_INPUT_NPORTS; i++)
    {
      if (pins[i] == 0)
        {
          continue;
        }

      cfg.port        = i;
      cfg.reserved    = 0;
      cfg.debounce_ms = 20;
      cfg.pins        = pins[i];

      ret = da1470x_snc_send(SNC_INPUT_CONFIG, &cfg, sizeof(cfg));
      if (ret < 0)
        {
          return ret;
        }
    }

  return OK;
}
#endif

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
 *   With the sensor node controller's input firmware, a handler is called
 *   for every settled press and release the controller reports: the M33
 *   takes no interrupt per bounce, and a reader of /dev/buttons can sleep
 *   in poll() until something happens.
 *
 *   Without it, this deliberately arms nothing and returns OK.  Arming a
 *   falling-edge interrupt on K1/K2 has been seen to wedge the system on
 *   the first press: black screen, silent console, and the core at 100%
 *   inside board_buttons() via the upper half's interrupt handler.  The
 *   cause is not established -- da1470x_gpioirq_dispatch() does clear the
 *   wake-up latch on entry, so it is not a simple unacknowledged interrupt
 *   -- and until it is, the interrupt stays off.  read() and poll() with a
 *   timeout still sample board_buttons() directly; what is lost is being
 *   woken by a press.  ARCH_IRQBUTTONS stays selected because
 *   INPUT_BUTTONS_LOWER depends on it.
 *
 ****************************************************************************/

#ifdef CONFIG_ARCH_IRQBUTTONS
int board_button_irq(int id, xcpt_t irqhandler, void *arg)
{
  if (id < 0 || id >= NUM_BUTTONS)
    {
      return -EINVAL;
    }

#ifdef CONFIG_DA1470X_SNC_FW_INPUT
  g_btn_handler[id] = irqhandler;
  g_btn_arg[id]     = arg;

  if (irqhandler != NULL && !g_btn_watched)
    {
      int ret = btn_snc_watch();

      if (ret < 0)
        {
          return ret;
        }

      g_btn_watched = true;
    }
#else
  UNUSED(irqhandler);
  UNUSED(arg);
#endif

  return OK;
}
#endif /* CONFIG_ARCH_IRQBUTTONS */

#endif /* CONFIG_ARCH_BUTTONS */
