/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_crg_gpu.h
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

/* Generated from the Renesas CMSIS device header DA1470x-00.h (SDK
 * 10.2.6.49).  Do not edit by hand; regenerate with tools/genhdr.py.
 */

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_CRG_GPU_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_CRG_GPU_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470x_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define DA1470X_CRG_GPU_CLK_GPU_OFFSET 0x0004 /* Control register for clocks in PD_GPU */

/* Register Addresses *******************************************************/

#define DA1470X_CRG_GPU_CLK_GPU (DA1470X_CRG_GPU_BASE + DA1470X_CRG_GPU_CLK_GPU_OFFSET)

/* Register Bit-field Definitions *******************************************/

/* CLK_GPU register */

#define CRG_GPU_CLK_GPU_GPU_ENABLE (1 << 0) /* Bit 0 */

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_CRG_GPU_H */
