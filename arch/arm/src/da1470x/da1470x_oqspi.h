/****************************************************************************
 * arch/arm/src/da1470x/da1470x_oqspi.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_OQSPI_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_OQSPI_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/mtd/mtd.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Geometry of the boot flash (Macronix MX25U6432) */

#define DA1470X_OQSPI_FLASH_SIZE      (8 * 1024 * 1024)
#define DA1470X_OQSPI_PAGE_SIZE       256
#define DA1470X_OQSPI_SECTOR_SIZE     4096

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
 * Name: da1470x_oqspi_initialize
 *
 * Description:
 *   Return an MTD device covering the whole boot flash.  The caller is
 *   expected to carve out a partition that does not overlap the code
 *   image (see CONFIG_DA1470X_OQSPI_MTD_OFFSET).
 *
 ****************************************************************************/

struct mtd_dev_s *da1470x_oqspi_initialize(void);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_OQSPI_H */
