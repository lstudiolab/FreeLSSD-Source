/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#ifndef XUL_SUBSYSTEM_H
#define XUL_SUBSYSTEM_H

#include <stddef.h>
#include <stdint.h>
#include "xul/status.h"

#ifdef __cplusplus
extern "C" {
#endif

#define XUL_ABI_VERSION 1u
#define XUL_SUBSYSTEM_NAME_MAX 48u
#define XUL_SUBSYSTEM_MAX 64u

typedef enum {
    XUL_PHASE_EARLY = 0,
    XUL_PHASE_CORE = 1,
    XUL_PHASE_PLATFORM = 2,
    XUL_PHASE_SERVICES = 3,
    XUL_PHASE_DRIVERS = 4,
    XUL_PHASE_LATE = 5
} xul_init_phase_t;

typedef struct xul_kernel_context xul_kernel_context_t;
typedef xul_status_t (*xul_subsystem_init_fn)(xul_kernel_context_t *context);
typedef void (*xul_subsystem_shutdown_fn)(xul_kernel_context_t *context);

typedef struct {
    uint32_t abi_version;
    const char *name;
    xul_init_phase_t phase;
    const char *const *dependencies;
    size_t dependency_count;
    xul_subsystem_init_fn initialize;
    xul_subsystem_shutdown_fn shutdown;
} xul_subsystem_descriptor_t;

xul_status_t xul_subsystem_register(const xul_subsystem_descriptor_t *descriptor);
xul_status_t xul_subsystem_validate(void);
xul_status_t xul_subsystem_start_all(xul_kernel_context_t *context);
void xul_subsystem_stop_all(xul_kernel_context_t *context);
size_t xul_subsystem_count(void);

#ifdef XUL_TESTING
void xul_subsystem_test_reset(void);
#endif

#ifdef __cplusplus
}
#endif
#endif
