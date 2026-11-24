#include "xiul/net.h"

static xiul_netif_t *interfaces[XIUL_NETIF_MAX];
static size_t interface_count;
static xiul_net_input_fn input_handler;
typedef struct { uint16_t protocol; xiul_net_protocol_fn handler; } xiul_protocol_entry_t;
static xiul_protocol_entry_t protocols[XIUL_NET_PROTOCOL_MAX];
static size_t protocol_count;

static int name_equal(const char *a, const char *b)
{
    size_t i = 0;
    if (a == 0 || b == 0) return 0;
    while (a[i] && b[i]) { if (a[i] != b[i]) return 0; ++i; }
    return a[i] == b[i];
}

static int is_registered(const xiul_netif_t *interface)
{
    size_t i;
    for (i = 0; i < interface_count; ++i)
        if (interfaces[i] == interface) return 1;
    return 0;
}

xiul_status_t xiul_net_set_input_handler(xiul_net_input_fn handler)
{
    input_handler = handler;
    return XIUL_OK;
}

xiul_status_t xiul_net_protocol_register(uint16_t protocol, xiul_net_protocol_fn handler)
{
    size_t i;
    if (protocol == 0 || handler == 0) return XIUL_E_INVALID;
    for (i = 0; i < protocol_count; ++i)
        if (protocols[i].protocol == protocol) return XIUL_E_DUPLICATE;
    if (protocol_count == XIUL_NET_PROTOCOL_MAX) return XIUL_E_CAPACITY;
    protocols[protocol_count].protocol = protocol;
    protocols[protocol_count].handler = handler;
    ++protocol_count;
    return XIUL_OK;
}

xiul_status_t xiul_net_protocol_unregister(uint16_t protocol)
{
    size_t i;
    for (i = 0; i < protocol_count; ++i) {
        if (protocols[i].protocol != protocol) continue;
        for (; i + 1 < protocol_count; ++i) protocols[i] = protocols[i + 1];
        --protocol_count;
        protocols[protocol_count].protocol = 0;
        protocols[protocol_count].handler = 0;
        return XIUL_OK;
    }
    return XIUL_E_NOT_FOUND;
}

xiul_status_t xiul_net_protocol_dispatch(xiul_netif_t *interface,
                                          xiul_net_packet_t *packet)
{
    size_t i;
    if (interface == 0 || packet == 0 || packet->data == 0 || packet->length == 0)
        return XIUL_E_INVALID;
    if (!is_registered(interface)) return XIUL_E_NOT_FOUND;
    for (i = 0; i < protocol_count; ++i)
        if (protocols[i].protocol == packet->protocol)
            return protocols[i].handler(interface, packet);
    return XIUL_E_UNSUPPORTED;
}

xiul_status_t xiul_netif_register(xiul_netif_t *interface)
{
    size_t i;
    if (interface == 0 || interface->name == 0 || interface->name[0] == '\0' ||
        interface->mtu == 0 || interface->transmit == 0) return XIUL_E_INVALID;
    for (i = 0; i < interface_count; ++i) {
        if (interfaces[i] == interface ||
            name_equal(interfaces[i]->name, interface->name)) return XIUL_E_DUPLICATE;
    }
    if (interface_count == XIUL_NETIF_MAX) return XIUL_E_CAPACITY;
    interfaces[interface_count++] = interface;
    return XIUL_OK;
}

xiul_status_t xiul_netif_unregister(xiul_netif_t *interface)
{
    size_t i;
    if (interface == 0) return XIUL_E_INVALID;
    for (i = 0; i < interface_count; ++i) {
        if (interfaces[i] != interface) continue;
        for (; i + 1 < interface_count; ++i) interfaces[i] = interfaces[i + 1];
        interfaces[--interface_count] = 0;
        return XIUL_OK;
    }
    return XIUL_E_NOT_FOUND;
}

xiul_status_t xiul_net_receive(xiul_netif_t *interface, xiul_net_packet_t *packet)
{
    if (interface == 0 || packet == 0 || packet->data == 0 || packet->length == 0)
        return XIUL_E_INVALID;
    if (!is_registered(interface)) return XIUL_E_NOT_FOUND;
    if (input_handler != 0) return input_handler(interface, packet);
    return xiul_net_protocol_dispatch(interface, packet);
}

xiul_status_t xiul_net_transmit(xiul_netif_t *interface, xiul_net_packet_t *packet)
{
    if (interface == 0 || packet == 0 || packet->data == 0 || packet->length == 0)
        return XIUL_E_INVALID;
    if (!is_registered(interface)) return XIUL_E_NOT_FOUND;
    if (packet->length > interface->mtu) return XIUL_E_INVALID;
    return interface->transmit(interface, packet);
}

size_t xiul_netif_count(void) { return interface_count; }

#ifdef XIUL_TESTING
void xiul_net_test_reset(void)
{
    size_t i;
    for (i = 0; i < interface_count; ++i) interfaces[i] = 0;
    interface_count = 0;
    input_handler = 0;
    for (i = 0; i < protocol_count; ++i) {\n        protocols[i].protocol = 0;\n        protocols[i].handler = 0;\n    }\n    protocol_count = 0;
}
#endif
