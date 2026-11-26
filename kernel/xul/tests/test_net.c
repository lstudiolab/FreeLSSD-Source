/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include <assert.h>
#include <stdio.h>
#include "xul/net.h"
#include "xul/nke.h"
static unsigned int received, transmitted, protocol_packets;
static xul_status_t input(xul_netif_t *n, xul_net_packet_t *p)
{ (void)n; ++received; return p->protocol == 0x0800 ? XUL_OK : XUL_E_UNSUPPORTED; }
static xul_status_t transmit(xul_netif_t *n, xul_net_packet_t *p)
{ (void)n; (void)p; ++transmitted; return XUL_OK; }
static xul_status_t ipv4_input(xul_netif_t *n, xul_net_packet_t *p)
{ (void)n; ++protocol_packets; return p->protocol == 0x0800 ? XUL_OK : XUL_E_INVALID; }
static xul_nke_action_t drop_ipv6(xul_nke_packet_t *p, void *context)
{ (void)context; return p->protocol == 0x86dd ? XUL_NKE_DROP : XUL_NKE_PASS; }

int main(void)
{
    char bytes[64];
    xul_net_packet_t packet = {bytes, sizeof(bytes), 0x0800, 0};
    xul_netif_t nic = {"eth0", 1500, 0, transmit};
    const xul_nke_hook_t filter = {"test-ipv6-drop", 10, drop_ipv6, 0};
    xul_net_test_reset();
    xul_nke_test_reset();
    assert(xul_netif_register(&nic) == XUL_OK);
    assert(xul_netif_register(&nic) == XUL_E_DUPLICATE);
    assert(xul_net_set_input_handler(input) == XUL_OK);
    assert(xul_net_receive(&nic, &packet) == XUL_OK && received == 1);
    assert(xul_net_transmit(&nic, &packet) == XUL_OK && transmitted == 1);
    packet.length = 1501;
    assert(xul_net_transmit(&nic, &packet) == XUL_E_INVALID);
    assert(xul_net_set_input_handler(0) == XUL_OK);
    assert(xul_net_protocol_register(0x0800, ipv4_input) == XUL_OK);
    assert(xul_net_protocol_register(0x0800, ipv4_input) == XUL_E_DUPLICATE);
    assert(xul_net_receive(&nic, &packet) == XUL_OK && protocol_packets == 1);
    assert(xul_net_protocol_unregister(0x0800) == XUL_OK);
    assert(xul_net_receive(&nic, &packet) == XUL_E_UNSUPPORTED);
    packet.length = sizeof(bytes);
    packet.protocol = 0x86dd;
    assert(xul_nke_register(&filter) == XUL_OK);
    assert(xul_net_receive(&nic, &packet) == XUL_E_DROPPED && received == 1);
    assert(xul_net_transmit(&nic, &packet) == XUL_E_DROPPED && transmitted == 1);
    assert(xul_nke_unregister(&filter) == XUL_OK);
    assert(xul_netif_unregister(&nic) == XUL_OK);
    assert(xul_net_receive(&nic, &packet) == XUL_E_NOT_FOUND);
    xul_net_test_reset();
    xul_nke_test_reset();
    puts("XUL network/NKE integration tests: PASS");
    return 0;
}
