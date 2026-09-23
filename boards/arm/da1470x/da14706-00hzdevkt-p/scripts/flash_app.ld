/****************************************************************************
 * boards/arm/da1470x/da14706-00hzdevkt-p/scripts/flash_app.ld
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

/* The DA14706 executes in place from the OQSPI flash, which the boot ROM
 * remaps to address 0.  The 1.5 MiB SYSRAM is split as follows:
 *
 *   0x20000000  64 KiB   RAM0-2   sensor node controller, not used
 *   0x20010000   1 MiB   RAM3-7   NuttX .data/.bss/heap
 *   0x20110000 128 KiB   RAM8     not used
 *   0x20130000 128 KiB   RAM9     not used
 *   0x20150000 192 KiB   RAM10    BLE controller (CMAC), not used
 */

/* The part has 8 MiB of flash, and the image may not have all of it.
 * CONFIG_DA1470X_OQSPI_MTD_OFFSET carves a window out of the top for the
 * filesystem, at 4 MiB by default, so the region below stops there.
 *
 * Claiming the whole device here instead, which is what this did, makes an
 * image that outgrows its half link quietly and then be eaten by the first
 * thing that writes to the filesystem.  Stopping at the boundary turns
 * that into a link error, which is where it belongs.
 *
 * There is no way to read the configuration from here -- this script is
 * not preprocessed -- so the two numbers are kept in step by hand.  Move
 * CONFIG_DA1470X_OQSPI_MTD_OFFSET and this LENGTH has to follow.
 */

MEMORY
{
  flash (rx)  : ORIGIN = 0x00000000, LENGTH = 4M
  sram  (rwx) : ORIGIN = 0x20010000, LENGTH = 1M
}

OUTPUT_ARCH(arm)
EXTERN(_vectors)
ENTRY(_stext)

SECTIONS
{
  /* The vector table must be the first thing in the image so that the
   * NVIC finds it at address 0 of the remapped flash.  The rest of the
   * code starts after the 512-byte header slot used by the image tools.
   */

  .vectors :
  {
    _svectors = ABSOLUTE(.);
    KEEP(*(.vectors))
    . = ALIGN(0x200);
    _evectors = ABSOLUTE(.);
  } > flash

  .text :
  {
    _stext = ABSOLUTE(.);
    *(.text .text.*)
    *(.fixup)
    *(.gnu.warning)
    *(.rodata .rodata.*)
    *(.gnu.linkonce.t.*)
    *(.glue_7)
    *(.glue_7t)
    *(.got)
    *(.gcc_except_table)
    *(.gnu.linkonce.r.*)
    _etext = ABSOLUTE(.);
  } > flash

  .init_section :
  {
    _sinit = ABSOLUTE(.);
    KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
    KEEP(*(.init_array EXCLUDE_FILE(*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o) .ctors))
    _einit = ABSOLUTE(.);
  } > flash

  .ARM.extab :
  {
    *(.ARM.extab*)
  } > flash

  __exidx_start = ABSOLUTE(.);
  .ARM.exidx :
  {
    *(.ARM.exidx*)
  } > flash
  __exidx_end = ABSOLUTE(.);

  _eronly = ABSOLUTE(.);

  .data :
  {
    _sdata = ABSOLUTE(.);
    *(.data .data.*)
    *(.gnu.linkonce.d.*)
    CONSTRUCTORS
    . = ALIGN(4);
    _edata = ABSOLUTE(.);
  } > sram AT > flash

  /* Code that must run from RAM (flash programming while executing in
   * place).  Copied by __start from _framfuncs.
   */

  .ramfunc ALIGN(4) :
  {
    _sramfuncs = ABSOLUTE(.);
    *(.ramfunc .ramfunc.*)
    . = ALIGN(4);
    _eramfuncs = ABSOLUTE(.);
  } > sram AT > flash

  _framfuncs = LOADADDR(.ramfunc);

  .bss :
  {
    _sbss = ABSOLUTE(.);
    *(.bss .bss.*)
    *(.gnu.linkonce.b.*)
    *(COMMON)
    . = ALIGN(8);
    _ebss = ABSOLUTE(.);
  } > sram

  /* Stabs debugging sections. */

  .stab 0 : { *(.stab) }
  .stabstr 0 : { *(.stabstr) }
  .stab.excl 0 : { *(.stab.excl) }
  .stab.exclstr 0 : { *(.stab.exclstr) }
  .stab.index 0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment 0 : { *(.comment) }
  .debug_abbrev 0 : { *(.debug_abbrev) }
  .debug_info 0 : { *(.debug_info) }
  .debug_line 0 : { *(.debug_line) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  .debug_aranges 0 : { *(.debug_aranges) }
}
