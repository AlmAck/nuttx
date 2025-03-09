/****************************************************************************
* boards/arm/da1470x/da14706-00hzdevkt-p/scripts/flash_net.ld
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

# ifdef CONFIG_USE_SNC
     /* reserve first 64K for SNC */
#    define RAMC_ORG                 0x10010000
#    define RAMS_ORG                 0x20010000
#    define SNC_MEM_LEN              0
# else
#    define RAMC_ORG                 0x10000000
#    define RAMS_ORG                 0x20000000
#    define SNC_MEM_LEN              64K
# endif

# define RAMS_LEN                         (1024K + SNC_MEM_LEN)

/* Define memory regions */
MEMORY
{
    /* Interrupt Vector Table - Use NVM start */
    IVT (rx) : ORIGIN = 0x0F000200, LENGTH = 8K - 0x200 /* bytes */
    //IVT (rx) : ORIGIN = 0x0, LENGTH = 128K

    /* Shared Memory between Main and M0 Cores */
    SHARED (rw): ORIGIN = 0x20110000, LENGTH = 0x20000 /* 128K */

    RAMC : ORIGIN = RAMC_ORG, LENGTH = 256K

    /* Main Core RAM */
    RAMS (rwx) : ORIGIN = RAMS_ORG, LENGTH = RAMS_LEN /* Full RAM range */

    /* Non-Volatile Memory for Code */
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 0x4000000 /* NVM range */

}

OUTPUT_ARCH(arm)
ENTRY(__start)         /* Treat __start as the anchor for dead code stripping */
EXTERN(_vectors)       /* Force the vectors to be included in the output */

/* Define sections */
SECTIONS
{
    .text :
    {
        _stext = ABSOLUTE(.);
        *(.vectors)
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
    } > FLASH

    .init_section : 
    {
        _sinit = ABSOLUTE(.);
        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP(*(.init_array EXCLUDE_FILE(*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o) .ctors))
        _einit = ABSOLUTE(.);
    } > FLASH

    .ARM.extab :
    {
        *(.ARM.extab*)
    } > FLASH

    __exidx_start = ABSOLUTE(.);
    .ARM.exidx :
    {
        *(.ARM.exidx*)
    } > FLASH
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
    } > RAMS AT > FLASH

    .stack_section (NOLOAD) : {
        *(.stack*)
    } > RAMS

    .bss :
    {
        _sbss = ABSOLUTE(.);
        *(.bss .bss.*)
        *(.gnu.linkonce.b.*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = ABSOLUTE(.);
    } > RAMS

    /*
    .shmem :
    {
        . = ALIGN(4);
        *(.shmem);
        KEEP(*(.shmem))
    } > SHARED
    */

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
