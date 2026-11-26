/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#ifndef XUL_INTERFACES_H
#define XUL_INTERFACES_H

/*
 * XUL-owned subsystem boundaries. Upstream Linux, FreeBSD and Mach-private
 * types must be translated by their adapter instead of crossing this ABI.
 */
#include <stddef.h>
#include <stdint.h>
#include "xul/status.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct xul_device xul_device_t;
typedef struct xul_driver xul_driver_t;
typedef struct xul_io_stream xul_io_stream_t;
typedef struct xul_net_packet xul_net_packet_t;
typedef struct xul_vnode xul_vnode_t;

typedef struct {
    void *(*allocate)(size_t size, size_t alignment);
    void (*deallocate)(void *address, size_t size);
    xul_status_t (*interrupt_register)(uint32_t irq,
                                        void (*handler)(void *), void *context);
    void (*interrupt_unregister)(uint32_t irq, void *context);
    uint64_t (*monotonic_time_ns)(void);
} xul_platform_ops_t;

typedef struct {
    xul_status_t (*probe)(xul_device_t *device);
    void (*remove)(xul_device_t *device);
    xul_status_t (*suspend)(xul_device_t *device);
    xul_status_t (*resume)(xul_device_t *device);
} xul_driver_ops_t;

typedef struct {
    xul_status_t (*read)(xul_io_stream_t *stream, void *buffer,
                          size_t capacity, size_t *transferred);
    xul_status_t (*write)(xul_io_stream_t *stream, const void *buffer,
                           size_t length, size_t *transferred);
    xul_status_t (*close)(xul_io_stream_t *stream);
} xul_io_stream_ops_t;

typedef struct {
    xul_status_t (*receive)(xul_net_packet_t *packet);
    xul_status_t (*transmit)(xul_net_packet_t *packet);
} xul_network_ops_t;

typedef struct {
    xul_status_t (*mount)(const char *source, const char *target);
    xul_status_t (*unmount)(const char *target);
    xul_status_t (*lookup)(xul_vnode_t *directory, const char *name,
                            xul_vnode_t **result);
} xul_vfs_ops_t;

#define XUL_NKE_ABI_VERSION 1u
typedef struct {
    uint32_t abi_version;
    const char *name;
    xul_status_t (*attach)(void);
    void (*detach)(void);
    xul_status_t (*process_packet)(xul_net_packet_t *packet);
} xul_nke_ops_t;

#ifdef __cplusplus
}
#endif
#endif
