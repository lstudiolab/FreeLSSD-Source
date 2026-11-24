#ifndef XIUL_SUBSYSTEM_H
#define XIUL_SUBSYSTEM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define XIUL_ABI_VERSION 1u
#define XIUL_SUBSYSTEM_NAME_MAX 48u

typedef enum {
    XIUL_OK = 0,
    XIUL_E_INVALID = -1,
    XIUL_E_DUPLICATE = -2,
    XIUL_E_DEPENDENCY = -3,
    XIUL_E_STATE = -4,
    XIUL_E_UNSUPPORTED = -5,
    XIUL_E_NOMEM = -6
} xiul_status_t;

typedef enum {
    XIUL_PHASE_EARLY = 0,
    XIUL_PHASE_CORE = 1,
    XIUL_PHASE_PLATFORM = 2,
    XIUL_PHASE_SERVICES = 3,
    XIUL_PHASE_DRIVERS = 4,
    XIUL_PHASE_LATE = 5
} xiul_init_phase_t;

typedef struct xiul_kernel_context xiul_kernel_context_t;

typedef xiul_status_t (*xiul_subsystem_init_fn)(
    xiul_kernel_context_t *context
);

typedef void (*xiul_subsystem_shutdown_fn)(
    xiul_kernel_context_t *context
);

typedef struct {
    uint32_t abi_version;
    const char *name;
    xiul_init_phase_t phase;
    const char *const *dependencies;
    size_t dependency_count;
    xiul_subsystem_init_fn initialize;
    xiul_subsystem_shutdown_fn shutdown;
} xiul_subsystem_descriptor_t;

/*
 * Descriptors are immutable and must remain alive for the kernel lifetime.
 * Registration and lifecycle execution are provided by the XIUL core.
 */
xiul_status_t xiul_subsystem_register(
    const xiul_subsystem_descriptor_t *descriptor
);

xiul_status_t xiul_subsystem_validate(void);

xiul_status_t xiul_subsystem_start_all(
    xiul_kernel_context_t *context
);

void xiul_subsystem_stop_all(
    xiul_kernel_context_t *context
);

#ifdef __cplusplus
}
#endif

#endif
