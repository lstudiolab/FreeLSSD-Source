/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 *
 * XIUL x86_64 loader entry. Loader passes xiul_boot_info_t in RDI.
 */
.section .text.boot,"ax",@progbits
.code64
.global xiul_arch_entry
.type xiul_arch_entry,@function
.extern xiul_kernel_entry

xiul_arch_entry:
    cli
    cld
    leaq xiul_boot_stack_top(%rip), %rsp
    andq $-16, %rsp
    xorq %rbp, %rbp
    call xiul_kernel_entry

1:
    cli
    hlt
    jmp 1b
.size xiul_arch_entry, .-xiul_arch_entry

.section .bss.boot,"aw",@nobits
.align 16
.global xiul_boot_stack_bottom
xiul_boot_stack_bottom:
    .skip 65536
.global xiul_boot_stack_top
xiul_boot_stack_top:
