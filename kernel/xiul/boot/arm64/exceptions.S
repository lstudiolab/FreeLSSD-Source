/* This file has been modified by L,Studio. */
.section .text.exceptions,"ax"
.align 11
.global xiul_arm64_vectors
xiul_arm64_vectors:
.rept 16
 b xiul_arm64_exception_entry
 .space 124
.endr
.global xiul_arm64_exception_initialize
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
 adrp x0, xiul_arm64_vectors
 add x0, x0, :lo12:xiul_arm64_vectors
 msr VBAR_EL1, x0
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
xiul_arm64_exception_entry:
 msr daifset, #0xf
 bl xiul_exception_fatal
2:
 wfe
 b 2b
.section .note.GNU-stack,"",%progbits
