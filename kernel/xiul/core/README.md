# XIUL Core

The executable core currently contains two tested facilities.

## Subsystem lifecycle manager

`subsystem.c` provides:
- fixed-capacity, allocation-free registration for early boot;
- ABI and descriptor validation;
- dependency existence and phase-order validation;
- dependency-cycle detection;
- deterministic phase-aware startup ordering;
- reverse-order shutdown;
- rollback of already-started subsystems if initialization fails.

Registration is a single-threaded early-boot operation and is rejected after startup begins. Descriptor storage must remain valid for the kernel lifetime. If an initializer fails, it must release any resources it acquired before returning.

## Device and driver registry

`device.c` provides:
- fixed-capacity device and driver registries;
- bus/vendor/device/class matching for platform, PCI, USB and VirtIO identifiers;
- driver probing when either a device or driver is registered;
- one bound driver per device;
- remove callbacks on driver or device unregister;
- explicit descriptor lifetime and external-serialization requirements.

This is XIUL's device ownership point. Linux driver adapters must translate their Linux bus/device and probe/remove interfaces into this registry. The registry does not yet execute unmodified Linux drivers or provide PCI/USB enumeration, interrupts, DMA or power management.

Neither facility replaces Mach scheduling, VM or IPC. Those remain owned by the Mach/platform integration.
