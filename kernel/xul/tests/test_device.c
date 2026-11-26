/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include <assert.h>
#include <stdio.h>
#include "xul/device.h"

static int probes;
static int driver_token;
static int removals;
static xul_status_t probe_ok(xul_device_t *device)
{
    ++probes;
    device->driver_data = &driver_token;
    return XUL_OK;
}
static void remove_device(xul_device_t *device)
{
    ++removals;
    device->driver_data = 0;
}
static const xul_device_id_t ids[] = {
    {XUL_BUS_PCI, 0x1234, 0x5678, XUL_ID_ANY, XUL_ID_ANY}
};
static const xul_driver_t driver = {
    "test-pci-driver", ids, 1, probe_ok, remove_device, 0, 0, 0
};

int main(void)
{
    xul_device_t device = {
        "test-device", {XUL_BUS_PCI, 0x1234, 0x5678, 0x02, 0x00},
        0, 0, 0
    };
    xul_device_test_reset();
    probes = removals = 0;

    assert(xul_device_register(&device) == XUL_OK);
    assert(device.bound_driver == 0);
    assert(xul_driver_register(&driver) == XUL_OK);
    assert(device.bound_driver == &driver);
    assert(probes == 1 && xul_device_count() == 1);
    assert(xul_driver_register(&driver) == XUL_E_DUPLICATE);
    assert(xul_driver_unregister(&driver) == XUL_OK);
    assert(device.bound_driver == 0 && removals == 1);
    assert(xul_device_unregister(&device) == XUL_OK);
    assert(xul_device_count() == 0 && xul_driver_count() == 0);
    xul_device_test_reset();
    puts("XUL device registry tests: PASS");
    return 0;
}
