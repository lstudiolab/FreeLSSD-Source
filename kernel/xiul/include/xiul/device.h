#ifndef XIUL_DEVICE_H
#define XIUL_DEVICE_H

#include <stddef.h>
#include <stdint.h>
#include "xiul/status.h"

#ifdef __cplusplus
extern "C" {
#endif

#define XIUL_DEVICE_MAX 128u
#define XIUL_DRIVER_MAX 64u
#define XIUL_ID_ANY UINT16_MAX

typedef enum {
    XIUL_BUS_PLATFORM = 0,
    XIUL_BUS_PCI = 1,
    XIUL_BUS_USB = 2,
    XIUL_BUS_VIRTIO = 3
} xiul_bus_type_t;

typedef struct {
    xiul_bus_type_t bus;
    uint16_t vendor_id;
    uint16_t device_id;
    uint16_t class_id;
    uint16_t subclass_id;
} xiul_device_id_t;

typedef struct xiul_device {
    const char *name;
    xiul_device_id_t id;
    void *platform_data;
    void *driver_data;
    const struct xiul_driver *bound_driver;
} xiul_device_t;

typedef struct xiul_driver {
    const char *name;
    const xiul_device_id_t *id_table;
    size_t id_count;
    xiul_status_t (*probe)(xiul_device_t *device);
    void (*remove)(xiul_device_t *device);
} xiul_driver_t;

/*
 * Registration and hotplug callbacks must be externally serialized.
 * Driver/device descriptors and their names/ID tables must outlive registration.
 */
xiul_status_t xiul_device_register(xiul_device_t *device);
xiul_status_t xiul_device_unregister(xiul_device_t *device);
xiul_status_t xiul_driver_register(const xiul_driver_t *driver);
xiul_status_t xiul_driver_unregister(const xiul_driver_t *driver);
size_t xiul_device_count(void);
size_t xiul_driver_count(void);

#ifdef XIUL_TESTING
void xiul_device_test_reset(void);
#endif

#ifdef __cplusplus
}
#endif
#endif
