/****************************************************************************
 * arch/arm/src/da1470x/da1470x_lcdc.h
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to you under the Apache License, Version
 * 2.0 (the "License"); you may not use this file except in compliance
 * with the License.
 *
 ****************************************************************************/

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_LCDC_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_LCDC_H

#include <nuttx/config.h>
#include <stdbool.h>
#include <stdint.h>

/****************************************************************************
 * Name: da1470x_lcdc_initialize
 *
 * Description:
 *   Bring up the LCDC block:
 *     1. Power up PD_GPU so the LCDC register region at 0x30030000 is
 *        actually accessible (reads return bus errors otherwise).
 *     2. Enable the LCDC clock in CRG_SYS (LCD_ENABLE, LCD_CLK_SEL=DivN).
 *     3. Release the LCD_RESET_REQ if it was asserted.
 *     4. Verify the controller is alive by reading LCDC_IDREG and
 *        comparing against the documented magic 0x87452365.
 *
 *   No panel/PHY-specific config is done here — the caller (panel driver)
 *   is responsible for configuring LCDC_MODE / DBIB_CFG / timings.
 *
 * Returned Value:
 *   OK on success; -ENODEV if the IDREG read-back doesn't match.
 *
 ****************************************************************************/

int da1470x_lcdc_initialize(void);

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_LCDC_H */
