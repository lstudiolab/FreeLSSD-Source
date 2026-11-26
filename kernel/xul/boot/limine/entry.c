/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 *
 * Limine protocol adapter. Converts Limine's pointer-based memory map into
 * XUL's stable, contiguous boot ABI before entering the common kernel path.
 */

#include <stddef.h>
#include <stdint.h>
#include "xul/boot.h"
#include "xul/kernel.h"
#include "xul/limine.h"
#include "xul/memory.h"

#define XUL_LIMINE_MAX_MEMORY_MAP_ENTRIES 512u

static xul_memory_region_t limine_memory_map[XUL_LIMINE_MAX_MEMORY_MAP_ENTRIES]
    __attribute__((aligned(16)));

static _Noreturn void limine_boot_failure(void)
{
#if defined(__x86_64__)
    for (;;) __asm__ volatile ("cli; hlt" : : : "memory");
#elif defined(__aarch64__)
    for (;;) __asm__ volatile ("msr daifset, #0xf; wfe" : : : "memory");
#else
#error Unsupported XUL Limine architecture
#endif
}

_Noreturn void xul_limine_entry(void)
{
    const xul_limine_hhdm_response_t *hhdm =
        (const xul_limine_hhdm_response_t *)xul_limine_hhdm_request.response;
    const xul_limine_memmap_response_t *map =
        (const xul_limine_memmap_response_t *)xul_limine_memmap_request.response;
    const xul_limine_executable_address_response_t *image =
        (const xul_limine_executable_address_response_t *)
            xul_limine_executable_address_request.response;
    xul_boot_info_t boot_info;
    uintptr_t map_virtual;
    uint64_t map_offset;
    size_t i;

    if (xul_limine_base_revision[2] != 0 || hhdm == 0 ||
        map == 0 || image == 0 || map->entries == 0 ||
        map->entry_count == 0 ||
        map->entry_count > XUL_LIMINE_MAX_MEMORY_MAP_ENTRIES)
        limine_boot_failure();

    map_virtual = (uintptr_t)limine_memory_map;
    if ((uint64_t)map_virtual < image->virtual_base)
        limine_boot_failure();
    map_offset = (uint64_t)map_virtual - image->virtual_base;
    if (image->physical_base > UINT64_MAX - map_offset)
        limine_boot_failure();

    for (i = 0; i < (size_t)map->entry_count; ++i) {
        const xul_limine_memmap_entry_t *source = map->entries[i];
        if (source == 0) limine_boot_failure();
        limine_memory_map[i].base = source->base;
        limine_memory_map[i].length = source->length;
        limine_memory_map[i].type =
            source->type == XUL_LIMINE_MEMMAP_USABLE
                ? XUL_MEMORY_TYPE_CONVENTIONAL : 0u;
        limine_memory_map[i].reserved = 0;
    }

    boot_info.magic = XUL_BOOT_INFO_MAGIC;
    boot_info.version = XUL_BOOT_INFO_VERSION;
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

    xul_kernel_entry(&boot_info);
}
