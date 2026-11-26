/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include <assert.h>
#include <stdio.h>
#include "xiul/linux_net.h"

static unsigned int transmitted;
static unsigned int received;

static xiul_status_t linux_xmit(xiul_linux_net_device_t *device,
                                 xiul_net_packet_t *packet)
{
    assert(device->private_data == &transmitted);
    assert(packet->data != 0 && packet->length != 0);
    ++transmitted;
    return XIUL_OK;
}

static xiul_status_t ipv4_handler(xiul_netif_t *interface,
                                   xiul_net_packet_t *packet)
{
    assert(interface->driver_context != 0);
    assert(packet->protocol == 0x0800);
    ++received;
    return XIUL_OK;
}

int main(void)
{
    char bytes[128];
    xiul_net_packet_t packet = {bytes, sizeof(bytes), 0x0800, 0};
    xiul_linux_net_device_t device = {
        .name = "xiul-test0",
        .mtu = 1500,
        .private_data = &transmitted,
        .ndo_start_xmit = linux_xmit
    };

    xiul_net_test_reset();
    assert(xiul_linux_netdev_register(&device) == XIUL_OK);
    assert(xiul_linux_netdev_register(&device) == XIUL_E_DUPLICATE);
    assert(xiul_net_protocol_register(0x0800, ipv4_handler) == XIUL_OK);

    /* BSD egress reaches the Linux driver's ndo_start_xmit callback. */
    assert(xiul_net_transmit(&device.xiul_interface, &packet) == XIUL_OK);
    assert(transmitted == 1);

    /* Linux-driver ingress enters the BSD protocol dispatch path. */
    assert(xiul_linux_netdev_receive(&device, &packet) == XIUL_OK);
    assert(received == 1);

    assert(xiul_net_protocol_unregister(0x0800) == XIUL_OK);
    assert(xiul_linux_netdev_unregister(&device) == XIUL_OK);
    assert(xiul_linux_netdev_receive(&device, &packet) == XIUL_E_STATE);

    xiul_net_test_reset();
    puts("XIUL Linux netdev/BSD network bridge tests: PASS");
    return 0;
}
