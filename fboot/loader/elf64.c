/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 *
 * Freestanding ELF64 validation shared by fBoot firmware loaders and host tests.
 */

#include <stddef.h>
#include <stdint.h>
#include "fboot/loader.h"

static int range_valid(uint64_t offset, uint64_t length, size_t total)
{
    return offset <= (uint64_t)total && length <= (uint64_t)total - offset;
}

static int power_of_two(uint64_t value)
{
    return value != 0 && (value & (value - 1u)) == 0;
}

fboot_status_t fboot_elf64_validate(const void *image,
                                    size_t image_size,
                                    uint16_t expected_machine,
                                    fboot_loaded_image_t *result)
{
    const uint8_t *bytes = (const uint8_t *)image;
    const fboot_elf64_header_t *header;
    const fboot_elf64_program_header_t *program_headers;
    uint64_t lowest = UINT64_MAX;
    uint64_t highest = 0;
    uint64_t phdr_bytes;
    uint16_t loads = 0;
    uint16_t i;
    uint16_t j;

    if (image == NULL || result == NULL ||
        image_size < sizeof(fboot_elf64_header_t))
        return FBOOT_E_ARGUMENT;

    header = (const fboot_elf64_header_t *)bytes;
    if (header->ident[0] != FBOOT_ELF_MAGIC_0 ||
        header->ident[1] != FBOOT_ELF_MAGIC_1 ||
        header->ident[2] != FBOOT_ELF_MAGIC_2 ||
        header->ident[3] != FBOOT_ELF_MAGIC_3 ||
        header->ident[4] != FBOOT_ELFCLASS64 ||
        header->ident[5] != FBOOT_ELFDATA2LSB ||
        header->ident[6] != 1u || header->type != FBOOT_ET_EXEC ||
        header->version != 1u ||
        header->machine != expected_machine ||
        header->header_size != sizeof(*header) ||
        header->program_header_entry_size != sizeof(*program_headers) ||
        header->program_header_count == 0 ||
        header->program_header_count > FBOOT_ELF_MAX_PHDRS)
        return FBOOT_E_FORMAT;

    if ((uint64_t)header->program_header_count >
        UINT64_MAX / sizeof(*program_headers))
        return FBOOT_E_RANGE;
    phdr_bytes = (uint64_t)header->program_header_count *
                 sizeof(*program_headers);
    if (!range_valid(header->program_header_offset, phdr_bytes, image_size))
        return FBOOT_E_RANGE;

    program_headers = (const fboot_elf64_program_header_t *)
        (bytes + (size_t)header->program_header_offset);

    for (i = 0; i < header->program_header_count; ++i) {
        const fboot_elf64_program_header_t *ph = &program_headers[i];
        uint64_t end;
        if (ph->type != FBOOT_PT_LOAD) continue;
        if (ph->file_size > ph->memory_size ||
            !range_valid(ph->offset, ph->file_size, image_size) ||
            ph->virtual_address > UINT64_MAX - ph->memory_size)
            return FBOOT_E_RANGE;
        if ((ph->flags & ~(FBOOT_PF_R | FBOOT_PF_W | FBOOT_PF_X)) != 0 ||
            (ph->flags & (FBOOT_PF_W | FBOOT_PF_X)) ==
                (FBOOT_PF_W | FBOOT_PF_X))
            return FBOOT_E_SECURITY;
        if (ph->alignment > 1u &&
            (!power_of_two(ph->alignment) ||
             ((ph->virtual_address - ph->offset) & (ph->alignment - 1u)) != 0))
            return FBOOT_E_FORMAT;
        if (ph->memory_size == 0) continue;
        end = ph->virtual_address + ph->memory_size;
        if (ph->virtual_address < lowest) lowest = ph->virtual_address;
        if (end > highest) highest = end;
        ++loads;
        for (j = 0; j < i; ++j) {
            const fboot_elf64_program_header_t *prior = &program_headers[j];
            uint64_t prior_end;
            if (prior->type != FBOOT_PT_LOAD || prior->memory_size == 0)
                continue;
            prior_end = prior->virtual_address + prior->memory_size;
            if (ph->virtual_address < prior_end &&
                prior->virtual_address < end)
                return FBOOT_E_FORMAT;
        }
    }

    if (loads == 0 || header->entry < lowest || header->entry >= highest)
        return FBOOT_E_FORMAT;

    result->physical_base = 0;
    result->virtual_base = lowest;
    result->image_size = highest - lowest;
    result->entry = header->entry;
    result->machine = header->machine;
    result->segment_count = loads;
    return FBOOT_OK;
}
