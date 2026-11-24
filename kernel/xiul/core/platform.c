#include "xiul/platform.h"

#define XIUL_BOOT_INFO_MIN_SIZE ((uint32_t)sizeof(xiul_boot_info_t))

xiul_status_t xiul_platform_initialize(const xiul_boot_info_t *boot_info)
{
    if (boot_info == 0 ||
        boot_info->magic != XIUL_BOOT_INFO_MAGIC ||
        boot_info->version != XIUL_BOOT_INFO_VERSION ||
        boot_info->structure_size < XIUL_BOOT_INFO_MIN_SIZE ||
        (boot_info->memory_map_entries != 0 &&
         boot_info->memory_map_physical == 0) ||
        boot_info->memory_map_entries >
            UINT64_MAX / sizeof(xiul_memory_region_t)) {
        return XIUL_E_INVALID;
    }
    return xiul_arch_early_init(boot_info);
}
