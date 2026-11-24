#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "xiul/pci.h"
#include "xiul/linux_pci.h"

typedef struct {
    uint32_t config[64];
    int bar_probe;
    unsigned int maps;
    unsigned int unmaps;
    unsigned char mmio[4096];
} fake_pci_t;

static xiul_status_t fake_read32(void *context, uint8_t bus, uint8_t device,
                                 uint8_t function, uint16_t offset, uint32_t *value)
{
    fake_pci_t *fake = context;
    if (bus != 0 || device != 1 || function != 0) return XIUL_E_NOT_FOUND;
    if (offset == 0x10u && fake->bar_probe) {
        *value = 0xfffff000u;
        return XIUL_OK;
    }
    *value = fake->config[offset / 4u];
    return XIUL_OK;
}

static xiul_status_t fake_write32(void *context, uint8_t bus, uint8_t device,
                                  uint8_t function, uint16_t offset, uint32_t value)
{
    fake_pci_t *fake = context;
    if (bus != 0 || device != 1 || function != 0) return XIUL_E_NOT_FOUND;
    if (offset == 0x10u && value == UINT32_MAX) {
        fake->bar_probe = 1;
        return XIUL_OK;
    }
    if (offset == 0x10u) fake->bar_probe = 0;
    fake->config[offset / 4u] = value;
    return XIUL_OK;
}

static xiul_status_t fake_read16(void *context, uint8_t bus, uint8_t device,
                                 uint8_t function, uint16_t offset, uint16_t *value)
{
    uint32_t word;
    xiul_status_t status = fake_read32(context, bus, device, function,
                                        (uint16_t)(offset & ~3u), &word);
    if (status == XIUL_OK) *value = (uint16_t)(word >> ((offset & 2u) * 8u));
    return status;
}

static xiul_status_t fake_write16(void *context, uint8_t bus, uint8_t device,
                                  uint8_t function, uint16_t offset, uint16_t value)
{
    fake_pci_t *fake = context;
    if (bus != 0 || device != 1 || function != 0 || offset != 0x04u)
        return XIUL_E_NOT_FOUND;
    fake->config[1] = (fake->config[1] & 0xffff0000u) | value;
    return XIUL_OK;
}

static volatile void *fake_map(void *context, uint64_t address, size_t length)
{
    fake_pci_t *fake = context;
    assert(address == 0xf0000000u && length == sizeof(fake->mmio));
    ++fake->maps;
    return fake->mmio;
}

static void fake_unmap(void *context, volatile void *mapping, size_t length)
{
    fake_pci_t *fake = context;
    assert(mapping == fake->mmio && length == sizeof(fake->mmio));
    ++fake->unmaps;
}

static int probed, removed;
static xiul_status_t driver_probe(xiul_linux_pci_device_t *device,
                                  const xiul_linux_pci_id_t *id)
{
    assert(id->vendor == 0x1af4 && id->device == 0x1001);
    assert(device->bus_number == 0 && device->device_number == 1);
    assert(device->function_number == 0 && device->irq == 11);
    assert(xiul_linux_pci_resource_start(device, 0) == 0xf0000000u);
    assert(xiul_linux_pci_resource_length(device, 0) == 4096);
    assert(xiul_linux_pci_iomap(device, 0) != 0);
    ++probed;
    return XIUL_OK;
}
static void driver_remove(xiul_linux_pci_device_t *device)
{
    (void)device;
    ++removed;
}

int main(void)
{
    fake_pci_t fake;
    xiul_pci_host_t host;
    size_t found = 0;
    const xiul_linux_pci_id_t ids[] = {
        {0x1af4, 0x1001, 0x01, 0x00}
    };
    const xiul_linux_pci_driver_t driver = {
        "xiul-pci-enumeration-test", ids, 1, driver_probe, driver_remove
    };
    memset(&fake, 0, sizeof(fake));
    fake.config[0] = 0x10011af4u;
    fake.config[2] = 0x01000000u;
    fake.config[3] = 0;
    fake.config[1] = 0x00000003u;
    fake.config[4] = 0xf0000000u;
    fake.config[15] = 11u;

    memset(&host, 0, sizeof(host));
    host.context = &fake;
    host.first_bus = 0;
    host.last_bus = 0;
    host.read16 = fake_read16;
    host.write16 = fake_write16;
    host.read32 = fake_read32;
    host.write32 = fake_write32;
    host.map_mmio = fake_map;
    host.unmap_mmio = fake_unmap;

    xiul_pci_test_reset();
    xiul_linux_pci_test_reset();
    xiul_device_test_reset();
    probed = removed = 0;
    assert(xiul_linux_pci_register_driver(&driver) == XIUL_OK);
    assert(xiul_pci_enumerate(&host, &found) == XIUL_OK);
    assert(found == 1 && xiul_pci_device_count() == 1);
    assert(probed == 1 && fake.maps == 1);\n    assert((fake.config[1] & 0xffffu) == 0x0007u);
    assert((fake.config[1] & 0xffffu) == 0x0003u);
    assert(xiul_pci_remove_all() == XIUL_OK);
    assert(removed == 1 && fake.unmaps == 1);\n    assert((fake.config[1] & 0xffffu) == 0x0003u);
    assert(xiul_linux_pci_unregister_driver(&driver) == XIUL_OK);
    assert(xiul_linux_pci_driver_count() == 0);
    xiul_device_test_reset();
    puts("XIUL PCI enumeration tests: PASS");
    return 0;
}
