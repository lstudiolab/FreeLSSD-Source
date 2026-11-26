<!--
This file has been modified by L,Studio.
Copyright (c) 2026 L,Studio. All rights reserved.
-->

# XUL Core

The executable XUL-owned core currently contains:

## Kernel boot orchestration

`kernel.c` owns a persistent kernel context, copies the boot handoff, registers the platform and PCI subsystems, validates dependencies, and starts them in order. PCI enumeration depends on platform initialization. Shutdown runs in reverse order and removes registered PCI devices before the platform subsystem stops.

This is the current XUL-owned boot coordinator. It is not an architecture entry point and does not yet call Mach's bootstrap, scheduler or VM initialization.

## Subsystem lifecycle manager

`subsystem.c` provides fixed-capacity registration, ABI and descriptor validation, dependency existence and phase-order checks, cycle detection, deterministic startup ordering, reverse shutdown and rollback after initialization failure.

Registration is an early-boot single-threaded operation. Descriptor storage must remain valid for the kernel lifetime. An initializer that fails must release resources it acquired before returning.

## Device and driver registry

`device.c` provides fixed-capacity device and driver registries, bus/vendor/device/class matching, probe/bind, remove/unbind and optional per-driver callback contexts. The callback context allows compatibility adapters to bind a specific upstream driver instance without global callback ambiguity.

Registration and hotplug operations must be externally serialized. Device and driver descriptors and their ID tables must outlive registration.

## PCI enumeration

`pci.c` scans a host-provided PCI bus range, identifies functions, sizes BARs with decode disabled, maps memory BARs through an optional host callback, registers devices and cleans up driver/resource state on removal. Architecture config-space backends are under `platform/x86_64` and `platform/arm64`.

The current platform backends do not yet provide an MMIO virtual mapping implementation, IRQ routing or DMA.

## Platform validation

`platform.c` validates the common boot-information structure before dispatching to the selected architecture's early initialization routine. Architecture-specific CPU, interrupt, console and PCI config-space primitives live under `platform/x86_64` and `platform/arm64`.

## Current limitations

The subsystem registry, PCI enumeration and device registry are XUL-owned mechanisms, not replacements for Mach task/thread, IPC, VM or scheduler code. The platform layer does not yet establish page tables, interrupt controllers, SMP, firmware discovery or the imported Mach machine-dependent runtime.
