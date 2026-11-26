/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 *
 * AArch64 exception vectors for EL1/EL2. Each architectural slot identifies
 * its vector class and reports the active EL's syndrome before fail-stop.
 */
.section .text.exceptions,"ax"
.align 11
.global xul_arm64_vectors
xul_arm64_vectors:
.macro XUL_VECTOR_SLOT number
    b xul_arm64_exception_entry_\number
    .space 124
.endm
XUL_VECTOR_SLOT 0
XUL_VECTOR_SLOT 1
XUL_VECTOR_SLOT 2
XUL_VECTOR_SLOT 3
XUL_VECTOR_SLOT 4
XUL_VECTOR_SLOT 5
XUL_VECTOR_SLOT 6
XUL_VECTOR_SLOT 7
XUL_VECTOR_SLOT 8
XUL_VECTOR_SLOT 9
XUL_VECTOR_SLOT 10
XUL_VECTOR_SLOT 11
XUL_VECTOR_SLOT 12
XUL_VECTOR_SLOT 13
XUL_VECTOR_SLOT 14
XUL_VECTOR_SLOT 15

.global xul_arm64_exception_initialize
.type xul_arm64_exception_initialize, %function
xul_arm64_exception_initialize:
    mrs x1, CurrentEL
    cmp x1, #4
    b.eq 1f
    cmp x1, #8
    b.ne 2f
    adrp x0, xul_arm64_vectors
    add x0, x0, :lo12:xul_arm64_vectors
    msr VBAR_EL2, x0
    dsb sy
    isb
    mov x0, #0
    ret
1:
    adrp x0, xul_arm64_vectors
    add x0, x0, :lo12:xul_arm64_vectors
    msr VBAR_EL1, x0
    dsb sy
    isb
    mov x0, #0
    ret
2:
    mov x0, #1
    ret
.size xul_arm64_exception_initialize, .-xul_arm64_exception_initialize

.macro XUL_EXCEPTION_ENTRY number
.global xul_arm64_exception_entry_\number
xul_arm64_exception_entry_\number:
    mov x0, #\number
    b xul_arm64_exception_common
.endm
XUL_EXCEPTION_ENTRY 0
XUL_EXCEPTION_ENTRY 1
XUL_EXCEPTION_ENTRY 2
XUL_EXCEPTION_ENTRY 3
XUL_EXCEPTION_ENTRY 4
XUL_EXCEPTION_ENTRY 5
XUL_EXCEPTION_ENTRY 6
XUL_EXCEPTION_ENTRY 7
XUL_EXCEPTION_ENTRY 8
XUL_EXCEPTION_ENTRY 9
XUL_EXCEPTION_ENTRY 10
XUL_EXCEPTION_ENTRY 11
XUL_EXCEPTION_ENTRY 12
XUL_EXCEPTION_ENTRY 13
XUL_EXCEPTION_ENTRY 14
XUL_EXCEPTION_ENTRY 15

.global xul_arm64_exception_common
.type xul_arm64_exception_common, %function
xul_arm64_exception_common:
    msr daifset, #0xf
    mrs x9, CurrentEL
    cmp x9, #8
    b.eq 3f
    mrs x1, ESR_EL1
    b 4f
3:
    mrs x1, ESR_EL2
4:
    bl xul_exception_dispatch
5:
    wfe
    b 5b
.size xul_arm64_exception_common, .-xul_arm64_exception_common

.section .note.GNU-stack,"",%progbits
