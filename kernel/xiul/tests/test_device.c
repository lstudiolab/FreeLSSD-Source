#include <assert.h>
#include <stdio.h>
#include "xiul/device.h"

static int probes;
static int removals;
static xiul_status_t probe_ok(xiul_device_t *device)
{
    ++probes;
    device->driver_data = (void *)1;
    return XIUL_OK;
}
static void remove_device(xiul_device_t *device)
{
    ++removals;
    device->driver_data = 0;
}
static const xiul_device_id_t ids[] = {
    {XIUL_BUS_PCI, 0x1234, 0x5678, XIUL_ID_ANY, XIUL_ID_ANY}
};
static const xiul_driver_t driver = {
    "test-pci-driver", ids, 1, probe_ok, remove_device
};

int main(void)
{
    xiul_device_t device = {
        "test-device", {XIUL_BUS_PCI, 0x1234, 0x5678, 0x02, 0x00},
        0, 0, 0
    };
    xiul_device_test_reset();
    probes = removals = 0;

    assert(xiul_device_register(&device) == XIUL_OK);
    assert(device.bound_driver == 0);
    assert(xiul_driver_register(&driver) == XIUL_OK);
    assert(device.bound_driver == &driver);
    assert(probes == 1 && xiul_device_count() == 1);
    assert(xiul_driver_register(&driver) == XIUL_E_DUPLICATE);
    assert(xiul_driver_unregister(&driver) == XIUL_OK);
    assert(device.bound_driver == 0 && removals == 1);
    assert(xiul_device_unregister(&device) == XIUL_OK);
    assert(xiul_device_count() == 0 && xiul_driver_count() == 0);
    xiul_device_test_reset();
    puts("XIUL device registry tests: PASS");
    return 0;
}
