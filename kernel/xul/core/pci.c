/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include "xul/pci.h"
#include "xul/device.h"
#include "xul/linux_pci.h"

typedef struct {
    xul_linux_pci_device_t pci;
    const xul_pci_host_t *host;
} xul_pci_slot_t;

static xul_pci_slot_t slots[XUL_PCI_MAX_ENUMERATED_DEVICES];
static size_t slot_count;

static xul_status_t read32(const xul_pci_host_t *host, uint8_t bus,
                            uint8_t device, uint8_t function,
                            uint16_t offset, uint32_t *value)
{
    if ((offset & 3u) != 0 || offset >= XUL_PCI_CONFIG_SPACE_SIZE)
        return XUL_E_INVALID;
    return host->read32(host->context, bus, device, function, offset, value);
}

static xul_status_t read16(const xul_pci_host_t *host, uint8_t bus,
                            uint8_t device, uint8_t function,
                            uint16_t offset, uint16_t *value)
{
    uint32_t word;
    xul_status_t status;
    if ((offset & 1u) != 0 || offset >= XUL_PCI_CONFIG_SPACE_SIZE)
        return XUL_E_INVALID;
    status = read32(host, bus, device, function, (uint16_t)(offset & ~3u), &word);
    if (status == XUL_OK)
        *value = (uint16_t)(word >> ((offset & 2u) * 8u));
    return status;
}

static xul_status_t write16(const xul_pci_host_t *host, uint8_t bus,
                             uint8_t device, uint8_t function,
                             uint16_t offset, uint16_t value)
{
    return host->write16(host->context, bus, device, function, offset, value);
}

static void make_name(char *output, uint8_t bus, uint8_t device, uint8_t function)
{
    static const char hex[] = "0123456789abcdef";
    output[0] = 'p'; output[1] = 'c'; output[2] = 'i';
    output[3] = hex[(bus >> 4) & 0xf]; output[4] = hex[bus & 0xf];
    output[5] = ':';
    output[6] = hex[(device >> 4) & 0xf]; output[7] = hex[device & 0xf];
    output[8] = '.';
    output[9] = (char)('0' + function);
    output[10] = '\0';
}

static void release_slot(xul_pci_slot_t *slot)
{
    unsigned int i;
    if (slot->pci.device.bound_driver != 0)
        (void)xul_device_unregister(&slot->pci.device);
    else if (slot->pci.device.name != 0)
        (void)xul_device_unregister(&slot->pci.device);
    xul_linux_pci_clear_master(&slot->pci);
    xul_linux_pci_disable_device(&slot->pci);
    if (slot->host != 0 && slot->host->unmap_mmio != 0) {
        for (i = 0; i < XUL_LINUX_PCI_BAR_COUNT; ++i) {
            if (slot->pci.bar_mapping[i] != 0 && slot->pci.bar_length[i] <= (uint64_t)SIZE_MAX) {
                slot->host->unmap_mmio(slot->host->context,
                    slot->pci.bar_mapping[i], (size_t)slot->pci.bar_length[i]);
            }
        }
    }
    slot->host = 0;
    slot->pci.device.name = 0;
}

static xul_status_t probe_bar(const xul_pci_host_t *host,
                               uint8_t bus, uint8_t device, uint8_t function,
                               unsigned int bar, xul_linux_pci_device_t *pci)
{
    uint16_t offset = (uint16_t)(0x10u + bar * 4u);
    uint16_t command;
    uint32_t original_low, mask_low, original_high = 0, mask_high = 0;
    uint64_t base, length;
    unsigned int type;
    xul_status_t status;

    status = read32(host, bus, device, function, offset, &original_low);
    if (status != XUL_OK || original_low == 0 || original_low == UINT32_MAX)
        return status;

    status = read16(host, bus, device, function, 0x04u, &command);
    if (status != XUL_OK) return status;

    if ((original_low & 1u) != 0) {
        pci->bar_flags[bar] = XUL_LINUX_PCI_BAR_IO;
        status = write16(host, bus, device, function, 0x04u,
                         (uint16_t)(command & ~UINT16_C(7)));
        if (status != XUL_OK) return status;
        status = host->write32(host->context, bus, device, function,
                               offset, UINT32_MAX);
        if (status == XUL_OK)
            status = read32(host, bus, device, function, offset, &mask_low);
        (void)host->write32(host->context, bus, device, function, offset, original_low);
        (void)write16(host, bus, device, function, 0x04u, command);
        if (status != XUL_OK) return status;
        base = original_low & ~UINT32_C(3);
        mask_low &= ~UINT32_C(3);
        if (mask_low != 0) {
            length = (uint32_t)(~mask_low + 1u);
            pci->bar_flags[bar] = XUL_LINUX_PCI_BAR_IO;
            pci->bar_start[bar] = base;
            pci->bar_length[bar] = length;
        }
        return XUL_OK;
    }

    type = (original_low >> 1) & 3u;
    if (type == 3u) return XUL_E_UNSUPPORTED;
    if (type == 2u) {
        if (bar + 1u >= XUL_LINUX_PCI_BAR_COUNT) return XUL_E_INVALID;
        status = read32(host, bus, device, function, (uint16_t)(offset + 4u), &original_high);
        if (status != XUL_OK) return status;
    }

    status = write16(host, bus, device, function, 0x04u,
                     (uint16_t)(command & ~UINT16_C(7)));
    if (status != XUL_OK) return status;
    status = host->write32(host->context, bus, device, function, offset, UINT32_MAX);
    if (status == XUL_OK && type == 2u)
        status = host->write32(host->context, bus, device, function,
                               (uint16_t)(offset + 4u), UINT32_MAX);
    if (status == XUL_OK)
        status = read32(host, bus, device, function, offset, &mask_low);
    if (status == XUL_OK && type == 2u)
        status = read32(host, bus, device, function,
                        (uint16_t)(offset + 4u), &mask_high);

    (void)host->write32(host->context, bus, device, function, offset, original_low);
    if (type == 2u)
        (void)host->write32(host->context, bus, device, function,
                            (uint16_t)(offset + 4u), original_high);
    (void)write16(host, bus, device, function, 0x04u, command);
    if (status != XUL_OK) return status;

    if (type == 2u) {
        base = ((uint64_t)original_high << 32) | (original_low & ~UINT32_C(0xf));
        {
            uint64_t mask = ((uint64_t)mask_high << 32) |
                            (mask_low & ~UINT32_C(0xf));
            if (mask == 0) return XUL_OK;
            length = (~mask) + 1u;
        }
    } else {
        uint32_t mask = mask_low & ~UINT32_C(0xf);
        base = original_low & ~UINT32_C(0xf);
        if (mask == 0) return XUL_OK;
        length = (uint32_t)(~mask + 1u);
    }
    if (length == 0) return XUL_OK;
    pci->bar_flags[bar] = XUL_LINUX_PCI_BAR_MEMORY;
    if ((original_low & 8u) != 0)
        pci->bar_flags[bar] |= XUL_LINUX_PCI_BAR_PREFETCHABLE;
    if (type == 2u)
        pci->bar_flags[bar] |= XUL_LINUX_PCI_BAR_64BIT;
    pci->bar_start[bar] = base;
    pci->bar_length[bar] = length;
    if (host->map_mmio != 0 && length <= (uint64_t)SIZE_MAX)
        pci->bar_mapping[bar] = host->map_mmio(host->context, base, (size_t)length);
    return XUL_OK;
}

static xul_status_t enumerate_function(const xul_pci_host_t *host,
                                        uint8_t bus, uint8_t device,
                                        uint8_t function, size_t *found)
{
    uint32_t identity, class_revision, header_register, interrupt_register;
    uint16_t command;
    unsigned int bar, bar_limit;
    xul_status_t status;
    xul_pci_slot_t *slot;
    uint16_t vendor, product;

    status = read32(host, bus, device, function, 0x00u, &identity);
    if (status != XUL_OK) return status;
    vendor = (uint16_t)identity;
    product = (uint16_t)(identity >> 16);
    if (vendor == UINT16_MAX || vendor == 0) return XUL_E_NOT_FOUND;
    if (slot_count == XUL_PCI_MAX_ENUMERATED_DEVICES) return XUL_E_CAPACITY;
    status = read32(host, bus, device, function, 0x08u, &class_revision);
    if (status != XUL_OK) return status;
    status = read32(host, bus, device, function, 0x0cu, &header_register);
    if (status != XUL_OK) return status;
    status = read32(host, bus, device, function, 0x3cu, &interrupt_register);
    if (status != XUL_OK) return status;
    status = read16(host, bus, device, function, 0x04u, &command);
    if (status != XUL_OK) return status;

    slot = &slots[slot_count];
    slot->host = host;
    slot->pci.device.id.bus = XUL_BUS_PCI;
    slot->pci.device.id.vendor_id = vendor;
    slot->pci.device.id.device_id = product;
    slot->pci.device.id.class_id = (uint16_t)(class_revision >> 24);
    slot->pci.device.id.subclass_id = (uint16_t)((class_revision >> 16) & 0xffu);
    slot->pci.bus_number = bus;
    slot->pci.device_number = device;
    slot->pci.function_number = function;
    slot->pci.irq = interrupt_register & 0xffu;
    slot->pci.host = host;
    slot->pci.original_command = command;
    make_name(slot->pci.name_storage, bus, device, function);
    slot->pci.device.name = slot->pci.name_storage;

    bar_limit = ((header_register >> 16) & 0x7fu) == 0 ? 6u : 2u;
    for (bar = 0; bar < bar_limit; ++bar) {
        uint32_t bar_low;
        status = read32(host, bus, device, function,
                        (uint16_t)(0x10u + bar * 4u), &bar_low);
        if (status != XUL_OK) break;
        status = probe_bar(host, bus, device, function, bar, &slot->pci);
        if (status == XUL_E_UNSUPPORTED) { status = XUL_OK; continue; }
        if (status != XUL_OK) break;
        if ((bar_low & 1u) == 0 && ((bar_low >> 1) & 3u) == 2u) ++bar;
    }
    (void)write16(host, bus, device, function, 0x04u, command);
    if (status != XUL_OK) {
        release_slot(slot);
        return status;
    }
    status = xul_device_register(&slot->pci.device);
    if (status != XUL_OK) {
        unsigned int i;
        if (host->unmap_mmio != 0) {
            for (i = 0; i < XUL_LINUX_PCI_BAR_COUNT; ++i)
                if (slot->pci.bar_mapping[i] != 0 &&
                    slot->pci.bar_length[i] <= (uint64_t)SIZE_MAX)
                    host->unmap_mmio(host->context, slot->pci.bar_mapping[i],
                                     (size_t)slot->pci.bar_length[i]);
        }
        slot->host = 0;
        slot->pci.device.name = 0;
        return status;
    }
    ++slot_count;
    ++*found;
    return XUL_OK;
}

xul_status_t xul_pci_enumerate(const xul_pci_host_t *host, size_t *found)
{
    uint16_t bus;
    size_t count = 0;
    xul_status_t status;
    if (host == 0 || found == 0 || host->read16 == 0 || host->write16 == 0 ||
        host->read32 == 0 || host->write32 == 0 ||
        host->first_bus > host->last_bus || slot_count != 0)
        return XUL_E_INVALID;
    *found = 0;
    for (bus = host->first_bus; bus <= host->last_bus; ++bus) {
        uint8_t device;
        for (device = 0; device < 32u; ++device) {
            uint32_t header;
            uint16_t vendor;
            status = read16(host, (uint8_t)bus, device, 0, 0x00u, &vendor);
            if (status != XUL_OK || vendor == UINT16_MAX || vendor == 0) continue;
            status = read32(host, (uint8_t)bus, device, 0, 0x0cu, &header);
            if (status != XUL_OK) continue;
            status = enumerate_function(host, (uint8_t)bus, device, 0, &count);
            if (status != XUL_OK && status != XUL_E_NOT_FOUND) goto fail;
            if (((header >> 16) & 0x80u) != 0) {
                uint8_t function;
                for (function = 1; function < 8u; ++function) {
                    status = enumerate_function(host, (uint8_t)bus, device,
                                                 function, &count);
                    if (status != XUL_OK && status != XUL_E_NOT_FOUND) goto fail;
                }
            }
        }
        if (bus == UINT8_MAX) break;
    }
    *found = count;
    return XUL_OK;
fail:
    (void)xul_pci_remove_all();
    return status;
}

xul_status_t xul_pci_remove_all(void)
{
    while (slot_count != 0) {
        xul_pci_slot_t *slot = &slots[--slot_count];
        release_slot(slot);
    }
    return XUL_OK;
}

size_t xul_pci_device_count(void) { return slot_count; }

#ifdef XUL_TESTING
void xul_pci_test_reset(void)
{
    (void)xul_pci_remove_all();
}
#endif
