/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 *
 * XUL AArch64 loader entry. Loader passes xul_boot_info_t in X0.
 */
.section .text.boot,"ax"
.align 4
.global xul_arch_entry
.type xul_arch_entry, %function
.extern xul_kernel_entry

xul_arch_entry:
    msr daifset, #0xf
    adrp x9, xul_boot_stack_top
    add x9, x9, :lo12:xul_boot_stack_top
    mov sp, x9
    and x9, x9, #-16
    mov sp, x9
    mov x29, xzr
    bl xul_kernel_entry

1:
    wfe
    b 1b
.size xul_arch_entry, .-xul_arch_entry

.section .bss.boot,"aw",%nobits
.align 4
.global xul_boot_stack_bottom
xul_boot_stack_bottom:
    .skip 65536
.global xul_boot_stack_top
xul_boot_stack_top:

.section .note.GNU-stack,"",%progbits
