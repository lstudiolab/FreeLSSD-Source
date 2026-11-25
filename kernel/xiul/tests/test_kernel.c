/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "xiul/kernel.h"
#include "xiul/subsystem.h"

static int sequence[8], sequence_count;
static size_t fake_devices;
static xiul_status_t platform_result = XIUL_OK;

xiul_status_t xiul_platform_initialize(const xiul_boot_info_t *boot_info)
{
    assert(boot_info != 0);
    assert(boot_info->magic == XIUL_BOOT_INFO_MAGIC);
    sequence[sequence_count++] = 1;
    return platform_result;
}

xiul_status_t xiul_platform_get_pci_host(xiul_pci_host_t *host)
{
    assert(host != 0);
    sequence[sequence_count++] = 2;
    return XIUL_OK;
}

xiul_status_t xiul_pci_enumerate(const xiul_pci_host_t *host, size_t *found)
{
    assert(host != 0 && found != 0);
    sequence[sequence_count++] = 3;
    *found = fake_devices;
    return XIUL_OK;
}

xiul_status_t xiul_pci_remove_all(void)
{
    sequence[sequence_count++] = 4;
    return XIUL_OK;
}

int main(void)
{
    xiul_boot_info_t boot = {0};
    const xiul_kernel_context_t *context;
    xiul_subsystem_test_reset();
    sequence_count = 0;
    fake_devices = 2;
    boot.magic = XIUL_BOOT_INFO_MAGIC;
    boot.version = XIUL_BOOT_INFO_VERSION;
    boot.structure_size = sizeof(boot);

    assert(xiul_kernel_boot(&boot) == XIUL_OK);
    assert(sequence_count == 3);
    assert(sequence[0] == 1 && sequence[1] == 2 && sequence[2] == 3);
    context = xiul_kernel_context();
    assert(context->pci_devices == 2);
    assert(context->boot_info.magic == XIUL_BOOT_INFO_MAGIC);
    xiul_kernel_shutdown();
    assert(sequence_count == 4 && sequence[3] == 4);

    puts("XIUL kernel boot orchestration tests: PASS");
    return 0;
}
