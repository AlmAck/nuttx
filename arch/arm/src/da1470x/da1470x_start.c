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

#include <assert.h>
#include <debug.h>
#include <stdint.h>

#include <arch/board/board.h>
#include <nuttx/init.h>

#include "arm_internal.h"
#include "nvic.h"

#include "da1470x_clockconfig.h"
#include "da1470x_lowputc.h"
#include "da1470x_pmu.h"
#include "da1470x_start.h"

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
#define showprogress(c) arm_lowputc(c)
#else
#define showprogress(c)
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

void __start(void) {
  const uint32_t *src;
  uint32_t *dest;

#ifdef CONFIG_ARMV8M_STACKCHECK
  /* Set the stack limit before we attempt to call any functions */

  __asm__ volatile("sub r10, sp, %0"
                   :
                   : "r"(CONFIG_IDLETHREAD_STACKSIZE - 64)
                   :);
#endif

  extern uint32_t _svectors_lma[];
  extern uint32_t _svectors[];
  extern uint32_t _evectors[];
  extern uint32_t _sretention_text_lma[];
  extern uint32_t _sretention_text[];
  extern uint32_t _eretention_text[];
  extern uint32_t _sram_data_lma[];
  extern uint32_t _sram_data[];
  extern uint32_t _eram_data[];
  extern uint32_t _sbss[];
  extern uint32_t _ebss[];

  /* Make sure that interrupts are disabled */

  __asm__ __volatile__("\tcpsid  i\n");

  /* Copy the vector table from FLASH to RAM0 */

  for (src = (const uint32_t *)_svectors_lma, dest = (uint32_t *)_svectors;
       dest < (uint32_t *)_evectors;) {
    *dest++ = *src++;
  }

  /* Set the vector table base address.  The default vector address is
   * 0x0000:0000 but if we are executing code that is positioned in SRAM or in
   * external FLASH, then we may need to reset the interrupt vector so that
   * it refers to the table in SRAM or in external FLASH.
   */

  putreg32((uintptr_t)_svectors, NVIC_VECTAB);

  /* Configure the UART so that we can get debug output as soon as possible */

  da1470x_pwr_init();
  da1470_clockconfig();
  da1470x_lowsetup();
  showprogress('A');

  /*  int my_heap_start = g_idle_topstack;
    int my_heap_size  = CONFIG_RAM_END - g_idle_topstack;

    _info("heap_start=%p heap_size=%lu, idle_top=%p\n",
      my_heap_start, my_heap_size, g_idle_topstack);
  */

  /* Clear .bss.  We'll do this inline (vs. calling memset) just to be
   * certain that there are no issues with the state of global variables.
   */

  for (dest = (uint32_t *)_sbss; dest < (uint32_t *)_ebss;) {
    *dest++ = 0;
  }

  /* Move the retention code section into RAMC. */

  for (src = (const uint32_t *)_sretention_text_lma,
      dest = (uint32_t *)_sretention_text;
       dest < (uint32_t *)_eretention_text;) {
    *dest++ = *src++;
  }

  showprogress('B');

  /* Move the consolidated data and retention data sections into RAMS. */

  for (src = (const uint32_t *)_sram_data_lma, dest = (uint32_t *)_sram_data;
       dest < (uint32_t *)_eram_data;) {
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

  for (;;)
    ;
}
