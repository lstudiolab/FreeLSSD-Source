#include <assert.h>
#include <stdio.h>
#include "xiul/nke.h"
static int sequence[4], count;
static xiul_nke_action_t record_pass(xiul_nke_packet_t *p, void *ctx)
{ (void)p; sequence[count++] = (int)(uintptr_t)ctx; return XIUL_NKE_PASS; }
static xiul_nke_action_t record_drop(xiul_nke_packet_t *p, void *ctx)
{ (void)p; sequence[count++] = (int)(uintptr_t)ctx; return XIUL_NKE_DROP; }
int main(void)
{
    char data[32];
    xiul_nke_packet_t packet = {data, sizeof(data), 0x0800};
    const xiul_nke_hook_t late = {"late", 20, record_pass, (void *)2};
    const xiul_nke_hook_t early = {"early", 10, record_pass, (void *)1};
    const xiul_nke_hook_t drop = {"drop", 30, record_drop, (void *)3};
    xiul_nke_test_reset(); count = 0;
    assert(xiul_nke_register(&late) == XIUL_OK);
    assert(xiul_nke_register(&early) == XIUL_OK);
    assert(xiul_nke_register(&drop) == XIUL_OK);
    assert(xiul_nke_process(&packet) == XIUL_NKE_DROP);
    assert(count == 3 && sequence[0] == 1 && sequence[1] == 2 && sequence[2] == 3);
    assert(xiul_nke_unregister(&early) == XIUL_OK);
    assert(xiul_nke_hook_count() == 2);
    xiul_nke_test_reset();
    puts("XIUL NKE hook tests: PASS");
    return 0;
}
