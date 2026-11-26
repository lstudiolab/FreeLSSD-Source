/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#ifndef XUL_DEVICE_H
#define XUL_DEVICE_H

#include <stddef.h>
#include <stdint.h>
#include "xul/status.h"

#ifdef __cplusplus
extern "C" {
#endif

#define XUL_DEVICE_MAX 128u
#define XUL_DRIVER_MAX 64u
#define XUL_DRIVER_ID_MAX 32u
#define XUL_ID_ANY UINT16_MAX

typedef enum {
    XUL_BUS_PLATFORM = 0,
    XUL_BUS_PCI = 1,
    XUL_BUS_USB = 2,
    XUL_BUS_VIRTIO = 3
} xul_bus_type_t;

typedef struct {
    xul_bus_type_t bus;
    uint16_t vendor_id;
    uint16_t device_id;
    uint16_t class_id;
    uint16_t subclass_id;
} xul_device_id_t;

typedef struct xul_device {
    const char *name;
    xul_device_id_t id;
    void *platform_data;
    void *driver_data;
    const struct xul_driver *bound_driver;
} xul_device_t;

typedef xul_status_t (*xul_driver_probe_context_fn)(xul_device_t *, void *);
typedef void (*xul_driver_remove_context_fn)(xul_device_t *, void *);

typedef struct xul_driver {
    const char *name;
    const xul_device_id_t *id_table;
    size_t id_count;
    xul_status_t (*probe)(xul_device_t *device);
    void (*remove)(xul_device_t *device);
    void *context;
    xul_driver_probe_context_fn probe_context;
    xul_driver_remove_context_fn remove_context;
} xul_driver_t;

/*
 * Registration and hotplug callbacks must be externally serialized.
 * Driver/device descriptors and their names/ID tables must outlive registration.
 */
xul_status_t xul_device_register(xul_device_t *device);
xul_status_t xul_device_unregister(xul_device_t *device);
xul_status_t xul_driver_register(const xul_driver_t *driver);
xul_status_t xul_driver_unregister(const xul_driver_t *driver);
size_t xul_device_count(void);
size_t xul_driver_count(void);

#ifdef XUL_TESTING
void xul_device_test_reset(void);
#endif

#ifdef __cplusplus
}
#endif
#endif
