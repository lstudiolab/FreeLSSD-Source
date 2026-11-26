/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#ifndef XUL_NKE_H
#define XUL_NKE_H
#include <stddef.h>
#include <stdint.h>
#include "xul/status.h"
#define XUL_NKE_MAX_HOOKS 32u
typedef enum { XUL_NKE_PASS = 0, XUL_NKE_DROP = 1 } xul_nke_action_t;
typedef struct { void *data; size_t length; uint16_t protocol; } xul_nke_packet_t;
typedef xul_nke_action_t (*xul_nke_hook_fn)(xul_nke_packet_t *, void *);
typedef struct {
    const char *name;
    int priority;
    xul_nke_hook_fn callback;
    void *context;
} xul_nke_hook_t;
xul_status_t xul_nke_register(const xul_nke_hook_t *hook);
xul_status_t xul_nke_unregister(const xul_nke_hook_t *hook);
xul_nke_action_t xul_nke_process(xul_nke_packet_t *packet);
size_t xul_nke_hook_count(void);
#ifdef XUL_TESTING
void xul_nke_test_reset(void);
#endif
#endif
