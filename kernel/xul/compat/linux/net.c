/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include "xul/linux_net.h"

static xul_status_t linux_netdev_transmit(xul_netif_t *interface,
                                            xul_net_packet_t *packet)
{
    xul_linux_net_device_t *device;
    if (interface == 0 || packet == 0) return XUL_E_INVALID;
    device = (xul_linux_net_device_t *)interface->driver_context;
    if (device == 0 || !device->registered || device->ndo_start_xmit == 0)
        return XUL_E_STATE;
    return device->ndo_start_xmit(device, packet);
}

xul_status_t xul_linux_netdev_register(xul_linux_net_device_t *device)
{
    xul_status_t status;
    if (device == 0 || device->name == 0 || device->name[0] == '\0' ||
        device->mtu == 0 || device->ndo_start_xmit == 0)
        return XUL_E_INVALID;
    if (device->registered) return XUL_E_DUPLICATE;

    device->xul_interface.name = device->name;
    device->xul_interface.mtu = device->mtu;
    device->xul_interface.driver_context = device;
    device->xul_interface.transmit = linux_netdev_transmit;
    status = xul_netif_register(&device->xul_interface);
    if (status != XUL_OK) {
        device->xul_interface.driver_context = 0;
        device->xul_interface.transmit = 0;
        return status;
    }
    device->registered = 1;
    return XUL_OK;
}

xul_status_t xul_linux_netdev_unregister(xul_linux_net_device_t *device)
{
    xul_status_t status;
    if (device == 0) return XUL_E_INVALID;
    if (!device->registered) return XUL_E_STATE;
    status = xul_netif_unregister(&device->xul_interface);
    if (status != XUL_OK) return status;
    device->registered = 0;
    device->xul_interface.driver_context = 0;
    device->xul_interface.transmit = 0;
    return XUL_OK;
}

xul_status_t xul_linux_netdev_receive(xul_linux_net_device_t *device,
                                         xul_net_packet_t *packet)
{
    if (device == 0 || packet == 0) return XUL_E_INVALID;
    if (!device->registered) return XUL_E_STATE;
    return xul_net_receive(&device->xul_interface, packet);
}
