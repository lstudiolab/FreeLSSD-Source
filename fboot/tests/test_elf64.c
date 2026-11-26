/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "fboot/loader.h"

static uint8_t image[1024];

static void make_image(void)
{
    fboot_elf64_header_t *h = (fboot_elf64_header_t *)image;
    fboot_elf64_program_header_t *p =
        (fboot_elf64_program_header_t *)(image + sizeof(*h));

    memset(image, 0, sizeof(image));
    h->ident[0] = FBOOT_ELF_MAGIC_0;
    h->ident[1] = FBOOT_ELF_MAGIC_1;
    h->ident[2] = FBOOT_ELF_MAGIC_2;
    h->ident[3] = FBOOT_ELF_MAGIC_3;
    h->ident[4] = FBOOT_ELFCLASS64;
    h->ident[5] = FBOOT_ELFDATA2LSB;
    h->ident[6] = 1;
    h->type = FBOOT_ET_EXEC;
    h->machine = FBOOT_EM_X86_64;
    h->version = 1;
    h->entry = UINT64_C(0xffffffff80000000);
    h->program_header_offset = sizeof(*h);
    h->header_size = sizeof(*h);
    h->program_header_entry_size = sizeof(*p);
    h->program_header_count = 2;

    p[0].type = FBOOT_PT_LOAD;
    p[0].flags = FBOOT_PF_R | FBOOT_PF_X;
    p[0].offset = 256;
    p[0].virtual_address = UINT64_C(0xffffffff80000000);
    p[0].file_size = 64;
    p[0].memory_size = 4096;
    p[0].alignment = 1;

    p[1].type = FBOOT_PT_LOAD;
    p[1].flags = FBOOT_PF_R | FBOOT_PF_W;
    p[1].offset = 512;
    p[1].virtual_address = UINT64_C(0xffffffff80001000);
    p[1].file_size = 32;
    p[1].memory_size = 4096;
    p[1].alignment = 1;
}

int main(void)
{
    fboot_loaded_image_t result;
    fboot_elf64_header_t *h;
    fboot_elf64_program_header_t *p;

    make_image();
    assert(fboot_elf64_validate(image, sizeof(image),
                                FBOOT_EM_X86_64, &result) == FBOOT_OK);
    assert(result.entry == UINT64_C(0xffffffff80000000));
    assert(result.segment_count == 2);
    assert(result.image_size == 8192);

    assert(fboot_elf64_validate(image, sizeof(image) - 1,
                                FBOOT_EM_X86_64, &result) == FBOOT_OK);
    assert(fboot_elf64_validate(image, sizeof(image),
                                FBOOT_EM_AARCH64, &result) == FBOOT_E_FORMAT);

    make_image();
    h = (fboot_elf64_header_t *)image;
    h->program_header_offset = UINT64_MAX - 8;
    assert(fboot_elf64_validate(image, sizeof(image),
                                FBOOT_EM_X86_64, &result) == FBOOT_E_RANGE);

    make_image();
    p = (fboot_elf64_program_header_t *)(image + sizeof(*h));
    p[1].virtual_address = p[0].virtual_address + 2048;
    assert(fboot_elf64_validate(image, sizeof(image),
                                FBOOT_EM_X86_64, &result) == FBOOT_E_FORMAT);

    make_image();
    p = (fboot_elf64_program_header_t *)(image + sizeof(*h));
    p[0].flags |= FBOOT_PF_W;
    assert(fboot_elf64_validate(image, sizeof(image),
                                FBOOT_EM_X86_64, &result) == FBOOT_E_SECURITY);

    make_image();
    h = (fboot_elf64_header_t *)image;
    h->entry = UINT64_C(0xffffffff90000000);
    assert(fboot_elf64_validate(image, sizeof(image),
                                FBOOT_EM_X86_64, &result) == FBOOT_E_FORMAT);

    puts("fBoot ELF64 validation tests: PASS");
    return 0;
}
