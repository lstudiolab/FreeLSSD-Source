/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#ifndef XIUL_PCI_HOST_H
#define XIUL_PCI_HOST_H
#include <stdint.h>
#include "xiul/pci.h"

typedef struct {
    volatile uint8_t *ecam_virtual_base;
    uint8_t first_bus;
    uint8_t last_bus;
    uint16_t segment;
} xiul_arm64_pci_ecam_context_t;

/* x86_64 legacy PCI configuration mechanism #1 (CF8/CFC). */
xiul_status_t xiul_pci_host_init_x86_64(xiul_pci_host_t *host,
                                        uint8_t first_bus, uint8_t last_bus);

/*
 * AArch64 ECAM backend. The ECAM virtual base must already be mapped by the
 * platform VM bootstrap. The context must outlive all enumerated PCI devices.
 */
xiul_status_t xiul_pci_host_init_arm64_ecam(
    xiul_pci_host_t *host, xiul_arm64_pci_ecam_context_t *context,
    volatile void *ecam_virtual_base, uint16_t segment,
    uint8_t first_bus, uint8_t last_bus);
#endif
