# XIUL Core

The executable XIUL-owned core currently contains:

## Subsystem lifecycle manager

`subsystem.c` provides fixed-capacity registration, ABI and descriptor validation, dependency existence and phase-order checks, cycle detection, deterministic startup ordering, reverse shutdown and rollback after initialization failure.

Registration is an early-boot single-threaded operation. Descriptor storage must remain valid for the kernel lifetime. An initializer that fails must release resources it acquired before returning.

## Device and driver registry

`device.c` provides fixed-capacity device and driver registries, bus/vendor/device/class matching, probe/bind, remove/unbind and optional per-driver callback contexts. The callback context exists so compatibility adapters can safely bind a specific upstream driver instance without global callback ambiguity.

Registration and hotplug operations must be externally serialized. Device and driver descriptors and their ID tables must outlive registration.

## Platform validation

`platform.c` validates the common boot-information structure before dispatching to the selected architecture's early initialization routine. Architecture-specific CPU, interrupt and console primitives live under `platform/x86_64` and `platform/arm64`.

This is not yet the Mach bootstrap or a complete platform initialization sequence.

## Current limitations

The subsystem registry and device registry are XIUL-owned mechanisms, not replacements for Mach task/thread, IPC, VM or scheduler code. The platform layer does not yet establish page tables, interrupt controllers, SMP, firmware discovery or the imported Mach machine-dependent runtime.
