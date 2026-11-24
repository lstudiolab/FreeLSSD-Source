#include "xiul/kernel.h"
#include "xiul/platform.h"
#include "xiul/pci.h"
#include "xiul/subsystem.h"

static xiul_kernel_context_t kernel_context;
static unsigned int kernel_started;

static xiul_status_t platform_subsystem_init(xiul_kernel_context_t *context)
{
    return xiul_platform_initialize(&context->boot_info);
}

static void platform_subsystem_shutdown(xiul_kernel_context_t *context)
{
    (void)context;
}

static xiul_status_t pci_subsystem_init(xiul_kernel_context_t *context)
{
    xiul_status_t status = xiul_platform_get_pci_host(&context->pci_host);
    if (status == XIUL_E_UNSUPPORTED) return XIUL_OK;
    if (status != XIUL_OK) return status;
    status = xiul_pci_enumerate(&context->pci_host, &context->pci_devices);
    if (status == XIUL_E_UNSUPPORTED) return XIUL_OK;
    return status;
}

static void pci_subsystem_shutdown(xiul_kernel_context_t *context)
{
    if (context->pci_devices != 0) {
        (void)xiul_pci_remove_all();
        context->pci_devices = 0;
    }
}

static const xiul_subsystem_descriptor_t platform_subsystem = {
    XIUL_ABI_VERSION, "platform", XIUL_PHASE_PLATFORM,
    0, 0, platform_subsystem_init,
    platform_subsystem_shutdown
};

static const char *const pci_dependencies[] = {"platform"};
static const xiul_subsystem_descriptor_t pci_subsystem = {
    XIUL_ABI_VERSION, "pci", XIUL_PHASE_PLATFORM,
    pci_dependencies, 1, pci_subsystem_init,
    pci_subsystem_shutdown
};

xiul_status_t xiul_kernel_boot(const xiul_boot_info_t *boot_info)
{
    xiul_status_t status;
    if (boot_info == 0 || kernel_started) return XIUL_E_INVALID;
    kernel_context.boot_info = *boot_info;
    kernel_context.pci_devices = 0;
    status = xiul_subsystem_register(&platform_subsystem);
    if (status != XIUL_OK) return status;
    status = xiul_subsystem_register(&pci_subsystem);
    if (status != XIUL_OK) return status;
    status = xiul_subsystem_validate();
    if (status != XIUL_OK) return status;
    status = xiul_subsystem_start_all(&kernel_context);
    if (status == XIUL_OK) kernel_started = 1;
    return status;
}

void xiul_kernel_shutdown(void)
{
    if (!kernel_started) return;
    xiul_subsystem_stop_all(&kernel_context);
    kernel_started = 0;
}

const xiul_kernel_context_t *xiul_kernel_context(void)
{
    return &kernel_context;
}
