<!--
This file has been modified by L,Studio.
Copyright (c) 2026 L,Studio. All rights reserved.
-->

# Linux Driver Compatibility Layer

Linux source is used as a pool for selected hardware drivers. Linux is not a second kernel and its scheduler, VM, networking stack, VFS or userspace ABI are not XIUL subsystems.

## Implemented first adapter

`pci.c` implements a narrow XIUL PCI-driver registration bridge:
- translates Linux-style PCI vendor/device/class match tables into XIUL device IDs;
- binds a selected driver through the shared XIUL device registry;
- forwards probe/remove callbacks with per-driver context;
- exposes BAR start/length/mapped-address accessors and driver-private data;
- tracks device enable state.

The generic PCI enumerator now scans a host-provided bus range, identifies functions, probes and sizes I/O and memory BARs with decode disabled, maps memory BARs through an optional host callback, registers devices in the shared XIUL device registry and unmaps resources on removal. x86_64 legacy CF8/CFC and AArch64 ECAM configuration-space backends are implemented and cross-compiled. The platform VM layer still needs to provide safe MMIO mappings; IRQ routing/allocation, DMA, power management and a Linux kernel API header environment are not implemented here. This adapter does not make arbitrary upstream Linux PCI drivers source-compatible yet.

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
