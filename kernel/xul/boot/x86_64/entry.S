/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 *
 * XUL x86_64 loader entry. Loader passes xul_boot_info_t in RDI.
 */
.section .text.boot,"ax",@progbits
.code64
.global xul_arch_entry
.type xul_arch_entry,@function
.extern xul_kernel_entry

xul_arch_entry:
    cli
    cld
    leaq xul_boot_stack_top(%rip), %rsp
    andq $-16, %rsp
    xorq %rbp, %rbp
    call xul_kernel_entry

1:
    cli
    hlt
    jmp 1b
.size xul_arch_entry, .-xul_arch_entry

.section .bss.boot,"aw",@nobits
.align 16
.global xul_boot_stack_bottom
xul_boot_stack_bottom:
    .skip 65536
.global xul_boot_stack_top
xul_boot_stack_top:

.section .note.GNU-stack,"",@progbits
