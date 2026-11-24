#ifndef XIUL_PLATFORM_H
#define XIUL_PLATFORM_H

#include <stdint.h>
#include "xiul/status.h"
#include "xiul/boot.h"
#include "xiul/pci_host.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uintptr_t xiul_irq_state_t;

/* Internal architecture hook implemented by exactly one platform target. */
xiul_status_t xiul_arch_early_init(const xiul_boot_info_t *boot_info);

/* Called once by the architecture entry path before general subsystem startup. */
xiul_status_t xiul_platform_initialize(const xiul_boot_info_t *boot_info);
xiul_status_t xiul_platform_get_pci_host(xiul_pci_host_t *host);
void xiul_platform_console_write(const char *text);
void xiul_platform_console_putc(char character);
uint64_t xiul_platform_cpu_id(void);
xiul_irq_state_t xiul_platform_irq_save_disable(void);
void xiul_platform_irq_restore(xiul_irq_state_t state);
void xiul_platform_cpu_relax(void);
_Noreturn void xiul_platform_halt(void);

#ifdef __cplusplus
}
#endif
#endif
