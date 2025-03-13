/****************************************************************************
 * arch/arm/src/da1470x/da1470x_start.c
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <assert.h>
#include <debug.h>
#include <nuttx/config.h>
#include <stdbool.h>
#include <stdint.h>

#include <arch/board/board.h>
#include <nuttx/init.h>

#include "arm_internal.h"
#include "da1470x_clockconfig.h"
#include "da1470x_gpio.h"
#include "da1470x_lowputc.h"
#include "da1470x_serial.h"
#include "nvic.h"

#include "da1470x_start.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register addresses */
#define SYS_CTRL_REG 0x50000024
#define SYS_CTRL_REG_DEBUGGER_ENABLE_MSK 0x80
#define SYS_CTRL_REG_REMAP_ADR0_MSK 0x7

#define WATCHDOG_CTRL_REG 0x50000704
#define WATCHDOG_CTRL_REG_WDOG_FREEZE_EN_MSK 0x4
#define SYS_WDOG_WATCHDOG_CTRL_REG_NMI_RST_Msk 0x1

#define RESET_FREEZE_REG 0x50040104
#define RESET_FREEZE_REG_FRZ_SYS_WDOG_MSK 0x08

#define SET_FREEZE_REG 0x50040100
#define SET_FREEZE_REG_FRZ_SYS_WDOG_MSK 0x08

#define RESET_STAT_REG 0x500000BC

#define MTB_POSITION_REG 0xE0043000
#define MTB_FLOW_REG 0xE0043008
#define MTB_MASTER_REG 0xE0043004
#define MTB_MASTER_REG_ENABLE_MSK                                              \
  0x80000008 /* Enable MTB, 4KB MTB buffer size */

#define CLK_RADIO_REG 0x50000054
#define CLK_RADIO_REG_CMAC_CLK_ENABLE_Msk 0x1

/* Memory regions */
#ifdef CONFIG_USE_SNC
#define MEMORY_SYSRAM_SIZE (0x00100000UL)
#else
#define MEMORY_SYSRAM_SIZE (0x00110000UL)
#endif

#define MEMORY_SYSRAM_REMAPPED_TO_ZERO_BASE (0x0UL)
#define MEMORY_SYSRAM_REMAPPED_TO_ZERO_END                                     \
  (MEMORY_SYSRAM_REMAPPED_TO_ZERO_BASE + MEMORY_SYSRAM_SIZE)
#define MEMORY_SYSRAM_SIZE_CODE_SIZE (0x40000)
#define MEMORY_SYSRAM_CODE_BASE (0x10010000UL)
#define MEMORY_SYSRAM_CODE_END                                                 \
  (MEMORY_SYSRAM_CODE_BASE + MEMORY_SYSRAM_SIZE_CODE_SIZE)

#ifdef CONFIG_USE_SNC
#define MEMORY_SYSRAM_BASE (0x20010000UL)
#else
#define MEMORY_SYSRAM_BASE (0x20000000UL)
#endif
#define MEMORY_SYSRAM_END (MEMORY_SYSRAM_BASE + MEMORY_SYSRAM_SIZE)

#if defined(CONFIG_DA1470X_USE_HW_QSPI2)
#define MEMORY_QSPIR_BASE (0x28000000UL)
#define MEMORY_QSPIR_END (MEMORY_QSPIR_BASE + 0x8000000UL)
#endif

/* Development/Production mode settings */
#ifndef CONFIG_DA1470X_DEVELOPMENT_MODE
#define CONFIG_DA1470X_DEVELOPMENT_MODE 1
#endif

/* MTB configuration */
#ifndef CONFIG_DA1470X_ENABLE_MTB
#define CONFIG_DA1470X_ENABLE_MTB 0
#endif

/* Code location */
#ifndef CONFIG_DA1470X_CODE_IN_RAM
#define CONFIG_DA1470X_CODE_IN_RAM 0
#endif

/* Skip magic check at startup */
#ifndef CONFIG_DA1470X_SKIP_MAGIC_CHECK
#define CONFIG_DA1470X_SKIP_MAGIC_CHECK 0
#endif

/* Debug symbols */
#ifdef CONFIG_DEBUG_FEATURES
#define showprogress(c) arm_lowputc(c)
#else
#define showprogress(c)
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_enable_debugger
 *
 * Description:
 *   Enable the hardware debugger
 *
 ****************************************************************************/

static inline void da1470x_enable_debugger(void) {
  uint32_t regval;

  /* Enable debugger */
  regval = getreg16(SYS_CTRL_REG);
  regval |= SYS_CTRL_REG_DEBUGGER_ENABLE_MSK;
  putreg16(regval, SYS_CTRL_REG);
}

/****************************************************************************
 * Name: da1470x_check_reset_status
 *
 * Description:
 *   Check if this is a cold reset or wakeup from deep sleep
 *
 * Return:
 *   true if cold reset, false if wakeup from deep sleep
 *
 ****************************************************************************/

static inline bool da1470x_check_reset_status(void) {
  uint32_t reset_stat;

  /* Check RESET_STAT_REG to determine if this is a wakeup from deep sleep */
  reset_stat = getreg32(RESET_STAT_REG);

  return (reset_stat != 0);
}

/****************************************************************************
 * Name: da1470x_configure_watchdog
 *
 * Description:
 *   Configure the watchdog based on development/production mode
 *
 ****************************************************************************/

static inline void da1470x_configure_watchdog(void) {
#if CONFIG_DA1470X_DEVELOPMENT_MODE
  /*
   * Set WATCHDOG_CTRL_REG[WDOG_FREEZE_EN] and reset WATCHDOG_CTRL_REG[NMI_RST]
   * so that watchdog can be frozen
   */
  putreg32(WATCHDOG_CTRL_REG_WDOG_FREEZE_EN_MSK, WATCHDOG_CTRL_REG);

  /* Freeze WDog at all times */
  putreg16(SET_FREEZE_REG_FRZ_SYS_WDOG_MSK, SET_FREEZE_REG);

  /* Enable debugger at all times */
  da1470x_enable_debugger();
#else
  /* Unfreeze WDog at all times */
  putreg16(RESET_FREEZE_REG_FRZ_SYS_WDOG_MSK, RESET_FREEZE_REG);

#if defined(CONFIG_DA1470X_ENABLE_DEBUGGER)
  /* Enable debugger */
  da1470x_enable_debugger();
#endif
#endif
}

/****************************************************************************
 * Name: da1470x_enable_mtb
 *
 * Description:
 *   Enable Micro Trace Buffer if configured
 *
 ****************************************************************************/

static inline void da1470x_enable_mtb(void) {
#if CONFIG_DA1470X_ENABLE_MTB
  putreg32(0, MTB_POSITION_REG);
  putreg32(0, MTB_FLOW_REG);
  putreg32(MTB_MASTER_REG_ENABLE_MSK, MTB_MASTER_REG);
#endif
}

/****************************************************************************
 * Name: da1470x_check_magic
 *
 * Description:
 *   Check for debug magic word and block to allow debugger attachment
 *
 ****************************************************************************/

static inline void da1470x_check_magic(void) {
#if !CONFIG_DA1470X_SKIP_MAGIC_CHECK
  /* Check for magic word at 0x0F001000 */
  volatile uint32_t *magic_addr = (volatile uint32_t *)0x0F001000;
  uint32_t magic_words[4];
  uint32_t i;

  /* Read the four words at the magic address */
  for (i = 0; i < 4; i++) {
    magic_words[i] = magic_addr[i];
  }

  /* Check if the magic pattern is present */
  if (magic_words[0] == 0xDEADBEEF && magic_words[1] == 0xDEADBEEF &&
      magic_words[2] == 0xDEADBEEF && magic_words[3] == 0xDEAD10CC) {
    uint16_t sys_ctrl_val;
    volatile uint32_t delay;

    /* Save current debugger setting */
    sys_ctrl_val = getreg16(SYS_CTRL_REG);

    /* Enable debugger */
    da1470x_enable_debugger();

    /* Block for ~500 msec (with a 32MHz clock) */
    for (delay = 0x150000; delay > 0; delay--) {
      asm volatile("nop");
    }

    /* Make sure this will happen only once by clearing the magic word */
    magic_addr[0] = 0;

    /* Restore debugger setting */
    putreg16(sys_ctrl_val, SYS_CTRL_REG);
  }
#endif
}

/****************************************************************************
 * Name: da1470x_configure_cache
 *
 * Description:
 *   Configure cache controller based on code location
 *
 ****************************************************************************/

static inline void da1470x_configure_cache(void) {
#if CONFIG_DA1470X_CODE_IN_RAM
  /* RAM projects should disable the cache controller */
  uint16_t regval = getreg16(SYS_CTRL_REG);
  regval &= ~(1 << 10); /* Clear bit 10 to disable cache */
  putreg16(regval, SYS_CTRL_REG);
#endif
}

/****************************************************************************
 * Name: da1470x_enable_ram
 *
 * Description:
 *   Enable additional RAM banks if configured
 *
 ****************************************************************************/

static inline void da1470x_enable_ram(void) {
#if defined(CONFIG_DA1470X_RAM9_SIZE) || defined(CONFIG_DA1470X_RAM10_SIZE)
  /* Enable RAM9, RAM10, RAM11 */
  uint32_t regval = getreg32(CLK_RADIO_REG);
  regval |= CLK_RADIO_REG_CMAC_CLK_ENABLE_Msk;
  putreg32(regval, CLK_RADIO_REG);
#endif
}

/****************************************************************************
 * Name: da1470x_systemInit
 *
 * Description:
 *   Perform system-specific initialization
 *
 ****************************************************************************/

void da1470x_systemInit(void) {
  /* Configure system clock */
  da1470_clockconfig();

  /* Configure the FPU if enabled */
#ifdef CONFIG_ARCH_FPU
  arm_fpuconfig();
#endif

  /* Configure stack checking if enabled */
#ifdef CONFIG_ARMV8M_STACKCHECK
  arm_stack_check_init();
#endif

  /* Other platform-specific initialization can be added here */
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_start
 *
 * Description:
 *   This is the reset entry point for the DA1470x
 *
 ****************************************************************************/

void up_start(void) {
  const uint32_t *src;
  uint32_t *dest;

  /* Disable interrupts */
  asm volatile("cpsid  i");

  /* Check if this is a cold reset or a wakeup from deep sleep */
  if (!da1470x_check_reset_status()) {
    /* This is a wakeup from deep sleep without state retention
     * Jump to wakeup handler (implementation would be elsewhere)
     */
    extern void wakeup_from_deepsleep(void);
    wakeup_from_deepsleep();

    /* Should not return */
    while (1) {
    }
  }

  /* Enable additional RAM if configured */
  da1470x_enable_ram();

  /* Enable Micro Trace Buffer if configured */
  da1470x_enable_mtb();

  /* Check for debug magic word */
  da1470x_check_magic();

  /* Configure cache controller */
  da1470x_configure_cache();

  /* Perform system-specific pre-initialization (from the original assembly) */
  extern void SystemInitPre(void);
  SystemInitPre();

  /* Initialize data sections */
  showprogress('A');

  /* Clear .bss section */
  for (dest = (uint32_t *)_sbss; dest < (uint32_t *)_ebss;) {
    *dest++ = 0;
  }

  showprogress('B');

  /* Copy the .data section from flash to SRAM */
  for (src = (const uint32_t *)_eronly, dest = (uint32_t *)_sdata;
       dest < (uint32_t *)_edata;) {
    *dest++ = *src++;
  }

  showprogress('C');

  /* Perform system initialization */
  da1470x_systemInit();

  /* Initialize the UART for early debug output */
  da1470x_lowsetup();

  showprogress('D');

  /* Perform board-specific initialization */
  da1470x_board_initialize();

  /* Perform early serial initialization */
#ifdef CONFIG_SERIAL_EARLYSERIALINIT
  da1470x_earlyserialinit();
#endif

  showprogress('E');

  /* Configure and freeze/unfreeze watchdog based on development mode */
  da1470x_configure_watchdog();

  showprogress('F');

  /* Start NuttX */
  showprogress('\r');
  showprogress('\n');

  nx_start();

  /* Should never get here */
  while (1) {
  }
}

/****************************************************************************
 * Name: arm_hardfault
 *
 * Description:
 *   This is Hard Fault exception handler for DA1470x
 *
 ****************************************************************************/

int arm_hardfault(int irq, void *context, void *arg) {
  /* Check if stack pointer is valid before proceeding */
  uint32_t sp;
  uint32_t sysram_remap;

  /* Get current stack pointer */
  asm volatile("mov %0, sp" : "=r"(sp));

  /* Check if address 0x0 is remapped to SYSRAM */
  sysram_remap = getreg32(SYS_CTRL_REG) & SYS_CTRL_REG_REMAP_ADR0_MSK;

  if (sysram_remap == 5) {
    /* Check if SP is in the valid SYSRAM remapped range */
    if (sp >= MEMORY_SYSRAM_REMAPPED_TO_ZERO_BASE &&
        sp < (MEMORY_SYSRAM_REMAPPED_TO_ZERO_END - 32)) {
      /* Valid SP - continue with regular fault handling */
      return -EFAULT;
    }
  }

  /* Check if SP is in SYSRAM CODE range */
  if (sp >= MEMORY_SYSRAM_CODE_BASE && sp < (MEMORY_SYSRAM_CODE_END - 32)) {
    /* Valid SP - continue with regular fault handling */
    return -EFAULT;
  }

  /* Check if SP is in regular SYSRAM range */
  if (sp >= MEMORY_SYSRAM_BASE && sp < (MEMORY_SYSRAM_END - 32)) {
    /* Valid SP - continue with regular fault handling */
    return -EFAULT;
  }

#if defined(CONFIG_DA1470X_USE_HW_QSPI2)
  /* Check if SP is in QSPI RAM range */
  if (sp >= MEMORY_QSPIR_BASE && sp < (MEMORY_QSPIR_END - 32)) {
    /* Valid SP - continue with regular fault handling */
    return -EFAULT;
  }
#endif

  /* Invalid stack pointer - configure watchdog based on development mode */
  da1470x_configure_watchdog();

  /* Wait for watchdog to reset or debugger to connect */
  while (1) {
  }

  /* Should never get here */
  return -EFAULT;
}

/****************************************************************************
 * Name: SystemClock functions
 *
 * Description:
 *   System clock related functions
 *
 ****************************************************************************/

/* System Core Clock Variable */
uint32_t SystemCoreClock = CONFIG_BOARD_CLOCK_FREQUENCY;

void SystemCoreClockUpdate(void) {
  /* Update SystemCoreClock based on actual clock configuration */
  SystemCoreClock = CONFIG_BOARD_CLOCK_FREQUENCY;
}

void SystemInit(void) {
  /* Update SystemCoreClock */
  SystemCoreClock = CONFIG_BOARD_CLOCK_FREQUENCY;

#if defined(__FPU_USED) && (__FPU_USED == 1U)
  /* Enable CP10 and CP11 full access (for FPU) */
  SCB->CPACR |= ((3U << 10U * 2U) | (3U << 11U * 2U));

  /* Enable hardware and lazy context save of FP state */
  FPU->FPCCR |= (FPU_FPCCR_ASPEN_Msk | FPU_FPCCR_LSPEN_Msk);
#endif

  /* Call DA1470x specific initialization */
  da1470x_systemInit();
}
