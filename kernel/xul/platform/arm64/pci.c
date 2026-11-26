/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include "xul/pci_host.h"

static volatile uint8_t *ecam_address(xul_arm64_pci_ecam_context_t *context,
                                      uint8_t bus, uint8_t device,
                                      uint8_t function, uint16_t offset)
{
    uint64_t bus_offset;
    if (context == 0 || context->ecam_virtual_base == 0 ||
        device >= 32u || function >= 8u || offset >= 256u)
        return 0;
    if (bus < context->first_bus || bus > context->last_bus) return 0;
    bus_offset = ((uint64_t)(bus - context->first_bus) << 20) |
                 ((uint64_t)device << 15) |
                 ((uint64_t)function << 12) | offset;
    return context->ecam_virtual_base + bus_offset;
}

static xul_status_t arm64_read32(void *opaque, uint8_t bus, uint8_t device,
                                  uint8_t function, uint16_t offset, uint32_t *value)
{
    xul_arm64_pci_ecam_context_t *context = opaque;
    volatile uint32_t *address;
    if (value == 0 || (offset & 3u) != 0) return XUL_E_INVALID;
    address = (volatile uint32_t *)ecam_address(context, bus, device, function, offset);
    if (address == 0) return XUL_E_INVALID;
    __asm__ volatile ("dmb osh" : : : "memory");
    *value = *address;
    __asm__ volatile ("dmb osh" : : : "memory");
    return XUL_OK;
}

static xul_status_t arm64_write32(void *opaque, uint8_t bus, uint8_t device,
                                   uint8_t function, uint16_t offset, uint32_t value)
{
    xul_arm64_pci_ecam_context_t *context = opaque;
    volatile uint32_t *address;
    if ((offset & 3u) != 0) return XUL_E_INVALID;
    address = (volatile uint32_t *)ecam_address(context, bus, device, function, offset);
    if (address == 0) return XUL_E_INVALID;
    __asm__ volatile ("dmb osh" : : : "memory");
    *address = value;
    __asm__ volatile ("dmb osh" : : : "memory");
    return XUL_OK;
}

static xul_status_t arm64_read16(void *opaque, uint8_t bus, uint8_t device,
                                  uint8_t function, uint16_t offset, uint16_t *value)
{
    xul_arm64_pci_ecam_context_t *context = opaque;
    volatile uint16_t *address;
    if (value == 0 || (offset & 1u) != 0) return XUL_E_INVALID;
    address = (volatile uint16_t *)ecam_address(context, bus, device, function, offset);
    if (address == 0) return XUL_E_INVALID;
    __asm__ volatile ("dmb osh" : : : "memory");
    *value = *address;
    __asm__ volatile ("dmb osh" : : : "memory");
    return XUL_OK;
}

static xul_status_t arm64_write16(void *opaque, uint8_t bus, uint8_t device,
                                   uint8_t function, uint16_t offset, uint16_t value)
{
    xul_arm64_pci_ecam_context_t *context = opaque;
    volatile uint16_t *address;
    if ((offset & 1u) != 0) return XUL_E_INVALID;
    address = (volatile uint16_t *)ecam_address(context, bus, device, function, offset);
    if (address == 0) return XUL_E_INVALID;
    __asm__ volatile ("dmb osh" : : : "memory");
    *address = value;
    __asm__ volatile ("dmb osh" : : : "memory");
    return XUL_OK;
}

xul_status_t xul_pci_host_init_arm64_ecam(
    xul_pci_host_t *host, xul_arm64_pci_ecam_context_t *context,
    volatile void *ecam_virtual_base, uint16_t segment,
    uint8_t first_bus, uint8_t last_bus)
{
    if (host == 0 || context == 0 || ecam_virtual_base == 0 ||
        first_bus > last_bus) return XUL_E_INVALID;
    context->ecam_virtual_base = (volatile uint8_t *)ecam_virtual_base;
    context->first_bus = first_bus;
    context->last_bus = last_bus;
    context->segment = segment;
    host->context = context;
    host->first_bus = first_bus;
    host->last_bus = last_bus;
    host->read16 = arm64_read16;
    host->write16 = arm64_write16;
    host->read32 = arm64_read32;
    host->write32 = arm64_write32;
    host->map_mmio = 0;
    host->unmap_mmio = 0;
    return XUL_OK;
}
