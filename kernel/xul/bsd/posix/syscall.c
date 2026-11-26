/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include "xul/posix.h"

static xul_posix_syscall_fn syscall_table[XUL_POSIX_SYSCALL_MAX];
static size_t registered_count;

xul_status_t xul_posix_register(uint32_t number, xul_posix_syscall_fn handler)
{
    if (number >= XUL_POSIX_SYSCALL_MAX || handler == 0) return XUL_E_INVALID;
    if (syscall_table[number] != 0) return XUL_E_DUPLICATE;
    syscall_table[number] = handler;
    ++registered_count;
    return XUL_OK;
}

xul_status_t xul_posix_unregister(uint32_t number)
{
    if (number >= XUL_POSIX_SYSCALL_MAX) return XUL_E_INVALID;
    if (syscall_table[number] == 0) return XUL_E_NOT_FOUND;
    syscall_table[number] = 0;
    --registered_count;
    return XUL_OK;
}

int64_t xul_posix_dispatch(uint32_t number, xul_posix_call_t *call)
{
    if (number >= XUL_POSIX_SYSCALL_MAX || syscall_table[number] == 0)
        return -(int64_t)XUL_POSIX_ENOSYS;
    if (call == 0) return -(int64_t)XUL_E_INVALID;
    return syscall_table[number](call);
}

size_t xul_posix_registered_count(void) { return registered_count; }

#ifdef XUL_TESTING
void xul_posix_test_reset(void)
{
    size_t i;
    for (i = 0; i < XUL_POSIX_SYSCALL_MAX; ++i) syscall_table[i] = 0;
    registered_count = 0;
}
#endif
