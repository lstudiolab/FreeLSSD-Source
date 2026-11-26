/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#ifndef XIUL_LINUX_NET_H
#define XIUL_LINUX_NET_H

#include <stdint.h>
#include "xiul/net.h"

/*
 * Minimal Linux net_device-facing bridge. A ported Linux driver's
 * ndo_start_xmit implementation is called for BSD-originated egress;
 * received frames are submitted to the BSD network ingress path.
 */
typedef struct xiul_linux_net_device xiul_linux_net_device_t;
typedef xiul_status_t (*xiul_linux_net_xmit_fn)(
    xiul_linux_net_device_t *device, xiul_net_packet_t *packet);

struct xiul_linux_net_device {
    const char *name;
    uint32_t mtu;
    void *private_data;
    xiul_linux_net_xmit_fn ndo_start_xmit;
    xiul_netif_t xiul_interface;
    unsigned int registered;
};

xiul_status_t xiul_linux_netdev_register(xiul_linux_net_device_t *device);
xiul_status_t xiul_linux_netdev_unregister(xiul_linux_net_device_t *device);
xiul_status_t xiul_linux_netdev_receive(xiul_linux_net_device_t *device,
                                         xiul_net_packet_t *packet);

#endif
