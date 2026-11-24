#include "xiul/linux_pci.h"

typedef struct {
    const xiul_linux_pci_driver_t *owner;
    xiul_device_id_t ids[XIUL_DRIVER_ID_MAX];
    xiul_driver_t bridge;
} xiul_linux_pci_registration_t;

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
    if (registration->owner->remove != 0)
        registration->owner->remove((xiul_linux_pci_device_t *)device);
}

xiul_status_t xiul_linux_pci_register_driver(const xiul_linux_pci_driver_t *driver)
{
    xiul_linux_pci_registration_t *registration;
    size_t i;
    xiul_status_t status;
    if (driver == 0 || driver->name == 0 || driver->name[0] == '\0' ||
        driver->id_table == 0 || driver->id_count == 0 ||
        driver->id_count > XIUL_DRIVER_ID_MAX || driver->probe == 0)
        return XIUL_E_INVALID;
    for (i = 0; i < registration_count; ++i)
        if (registrations[i].owner == driver) return XIUL_E_DUPLICATE;
    if (registration_count == XIUL_LINUX_PCI_DRIVER_MAX) return XIUL_E_CAPACITY;
    registration = &registrations[registration_count];
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
    ++registration_count;
    status = xiul_driver_register(&registration->bridge);
    if (status != XIUL_OK) {
        --registration_count;
        registration->owner = 0;
        return status;
    }
    return XIUL_OK;
}

xiul_status_t xiul_linux_pci_unregister_driver(const xiul_linux_pci_driver_t *driver)
{
    size_t i;
    xiul_status_t status;
    if (driver == 0) return XIUL_E_INVALID;
    for (i = 0; i < registration_count; ++i) {
        xiul_linux_pci_registration_t *registration = &registrations[i];
        if (registration->owner != driver) continue;
        status = xiul_driver_unregister(&registration->bridge);
        if (status != XIUL_OK) return status;
        for (; i + 1 < registration_count; ++i)
            registrations[i] = registrations[i + 1];
        --registration_count;
        registrations[registration_count].owner = 0;
        return XIUL_OK;
    }
    return XIUL_E_NOT_FOUND;
}

xiul_status_t xiul_linux_pci_enable_device(xiul_linux_pci_device_t *device)
{
    if (device == 0 || device->device.id.bus != XIUL_BUS_PCI)
        return XIUL_E_INVALID;
    device->enabled = 1;
    return XIUL_OK;
}

void xiul_linux_pci_disable_device(xiul_linux_pci_device_t *device)
{
    if (device != 0) device->enabled = 0;
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
    while (registration_count != 0) {
        xiul_linux_pci_registration_t *r = &registrations[registration_count - 1];
        (void)xiul_driver_unregister(&r->bridge);
        r->owner = 0;
        --registration_count;
    }
}
#endif
