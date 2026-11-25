<!--
This file has been modified by L,Studio.
Copyright (c) 2026 L,Studio. All rights reserved.
-->

# XIUL architecture platform layer

This directory is XIUL-owned glue; it does not replace the imported Mach machine-dependent implementation.

## Supported compile targets

- `x86_64`: privileged CPU/interrupt primitives and a bounded-timeout 16550 COM1 console.
- `arm64`: DAIF interrupt masking, MPIDR CPU identification, WFE/YIELD idle primitives, a boot-information-configured PL011 console and PCIe ECAM config-space access.\n- `x86_64`: legacy PCI configuration mechanism #1 (CF8/CFC), serialized across CPUs and with local interrupts masked during each config transaction.

Both targets use the common `xiul_boot_info_t` hand-off contract. Memory-map and firmware-table addresses are physical. The UART address is a virtual address that must already be mapped by the bootloader/entry path before early console use.

## Build verification

`make -C kernel/xiul arch-build` compiles the common platform validator and architecture-specific platform source into freestanding objects for x86_64 and AArch64. This is a compile check, not a linked or bootable kernel image.

## Current limitations

These are early platform primitives, not a complete board-support package. The x86_64 path assumes legacy COM1 I/O ports and legacy PCI configuration mechanism #1. The AArch64 path assumes a PL011-compatible UART and an already mapped PCIe ECAM region supplied by platform firmware. Neither path yet discovers ACPI/UEFI or Device Tree hardware, configures an interrupt controller, initializes SMP, establishes page tables, supplies an MMIO mapping implementation, or enters the imported Mach scheduler. The boot protocol, architecture entry assembly, linker scripts and complete platform bring-up remain to be integrated.
