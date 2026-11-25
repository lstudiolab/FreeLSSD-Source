/*
 * Modified by L,Studio.
 * Copyright (c) 2026 L,Studio.
 */

/*
 * Modified by L,Studio.
 * Copyright (c) 2026 L,Studio.
 */

#include "xiul/posix.h"

static xiul_posix_syscall_fn syscall_table[XIUL_POSIX_SYSCALL_MAX];
static size_t registered_count;

xiul_status_t xiul_posix_register(uint32_t number, xiul_posix_syscall_fn handler)
{
    if (number >= XIUL_POSIX_SYSCALL_MAX || handler == 0) return XIUL_E_INVALID;
    if (syscall_table[number] != 0) return XIUL_E_DUPLICATE;
    syscall_table[number] = handler;
    ++registered_count;
    return XIUL_OK;
}

xiul_status_t xiul_posix_unregister(uint32_t number)
{
    if (number >= XIUL_POSIX_SYSCALL_MAX) return XIUL_E_INVALID;
    if (syscall_table[number] == 0) return XIUL_E_NOT_FOUND;
    syscall_table[number] = 0;
    --registered_count;
    return XIUL_OK;
}

int64_t xiul_posix_dispatch(uint32_t number, xiul_posix_call_t *call)
{
    if (number >= XIUL_POSIX_SYSCALL_MAX || syscall_table[number] == 0)
        return -(int64_t)XIUL_POSIX_ENOSYS;
    if (call == 0) return -(int64_t)XIUL_E_INVALID;
    return syscall_table[number](call);
}

size_t xiul_posix_registered_count(void) { return registered_count; }

#ifdef XIUL_TESTING
void xiul_posix_test_reset(void)
{
    size_t i;
    for (i = 0; i < XIUL_POSIX_SYSCALL_MAX; ++i) syscall_table[i] = 0;
    registered_count = 0;
}
#endif
