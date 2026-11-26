/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */
OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)
ENTRY(xul_limine_entry)

PHDRS
{
    limine_requests PT_LOAD FLAGS(6);
    text PT_LOAD FLAGS(5);
    rodata PT_LOAD FLAGS(4);
    data PT_LOAD FLAGS(6);
}

SECTIONS
{
    . = 0xffffffff80000000;
    __xul_kernel_start = .;

    .limine_requests ALIGN(8) : {
        KEEP(*(.limine_requests_start))
        KEEP(*(.limine_requests))
        KEEP(*(.limine_requests_end))
    } :limine_requests

    .text ALIGN(0x1000) : {
        KEEP(*(.text.boot))
        *(.text .text.*)
    } :text

    .rodata ALIGN(0x1000) : { *(.rodata .rodata.*) } :rodata
    .data ALIGN(0x1000) : { *(.data .data.*) } :data
    .bss ALIGN(0x1000) (NOLOAD) : {
        *(COMMON)
        *(.bss.boot)
        *(.bss .bss.*)
    } :data

    . = ALIGN(0x1000);
    __xul_kernel_end = .;

    /DISCARD/ : { *(.comment) *(.note*) *(.eh_frame*) }
}
