/****************************************************************************
 * arch/arm/src/nrf53/nrf53_gpio.c
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

#include <nuttx/spinlock.h>

#include "arm_internal.h"
#include "hardware/da1470x_gpio.h"
#include "da1470x_gpio.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_gpio_regget
 *
 * Description:
 *   Get a register address for given GPIO port and register offset
 *
 ****************************************************************************/

static inline uint32_t da1470x_gpio_regget(int port, uint32_t offset)
{
  uint32_t base = 0;

  /* Get base address for port */

  if (port == 0)
    {
      base = DA1470_GPIO_P0_BASE;
    }
  else if (port == 1)
    {
      base = NRF53_GPIO_P1_BASE;
    }
  else if (port == 2)
    {
      base = NRF53_GPIO_P2_BASE;
    }

  return (base + offset);
}

/****************************************************************************
 * Name: da1470x_gpio_input
 *
 * Description:
 *   Configure a GPIO input pin based on bit-encoded description of the pin.
 *
 ****************************************************************************/

static inline void da1470x_gpio_input(unsigned int port, unsigned int pin)
{
  /* Configure the pin as an input */
  modifyreg32(DA1470_GPIO_MODE_OFFSET(port, pin), DA1470_GPIO_BASE, GPIO_INPUT | GPIO_FUNC_GPIO);
}

/****************************************************************************
 * Name: da1470x_gpio_output
 *
 * Description:
 *   Configure a GPIO output pin based on bit-encoded description of the pin.
 *
 ****************************************************************************/

static inline void da1470x_gpio_output(da1470x_pinset_t cfgset,
                                     unsigned int port, unsigned int pin)
{
  /* Configure the pin as an output */

  modifyreg32(DA1470_GPIO_MODE_OFFSET(port, pin), DA1470_GPIO_BASE, GPIO_OUTPUT | GPIO_FUNC_GPIO);

// TODO need to be done before setting the pin as output?
  da1470x_gpio_write(cfgset, ((cfgset & GPIO_VALUE) != GPIO_VALUE_ZERO));
}

/****************************************************************************
 * Name: nrf53_gpio_mode
 *
 * Description:
 *   Configure a GPIO mode based on bit-encoded description of the pin.
 *
 ****************************************************************************/

static inline void da1470x_gpio_mode(da1470x_pinset_t cfgset,
                                   unsigned int port, unsigned int pin)
{
  uint32_t mode;
  uint32_t regval;
  uint32_t offset;

  offset = DA1470_GPIO_MODE_OFFSET(port, pin);

  mode = cfgset & GPIO_MODE_MASK;

  regval = getreg32(offset);
  regval &= ~GPIO_MODE_REG_PUPD_MASK(pin);

  if (mode == GPIO_PULLUP)
    {
      regval |= GPIO_CNF_PULL_UP;
    }
  else if (mode == GPIO_PULLDOWN)
    {
      regval |= GPIO_CNF_PULL_DOWN;
    }

  putreg32(regval, offset);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_gpio_config
 *
 * Description:
 *   Configure a GPIO pin based on bit-encoded description of the pin.
 *
 ****************************************************************************/

int da1470x_gpio_config(da1470x_pinset_t cfgset)
{
  unsigned int port = 0;
  unsigned int pin;
  //irqstate_t flags;
  int ret = OK;

  /* Verify that this hardware supports the select GPIO port */

  port = (cfgset & GPIO_PORT_MASK) >> GPIO_PORT_SHIFT;

  if (port < DA1470_GPIO_NPORTS)
    {
      /* Get the pin number and select the port configuration register for
       * that pin.
       */

      pin = GPIO_PIN_DECODE(cfgset);

      //flags = spin_lock_irqsave(NULL);

      /* First, configure the port as a generic input so that we have a
       * known starting point and consistent behavior during the re-
       * configuration.
       */

      da1470x_gpio_input(port, pin);

      /* Set the mode bits */

      da1470x_gpio_mode(cfgset, port, pin);

      //spin_unlock_irqrestore(NULL, flags);
    }
  else
    {
      ret = -EINVAL;
    }

  return ret;
}

/****************************************************************************
 * Name: da1470x_gpio_unconfig
 *
 * Description:
 *   Unconfigure a GPIO pin based on bit-encoded description of the pin.
 *
 ****************************************************************************/

int da1470x_gpio_unconfig(da1470x_pinset_t cfgset)
{
  unsigned int pin;
  unsigned int port = 0;
  uint32_t offset;

  /* Get port and pin number */

  pin  = GPIO_PIN_DECODE(cfgset);
  port = GPIO_PORT_DECODE(cfgset);

  /* Get address offset */

  offset = DA1470_GPIO_MODE_OFFSET(port, pin);

  /* Configure as input and disconnect input buffer */

  putreg32(GPIO_CNF_PULL_DISABLED, offset); //set as input

  return OK;
}

/****************************************************************************
 * Name: da1470x_gpio_write
 *
 * Description:
 *   Write one or zero to the selected GPIO pin
 *
 ****************************************************************************/

void da1470x_gpio_write(da1470x_pinset_t pinset, bool value)
{
  unsigned int pin;
  unsigned int port = 0;
  uint32_t offset;

  /* Get port and pin number */

  pin  = GPIO_PIN_DECODE(pinset);
  port = GPIO_PORT_DECODE(pinset);

  /* Get register address */

  offset = DA1470_GPIO_SET_DATA_OFFSET(port);

  /* Put register value */

  putreg32(1 << pin, offset);
}

/****************************************************************************
 * Name: da1470x_gpio_read
 *
 * Description:
 *   Read one or zero from the selected GPIO pin
 *
 ****************************************************************************/

bool da1470x_gpio_read(da1470x_pinset_t pinset)
{
  unsigned int port;
  unsigned int pin;
  uint32_t regval;
  uint32_t offset;

  /* Get port and pin number */

  pin  = GPIO_PIN_DECODE(pinset);
  port = GPIO_PORT_DECODE(pinset);

  /* Get register address */

  offset = DA1470_GPIO_DATA_OFFSET(port);

  /* Get register value */

  regval = getreg32(offset);

  return (regval >> pin) & 1UL;
}
