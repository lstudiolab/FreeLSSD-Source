#include "xiul/pci.h"
#include "xiul/device.h"
#include "xiul/linux_pci.h"

typedef struct {
    xiul_linux_pci_device_t pci;
    const xiul_pci_host_t *host;
} xiul_pci_slot_t;

static xiul_pci_slot_t slots[XIUL_PCI_MAX_ENUMERATED_DEVICES];
static size_t slot_count;

static xiul_status_t read32(const xiul_pci_host_t *host, uint8_t bus,
                            uint8_t device, uint8_t function,
                            uint16_t offset, uint32_t *value)
{
    if ((offset & 3u) != 0 || offset >= XIUL_PCI_CONFIG_SPACE_SIZE)
        return XIUL_E_INVALID;
    return host->read32(host->context, bus, device, function, offset, value);
}

static xiul_status_t read16(const xiul_pci_host_t *host, uint8_t bus,
                            uint8_t device, uint8_t function,
                            uint16_t offset, uint16_t *value)
{
    uint32_t word;
    xiul_status_t status;
    if ((offset & 1u) != 0 || offset >= XIUL_PCI_CONFIG_SPACE_SIZE)
        return XIUL_E_INVALID;
    status = read32(host, bus, device, function, (uint16_t)(offset & ~3u), &word);
    if (status == XIUL_OK)
        *value = (uint16_t)(word >> ((offset & 2u) * 8u));
    return status;
}

static xiul_status_t write16(const xiul_pci_host_t *host, uint8_t bus,
                             uint8_t device, uint8_t function,
                             uint16_t offset, uint16_t value)
{
    return host->write16(host->context, bus, device, function, offset, value);
}

static void make_name(char *output, uint8_t bus, uint8_t device, uint8_t function)
{
    static const char hex[] = "0123456789abcdef";
    output[0] = 'p'; output[1] = 'c'; output[2] = 'i';
    output[3] = hex[(bus >> 4) & 0xf]; output[4] = hex[bus & 0xf];
    output[5] = ':';
    output[6] = hex[(device >> 4) & 0xf]; output[7] = hex[device & 0xf];
    output[8] = '.';
    output[9] = (char)('0' + function);
    output[10] = '\0';
}

static void release_slot(xiul_pci_slot_t *slot)
{
    unsigned int i;
    if (slot->pci.device.bound_driver != 0)
        (void)xiul_device_unregister(&slot->pci.device);
    else if (slot->pci.device.name != 0)
        (void)xiul_device_unregister(&slot->pci.device);
    if (slot->host != 0 && slot->host->unmap_mmio != 0) {
        for (i = 0; i < XIUL_LINUX_PCI_BAR_COUNT; ++i) {
            if (slot->pci.bar_mapping[i] != 0 && slot->pci.bar_length[i] <= (uint64_t)SIZE_MAX) {
                slot->host->unmap_mmio(slot->host->context,
                    slot->pci.bar_mapping[i], (size_t)slot->pci.bar_length[i]);
            }
        }
    }
    slot->host = 0;
    slot->pci.device.name = 0;
}

static xiul_status_t probe_bar(const xiul_pci_host_t *host,
                               uint8_t bus, uint8_t device, uint8_t function,
                               unsigned int bar, xiul_linux_pci_device_t *pci)
{
    uint16_t offset = (uint16_t)(0x10u + bar * 4u);
    uint32_t original_low, mask_low, original_high = 0, mask_high = 0;
    uint64_t base, mask, length;
    unsigned int type;
    xiul_status_t status;

    status = read32(host, bus, device, function, offset, &original_low);
    if (status != XIUL_OK || original_low == 0 || original_low == UINT32_MAX)
        return status;

    if ((original_low & 1u) != 0) {
        base = original_low & ~UINT32_C(3);
        pci->bar_start[bar] = base;
        return XIUL_OK;
    }

    type = (original_low >> 1) & 3u;
    if (type == 3u) return XIUL_E_UNSUPPORTED;
    if (type == 2u) {
        if (bar + 1u >= XIUL_LINUX_PCI_BAR_COUNT) return XIUL_E_INVALID;
        status = read32(host, bus, device, function, (uint16_t)(offset + 4u), &original_high);
        if (status != XIUL_OK) return status;
    }

    status = write16(host, bus, device, function, 0x04u, 0);
    if (status != XIUL_OK) return status;
    status = host->write32(host->context, bus, device, function, offset, UINT32_MAX);
    if (status == XIUL_OK && type == 2u)
        status = host->write32(host->context, bus, device, function,
                               (uint16_t)(offset + 4u), UINT32_MAX);
    if (status == XIUL_OK)
        status = read32(host, bus, device, function, offset, &mask_low);
    if (status == XIUL_OK && type == 2u)
        status = read32(host, bus, device, function,
                        (uint16_t)(offset + 4u), &mask_high);

    (void)host->write32(host->context, bus, device, function, offset, original_low);
    if (type == 2u)
        (void)host->write32(host->context, bus, device, function,
                            (uint16_t)(offset + 4u), original_high);
    (void)write16(host, bus, device, function, 0x04u, 0x0007u);
    if (status != XIUL_OK) return status;

    if (type == 2u) {
        base = ((uint64_t)original_high << 32) | (original_low & ~UINT32_C(0xf));
        mask = ((uint64_t)mask_high << 32) | (mask_low & ~UINT32_C(0xf));
    } else {
        base = original_low & ~UINT32_C(0xf);
        mask = mask_low & ~UINT32_C(0xf);
    }
    if (mask == 0) return XIUL_OK;
    length = (~mask) + 1u;
    if (length == 0) return XIUL_OK;
    pci->bar_start[bar] = base;
    pci->bar_length[bar] = length;
    if (host->map_mmio != 0 && length <= (uint64_t)SIZE_MAX)
        pci->bar_mapping[bar] = host->map_mmio(host->context, base, (size_t)length);
    return XIUL_OK;
}

static xiul_status_t enumerate_function(const xiul_pci_host_t *host,
                                        uint8_t bus, uint8_t device,
                                        uint8_t function, size_t *found)
{
    uint32_t identity, class_revision, header_register, interrupt_register;
    uint16_t command;
    unsigned int bar, bar_limit;
    xiul_status_t status;
    xiul_pci_slot_t *slot;
    uint16_t vendor, product;

    status = read32(host, bus, device, function, 0x00u, &identity);
    if (status != XIUL_OK) return status;
    vendor = (uint16_t)identity;
    product = (uint16_t)(identity >> 16);
    if (vendor == UINT16_MAX || vendor == 0) return XIUL_E_NOT_FOUND;
    if (slot_count == XIUL_PCI_MAX_ENUMERATED_DEVICES) return XIUL_E_CAPACITY;
    status = read32(host, bus, device, function, 0x08u, &class_revision);
    if (status != XIUL_OK) return status;
    status = read32(host, bus, device, function, 0x0cu, &header_register);
    if (status != XIUL_OK) return status;
    status = read32(host, bus, device, function, 0x3cu, &interrupt_register);
    if (status != XIUL_OK) return status;
    status = read16(host, bus, device, function, 0x04u, &command);
    if (status != XIUL_OK) return status;

    slot = &slots[slot_count];
    slot->host = host;
    slot->pci.device.id.bus = XIUL_BUS_PCI;
    slot->pci.device.id.vendor_id = vendor;
    slot->pci.device.id.device_id = product;
    slot->pci.device.id.class_id = (uint16_t)(class_revision >> 24);
    slot->pci.device.id.subclass_id = (uint16_t)((class_revision >> 16) & 0xffu);
    slot->pci.bus_number = bus;
    slot->pci.device_number = device;
    slot->pci.function_number = function;
    slot->pci.irq = interrupt_register & 0xffu;
    make_name(slot->pci.name_storage, bus, device, function);
    slot->pci.device.name = slot->pci.name_storage;

    bar_limit = ((header_register >> 16) & 0x7fu) == 0 ? 6u : 2u;
    for (bar = 0; bar < bar_limit; ++bar) {
        uint32_t bar_low;
        status = read32(host, bus, device, function,
                        (uint16_t)(0x10u + bar * 4u), &bar_low);
        if (status != XIUL_OK) break;
        status = probe_bar(host, bus, device, function, bar, &slot->pci);
        if (status == XIUL_E_UNSUPPORTED) { status = XIUL_OK; continue; }
        if (status != XIUL_OK) break;
        if ((bar_low & 1u) == 0 && ((bar_low >> 1) & 3u) == 2u) ++bar;
    }
    (void)write16(host, bus, device, function, 0x04u, command);
    if (status != XIUL_OK) {
        slot->host = 0;
        slot->pci.device.name = 0;
        return status;
    }
    status = xiul_device_register(&slot->pci.device);
    if (status != XIUL_OK) {
        unsigned int i;
        if (host->unmap_mmio != 0) {
            for (i = 0; i < XIUL_LINUX_PCI_BAR_COUNT; ++i)
                if (slot->pci.bar_mapping[i] != 0 &&
                    slot->pci.bar_length[i] <= (uint64_t)SIZE_MAX)
                    host->unmap_mmio(host->context, slot->pci.bar_mapping[i],
                                     (size_t)slot->pci.bar_length[i]);
        }
        slot->host = 0;
        slot->pci.device.name = 0;
        return status;
    }
    ++slot_count;
    ++*found;
    return XIUL_OK;
}

xiul_status_t xiul_pci_enumerate(const xiul_pci_host_t *host, size_t *found)
{
    uint16_t bus;
    size_t count = 0;
    xiul_status_t status;
    if (host == 0 || found == 0 || host->read16 == 0 || host->write16 == 0 ||
        host->read32 == 0 || host->write32 == 0 ||
        host->first_bus > host->last_bus || slot_count != 0)
        return XIUL_E_INVALID;
    *found = 0;
    for (bus = host->first_bus; bus <= host->last_bus; ++bus) {
        uint8_t device;
        for (device = 0; device < 32u; ++device) {
            uint32_t header;
            uint16_t vendor;
            status = read16(host, (uint8_t)bus, device, 0, 0x00u, &vendor);
            if (status != XIUL_OK || vendor == UINT16_MAX || vendor == 0) continue;
            status = read32(host, (uint8_t)bus, device, 0, 0x0cu, &header);
            if (status != XIUL_OK) continue;
            status = enumerate_function(host, (uint8_t)bus, device, 0, &count);
            if (status != XIUL_OK && status != XIUL_E_NOT_FOUND) goto fail;
            if (((header >> 16) & 0x80u) != 0) {
                uint8_t function;
                for (function = 1; function < 8u; ++function) {
                    status = enumerate_function(host, (uint8_t)bus, device,
                                                 function, &count);
                    if (status != XIUL_OK && status != XIUL_E_NOT_FOUND) goto fail;
                }
            }
        }
        if (bus == UINT8_MAX) break;
    }
    *found = count;
    return XIUL_OK;
fail:
    (void)xiul_pci_remove_all();
    return status;
}

xiul_status_t xiul_pci_remove_all(void)
{
    while (slot_count != 0) {
        xiul_pci_slot_t *slot = &slots[--slot_count];
        release_slot(slot);
    }
    return XIUL_OK;
}

size_t xiul_pci_device_count(void) { return slot_count; }

#ifdef XIUL_TESTING
void xiul_pci_test_reset(void)
{
    (void)xiul_pci_remove_all();
}
#endif
