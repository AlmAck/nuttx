/****************************************************************************
 * arch/arm/src/da1470x/hardware/da1470x_memorymap_cpuapp.h
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

#ifndef __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_MEMORYMAP_CPUAPP_H
#define __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_MEMORYMAP_CPUAPP_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Memory regions (CPUAPP / Cortex-M33 view) ********************************/

#define DA1470X_REMAP_BASE 0x00000000        /* Remapped boot device (OQSPI XiP) */
#define DA1470X_REMAP_SIZE 0x00800000        /* 8 MiB remap window */
#define DA1470X_IVT_BASE 0x0f000000          /* RAM0 interrupt vector table alias */
#define DA1470X_ROM_BASE 0x0f020000          /* Boot ROM (64 KiB) */
#define DA1470X_SYSRAM_CODE_BASE 0x10000000  /* SYSRAM, code bus view (320 KiB) */
#define DA1470X_CACHERAM_BASE 0x10060000     /* Cache RAM */
#define DA1470X_OTPC_MEM_BASE 0x10080000     /* OTP memory array (64 KiB) */
#define DA1470X_OQSPIF_MEM_BASE 0x18000000   /* OQSPI flash XiP window (128 MiB) */
#define DA1470X_SRAM_BASE 0x20000000         /* SYSRAM, data bus view */
#define DA1470X_SRAM_SIZE 0x00180000         /* 1536 KiB total SYSRAM */
#define DA1470X_SRAM_END 0x20180000          /* End of SYSRAM */
#define DA1470X_SRAM8_BASE 0x20110000        /* RAM8: 128 KiB shared area */
#define DA1470X_SRAM9_BASE 0x20130000        /* RAM9: 128 KiB (CMAC data) */
#define DA1470X_SRAM10_BASE 0x20150000       /* RAM10: 192 KiB (CMAC code) */
#define DA1470X_QSPIC2_MEM_BASE 0x28000000   /* QSPIC2 PSRAM window (128 MiB) */
#define DA1470X_OQSPIF_MEM_S_BASE 0x38000000 /* OQSPI XiP window, secure alias */
#define DA1470X_CMAC_BASE 0x40000000         /* CMAC register window */
#define DA1470X_QSPIC_MEM_BASE 0x48000000    /* QSPIC flash window (128 MiB) */
#define DA1470X_CORTEXM33_BASE 0xe0000000    /* Cortex-M33 private peripheral bus */
#define DA1470X_FLASH_BASE 0x00000000        /* Alias: code runs from remapped flash */

/* Peripheral register base addresses ***************************************/

#define DA1470X_AES_HASH_BASE 0x30040000     /* CMSIS: AES_HASH */
#define DA1470X_ANAMISC_BIF_BASE 0x50050600  /* CMSIS: ANAMISC_BIF */
#define DA1470X_CACHE_BASE 0x100c0000        /* CMSIS: CACHE */
#define DA1470X_CHARGER_BASE 0x51000600      /* CMSIS: CHARGER */
#define DA1470X_CHG_DET_BASE 0x50040300      /* CMSIS: CHG_DET */
#define DA1470X_CHIP_VERSION_BASE 0x50040000 /* CMSIS: CHIP_VERSION */
#define DA1470X_CRG_AUD_BASE 0x50030000      /* CMSIS: CRG_AUD */
#define DA1470X_CRG_CTRL_BASE 0x50060000     /* CMSIS: CRG_CTRL */
#define DA1470X_CRG_GPU_BASE 0x51001000      /* CMSIS: CRG_GPU */
#define DA1470X_CRG_SNC_BASE 0x50020900      /* CMSIS: CRG_SNC */
#define DA1470X_CRG_SYS_BASE 0x50040400      /* CMSIS: CRG_SYS */
#define DA1470X_CRG_TOP_BASE 0x50000000      /* CMSIS: CRG_TOP */
#define DA1470X_CRG_VSYS_BASE 0x50000b00     /* CMSIS: CRG_VSYS */
#define DA1470X_CRG_XTAL_BASE 0x50050400     /* CMSIS: CRG_XTAL */
#define DA1470X_DCACHE_BASE 0x30100000       /* CMSIS: DCACHE */
#define DA1470X_DCDC_BASE 0x50000300         /* CMSIS: DCDC */
#define DA1470X_DCDC_BOOST_BASE 0x50000500   /* CMSIS: DCDC_BOOST */
#define DA1470X_DMA_BASE 0x51000400          /* CMSIS: DMA */
#define DA1470X_EMMC_BASE 0x30058000         /* CMSIS: EMMC */
#define DA1470X_GPADC_BASE 0x50020800        /* CMSIS: GPADC */
#define DA1470X_GPIO_BASE 0x50050100         /* CMSIS: GPIO */
#define DA1470X_GPREG_BASE 0x50040100        /* CMSIS: GPREG */
#define DA1470X_GPU_CORE_BASE 0x51001200     /* CMSIS: GPU_CORE */
#define DA1470X_GPU_REG_BASE 0x51001100      /* CMSIS: GPU_REG */
#define DA1470X_I2C0_BASE 0x50020600         /* CMSIS: I2C */
#define DA1470X_I2C1_BASE 0x50020700         /* CMSIS: I2C2 */
#define DA1470X_I2C2_BASE 0x50020500         /* CMSIS: I2C3 */
#define DA1470X_I3C_BASE 0x50020c00          /* CMSIS: I3C */
#define DA1470X_LCDC_BASE 0x30030000         /* CMSIS: LCDC */
#define DA1470X_MEMCTRL_BASE 0x50050000      /* CMSIS: MEMCTRL */
#define DA1470X_OQSPIF_BASE 0x36000000       /* CMSIS: OQSPIF */
#define DA1470X_OTPC_BASE 0x30070000         /* CMSIS: OTPC */
#define DA1470X_PCM1_BASE 0x50030300         /* CMSIS: PCM1 */
#define DA1470X_PDC_BASE 0x50000200          /* CMSIS: PDC */
#define DA1470X_PWMLED_BASE 0x50010600       /* CMSIS: PWMLED */
#define DA1470X_QSPIC_BASE 0x46000000        /* CMSIS: QSPIC */
#define DA1470X_QSPIC2_BASE 0x26000000       /* CMSIS: QSPIC2 */
#define DA1470X_RTC_BASE 0x50000800          /* CMSIS: RTC */
#define DA1470X_SDADC_BASE 0x50040500        /* CMSIS: SDADC */
#define DA1470X_SNC_BASE 0x50021000          /* CMSIS: SNC */
#define DA1470X_SPI0_BASE 0x50020300         /* CMSIS: SPI */
#define DA1470X_SPI1_BASE 0x50020400         /* CMSIS: SPI2 */
#define DA1470X_SPI2_BASE 0x51000200         /* CMSIS: SPI3 */
#define DA1470X_SRC1_BASE 0x50030100         /* CMSIS: SRC1 */
#define DA1470X_SRC2_BASE 0x50030200         /* CMSIS: SRC2 */
#define DA1470X_SYSBUS_BASE 0x30020000       /* CMSIS: SYSBUS */
#define DA1470X_SYSBUS_ICM_BASE 0x50040600   /* CMSIS: SYSBUS_ICM */
#define DA1470X_SYS_WDOG_BASE 0x50000700     /* CMSIS: SYS_WDOG */
#define DA1470X_TIMER_BASE 0x50010000        /* CMSIS: TIMER */
#define DA1470X_TIMER2_BASE 0x50010100       /* CMSIS: TIMER2 */
#define DA1470X_TIMER3_BASE 0x50010200       /* CMSIS: TIMER3 */
#define DA1470X_TIMER4_BASE 0x50010300       /* CMSIS: TIMER4 */
#define DA1470X_TIMER5_BASE 0x50010400       /* CMSIS: TIMER5 */
#define DA1470X_TIMER6_BASE 0x50010500       /* CMSIS: TIMER6 */
#define DA1470X_UART0_BASE 0x50020000        /* CMSIS: UART */
#define DA1470X_UART1_BASE 0x50020100        /* CMSIS: UART2 */
#define DA1470X_UART2_BASE 0x50020200        /* CMSIS: UART3 */
#define DA1470X_USB_BASE 0x51000000          /* CMSIS: USB */
#define DA1470X_VAD_BASE 0x50000c00          /* CMSIS: VAD */
#define DA1470X_WAKEUP_BASE 0x50000900       /* CMSIS: WAKEUP */

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_MEMORYMAP_CPUAPP_H */
