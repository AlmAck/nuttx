/* 
 * DA14706 NuttX Linker Script
 * Based on Dialog Semiconductor memory programming documentation
 */

/* Memory region definitions */
__SNC_BASE = 0x20000000;         /* RAM1 and RAM2 for SNC */
__SNC_SIZE = 64K;                /* 64KB for SNC */
__SHARED_BASE = 0x20110000;      /* RAM8 for shared memory between CPUs */
__SHARED_SIZE = 128K;            /* 128KB for shared memory */
__IVT_BASE = 0x0F000000 + 0x200; /* IVT in RAM0 */
__IVT_SIZE = 8K - 0x200;         /* 8KB - 0x200 for IVT */
__CMAC_BASE = 0x20150000;        /* RAM10 for potential CMAC code */
__CMAC_SIZE = 0;                 /* Configurable based on project needs */
__RAMC_BASE = 0x10010000;        /* RAM3 for retained code, remapped */
__RAMC_SIZE = 256K;              /* 256KB for retained code */
__RAMS_BASE = 0x20050000;        /* RAM4-RAM7 for retained data */
__RAMS_SIZE = 768K;              /* 768KB for retained data */
__STACK_SIZE = 0x1000;           /* 4 KB for idle thread & exception handling */

MEMORY
{
    IVT (rx)    : ORIGIN = __IVT_BASE, LENGTH = __IVT_SIZE
    SNC (rwx)   : ORIGIN = __SNC_BASE, LENGTH = __SNC_SIZE
    SHARED (rw) : ORIGIN = __SHARED_BASE, LENGTH = __SHARED_SIZE
    CMAC (rx)   : ORIGIN = __CMAC_BASE, LENGTH = __CMAC_SIZE
    RAMC (x)    : ORIGIN = __RAMC_BASE, LENGTH = __RAMC_SIZE
    RAMS (rwx)  : ORIGIN = __RAMS_BASE, LENGTH = __RAMS_SIZE
    FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 4M
}

OUTPUT_ARCH(arm)
ENTRY(__start)         /* Treat __start as the anchor for dead code stripping */
EXTERN(_vectors)       /* Force the vectors to be included in the output */

SECTIONS
{
    /* Interrupt Vector Table */
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } > IVT

    /* Code section in FLASH */
    .text :
    {
        . = ALIGN(4);
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
        . = ALIGN(4);
        __etext = ABSOLUTE(.);

    } > FLASH

    /* Retained code section */
    .retention_text : AT (0x0 + __etext)
    {
        . = ALIGN(4);
        __retention_text_start__ = .;
        *(text_retained)
        *libnosys.a:sbrk.o (.text*)
        *libgcc.a:_aeabi_uldivmod.o (.text*)
        *libgcc.a:_muldi3.o (.text*)
        *libgcc.a:_dvmd_tls.o (.text*)
        *libgcc.a:bpabi.o (.text*)
        *libgcc.a:_udivdi3.o (.text*)
        *libgcc.a:_clzdi2.o (.text*)
        *libgcc.a:_clzsi2.o (.text*)
        . = ALIGN(4);
        __retention_text_end__ = .;
    } > RAMC

    /* Ensure RAM data doesn't overlap with retained code */
    .RAM_UNUSED_THROUGH_CPU_S (NOLOAD) :
    {
        . += (__retention_text_end__ - __retention_text_start__);
    } > RAMS

    /* Retained initialized data */
    .retention_ram_init : AT (0x0 + __etext + (__retention_text_end__ - __retention_text_start__))
    {
        . = ALIGN(4);
        __retention_ram_init_start__ = .;
        *(privileged_data_init)
        *(.retention)
        *(vtable)
        *(retention_mem_init)
        *(retention_mem_const)
        *libg_nano.a:* (.data*)
        *libnosys.a:* (.data*)
        *libgcc.a:* (.data*)
        *crtbegin.o (.data*)
        KEEP(*(.jcr*))
        . = ALIGN(4);
        __retention_ram_init_end__ = .;
    } > RAMS

    /* Retained uninitialized data (zero-initialized) */
    .retention_ram_zi (NOLOAD) :
    {
        . = ALIGN(4);
        __retention_ram_zi_start__ = .;
        *(privileged_data_zi)
        *(retention_mem_zi)
        . = ALIGN(4);
        __retention_ram_zi_end__ = .;
    } > RAMS

    /* Shared memory section for CPU communication */
    .shared_section_retained_zi (NOLOAD) :
    {
        . = ALIGN(4);
        __shared_section_retained_zi_start__ = .;
        KEEP(*(.snc_shared_ram_area*))
        *(retention_mem_shared_zi)
        . = ALIGN(4);
        __shared_section_retained_zi_end__ = .;
    } > SHARED

    /* SNC shared memory section */
    .snc_shared (NOLOAD) :
    {
        . = ALIGN(4);
        __snc_shared_start__ = .;
        __snc_shared_space_start__ = .;
        KEEP(*(retention_mem_shared_zi))
        __snc_shared_space_end__ = .;
        __snc_exception_space_start__ = .;
        KEEP(*(nmi_info))
        KEEP(*(hard_fault_info))
        __snc_exception_space_end__ = .;
        . = ALIGN(4);
        __snc_shared_end__ = .;
    } > SHARED

    /* Data section (initialized data) */
    .data :
    {
        . = ALIGN(4);
        __data_start__ = .;
        EXCLUDE_FILE(*libg_nano.a:* *libnosys.a:* *libgcc.a:* *crtbegin.o) *(.data*)
        . = ALIGN(4);
        __data_end__ = .;
    } > RAMS AT > FLASH

    /* BSS section (zero-initialized data) */
    .bss :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        EXCLUDE_FILE(*libg_nano.a:* *libnosys.a:* *libgcc.a:* *crtbegin.o) *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
        __end__ = .;
    } > RAMS

    /* Stack section */
    .stack_section (NOLOAD) :
    {
        . = . + 1;
        . = ALIGN(8);
        __StackLimit = .;
        KEEP(*(.stack*))
        . = ALIGN(8);
        . = . + __STACK_SIZE;
        __StackTop = .;
        PROVIDE(__stack = __StackTop);
    } > RAMS

    /* Discard unused sections */
    /DISCARD/ :
    {
        *(.comment)
        *(.ARM.attributes)
    }

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

/* Provide symbols needed by the code */
PROVIDE(_stext = ADDR(.text));
PROVIDE(_etext = ADDR(.text) + SIZEOF(.text));
PROVIDE(_eronly = ADDR(.text) + SIZEOF(.text));
PROVIDE(_sdata = ADDR(.data));
PROVIDE(_edata = ADDR(.data) + SIZEOF(.data));
PROVIDE(_sbss = ADDR(.bss));
PROVIDE(_ebss = ADDR(.bss) + SIZEOF(.bss));