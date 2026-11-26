/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#ifndef XUL_LINUX_NET_H
#define XUL_LINUX_NET_H

#include <stdint.h>
#include "xul/net.h"

/*
 * Minimal Linux net_device-facing bridge. A ported Linux driver's
 * ndo_start_xmit implementation is called for BSD-originated egress;
 * received frames are submitted to the BSD network ingress path.
 */
typedef struct xul_linux_net_device xul_linux_net_device_t;
typedef xul_status_t (*xul_linux_net_xmit_fn)(
    xul_linux_net_device_t *device, xul_net_packet_t *packet);

struct xul_linux_net_device {
    const char *name;
    uint32_t mtu;
    void *private_data;
    xul_linux_net_xmit_fn ndo_start_xmit;
    xul_netif_t xul_interface;
    unsigned int registered;
};

xul_status_t xul_linux_netdev_register(xul_linux_net_device_t *device);
xul_status_t xul_linux_netdev_unregister(xul_linux_net_device_t *device);
xul_status_t xul_linux_netdev_receive(xul_linux_net_device_t *device,
                                         xul_net_packet_t *packet);

#endif
