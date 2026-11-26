/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "xul/kernel.h"
#include "xul/subsystem.h"
#include "xul/memory.h"

static int sequence[8], sequence_count;
static const xul_memory_region_t boot_memory[] = {
    {0x100000, 0x100000, XUL_MEMORY_TYPE_CONVENTIONAL, 0}
};
static size_t fake_devices;
static xul_status_t platform_result = XUL_OK;

xul_status_t xul_platform_initialize(const xul_boot_info_t *boot_info)
{
    assert(boot_info != 0);
    assert(boot_info->magic == XUL_BOOT_INFO_MAGIC);
    sequence[sequence_count++] = 1;
    return platform_result;
}

xul_status_t xul_platform_get_pci_host(xul_pci_host_t *host)
{
    assert(host != 0);
    sequence[sequence_count++] = 2;
    return XUL_OK;
}

xul_status_t xul_pci_enumerate(const xul_pci_host_t *host, size_t *found)
{
    assert(host != 0 && found != 0);
    sequence[sequence_count++] = 3;
    *found = fake_devices;
    return XUL_OK;
}

xul_status_t xul_pci_remove_all(void)
{
    sequence[sequence_count++] = 4;
    return XUL_OK;
}

int main(void)
{
    xul_boot_info_t boot = {0};
    const xul_kernel_context_t *context;
    xul_subsystem_test_reset();
    sequence_count = 0;
    fake_devices = 2;
    boot.magic = XUL_BOOT_INFO_MAGIC;
    boot.version = XUL_BOOT_INFO_VERSION;
    boot.structure_size = sizeof(boot);
    boot.memory_map_physical = 0x1000;
    boot.memory_map_virtual = (uint64_t)(uintptr_t)boot_memory;
    boot.memory_map_entries = sizeof(boot_memory) / sizeof(boot_memory[0]);

    assert(xul_kernel_boot(&boot) == XUL_OK);
    assert(sequence_count == 3);
    assert(sequence[0] == 1 && sequence[1] == 2 && sequence[2] == 3);
    context = xul_kernel_context();
    assert(context->pci_devices == 2);
    assert(context->boot_info.magic == XUL_BOOT_INFO_MAGIC);
    xul_kernel_shutdown();
    assert(sequence_count == 4 && sequence[3] == 4);

    puts("XUL kernel boot orchestration tests: PASS");
    return 0;
}
