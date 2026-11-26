/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#ifndef XUL_PCI_H
#define XUL_PCI_H
#include <stddef.h>
#include <stdint.h>
#include "xul/status.h"

#define XUL_PCI_MAX_ENUMERATED_DEVICES 128u
#define XUL_PCI_CONFIG_SPACE_SIZE 256u

typedef struct {
    void *context;
    uint8_t first_bus;
    uint8_t last_bus;
    xul_status_t (*read16)(void *, uint8_t, uint8_t, uint8_t, uint16_t, uint16_t *);
    xul_status_t (*write16)(void *, uint8_t, uint8_t, uint8_t, uint16_t, uint16_t);
    xul_status_t (*read32)(void *, uint8_t, uint8_t, uint8_t, uint16_t, uint32_t *);
    xul_status_t (*write32)(void *, uint8_t, uint8_t, uint8_t, uint16_t, uint32_t);
    volatile void *(*map_mmio)(void *, uint64_t, size_t);
    void (*unmap_mmio)(void *, volatile void *, size_t);
} xul_pci_host_t;

/*
 * Enumerate a host bridge's configured bus range. The host and its callbacks
 * must remain valid until xul_pci_remove_all() completes. Calls are serialized.
 */
xul_status_t xul_pci_enumerate(const xul_pci_host_t *host, size_t *found);
xul_status_t xul_pci_remove_all(void);
size_t xul_pci_device_count(void);

#ifdef XUL_TESTING
void xul_pci_test_reset(void);
#endif
#endif
