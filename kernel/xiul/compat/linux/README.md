# Linux Driver Compatibility Layer

Linux source is used as a pool for selected hardware drivers. Linux is not a second kernel and its scheduler, VM, networking stack, VFS or userspace ABI are not XIUL subsystems.

## Implemented first adapter

`pci.c` implements a narrow XIUL PCI-driver registration bridge:
- translates Linux-style PCI vendor/device/class match tables into XIUL device IDs;
- binds a selected driver through the shared XIUL device registry;
- forwards probe/remove callbacks with per-driver context;
- exposes BAR start/length/mapped-address accessors and driver-private data;
- tracks device enable state.

The current BAR mapping and enable functions are metadata-level hooks only. PCI enumeration, configuration-space access, resource reservation, MMIO mapping, IRQ allocation, DMA, power management and a Linux kernel API header environment are not implemented here. This adapter does not make arbitrary upstream Linux PCI drivers source-compatible yet.

## Remaining compatibility APIs

Each selected driver still needs a dependency inventory and the relevant XIUL-backed implementation for:
- memory allocation and mapping;
- DMA mapping and synchronization;
- interrupt registration;
- locks, atomics and memory ordering;
- timers, workqueues and deferred execution;
- power-management and device lifecycle;
- logging and error reporting.

Drivers are not considered ported merely because their source files are present. Each driver requires an explicit dependency list, adapter coverage, build result and hardware or emulator test.

Run `make -C kernel/xiul test` for the PCI bridge registration and lifecycle tests.
