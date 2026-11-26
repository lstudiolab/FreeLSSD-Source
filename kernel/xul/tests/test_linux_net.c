/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include <assert.h>
#include <stdio.h>
#include "xul/linux_net.h"
#include "xul/nke.h"

static unsigned int transmitted;
static unsigned int received;

static xul_nke_action_t drop_ipv6(xul_nke_packet_t *packet, void *context)
{
    (void)context;
    return packet->protocol == 0x86dd ? XUL_NKE_DROP : XUL_NKE_PASS;
}

static xul_status_t linux_xmit(xul_linux_net_device_t *device,
                                 xul_net_packet_t *packet)
{
    assert(device->private_data == &transmitted);
    assert(packet->data != 0 && packet->length != 0);
    ++transmitted;
    return XUL_OK;
}

static xul_status_t ipv4_handler(xul_netif_t *interface,
                                   xul_net_packet_t *packet)
{
    assert(interface->driver_context != 0);
    assert(packet->protocol == 0x0800);
    ++received;
    return XUL_OK;
}

int main(void)
{
    char bytes[128];
    xul_net_packet_t packet = {bytes, sizeof(bytes), 0x0800, 0};
    const xul_nke_hook_t filter = {"linux-net-ipv6-filter", 10, drop_ipv6, 0};
    xul_linux_net_device_t device = {
        .name = "xul-test0",
        .mtu = 1500,
        .private_data = &transmitted,
        .ndo_start_xmit = linux_xmit
    };

    xul_net_test_reset();
    xul_nke_test_reset();
    assert(xul_linux_netdev_register(&device) == XUL_OK);
    assert(xul_linux_netdev_register(&device) == XUL_E_DUPLICATE);
    assert(xul_net_protocol_register(0x0800, ipv4_handler) == XUL_OK);

    /* BSD egress reaches the Linux driver's ndo_start_xmit callback. */
    assert(xul_net_transmit(&device.xul_interface, &packet) == XUL_OK);
    assert(transmitted == 1);

    /* Linux-driver ingress enters the BSD protocol dispatch path. */
    assert(xul_linux_netdev_receive(&device, &packet) == XUL_OK);
    assert(received == 1);

    /* The NKE filter is part of the Linux -> BSD ingress and egress path. */
    packet.protocol = 0x86dd;
    assert(xul_nke_register(&filter) == XUL_OK);
    assert(xul_linux_netdev_receive(&device, &packet) == XUL_E_DROPPED);
    assert(xul_net_transmit(&device.xul_interface, &packet) == XUL_E_DROPPED);
    assert(received == 1 && transmitted == 1);
    assert(xul_nke_unregister(&filter) == XUL_OK);

    assert(xul_net_protocol_unregister(0x0800) == XUL_OK);
    assert(xul_linux_netdev_unregister(&device) == XUL_OK);
    assert(xul_linux_netdev_receive(&device, &packet) == XUL_E_STATE);

    xul_net_test_reset();
    xul_nke_test_reset();
    puts("XUL Linux netdev/BSD/NKE integration tests: PASS");
    return 0;
}
