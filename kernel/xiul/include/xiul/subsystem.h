#ifndef XIUL_SUBSYSTEM_H
#define XIUL_SUBSYSTEM_H

#include <stddef.h>
#include <stdint.h>
#include "xiul/status.h"

#ifdef __cplusplus
extern "C" {
#endif

#define XIUL_ABI_VERSION 1u
#define XIUL_SUBSYSTEM_NAME_MAX 48u
#define XIUL_SUBSYSTEM_MAX 64u

typedef enum {
    XIUL_PHASE_EARLY = 0,
    XIUL_PHASE_CORE = 1,
    XIUL_PHASE_PLATFORM = 2,
    XIUL_PHASE_SERVICES = 3,
    XIUL_PHASE_DRIVERS = 4,
    XIUL_PHASE_LATE = 5
} xiul_init_phase_t;

typedef struct xiul_kernel_context xiul_kernel_context_t;
typedef xiul_status_t (*xiul_subsystem_init_fn)(xiul_kernel_context_t *context);
typedef void (*xiul_subsystem_shutdown_fn)(xiul_kernel_context_t *context);

typedef struct {
    uint32_t abi_version;
    const char *name;
    xiul_init_phase_t phase;
    const char *const *dependencies;
    size_t dependency_count;
    xiul_subsystem_init_fn initialize;
    xiul_subsystem_shutdown_fn shutdown;
} xiul_subsystem_descriptor_t;

xiul_status_t xiul_subsystem_register(const xiul_subsystem_descriptor_t *descriptor);
xiul_status_t xiul_subsystem_validate(void);
xiul_status_t xiul_subsystem_start_all(xiul_kernel_context_t *context);
void xiul_subsystem_stop_all(xiul_kernel_context_t *context);
size_t xiul_subsystem_count(void);

#ifdef XIUL_TESTING
void xiul_subsystem_test_reset(void);
#endif

#ifdef __cplusplus
}
#endif
#endif
