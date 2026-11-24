#include "xiul/device.h"

static xiul_device_t *devices[XIUL_DEVICE_MAX];
static const xiul_driver_t *drivers[XIUL_DRIVER_MAX];
static size_t devices_used;
static size_t drivers_used;

static int same_name(const char *a, const char *b)
{
    size_t i = 0;
    if (a == 0 || b == 0) return 0;
    while (a[i] && b[i]) {
        if (a[i] != b[i]) return 0;
        ++i;
    }
    return a[i] == b[i];
}

static int id_matches(const xiul_device_id_t *pattern,
                      const xiul_device_id_t *actual)
{
    return pattern->bus == actual->bus &&
        (pattern->vendor_id == XIUL_ID_ANY || pattern->vendor_id == actual->vendor_id) &&
        (pattern->device_id == XIUL_ID_ANY || pattern->device_id == actual->device_id) &&
        (pattern->class_id == XIUL_ID_ANY || pattern->class_id == actual->class_id) &&
        (pattern->subclass_id == XIUL_ID_ANY || pattern->subclass_id == actual->subclass_id);
}

static int driver_matches(const xiul_driver_t *driver,
                          const xiul_device_t *device)
{
    size_t i;
    for (i = 0; i < driver->id_count; ++i) {
        if (id_matches(&driver->id_table[i], &device->id)) return 1;
    }
    return 0;
}

static void try_bind(xiul_device_t *device)
{
    size_t i;
    if (device->bound_driver != 0) return;
    for (i = 0; i < drivers_used; ++i) {
        const xiul_driver_t *driver = drivers[i];
        if (!driver_matches(driver, device)) continue;
        if ((driver->probe_context != 0\n                ? driver->probe_context(device, driver->context)\n                : driver->probe(device)) == XIUL_OK) {
            device->bound_driver = driver;
            return;
        }
        device->driver_data = 0;
    }
}

xiul_status_t xiul_device_register(xiul_device_t *device)
{
    size_t i;
    if (device == 0 || device->name == 0 || device->name[0] == '\0' ||
        device->id.bus < XIUL_BUS_PLATFORM || device->id.bus > XIUL_BUS_VIRTIO) {
        return XIUL_E_INVALID;
    }
    for (i = 0; i < devices_used; ++i) {
        if (devices[i] == device || same_name(devices[i]->name, device->name)) {
            return XIUL_E_DUPLICATE;
        }
    }
    if (devices_used == XIUL_DEVICE_MAX) return XIUL_E_CAPACITY;
    device->driver_data = 0;
    device->bound_driver = 0;
    devices[devices_used++] = device;
    try_bind(device);
    return XIUL_OK;
}

xiul_status_t xiul_device_unregister(xiul_device_t *device)
{
    size_t i;
    if (device == 0) return XIUL_E_INVALID;
    for (i = 0; i < devices_used; ++i) {
        if (devices[i] != device) continue;
        if (device->bound_driver != 0) {
            if (device->bound_driver->remove != 0) {
                device->bound_driver->remove(device);
            }
            device->bound_driver = 0;
            device->driver_data = 0;
        }
        for (; i + 1 < devices_used; ++i) devices[i] = devices[i + 1];
        devices[--devices_used] = 0;
        return XIUL_OK;
    }
    return XIUL_E_NOT_FOUND;
}

xiul_status_t xiul_driver_register(const xiul_driver_t *driver)
{
    size_t i;
    if (driver == 0 || driver->name == 0 || driver->name[0] == '\0' ||
        driver->id_table == 0 || driver->id_count == 0 ||
        driver->id_count > XIUL_DRIVER_ID_MAX || driver->probe == 0) {
        return XIUL_E_INVALID;
    }
    for (i = 0; i < drivers_used; ++i) {
        if (drivers[i] == driver || same_name(drivers[i]->name, driver->name)) {
            return XIUL_E_DUPLICATE;
        }
    }
    if (drivers_used == XIUL_DRIVER_MAX) return XIUL_E_CAPACITY;
    drivers[drivers_used++] = driver;
    for (i = 0; i < devices_used; ++i) try_bind(devices[i]);
    return XIUL_OK;
}

xiul_status_t xiul_driver_unregister(const xiul_driver_t *driver)
{
    size_t i, j;
    if (driver == 0) return XIUL_E_INVALID;
    for (i = 0; i < drivers_used; ++i) {
        if (drivers[i] != driver) continue;
        for (j = 0; j < devices_used; ++j) {
            if (devices[j]->bound_driver != driver) continue;
            if (driver->remove != 0) driver->remove(devices[j]);
            devices[j]->bound_driver = 0;
            devices[j]->driver_data = 0;
        }
        for (; i + 1 < drivers_used; ++i) drivers[i] = drivers[i + 1];
        drivers[--drivers_used] = 0;
        return XIUL_OK;
    }
    return XIUL_E_NOT_FOUND;
}

size_t xiul_device_count(void) { return devices_used; }
size_t xiul_driver_count(void) { return drivers_used; }

#ifdef XIUL_TESTING
void xiul_device_test_reset(void)
{
    size_t i;
    for (i = 0; i < devices_used; ++i) {
        if (devices[i]->bound_driver && devices[i]->bound_driver->remove) {
            devices[i]->bound_driver->remove(devices[i]);
        }
        devices[i]->bound_driver = 0;
        devices[i]->driver_data = 0;
        devices[i] = 0;
    }
    for (i = 0; i < drivers_used; ++i) drivers[i] = 0;
    devices_used = 0;
    drivers_used = 0;
}
#endif
