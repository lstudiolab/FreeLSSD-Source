#ifndef XIUL_KERNEL_H
#define XIUL_KERNEL_H
#include <stddef.h>
#include "xiul/boot.h"
#include "xiul/pci_host.h"
#include "xiul/status.h"
#include "xiul/subsystem.h"

struct xiul_kernel_context {
    xiul_boot_info_t boot_info;
    xiul_pci_host_t pci_host;
    size_t pci_devices;
};

/* Entry called by the architecture boot stub after the handoff is available. */
xiul_status_t xiul_kernel_boot(const xiul_boot_info_t *boot_info);
void xiul_kernel_shutdown(void);
const xiul_kernel_context_t *xiul_kernel_context(void);

#endif
