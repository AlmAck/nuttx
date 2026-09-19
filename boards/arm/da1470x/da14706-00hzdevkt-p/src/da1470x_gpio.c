/****************************************************************************
 * boards/arm/da1470x/da14706-00hzdevkt-p/src/da1470x_gpio.c
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

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <debug.h>

#include <nuttx/irq.h>
#include <nuttx/ioexpander/gpio.h>
#include <arch/board/board.h>

#include "da1470x_gpio.h"
#include "da14706-00hzdevkt-p.h"

#ifdef CONFIG_DEV_GPIO

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* /dev/gpio0 = LED1 (output), /dev/gpio1 = K1 and /dev/gpio2 = K2, both
 * interrupt inputs.
 */

#define BOARD_NGPIOOUT  1
#define BOARD_NGPIOINT  2

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct da1470x_gpio_dev_s
{
  struct gpio_dev_s gpio;
  uint8_t id;
};

struct da1470x_gpint_dev_s
{
  struct da1470x_gpio_dev_s da1470xgpio;
  pin_interrupt_t callback;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int gpout_read(struct gpio_dev_s *dev, bool *value);
static int gpout_write(struct gpio_dev_s *dev, bool value);
#ifdef CONFIG_DA1470X_GPIO_IRQ
static int gpint_read(struct gpio_dev_s *dev, bool *value);
static int gpint_attach(struct gpio_dev_s *dev, pin_interrupt_t callback);
static int gpint_enable(struct gpio_dev_s *dev, bool enable);
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct gpio_operations_s gpout_ops =
{
  .go_read   = gpout_read,
  .go_write  = gpout_write,
  .go_attach = NULL,
  .go_enable = NULL,
};

static const uint32_t g_gpiooutputs[BOARD_NGPIOOUT] =
{
  GPIO_LED1,
};

static struct da1470x_gpio_dev_s g_gpout[BOARD_NGPIOOUT];

#ifdef CONFIG_DA1470X_GPIO_IRQ
static const struct gpio_operations_s gpint_ops =
{
  .go_read   = gpint_read,
  .go_write  = NULL,
  .go_attach = gpint_attach,
  .go_enable = gpint_enable,
};

static const uint32_t g_gpiointinputs[BOARD_NGPIOINT] =
{
  GPIO_BUTTON1,
  GPIO_BUTTON2,
};

static struct da1470x_gpint_dev_s g_gpint[BOARD_NGPIOINT];
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: gpout_read / gpout_write
 ****************************************************************************/

static int gpout_read(struct gpio_dev_s *dev, bool *value)
{
  struct da1470x_gpio_dev_s *priv = (struct da1470x_gpio_dev_s *)dev;

  DEBUGASSERT(priv != NULL && value != NULL);
  DEBUGASSERT(priv->id < BOARD_NGPIOOUT);

  *value = da1470x_gpio_read(g_gpiooutputs[priv->id]);
  return OK;
}

static int gpout_write(struct gpio_dev_s *dev, bool value)
{
  struct da1470x_gpio_dev_s *priv = (struct da1470x_gpio_dev_s *)dev;

  DEBUGASSERT(priv != NULL);
  DEBUGASSERT(priv->id < BOARD_NGPIOOUT);

  da1470x_gpio_write(g_gpiooutputs[priv->id], value);
  return OK;
}

#ifdef CONFIG_DA1470X_GPIO_IRQ

/****************************************************************************
 * Name: gpint_interrupt
 *
 * Description:
 *   Arch-level GPIO edge handler; forwards to the registered upper-half
 *   callback.
 *
 ****************************************************************************/

static int gpint_interrupt(int irq, void *context, void *arg)
{
  struct da1470x_gpint_dev_s *priv = (struct da1470x_gpint_dev_s *)arg;

  DEBUGASSERT(priv != NULL && priv->callback != NULL);
  gpioinfo("Interrupt! callback=%p\n", priv->callback);

  priv->callback(&priv->da1470xgpio.gpio, priv->da1470xgpio.id);
  return OK;
}

/****************************************************************************
 * Name: gpint_read / gpint_attach / gpint_enable
 ****************************************************************************/

static int gpint_read(struct gpio_dev_s *dev, bool *value)
{
  struct da1470x_gpint_dev_s *priv = (struct da1470x_gpint_dev_s *)dev;

  DEBUGASSERT(priv != NULL && value != NULL);
  DEBUGASSERT(priv->da1470xgpio.id < BOARD_NGPIOINT);

  *value = da1470x_gpio_read(g_gpiointinputs[priv->da1470xgpio.id]);
  return OK;
}

static int gpint_attach(struct gpio_dev_s *dev, pin_interrupt_t callback)
{
  struct da1470x_gpint_dev_s *priv = (struct da1470x_gpint_dev_s *)dev;
  uint32_t pinset = g_gpiointinputs[priv->da1470xgpio.id];

  gpioinfo("Attaching the callback\n");

  /* Buttons are active low: report the press on the falling edge */

  da1470x_gpioirq_attach(pinset, DA1470X_GPIO_EDGE_FALLING,
                         gpint_interrupt, priv);
  da1470x_gpioirq_disable(pinset);

  priv->callback = callback;
  return OK;
}

static int gpint_enable(struct gpio_dev_s *dev, bool enable)
{
  struct da1470x_gpint_dev_s *priv = (struct da1470x_gpint_dev_s *)dev;
  uint32_t pinset = g_gpiointinputs[priv->da1470xgpio.id];

  if (enable)
    {
      if (priv->callback != NULL)
        {
          da1470x_gpioirq_enable(pinset);
        }
    }
  else
    {
      da1470x_gpioirq_disable(pinset);
    }

  return OK;
}
#endif /* CONFIG_DA1470X_GPIO_IRQ */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_gpio_initialize
 ****************************************************************************/

int da1470x_gpio_initialize(void)
{
  int pincount = 0;
  int i;

  for (i = 0; i < BOARD_NGPIOOUT; i++)
    {
      g_gpout[i].gpio.gp_pintype = GPIO_OUTPUT_PIN;
      g_gpout[i].gpio.gp_ops     = &gpout_ops;
      g_gpout[i].id              = i;
      gpio_pin_register(&g_gpout[i].gpio, pincount);

      da1470x_gpio_config(g_gpiooutputs[i]);
      pincount++;
    }

#ifdef CONFIG_DA1470X_GPIO_IRQ
  for (i = 0; i < BOARD_NGPIOINT; i++)
    {
      g_gpint[i].da1470xgpio.gpio.gp_pintype = GPIO_INTERRUPT_PIN;
      g_gpint[i].da1470xgpio.gpio.gp_ops     = &gpint_ops;
      g_gpint[i].da1470xgpio.id              = i;
      gpio_pin_register(&g_gpint[i].da1470xgpio.gpio, pincount);

      da1470x_gpio_config(g_gpiointinputs[i]);
      pincount++;
    }
#endif

  return OK;
}

#endif /* CONFIG_DEV_GPIO */
