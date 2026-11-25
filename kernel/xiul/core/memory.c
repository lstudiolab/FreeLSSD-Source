/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include "xiul/memory.h"

static xiul_memory_extent_t extents[XIUL_MEMORY_MAX_EXTENTS];
static size_t extent_count;
static uint64_t total_pages;
static uint64_t free_pages;
static unsigned int initialized;

static void remove_extent(size_t index)
{
    size_t i;
    for (i = index + 1; i < extent_count; ++i) extents[i - 1] = extents[i];
    --extent_count;
}

static void insert_extent(size_t index, xiul_memory_extent_t extent)
{
    size_t i;
    for (i = extent_count; i > index; --i) extents[i] = extents[i - 1];
    extents[index] = extent;
    ++extent_count;
}

xiul_status_t xiul_memory_initialize(const xiul_memory_region_t *regions,
                                     size_t region_count)
{
    size_t i;
    if ((regions == 0 && region_count != 0) || initialized) return XIUL_E_INVALID;
    extent_count = 0;
    total_pages = 0;
    free_pages = 0;

    for (i = 0; i < region_count; ++i) {
        uint64_t end, base, length;
        xiul_memory_extent_t extent;
        size_t position;
        if (regions[i].type != XIUL_MEMORY_TYPE_CONVENTIONAL || regions[i].length == 0)
            continue;
        if (regions[i].base > UINT64_MAX - regions[i].length) return XIUL_E_INVALID;
        end = regions[i].base + regions[i].length;
        if (regions[i].base > UINT64_MAX - (XIUL_PAGE_SIZE - 1u)) return XIUL_E_INVALID;
        base = (regions[i].base + XIUL_PAGE_SIZE - 1u) & ~(XIUL_PAGE_SIZE - 1u);
        end &= ~(XIUL_PAGE_SIZE - 1u);
        if (end <= base) continue;
        length = end - base;
        if (extent_count == XIUL_MEMORY_MAX_EXTENTS) return XIUL_E_CAPACITY;
        extent.base = base;
        extent.length = length;

        position = 0;
        while (position < extent_count && extents[position].base < base) ++position;
        if (position != 0 &&
            extents[position - 1].base + extents[position - 1].length > base)
            return XIUL_E_INVALID;
        if (position < extent_count && end > extents[position].base)
            return XIUL_E_INVALID;
        insert_extent(position, extent);
    }

    /* Coalesce adjacent conventional-memory regions. */
    i = 0;
    while (i + 1 < extent_count) {
        if (extents[i].base + extents[i].length == extents[i + 1].base) {
            extents[i].length += extents[i + 1].length;
            remove_extent(i + 1);
        } else {
            ++i;
        }
    }

    for (i = 0; i < extent_count; ++i) {
        uint64_t pages = extents[i].length / XIUL_PAGE_SIZE;
        if (total_pages > UINT64_MAX - pages) return XIUL_E_INVALID;
        total_pages += pages;
    }
    free_pages = total_pages;
    initialized = 1;
    return XIUL_OK;
}

xiul_status_t xiul_memory_allocate_pages(size_t page_count, uint64_t *physical_address)
{
    size_t i;
    uint64_t bytes;
    if (!initialized || physical_address == 0 || page_count == 0)
        return XIUL_E_INVALID;
    if ((uint64_t)page_count > UINT64_MAX / XIUL_PAGE_SIZE)
        return XIUL_E_INVALID;
    bytes = (uint64_t)page_count * XIUL_PAGE_SIZE;
    for (i = 0; i < extent_count; ++i) {
        if (extents[i].length < bytes) continue;
        *physical_address = extents[i].base;
        extents[i].base += bytes;
        extents[i].length -= bytes;
        free_pages -= (uint64_t)page_count;
        if (extents[i].length == 0) remove_extent(i);
        return XIUL_OK;
    }
    return XIUL_E_NOMEM;
}

xiul_status_t xiul_memory_release_pages(uint64_t physical_address, size_t page_count)
{
    uint64_t bytes, end;
    size_t position = 0;
    xiul_memory_extent_t extent;
    if (!initialized || page_count == 0 ||
        (physical_address & (XIUL_PAGE_SIZE - 1u)) != 0 ||
        (uint64_t)page_count > UINT64_MAX / XIUL_PAGE_SIZE)
        return XIUL_E_INVALID;
    bytes = (uint64_t)page_count * XIUL_PAGE_SIZE;
    if (physical_address > UINT64_MAX - bytes) return XIUL_E_INVALID;
    end = physical_address + bytes;
    while (position < extent_count && extents[position].base < physical_address)
        ++position;
    if ((position != 0 &&
         extents[position - 1].base + extents[position - 1].length > physical_address) ||
        (position < extent_count && end > extents[position].base))
        return XIUL_E_INVALID;
    if (free_pages > total_pages - (uint64_t)page_count) return XIUL_E_INVALID;
    if (extent_count == XIUL_MEMORY_MAX_EXTENTS &&
        !(position != 0 &&
          extents[position - 1].base + extents[position - 1].length == physical_address) &&
        !(position < extent_count && end == extents[position].base))
        return XIUL_E_CAPACITY;

    extent.base = physical_address;
    extent.length = bytes;
    insert_extent(position, extent);
    if (position != 0 &&
        extents[position - 1].base + extents[position - 1].length == extents[position].base) {
        extents[position - 1].length += extents[position].length;
        remove_extent(position);
        --position;
    }
    if (position + 1 < extent_count &&
        extents[position].base + extents[position].length == extents[position + 1].base) {
        extents[position].length += extents[position + 1].length;
        remove_extent(position + 1);
    }
    free_pages += (uint64_t)page_count;
    return XIUL_OK;
}

void xiul_memory_get_stats(xiul_memory_stats_t *stats)
{
    if (stats == 0) return;
    stats->total_pages = total_pages;
    stats->free_pages = free_pages;
    stats->extent_count = extent_count;
}
