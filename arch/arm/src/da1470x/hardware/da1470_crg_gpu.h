/****************************************************************************
 * ./tmp/da1470_crg_gpu.h
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

#ifndef __DA1470_CRG_GPU_H
#define __DA1470_CRG_GPU_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register offsets *********************************************************/

#define DA1470_CRG_GPU_CLK_GPU_OFFSET         0x0004 /* Control register for clocks in PD_GPU */

/* Register addresses *******************************************************/

#define DA1470_CRG_GPU_BASE                   0x51001000 /* CRG_GPU registers */

#define DA1470_CRG_GPU_CLK_GPU                (DA1470_CRG_GPU_BASE + DA1470_CRG_GPU_CLK_GPU_OFFSET) /* Control register for clocks in PD_GPU */

/* Register bit definitions *************************************************/


/* CLK_GPU Register */

#define CRG_GPU_MIPI_D_PHY_EN                 (1U << 3) /* No description available. */
#define CRG_GPU_MIPI_PHY_EN                   (1U << 2) /* No description available. */
#define CRG_GPU_MIPI_DSI_EN                   (1U << 1) /* No description available. */
#define CRG_GPU_GPU_ENABLE                    (1U << 0) /* No description available. */

#endif /* __DA1470_CRG_GPU_H */
