/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#ifndef XUL_NET_H
#define XUL_NET_H
#include <stddef.h>
#include <stdint.h>
#include "xul/status.h"
#define XUL_NETIF_MAX 32u
#define XUL_NET_PROTOCOL_MAX 64u

typedef struct {
    void *data;
    size_t length;
    uint16_t protocol;
    uint16_t flags;
} xul_net_packet_t;

typedef struct xul_netif xul_netif_t;
typedef xul_status_t (*xul_netif_transmit_fn)(xul_netif_t *, xul_net_packet_t *);
typedef xul_status_t (*xul_net_input_fn)(xul_netif_t *, xul_net_packet_t *);
typedef xul_status_t (*xul_net_protocol_fn)(xul_netif_t *, xul_net_packet_t *);

struct xul_netif {
    const char *name;
    uint32_t mtu;
    void *driver_context;
    xul_netif_transmit_fn transmit;
};

xul_status_t xul_net_set_input_handler(xul_net_input_fn handler);
xul_status_t xul_net_protocol_register(uint16_t protocol, xul_net_protocol_fn handler);
xul_status_t xul_net_protocol_unregister(uint16_t protocol);
xul_status_t xul_net_protocol_dispatch(xul_netif_t *interface, xul_net_packet_t *packet);
xul_status_t xul_netif_register(xul_netif_t *interface);
xul_status_t xul_netif_unregister(xul_netif_t *interface);
xul_status_t xul_net_receive(xul_netif_t *interface, xul_net_packet_t *packet);
xul_status_t xul_net_transmit(xul_netif_t *interface, xul_net_packet_t *packet);
size_t xul_netif_count(void);
#ifdef XUL_TESTING
void xul_net_test_reset(void);
#endif
#endif
