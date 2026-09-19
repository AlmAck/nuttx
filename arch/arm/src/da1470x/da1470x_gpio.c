/****************************************************************************
 * arch/arm/src/da1470x/da1470x_gpio.c
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
#include <arch/irq.h>

#include "arm_internal.h"
#include "hardware/da1470x_gpio.h"
#include "hardware/da1470x_wakeup.h"
#include "hardware/da1470x_crg_top.h"
#include "da1470x_gpio.h"
#include "da1470x_pdc.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define GPIO_NPINS_PER_PORT   32

/****************************************************************************
 * Private Types
 ****************************************************************************/

#ifdef CONFIG_DA1470X_GPIO_IRQ
struct da1470x_gpioirq_slot_s
{
  xcpt_t handler;
  void  *arg;
};
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const uint8_t g_port_npins[DA1470X_GPIO_NPORTS] =
{
  DA1470X_GPIO_PORT0_NPINS,
  DA1470X_GPIO_PORT1_NPINS,
  DA1470X_GPIO_PORT2_NPINS
};

#ifdef CONFIG_DA1470X_GPIO_IRQ
static struct da1470x_gpioirq_slot_s
  g_gpioirq[DA1470X_GPIO_NPORTS][GPIO_NPINS_PER_PORT];
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_gpio_decode
 *
 * Description:
 *   Extract and validate port and pin from a pinset.
 *
 ****************************************************************************/

static int da1470x_gpio_decode(da1470x_pinset_t pinset,
                               unsigned int *port, unsigned int *pin)
{
  *port = GPIO_PORT_DECODE(pinset);
  *pin  = GPIO_PIN_DECODE(pinset);

  if (*port >= DA1470X_GPIO_NPORTS || *pin >= g_port_npins[*port])
    {
      return -EINVAL;
    }

  return OK;
}

/****************************************************************************
 * Name: da1470x_gpio_latch_bits
 *
 * Description:
 *   Address of the SET_PAD_LATCH / RESET_PAD_LATCH register of a port.
 *
 ****************************************************************************/

static uintptr_t da1470x_gpio_latch_reg(int port, bool set)
{
  switch (port)
    {
      case 1:
        return set ? DA1470X_CRG_TOP_P1_SET_PAD_LATCH :
                     DA1470X_CRG_TOP_P1_RESET_PAD_LATCH;

      case 2:
        return set ? DA1470X_CRG_TOP_P2_SET_PAD_LATCH :
                     DA1470X_CRG_TOP_P2_RESET_PAD_LATCH;

      case 0:
      default:
        return set ? DA1470X_CRG_TOP_P0_SET_PAD_LATCH :
                     DA1470X_CRG_TOP_P0_RESET_PAD_LATCH;
    }
}

/****************************************************************************
 * Name: da1470x_gpioirq_dispatch
 *
 * Description:
 *   Common per-port WKUP interrupt handler: acknowledge every pending pin
 *   on the port and call the registered handlers.
 *
 ****************************************************************************/

#ifdef CONFIG_DA1470X_GPIO_IRQ
static int da1470x_gpioirq_dispatch(int port)
{
  uint32_t status = getreg32(DA1470X_WAKEUP_STATUS_P(port));
  uint32_t pending = status;
  int pin;

  /* Writing ones to WKUP_CLEAR_Px clears the matching STATUS bits */

  putreg32(status, DA1470X_WAKEUP_CLEAR_P(port));

  while (pending != 0)
    {
      pin      = __builtin_ctz(pending);
      pending &= ~(1u << pin);

      if (g_gpioirq[port][pin].handler != NULL)
        {
          da1470x_pinset_t pinset = (port << GPIO_PORT_SHIFT) |
                                    GPIO_PIN(pin);

          g_gpioirq[port][pin].handler((int)pinset, NULL,
                                       g_gpioirq[port][pin].arg);
        }
    }

  return OK;
}

static int da1470x_gpioirq_isr_p0(int irq, void *ctx, void *arg)
{
  return da1470x_gpioirq_dispatch(0);
}

static int da1470x_gpioirq_isr_p1(int irq, void *ctx, void *arg)
{
  return da1470x_gpioirq_dispatch(1);
}

static int da1470x_gpioirq_isr_p2(int irq, void *ctx, void *arg)
{
  return da1470x_gpioirq_dispatch(2);
}
#endif /* CONFIG_DA1470X_GPIO_IRQ */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_gpio_config
 ****************************************************************************/

int da1470x_gpio_config(da1470x_pinset_t cfgset)
{
  unsigned int port;
  unsigned int pin;
  irqstate_t flags;
  uint32_t mode;
  int ret;

  ret = da1470x_gpio_decode(cfgset, &port, &pin);
  if (ret < 0)
    {
      return ret;
    }

  /* Build the Pxy_MODE value: function, pull/direction, open drain */

  mode = GPIO_FUNC_DECODE(cfgset) << GPIO_MODE_PID_SHIFT;

  if ((cfgset & GPIO_DIR_MASK) == GPIO_OUTPUT)
    {
      mode |= GPIO_MODE_PUPD_OUTPUT;
    }
  else if ((cfgset & GPIO_MODE_MASK) == GPIO_PULLUP)
    {
      mode |= GPIO_MODE_PUPD_PULLUP;
    }
  else if ((cfgset & GPIO_MODE_MASK) == GPIO_PULLDOWN)
    {
      mode |= GPIO_MODE_PUPD_PULLDOWN;
    }
  else
    {
      mode |= GPIO_MODE_PUPD_INPUT;
    }

  if ((cfgset & GPIO_OPENDRAIN) != 0)
    {
      mode |= GPIO_MODE_PPOD;
    }

  flags = enter_critical_section();

  /* Preset the output level before enabling the driver so the pin never
   * glitches to the wrong state.
   */

  if ((cfgset & GPIO_DIR_MASK) == GPIO_OUTPUT)
    {
      da1470x_gpio_write(cfgset, (cfgset & GPIO_VALUE) != 0);
    }

  /* Pad power rail selection */

  if ((cfgset & GPIO_PADPWR_MASK) == GPIO_V18P)
    {
      modifyreg32(DA1470X_GPIO_PADPWR_CTRL(port), 0, 1u << pin);
    }
  else
    {
      modifyreg32(DA1470X_GPIO_PADPWR_CTRL(port), 1u << pin, 0);
    }

  putreg32(mode, DA1470X_GPIO_MODE(port, pin));

  /* Open the pad latch so the new configuration reaches the pad */

  putreg32(1u << pin, da1470x_gpio_latch_reg(port, true));

  leave_critical_section(flags);
  return OK;
}

/****************************************************************************
 * Name: da1470x_gpio_unconfig
 ****************************************************************************/

int da1470x_gpio_unconfig(da1470x_pinset_t cfgset)
{
  unsigned int port;
  unsigned int pin;
  int ret;

  ret = da1470x_gpio_decode(cfgset, &port, &pin);
  if (ret < 0)
    {
      return ret;
    }

  putreg32(GPIO_MODE_PUPD_INPUT, DA1470X_GPIO_MODE(port, pin));
  return OK;
}

/****************************************************************************
 * Name: da1470x_gpio_write
 ****************************************************************************/

void da1470x_gpio_write(da1470x_pinset_t pinset, bool value)
{
  unsigned int port;
  unsigned int pin;

  if (da1470x_gpio_decode(pinset, &port, &pin) < 0)
    {
      return;
    }

  if (value)
    {
      putreg32(1u << pin, DA1470X_GPIO_SET_DATA(port));
    }
  else
    {
      putreg32(1u << pin, DA1470X_GPIO_RESET_DATA(port));
    }
}

/****************************************************************************
 * Name: da1470x_gpio_read
 ****************************************************************************/

bool da1470x_gpio_read(da1470x_pinset_t pinset)
{
  unsigned int port;
  unsigned int pin;

  if (da1470x_gpio_decode(pinset, &port, &pin) < 0)
    {
      return false;
    }

  return ((getreg32(DA1470X_GPIO_DATA(port)) >> pin) & 1) != 0;
}

/****************************************************************************
 * Name: da1470x_gpio_latch_enable / da1470x_gpio_latch_disable
 ****************************************************************************/

void da1470x_gpio_latch_enable(int port)
{
  if (port >= 0 && port < DA1470X_GPIO_NPORTS)
    {
      putreg32(0xffffffff, da1470x_gpio_latch_reg(port, true));
    }
}

void da1470x_gpio_latch_disable(int port)
{
  if (port >= 0 && port < DA1470X_GPIO_NPORTS)
    {
      putreg32(0xffffffff, da1470x_gpio_latch_reg(port, false));
    }
}

/****************************************************************************
 * Name: da1470x_gpio_dump
 ****************************************************************************/

#ifdef CONFIG_DEBUG_GPIO_INFO
int da1470x_gpio_dump(da1470x_pinset_t pinset, const char *msg)
{
  unsigned int port;
  unsigned int pin;
  irqstate_t flags;

  if (da1470x_gpio_decode(pinset, &port, &pin) < 0)
    {
      return -EINVAL;
    }

  flags = enter_critical_section();
  gpioinfo("P%u_%02u (%s):\n", port, pin, msg);
  gpioinfo("  MODE: %08" PRIx32 " DATA: %08" PRIx32 "\n",
           getreg32(DA1470X_GPIO_MODE(port, pin)),
           getreg32(DA1470X_GPIO_DATA(port)));
  gpioinfo("  PADPWR: %08" PRIx32 " LATCH: %08" PRIx32 "\n",
           getreg32(DA1470X_GPIO_PADPWR_CTRL(port)),
           getreg32(DA1470X_CRG_TOP_P0_PAD_LATCH + (port * 12)));
  leave_critical_section(flags);
  return OK;
}
#endif

#ifdef CONFIG_DA1470X_GPIO_IRQ

/****************************************************************************
 * Name: da1470x_gpioirq_initialize
 ****************************************************************************/

void da1470x_gpioirq_initialize(void)
{
  int p;

  /* The wake-up controller is clocked through CLK_TMR; its registers
   * ignore writes until then.
   */

  modifyreg32(DA1470X_CRG_TOP_CLK_TMR, 0, CRG_TOP_CLK_TMR_WAKEUPCT_ENABLE);
  putreg32(0, DA1470X_WAKEUP_WKUP_CTRL);

  for (p = 0; p < DA1470X_GPIO_NPORTS; p++)
    {
      putreg32(0, DA1470X_WAKEUP_SELECT_P(p));
      putreg32(0, DA1470X_WAKEUP_SEL_GPIO_P(p));
      putreg32(0, DA1470X_WAKEUP_SEL1_GPIO_P(p));
      putreg32(0, DA1470X_WAKEUP_POL_P(p));
      putreg32(0xffffffff, DA1470X_WAKEUP_CLEAR_P(p));
    }

  /* No pin is selected yet, so enabling the NVIC lines is safe */

  irq_attach(DA1470X_IRQ_GPIO_P0, da1470x_gpioirq_isr_p0, NULL);
  irq_attach(DA1470X_IRQ_GPIO_P1, da1470x_gpioirq_isr_p1, NULL);
  irq_attach(DA1470X_IRQ_GPIO_P2, da1470x_gpioirq_isr_p2, NULL);

  up_enable_irq(DA1470X_IRQ_GPIO_P0);
  up_enable_irq(DA1470X_IRQ_GPIO_P1);
  up_enable_irq(DA1470X_IRQ_GPIO_P2);
}

/****************************************************************************
 * Name: da1470x_gpioirq_attach
 ****************************************************************************/

int da1470x_gpioirq_attach(da1470x_pinset_t pinset,
                           enum da1470x_gpio_edge_e edge,
                           xcpt_t handler, void *arg)
{
  unsigned int port;
  unsigned int pin;
  irqstate_t flags;
  uint32_t mask;
  int ret;

  ret = da1470x_gpio_decode(pinset, &port, &pin);
  if (ret < 0)
    {
      return ret;
    }

  mask  = 1u << pin;
  flags = enter_critical_section();

  g_gpioirq[port][pin].handler = handler;
  g_gpioirq[port][pin].arg     = arg;

  /* Polarity: 0 = rising, 1 = falling */

  if (edge == DA1470X_GPIO_EDGE_FALLING)
    {
      modifyreg32(DA1470X_WAKEUP_POL_P(port), 0, mask);
    }
  else
    {
      modifyreg32(DA1470X_WAKEUP_POL_P(port), mask, 0);
    }

  /* Clear any stale event, then route the pin to the non-debounced
   * GPIO_Px interrupt with edge sensitivity, or drop it.
   */

  putreg32(mask, DA1470X_WAKEUP_CLEAR_P(port));

  if (handler != NULL)
    {
      modifyreg32(DA1470X_WAKEUP_SEL1_GPIO_P(port), 0, mask);
      modifyreg32(DA1470X_WAKEUP_SEL_GPIO_P(port), 0, mask);
    }
  else
    {
      modifyreg32(DA1470X_WAKEUP_SEL_GPIO_P(port), mask, 0);
      modifyreg32(DA1470X_WAKEUP_SEL1_GPIO_P(port), mask, 0);
    }

  leave_critical_section(flags);
  return OK;
}

/****************************************************************************
 * Name: da1470x_gpioirq_enable
 ****************************************************************************/

void da1470x_gpioirq_enable(da1470x_pinset_t pinset)
{
  unsigned int port;
  unsigned int pin;
  irqstate_t flags;

  if (da1470x_gpio_decode(pinset, &port, &pin) < 0)
    {
      return;
    }

  flags = enter_critical_section();
  putreg32(1u << pin, DA1470X_WAKEUP_CLEAR_P(port));
  modifyreg32(DA1470X_WAKEUP_SEL_GPIO_P(port), 0, 1u << pin);
  leave_critical_section(flags);

#ifdef CONFIG_DA1470X_PDC
  /* Watching the pin is only half of a wake-up source.  An interrupt
   * cannot reach a processor whose power domain has been switched off;
   * only the power domain controller can bring it back.  Register the
   * trigger here, next to the configuration it depends on, so that the
   * two cannot drift apart -- which is exactly what happened while they
   * were kept in separate places.
   */

  if (da1470x_pdc_add(DA1470X_PDC_TRIG_P0_GPIO + port, pin,
                      DA1470X_PDC_MASTER_CM33,
                      DA1470X_PDC_FLAG_EN_XTAL) < 0)
    {
      gpiowarn("P%u.%02u has no PDC entry and will not wake the system\n",
               port, pin);
    }
#endif
}

/****************************************************************************
 * Name: da1470x_gpioirq_disable
 ****************************************************************************/

void da1470x_gpioirq_disable(da1470x_pinset_t pinset)
{
  unsigned int port;
  unsigned int pin;
  irqstate_t flags;
#ifdef CONFIG_DA1470X_PDC
  int index;
#endif

  if (da1470x_gpio_decode(pinset, &port, &pin) < 0)
    {
      return;
    }

  flags = enter_critical_section();
  modifyreg32(DA1470X_WAKEUP_SEL_GPIO_P(port), 1u << pin, 0);
  leave_critical_section(flags);

#ifdef CONFIG_DA1470X_PDC
  /* Give the lookup table entry back; it is a scarce resource shared with
   * the other two processors.
   */

  index = da1470x_pdc_find(DA1470X_PDC_TRIG_P0_GPIO + port, pin,
                           DA1470X_PDC_MASTER_CM33);
  if (index != DA1470X_PDC_INVALID_ENTRY)
    {
      da1470x_pdc_remove(index);
    }
#endif
}

#endif /* CONFIG_DA1470X_GPIO_IRQ */
