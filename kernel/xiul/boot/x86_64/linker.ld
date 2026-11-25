/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */
OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)
ENTRY(xiul_arch_entry)

SECTIONS
{
    . = 0xffffffff80000000;
    __xiul_kernel_start = .;

    .text ALIGN(0x1000) : {
        KEEP(*(.text.boot))
        *(.text .text.*)
    }

    .rodata ALIGN(0x1000) : { *(.rodata .rodata.*) }
    .data ALIGN(0x1000) : { *(.data .data.*) }
    .bss ALIGN(0x1000) (NOLOAD) : {
        *(COMMON)
        *(.bss.boot)
        *(.bss .bss.*)
    }

    . = ALIGN(0x1000);
    __xiul_kernel_end = .;

    /DISCARD/ : { *(.comment) *(.note*) *(.eh_frame*) }
}
