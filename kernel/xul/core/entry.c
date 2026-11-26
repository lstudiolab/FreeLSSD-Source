/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 *
 * Common C entry reached after the architecture stub installs its stack.
 * Keep early-boot diagnostics freestanding: no libc or formatted I/O.
 */
#include "xul/kernel.h"
#include "xul/platform.h"
#include "xul/memory.h"

static void write_unsigned(uint64_t value)
{
    char digits[20];
    unsigned int count = 0;

    do {
        digits[count++] = (char)('0' + (value % 10u));
        value /= 10u;
    } while (value != 0 && count < sizeof(digits));

    while (count != 0) {
        xul_platform_console_putc(digits[--count]);
    }
}

_Noreturn void xul_kernel_entry(const xul_boot_info_t *boot_info)
{
    xul_status_t status = xul_kernel_boot(boot_info);

    if (status == XUL_OK) {
        const xul_kernel_context_t *context = xul_kernel_context();
        xul_memory_stats_t memory;

        xul_memory_get_stats(&memory);
        xul_platform_console_write("XUL: core bootstrap completed\n");
        xul_platform_console_write("XUL: physical memory allocator online; total_pages=");
        write_unsigned(memory.total_pages);
        xul_platform_console_write(" free_pages=");
        write_unsigned(memory.free_pages);
        xul_platform_console_write("\n");
        xul_platform_console_write("XUL: PCI devices enumerated=");
        write_unsigned((uint64_t)context->pci_devices);
        xul_platform_console_write("\n");
        xul_platform_console_write("XUL: timer configured; entering interrupt-driven idle loop\n");
        xul_platform_idle();
    } else {
        xul_platform_console_write("XUL: bootstrap failed; status=");
        write_unsigned((uint64_t)status);
        xul_platform_console_write("; halting\n");
    }

    xul_platform_halt();
}
