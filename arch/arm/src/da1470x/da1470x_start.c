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

#include <nuttx/config.h>

#include <stdint.h>
#include <assert.h>
#include <debug.h>

#include <nuttx/init.h>
#include <arch/board/board.h>

#include "arm_internal.h"
#include "nvic.h"

#include "da1470x_lowputc.h"
#include "da1470x_start.h"
#include "da1470x_gpio.h"
#include "da1470x_serial.h"
#include "da1470x_clockconfig.h"
#include "da1470x_pmu.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Name: showprogress
 *
 * Description:
 *   Print a character on the UART to show boot status.
 *
 ****************************************************************************/

#ifdef CONFIG_DEBUG_FEATURES
#  define showprogress(c) arm_lowputc(c)
#else
#  define showprogress(c)
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#ifdef CONFIG_ARMV8M_STACKCHECK
/* we need to get r10 set before we can allow instrumentation calls */

void __start(void) noinstrument_function;
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: __start
 *
 * Description:
 *   This is the reset entry point.
 *
 ****************************************************************************/

void __start(void)
{
  const uint32_t *src;
  uint32_t *dest;

#ifdef CONFIG_ARMV8M_STACKCHECK
  /* Set the stack limit before we attempt to call any functions */

  __asm__ volatile("sub r10, sp, %0" : :
                   "r"(CONFIG_IDLETHREAD_STACKSIZE - 64) :);
#endif

  /* Make sure that interrupts are disabled */

  __asm__ __volatile__ ("\tcpsid  i\n");

  /* Set the vector table base address. On the DA1470x the boot ROM
   * configures REMAP_ADR0 = OQSPI Flash, so the vector table provided by
   * the linker (at the start of the image binary) is accessible at
   * address 0x00000000 via remapping. The .vectors VMA at 0x0F000200
   * (and its 0x20000200 data-bus alias) is uninitialised RAM — nothing
   * copies the vectors there. Point NVIC_VECTAB at the remapped flash
   * location where the vectors actually live.
   */

  putreg32(0x00000000, NVIC_VECTAB);

  __asm__ __volatile__ ("dsb" : : : "memory");
  __asm__ __volatile__ ("isb" : : : "memory");

  /* Configure the UART so that we can get debug output as soon as possible */

  da1470x_pwr_init();

  /* Freeze both watchdog control paths. The DA1470x SYS WDT is enabled out
   * of reset and will reset the chip after ~10s during bring-up. Belt +
   * suspenders:
   *   - GPREG.SET_FREEZE_REG bit 3 = FRZ_SYS_WDOG (the "official" path).
   *   - SYS_WDOG.WATCHDOG_CTRL_REG bit 2 = WDOG_FREEZE_EN (direct enable).
   */

  putreg32(0x8, 0x50040100);  /* GPREG.SET_FREEZE_REG  <- FRZ_SYS_WDOG */
  putreg32(getreg32(0x50000704) | 0x4, 0x50000704);  /* SYS_WDOG CTRL.WDOG_FREEZE_EN */

  da1470_clockconfig();
  da1470x_lowsetup();
  showprogress('A');

  int my_heap_start = g_idle_topstack;
  int my_heap_size  = CONFIG_RAM_END - g_idle_topstack;

  _info("heap_start=%p heap_size=%lu, idle_top=%p\n",
    my_heap_start, my_heap_size, g_idle_topstack);

  /* Clear .bss.  We'll do this inline (vs. calling memset) just to be
   * certain that there are no issues with the state of global variables.
   */

  for (dest = (uint32_t *)_sbss; dest < (uint32_t *)_ebss; )
    {
      *dest++ = 0;
    }

  showprogress('B');

  /* Move the initialized data section from his temporary holding spot in
   * FLASH into the correct place in SRAM.  The correct place in SRAM is
   * give by _sdata and _edata.  The temporary location is in FLASH at the
   * end of all of the other read-only data (.text, .rodata) at _eronly.
   */

  for (src = (const uint32_t *)_eronly,
       dest = (uint32_t *)_sdata; dest < (uint32_t *)_edata;
      )
    {
      *dest++ = *src++;
    }

  showprogress('C');

#ifdef CONFIG_ARMV8M_STACKCHECK
  arm_stack_check_init();
#endif

#ifdef CONFIG_ARCH_FPU
  /* Initialize the FPU (if available) */

  arm_fpuconfig();
#endif

#ifdef CONFIG_ARCH_PERF_EVENTS
  up_perf_init((void *)BOARD_SYSTICK_CLOCK);
#endif

  /* Perform early serial initialization */

#ifdef USE_EARLYSERIALINIT
  da1470x_earlyserialinit();
#endif
  showprogress('D');

  /* Initialize onboard resources */

  da1470x_board_initialize();
  showprogress('F');

  /* Then start NuttX */

  showprogress('\r');
  showprogress('\n');

  nx_start();

  /* Shoulnd't get here */

  for (; ; );
}
