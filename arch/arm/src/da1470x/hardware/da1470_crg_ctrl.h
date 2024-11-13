/****************************************************************************
 * ./tmp/da1470_crg_ctrl.h
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

#ifndef __DA1470_CRG_CTRL_H
#define __DA1470_CRG_CTRL_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register offsets *********************************************************/

#define DA1470_CRG_CTRL_CLK_PDCTRL_OFFSET     0x0004 /* Clock control settings for PD_CTRL */

/* Register addresses *******************************************************/

#define DA1470_CRG_CTRL_BASE                  0x50060000 /* CRG_CTRL registers */

#define DA1470_CRG_CTRL_CLK_PDCTRL            (DA1470_CRG_CTRL_BASE + DA1470_CRG_CTRL_CLK_PDCTRL_OFFSET) /* Clock control settings for PD_CTRL */

/* Register bit definitions *************************************************/


/* CLK_PDCTRL Register */

#define CRG_CTRL_EMMC_INV_TX_CLK              (1U << 13) /* Inverts the clock in the TX path */
#define CRG_CTRL_EMMC_INV_RX_CLK              (1U << 12) /* Invert the clock in the RX path, cascaded with INV_TX_CLK */
#define CRG_CTRL_EMMC_ENABLE                  (1U << 11) /* Enables the clock. */
#define CRG_CTRL_EMMC_CLK_DIV_MASK            (0xF << 7) /* clock divider setting 0x0 : divide by 16 0x1 : divide by 1 0x2 : divide by 2 0x4 */
#define CRG_CTRL_EMMC_CLK_DIV_POS             (7) /* clock divider setting 0x0 : divide by 16 0x1 : divide by 1 0x2 : divide by 2 0x4 */
#define CRG_CTRL_SDIO_INV_TX_CLK              (1U << 6) /* Inverts the clock in the TX path */
#define CRG_CTRL_SDIO_INV_RX_CLK              (1U << 5) /* Invert the clock in the RX path, cascaded with INV_TX_CLK */
#define CRG_CTRL_SDIO_ENABLE                  (1U << 4) /* Enables the clock. */
#define CRG_CTRL_SDIO_CLK_DIV_MASK            (0xF << 0) /* clock divider setting 0x0 : divide by 16 0x1 : divide by 1 0x2 : divide by 2 0x4 */
#define CRG_CTRL_SDIO_CLK_DIV_POS             (0) /* clock divider setting 0x0 : divide by 16 0x1 : divide by 1 0x2 : divide by 2 0x4 */

#endif /* __DA1470_CRG_CTRL_H */
