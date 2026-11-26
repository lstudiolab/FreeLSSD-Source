/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include "xiul/linux_net.h"

static xiul_status_t linux_netdev_transmit(xiul_netif_t *interface,
                                            xiul_net_packet_t *packet)
{
    xiul_linux_net_device_t *device;
    if (interface == 0 || packet == 0) return XIUL_E_INVALID;
    device = (xiul_linux_net_device_t *)interface->driver_context;
    if (device == 0 || !device->registered || device->ndo_start_xmit == 0)
        return XIUL_E_STATE;
    return device->ndo_start_xmit(device, packet);
}

xiul_status_t xiul_linux_netdev_register(xiul_linux_net_device_t *device)
{
    xiul_status_t status;
    if (device == 0 || device->name == 0 || device->name[0] == '\0' ||
        device->mtu == 0 || device->ndo_start_xmit == 0)
        return XIUL_E_INVALID;
    if (device->registered) return XIUL_E_DUPLICATE;

    device->xiul_interface.name = device->name;
    device->xiul_interface.mtu = device->mtu;
    device->xiul_interface.driver_context = device;
    device->xiul_interface.transmit = linux_netdev_transmit;
    status = xiul_netif_register(&device->xiul_interface);
    if (status != XIUL_OK) {
        device->xiul_interface.driver_context = 0;
        device->xiul_interface.transmit = 0;
        return status;
    }
    device->registered = 1;
    return XIUL_OK;
}

xiul_status_t xiul_linux_netdev_unregister(xiul_linux_net_device_t *device)
{
    xiul_status_t status;
    if (device == 0) return XIUL_E_INVALID;
    if (!device->registered) return XIUL_E_STATE;
    status = xiul_netif_unregister(&device->xiul_interface);
    if (status != XIUL_OK) return status;
    device->registered = 0;
    device->xiul_interface.driver_context = 0;
    device->xiul_interface.transmit = 0;
    return XIUL_OK;
}

xiul_status_t xiul_linux_netdev_receive(xiul_linux_net_device_t *device,
                                         xiul_net_packet_t *packet)
{
    if (device == 0 || packet == 0) return XIUL_E_INVALID;
    if (!device->registered) return XIUL_E_STATE;
    return xiul_net_receive(&device->xiul_interface, packet);
}
