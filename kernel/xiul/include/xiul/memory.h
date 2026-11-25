/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#ifndef XIUL_MEMORY_H
#define XIUL_MEMORY_H

#include <stddef.h>
#include <stdint.h>
#include "xiul/boot.h"
#include "xiul/status.h"

#define XIUL_PAGE_SIZE UINT64_C(4096)
#define XIUL_MEMORY_MAX_EXTENTS 256u
#define XIUL_MEMORY_TYPE_CONVENTIONAL 7u

typedef struct {
    uint64_t base;
    uint64_t length;
} xiul_memory_extent_t;

typedef struct {
    uint64_t total_pages;
    uint64_t free_pages;
    size_t extent_count;
} xiul_memory_stats_t;

/* Bootstrap physical allocator; calls are serialized until the VM/lock layer exists. */
xiul_status_t xiul_memory_initialize(const xiul_memory_region_t *regions,
                                     size_t region_count);
xiul_status_t xiul_memory_allocate_pages(size_t page_count, uint64_t *physical_address);
xiul_status_t xiul_memory_release_pages(uint64_t physical_address, size_t page_count);
void xiul_memory_get_stats(xiul_memory_stats_t *stats);

#endif
