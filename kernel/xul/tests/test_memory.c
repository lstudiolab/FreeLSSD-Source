/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include <assert.h>
#include <stdio.h>
#include "xul/memory.h"

int main(void)
{
    const xul_memory_region_t map[] = {
        {0x1003, 0x4ffd, XUL_MEMORY_TYPE_CONVENTIONAL, 0},
        {0x6000, 0x4000, XUL_MEMORY_TYPE_CONVENTIONAL, 0},
        {0x20000, 0x1000, 1, 0}
    };
    const xul_memory_region_t overlapping[] = {
        {0x1000, 0x4000, XUL_MEMORY_TYPE_CONVENTIONAL, 0},
        {0x3000, 0x2000, XUL_MEMORY_TYPE_CONVENTIONAL, 0}
    };
    xul_memory_stats_t stats;
    uint64_t first, second;

    assert(xul_memory_initialize(overlapping, 2) == XUL_E_INVALID);
    assert(xul_memory_initialize(map, 3) == XUL_OK);
    xul_memory_get_stats(&stats);
    assert(stats.total_pages == 8);
    assert(stats.free_pages == 8);
    assert(stats.extent_count == 1);

    assert(xul_memory_allocate_pages(2, &first) == XUL_OK);
    assert(first == 0x2000);
    assert(xul_memory_allocate_pages(1, &second) == XUL_OK);
    assert(second == 0x4000);
    assert(xul_memory_allocate_pages(20, &second) == XUL_E_NOMEM);
    assert(xul_memory_release_pages(0x9000, 1) == XUL_E_INVALID);
    assert(xul_memory_release_pages(first, 2) == XUL_OK);
    assert(xul_memory_release_pages(first, 2) == XUL_E_INVALID);
    assert(xul_memory_release_pages(second, 1) == XUL_OK);
    xul_memory_get_stats(&stats);
    assert(stats.free_pages == stats.total_pages);
    assert(stats.extent_count == 1);

    puts("XUL physical page allocator tests: PASS");
    return 0;
}
