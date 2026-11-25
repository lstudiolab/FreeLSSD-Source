/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#ifndef XIUL_PCI_H
#define XIUL_PCI_H
#include <stddef.h>
#include <stdint.h>
#include "xiul/status.h"

#define XIUL_PCI_MAX_ENUMERATED_DEVICES 128u
#define XIUL_PCI_CONFIG_SPACE_SIZE 256u

typedef struct {
    void *context;
    uint8_t first_bus;
    uint8_t last_bus;
    xiul_status_t (*read16)(void *, uint8_t, uint8_t, uint8_t, uint16_t, uint16_t *);
    xiul_status_t (*write16)(void *, uint8_t, uint8_t, uint8_t, uint16_t, uint16_t);
    xiul_status_t (*read32)(void *, uint8_t, uint8_t, uint8_t, uint16_t, uint32_t *);
    xiul_status_t (*write32)(void *, uint8_t, uint8_t, uint8_t, uint16_t, uint32_t);
    volatile void *(*map_mmio)(void *, uint64_t, size_t);
    void (*unmap_mmio)(void *, volatile void *, size_t);
} xiul_pci_host_t;

/*
 * Enumerate a host bridge's configured bus range. The host and its callbacks
 * must remain valid until xiul_pci_remove_all() completes. Calls are serialized.
 */
xiul_status_t xiul_pci_enumerate(const xiul_pci_host_t *host, size_t *found);
xiul_status_t xiul_pci_remove_all(void);
size_t xiul_pci_device_count(void);

#ifdef XIUL_TESTING
void xiul_pci_test_reset(void);
#endif
#endif
