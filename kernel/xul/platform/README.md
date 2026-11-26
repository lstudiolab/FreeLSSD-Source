<!--
This file has been modified by L,Studio.
Copyright (c) 2026 L,Studio. All rights reserved.
-->

# XUL architecture platform layer

This directory is XUL-owned glue; it does not replace the imported Mach machine-dependent implementation.

## Exception handling

- `x86_64`: installs a 256-entry IDT using the active code-segment selector. Every vector has a dedicated stub; vectors with architectural error codes preserve the CPU-pushed error-code slot. Unhandled vectors enter a fail-stop handler.
- `arm64`: installs a 2 KiB-aligned 16-slot EL1 VBAR table. All vector classes currently enter a fail-stop handler. The loader must enter at EL1.

These tables establish deterministic exception entry, not a functioning interrupt subsystem. GIC/APIC initialization, IRQ routing, timer interrupts, exception recovery and scheduling are not implemented.

## Supported compile targets

- `x86_64`: privileged CPU/interrupt primitives and a bounded-timeout 16550 COM1 console.
- `arm64`: DAIF interrupt masking, MPIDR CPU identification, WFE/YIELD idle primitives, a boot-information-configured PL011 console and PCIe ECAM config-space access.\n- `x86_64`: legacy PCI configuration mechanism #1 (CF8/CFC), serialized across CPUs and with local interrupts masked during each config transaction.

Both targets use the common `xul_boot_info_t` hand-off contract. Memory-map and firmware-table addresses are physical. The UART address is a virtual address that must already be mapped by the bootloader/entry path before early console use.

## Build verification

`make -C kernel/xul arch-build` compiles the common platform validator and architecture-specific platform source into freestanding objects for x86_64 and AArch64. This is a compile check, not a linked or bootable kernel image.

## Current limitations

These are early platform primitives, not a complete board-support package. The x86_64 path assumes legacy COM1 I/O ports and legacy PCI configuration mechanism #1. The AArch64 path assumes a PL011-compatible UART and an already mapped PCIe ECAM region supplied by platform firmware. Neither path yet discovers ACPI/UEFI or Device Tree hardware, configures an interrupt controller, initializes SMP, establishes page tables, supplies an MMIO mapping implementation, or enters the imported Mach scheduler. The boot protocol, architecture entry assembly, linker scripts and complete platform bring-up remain to be integrated.
