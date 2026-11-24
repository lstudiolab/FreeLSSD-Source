#include <assert.h>
#include <stdio.h>
#include "xiul/linux_pci.h"

static int probed, removed, probed_b, removed_b, driver_token, driver_token_b;
static xiul_status_t probe(xiul_linux_pci_device_t *device,
                           const xiul_linux_pci_id_t *id)
{
    assert(id->vendor == 0x1af4);
    ++probed;
    assert(xiul_linux_pci_enable_device(device) == XIUL_OK);
    xiul_linux_pci_set_drvdata(device, &driver_token);
    return XIUL_OK;
}
static void remove_device(xiul_linux_pci_device_t *device)
{
    ++removed;
    xiul_linux_pci_disable_device(device);
    xiul_linux_pci_set_drvdata(device, 0);
}
static xiul_status_t probe_b(xiul_linux_pci_device_t *device,
                             const xiul_linux_pci_id_t *id)
{
    assert(id->vendor == 0x8086);
    ++probed_b;
    xiul_linux_pci_set_drvdata(device, &driver_token_b);
    return XIUL_OK;
}
static void remove_b(xiul_linux_pci_device_t *device)
{
    ++removed_b;
    xiul_linux_pci_set_drvdata(device, 0);
}

int main(void)
{
    const xiul_linux_pci_id_t ids[] = {
        {0x1af4, 0x1001, XIUL_ID_ANY, XIUL_ID_ANY}
    };
    const xiul_linux_pci_id_t ids_b[] = {
        {0x8086, 0x100e, XIUL_ID_ANY, XIUL_ID_ANY}
    };
    const xiul_linux_pci_driver_t driver = {
        "xiul-virtio-block-test", ids, 1, probe, remove_device
    };
    const xiul_linux_pci_driver_t driver_b = {
        "xiul-intel-nic-test", ids_b, 1, probe_b, remove_b
    };
    xiul_linux_pci_device_t device = {0};
    xiul_linux_pci_device_t device_b = {0};

    xiul_device_test_reset();
    xiul_linux_pci_test_reset();
    probed = removed = probed_b = removed_b = 0;

    /* Device-first registration path. */
    device.device.name = "virtio-blk0";
    device.device.id.bus = XIUL_BUS_PCI;
    device.device.id.vendor_id = 0x1af4;
    device.device.id.device_id = 0x1001;
    device.bar_start[0] = 0xf0000000u;
    device.bar_length[0] = 4096;
    device.bar_mapping[0] = (volatile void *)0x1000;
    assert(xiul_device_register(&device.device) == XIUL_OK);
    assert(xiul_linux_pci_register_driver(&driver) == XIUL_OK);
    assert(probed == 1 && device.enabled == 1);
    assert(xiul_linux_pci_get_drvdata(&device) == &driver_token);
    assert(xiul_linux_pci_resource_start(&device, 0) == 0xf0000000u);
    assert(xiul_linux_pci_resource_length(&device, 0) == 4096);
    assert(xiul_linux_pci_iomap(&device, 0) == (volatile void *)0x1000);

    /* Register a second driver, remove the first, then exercise driver-first binding.
       The remaining bridge must retain its stable context address. */
    assert(xiul_linux_pci_register_driver(&driver_b) == XIUL_OK);
    assert(xiul_linux_pci_unregister_driver(&driver) == XIUL_OK);
    assert(removed == 1 && device.enabled == 0);
    assert(xiul_device_unregister(&device.device) == XIUL_OK);

    device_b.device.name = "intel-nic0";
    device_b.device.id.bus = XIUL_BUS_PCI;
    device_b.device.id.vendor_id = 0x8086;
    device_b.device.id.device_id = 0x100e;
    assert(xiul_device_count() == 0);
    assert(xiul_device_register(&device_b.device) == XIUL_OK);
    assert(probed_b == 1);
    assert(xiul_linux_pci_get_drvdata(&device_b) == &driver_token_b);
    assert(xiul_linux_pci_unregister_driver(&driver_b) == XIUL_OK);
    assert(removed_b == 1);
    assert(xiul_device_unregister(&device_b.device) == XIUL_OK);
    assert(xiul_linux_pci_driver_count() == 0);

    xiul_linux_pci_test_reset();
    xiul_device_test_reset();
    puts("XIUL Linux PCI adapter tests: PASS");
    return 0;
}
