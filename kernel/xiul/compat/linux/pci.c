#include "xiul/linux_pci.h"
#include "xiul/pci.h"

typedef struct {
    const xiul_linux_pci_driver_t *owner;
    xiul_device_id_t ids[XIUL_DRIVER_ID_MAX];
    xiul_driver_t bridge;
} xiul_linux_pci_registration_t;

/* Slots never move: XIUL stores pointers to each bridge and its context. */
static xiul_linux_pci_registration_t registrations[XIUL_LINUX_PCI_DRIVER_MAX];
static size_t registration_count;

static int id_matches(const xiul_linux_pci_id_t *id,
                      const xiul_linux_pci_device_t *device)
{
    const xiul_device_id_t *actual = &device->device.id;
    return (id->vendor == XIUL_ID_ANY || id->vendor == actual->vendor_id) &&
        (id->device == XIUL_ID_ANY || id->device == actual->device_id) &&
        (id->class_id == XIUL_ID_ANY || id->class_id == actual->class_id) &&
        (id->subclass_id == XIUL_ID_ANY || id->subclass_id == actual->subclass_id);
}

static const xiul_linux_pci_id_t *find_id(
    const xiul_linux_pci_driver_t *driver, const xiul_linux_pci_device_t *device)
{
    size_t i;
    for (i = 0; i < driver->id_count; ++i)
        if (id_matches(&driver->id_table[i], device)) return &driver->id_table[i];
    return 0;
}

static xiul_status_t bridge_probe(xiul_device_t *device, void *context)
{
    xiul_linux_pci_registration_t *registration =
        (xiul_linux_pci_registration_t *)context;
    xiul_linux_pci_device_t *pci = (xiul_linux_pci_device_t *)device;
    const xiul_linux_pci_id_t *id = find_id(registration->owner, pci);
    if (id == 0) return XIUL_E_NOT_FOUND;
    return registration->owner->probe(pci, id);
}

static void bridge_remove(xiul_device_t *device, void *context)
{
    xiul_linux_pci_registration_t *registration =
        (xiul_linux_pci_registration_t *)context;
    if (registration->owner != 0 && registration->owner->remove != 0)
        registration->owner->remove((xiul_linux_pci_device_t *)device);
}

xiul_status_t xiul_linux_pci_register_driver(const xiul_linux_pci_driver_t *driver)
{
    xiul_linux_pci_registration_t *registration = 0;
    size_t i;
    xiul_status_t status;
    if (driver == 0 || driver->name == 0 || driver->name[0] == '\0' ||
        driver->id_table == 0 || driver->id_count == 0 ||
        driver->id_count > XIUL_DRIVER_ID_MAX || driver->probe == 0)
        return XIUL_E_INVALID;
    for (i = 0; i < XIUL_LINUX_PCI_DRIVER_MAX; ++i) {
        if (registrations[i].owner == driver) return XIUL_E_DUPLICATE;
        if (registration == 0 && registrations[i].owner == 0)
            registration = &registrations[i];
    }
    if (registration == 0) return XIUL_E_CAPACITY;
    registration->owner = driver;
    for (i = 0; i < driver->id_count; ++i) {
        registration->ids[i].bus = XIUL_BUS_PCI;
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
    status = xiul_driver_register(&registration->bridge);
    if (status != XIUL_OK) {
        registration->owner = 0;
        return status;
    }
    ++registration_count;
    return XIUL_OK;
}

xiul_status_t xiul_linux_pci_unregister_driver(const xiul_linux_pci_driver_t *driver)
{
    size_t i;
    xiul_status_t status;
    if (driver == 0) return XIUL_E_INVALID;
    for (i = 0; i < XIUL_LINUX_PCI_DRIVER_MAX; ++i) {
        xiul_linux_pci_registration_t *registration = &registrations[i];
        if (registration->owner != driver) continue;
        status = xiul_driver_unregister(&registration->bridge);
        if (status != XIUL_OK) return status;
        registration->owner = 0;
        --registration_count;
        return XIUL_OK;
    }
    return XIUL_E_NOT_FOUND;
}

xiul_status_t xiul_linux_pci_enable_device(xiul_linux_pci_device_t *device)
{
    uint16_t command;
    unsigned int i;
    xiul_status_t status;
    if (device == 0 || device->device.id.bus != XIUL_BUS_PCI)
        return XIUL_E_INVALID;
    if (device->host != 0) {
        status = device->host->read16(device->host->context, device->bus_number,
            device->device_number, device->function_number, 0x04u, &command);
        if (status != XIUL_OK) return status;
        for (i = 0; i < XIUL_LINUX_PCI_BAR_COUNT; ++i) {
            if ((device->bar_flags[i] & XIUL_LINUX_PCI_BAR_IO) != 0) command |= 1u;
            if ((device->bar_flags[i] & XIUL_LINUX_PCI_BAR_MEMORY) != 0) command |= 2u;
        }
        status = device->host->write16(device->host->context, device->bus_number,
            device->device_number, device->function_number, 0x04u, command);
        if (status != XIUL_OK) return status;
    }
    device->enabled = 1;
    return XIUL_OK;
}

void xiul_linux_pci_disable_device(xiul_linux_pci_device_t *device)
{
    uint16_t command;
    if (device == 0) return;
    if (device->host != 0 &&
        device->host->read16(device->host->context, device->bus_number,
            device->device_number, device->function_number, 0x04u, &command) == XIUL_OK) {
        command = (uint16_t)((command & ~UINT16_C(3)) |
                             (device->original_command & UINT16_C(3)));
        (void)device->host->write16(device->host->context, device->bus_number,
            device->device_number, device->function_number, 0x04u, command);
    }
    device->enabled = 0;
}

xiul_status_t xiul_linux_pci_set_master(xiul_linux_pci_device_t *device)
{
    uint16_t command;
    xiul_status_t status;
    if (device == 0 || device->device.id.bus != XIUL_BUS_PCI)
        return XIUL_E_INVALID;
    if (device->host == 0) return XIUL_E_UNSUPPORTED;
    status = device->host->read16(device->host->context, device->bus_number,
        device->device_number, device->function_number, 0x04u, &command);
    if (status != XIUL_OK) return status;
    return device->host->write16(device->host->context, device->bus_number,
        device->device_number, device->function_number, 0x04u,
        (uint16_t)(command | UINT16_C(4)));
}

void xiul_linux_pci_clear_master(xiul_linux_pci_device_t *device)
{
    uint16_t command;
    if (device == 0 || device->host == 0) return;
    if (device->host->read16(device->host->context, device->bus_number,
        device->device_number, device->function_number, 0x04u, &command) != XIUL_OK)
        return;
    (void)device->host->write16(device->host->context, device->bus_number,
        device->device_number, device->function_number, 0x04u,
        (uint16_t)(command & ~UINT16_C(4)));
}

uint64_t xiul_linux_pci_resource_start(const xiul_linux_pci_device_t *device, unsigned int bar)
{
    return device != 0 && bar < XIUL_LINUX_PCI_BAR_COUNT ? device->bar_start[bar] : 0;
}

uint64_t xiul_linux_pci_resource_length(const xiul_linux_pci_device_t *device, unsigned int bar)
{
    return device != 0 && bar < XIUL_LINUX_PCI_BAR_COUNT ? device->bar_length[bar] : 0;
}

volatile void *xiul_linux_pci_iomap(const xiul_linux_pci_device_t *device, unsigned int bar)
{
    return device != 0 && bar < XIUL_LINUX_PCI_BAR_COUNT
        ? device->bar_mapping[bar] : 0;
}

void xiul_linux_pci_set_drvdata(xiul_linux_pci_device_t *device, void *data)
{
    if (device != 0) device->driver_data = data;
}

void *xiul_linux_pci_get_drvdata(const xiul_linux_pci_device_t *device)
{
    return device != 0 ? device->driver_data : 0;
}

size_t xiul_linux_pci_driver_count(void) { return registration_count; }

#ifdef XIUL_TESTING
void xiul_linux_pci_test_reset(void)
{
    size_t i;
    for (i = 0; i < XIUL_LINUX_PCI_DRIVER_MAX; ++i) {
        xiul_linux_pci_registration_t *r = &registrations[i];
        if (r->owner != 0) {
            (void)xiul_driver_unregister(&r->bridge);
            r->owner = 0;
        }
    }
    registration_count = 0;
}
#endif
