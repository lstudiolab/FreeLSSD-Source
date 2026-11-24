#ifndef XIUL_INTERFACES_H
#define XIUL_INTERFACES_H

/*
 * XIUL-owned integration contracts.
 *
 * This header deliberately defines boundaries rather than importing Linux,
 * FreeBSD or XNU kernel-private types into the common XIUL interface.
 * Concrete adapters must translate upstream types at their boundary.
 */

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int32_t xiul_status_t;

enum {
    XIUL_OK = 0,
    XIUL_E_INVALID = -1,
    XIUL_E_UNAVAILABLE = -2,
    XIUL_E_NOT_SUPPORTED = -3,
    XIUL_E_NO_MEMORY = -4,
    XIUL_E_BUSY = -5
};

typedef struct xiul_device xiul_device_t;
typedef struct xiul_driver xiul_driver_t;
typedef struct xiul_io_stream xiul_io_stream_t;
typedef struct xiul_net_packet xiul_net_packet_t;
typedef struct xiul_vnode xiul_vnode_t;

/* Platform services are supplied by the Mach/platform integration. */
typedef struct {
    void *(*allocate)(size_t size, size_t alignment);
    void (*deallocate)(void *address, size_t size);
    xiul_status_t (*interrupt_register)(uint32_t irq,
                                        void (*handler)(void *),
                                        void *context);
    void (*interrupt_unregister)(uint32_t irq, void *context);
    uint64_t (*monotonic_time_ns)(void);
} xiul_platform_ops_t;

/* XIUL owns device lifetime; Linux drivers are adapted to this contract. */
typedef struct {
    xiul_status_t (*probe)(xiul_device_t *device);
    void (*remove)(xiul_device_t *device);
    xiul_status_t (*suspend)(xiul_device_t *device);
    xiul_status_t (*resume)(xiul_device_t *device);
} xiul_driver_ops_t;

/* I/O streams provide a common device-to-consumer data path. */
typedef struct {
    xiul_status_t (*read)(xiul_io_stream_t *stream,
                          void *buffer, size_t capacity, size_t *transferred);
    xiul_status_t (*write)(xiul_io_stream_t *stream,
                           const void *buffer, size_t length,
                           size_t *transferred);
    xiul_status_t (*close)(xiul_io_stream_t *stream);
} xiul_io_stream_ops_t;

/* BSD-derived networking is adapted to this packet hand-off boundary. */
typedef struct {
    xiul_status_t (*receive)(xiul_net_packet_t *packet);
    xiul_status_t (*transmit)(xiul_net_packet_t *packet);
} xiul_network_ops_t;

/* BSD-derived filesystems register through the XIUL VFS boundary. */
typedef struct {
    xiul_status_t (*mount)(const char *source, const char *target);
    xiul_status_t (*unmount)(const char *target);
    xiul_status_t (*lookup)(xiul_vnode_t *directory, const char *name,
                            xiul_vnode_t **result);
} xiul_vfs_ops_t;

/* NKE modules are versioned; implementations must guarantee safe unload. */
#define XIUL_NKE_ABI_VERSION 1u

typedef struct {
    uint32_t abi_version;
    const char *name;
    xiul_status_t (*attach)(void);
    void (*detach)(void);
    xiul_status_t (*process_packet)(xiul_net_packet_t *packet);
} xiul_nke_ops_t;

#ifdef __cplusplus
}
#endif

#endif /* XIUL_INTERFACES_H */
