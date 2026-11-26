/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 *
 * Limine protocol adapter. Converts Limine's pointer-based memory map into
 * XIUL's stable, contiguous boot ABI before entering the common kernel path.
 */

#include <stddef.h>
#include <stdint.h>
#include "xiul/boot.h"
#include "xiul/kernel.h"
#include "xiul/limine.h"
#include "xiul/memory.h"

#define XIUL_LIMINE_MAX_MEMORY_MAP_ENTRIES 512u

static xiul_memory_region_t limine_memory_map[XIUL_LIMINE_MAX_MEMORY_MAP_ENTRIES]
    __attribute__((aligned(16)));

static _Noreturn void limine_boot_failure(void)
{
#if defined(__x86_64__)
    for (;;) __asm__ volatile ("cli; hlt" : : : "memory");
#elif defined(__aarch64__)
    for (;;) __asm__ volatile ("msr daifset, #0xf; wfe" : : : "memory");
#else
#error Unsupported XIUL Limine architecture
#endif
}

_Noreturn void xiul_limine_entry(void)
{
    const xiul_limine_hhdm_response_t *hhdm =
        (const xiul_limine_hhdm_response_t *)xiul_limine_hhdm_request.response;
    const xiul_limine_memmap_response_t *map =
        (const xiul_limine_memmap_response_t *)xiul_limine_memmap_request.response;
    const xiul_limine_executable_address_response_t *image =
        (const xiul_limine_executable_address_response_t *)
            xiul_limine_executable_address_request.response;
    xiul_boot_info_t boot_info;
    uintptr_t map_virtual;
    uint64_t map_offset;
    size_t i;

    if (xiul_limine_base_revision[2] != 0 || hhdm == 0 ||
        map == 0 || image == 0 || map->entries == 0 ||
        map->entry_count == 0 ||
        map->entry_count > XIUL_LIMINE_MAX_MEMORY_MAP_ENTRIES)
        limine_boot_failure();

    map_virtual = (uintptr_t)limine_memory_map;
    if ((uint64_t)map_virtual < image->virtual_base)
        limine_boot_failure();
    map_offset = (uint64_t)map_virtual - image->virtual_base;
    if (image->physical_base > UINT64_MAX - map_offset)
        limine_boot_failure();

    for (i = 0; i < (size_t)map->entry_count; ++i) {
        const xiul_limine_memmap_entry_t *source = map->entries[i];
        if (source == 0) limine_boot_failure();
        limine_memory_map[i].base = source->base;
        limine_memory_map[i].length = source->length;
        limine_memory_map[i].type =
            source->type == XIUL_LIMINE_MEMMAP_USABLE
                ? XIUL_MEMORY_TYPE_CONVENTIONAL : 0u;
        limine_memory_map[i].reserved = 0;
    }

    boot_info.magic = XIUL_BOOT_INFO_MAGIC;
    boot_info.version = XIUL_BOOT_INFO_VERSION;
    boot_info.structure_size = (uint32_t)sizeof(boot_info);
    boot_info.memory_map_physical = image->physical_base + map_offset;
    boot_info.memory_map_virtual = (uint64_t)map_virtual;
    boot_info.memory_map_entries = map->entry_count;
    boot_info.uart_virtual = 0;
    boot_info.uart_clock_hz = 0;
    boot_info.uart_baud_rate = 0;
    boot_info.pci_ecam_virtual = 0;
    boot_info.pci_segment = 0;
    boot_info.pci_first_bus = 0;
    boot_info.pci_last_bus = 0;
    boot_info.firmware_table_physical = 0;
    boot_info.framebuffer_physical = 0;
    boot_info.framebuffer_width = 0;
    boot_info.framebuffer_height = 0;
    boot_info.framebuffer_pitch = 0;
    boot_info.framebuffer_format = 0;

    /* Force a read of the HHDM response before dereferencing protocol data. */
    if (hhdm->offset == 0) limine_boot_failure();

    xiul_kernel_entry(&boot_info);
}
