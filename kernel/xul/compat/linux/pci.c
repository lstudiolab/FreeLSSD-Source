/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include "xul/linux_pci.h"
#include "xul/pci.h"

typedef struct {
    const xul_linux_pci_driver_t *owner;
    xul_device_id_t ids[XUL_DRIVER_ID_MAX];
    xul_driver_t bridge;
} xul_linux_pci_registration_t;

/* Slots never move: XUL stores pointers to each bridge and its context. */
static xul_linux_pci_registration_t registrations[XUL_LINUX_PCI_DRIVER_MAX];
static size_t registration_count;

static int id_matches(const xul_linux_pci_id_t *id,
                      const xul_linux_pci_device_t *device)
{
    const xul_device_id_t *actual = &device->device.id;
    return (id->vendor == XUL_ID_ANY || id->vendor == actual->vendor_id) &&
        (id->device == XUL_ID_ANY || id->device == actual->device_id) &&
        (id->class_id == XUL_ID_ANY || id->class_id == actual->class_id) &&
        (id->subclass_id == XUL_ID_ANY || id->subclass_id == actual->subclass_id);
}

static const xul_linux_pci_id_t *find_id(
    const xul_linux_pci_driver_t *driver, const xul_linux_pci_device_t *device)
{
    size_t i;
    for (i = 0; i < driver->id_count; ++i)
        if (id_matches(&driver->id_table[i], device)) return &driver->id_table[i];
    return 0;
}

static xul_status_t bridge_probe(xul_device_t *device, void *context)
{
    xul_linux_pci_registration_t *registration =
        (xul_linux_pci_registration_t *)context;
    xul_linux_pci_device_t *pci = (xul_linux_pci_device_t *)device;
    const xul_linux_pci_id_t *id = find_id(registration->owner, pci);
    if (id == 0) return XUL_E_NOT_FOUND;
    return registration->owner->probe(pci, id);
}

static void bridge_remove(xul_device_t *device, void *context)
{
    xul_linux_pci_registration_t *registration =
        (xul_linux_pci_registration_t *)context;
    if (registration->owner != 0 && registration->owner->remove != 0)
        registration->owner->remove((xul_linux_pci_device_t *)device);
}

xul_status_t xul_linux_pci_register_driver(const xul_linux_pci_driver_t *driver)
{
    xul_linux_pci_registration_t *registration = 0;
    size_t i;
    xul_status_t status;
    if (driver == 0 || driver->name == 0 || driver->name[0] == '\0' ||
        driver->id_table == 0 || driver->id_count == 0 ||
        driver->id_count > XUL_DRIVER_ID_MAX || driver->probe == 0)
        return XUL_E_INVALID;
    for (i = 0; i < XUL_LINUX_PCI_DRIVER_MAX; ++i) {
        if (registrations[i].owner == driver) return XUL_E_DUPLICATE;
        if (registration == 0 && registrations[i].owner == 0)
            registration = &registrations[i];
    }
    if (registration == 0) return XUL_E_CAPACITY;
    registration->owner = driver;
    for (i = 0; i < driver->id_count; ++i) {
        registration->ids[i].bus = XUL_BUS_PCI;
        registration->ids[i].vendor_id = driver->id_table[i].vendor;
        registration->ids[i].device_id = driver->id_table[i].device;
        registration->ids[i].class_id = driver->id_table[i].class_id;
        registration->ids[i].subclass_id = driver->id_table[i].subclass_id;
    }
    registration->bridge.name = driver->name;
    registration->bridge.id_table = registration->ids;
    registration->bridge.id_count = driver->id_count;
    registration->bridge.context = registration;
    registration->bridge.probe_context = bridge_probe;
    registration->bridge.remove_context = bridge_remove;
    status = xul_driver_register(&registration->bridge);
    if (status != XUL_OK) {
        registration->owner = 0;
        return status;
    }
    ++registration_count;
    return XUL_OK;
}

xul_status_t xul_linux_pci_unregister_driver(const xul_linux_pci_driver_t *driver)
{
    size_t i;
    xul_status_t status;
    if (driver == 0) return XUL_E_INVALID;
    for (i = 0; i < XUL_LINUX_PCI_DRIVER_MAX; ++i) {
        xul_linux_pci_registration_t *registration = &registrations[i];
        if (registration->owner != driver) continue;
        status = xul_driver_unregister(&registration->bridge);
        if (status != XUL_OK) return status;
        registration->owner = 0;
        --registration_count;
        return XUL_OK;
    }
    return XUL_E_NOT_FOUND;
}

xul_status_t xul_linux_pci_enable_device(xul_linux_pci_device_t *device)
{
    uint16_t command;
    unsigned int i;
    xul_status_t status;
    if (device == 0 || device->device.id.bus != XUL_BUS_PCI)
        return XUL_E_INVALID;
    if (device->host != 0) {
        status = device->host->read16(device->host->context, device->bus_number,
            device->device_number, device->function_number, 0x04u, &command);
        if (status != XUL_OK) return status;
        for (i = 0; i < XUL_LINUX_PCI_BAR_COUNT; ++i) {
            if ((device->bar_flags[i] & XUL_LINUX_PCI_BAR_IO) != 0) command |= 1u;
            if ((device->bar_flags[i] & XUL_LINUX_PCI_BAR_MEMORY) != 0) command |= 2u;
        }
        status = device->host->write16(device->host->context, device->bus_number,
            device->device_number, device->function_number, 0x04u, command);
        if (status != XUL_OK) return status;
    }
    device->enabled = 1;
    return XUL_OK;
}

void xul_linux_pci_disable_device(xul_linux_pci_device_t *device)
{
    uint16_t command;
    if (device == 0) return;
    if (device->host != 0 &&
        device->host->read16(device->host->context, device->bus_number,
            device->device_number, device->function_number, 0x04u, &command) == XUL_OK) {
        command = (uint16_t)((command & ~UINT16_C(3)) |
                             (device->original_command & UINT16_C(3)));
        (void)device->host->write16(device->host->context, device->bus_number,
            device->device_number, device->function_number, 0x04u, command);
    }
    device->enabled = 0;
}

xul_status_t xul_linux_pci_set_master(xul_linux_pci_device_t *device)
{
    uint16_t command;
    xul_status_t status;
    if (device == 0 || device->device.id.bus != XUL_BUS_PCI)
        return XUL_E_INVALID;
    if (device->host == 0) return XUL_E_UNSUPPORTED;
    status = device->host->read16(device->host->context, device->bus_number,
        device->device_number, device->function_number, 0x04u, &command);
    if (status != XUL_OK) return status;
    return device->host->write16(device->host->context, device->bus_number,
        device->device_number, device->function_number, 0x04u,
        (uint16_t)(command | UINT16_C(4)));
}

void xul_linux_pci_clear_master(xul_linux_pci_device_t *device)
{
    uint16_t command;
    if (device == 0 || device->host == 0) return;
    if (device->host->read16(device->host->context, device->bus_number,
        device->device_number, device->function_number, 0x04u, &command) != XUL_OK)
        return;
    (void)device->host->write16(device->host->context, device->bus_number,
        device->device_number, device->function_number, 0x04u,
        (uint16_t)(command & ~UINT16_C(4)));
}

uint64_t xul_linux_pci_resource_start(const xul_linux_pci_device_t *device, unsigned int bar)
{
    return device != 0 && bar < XUL_LINUX_PCI_BAR_COUNT ? device->bar_start[bar] : 0;
}

uint64_t xul_linux_pci_resource_length(const xul_linux_pci_device_t *device, unsigned int bar)
{
    return device != 0 && bar < XUL_LINUX_PCI_BAR_COUNT ? device->bar_length[bar] : 0;
}

volatile void *xul_linux_pci_iomap(const xul_linux_pci_device_t *device, unsigned int bar)
{
    return device != 0 && bar < XUL_LINUX_PCI_BAR_COUNT
        ? device->bar_mapping[bar] : 0;
}

void xul_linux_pci_set_drvdata(xul_linux_pci_device_t *device, void *data)
{
    if (device != 0) device->driver_data = data;
}

void *xul_linux_pci_get_drvdata(const xul_linux_pci_device_t *device)
{
    return device != 0 ? device->driver_data : 0;
}

size_t xul_linux_pci_driver_count(void) { return registration_count; }

#ifdef XUL_TESTING
void xul_linux_pci_test_reset(void)
{
    size_t i;
    for (i = 0; i < XUL_LINUX_PCI_DRIVER_MAX; ++i) {
        xul_linux_pci_registration_t *r = &registrations[i];
        if (r->owner != 0) {
            (void)xul_driver_unregister(&r->bridge);
            r->owner = 0;
        }
    }
    registration_count = 0;
}
#endif
