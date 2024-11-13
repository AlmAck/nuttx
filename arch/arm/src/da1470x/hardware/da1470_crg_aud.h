/****************************************************************************
 * ./tmp/da1470_crg_aud.h
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

#ifndef __DA1470_CRG_AUD_H
#define __DA1470_CRG_AUD_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register offsets *********************************************************/

#define DA1470_CRG_AUD_PCM_DIV_OFFSET         0x0040 /* PCM divider and enables */
#define DA1470_CRG_AUD_PCM_FDIV_OFFSET        0x0044 /* PCM fractional division register */
#define DA1470_CRG_AUD_PDM_DIV_OFFSET         0x0048 /* PDM divider and enables */
#define DA1470_CRG_AUD_SRC_DIV_OFFSET         0x004C /* SRC divider and enables */

/* Register addresses *******************************************************/

#define DA1470_CRG_AUD_BASE                   0x50030000 /* CRG_AUD registers */

#define DA1470_CRG_AUD_PCM_DIV                (DA1470_CRG_AUD_BASE + DA1470_CRG_AUD_PCM_DIV_OFFSET) /* PCM divider and enables */
#define DA1470_CRG_AUD_PCM_FDIV               (DA1470_CRG_AUD_BASE + DA1470_CRG_AUD_PCM_FDIV_OFFSET) /* PCM fractional division register */
#define DA1470_CRG_AUD_PDM_DIV                (DA1470_CRG_AUD_BASE + DA1470_CRG_AUD_PDM_DIV_OFFSET) /* PDM divider and enables */
#define DA1470_CRG_AUD_SRC_DIV                (DA1470_CRG_AUD_BASE + DA1470_CRG_AUD_SRC_DIV_OFFSET) /* SRC divider and enables */

/* Register bit definitions *************************************************/


/* PCM_DIV Register */

#define CRG_AUD_PCM_SRC_SEL                   (1U << 13) /* Selects the clock source 1 = DIV1 clock 0 = DIVN clock */
#define CRG_AUD_CLK_PCM_EN                    (1U << 12) /* Enable for the internally generated PCM clock The PCM_DIV must be set before or  */
#define CRG_AUD_PCM_DIV_MASK                  (0xFFF << 0) /* PCM clock divider. Minimum value is 0x2. */
#define CRG_AUD_PCM_DIV_POS                   (0) /* PCM clock divider. Minimum value is 0x2. */

/* PCM_FDIV Register */

#define CRG_AUD_PCM_FDIV_MASK                 (0xFFFF << 0) /* These bits define the fractional division part of the PCM clock. The left most ' */
#define CRG_AUD_PCM_FDIV_POS                  (0) /* These bits define the fractional division part of the PCM clock. The left most ' */

/* PDM_DIV Register */

#define CRG_AUD_PDM_MASTER_MODE               (1U << 9) /* Master mode selection 0: slave mode 1: master mode */
#define CRG_AUD_CLK_PDM_EN                    (1U << 8) /* Enable for the internally generated PDM clock The PDM_DIV must be set before or  */
#define CRG_AUD_PDM_DIV_MASK                  (0xFF << 0) /* PDM clock divider */
#define CRG_AUD_PDM_DIV_POS                   (0) /* PDM clock divider */

/* SRC_DIV Register */

#define CRG_AUD_CLK_SRC2_EN                   (1U << 17) /* Enable for the internally generated SRC2 clock The SRC2_DIV must be set before o */
#define CRG_AUD_CLK_SRC_EN                    (1U << 16) /* Enable for the internally generated SRC clock The SRC_DIV must be set before or  */
#define CRG_AUD_SRC2_DIV_MASK                 (0xFF << 8) /* SRC2 clock divider */
#define CRG_AUD_SRC2_DIV_POS                  (8) /* SRC2 clock divider */
#define CRG_AUD_SRC_DIV_MASK                  (0xFF << 0) /* SRC clock divider */
#define CRG_AUD_SRC_DIV_POS                   (0) /* SRC clock divider */

#endif /* __DA1470_CRG_AUD_H */
