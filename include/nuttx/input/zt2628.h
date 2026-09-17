/****************************************************************************
 * include/nuttx/input/zt2628.h
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

#ifndef __INCLUDE_NUTTX_INPUT_ZT2628_H
#define __INCLUDE_NUTTX_INPUT_ZT2628_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/irq.h>
#include <nuttx/i2c/i2c_master.h>

#include <stdbool.h>
#include <stdint.h>

#ifdef CONFIG_INPUT_ZT2628

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define ZT2628_I2C_ADDRESS   0x20

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* Board glue for the Zinitix ZT2628 capacitive touch controller.  All the
 * pin handling sits behind callbacks so the driver does not depend on the
 * MCU's GPIO API.
 */

struct zt2628_config_s
{
  uint8_t  address;      /* 7-bit I2C address (0x20) */
  uint32_t frequency;    /* I2C frequency, up to 400 kHz */
  uint16_t xres;         /* Panel width, for clamping */
  uint16_t yres;         /* Panel height, for clamping */

  /* Attach the interrupt handler to the INT line (falling edge) */

  int  (*attach)(FAR const struct zt2628_config_s *config, xcpt_t isr,
                 FAR void *arg);

  /* Mask or unmask the INT line */

  void (*enable)(FAR const struct zt2628_config_s *config, bool enable);

  /* Drive the controller supply and reset line */

  void (*power)(FAR const struct zt2628_config_s *config, bool on);
  void (*reset)(FAR const struct zt2628_config_s *config, bool assert);
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Name: zt2628_register
 *
 * Description:
 *   Power up and initialise the controller and register it as
 *   /dev/inputN.
 *
 ****************************************************************************/

int zt2628_register(FAR struct i2c_master_s *i2c,
                    FAR const struct zt2628_config_s *config, int minor);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* CONFIG_INPUT_ZT2628 */
#endif /* __INCLUDE_NUTTX_INPUT_ZT2628_H */
