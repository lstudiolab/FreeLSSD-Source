/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include <assert.h>
#include <stdio.h>
#include "xul/platform.h"

static unsigned int arch_calls;
static xul_status_t arch_result = XUL_OK;

xul_status_t xul_arch_early_init(const xul_boot_info_t *boot_info)
{
    assert(boot_info != 0);
    ++arch_calls;
    return arch_result;
}

int main(void)
{
    xul_boot_info_t info = {0};
    info.magic = XUL_BOOT_INFO_MAGIC;
    info.version = XUL_BOOT_INFO_VERSION;
    info.structure_size = sizeof(info);

    assert(xul_platform_initialize(0) == XUL_E_INVALID);
    assert(arch_calls == 0);
    info.magic = 0;
    assert(xul_platform_initialize(&info) == XUL_E_INVALID);
    assert(arch_calls == 0);
    info.magic = XUL_BOOT_INFO_MAGIC;
    info.version++;
    assert(xul_platform_initialize(&info) == XUL_E_INVALID);
    assert(arch_calls == 0);
    info.version = XUL_BOOT_INFO_VERSION;
    info.memory_map_entries = 1;
    info.memory_map_physical = 0x1000;
    info.memory_map_virtual = 0;
    assert(xul_platform_initialize(&info) == XUL_E_INVALID);
    assert(arch_calls == 0);
    info.memory_map_virtual = 0xffff800000001000ULL;
    info.memory_map_entries = UINT64_MAX / sizeof(xul_memory_region_t) + 1u;
    assert(xul_platform_initialize(&info) == XUL_E_INVALID);
    info.memory_map_entries = 1;
    info.memory_map_physical = UINT64_MAX - 7u;
    info.memory_map_virtual = 0xffff800000001000ULL;
    assert(xul_platform_initialize(&info) == XUL_E_INVALID);
    assert(arch_calls == 0);
    info.memory_map_entries = 1;
    info.memory_map_physical = 0x1000;
    info.memory_map_virtual = 0xffff800000001000ULL;
    assert(xul_platform_initialize(&info) == XUL_OK);
    assert(arch_calls == 1);
    info.memory_map_entries = 0;
    info.memory_map_physical = 0;
    info.memory_map_virtual = 0;
    assert(xul_platform_initialize(&info) == XUL_OK);
    assert(arch_calls == 2);
    arch_result = XUL_E_INIT;
    assert(xul_platform_initialize(&info) == XUL_E_INIT);
    assert(arch_calls == 3);
    puts("XUL boot handoff validation tests: PASS");
    return 0;
}
