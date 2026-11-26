/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 *
 * Limine boot protocol declarations, derived from the 0BSD-licensed
 * Limine protocol specification and limine.h.
 *
 * Copyright (C) 2022-2026 Mintsuki and contributors.
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted.
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 * SPDX-License-Identifier: 0BSD
 */

#ifndef XUL_LIMINE_PROTOCOL_H
#define XUL_LIMINE_PROTOCOL_H

#include <stdint.h>

#define XUL_LIMINE_REQUESTS_START { \
    UINT64_C(0xf6b8f4b39de7d1ae), UINT64_C(0xfab91a6940fcb9cf), \
    UINT64_C(0x785c6ed015d3e316), UINT64_C(0x181e920a7852b9d9) }
#define XUL_LIMINE_REQUESTS_END { \
    UINT64_C(0xadc0e0531bb10d03), UINT64_C(0x9572709f31764c62) }
#define XUL_LIMINE_BASE_REVISION { \
    UINT64_C(0xf9562b2d5c95a6c8), UINT64_C(0x6a7b384944536bdc), UINT64_C(3) }
#define XUL_LIMINE_COMMON_MAGIC_0 UINT64_C(0xc7b1dd30df4c8b88)
#define XUL_LIMINE_COMMON_MAGIC_1 UINT64_C(0x0a82e883a194f07b)

#define XUL_LIMINE_HHDM_REQUEST_ID { XUL_LIMINE_COMMON_MAGIC_0, \
    XUL_LIMINE_COMMON_MAGIC_1, UINT64_C(0x48dcf1cb8ad2b852), \
    UINT64_C(0x63984e959a98244b) }
#define XUL_LIMINE_MEMMAP_REQUEST_ID { XUL_LIMINE_COMMON_MAGIC_0, \
    XUL_LIMINE_COMMON_MAGIC_1, UINT64_C(0x67cf3d9d378a806f), \
    UINT64_C(0xe304acdfc50c3c62) }
#define XUL_LIMINE_STACK_SIZE_REQUEST_ID { XUL_LIMINE_COMMON_MAGIC_0, \
    XUL_LIMINE_COMMON_MAGIC_1, UINT64_C(0x224ef0460a8e8926), \
    UINT64_C(0xe1cb0fc25f46ea3d) }
#define XUL_LIMINE_EXECUTABLE_ADDRESS_REQUEST_ID { XUL_LIMINE_COMMON_MAGIC_0, \
    XUL_LIMINE_COMMON_MAGIC_1, UINT64_C(0x71ba76863cc55f63), \
    UINT64_C(0xb2644a48c516a487) }

#define XUL_LIMINE_MEMMAP_USABLE UINT64_C(0)

typedef struct {
    uint64_t revision;
    uint64_t offset;
} xul_limine_hhdm_response_t;

typedef struct {
    uint64_t id[4];
    uint64_t revision;
    volatile xul_limine_hhdm_response_t *response;
} xul_limine_hhdm_request_t;

typedef struct {
    uint64_t base;
    uint64_t length;
    uint64_t type;
} xul_limine_memmap_entry_t;

typedef struct {
    uint64_t revision;
    uint64_t entry_count;
    xul_limine_memmap_entry_t **entries;
} xul_limine_memmap_response_t;

typedef struct {
    uint64_t id[4];
    uint64_t revision;
    volatile xul_limine_memmap_response_t *response;
} xul_limine_memmap_request_t;

typedef struct {
    uint64_t revision;
    uint64_t physical_base;
    uint64_t virtual_base;
} xul_limine_executable_address_response_t;

typedef struct {
    uint64_t id[4];
    uint64_t revision;
    volatile xul_limine_executable_address_response_t *response;
} xul_limine_executable_address_request_t;

typedef struct {
    uint64_t id[4];
    uint64_t revision;
    void *response;
    uint64_t stack_size;
} xul_limine_stack_size_request_t;

extern volatile uint64_t xul_limine_base_revision[3];
extern volatile xul_limine_hhdm_request_t xul_limine_hhdm_request;
extern volatile xul_limine_memmap_request_t xul_limine_memmap_request;
extern volatile xul_limine_executable_address_request_t
    xul_limine_executable_address_request;

#endif
