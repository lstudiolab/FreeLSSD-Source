/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include "xul/kernel.h"
#include "xul/platform.h"
#include "xul/pci.h"
#include "xul/subsystem.h"
#include "xul/memory.h"
#include "xul/ipc.h"

static xul_kernel_context_t kernel_context;
static unsigned int kernel_started;

static int boot_contract_valid(const xul_boot_info_t *boot_info)
{
    uint64_t map_bytes;

    if (boot_info == 0 ||
        boot_info->magic != XUL_BOOT_INFO_MAGIC ||
        boot_info->version != XUL_BOOT_INFO_VERSION ||
        boot_info->structure_size < sizeof(*boot_info) ||
        boot_info->memory_map_entries == 0 ||
        boot_info->memory_map_entries > (uint64_t)SIZE_MAX ||
        boot_info->memory_map_physical == 0 ||
        boot_info->memory_map_virtual == 0 ||
        (boot_info->memory_map_physical % _Alignof(xul_memory_region_t)) != 0 ||
        (boot_info->memory_map_virtual % _Alignof(xul_memory_region_t)) != 0 ||
        boot_info->memory_map_entries >
            UINT64_MAX / sizeof(xul_memory_region_t)) {
        return 0;
    }

    map_bytes = boot_info->memory_map_entries * sizeof(xul_memory_region_t);
    return boot_info->memory_map_physical <= UINT64_MAX - map_bytes &&
           boot_info->memory_map_virtual <= UINT64_MAX - map_bytes;
}

static xul_status_t mach_ipc_subsystem_init(xul_kernel_context_t *context)
{
    (void)context;
    return xul_ipc_initialize();
}

static void mach_ipc_subsystem_shutdown(xul_kernel_context_t *context)
{
    (void)context;
    xul_ipc_shutdown();
}

static const xul_subsystem_descriptor_t mach_ipc_subsystem = {
    XUL_ABI_VERSION, "mach-ipc", XUL_PHASE_CORE,
    0, 0, mach_ipc_subsystem_init, mach_ipc_subsystem_shutdown
};

static xul_status_t platform_subsystem_init(xul_kernel_context_t *context)
{
    xul_status_t status = xul_platform_initialize(&context->boot_info);
    const xul_memory_region_t *regions;
    if (status != XUL_OK) return status;
    regions = context->boot_info.memory_map_entries == 0 ? 0 :
        (const xul_memory_region_t *)(uintptr_t)context->boot_info.memory_map_virtual;
    return xul_memory_initialize(regions,
        (size_t)context->boot_info.memory_map_entries);
}

static void platform_subsystem_shutdown(xul_kernel_context_t *context)
{
    (void)context;
}

static xul_status_t pci_subsystem_init(xul_kernel_context_t *context)
{
    xul_status_t status = xul_platform_get_pci_host(&context->pci_host);
    if (status == XUL_E_UNSUPPORTED) return XUL_OK;
    if (status != XUL_OK) return status;
    status = xul_pci_enumerate(&context->pci_host, &context->pci_devices);
    if (status == XUL_E_UNSUPPORTED) return XUL_OK;
    return status;
}

static void pci_subsystem_shutdown(xul_kernel_context_t *context)
{
    if (context->pci_devices != 0) {
        (void)xul_pci_remove_all();
        context->pci_devices = 0;
    }
}

static const xul_subsystem_descriptor_t platform_subsystem = {
    XUL_ABI_VERSION, "platform", XUL_PHASE_PLATFORM,
    0, 0, platform_subsystem_init,
    platform_subsystem_shutdown
};

static const char *const pci_dependencies[] = {"platform"};
static const xul_subsystem_descriptor_t pci_subsystem = {
    XUL_ABI_VERSION, "pci", XUL_PHASE_PLATFORM,
    pci_dependencies, 1, pci_subsystem_init,
    pci_subsystem_shutdown
};

xul_status_t xul_kernel_boot(const xul_boot_info_t *boot_info)
{
    xul_status_t status;
    if (kernel_started || !boot_contract_valid(boot_info))
        return XUL_E_INVALID;
    kernel_context.boot_info = *boot_info;
    kernel_context.pci_devices = 0;
    status = xul_subsystem_register(&mach_ipc_subsystem);
    if (status != XUL_OK) return status;
    status = xul_subsystem_register(&platform_subsystem);
    if (status != XUL_OK) return status;
    status = xul_subsystem_register(&pci_subsystem);
    if (status != XUL_OK) return status;
    status = xul_subsystem_validate();
    if (status != XUL_OK) return status;
    status = xul_subsystem_start_all(&kernel_context);
    if (status == XUL_OK) kernel_started = 1;
    return status;
}

void xul_kernel_shutdown(void)
{
    if (!kernel_started) return;
    xul_subsystem_stop_all(&kernel_context);
    kernel_started = 0;
}

const xul_kernel_context_t *xul_kernel_context(void)
{
    return &kernel_context;
}
