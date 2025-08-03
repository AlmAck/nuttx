/* 
 * DA14706 NuttX Linker Script
 * Based on Dialog Semiconductor memory programming documentation
 */

/*
 * Interrupt vector remmaping overhead.
 * This is required only for FLASH builds, where the IVT is
 * copied at RAM0 to improve performance.
 * For RAM builds this overhead is not needed, but it still exists so that
 * NMI and HardFault info blocks are always at the same address.
 */
IVT_OVERHEAD = 0x200;

/* Memory region definitions */
__SNC_BASE = 0x20000000;         /* RAM1 and RAM2 for SNC */
__SNC_SIZE = 64K;                /* 64KB for SNC */
__SHARED_BASE = 0x20110000;      /* RAM8 for shared memory between CPUs */
__SHARED_SIZE = 128K;            /* 128KB for shared memory */
__IVT_BASE = 0x0F000000  + IVT_OVERHEAD;         /* IVT in RAM0 */
__IVT_SIZE = 8K - IVT_OVERHEAD;                 /* 8KB - 0x200 for IVT */
__CMAC_BASE = 0x20150000;        /* RAM10 for potential CMAC code */
__CMAC_SIZE = 0;                 /* Configurable based on project needs */
__RAMC_BASE = 0x10010000;        /* RAM3 for retained code, remapped */
__RAMC_SIZE = 256K;              /* 256KB for retained code */
__RAMS_BASE = 0x20010000;        /* RAM4-RAM7 for retained data */
__RAMS_SIZE = 1024K;       /* 1MB for retained data, 64K reserved for SNC */
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
    /*
    .vectors :
    {
        . = ALIGN(4);
        KEEP(*(.vectors))
        . = ALIGN(4);
    } > IVT
    */

    /* Code section in FLASH */
    .text : {
        _stext = ABSOLUTE(.);
        *(.vectors)                   /* Vector table */
        *(.text .text.*)              /* Code sections */
        *(.fixup)                     /* Fixup sections */
        *(.gnu.warning)               /* GNU warnings */
        *(.rodata .rodata.*)          /* Read-only data */
        *(.gnu.linkonce.t.*)          /* Link-once sections */
        *(.glue_7)                    /* ARM glue code */
        *(.glue_7t)
        *(.got)                       /* Global Offset Table */
        *(.gcc_except_table)          /* Exception tables */
        *(.gnu.linkonce.r.*)          /* More link-once sections */
        __etext = ABSOLUTE(.);
    } > FLASH

    .init_section : {
        _sinit = ABSOLUTE(.);
        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP(*(.init_array EXCLUDE_FILE(*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o) .ctors))
        _einit = ABSOLUTE(.);
    } > FLASH

    .ARM.extab : {
        *(.ARM.extab*)
    } > FLASH

    .ARM.exidx : {
        __exidx_start = ABSOLUTE(.);
        *(.ARM.exidx*)
        __exidx_end = ABSOLUTE(.);
    } > FLASH

    _eronly = ABSOLUTE(.);

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
    .data : {
        _sdata = ABSOLUTE(.);
        *(.data .data.*)
        *(.gnu.linkonce.d.*)
        CONSTRUCTORS
        . = ALIGN(4);
        _edata = ABSOLUTE(.);
    } > RAMS AT > FLASH

    /* BSS section (zero-initialized data) */
    .bss : {
        _sbss = ABSOLUTE(.);
        *(.bss .bss.*)
        *(.gnu.linkonce.b.*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = ABSOLUTE(.);
    } > RAMS

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
