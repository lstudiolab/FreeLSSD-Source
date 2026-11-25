/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include "xiul/nke.h"

static const xiul_nke_hook_t *hooks[XIUL_NKE_MAX_HOOKS];
static size_t hook_count;

static int name_equal(const char *a, const char *b)
{
    size_t i = 0;
    if (a == 0 || b == 0) return 0;
    while (a[i] && b[i]) { if (a[i] != b[i]) return 0; ++i; }
    return a[i] == b[i];
}

xiul_status_t xiul_nke_register(const xiul_nke_hook_t *hook)
{
    size_t i, position;
    if (hook == 0 || hook->name == 0 || hook->name[0] == '\0' ||
        hook->callback == 0) return XIUL_E_INVALID;
    for (i = 0; i < hook_count; ++i)
        if (hooks[i] == hook || name_equal(hooks[i]->name, hook->name))
            return XIUL_E_DUPLICATE;
    if (hook_count == XIUL_NKE_MAX_HOOKS) return XIUL_E_CAPACITY;
    position = hook_count;
    while (position > 0 && hooks[position - 1]->priority > hook->priority) {
        hooks[position] = hooks[position - 1];
        --position;
    }
    hooks[position] = hook;
    ++hook_count;
    return XIUL_OK;
}

xiul_status_t xiul_nke_unregister(const xiul_nke_hook_t *hook)
{
    size_t i;
    if (hook == 0) return XIUL_E_INVALID;
    for (i = 0; i < hook_count; ++i) {
        if (hooks[i] != hook) continue;
        for (; i + 1 < hook_count; ++i) hooks[i] = hooks[i + 1];
        hooks[--hook_count] = 0;
        return XIUL_OK;
    }
    return XIUL_E_NOT_FOUND;
}

xiul_nke_action_t xiul_nke_process(xiul_nke_packet_t *packet)
{
    size_t i;
    if (packet == 0 || packet->data == 0 || packet->length == 0)
        return XIUL_NKE_DROP;
    for (i = 0; i < hook_count; ++i)
        if (hooks[i]->callback(packet, hooks[i]->context) == XIUL_NKE_DROP)
            return XIUL_NKE_DROP;
    return XIUL_NKE_PASS;
}

size_t xiul_nke_hook_count(void) { return hook_count; }

#ifdef XIUL_TESTING
void xiul_nke_test_reset(void)
{
    size_t i;
    for (i = 0; i < hook_count; ++i) hooks[i] = 0;
    hook_count = 0;
}
#endif
