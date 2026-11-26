/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include <stdatomic.h>
#include "xul/pci_host.h"
#include "xul/platform.h"

#define PCI_CONFIG_ADDRESS 0x0cf8u
#define PCI_CONFIG_DATA 0x0cfcu

static atomic_flag config_lock = ATOMIC_FLAG_INIT;

static inline void port_out32(uint16_t port, uint32_t value)
{
    __asm__ volatile ("outl %0, %1" : : "a"(value), "Nd"(port));
}
static inline void port_out16(uint16_t port, uint16_t value)
{
    __asm__ volatile ("outw %0, %1" : : "a"(value), "Nd"(port));
}
static inline uint32_t port_in32(uint16_t port)
{
    uint32_t value;
    __asm__ volatile ("inl %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}
static inline uint16_t port_in16(uint16_t port)
{
    uint16_t value;
    __asm__ volatile ("inw %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static xul_status_t config_address(uint8_t bus, uint8_t device,
                                    uint8_t function, uint16_t offset,
                                    uint32_t *address)
{
    if (device >= 32u || function >= 8u || offset >= 256u ||
        (offset & 3u) != 0) return XUL_E_INVALID;
    *address = UINT32_C(0x80000000) |
        ((uint32_t)bus << 16) | ((uint32_t)device << 11) |
        ((uint32_t)function << 8) | (uint32_t)offset;
    return XUL_OK;
}

static xul_irq_state_t lock_config(void)
{
    xul_irq_state_t state = xul_platform_irq_save_disable();
    while (atomic_flag_test_and_set_explicit(&config_lock, memory_order_acquire))
        xul_platform_cpu_relax();
    return state;
}
static void unlock_config(xul_irq_state_t state)
{
    atomic_flag_clear_explicit(&config_lock, memory_order_release);
    xul_platform_irq_restore(state);
}

static xul_status_t x86_read32(void *context, uint8_t bus, uint8_t device,
                                uint8_t function, uint16_t offset, uint32_t *value)
{
    uint32_t address;
    xul_irq_state_t irq;
    xul_status_t status;
    (void)context;
    if (value == 0) return XUL_E_INVALID;
    status = config_address(bus, device, function, offset, &address);
    if (status != XUL_OK) return status;
    irq = lock_config();
    port_out32(PCI_CONFIG_ADDRESS, address);
    *value = port_in32(PCI_CONFIG_DATA);
    unlock_config(irq);
    return XUL_OK;
}

static xul_status_t x86_write32(void *context, uint8_t bus, uint8_t device,
                                 uint8_t function, uint16_t offset, uint32_t value)
{
    uint32_t address;
    xul_irq_state_t irq;
    xul_status_t status;
    (void)context;
    status = config_address(bus, device, function, offset, &address);
    if (status != XUL_OK) return status;
    irq = lock_config();
    port_out32(PCI_CONFIG_ADDRESS, address);
    port_out32(PCI_CONFIG_DATA, value);
    unlock_config(irq);
    return XUL_OK;
}

static xul_status_t x86_read16(void *context, uint8_t bus, uint8_t device,
                                uint8_t function, uint16_t offset, uint16_t *value)
{
    uint32_t address;
    xul_irq_state_t irq;
    xul_status_t status;
    (void)context;
    if (value == 0 || offset >= 256u || (offset & 1u) != 0)
        return XUL_E_INVALID;
    address = UINT32_C(0x80000000) |
        ((uint32_t)bus << 16) | ((uint32_t)device << 11) |
        ((uint32_t)function << 8) | (uint32_t)(offset & ~3u);
    if (device >= 32u || function >= 8u) return XUL_E_INVALID;
    status = XUL_OK;
    irq = lock_config();
    port_out32(PCI_CONFIG_ADDRESS, address);
    *value = port_in16((uint16_t)(PCI_CONFIG_DATA + (offset & 2u)));
    unlock_config(irq);
    return status;
}

static xul_status_t x86_write16(void *context, uint8_t bus, uint8_t device,
                                 uint8_t function, uint16_t offset, uint16_t value)
{
    uint32_t address;
    xul_irq_state_t irq;
    (void)context;
    if (device >= 32u || function >= 8u || offset >= 256u || (offset & 1u) != 0)
        return XUL_E_INVALID;
    address = UINT32_C(0x80000000) |
        ((uint32_t)bus << 16) | ((uint32_t)device << 11) |
        ((uint32_t)function << 8) | (uint32_t)(offset & ~3u);
    irq = lock_config();
    port_out32(PCI_CONFIG_ADDRESS, address);
    port_out16((uint16_t)(PCI_CONFIG_DATA + (offset & 2u)), value);
    unlock_config(irq);
    return XUL_OK;
}

xul_status_t xul_pci_host_init_x86_64(xul_pci_host_t *host,
                                        uint8_t first_bus, uint8_t last_bus)
{
    if (host == 0 || first_bus > last_bus) return XUL_E_INVALID;
    host->context = 0;
    host->first_bus = first_bus;
    host->last_bus = last_bus;
    host->read16 = x86_read16;
    host->write16 = x86_write16;
    host->read32 = x86_read32;
    host->write32 = x86_write32;
    host->map_mmio = 0;
    host->unmap_mmio = 0;
    return XUL_OK;
}
