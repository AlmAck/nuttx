/****************************************************************************
 * arch/arm/src/da1470x/da1470x_tcs.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_TCS_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_TCS_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <stddef.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Group identifiers used by the factory configuration script.  Groups up to
 * 0x09 and the two radio mode groups hold register address and value pairs;
 * every other group holds plain trim values.
 */

#define DA1470X_TCS_PD_SYS          0x01  /* Always-on system registers */
#define DA1470X_TCS_PD_SNC          0x02  /* Sensor node domain */
#define DA1470X_TCS_PD_MEM          0x03  /* Memory domain */
#define DA1470X_TCS_PD_TMR          0x04  /* Timer domain */
#define DA1470X_TCS_PD_AUDIO        0x05  /* Audio domain */
#define DA1470X_TCS_PD_RAD          0x06  /* Radio domain */
#define DA1470X_TCS_PD_SYNTH        0x07  /* Synthesiser */
#define DA1470X_TCS_PD_GPU          0x08  /* Graphics domain */
#define DA1470X_TCS_PD_CTRL         0x09  /* Controller domain */
#define DA1470X_TCS_BD_ADDR         0x20  /* Factory Bluetooth address */
#define DA1470X_TCS_SD_ADC_SINGLE   0x40
#define DA1470X_TCS_SD_ADC_DIFF     0x41
#define DA1470X_TCS_GP_ADC_SINGLE   0x42
#define DA1470X_TCS_GP_ADC_DIFF     0x43
#define DA1470X_TCS_TEMP_SENS_25C   0x44
#define DA1470X_TCS_TEMP_SENS_BG    0x45
#define DA1470X_TCS_BUCK_TRIM       0x46
#define DA1470X_TCS_RCHS_64MHZ      0x47
#define DA1470X_TCS_RCLP_32KHZ      0x48
#define DA1470X_TCS_PD_RAD_MODE1    0x80
#define DA1470X_TCS_PD_SYNTH_MODE1  0x81
#define DA1470X_TCS_PD_RAD_MODE2    0x82
#define DA1470X_TCS_PD_SYNTH_MODE2  0x83
#define DA1470X_TCS_PD_RAD_COEFF    0x84
#define DA1470X_TCS_CHIP_ID         0xc0
#define DA1470X_TCS_PROD_INFO       0xc1
#define DA1470X_TCS_PROD_WAFER      0xc2
#define DA1470X_TCS_TESTPROG_VER    0xc3

/* One past the highest group identifier that is stored */

#define DA1470X_TCS_GROUP_MAX       0xe0

/* Layout of one entry of the attribute array.  The Bluetooth controller
 * firmware reads the array with the same layout, so it must not change.
 */

#define TCS_ATTR_TYPE_SHIFT         0
#define TCS_ATTR_TYPE_MASK          (0x1 << TCS_ATTR_TYPE_SHIFT)
#define TCS_ATTR_START_SHIFT        1
#define TCS_ATTR_START_MASK         (0x1ff << TCS_ATTR_START_SHIFT)
#define TCS_ATTR_SIZE_SHIFT         10
#define TCS_ATTR_SIZE_MASK          (0x3f << TCS_ATTR_SIZE_SHIFT)

#define TCS_ATTR_TYPE_TRIM          0  /* Plain values */
#define TCS_ATTR_TYPE_REG_PAIR      1  /* Register address and value pairs */

#define TCS_ATTR_START_EMPTY        0x1ff  /* Group not present */

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_tcs_initialize
 *
 * Description:
 *   Read the factory configuration script from the one-time-programmable
 *   memory and keep the trim values it carries.  Must be called early in
 *   the boot path, while the system still runs on the 32MHz oscillator,
 *   and before anything that depends on a trimmed block.
 *
 *   When no configuration script is found the chip keeps its reset trim
 *   values: everything still runs, but the radio is far off target.
 *
 ****************************************************************************/

void da1470x_tcs_initialize(void);

/****************************************************************************
 * Name: da1470x_tcs_apply
 *
 * Description:
 *   Write the register address and value pairs of one group.  The pairs of
 *   a power domain have to be written again every time that domain is
 *   powered up.
 *
 * Input Parameters:
 *   gid - Group identifier, one of the DA1470X_TCS_PD_* values.
 *
 * Returned Value:
 *   The number of registers written, zero if the group is absent, or a
 *   negated errno value if the group does not hold register pairs.
 *
 ****************************************************************************/

int da1470x_tcs_apply(uint8_t gid);

/****************************************************************************
 * Name: da1470x_tcs_values
 *
 * Description:
 *   Return the stored words of one group.
 *
 * Input Parameters:
 *   gid    - Group identifier.
 *   values - Receives a pointer to the first word of the group.
 *   count  - Receives the number of words.
 *
 * Returned Value:
 *   Zero on success, -ENOENT if the group is not present.
 *
 ****************************************************************************/

int da1470x_tcs_values(uint8_t gid, const uint32_t **values,
                       uint8_t *count);

/****************************************************************************
 * Name: da1470x_tcs_attributes
 *
 * Description:
 *   Return the attribute array, indexed by group identifier.  It is handed
 *   to the Bluetooth controller, which looks its own groups up in it.
 *
 * Input Parameters:
 *   count - Receives the number of entries, DA1470X_TCS_GROUP_MAX.
 *
 ****************************************************************************/

const uint16_t *da1470x_tcs_attributes(size_t *count);

/****************************************************************************
 * Name: da1470x_tcs_data
 *
 * Description:
 *   Return the value array the attributes point into.
 *
 * Input Parameters:
 *   count - Receives the number of words in use.
 *
 ****************************************************************************/

const uint32_t *da1470x_tcs_data(size_t *count);

/****************************************************************************
 * Name: da1470x_tcs_bdaddr
 *
 * Description:
 *   Return the Bluetooth device address programmed at the factory.  Not
 *   every part carries one.
 *
 * Input Parameters:
 *   addr - Receives the six address bytes, least significant first.
 *
 * Returned Value:
 *   Zero on success, -ENOENT if the part has no factory address.
 *
 ****************************************************************************/

int da1470x_tcs_bdaddr(uint8_t addr[6]);

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_TCS_H */
