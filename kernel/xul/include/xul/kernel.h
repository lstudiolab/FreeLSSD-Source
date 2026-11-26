/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#ifndef XUL_KERNEL_H
#define XUL_KERNEL_H
#include <stddef.h>
#include "xul/boot.h"
#include "xul/pci_host.h"
#include "xul/status.h"
#include "xul/subsystem.h"

struct xul_kernel_context {
    xul_boot_info_t boot_info;
    xul_pci_host_t pci_host;
    size_t pci_devices;
};

/* Common non-returning C entry used by the Limine and legacy boot paths. */
_Noreturn void xul_kernel_entry(const xul_boot_info_t *boot_info);

/* Entry called by the architecture boot stub after the handoff is available. */
xul_status_t xul_kernel_boot(const xul_boot_info_t *boot_info);
void xul_kernel_shutdown(void);
const xul_kernel_context_t *xul_kernel_context(void);

#endif
