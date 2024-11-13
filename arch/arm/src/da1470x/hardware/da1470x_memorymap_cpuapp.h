/****************************************************************************
 * arch/arm/src/nrf53/hardware/nrf53_memorymap_cpuapp.h
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

#ifndef __ARCH_ARM_SRC_NRF53_HARDWARE_NRF53_MEMORYMAP_CPUAPP_H
#define __ARCH_ARM_SRC_NRF53_HARDWARE_NRF53_MEMORYMAP_CPUAPP_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Memory Map */

#define NRF53_FLASH_BASE        0x00000000 /* Flash memory Start Address */
#define NRF53_SRAM_BASE         0x20000000 /* SRAM Start Address */

#define NRF53_CORTEXM33_BASE    0xe0000000 /* Cortex-M33 Private Peripheral Bus */

/* APB Peripherals */

#define DA1470X_CACHE_BASE    0x100c0000 /* 0x100c0000-0x100c0053: 1kB CACHE registers */

#define DA1470X_LCDC_BASE     0x30030000 /* 0x30030000-0x300307ff: 2kB LCDC registers */
#define DA1470X_AES_HASH_BASE 0x30040000 /* 0x30040000-0x30040103: 1kB AES_HASH registers */
#define DA1470X_EMMC_BASE     0x30058000 /* 0x30058000-0x30058f6f: 3kB EMMC registers */
#define DA1470X_DCACHE_BASE   0x30100000 /* 0x30100000-0x30100027: 1kB DCACHE registers */
#define DA1470X_OQSPIF_BASE   0x36000000 /* 0x36000000-0x36000133: 1kB OQSPIF registers */

#define DA1470X_CRG_TOP_BASE  0x50000000 /* 0x50000000-0x500000fb: 1kB CRG_TOP registers */
#define DA1470X_CRG_VSYS_BASE 0x50000b00 /* 0x50000b00-0x50000b0f: 1kB CRG_VSYS registers */
#define DA1470X_DCDC_BASE     0x50000300 /* 0x50000300-0x50000303: 1kB DCDC registers */
#define DA1470X_DCDC_BOOST_BASE 0x50000500 /* 0x50000500-0x50000517: 1kB DCDC_BOOST registers */
#define DA1470X_I2C2_BASE     0x50020700 /* 0x50020700-0x500207a7: 1kB I2C2 registers */
#define DA1470X_I2C3_BASE     0x50020500 /* 0x50020500-0x500205a7: 1kB I2C3 registers */
#define DA1470X_I2C_BASE      0x50020600 /* 0x50020600-0x500206a7: 1kB I2C registers */
#define DA1470X_GPADC_BASE    0x50020800 /* 0x50020800-0x50020823: 1kB GPADC registers */
#define DA1470X_CRG_SNC_BASE  0x50020900 /* 0x50020900-0x5002090f: 1kB CRG_SNC registers */
#define DA1470X_I3C_BASE      0x50020c00 /* 0x50020c00-0x50020e9f: 1kB I3C registers */
#define DA1470X_CRG_AUD_BASE  0x50030000 /* 0x50030000-0x5003004f: 1kB CRG_AUD registers */
#define DA1470X_GPREG_BASE    0x50040100 /* 0x50040100-0x5004011b: 1kB GPREG registers */
#define DA1470X_CHG_DET_BASE  0x50040300 /* 0x50040300-0x50040327: 1kB CHG_DET registers */
#define DA1470X_CRG_SYS_BASE  0x50040400 /* 0x50040400-0x50040413: 1kB CRG_SYS registers */
#define DA1470X_MEMCTRL_BASE  0x50050000 /* 0x50050000-0x50050087: 1kB MEMCTRL registers */
#define DA1470X_GPIO_BASE     0x50050100 /* 0x50050100-0x50050283: 1kB GPIO registers */
#define DA1470X_CRG_XTAL_BASE 0x50050400 /* 0x50050400-0x5005049b: 1kB CRG_XTAL registers */
#define DA1470X_ANAMISC_BIF_BASE 0x50050600 /* 0x50050600-0x5005061f: 1kB ANAMISC_BIF registers */
#define DA1470X_CRG_CTRL_BASE 0x50060000 /* 0x50060000-0x50060007: 1kB CRG_CTRL registers */

#define DA1470X_DMA_BASE      0x51000400 /* 0x51000400-0x51000517: 1kB DMA registers */
#define DA1470X_CHARGER_BASE  0x51000600 /* 0x51000600-0x51000687: 1kB CHARGER registers */
#define DA1470X_CRG_GPU_BASE  0x51001000 /* 0x51001000-0x51001007: 1kB CRG_GPU registers */
#define DA1470X_GPU_REG_BASE  0x51001100 /* 0x51001100-0x51001103: 1kB GPU_REG registers */
#define DA1470X_GPU_CORE_BASE 0x51001200 /* 0x51001200-0x510012f7: 1kB GPU_CORE registers */
#define DA1470X_DSI2_BASE     0x51001300 /* 0x51001300-0x510014af: 1kB DSI2 registers */
#define DA1470X_DSIDPHY_REG_BASE 0x51001500 /* 0x51001500-0x51001537: 1kB DSIDPHY_REG registers */


#define DA1470X_AES_HASH_BASE               0x30040000UL
#define DA1470X_ANAMISC_BIF_BASE            0x50050600UL
#define DA1470X_CACHE_BASE                  0x100C0000UL
#define DA1470X_CHARGER_BASE                0x51000600UL
#define DA1470X_CHG_DET_BASE                0x50040300UL
#define DA1470X_CHIP_VERSION_BASE           0x50040000UL
#define DA1470X_CRG_AUD_BASE                0x50030000UL
#define DA1470X_CRG_CTRL_BASE               0x50060000UL
#define DA1470X_CRG_GPU_BASE                0x51001000UL
#define DA1470X_CRG_SNC_BASE                0x50020900UL
#define DA1470X_CRG_SYS_BASE                0x50040400UL
#define DA1470X_CRG_TOP_BASE                0x50000000UL
#define DA1470X_CRG_VSYS_BASE               0x50000B00UL
#define DA1470X_CRG_XTAL_BASE               0x50050400UL
#define DA1470X_DCACHE_BASE                 0x30100000UL
#define DA1470X_DCDC_BASE                   0x50000300UL
#define DA1470X_DCDC_BOOST_BASE             0x50000500UL
#define DA1470X_DMA_BASE                    0x51000400UL
#define DA1470X_EMMC_BASE                   0x30058000UL
#define DA1470X_GPADC_BASE                  0x50020800UL
#define DA1470X_GPIO_BASE                   0x50050100UL
#define DA1470X_GPREG_BASE                  0x50040100UL
#define DA1470X_GPU_CORE_BASE               0x51001200UL
#define DA1470X_GPU_REG_BASE                0x51001100UL
#define DA1470X_I2C_BASE                    0x50020600UL
#define DA1470X_I2C2_BASE                   0x50020700UL
#define DA1470X_I2C3_BASE                   0x50020500UL
#define DA1470X_I3C_BASE                    0x50020C00UL
#define DA1470X_LCDC_BASE                   0x30030000UL
#define DA1470X_MEMCTRL_BASE                0x50050000UL
#define DA1470X_OQSPIF_BASE                 0x36000000UL
#define DA1470X_OTPC_BASE                   0x30070000UL
#define DA1470X_PCM1_BASE                   0x50030300UL
#define DA1470X_PDC_BASE                    0x50000200UL
#define DA1470X_PWMLED_BASE                 0x50010600UL
#define DA1470X_QSPIC_BASE                  0x46000000UL
#define DA1470X_QSPIC2_BASE                 0x26000000UL
#define DA1470X_RTC_BASE                    0x50000800UL
#define DA1470X_SDADC_BASE                  0x50040500UL
#define DA1470X_SNC_BASE                    0x50021000UL
#define DA1470X_SPI_BASE                    0x50020300UL
#define DA1470X_SPI2_BASE                   0x50020400UL
#define DA1470X_SPI3_BASE                   0x51000200UL
#define DA1470X_SRC1_BASE                   0x50030100UL
#define DA1470X_SRC2_BASE                   0x50030200UL
#define DA1470X_SYS_WDOG_BASE               0x50000700UL
#define DA1470X_SYSBUS_BASE                 0x30020000UL
#define DA1470X_SYSBUS_ICM_BASE             0x50040600UL
#define DA1470X_TIMER_BASE                  0x50010000UL
#define DA1470X_TIMER2_BASE                 0x50010100UL
#define DA1470X_TIMER3_BASE                 0x50010200UL
#define DA1470X_TIMER4_BASE                 0x50010300UL
#define DA1470X_TIMER5_BASE                 0x50010400UL
#define DA1470X_TIMER6_BASE                 0x50010500UL
#define DA1470X_UART_BASE                   0x50020000UL
#define DA1470X_UART2_BASE                  0x50020100UL
#define DA1470X_UART3_BASE                  0x50020200UL
#define DA1470X_USB_BASE                    0x51000000UL
#define DA1470X_VAD_BASE                    0x50000C00UL
#define DA1470X_WAKEUP_BASE                 0x50000900UL


#endif /* __ARCH_ARM_SRC_NRF53_HARDWARE_NRF53_MEMORYMAP_CPUAPP_H */
