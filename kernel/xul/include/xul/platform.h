/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#ifndef XUL_PLATFORM_H
#define XUL_PLATFORM_H

#include <stdint.h>
#include "xul/status.h"
#include "xul/boot.h"
#include "xul/pci_host.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uintptr_t xul_irq_state_t;

/* Internal architecture hook implemented by exactly one platform target. */
xul_status_t xul_arch_early_init(const xul_boot_info_t *boot_info);

/* Called once by the architecture entry path before general subsystem startup. */
xul_status_t xul_platform_initialize(const xul_boot_info_t *boot_info);
xul_status_t xul_platform_get_pci_host(xul_pci_host_t *host);
void xul_platform_console_write(const char *text);
void xul_platform_console_putc(char character);
uint64_t xul_platform_cpu_id(void);
xul_irq_state_t xul_platform_irq_save_disable(void);
void xul_platform_irq_restore(xul_irq_state_t state);
void xul_platform_cpu_relax(void);
void xul_platform_irq_eoi(uint64_t vector);
_Noreturn void xul_platform_idle(void);
_Noreturn void xul_platform_halt(void);

#ifdef __cplusplus
}
#endif
#endif
