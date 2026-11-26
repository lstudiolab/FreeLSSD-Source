/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include "xul/net.h"
#include "xul/nke.h"

static xul_netif_t *interfaces[XUL_NETIF_MAX];
static size_t interface_count;
static xul_net_input_fn input_handler;
typedef struct { uint16_t protocol; xul_net_protocol_fn handler; } xul_protocol_entry_t;
static xul_protocol_entry_t protocols[XUL_NET_PROTOCOL_MAX];
static size_t protocol_count;

static int name_equal(const char *a, const char *b)
{
    size_t i = 0;
    if (a == 0 || b == 0) return 0;
    while (a[i] && b[i]) { if (a[i] != b[i]) return 0; ++i; }
    return a[i] == b[i];
}

static int is_registered(const xul_netif_t *interface)
{
    size_t i;
    for (i = 0; i < interface_count; ++i)
        if (interfaces[i] == interface) return 1;
    return 0;
}

/*
 * NKE is on the actual BSD network ingress/egress path. Hooks receive a
 * temporary packet descriptor; edits are copied back only when the packet
 * is accepted. A hook may therefore rewrite payload, length or protocol.
 */
static xul_status_t apply_nke(xul_net_packet_t *packet)
{
    xul_nke_packet_t extension_packet;
    xul_nke_action_t action;
    extension_packet.data = packet->data;
    extension_packet.length = packet->length;
    extension_packet.protocol = packet->protocol;
    action = xul_nke_process(&extension_packet);
    if (action == XUL_NKE_DROP) return XUL_E_DROPPED;
    if (extension_packet.data == 0 || extension_packet.length == 0)
        return XUL_E_INVALID;
    packet->data = extension_packet.data;
    packet->length = extension_packet.length;
    packet->protocol = extension_packet.protocol;
    return XUL_OK;
}

xul_status_t xul_net_set_input_handler(xul_net_input_fn handler)
{
    input_handler = handler;
    return XUL_OK;
}

xul_status_t xul_net_protocol_register(uint16_t protocol, xul_net_protocol_fn handler)
{
    size_t i;
    if (protocol == 0 || handler == 0) return XUL_E_INVALID;
    for (i = 0; i < protocol_count; ++i)
        if (protocols[i].protocol == protocol) return XUL_E_DUPLICATE;
    if (protocol_count == XUL_NET_PROTOCOL_MAX) return XUL_E_CAPACITY;
    protocols[protocol_count].protocol = protocol;
    protocols[protocol_count].handler = handler;
    ++protocol_count;
    return XUL_OK;
}

xul_status_t xul_net_protocol_unregister(uint16_t protocol)
{
    size_t i;
    for (i = 0; i < protocol_count; ++i) {
        if (protocols[i].protocol != protocol) continue;
        for (; i + 1 < protocol_count; ++i) protocols[i] = protocols[i + 1];
        --protocol_count;
        protocols[protocol_count].protocol = 0;
        protocols[protocol_count].handler = 0;
        return XUL_OK;
    }
    return XUL_E_NOT_FOUND;
}

xul_status_t xul_net_protocol_dispatch(xul_netif_t *interface,
                                          xul_net_packet_t *packet)
{
    size_t i;
    if (interface == 0 || packet == 0 || packet->data == 0 || packet->length == 0)
        return XUL_E_INVALID;
    if (!is_registered(interface)) return XUL_E_NOT_FOUND;
    for (i = 0; i < protocol_count; ++i)
        if (protocols[i].protocol == packet->protocol)
            return protocols[i].handler(interface, packet);
    return XUL_E_UNSUPPORTED;
}

xul_status_t xul_netif_register(xul_netif_t *interface)
{
    size_t i;
    if (interface == 0 || interface->name == 0 || interface->name[0] == '\0' ||
        interface->mtu == 0 || interface->transmit == 0) return XUL_E_INVALID;
    for (i = 0; i < interface_count; ++i) {
        if (interfaces[i] == interface ||
            name_equal(interfaces[i]->name, interface->name)) return XUL_E_DUPLICATE;
    }
    if (interface_count == XUL_NETIF_MAX) return XUL_E_CAPACITY;
    interfaces[interface_count++] = interface;
    return XUL_OK;
}

xul_status_t xul_netif_unregister(xul_netif_t *interface)
{
    size_t i;
    if (interface == 0) return XUL_E_INVALID;
    for (i = 0; i < interface_count; ++i) {
        if (interfaces[i] != interface) continue;
        for (; i + 1 < interface_count; ++i) interfaces[i] = interfaces[i + 1];
        interfaces[--interface_count] = 0;
        return XUL_OK;
    }
    return XUL_E_NOT_FOUND;
}

xul_status_t xul_net_receive(xul_netif_t *interface, xul_net_packet_t *packet)
{
    xul_status_t status;
    if (interface == 0 || packet == 0 || packet->data == 0 || packet->length == 0)
        return XUL_E_INVALID;
    if (!is_registered(interface)) return XUL_E_NOT_FOUND;
    status = apply_nke(packet);
    if (status != XUL_OK) return status;
    if (input_handler != 0) return input_handler(interface, packet);
    return xul_net_protocol_dispatch(interface, packet);
}

xul_status_t xul_net_transmit(xul_netif_t *interface, xul_net_packet_t *packet)
{
    xul_status_t status;
    if (interface == 0 || packet == 0 || packet->data == 0 || packet->length == 0)
        return XUL_E_INVALID;
    if (!is_registered(interface)) return XUL_E_NOT_FOUND;
    status = apply_nke(packet);
    if (status != XUL_OK) return status;
    if (packet->length > interface->mtu) return XUL_E_INVALID;
    return interface->transmit(interface, packet);
}

size_t xul_netif_count(void) { return interface_count; }

#ifdef XUL_TESTING
void xul_net_test_reset(void)
{
    size_t i;
    for (i = 0; i < interface_count; ++i) interfaces[i] = 0;
    interface_count = 0;
    input_handler = 0;
    for (i = 0; i < protocol_count; ++i) {
        protocols[i].protocol = 0;
        protocols[i].handler = 0;
    }
    protocol_count = 0;
}
#endif
