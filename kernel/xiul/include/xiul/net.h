#ifndef XIUL_NET_H
#define XIUL_NET_H
#include <stddef.h>
#include <stdint.h>
#include "xiul/status.h"
#define XIUL_NETIF_MAX 32u

typedef struct {
    void *data;
    size_t length;
    uint16_t protocol;
    uint16_t flags;
} xiul_net_packet_t;

typedef struct xiul_netif xiul_netif_t;
typedef xiul_status_t (*xiul_netif_transmit_fn)(xiul_netif_t *, xiul_net_packet_t *);
typedef xiul_status_t (*xiul_net_input_fn)(xiul_netif_t *, xiul_net_packet_t *);

struct xiul_netif {
    const char *name;
    uint32_t mtu;
    void *driver_context;
    xiul_netif_transmit_fn transmit;
};

xiul_status_t xiul_net_set_input_handler(xiul_net_input_fn handler);
xiul_status_t xiul_netif_register(xiul_netif_t *interface);
xiul_status_t xiul_netif_unregister(xiul_netif_t *interface);
xiul_status_t xiul_net_receive(xiul_netif_t *interface, xiul_net_packet_t *packet);
xiul_status_t xiul_net_transmit(xiul_netif_t *interface, xiul_net_packet_t *packet);
size_t xiul_netif_count(void);
#ifdef XIUL_TESTING
void xiul_net_test_reset(void);
#endif
#endif
