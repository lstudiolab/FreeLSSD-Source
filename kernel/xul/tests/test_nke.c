/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include <assert.h>
#include <stdio.h>
#include "xul/nke.h"
static int sequence[4], count, tokens[4];
static xul_nke_action_t record_pass(xul_nke_packet_t *p, void *ctx)
{ (void)p; sequence[count++] = ctx == &tokens[1] ? 1 : ctx == &tokens[2] ? 2 : 3; return XUL_NKE_PASS; }
static xul_nke_action_t record_drop(xul_nke_packet_t *p, void *ctx)
{ (void)p; sequence[count++] = ctx == &tokens[1] ? 1 : ctx == &tokens[2] ? 2 : 3; return XUL_NKE_DROP; }
int main(void)
{
    char data[32];
    xul_nke_packet_t packet = {data, sizeof(data), 0x0800};
    const xul_nke_hook_t late = {"late", 20, record_pass, &tokens[2]};
    const xul_nke_hook_t early = {"early", 10, record_pass, &tokens[1]};
    const xul_nke_hook_t drop = {"drop", 30, record_drop, &tokens[3]};
    xul_nke_test_reset(); count = 0;
    assert(xul_nke_register(&late) == XUL_OK);
    assert(xul_nke_register(&early) == XUL_OK);
    assert(xul_nke_register(&drop) == XUL_OK);
    assert(xul_nke_process(&packet) == XUL_NKE_DROP);
    assert(count == 3 && sequence[0] == 1 && sequence[1] == 2 && sequence[2] == 3);
    assert(xul_nke_unregister(&early) == XUL_OK);
    assert(xul_nke_hook_count() == 2);
    xul_nke_test_reset();
    puts("XUL NKE hook tests: PASS");
    return 0;
}
