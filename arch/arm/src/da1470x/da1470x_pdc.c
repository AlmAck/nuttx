/****************************************************************************
 * arch/arm/src/da1470x/da1470x_pdc.c
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

/* The Power Domain Controller holds a 16-entry lookup table.  Each entry
 * names a trigger (a GPIO pin or a peripheral event), the master to wake
 * (CM33, CMAC or SNC) and which resources to bring up with it.  When a
 * trigger fires the PMU powers the master's domains, sets the entry's
 * pending bit and raises the PDC interrupt; the master must acknowledge
 * the entry before it can sleep again.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <debug.h>

#include <nuttx/arch.h>
#include <nuttx/irq.h>

#include "arm_internal.h"
#include "hardware/da1470x_pdc.h"
#include "da1470x_pdc.h"

#ifdef CONFIG_DA1470X_PDC

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define PDC_CTRL(n)  (DA1470X_PDC_BASE + DA1470X_PDC_CTRL0_OFFSET + ((n) << 2))

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: pdc_entry_value
 ****************************************************************************/

static uint32_t pdc_entry_value(int trig_select, int trig_id, int master,
                                uint32_t flags)
{
  uint32_t value;

  value  = PDC_CTRL0_TRIG_SELECT(trig_select);
  value |= PDC_CTRL0_TRIG_ID(trig_id);
  value |= PDC_CTRL0_MASTER(master);

  if ((flags & DA1470X_PDC_FLAG_EN_XTAL) != 0)
    {
      value |= PDC_CTRL0_EN_XTAL;
    }

  if ((flags & DA1470X_PDC_FLAG_EN_TMR) != 0)
    {
      value |= PDC_CTRL0_EN_TMR;
    }

  if ((flags & DA1470X_PDC_FLAG_EN_SNC) != 0)
    {
      value |= PDC_CTRL0_EN_SNC;
    }

  return value;
}

/****************************************************************************
 * Name: pdc_interrupt
 *
 * Description:
 *   Wake-up interrupt.  The peripheral that caused the wake-up raises its
 *   own interrupt; here only the PDC bookkeeping is cleared.
 *
 ****************************************************************************/

static int pdc_interrupt(int irq, void *context, void *arg)
{
  da1470x_pdc_ack_all_cm33();
  return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_pdc_initialize
 ****************************************************************************/

void da1470x_pdc_initialize(void)
{
  int i;

  for (i = 0; i < DA1470X_PDC_LUT_SIZE; i++)
    {
      putreg32(0, PDC_CTRL(i));
    }

  da1470x_pdc_ack_all_cm33();

  irq_attach(DA1470X_IRQ_PDC, pdc_interrupt, NULL);
  up_enable_irq(DA1470X_IRQ_PDC);
}

/****************************************************************************
 * Name: da1470x_pdc_find
 ****************************************************************************/

int da1470x_pdc_find(int trig_select, int trig_id, int master)
{
  uint32_t want = pdc_entry_value(trig_select, trig_id, master, 0);
  uint32_t mask = PDC_CTRL0_TRIG_SELECT_MASK | PDC_CTRL0_TRIG_ID_MASK |
                  PDC_CTRL0_MASTER_MASK;
  int i;

  for (i = 0; i < DA1470X_PDC_LUT_SIZE; i++)
    {
      uint32_t value = getreg32(PDC_CTRL(i));

      if ((value & PDC_CTRL0_MASTER_MASK) != 0 && (value & mask) == want)
        {
          return i;
        }
    }

  return DA1470X_PDC_INVALID_ENTRY;
}

/****************************************************************************
 * Name: da1470x_pdc_add
 ****************************************************************************/

int da1470x_pdc_add(int trig_select, int trig_id, int master,
                    uint32_t flags)
{
  uint32_t value = pdc_entry_value(trig_select, trig_id, master, flags);
  irqstate_t irqflags;
  int i;

  if (master == DA1470X_PDC_MASTER_INVALID)
    {
      return -EINVAL;
    }

  irqflags = enter_critical_section();

  /* Reuse an identical entry */

  for (i = 0; i < DA1470X_PDC_LUT_SIZE; i++)
    {
      if (getreg32(PDC_CTRL(i)) == value)
        {
          leave_critical_section(irqflags);
          return i;
        }
    }

  /* Otherwise take the first free slot (MASTER == INVALID) */

  for (i = 0; i < DA1470X_PDC_LUT_SIZE; i++)
    {
      if ((getreg32(PDC_CTRL(i)) & PDC_CTRL0_MASTER_MASK) == 0)
        {
          putreg32(value, PDC_CTRL(i));
          leave_critical_section(irqflags);
          return i;
        }
    }

  leave_critical_section(irqflags);
  return -ENOSPC;
}

/****************************************************************************
 * Name: da1470x_pdc_remove
 ****************************************************************************/

void da1470x_pdc_remove(int index)
{
  if (index >= 0 && index < DA1470X_PDC_LUT_SIZE)
    {
      putreg32(0, PDC_CTRL(index));
      da1470x_pdc_acknowledge(index);
    }
}

/****************************************************************************
 * Name: da1470x_pdc_set_pending / da1470x_pdc_acknowledge
 ****************************************************************************/

void da1470x_pdc_set_pending(int index)
{
  if (index >= 0 && index < DA1470X_PDC_LUT_SIZE)
    {
      putreg32(index, DA1470X_PDC_SET_PENDING);
    }
}

void da1470x_pdc_acknowledge(int index)
{
  if (index >= 0 && index < DA1470X_PDC_LUT_SIZE)
    {
      putreg32(index, DA1470X_PDC_ACKNOWLEDGE);
    }
}

/****************************************************************************
 * Name: da1470x_pdc_is_pending
 ****************************************************************************/

bool da1470x_pdc_is_pending(int index)
{
  if (index < 0 || index >= DA1470X_PDC_LUT_SIZE)
    {
      return false;
    }

  return (getreg32(DA1470X_PDC_PENDING_CM33) & (1u << index)) != 0;
}

/****************************************************************************
 * Name: da1470x_pdc_ack_all_cm33
 ****************************************************************************/

void da1470x_pdc_ack_all_cm33(void)
{
  uint32_t pending = getreg32(DA1470X_PDC_PENDING_CM33) &
                     PDC_PENDING_CM33_PENDING_MASK;
  int i;

  while (pending != 0)
    {
      i = __builtin_ctz(pending);
      pending &= ~(1u << i);
      putreg32(i, DA1470X_PDC_ACKNOWLEDGE);
    }
}

#endif /* CONFIG_DA1470X_PDC */
