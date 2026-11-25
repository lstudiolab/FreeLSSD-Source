/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 *
 * AArch64 exception vectors for EL1/EL2. Each architectural slot identifies
 * its vector class and reports the active EL's syndrome before fail-stop.
 */
.section .text.exceptions,"ax"
.align 11
.global xiul_arm64_vectors
xiul_arm64_vectors:
.macro XIUL_VECTOR_SLOT number
    b xiul_arm64_exception_entry_\number
    .space 124
.endm
XIUL_VECTOR_SLOT 0
XIUL_VECTOR_SLOT 1
XIUL_VECTOR_SLOT 2
XIUL_VECTOR_SLOT 3
XIUL_VECTOR_SLOT 4
XIUL_VECTOR_SLOT 5
XIUL_VECTOR_SLOT 6
XIUL_VECTOR_SLOT 7
XIUL_VECTOR_SLOT 8
XIUL_VECTOR_SLOT 9
XIUL_VECTOR_SLOT 10
XIUL_VECTOR_SLOT 11
XIUL_VECTOR_SLOT 12
XIUL_VECTOR_SLOT 13
XIUL_VECTOR_SLOT 14
XIUL_VECTOR_SLOT 15

.global xiul_arm64_exception_initialize
.type xiul_arm64_exception_initialize, %function
xiul_arm64_exception_initialize:
    mrs x1, CurrentEL
    cmp x1, #4
    b.eq 1f
    cmp x1, #8
    b.ne 2f
    adrp x0, xiul_arm64_vectors
    add x0, x0, :lo12:xiul_arm64_vectors
    msr VBAR_EL2, x0
    dsb sy
    isb
    mov x0, #0
    ret
1:
    adrp x0, xiul_arm64_vectors
    add x0, x0, :lo12:xiul_arm64_vectors
    msr VBAR_EL1, x0
    dsb sy
    isb
    mov x0, #0
    ret
2:
    mov x0, #1
    ret
.size xiul_arm64_exception_initialize, .-xiul_arm64_exception_initialize

.macro XIUL_EXCEPTION_ENTRY number
.global xiul_arm64_exception_entry_\number
xiul_arm64_exception_entry_\number:
    mov x0, #\number
    b xiul_arm64_exception_common
.endm
XIUL_EXCEPTION_ENTRY 0
XIUL_EXCEPTION_ENTRY 1
XIUL_EXCEPTION_ENTRY 2
XIUL_EXCEPTION_ENTRY 3
XIUL_EXCEPTION_ENTRY 4
XIUL_EXCEPTION_ENTRY 5
XIUL_EXCEPTION_ENTRY 6
XIUL_EXCEPTION_ENTRY 7
XIUL_EXCEPTION_ENTRY 8
XIUL_EXCEPTION_ENTRY 9
XIUL_EXCEPTION_ENTRY 10
XIUL_EXCEPTION_ENTRY 11
XIUL_EXCEPTION_ENTRY 12
XIUL_EXCEPTION_ENTRY 13
XIUL_EXCEPTION_ENTRY 14
XIUL_EXCEPTION_ENTRY 15

.global xiul_arm64_exception_common
.type xiul_arm64_exception_common, %function
xiul_arm64_exception_common:
    msr daifset, #0xf
    mrs x9, CurrentEL
    cmp x9, #8
    b.eq 3f
    mrs x1, ESR_EL1
    b 4f
3:
    mrs x1, ESR_EL2
4:
    bl xiul_exception_dispatch
5:
    wfe
    b 5b
.size xiul_arm64_exception_common, .-xiul_arm64_exception_common

.section .note.GNU-stack,"",%progbits
