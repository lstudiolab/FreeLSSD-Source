/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#ifndef FBOOT_LOADER_H
#define FBOOT_LOADER_H

#include <stdint.h>
#include <stddef.h>
#include "fboot/elf64.h"

typedef enum {
    FBOOT_OK = 0,
    FBOOT_E_ARGUMENT,
    FBOOT_E_FORMAT,
    FBOOT_E_UNSUPPORTED,
    FBOOT_E_RANGE,
    FBOOT_E_MEMORY,
    FBOOT_E_FIRMWARE,
    FBOOT_E_SECURITY
} fboot_status_t;

typedef struct {
    uint64_t physical_base;
    uint64_t virtual_base;
    uint64_t image_size;
    uint64_t entry;
    uint16_t machine;
    uint16_t segment_count;
} fboot_loaded_image_t;

/* Pure validation: safe to host-test without UEFI firmware. */
fboot_status_t fboot_elf64_validate(const void *image,
                                    size_t image_size,
                                    uint16_t expected_machine,
                                    fboot_loaded_image_t *result);

#endif
