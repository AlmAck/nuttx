/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_otpc.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_OTPC_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_OTPC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470x_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define DA1470X_OTPC_MODE_OFFSET 0x0000  /* Mode register */
#define DA1470X_OTPC_STAT_OFFSET 0x0004  /* Status register */
#define DA1470X_OTPC_PADDR_OFFSET 0x0008 /* The address of the word that will be pro */
#define DA1470X_OTPC_PWORD_OFFSET 0x000c /* The 32-bit word that will be programmed, */
#define DA1470X_OTPC_TIM1_OFFSET 0x0010  /* Various timing parameters of the OTP cel */
#define DA1470X_OTPC_TIM2_OFFSET 0x0014  /* Various timing parameters of the OTP cel */

/* Register Addresses *******************************************************/

#define DA1470X_OTPC_MODE (DA1470X_OTPC_BASE + DA1470X_OTPC_MODE_OFFSET)
#define DA1470X_OTPC_STAT (DA1470X_OTPC_BASE + DA1470X_OTPC_STAT_OFFSET)
#define DA1470X_OTPC_PADDR (DA1470X_OTPC_BASE + DA1470X_OTPC_PADDR_OFFSET)
#define DA1470X_OTPC_PWORD (DA1470X_OTPC_BASE + DA1470X_OTPC_PWORD_OFFSET)
#define DA1470X_OTPC_TIM1 (DA1470X_OTPC_BASE + DA1470X_OTPC_TIM1_OFFSET)
#define DA1470X_OTPC_TIM2 (DA1470X_OTPC_BASE + DA1470X_OTPC_TIM2_OFFSET)

/* Register Bit-field Definitions *******************************************/

/* MODE register */

#define OTPC_MODE_MODE_MODE_SHIFT (0)       /* Bits 0-2 */
#define OTPC_MODE_MODE_MODE_MASK (0x7 << OTPC_MODE_MODE_MODE_SHIFT)
#  define OTPC_MODE_MODE_MODE(n) ((uint32_t)(n) << OTPC_MODE_MODE_MODE_SHIFT)
#define OTPC_MODE_MODE_USE_TST_ROW (1 << 4) /* Bit 4 */
#define OTPC_MODE_MODE_HT_MARG_EN (1 << 5)  /* Bit 5 */
#define OTPC_MODE_MODE_PRG_SEL_SHIFT (6)    /* Bits 6-7 */
#define OTPC_MODE_MODE_PRG_SEL_MASK (0x3 << OTPC_MODE_MODE_PRG_SEL_SHIFT)
#  define OTPC_MODE_MODE_PRG_SEL(n) ((uint32_t)(n) << OTPC_MODE_MODE_PRG_SEL_SHIFT)

/* STAT register */

#define OTPC_STAT_STAT_PRDY (1 << 0)       /* Bit 0 */
#define OTPC_STAT_STAT_PBUF_EMPTY (1 << 1) /* Bit 1 */
#define OTPC_STAT_STAT_MRDY (1 << 2)       /* Bit 2 */

/* PADDR register */

#define OTPC_PADDR_PADDR_SHIFT (0) /* Bits 0-9 */
#define OTPC_PADDR_PADDR_MASK (0x3ff << OTPC_PADDR_PADDR_SHIFT)
#  define OTPC_PADDR_PADDR(n) ((uint32_t)(n) << OTPC_PADDR_PADDR_SHIFT)

/* PWORD register */

#define OTPC_PWORD_PWORD_SHIFT (0) /* Bits 0-31 */
#define OTPC_PWORD_PWORD_MASK (0xffffffff << OTPC_PWORD_PWORD_SHIFT)
#  define OTPC_PWORD_PWORD(n) ((uint32_t)(n) << OTPC_PWORD_PWORD_SHIFT)

/* TIM1 register */

#define OTPC_TIM1_TIM1_CC_T_1US_SHIFT (0)  /* Bits 0-7 */
#define OTPC_TIM1_TIM1_CC_T_1US_MASK (0xff << OTPC_TIM1_TIM1_CC_T_1US_SHIFT)
#  define OTPC_TIM1_TIM1_CC_T_1US(n) ((uint32_t)(n) << OTPC_TIM1_TIM1_CC_T_1US_SHIFT)
#define OTPC_TIM1_TIM1_CC_T_20NS_SHIFT (8) /* Bits 8-10 */
#define OTPC_TIM1_TIM1_CC_T_20NS_MASK (0x7 << OTPC_TIM1_TIM1_CC_T_20NS_SHIFT)
#  define OTPC_TIM1_TIM1_CC_T_20NS(n) ((uint32_t)(n) << OTPC_TIM1_TIM1_CC_T_20NS_SHIFT)
#define OTPC_TIM1_TIM1_CC_T_RD_SHIFT (12)  /* Bits 12-15 */
#define OTPC_TIM1_TIM1_CC_T_RD_MASK (0xf << OTPC_TIM1_TIM1_CC_T_RD_SHIFT)
#  define OTPC_TIM1_TIM1_CC_T_RD(n) ((uint32_t)(n) << OTPC_TIM1_TIM1_CC_T_RD_SHIFT)
#define OTPC_TIM1_TIM1_US_T_PL_SHIFT (16)  /* Bits 16-19 */
#define OTPC_TIM1_TIM1_US_T_PL_MASK (0xf << OTPC_TIM1_TIM1_US_T_PL_SHIFT)
#  define OTPC_TIM1_TIM1_US_T_PL(n) ((uint32_t)(n) << OTPC_TIM1_TIM1_US_T_PL_SHIFT)
#define OTPC_TIM1_TIM1_US_T_CS_SHIFT (20)  /* Bits 20-23 */
#define OTPC_TIM1_TIM1_US_T_CS_MASK (0xf << OTPC_TIM1_TIM1_US_T_CS_SHIFT)
#  define OTPC_TIM1_TIM1_US_T_CS(n) ((uint32_t)(n) << OTPC_TIM1_TIM1_US_T_CS_SHIFT)
#define OTPC_TIM1_TIM1_US_T_CSP_SHIFT (24) /* Bits 24-30 */
#define OTPC_TIM1_TIM1_US_T_CSP_MASK (0x7f << OTPC_TIM1_TIM1_US_T_CSP_SHIFT)
#  define OTPC_TIM1_TIM1_US_T_CSP(n) ((uint32_t)(n) << OTPC_TIM1_TIM1_US_T_CSP_SHIFT)

/* TIM2 register */

#define OTPC_TIM2_TIM2_US_T_PW_SHIFT (0)      /* Bits 0-4 */
#define OTPC_TIM2_TIM2_US_T_PW_MASK (0x1f << OTPC_TIM2_TIM2_US_T_PW_SHIFT)
#  define OTPC_TIM2_TIM2_US_T_PW(n) ((uint32_t)(n) << OTPC_TIM2_TIM2_US_T_PW_SHIFT)
#define OTPC_TIM2_TIM2_US_T_PWI_SHIFT (5)     /* Bits 5-7 */
#define OTPC_TIM2_TIM2_US_T_PWI_MASK (0x7 << OTPC_TIM2_TIM2_US_T_PWI_SHIFT)
#  define OTPC_TIM2_TIM2_US_T_PWI(n) ((uint32_t)(n) << OTPC_TIM2_TIM2_US_T_PWI_SHIFT)
#define OTPC_TIM2_TIM2_US_T_PPR_SHIFT (8)     /* Bits 8-14 */
#define OTPC_TIM2_TIM2_US_T_PPR_MASK (0x7f << OTPC_TIM2_TIM2_US_T_PPR_SHIFT)
#  define OTPC_TIM2_TIM2_US_T_PPR(n) ((uint32_t)(n) << OTPC_TIM2_TIM2_US_T_PPR_SHIFT)
#define OTPC_TIM2_TIM2_US_T_PPS_SHIFT (16)    /* Bits 16-20 */
#define OTPC_TIM2_TIM2_US_T_PPS_MASK (0x1f << OTPC_TIM2_TIM2_US_T_PPS_SHIFT)
#  define OTPC_TIM2_TIM2_US_T_PPS(n) ((uint32_t)(n) << OTPC_TIM2_TIM2_US_T_PPS_SHIFT)
#define OTPC_TIM2_TIM2_US_T_VDS_SHIFT (21)    /* Bits 21-23 */
#define OTPC_TIM2_TIM2_US_T_VDS_MASK (0x7 << OTPC_TIM2_TIM2_US_T_VDS_SHIFT)
#  define OTPC_TIM2_TIM2_US_T_VDS(n) ((uint32_t)(n) << OTPC_TIM2_TIM2_US_T_VDS_SHIFT)
#define OTPC_TIM2_TIM2_US_T_PPH_SHIFT (24)    /* Bits 24-28 */
#define OTPC_TIM2_TIM2_US_T_PPH_MASK (0x1f << OTPC_TIM2_TIM2_US_T_PPH_SHIFT)
#  define OTPC_TIM2_TIM2_US_T_PPH(n) ((uint32_t)(n) << OTPC_TIM2_TIM2_US_T_PPH_SHIFT)
#define OTPC_TIM2_TIM2_US_T_SAS_SHIFT (29)    /* Bits 29-30 */
#define OTPC_TIM2_TIM2_US_T_SAS_MASK (0x3 << OTPC_TIM2_TIM2_US_T_SAS_SHIFT)
#  define OTPC_TIM2_TIM2_US_T_SAS(n) ((uint32_t)(n) << OTPC_TIM2_TIM2_US_T_SAS_SHIFT)
#define OTPC_TIM2_TIM2_US_ADD_CC_EN (1 << 31) /* Bit 31 */

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_OTPC_H */
