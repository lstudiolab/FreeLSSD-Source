/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 *
 * Common C entry reached after the architecture stub installs its stack.
 */
#include "xiul/kernel.h"
#include "xiul/platform.h"

_Noreturn void xiul_kernel_entry(const xiul_boot_info_t *boot_info)
{
    xiul_status_t status = xiul_kernel_boot(boot_info);

    if (status == XIUL_OK) {
        xiul_platform_console_write("XIUL: core bootstrap completed; runtime services are not yet online\n");
    } else {
        xiul_platform_console_write("XIUL: bootstrap failed; halting\n");
    }

    xiul_platform_halt();
}
