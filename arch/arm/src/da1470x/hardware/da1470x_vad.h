/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_vad.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_VAD_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_VAD_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "hardware/da1470x_memorymap.h"
#include <nuttx/config.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register offsets *********************************************************/

#define DA1470X_VAD_CTRL0_OFFSET 0x0000 /* VAD Control Register 0 */
#define DA1470X_VAD_CTRL1_OFFSET 0x0004 /* VAD Control Register 1 */
#define DA1470X_VAD_CTRL2_OFFSET 0x0008 /* VAD Control Register 2 */
#define DA1470X_VAD_CTRL3_OFFSET 0x000C /* VAD Control Register 3 */
#define DA1470X_VAD_CTRL4_OFFSET 0x0010 /* VAD Control Register 4 */

/* Register addresses *******************************************************/

#define DA1470X_VAD_CTRL0 (DA1470X_VAD_BASE + DA1470X_VAD_CTRL0_OFFSET)
#define DA1470X_VAD_CTRL1 (DA1470X_VAD_BASE + DA1470X_VAD_CTRL1_OFFSET)
#define DA1470X_VAD_CTRL2 (DA1470X_VAD_BASE + DA1470X_VAD_CTRL2_OFFSET)
#define DA1470X_VAD_CTRL3 (DA1470X_VAD_BASE + DA1470X_VAD_CTRL3_OFFSET)
#define DA1470X_VAD_CTRL4 (DA1470X_VAD_BASE + DA1470X_VAD_CTRL4_OFFSET)

/* Register bit definitions *************************************************/

/* VAD_CTRL3_REG */

#define VAD_CTRL3_VAD_SB (1 << 7)    /* Bit 7: Stand-by mode (1: deactivated) */
#define VAD_CTRL3_VAD_SLEEP (1 << 6) /* Bit 6: Sleep mode (1: sleep) */

/* VAD_CTRL4_REG */

#define VAD_CTRL4_VAD_IRQ_MODE (1 << 2) /* Bit 2: IRQ mode selection */
#define VAD_CTRL4_VAD_IRQ_FLAG                                                 \
  (1 << 1) /* Bit 1: Interrupt ReQuest flag (W1C) */

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_VAD_H */
