/****************************************************************************
 * arch/arm/src/da1470x/da1470x_lcdc.c
 *
 * Minimal LCDC bring-up for DA1470x. Powers PD_GPU and the LCDC clock
 * via CRG_SYS, then verifies the controller is alive by reading the
 * IDREG magic. Panel-specific mode programming (DBI / DPI / DSI) and
 * the command/pixel data path are deliberately deferred to a panel
 * driver layer once we have a real panel attached and TE/RESX pins
 * routed.
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to you under the Apache License, Version
 * 2.0 (the "License"); you may not use this file except in compliance
 * with the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <debug.h>
#include <errno.h>
#include <stdint.h>

#include <nuttx/arch.h>

#include "arm_internal.h"
#include "hardware/da1470x_crg_sys.h"
#include "hardware/da1470x_lcdc.h"
#include "da1470x_lcdc.h"
#include "da1470x_pmu.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int da1470x_lcdc_initialize(void)
{
  /* PD_GPU hosts the LCDC register region at 0x30030000. Without it
   * powered, accesses produce undefined results / bus errors. The
   * pd_enable helper blocks until SYS_STAT.GPU_IS_UP asserts.
   */

  da1470x_pd_enable(DA1470X_PD_GPU);

  /* Release any pending LCD soft-reset. CRG_SYS has the SET/RESET-style
   * register pair: writing 1 to a bit in SET_CLK_SYS sets it; writing 1
   * to the same bit in RESET_CLK_SYS clears it.
   */

  putreg32(CRG_SYS_LCD_RESET_REQ, DA1470_CRG_SYS_RESET_CLK_SYS);

  /* Select DivN (= 32 MHz) as the LCDC source clock. Clear CLK_SEL via
   * the RESET register, then set ENABLE via the SET register.
   */

  putreg32(CRG_SYS_LCD_CLK_SEL, DA1470_CRG_SYS_RESET_CLK_SYS);
  putreg32(CRG_SYS_LCD_ENABLE,  DA1470_CRG_SYS_SET_CLK_SYS);

  /* Sanity check: the LCDC IDREG holds a documented magic value. If we
   * don't read it back, either the power domain didn't actually come
   * up, the clock isn't gated, or the LCDC base address is wrong for
   * this silicon revision.
   */

  uint32_t id = getreg32(DA1470_LCDC_IDREG);
  if (id != DA1470_LCDC_IDREG_MAGIC)
    {
      lcderr("LCDC IDREG mismatch: read 0x%08lx, expected 0x%08lx\n",
             (unsigned long)id, (unsigned long)DA1470_LCDC_IDREG_MAGIC);
      return -ENODEV;
    }

  return OK;
}
