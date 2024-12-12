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

/* Memory Map */

#define DA1470X_FLASH_BASE                    0x00000000 /* Flash memory Start Address */
#define DA1470X_SRAM_BASE                     0x20000000 /* SRAM Start Address */

#define DA1470X_CORTEXM33_BASE                0xe0000000 /* Cortex-M33 Private Peripheral Bus */

/* APB Peripherals */

#define DA1470X_AES_HASH_BASE                 0x30040000 /* 0x30040000-0x30040103: 1KB AES_HASH registers */
#define DA1470X_ANAMISC_BIF_BASE              0x50050600 /* 0x50050600-0x5005061F: 1KB ANAMISC_BIF registers */
#define DA1470X_CACHE_BASE                    0x100C0000 /* 0x100C0000-0x100C0053: 1KB CACHE registers */
#define DA1470X_CHARGER_BASE                  0x51000600 /* 0x51000600-0x51000687: 1KB CHARGER registers */
#define DA1470X_CHG_DET_BASE                  0x50040300 /* 0x50040300-0x50040327: 1KB CHG_DET registers */
#define DA1470X_CHIP_VERSION_BASE             0x50040000 /* 0x50040000-0x500400FF: 1KB CHIP_VERSION registers */
#define DA1470X_CRG_AUD_BASE                  0x50030000 /* 0x50030000-0x5003004F: 1KB CRG_AUD registers */
#define DA1470X_CRG_CTRL_BASE                 0x50060000 /* 0x50060000-0x50060007: 1KB CRG_CTRL registers */
#define DA1470X_CRG_GPU_BASE                  0x51001000 /* 0x51001000-0x51001007: 1KB CRG_GPU registers */
#define DA1470X_CRG_SNC_BASE                  0x50020900 /* 0x50020900-0x5002090F: 1KB CRG_SNC registers */
#define DA1470X_CRG_SYS_BASE                  0x50040400 /* 0x50040400-0x50040413: 1KB CRG_SYS registers */
#define DA1470X_CRG_TOP_BASE                  0x50000000 /* 0x50000000-0x500000FB: 1KB CRG_TOP registers */
#define DA1470X_CRG_VSYS_BASE                 0x50000B00 /* 0x50000B00-0x50000B0F: 1KB CRG_VSYS registers */
#define DA1470X_CRG_XTAL_BASE                 0x50050400 /* 0x50050400-0x5005049B: 1KB CRG_XTAL registers */
#define DA1470X_DCACHE_BASE                   0x30100000 /* 0x30100000-0x30100027: 1KB DCACHE registers */
#define DA1470X_DCDC_BASE                     0x50000300 /* 0x50000300-0x50000303: 1KB DCDC registers */
#define DA1470X_DCDC_BOOST_BASE               0x50000500 /* 0x50000500-0x50000517: 1KB DCDC_BOOST registers */
#define DA1470X_DMA_BASE                      0x51000400 /* 0x51000400-0x51000517: 1KB DMA registers */
#define DA1470X_EMMC_BASE                     0x30058000 /* 0x30058000-0x30058F6F: 3KB EMMC registers */
#define DA1470X_GPADC_BASE                    0x50020800 /* 0x50020800-0x50020823: 1KB GPADC registers */
#define DA1470X_GPIO_BASE                     0x50050100 /* 0x50050100-0x50050283: 1KB GPIO registers */
#define DA1470X_GPREG_BASE                    0x50040100 /* 0x50040100-0x5004011B: 1KB GPREG registers */
#define DA1470X_GPU_CORE_BASE                 0x51001200 /* 0x51001200-0x510012F7: 1KB GPU_CORE registers */
#define DA1470X_GPU_REG_BASE                  0x51001100 /* 0x51001100-0x51001103: 1KB GPU_REG registers */
#define DA1470X_I2C_BASE                      0x50020600 /* 0x50020600-0x500206A7: 1KB I2C registers */
#define DA1470X_I2C2_BASE                     0x50020700 /* 0x50020700-0x500207A7: 1KB I2C2 registers */
#define DA1470X_I2C3_BASE                     0x50020500 /* 0x50020500-0x500205A7: 1KB I2C3 registers */
#define DA1470X_I3C_BASE                      0x50020C00 /* 0x50020C00-0x50020E9F: 1KB I3C registers */
#define DA1470X_LCDC_BASE                     0x30030000 /* 0x30030000-0x300307FF: 2KB LCDC registers */
#define DA1470X_MEMCTRL_BASE                  0x50050000 /* 0x50050000-0x50050087: 1KB MEMCTRL registers */
#define DA1470X_OQSPIF_BASE                   0x36000000 /* 0x36000000-0x36000133: 1KB OQSPIF registers */
#define DA1470X_OTPC_BASE                     0x30070000 /* 0x30070000-0x30070017: 1KB OTPC registers */
#define DA1470X_PCM1_BASE                     0x50030300 /* 0x50030300-0x50030313: 1KB PCM1 registers */
#define DA1470X_PDC_BASE                      0x50000200 /* 0x50000200-0x50000297: 1KB PDC registers */
#define DA1470X_PWMLED_BASE                   0x50010600 /* 0x50010600-0x50010623: 1KB PWMLED registers */
#define DA1470X_QSPIC_BASE                    0x46000000 /* 0x46000000-0x46000047: 1KB QSPIC registers */
#define DA1470X_QSPIC2_BASE                   0x26000000 /* 0x26000000-0x26000047: 1KB QSPIC2 registers */
#define DA1470X_RTC_BASE                      0x50000800 /* 0x50000800-0x50000897: 1KB RTC registers */
#define DA1470X_SDADC_BASE                    0x50040500 /* 0x50040500-0x50040517: 1KB SDADC registers */
#define DA1470X_SNC_BASE                      0x50021000 /* 0x50021000-0x50021007: 1KB SNC registers */
#define DA1470X_SPI_BASE                      0x50020300 /* 0x50020300-0x5002032F: 1KB SPI registers */
#define DA1470X_SPI2_BASE                     0x50020400 /* 0x50020400-0x5002042F: 1KB SPI2 registers */
#define DA1470X_SPI3_BASE                     0x51000200 /* 0x51000200-0x5100022F: 1KB SPI3 registers */
#define DA1470X_SRC1_BASE                     0x50030100 /* 0x50030100-0x50030137: 1KB SRC1 registers */
#define DA1470X_SRC2_BASE                     0x50030200 /* 0x50030200-0x50030237: 1KB SRC2 registers */
#define DA1470X_SYS_WDOG_BASE                 0x50000700 /* 0x50000700-0x50000707: 1KB SYS_WDOG registers */
#define DA1470X_SYSBUS_BASE                   0x30020000 /* 0x30020000-0x30020093: 1KB SYSBUS registers */
#define DA1470X_SYSBUS_ICM_BASE               0x50040600 /* 0x50040600-0x50040613: 1KB SYSBUS_ICM registers */
#define DA1470X_TIMER_BASE                    0x50010000 /* 0x50010000-0x5001005B: 1KB TIMER registers */
#define DA1470X_TIMER2_BASE                   0x50010100 /* 0x50010100-0x5001015B: 1KB TIMER2 registers */
#define DA1470X_TIMER3_BASE                   0x50010200 /* 0x50010200-0x5001025B: 1KB TIMER3 registers */
#define DA1470X_TIMER4_BASE                   0x50010300 /* 0x50010300-0x5001035B: 1KB TIMER4 registers */
#define DA1470X_TIMER5_BASE                   0x50010400 /* 0x50010400-0x5001045B: 1KB TIMER5 registers */
#define DA1470X_TIMER6_BASE                   0x50010500 /* 0x50010500-0x5001055B: 1KB TIMER6 registers */
#define DA1470X_UART0_BASE                    0x50020000 /* 0x50020000-0x500200FF: 1KB UART registers */
#define DA1470X_UART1_BASE                    0x50020100 /* 0x50020100-0x500201FF: 1KB UART2 registers */
#define DA1470X_UART2_BASE                    0x50020200 /* 0x50020200-0x500202FF: 1KB UART3 registers */
#define DA1470X_USB_BASE                      0x51000000 /* 0x51000000-0x510001AF: 1KB USB registers */
#define DA1470X_VAD_BASE                      0x50000C00 /* 0x50000C00-0x50000C23: 1KB VAD registers */
#define DA1470X_WAKEUP_BASE                   0x50000900 /* 0x50000900-0x5000094F: 1KB WAKEUP registers */

// NOTE: change UART to UART0 and shifted the others by 1 to make coding easier

#endif /* __ARCH_ARM_SRC_DA1470X_HARDWARE_DA1470X_MEMORYMAP_CPUAPP_H */
