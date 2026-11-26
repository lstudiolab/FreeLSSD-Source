/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#ifndef FBOOT_ELF64_H
#define FBOOT_ELF64_H

#include <stdint.h>

#define FBOOT_ELF_MAGIC_0 0x7fu
#define FBOOT_ELF_MAGIC_1 'E'
#define FBOOT_ELF_MAGIC_2 'L'
#define FBOOT_ELF_MAGIC_3 'F'
#define FBOOT_ELFCLASS64 2u
#define FBOOT_ELFDATA2LSB 1u
#define FBOOT_ET_EXEC 2u
#define FBOOT_EM_X86_64 62u
#define FBOOT_EM_AARCH64 183u
#define FBOOT_PT_LOAD 1u
#define FBOOT_PF_X 1u
#define FBOOT_PF_W 2u
#define FBOOT_PF_R 4u
#define FBOOT_ELF_MAX_PHDRS 128u

typedef struct {
    uint8_t ident[16];
    uint16_t type;
    uint16_t machine;
    uint32_t version;
    uint64_t entry;
    uint64_t program_header_offset;
    uint64_t section_header_offset;
    uint32_t flags;
    uint16_t header_size;
    uint16_t program_header_entry_size;
    uint16_t program_header_count;
    uint16_t section_header_entry_size;
    uint16_t section_header_count;
    uint16_t section_name_index;
} fboot_elf64_header_t;

typedef struct {
    uint32_t type;
    uint32_t flags;
    uint64_t offset;
    uint64_t virtual_address;
    uint64_t physical_address;
    uint64_t file_size;
    uint64_t memory_size;
    uint64_t alignment;
} fboot_elf64_program_header_t;

#endif
