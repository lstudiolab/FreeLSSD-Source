/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#ifndef XUL_LINUX_PCI_H
#define XUL_LINUX_PCI_H
#include <stddef.h>
#include <stdint.h>
#include "xul/device.h"
#include "xul/pci.h"

#define XUL_LINUX_PCI_BAR_COUNT 6u
#define XUL_LINUX_PCI_BAR_IO (1u << 0)
#define XUL_LINUX_PCI_BAR_MEMORY (1u << 1)
#define XUL_LINUX_PCI_BAR_PREFETCHABLE (1u << 2)
#define XUL_LINUX_PCI_BAR_64BIT (1u << 3)
#define XUL_LINUX_PCI_DRIVER_MAX 32u

typedef struct {
    uint16_t vendor;
    uint16_t device;
    uint16_t class_id;
    uint16_t subclass_id;
} xul_linux_pci_id_t;

/* The XUL device is first so the registry callback can recover this object. */
typedef struct {
    xul_device_t device;
    char name_storage[24];
    uint8_t bus_number;
    uint8_t device_number;
    uint8_t function_number;
    const xul_pci_host_t *host;
    uint16_t original_command;
    uint8_t bar_flags[XUL_LINUX_PCI_BAR_COUNT];
    uint64_t bar_start[XUL_LINUX_PCI_BAR_COUNT];
    uint64_t bar_length[XUL_LINUX_PCI_BAR_COUNT];
    volatile void *bar_mapping[XUL_LINUX_PCI_BAR_COUNT];
    uint32_t irq;
    void *driver_data;
    unsigned int enabled;
} xul_linux_pci_device_t;

typedef struct {
    const char *name;
    const xul_linux_pci_id_t *id_table;
    size_t id_count;
    xul_status_t (*probe)(xul_linux_pci_device_t *,
                           const xul_linux_pci_id_t *);
    void (*remove)(xul_linux_pci_device_t *);
} xul_linux_pci_driver_t;

xul_status_t xul_linux_pci_register_driver(const xul_linux_pci_driver_t *driver);
xul_status_t xul_linux_pci_unregister_driver(const xul_linux_pci_driver_t *driver);
xul_status_t xul_linux_pci_enable_device(xul_linux_pci_device_t *device);
void xul_linux_pci_disable_device(xul_linux_pci_device_t *device);
xul_status_t xul_linux_pci_set_master(xul_linux_pci_device_t *device);
void xul_linux_pci_clear_master(xul_linux_pci_device_t *device);
uint64_t xul_linux_pci_resource_start(const xul_linux_pci_device_t *device, unsigned int bar);
uint64_t xul_linux_pci_resource_length(const xul_linux_pci_device_t *device, unsigned int bar);
volatile void *xul_linux_pci_iomap(const xul_linux_pci_device_t *device, unsigned int bar);
void xul_linux_pci_set_drvdata(xul_linux_pci_device_t *device, void *data);
void *xul_linux_pci_get_drvdata(const xul_linux_pci_device_t *device);
size_t xul_linux_pci_driver_count(void);
#ifdef XUL_TESTING
void xul_linux_pci_test_reset(void);
#endif
#endif
