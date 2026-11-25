/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 *
 * AArch64 EL1 vector table. All exception classes fail-stop until the Mach
 * exception and interrupt dispatch layers are integrated.
 */
.section .text.exceptions,"ax"
.align 11
.global xiul_arm64_vectors
.type xiul_arm64_vectors, %function
xiul_arm64_vectors:
.rept 16
    b xiul_arm64_exception_entry
    .space 124
.endr
.size xiul_arm64_vectors, .-xiul_arm64_vectors

.global xiul_arm64_exception_initialize
.type xiul_arm64_exception_initialize, %function
xiul_arm64_exception_initialize:
    adrp x0, xiul_arm64_vectors
    add x0, x0, :lo12:xiul_arm64_vectors
    msr VBAR_EL1, x0
    dsb sy
    isb
    ret
.size xiul_arm64_exception_initialize, .-xiul_arm64_exception_initialize

xiul_arm64_exception_entry:
    msr daifset, #0xf
    bl xiul_exception_fatal
1:
    wfe
    b 1b

.section .note.GNU-stack,"",%progbits
