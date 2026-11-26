/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include "xul/limine.h"

#define XUL_USED_SECTION(name) \
    __attribute__((used, section(name), aligned(8)))

volatile uint64_t xul_limine_requests_start_marker[4]
    XUL_USED_SECTION(".limine_requests_start") = XUL_LIMINE_REQUESTS_START;

volatile uint64_t xul_limine_base_revision[3]
    XUL_USED_SECTION(".limine_requests") = XUL_LIMINE_BASE_REVISION;

volatile xul_limine_hhdm_request_t xul_limine_hhdm_request
    XUL_USED_SECTION(".limine_requests") = {
        XUL_LIMINE_HHDM_REQUEST_ID, 0, 0
    };

volatile xul_limine_memmap_request_t xul_limine_memmap_request
    XUL_USED_SECTION(".limine_requests") = {
        XUL_LIMINE_MEMMAP_REQUEST_ID, 0, 0
    };

volatile xul_limine_executable_address_request_t
xul_limine_executable_address_request
    XUL_USED_SECTION(".limine_requests") = {
        XUL_LIMINE_EXECUTABLE_ADDRESS_REQUEST_ID, 0, 0
    };

static volatile xul_limine_stack_size_request_t xul_limine_stack_size_request
    XUL_USED_SECTION(".limine_requests") = {
        XUL_LIMINE_STACK_SIZE_REQUEST_ID, 0, 0, UINT64_C(65536)
    };

volatile uint64_t xul_limine_requests_end_marker[2]
    XUL_USED_SECTION(".limine_requests_end") = XUL_LIMINE_REQUESTS_END;
