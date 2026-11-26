/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#ifndef XUL_MEMORY_H
#define XUL_MEMORY_H

#include <stddef.h>
#include <stdint.h>
#include "xul/boot.h"
#include "xul/status.h"

#define XUL_PAGE_SIZE UINT64_C(4096)
#define XUL_MEMORY_MAX_EXTENTS 256u
#define XUL_MEMORY_TYPE_CONVENTIONAL 7u

typedef struct {
    uint64_t base;
    uint64_t length;
} xul_memory_extent_t;

typedef struct {
    uint64_t total_pages;
    uint64_t free_pages;
    size_t extent_count;
} xul_memory_stats_t;

/* Bootstrap physical allocator; calls are serialized until the VM/lock layer exists. */
xul_status_t xul_memory_initialize(const xul_memory_region_t *regions,
                                     size_t region_count);
xul_status_t xul_memory_allocate_pages(size_t page_count, uint64_t *physical_address);
xul_status_t xul_memory_release_pages(uint64_t physical_address, size_t page_count);
void xul_memory_get_stats(xul_memory_stats_t *stats);

#endif
