/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_pdc.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_PDC_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_PDC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "da1470x_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define DA1470X_PDC_CTRL0_OFFSET 0x0000        /* PDC control register */
#define DA1470X_PDC_CTRL1_OFFSET 0x0004        /* PDC control register */
#define DA1470X_PDC_CTRL2_OFFSET 0x0008        /* PDC control register */
#define DA1470X_PDC_CTRL3_OFFSET 0x000c        /* PDC control register */
#define DA1470X_PDC_CTRL4_OFFSET 0x0010        /* PDC control register */
#define DA1470X_PDC_CTRL5_OFFSET 0x0014        /* PDC control register */
#define DA1470X_PDC_CTRL6_OFFSET 0x0018        /* PDC control register */
#define DA1470X_PDC_CTRL7_OFFSET 0x001c        /* PDC control register */
#define DA1470X_PDC_CTRL8_OFFSET 0x0020        /* PDC control register */
#define DA1470X_PDC_CTRL9_OFFSET 0x0024        /* PDC control register */
#define DA1470X_PDC_CTRL10_OFFSET 0x0028       /* PDC control register */
#define DA1470X_PDC_CTRL11_OFFSET 0x002c       /* PDC control register */
#define DA1470X_PDC_CTRL12_OFFSET 0x0030       /* PDC control register */
#define DA1470X_PDC_CTRL13_OFFSET 0x0034       /* PDC control register */
#define DA1470X_PDC_CTRL14_OFFSET 0x0038       /* PDC control register */
#define DA1470X_PDC_CTRL15_OFFSET 0x003c       /* PDC control register */
#define DA1470X_PDC_ACKNOWLEDGE_OFFSET 0x0080  /* Clear a pending PDC bit */
#define DA1470X_PDC_PENDING_OFFSET 0x0084      /* Shows any pending wakup event */
#define DA1470X_PDC_PENDING_SNC_OFFSET 0x0088  /* Shows any pending IRQ to SNC */
#define DA1470X_PDC_PENDING_CM33_OFFSET 0x008c /* Shows any pending IRQ to CM33 */
#define DA1470X_PDC_PENDING_CMAC_OFFSET 0x0090 /* Shows any pending IRQ to CMAC */
#define DA1470X_PDC_SET_PENDING_OFFSET 0x0094  /* Set a pending PDC bit */

/* Register Addresses *******************************************************/

#define DA1470X_PDC_CTRL0 (DA1470X_PDC_BASE + DA1470X_PDC_CTRL0_OFFSET)
#define DA1470X_PDC_CTRL1 (DA1470X_PDC_BASE + DA1470X_PDC_CTRL1_OFFSET)
#define DA1470X_PDC_CTRL2 (DA1470X_PDC_BASE + DA1470X_PDC_CTRL2_OFFSET)
#define DA1470X_PDC_CTRL3 (DA1470X_PDC_BASE + DA1470X_PDC_CTRL3_OFFSET)
#define DA1470X_PDC_CTRL4 (DA1470X_PDC_BASE + DA1470X_PDC_CTRL4_OFFSET)
#define DA1470X_PDC_CTRL5 (DA1470X_PDC_BASE + DA1470X_PDC_CTRL5_OFFSET)
#define DA1470X_PDC_CTRL6 (DA1470X_PDC_BASE + DA1470X_PDC_CTRL6_OFFSET)
#define DA1470X_PDC_CTRL7 (DA1470X_PDC_BASE + DA1470X_PDC_CTRL7_OFFSET)
#define DA1470X_PDC_CTRL8 (DA1470X_PDC_BASE + DA1470X_PDC_CTRL8_OFFSET)
#define DA1470X_PDC_CTRL9 (DA1470X_PDC_BASE + DA1470X_PDC_CTRL9_OFFSET)
#define DA1470X_PDC_CTRL10 (DA1470X_PDC_BASE + DA1470X_PDC_CTRL10_OFFSET)
#define DA1470X_PDC_CTRL11 (DA1470X_PDC_BASE + DA1470X_PDC_CTRL11_OFFSET)
#define DA1470X_PDC_CTRL12 (DA1470X_PDC_BASE + DA1470X_PDC_CTRL12_OFFSET)
#define DA1470X_PDC_CTRL13 (DA1470X_PDC_BASE + DA1470X_PDC_CTRL13_OFFSET)
#define DA1470X_PDC_CTRL14 (DA1470X_PDC_BASE + DA1470X_PDC_CTRL14_OFFSET)
#define DA1470X_PDC_CTRL15 (DA1470X_PDC_BASE + DA1470X_PDC_CTRL15_OFFSET)
#define DA1470X_PDC_ACKNOWLEDGE (DA1470X_PDC_BASE + DA1470X_PDC_ACKNOWLEDGE_OFFSET)
#define DA1470X_PDC_PENDING (DA1470X_PDC_BASE + DA1470X_PDC_PENDING_OFFSET)
#define DA1470X_PDC_PENDING_SNC (DA1470X_PDC_BASE + DA1470X_PDC_PENDING_SNC_OFFSET)
#define DA1470X_PDC_PENDING_CM33 (DA1470X_PDC_BASE + DA1470X_PDC_PENDING_CM33_OFFSET)
#define DA1470X_PDC_PENDING_CMAC (DA1470X_PDC_BASE + DA1470X_PDC_PENDING_CMAC_OFFSET)
#define DA1470X_PDC_SET_PENDING (DA1470X_PDC_BASE + DA1470X_PDC_SET_PENDING_OFFSET)

/* Register Bit-field Definitions *******************************************/

/* CTRL0 register */

#define PDC_CTRL0_TRIG_SELECT_SHIFT (0) /* Bits 0-1 */
#define PDC_CTRL0_TRIG_SELECT_MASK (0x3 << PDC_CTRL0_TRIG_SELECT_SHIFT)
#  define PDC_CTRL0_TRIG_SELECT(n) ((uint32_t)(n) << PDC_CTRL0_TRIG_SELECT_SHIFT)
#define PDC_CTRL0_TRIG_ID_SHIFT (2)     /* Bits 2-6 */
#define PDC_CTRL0_TRIG_ID_MASK (0x1f << PDC_CTRL0_TRIG_ID_SHIFT)
#  define PDC_CTRL0_TRIG_ID(n) ((uint32_t)(n) << PDC_CTRL0_TRIG_ID_SHIFT)
#define PDC_CTRL0_EN_XTAL (1 << 7)      /* Bit 7 */
#define PDC_CTRL0_EN_TMR (1 << 8)       /* Bit 8 */
#define PDC_CTRL0_EN_SNC (1 << 10)      /* Bit 10 */
#define PDC_CTRL0_MASTER_SHIFT (11)     /* Bits 11-12 */
#define PDC_CTRL0_MASTER_MASK (0x3 << PDC_CTRL0_MASTER_SHIFT)
#  define PDC_CTRL0_MASTER(n) ((uint32_t)(n) << PDC_CTRL0_MASTER_SHIFT)

/* CTRL1 register */

#define PDC_CTRL1_TRIG_SELECT_SHIFT (0) /* Bits 0-1 */
#define PDC_CTRL1_TRIG_SELECT_MASK (0x3 << PDC_CTRL1_TRIG_SELECT_SHIFT)
#  define PDC_CTRL1_TRIG_SELECT(n) ((uint32_t)(n) << PDC_CTRL1_TRIG_SELECT_SHIFT)
#define PDC_CTRL1_TRIG_ID_SHIFT (2)     /* Bits 2-6 */
#define PDC_CTRL1_TRIG_ID_MASK (0x1f << PDC_CTRL1_TRIG_ID_SHIFT)
#  define PDC_CTRL1_TRIG_ID(n) ((uint32_t)(n) << PDC_CTRL1_TRIG_ID_SHIFT)
#define PDC_CTRL1_EN_XTAL (1 << 7)      /* Bit 7 */
#define PDC_CTRL1_EN_TMR (1 << 8)       /* Bit 8 */
#define PDC_CTRL1_EN_SNC (1 << 10)      /* Bit 10 */
#define PDC_CTRL1_MASTER_SHIFT (11)     /* Bits 11-12 */
#define PDC_CTRL1_MASTER_MASK (0x3 << PDC_CTRL1_MASTER_SHIFT)
#  define PDC_CTRL1_MASTER(n) ((uint32_t)(n) << PDC_CTRL1_MASTER_SHIFT)

/* CTRL2 register */

#define PDC_CTRL2_TRIG_SELECT_SHIFT (0) /* Bits 0-1 */
#define PDC_CTRL2_TRIG_SELECT_MASK (0x3 << PDC_CTRL2_TRIG_SELECT_SHIFT)
#  define PDC_CTRL2_TRIG_SELECT(n) ((uint32_t)(n) << PDC_CTRL2_TRIG_SELECT_SHIFT)
#define PDC_CTRL2_TRIG_ID_SHIFT (2)     /* Bits 2-6 */
#define PDC_CTRL2_TRIG_ID_MASK (0x1f << PDC_CTRL2_TRIG_ID_SHIFT)
#  define PDC_CTRL2_TRIG_ID(n) ((uint32_t)(n) << PDC_CTRL2_TRIG_ID_SHIFT)
#define PDC_CTRL2_EN_XTAL (1 << 7)      /* Bit 7 */
#define PDC_CTRL2_EN_TMR (1 << 8)       /* Bit 8 */
#define PDC_CTRL2_EN_SNC (1 << 10)      /* Bit 10 */
#define PDC_CTRL2_MASTER_SHIFT (11)     /* Bits 11-12 */
#define PDC_CTRL2_MASTER_MASK (0x3 << PDC_CTRL2_MASTER_SHIFT)
#  define PDC_CTRL2_MASTER(n) ((uint32_t)(n) << PDC_CTRL2_MASTER_SHIFT)

/* CTRL3 register */

#define PDC_CTRL3_TRIG_SELECT_SHIFT (0) /* Bits 0-1 */
#define PDC_CTRL3_TRIG_SELECT_MASK (0x3 << PDC_CTRL3_TRIG_SELECT_SHIFT)
#  define PDC_CTRL3_TRIG_SELECT(n) ((uint32_t)(n) << PDC_CTRL3_TRIG_SELECT_SHIFT)
#define PDC_CTRL3_TRIG_ID_SHIFT (2)     /* Bits 2-6 */
#define PDC_CTRL3_TRIG_ID_MASK (0x1f << PDC_CTRL3_TRIG_ID_SHIFT)
#  define PDC_CTRL3_TRIG_ID(n) ((uint32_t)(n) << PDC_CTRL3_TRIG_ID_SHIFT)
#define PDC_CTRL3_EN_XTAL (1 << 7)      /* Bit 7 */
#define PDC_CTRL3_EN_TMR (1 << 8)       /* Bit 8 */
#define PDC_CTRL3_EN_SNC (1 << 10)      /* Bit 10 */
#define PDC_CTRL3_MASTER_SHIFT (11)     /* Bits 11-12 */
#define PDC_CTRL3_MASTER_MASK (0x3 << PDC_CTRL3_MASTER_SHIFT)
#  define PDC_CTRL3_MASTER(n) ((uint32_t)(n) << PDC_CTRL3_MASTER_SHIFT)

/* CTRL4 register */

#define PDC_CTRL4_TRIG_SELECT_SHIFT (0) /* Bits 0-1 */
#define PDC_CTRL4_TRIG_SELECT_MASK (0x3 << PDC_CTRL4_TRIG_SELECT_SHIFT)
#  define PDC_CTRL4_TRIG_SELECT(n) ((uint32_t)(n) << PDC_CTRL4_TRIG_SELECT_SHIFT)
#define PDC_CTRL4_TRIG_ID_SHIFT (2)     /* Bits 2-6 */
#define PDC_CTRL4_TRIG_ID_MASK (0x1f << PDC_CTRL4_TRIG_ID_SHIFT)
#  define PDC_CTRL4_TRIG_ID(n) ((uint32_t)(n) << PDC_CTRL4_TRIG_ID_SHIFT)
#define PDC_CTRL4_EN_XTAL (1 << 7)      /* Bit 7 */
#define PDC_CTRL4_EN_TMR (1 << 8)       /* Bit 8 */
#define PDC_CTRL4_EN_SNC (1 << 10)      /* Bit 10 */
#define PDC_CTRL4_MASTER_SHIFT (11)     /* Bits 11-12 */
#define PDC_CTRL4_MASTER_MASK (0x3 << PDC_CTRL4_MASTER_SHIFT)
#  define PDC_CTRL4_MASTER(n) ((uint32_t)(n) << PDC_CTRL4_MASTER_SHIFT)

/* CTRL5 register */

#define PDC_CTRL5_TRIG_SELECT_SHIFT (0) /* Bits 0-1 */
#define PDC_CTRL5_TRIG_SELECT_MASK (0x3 << PDC_CTRL5_TRIG_SELECT_SHIFT)
#  define PDC_CTRL5_TRIG_SELECT(n) ((uint32_t)(n) << PDC_CTRL5_TRIG_SELECT_SHIFT)
#define PDC_CTRL5_TRIG_ID_SHIFT (2)     /* Bits 2-6 */
#define PDC_CTRL5_TRIG_ID_MASK (0x1f << PDC_CTRL5_TRIG_ID_SHIFT)
#  define PDC_CTRL5_TRIG_ID(n) ((uint32_t)(n) << PDC_CTRL5_TRIG_ID_SHIFT)
#define PDC_CTRL5_EN_XTAL (1 << 7)      /* Bit 7 */
#define PDC_CTRL5_EN_TMR (1 << 8)       /* Bit 8 */
#define PDC_CTRL5_EN_SNC (1 << 10)      /* Bit 10 */
#define PDC_CTRL5_MASTER_SHIFT (11)     /* Bits 11-12 */
#define PDC_CTRL5_MASTER_MASK (0x3 << PDC_CTRL5_MASTER_SHIFT)
#  define PDC_CTRL5_MASTER(n) ((uint32_t)(n) << PDC_CTRL5_MASTER_SHIFT)

/* CTRL6 register */

#define PDC_CTRL6_TRIG_SELECT_SHIFT (0) /* Bits 0-1 */
#define PDC_CTRL6_TRIG_SELECT_MASK (0x3 << PDC_CTRL6_TRIG_SELECT_SHIFT)
#  define PDC_CTRL6_TRIG_SELECT(n) ((uint32_t)(n) << PDC_CTRL6_TRIG_SELECT_SHIFT)
#define PDC_CTRL6_TRIG_ID_SHIFT (2)     /* Bits 2-6 */
#define PDC_CTRL6_TRIG_ID_MASK (0x1f << PDC_CTRL6_TRIG_ID_SHIFT)
#  define PDC_CTRL6_TRIG_ID(n) ((uint32_t)(n) << PDC_CTRL6_TRIG_ID_SHIFT)
#define PDC_CTRL6_EN_XTAL (1 << 7)      /* Bit 7 */
#define PDC_CTRL6_EN_TMR (1 << 8)       /* Bit 8 */
#define PDC_CTRL6_EN_SNC (1 << 10)      /* Bit 10 */
#define PDC_CTRL6_MASTER_SHIFT (11)     /* Bits 11-12 */
#define PDC_CTRL6_MASTER_MASK (0x3 << PDC_CTRL6_MASTER_SHIFT)
#  define PDC_CTRL6_MASTER(n) ((uint32_t)(n) << PDC_CTRL6_MASTER_SHIFT)

/* CTRL7 register */

#define PDC_CTRL7_TRIG_SELECT_SHIFT (0) /* Bits 0-1 */
#define PDC_CTRL7_TRIG_SELECT_MASK (0x3 << PDC_CTRL7_TRIG_SELECT_SHIFT)
#  define PDC_CTRL7_TRIG_SELECT(n) ((uint32_t)(n) << PDC_CTRL7_TRIG_SELECT_SHIFT)
#define PDC_CTRL7_TRIG_ID_SHIFT (2)     /* Bits 2-6 */
#define PDC_CTRL7_TRIG_ID_MASK (0x1f << PDC_CTRL7_TRIG_ID_SHIFT)
#  define PDC_CTRL7_TRIG_ID(n) ((uint32_t)(n) << PDC_CTRL7_TRIG_ID_SHIFT)
#define PDC_CTRL7_EN_XTAL (1 << 7)      /* Bit 7 */
#define PDC_CTRL7_EN_TMR (1 << 8)       /* Bit 8 */
#define PDC_CTRL7_EN_SNC (1 << 10)      /* Bit 10 */
#define PDC_CTRL7_MASTER_SHIFT (11)     /* Bits 11-12 */
#define PDC_CTRL7_MASTER_MASK (0x3 << PDC_CTRL7_MASTER_SHIFT)
#  define PDC_CTRL7_MASTER(n) ((uint32_t)(n) << PDC_CTRL7_MASTER_SHIFT)

/* CTRL8 register */

#define PDC_CTRL8_TRIG_SELECT_SHIFT (0) /* Bits 0-1 */
#define PDC_CTRL8_TRIG_SELECT_MASK (0x3 << PDC_CTRL8_TRIG_SELECT_SHIFT)
#  define PDC_CTRL8_TRIG_SELECT(n) ((uint32_t)(n) << PDC_CTRL8_TRIG_SELECT_SHIFT)
#define PDC_CTRL8_TRIG_ID_SHIFT (2)     /* Bits 2-6 */
#define PDC_CTRL8_TRIG_ID_MASK (0x1f << PDC_CTRL8_TRIG_ID_SHIFT)
#  define PDC_CTRL8_TRIG_ID(n) ((uint32_t)(n) << PDC_CTRL8_TRIG_ID_SHIFT)
#define PDC_CTRL8_EN_XTAL (1 << 7)      /* Bit 7 */
#define PDC_CTRL8_EN_TMR (1 << 8)       /* Bit 8 */
#define PDC_CTRL8_EN_SNC (1 << 10)      /* Bit 10 */
#define PDC_CTRL8_MASTER_SHIFT (11)     /* Bits 11-12 */
#define PDC_CTRL8_MASTER_MASK (0x3 << PDC_CTRL8_MASTER_SHIFT)
#  define PDC_CTRL8_MASTER(n) ((uint32_t)(n) << PDC_CTRL8_MASTER_SHIFT)

/* CTRL9 register */

#define PDC_CTRL9_TRIG_SELECT_SHIFT (0) /* Bits 0-1 */
#define PDC_CTRL9_TRIG_SELECT_MASK (0x3 << PDC_CTRL9_TRIG_SELECT_SHIFT)
#  define PDC_CTRL9_TRIG_SELECT(n) ((uint32_t)(n) << PDC_CTRL9_TRIG_SELECT_SHIFT)
#define PDC_CTRL9_TRIG_ID_SHIFT (2)     /* Bits 2-6 */
#define PDC_CTRL9_TRIG_ID_MASK (0x1f << PDC_CTRL9_TRIG_ID_SHIFT)
#  define PDC_CTRL9_TRIG_ID(n) ((uint32_t)(n) << PDC_CTRL9_TRIG_ID_SHIFT)
#define PDC_CTRL9_EN_XTAL (1 << 7)      /* Bit 7 */
#define PDC_CTRL9_EN_TMR (1 << 8)       /* Bit 8 */
#define PDC_CTRL9_EN_SNC (1 << 10)      /* Bit 10 */
#define PDC_CTRL9_MASTER_SHIFT (11)     /* Bits 11-12 */
#define PDC_CTRL9_MASTER_MASK (0x3 << PDC_CTRL9_MASTER_SHIFT)
#  define PDC_CTRL9_MASTER(n) ((uint32_t)(n) << PDC_CTRL9_MASTER_SHIFT)

/* CTRL10 register */

#define PDC_CTRL10_TRIG_SELECT_SHIFT (0) /* Bits 0-1 */
#define PDC_CTRL10_TRIG_SELECT_MASK (0x3 << PDC_CTRL10_TRIG_SELECT_SHIFT)
#  define PDC_CTRL10_TRIG_SELECT(n) ((uint32_t)(n) << PDC_CTRL10_TRIG_SELECT_SHIFT)
#define PDC_CTRL10_TRIG_ID_SHIFT (2)     /* Bits 2-6 */
#define PDC_CTRL10_TRIG_ID_MASK (0x1f << PDC_CTRL10_TRIG_ID_SHIFT)
#  define PDC_CTRL10_TRIG_ID(n) ((uint32_t)(n) << PDC_CTRL10_TRIG_ID_SHIFT)
#define PDC_CTRL10_EN_XTAL (1 << 7)      /* Bit 7 */
#define PDC_CTRL10_EN_TMR (1 << 8)       /* Bit 8 */
#define PDC_CTRL10_EN_SNC (1 << 10)      /* Bit 10 */
#define PDC_CTRL10_MASTER_SHIFT (11)     /* Bits 11-12 */
#define PDC_CTRL10_MASTER_MASK (0x3 << PDC_CTRL10_MASTER_SHIFT)
#  define PDC_CTRL10_MASTER(n) ((uint32_t)(n) << PDC_CTRL10_MASTER_SHIFT)

/* CTRL11 register */

#define PDC_CTRL11_TRIG_SELECT_SHIFT (0) /* Bits 0-1 */
#define PDC_CTRL11_TRIG_SELECT_MASK (0x3 << PDC_CTRL11_TRIG_SELECT_SHIFT)
#  define PDC_CTRL11_TRIG_SELECT(n) ((uint32_t)(n) << PDC_CTRL11_TRIG_SELECT_SHIFT)
#define PDC_CTRL11_TRIG_ID_SHIFT (2)     /* Bits 2-6 */
#define PDC_CTRL11_TRIG_ID_MASK (0x1f << PDC_CTRL11_TRIG_ID_SHIFT)
#  define PDC_CTRL11_TRIG_ID(n) ((uint32_t)(n) << PDC_CTRL11_TRIG_ID_SHIFT)
#define PDC_CTRL11_EN_XTAL (1 << 7)      /* Bit 7 */
#define PDC_CTRL11_EN_TMR (1 << 8)       /* Bit 8 */
#define PDC_CTRL11_EN_SNC (1 << 10)      /* Bit 10 */
#define PDC_CTRL11_MASTER_SHIFT (11)     /* Bits 11-12 */
#define PDC_CTRL11_MASTER_MASK (0x3 << PDC_CTRL11_MASTER_SHIFT)
#  define PDC_CTRL11_MASTER(n) ((uint32_t)(n) << PDC_CTRL11_MASTER_SHIFT)

/* CTRL12 register */

#define PDC_CTRL12_TRIG_SELECT_SHIFT (0) /* Bits 0-1 */
#define PDC_CTRL12_TRIG_SELECT_MASK (0x3 << PDC_CTRL12_TRIG_SELECT_SHIFT)
#  define PDC_CTRL12_TRIG_SELECT(n) ((uint32_t)(n) << PDC_CTRL12_TRIG_SELECT_SHIFT)
#define PDC_CTRL12_TRIG_ID_SHIFT (2)     /* Bits 2-6 */
#define PDC_CTRL12_TRIG_ID_MASK (0x1f << PDC_CTRL12_TRIG_ID_SHIFT)
#  define PDC_CTRL12_TRIG_ID(n) ((uint32_t)(n) << PDC_CTRL12_TRIG_ID_SHIFT)
#define PDC_CTRL12_EN_XTAL (1 << 7)      /* Bit 7 */
#define PDC_CTRL12_EN_TMR (1 << 8)       /* Bit 8 */
#define PDC_CTRL12_EN_SNC (1 << 10)      /* Bit 10 */
#define PDC_CTRL12_MASTER_SHIFT (11)     /* Bits 11-12 */
#define PDC_CTRL12_MASTER_MASK (0x3 << PDC_CTRL12_MASTER_SHIFT)
#  define PDC_CTRL12_MASTER(n) ((uint32_t)(n) << PDC_CTRL12_MASTER_SHIFT)

/* CTRL13 register */

#define PDC_CTRL13_TRIG_SELECT_SHIFT (0) /* Bits 0-1 */
#define PDC_CTRL13_TRIG_SELECT_MASK (0x3 << PDC_CTRL13_TRIG_SELECT_SHIFT)
#  define PDC_CTRL13_TRIG_SELECT(n) ((uint32_t)(n) << PDC_CTRL13_TRIG_SELECT_SHIFT)
#define PDC_CTRL13_TRIG_ID_SHIFT (2)     /* Bits 2-6 */
#define PDC_CTRL13_TRIG_ID_MASK (0x1f << PDC_CTRL13_TRIG_ID_SHIFT)
#  define PDC_CTRL13_TRIG_ID(n) ((uint32_t)(n) << PDC_CTRL13_TRIG_ID_SHIFT)
#define PDC_CTRL13_EN_XTAL (1 << 7)      /* Bit 7 */
#define PDC_CTRL13_EN_TMR (1 << 8)       /* Bit 8 */
#define PDC_CTRL13_EN_SNC (1 << 10)      /* Bit 10 */
#define PDC_CTRL13_MASTER_SHIFT (11)     /* Bits 11-12 */
#define PDC_CTRL13_MASTER_MASK (0x3 << PDC_CTRL13_MASTER_SHIFT)
#  define PDC_CTRL13_MASTER(n) ((uint32_t)(n) << PDC_CTRL13_MASTER_SHIFT)

/* CTRL14 register */

#define PDC_CTRL14_TRIG_SELECT_SHIFT (0) /* Bits 0-1 */
#define PDC_CTRL14_TRIG_SELECT_MASK (0x3 << PDC_CTRL14_TRIG_SELECT_SHIFT)
#  define PDC_CTRL14_TRIG_SELECT(n) ((uint32_t)(n) << PDC_CTRL14_TRIG_SELECT_SHIFT)
#define PDC_CTRL14_TRIG_ID_SHIFT (2)     /* Bits 2-6 */
#define PDC_CTRL14_TRIG_ID_MASK (0x1f << PDC_CTRL14_TRIG_ID_SHIFT)
#  define PDC_CTRL14_TRIG_ID(n) ((uint32_t)(n) << PDC_CTRL14_TRIG_ID_SHIFT)
#define PDC_CTRL14_EN_XTAL (1 << 7)      /* Bit 7 */
#define PDC_CTRL14_EN_TMR (1 << 8)       /* Bit 8 */
#define PDC_CTRL14_EN_SNC (1 << 10)      /* Bit 10 */
#define PDC_CTRL14_MASTER_SHIFT (11)     /* Bits 11-12 */
#define PDC_CTRL14_MASTER_MASK (0x3 << PDC_CTRL14_MASTER_SHIFT)
#  define PDC_CTRL14_MASTER(n) ((uint32_t)(n) << PDC_CTRL14_MASTER_SHIFT)

/* CTRL15 register */

#define PDC_CTRL15_TRIG_SELECT_SHIFT (0) /* Bits 0-1 */
#define PDC_CTRL15_TRIG_SELECT_MASK (0x3 << PDC_CTRL15_TRIG_SELECT_SHIFT)
#  define PDC_CTRL15_TRIG_SELECT(n) ((uint32_t)(n) << PDC_CTRL15_TRIG_SELECT_SHIFT)
#define PDC_CTRL15_TRIG_ID_SHIFT (2)     /* Bits 2-6 */
#define PDC_CTRL15_TRIG_ID_MASK (0x1f << PDC_CTRL15_TRIG_ID_SHIFT)
#  define PDC_CTRL15_TRIG_ID(n) ((uint32_t)(n) << PDC_CTRL15_TRIG_ID_SHIFT)
#define PDC_CTRL15_EN_XTAL (1 << 7)      /* Bit 7 */
#define PDC_CTRL15_EN_TMR (1 << 8)       /* Bit 8 */
#define PDC_CTRL15_EN_SNC (1 << 10)      /* Bit 10 */
#define PDC_CTRL15_MASTER_SHIFT (11)     /* Bits 11-12 */
#define PDC_CTRL15_MASTER_MASK (0x3 << PDC_CTRL15_MASTER_SHIFT)
#  define PDC_CTRL15_MASTER(n) ((uint32_t)(n) << PDC_CTRL15_MASTER_SHIFT)

/* ACKNOWLEDGE register */

#define PDC_ACKNOWLEDGE_ACKNOWLEDGE_SHIFT (0) /* Bits 0-4 */
#define PDC_ACKNOWLEDGE_ACKNOWLEDGE_MASK (0x1f << PDC_ACKNOWLEDGE_ACKNOWLEDGE_SHIFT)
#  define PDC_ACKNOWLEDGE_ACKNOWLEDGE(n) ((uint32_t)(n) << PDC_ACKNOWLEDGE_ACKNOWLEDGE_SHIFT)

/* PENDING register */

#define PDC_PENDING_PENDING_SHIFT (0) /* Bits 0-15 */
#define PDC_PENDING_PENDING_MASK (0xffff << PDC_PENDING_PENDING_SHIFT)
#  define PDC_PENDING_PENDING(n) ((uint32_t)(n) << PDC_PENDING_PENDING_SHIFT)

/* PENDING_SNC register */

#define PDC_PENDING_SNC_PENDING_SHIFT (0) /* Bits 0-15 */
#define PDC_PENDING_SNC_PENDING_MASK (0xffff << PDC_PENDING_SNC_PENDING_SHIFT)
#  define PDC_PENDING_SNC_PENDING(n) ((uint32_t)(n) << PDC_PENDING_SNC_PENDING_SHIFT)

/* PENDING_CM33 register */

#define PDC_PENDING_CM33_PENDING_SHIFT (0) /* Bits 0-15 */
#define PDC_PENDING_CM33_PENDING_MASK (0xffff << PDC_PENDING_CM33_PENDING_SHIFT)
#  define PDC_PENDING_CM33_PENDING(n) ((uint32_t)(n) << PDC_PENDING_CM33_PENDING_SHIFT)

/* PENDING_CMAC register */

#define PDC_PENDING_CMAC_PENDING_SHIFT (0) /* Bits 0-15 */
#define PDC_PENDING_CMAC_PENDING_MASK (0xffff << PDC_PENDING_CMAC_PENDING_SHIFT)
#  define PDC_PENDING_CMAC_PENDING(n) ((uint32_t)(n) << PDC_PENDING_CMAC_PENDING_SHIFT)

/* SET_PENDING register */

#define PDC_SET_PENDING_SET_PENDING_SHIFT (0) /* Bits 0-4 */
#define PDC_SET_PENDING_SET_PENDING_MASK (0x1f << PDC_SET_PENDING_SET_PENDING_SHIFT)
#  define PDC_SET_PENDING_SET_PENDING(n) ((uint32_t)(n) << PDC_SET_PENDING_SET_PENDING_SHIFT)

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_PDC_H */
