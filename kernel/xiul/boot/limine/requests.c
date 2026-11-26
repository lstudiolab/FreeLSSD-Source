/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include "xiul/limine.h"

#define XIUL_USED_SECTION(name) \
    __attribute__((used, section(name), aligned(8)))

volatile uint64_t xiul_limine_requests_start_marker[4]
    XIUL_USED_SECTION(".limine_requests_start") = XIUL_LIMINE_REQUESTS_START;

volatile uint64_t xiul_limine_base_revision[3]
    XIUL_USED_SECTION(".limine_requests") = XIUL_LIMINE_BASE_REVISION;

volatile xiul_limine_hhdm_request_t xiul_limine_hhdm_request
    XIUL_USED_SECTION(".limine_requests") = {
        XIUL_LIMINE_HHDM_REQUEST_ID, 0, 0
    };

volatile xiul_limine_memmap_request_t xiul_limine_memmap_request
    XIUL_USED_SECTION(".limine_requests") = {
        XIUL_LIMINE_MEMMAP_REQUEST_ID, 0, 0
    };

volatile xiul_limine_executable_address_request_t
xiul_limine_executable_address_request
    XIUL_USED_SECTION(".limine_requests") = {
        XIUL_LIMINE_EXECUTABLE_ADDRESS_REQUEST_ID, 0, 0
    };

static volatile xiul_limine_stack_size_request_t xiul_limine_stack_size_request
    XIUL_USED_SECTION(".limine_requests") = {
        XIUL_LIMINE_STACK_SIZE_REQUEST_ID, 0, 0, UINT64_C(65536)
    };

volatile uint64_t xiul_limine_requests_end_marker[2]
    XIUL_USED_SECTION(".limine_requests_end") = XIUL_LIMINE_REQUESTS_END;
