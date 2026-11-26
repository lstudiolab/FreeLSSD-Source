/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 *
 * Common C entry reached after the architecture stub installs its stack.
 */
#include "xul/kernel.h"
#include "xul/platform.h"

_Noreturn void xul_kernel_entry(const xul_boot_info_t *boot_info)
{
    xul_status_t status = xul_kernel_boot(boot_info);

    if (status == XUL_OK) {
        xul_platform_console_write("XUL: core bootstrap completed; runtime services are not yet online\n");
    } else {
        xul_platform_console_write("XUL: bootstrap failed; halting\n");
    }

    xul_platform_halt();
}
