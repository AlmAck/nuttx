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

#include "hardware/da1470x_cache.h"
#include "hardware/da1470x_crg_top.h"
#include "hardware/da1470x_gpreg.h"
#include "hardware/da1470x_sys_wdog.h"
#include "da1470x_lowputc.h"
#include "da1470x_start.h"
#include "da1470x_serial.h"
#include "da1470x_clockconfig.h"
#include "da1470x_pmu.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifdef CONFIG_DEBUG_FEATURES
#  define showprogress(c) arm_lowputc(c)
#else
#  define showprogress(c)
#endif

/****************************************************************************
 * Public Data
 ****************************************************************************/

/* Linker-provided bounds of the .ramfunc section (code that must run from
 * RAM, e.g. flash programming).
 */

extern uint8_t _sramfuncs[];
extern uint8_t _eramfuncs[];
extern uint8_t _framfuncs[];

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#ifdef CONFIG_ARMV8M_STACKCHECK
/* we need to get r10 set before we can allow instrumentation calls */

void __start(void) noinstrument_function;
#endif

/****************************************************************************
 * Name: da1470x_cache_flush
 *
 * Description:
 *   Drop whatever the flash cache holds from before this boot (the ROM
 *   does not flush it when it re-sizes the cacheable window).  Toggling
 *   the cache RAM mux flushes the cache.
 *
 ****************************************************************************/

static void da1470x_cache_flush(void)
{
  int i;

  if ((getreg32(DA1470X_CACHE_CTRL2) & CACHE_CTRL2_FLUSH_DISABLE) != 0)
    {
      return;
    }

  modifyreg32(DA1470X_CRG_TOP_SYS_CTRL, CRG_TOP_SYS_CTRL_CACHERAM_MUX, 0);
  modifyreg32(DA1470X_CRG_TOP_SYS_CTRL, 0, CRG_TOP_SYS_CTRL_CACHERAM_MUX);

  for (i = 0; i < 100000; i++)
    {
      if ((getreg32(DA1470X_CACHE_CTRL2) & CACHE_CTRL2_FLUSHED) != 0)
        {
          break;
        }
    }

  modifyreg32(DA1470X_CACHE_CTRL2, CACHE_CTRL2_FLUSHED, 0);
}

/****************************************************************************
 * Name: da1470x_wdog_freeze
 *
 * Description:
 *   The SYS watchdog runs out of reset and would reset the chip after
 *   about ten seconds.  Freeze it through both available paths until a
 *   watchdog driver takes over.
 *
 ****************************************************************************/

#ifdef CONFIG_DA1470X_WDOG_FREEZE
static void da1470x_wdog_freeze(void)
{
  putreg32(GPREG_SET_FREEZE_FRZ_SYS_WDOG, DA1470X_GPREG_SET_FREEZE);
  modifyreg32(DA1470X_SYS_WDOG_WATCHDOG_CTRL, 0,
              SYS_WDOG_WATCHDOG_CTRL_WDOG_FREEZE_EN);
}
#else
#  define da1470x_wdog_freeze()
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

  /* The boot ROM remaps the OQSPI flash to address zero, so the vector
   * table at the start of the image is reachable there.  Point the NVIC
   * at it explicitly.
   */

  putreg32(0x00000000, NVIC_VECTAB);

  __asm__ __volatile__ ("dsb" : : : "memory");
  __asm__ __volatile__ ("isb" : : : "memory");

  /* Enable the FPU before any code that the compiler may have given
   * floating-point registers to use.
   */

#ifdef CONFIG_ARCH_FPU
  arm_fpuconfig();
#endif

  da1470x_cache_flush();
  da1470x_pwr_init();
  da1470x_wdog_freeze();

  /* Clear .bss.  We'll do this inline (vs. calling memset) just to be
   * certain that there are no issues with the state of global variables.
   */

  for (dest = (uint32_t *)_sbss; dest < (uint32_t *)_ebss; )
    {
      *dest++ = 0;
    }

  /* Move the initialized data section from its temporary holding spot in
   * FLASH into the correct place in SRAM.  The correct place in SRAM is
   * given by _sdata and _edata.  The temporary location is in FLASH at the
   * end of all of the other read-only data (.text, .rodata) at _eronly.
   */

  for (src = (const uint32_t *)_eronly,
       dest = (uint32_t *)_sdata; dest < (uint32_t *)_edata;
      )
    {
      *dest++ = *src++;
    }

  /* Copy any necessary code sections from FLASH to RAM.  The correct
   * destination in SRAM is given by _sramfuncs and _eramfuncs.  The
   * temporary location is in flash after the data initialization code
   * at _framfuncs.  This must precede the clock configuration, which
   * reprograms the flash from RAM when the PLL is selected.
   */

  for (src = (const uint32_t *)_framfuncs,
       dest = (uint32_t *)_sramfuncs; dest < (uint32_t *)_eramfuncs;
      )
    {
      *dest++ = *src++;
    }

  da1470x_clockconfig();
  da1470x_lowsetup();
  showprogress('A');
  showprogress('B');
  showprogress('C');

#ifdef CONFIG_ARMV8M_STACKCHECK
  arm_stack_check_init();
#endif

#ifdef CONFIG_ARCH_PERF_EVENTS
  up_perf_init((void *)DA1470X_SYSCLK_FREQ);
#endif

  /* Perform early serial initialization */

#ifdef USE_EARLYSERIALINIT
  da1470x_earlyserialinit();
#endif
  showprogress('D');

  /* Initialize onboard resources */

  da1470x_board_initialize();
  showprogress('E');

  /* Then start NuttX */

  showprogress('\r');
  showprogress('\n');

  nx_start();

  /* Shouldn't get here */

  for (; ; );
}
