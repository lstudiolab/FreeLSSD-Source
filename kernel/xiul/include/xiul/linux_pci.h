#ifndef XIUL_LINUX_PCI_H
#define XIUL_LINUX_PCI_H
#include <stddef.h>
#include <stdint.h>
#include "xiul/device.h"

#define XIUL_LINUX_PCI_BAR_COUNT 6u
#define XIUL_LINUX_PCI_BAR_IO (1u << 0)
#define XIUL_LINUX_PCI_BAR_MEMORY (1u << 1)
#define XIUL_LINUX_PCI_BAR_PREFETCHABLE (1u << 2)
#define XIUL_LINUX_PCI_BAR_64BIT (1u << 3)
#define XIUL_LINUX_PCI_DRIVER_MAX 32u

typedef struct {
    uint16_t vendor;
    uint16_t device;
    uint16_t class_id;
    uint16_t subclass_id;
} xiul_linux_pci_id_t;

/* The XIUL device is first so the registry callback can recover this object. */
struct xiul_pci_host;
typedef struct {
    xiul_device_t device;
    char name_storage[24];
    uint8_t bus_number;
    uint8_t device_number;
    uint8_t function_number;
    const struct xiul_pci_host *host;
    uint16_t original_command;
    uint8_t bar_flags[XIUL_LINUX_PCI_BAR_COUNT];
    uint64_t bar_start[XIUL_LINUX_PCI_BAR_COUNT];
    uint64_t bar_length[XIUL_LINUX_PCI_BAR_COUNT];
    volatile void *bar_mapping[XIUL_LINUX_PCI_BAR_COUNT];
    uint32_t irq;
    void *driver_data;
    unsigned int enabled;
} xiul_linux_pci_device_t;

typedef struct {
    const char *name;
    const xiul_linux_pci_id_t *id_table;
    size_t id_count;
    xiul_status_t (*probe)(xiul_linux_pci_device_t *,
                           const xiul_linux_pci_id_t *);
    void (*remove)(xiul_linux_pci_device_t *);
} xiul_linux_pci_driver_t;

xiul_status_t xiul_linux_pci_register_driver(const xiul_linux_pci_driver_t *driver);
xiul_status_t xiul_linux_pci_unregister_driver(const xiul_linux_pci_driver_t *driver);
xiul_status_t xiul_linux_pci_enable_device(xiul_linux_pci_device_t *device);
void xiul_linux_pci_disable_device(xiul_linux_pci_device_t *device);
xiul_status_t xiul_linux_pci_set_master(xiul_linux_pci_device_t *device);
void xiul_linux_pci_clear_master(xiul_linux_pci_device_t *device);
uint64_t xiul_linux_pci_resource_start(const xiul_linux_pci_device_t *device, unsigned int bar);
uint64_t xiul_linux_pci_resource_length(const xiul_linux_pci_device_t *device, unsigned int bar);
volatile void *xiul_linux_pci_iomap(const xiul_linux_pci_device_t *device, unsigned int bar);
void xiul_linux_pci_set_drvdata(xiul_linux_pci_device_t *device, void *data);
void *xiul_linux_pci_get_drvdata(const xiul_linux_pci_device_t *device);
size_t xiul_linux_pci_driver_count(void);
#ifdef XIUL_TESTING
void xiul_linux_pci_test_reset(void);
#endif
#endif
