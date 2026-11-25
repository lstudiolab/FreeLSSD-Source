/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include <assert.h>
#include <stdio.h>
#include "xiul/platform.h"

static unsigned int arch_calls;
static xiul_status_t arch_result = XIUL_OK;

xiul_status_t xiul_arch_early_init(const xiul_boot_info_t *boot_info)
{
    assert(boot_info != 0);
    ++arch_calls;
    return arch_result;
}

int main(void)
{
    xiul_boot_info_t info = {0};
    info.magic = XIUL_BOOT_INFO_MAGIC;
    info.version = XIUL_BOOT_INFO_VERSION;
    info.structure_size = sizeof(info);

    assert(xiul_platform_initialize(0) == XIUL_E_INVALID);
    assert(arch_calls == 0);
    info.magic = 0;
    assert(xiul_platform_initialize(&info) == XIUL_E_INVALID);
    assert(arch_calls == 0);
    info.magic = XIUL_BOOT_INFO_MAGIC;
    info.version++;
    assert(xiul_platform_initialize(&info) == XIUL_E_INVALID);
    assert(arch_calls == 0);
    info.version = XIUL_BOOT_INFO_VERSION;
    info.memory_map_entries = 1;
    info.memory_map_physical = 0x1000;
    info.memory_map_virtual = 0;
    assert(xiul_platform_initialize(&info) == XIUL_E_INVALID);
    assert(arch_calls == 0);
    info.memory_map_virtual = 0xffff800000001000ULL;
    info.memory_map_entries = UINT64_MAX / sizeof(xiul_memory_region_t) + 1u;
    assert(xiul_platform_initialize(&info) == XIUL_E_INVALID);
    info.memory_map_entries = 0;
    info.memory_map_physical = 0;
    info.memory_map_virtual = 0;
    assert(xiul_platform_initialize(&info) == XIUL_OK);
    assert(arch_calls == 1);
    arch_result = XIUL_E_INIT;
    assert(xiul_platform_initialize(&info) == XIUL_E_INIT);
    assert(arch_calls == 2);
    puts("XIUL boot handoff validation tests: PASS");
    return 0;
}
