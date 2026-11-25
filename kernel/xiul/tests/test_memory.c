/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include <assert.h>
#include <stdio.h>
#include "xiul/memory.h"

int main(void)
{
    const xiul_memory_region_t map[] = {
        {0x1003, 0x4ffd, XIUL_MEMORY_TYPE_CONVENTIONAL, 0},
        {0x6000, 0x4000, XIUL_MEMORY_TYPE_CONVENTIONAL, 0},
        {0x20000, 0x1000, 1, 0}
    };
    const xiul_memory_region_t overlapping[] = {
        {0x1000, 0x4000, XIUL_MEMORY_TYPE_CONVENTIONAL, 0},
        {0x3000, 0x2000, XIUL_MEMORY_TYPE_CONVENTIONAL, 0}
    };
    xiul_memory_stats_t stats;
    uint64_t first, second;

    assert(xiul_memory_initialize(overlapping, 2) == XIUL_E_INVALID);
    assert(xiul_memory_initialize(map, 3) == XIUL_OK);
    xiul_memory_get_stats(&stats);
    assert(stats.total_pages == 8);
    assert(stats.free_pages == 8);
    assert(stats.extent_count == 1);

    assert(xiul_memory_allocate_pages(2, &first) == XIUL_OK);
    assert(first == 0x2000);
    assert(xiul_memory_allocate_pages(1, &second) == XIUL_OK);
    assert(second == 0x4000);
    assert(xiul_memory_allocate_pages(20, &second) == XIUL_E_NOMEM);
    assert(xiul_memory_release_pages(0x9000, 1) == XIUL_E_INVALID);
    assert(xiul_memory_release_pages(first, 2) == XIUL_OK);
    assert(xiul_memory_release_pages(first, 2) == XIUL_E_INVALID);
    assert(xiul_memory_release_pages(second, 1) == XIUL_OK);
    xiul_memory_get_stats(&stats);
    assert(stats.free_pages == stats.total_pages);
    assert(stats.extent_count == 1);

    puts("XIUL physical page allocator tests: PASS");
    return 0;
}
