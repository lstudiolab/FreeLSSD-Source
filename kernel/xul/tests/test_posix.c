/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include <assert.h>
#include <stdio.h>
#include "xul/posix.h"
static int64_t test_call(xul_posix_call_t *call)
{ return (int64_t)call->arguments[0] + (int64_t)call->arguments[1]; }
int main(void)
{
    xul_posix_call_t call = {{7, 9, 0, 0, 0, 0}, 0};
    xul_posix_test_reset();
    assert(xul_posix_dispatch(1, &call) == -XUL_POSIX_ENOSYS);
    assert(xul_posix_register(1, test_call) == XUL_OK);
    assert(xul_posix_register(1, test_call) == XUL_E_DUPLICATE);
    assert(xul_posix_dispatch(1, &call) == 16);
    assert(xul_posix_dispatch(1, 0) == -XUL_E_INVALID);
    assert(xul_posix_unregister(1) == XUL_OK);
    assert(xul_posix_registered_count() == 0);
    xul_posix_test_reset();
    puts("XUL POSIX syscall dispatch tests: PASS");
    return 0;
}
