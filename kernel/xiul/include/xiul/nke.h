/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#ifndef XIUL_NKE_H
#define XIUL_NKE_H
#include <stddef.h>
#include <stdint.h>
#include "xiul/status.h"
#define XIUL_NKE_MAX_HOOKS 32u
typedef enum { XIUL_NKE_PASS = 0, XIUL_NKE_DROP = 1 } xiul_nke_action_t;
typedef struct { void *data; size_t length; uint16_t protocol; } xiul_nke_packet_t;
typedef xiul_nke_action_t (*xiul_nke_hook_fn)(xiul_nke_packet_t *, void *);
typedef struct {
    const char *name;
    int priority;
    xiul_nke_hook_fn callback;
    void *context;
} xiul_nke_hook_t;
xiul_status_t xiul_nke_register(const xiul_nke_hook_t *hook);
xiul_status_t xiul_nke_unregister(const xiul_nke_hook_t *hook);
xiul_nke_action_t xiul_nke_process(xiul_nke_packet_t *packet);
size_t xiul_nke_hook_count(void);
#ifdef XIUL_TESTING
void xiul_nke_test_reset(void);
#endif
#endif
