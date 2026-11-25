/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 *
 * XIUL AArch64 loader entry. Loader passes xiul_boot_info_t in X0.
 */
.section .text.boot,"ax"
.align 4
.global xiul_arch_entry
.type xiul_arch_entry, %function
.extern xiul_kernel_entry

xiul_arch_entry:
    msr daifset, #0xf
    adrp x9, xiul_boot_stack_top
    add x9, x9, :lo12:xiul_boot_stack_top
    mov sp, x9
    and x9, x9, #-16
    mov sp, x9
    mov x29, xzr
    bl xiul_kernel_entry

1:
    wfe
    b 1b
.size xiul_arch_entry, .-xiul_arch_entry

.section .bss.boot,"aw",%nobits
.align 4
.global xiul_boot_stack_bottom
xiul_boot_stack_bottom:
    .skip 65536
.global xiul_boot_stack_top
xiul_boot_stack_top:
