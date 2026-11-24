#include <assert.h>
#include <stdio.h>
#include "xiul/posix.h"
static int64_t test_call(xiul_posix_call_t *call)
{ return (int64_t)call->arguments[0] + (int64_t)call->arguments[1]; }
int main(void)
{
    xiul_posix_call_t call = {{7, 9, 0, 0, 0, 0}, 0};
    xiul_posix_test_reset();
    assert(xiul_posix_dispatch(1, &call) == -XIUL_POSIX_ENOSYS);
    assert(xiul_posix_register(1, test_call) == XIUL_OK);
    assert(xiul_posix_register(1, test_call) == XIUL_E_DUPLICATE);
    assert(xiul_posix_dispatch(1, &call) == 16);
    assert(xiul_posix_dispatch(1, 0) == -XIUL_E_INVALID);
    assert(xiul_posix_unregister(1) == XIUL_OK);
    assert(xiul_posix_registered_count() == 0);
    xiul_posix_test_reset();
    puts("XIUL POSIX syscall dispatch tests: PASS");
    return 0;
}
