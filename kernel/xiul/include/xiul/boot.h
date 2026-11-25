/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#ifndef XIUL_BOOT_H
#define XIUL_BOOT_H

#include <stdint.h>

#define XIUL_BOOT_INFO_MAGIC UINT64_C(0x5849554c424f4f54)
#define XIUL_BOOT_INFO_VERSION 1u

/*
 * Bootloader hand-off contract shared by the x86_64 and AArch64 entry paths.
 * All addresses are physical until the platform VM layer explicitly maps them.
 */
typedef struct {
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t reserved;
} xiul_memory_region_t;

typedef struct {
    uint64_t magic;
    uint32_t version;
    uint32_t structure_size;
    uint64_t memory_map_physical;
    uint64_t memory_map_entries;
    uint64_t uart_virtual;
    uint32_t uart_clock_hz;
    uint32_t uart_baud_rate;
    uint64_t pci_ecam_virtual;
    uint16_t pci_segment;
    uint8_t pci_first_bus;
    uint8_t pci_last_bus;
    uint64_t firmware_table_physical;
    uint64_t framebuffer_physical;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_format;
} xiul_boot_info_t;

#endif
