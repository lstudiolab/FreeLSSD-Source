/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include <assert.h>
#include <stdio.h>
#include "xul/linux_pci.h"

static int probed, removed, probed_b, removed_b, driver_token, driver_token_b;
static xul_status_t probe(xul_linux_pci_device_t *device,
                           const xul_linux_pci_id_t *id)
{
    assert(id->vendor == 0x1af4);
    ++probed;
    assert(xul_linux_pci_enable_device(device) == XUL_OK);
    xul_linux_pci_set_drvdata(device, &driver_token);
    return XUL_OK;
}
static void remove_device(xul_linux_pci_device_t *device)
{
    ++removed;
    xul_linux_pci_disable_device(device);
    xul_linux_pci_set_drvdata(device, 0);
}
static xul_status_t probe_b(xul_linux_pci_device_t *device,
                             const xul_linux_pci_id_t *id)
{
    assert(id->vendor == 0x8086);
    ++probed_b;
    xul_linux_pci_set_drvdata(device, &driver_token_b);
    return XUL_OK;
}
static void remove_b(xul_linux_pci_device_t *device)
{
    ++removed_b;
    xul_linux_pci_set_drvdata(device, 0);
}

int main(void)
{
    const xul_linux_pci_id_t ids[] = {
        {0x1af4, 0x1001, XUL_ID_ANY, XUL_ID_ANY}
    };
    const xul_linux_pci_id_t ids_b[] = {
        {0x8086, 0x100e, XUL_ID_ANY, XUL_ID_ANY}
    };
    const xul_linux_pci_driver_t driver = {
        "xul-virtio-block-test", ids, 1, probe, remove_device
    };
    const xul_linux_pci_driver_t driver_b = {
        "xul-intel-nic-test", ids_b, 1, probe_b, remove_b
    };
    xul_linux_pci_device_t device = {0};
    xul_linux_pci_device_t device_b = {0};

    xul_device_test_reset();
    xul_linux_pci_test_reset();
    probed = removed = probed_b = removed_b = 0;

    /* Device-first registration path. */
    device.device.name = "virtio-blk0";
    device.device.id.bus = XUL_BUS_PCI;
    device.device.id.vendor_id = 0x1af4;
    device.device.id.device_id = 0x1001;
    device.bar_start[0] = 0xf0000000u;
    device.bar_length[0] = 4096;
    device.bar_mapping[0] = (volatile void *)0x1000;
    assert(xul_device_register(&device.device) == XUL_OK);
    assert(xul_linux_pci_register_driver(&driver) == XUL_OK);
    assert(probed == 1 && device.enabled == 1);
    assert(xul_linux_pci_get_drvdata(&device) == &driver_token);
    assert(xul_linux_pci_resource_start(&device, 0) == 0xf0000000u);
    assert(xul_linux_pci_resource_length(&device, 0) == 4096);
    assert(xul_linux_pci_iomap(&device, 0) == (volatile void *)0x1000);

    /* Register a second driver, remove the first, then exercise driver-first binding.
       The remaining bridge must retain its stable context address. */
    assert(xul_linux_pci_register_driver(&driver_b) == XUL_OK);
    assert(xul_linux_pci_unregister_driver(&driver) == XUL_OK);
    assert(removed == 1 && device.enabled == 0);
    assert(xul_device_unregister(&device.device) == XUL_OK);

    device_b.device.name = "intel-nic0";
    device_b.device.id.bus = XUL_BUS_PCI;
    device_b.device.id.vendor_id = 0x8086;
    device_b.device.id.device_id = 0x100e;
    assert(xul_device_count() == 0);
    assert(xul_device_register(&device_b.device) == XUL_OK);
    assert(probed_b == 1);
    assert(xul_linux_pci_get_drvdata(&device_b) == &driver_token_b);
    assert(xul_linux_pci_unregister_driver(&driver_b) == XUL_OK);
    assert(removed_b == 1);
    assert(xul_device_unregister(&device_b.device) == XUL_OK);
    assert(xul_linux_pci_driver_count() == 0);

    xul_linux_pci_test_reset();
    xul_device_test_reset();
    puts("XUL Linux PCI adapter tests: PASS");
    return 0;
}
