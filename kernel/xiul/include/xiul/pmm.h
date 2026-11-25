/* This file has been modified by L,Studio. Copyright (c) 2026 L,Studio. All rights reserved. */
#ifndef XIUL_PMM_H
#define XIUL_PMM_H
#include <stddef.h>
#include <stdint.h>
#include "xiul/boot.h"
#include "xiul/status.h"
#define XIUL_PMM_PAGE_SIZE UINT64_C(4096)
#define XIUL_PMM_MAX_RANGES 512u
#define XIUL_PMM_MAX_ALLOCATIONS 1024u
#define XIUL_MEMORY_USABLE 1u
/* Returns physical addresses. Serialize all calls after SMP starts. */
xiul_status_t xiul_pmm_initialize(const xiul_memory_region_t *regions, size_t count);
xiul_status_t xiul_pmm_allocate(size_t pages, size_t alignment_pages, uint64_t *physical_address);
xiul_status_t xiul_pmm_release(uint64_t physical_address, size_t pages);
uint64_t xiul_pmm_free_pages(void);
int xiul_pmm_is_initialized(void);
#endif
