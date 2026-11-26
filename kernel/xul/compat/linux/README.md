<!--
This file has been modified by L,Studio.
Copyright (c) 2026 L,Studio. All rights reserved.
-->

# Linux Driver Environment

Linux is included in XUL to reuse its extensive hardware-driver ecosystem. Linux driver source remains in `linux/`; XUL must not rewrite those drivers as unrelated native drivers.

The compatibility environment preserves the Linux driver model and implements the kernel services required by each selected driver on top of XUL's Mach/platform and device facilities. It is not enough to provide Linux headers: each driver needs working implementations of every kernel API it uses.

## Device ownership

- XUL's device framework is the authority for physical device ownership and lifecycle.
- A device is bound to either one Mach-native driver or one Linux driver at a time.
- Linux drivers receive validated resources through the compatibility environment.
- Probe failure must release every resource acquired by the driver.
- Remove/handoff must stop asynchronous work, mask and synchronize interrupts, stop DMA, release mappings and detach the device before ownership changes.

## Compatibility services

A driver port must declare and test its use of:

- Linux device, bus and driver-model APIs;
- PCI/USB enumeration and resource management;
- MMIO and port I/O mapping;
- coherent and streaming DMA mapping/synchronization;
- IRQ registration, masking and synchronization;
- memory allocation, page mapping and reference management;
- locks, atomics, wait queues and memory ordering;
- timers, workqueues and deferred execution;
- power management, reset and hotplug lifecycle;
- logging, tracing and error reporting.

The current `pci.c` adapter is only a narrow PCI registration bridge. It does not make arbitrary upstream Linux drivers buildable or runnable. Platform MMIO mapping, interrupt routing, DMA, most Linux kernel APIs and real driver ports remain outstanding.

## Network devices

A Linux network driver owns the NIC hardware through XUL's device framework. Its transmit/receive queues are connected through a defined packet adapter to BSD networking. The Linux network protocol stack is not used as a replacement for the BSD stack.

## Driver acceptance criteria

A driver is considered integrated only when its unmodified or explicitly patched upstream source:
1. builds against the pinned Linux source revision and XUL compatibility headers;
2. binds through XUL's device ownership framework;
3. handles interrupts and DMA safely;
4. passes probe/remove and failure-unwind tests;
5. passes an emulator or physical-device test.

Preserve the original Linux license and copyright notices. Review GPL obligations before linking or distributing Linux-derived code.
