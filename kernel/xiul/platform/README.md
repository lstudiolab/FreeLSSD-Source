# XIUL architecture platform layer

This directory is XIUL-owned glue; it does not replace the imported Mach machine-dependent implementation.

## Supported compile targets

- `x86_64`: privileged CPU/interrupt primitives and a bounded-timeout 16550 COM1 console.
- `arm64`: DAIF interrupt masking, MPIDR CPU identification, WFE/YIELD idle primitives and a boot-information-configured PL011 console.

Both targets use the common `xiul_boot_info_t` hand-off contract. Memory-map and firmware-table addresses are physical. The UART address is a virtual address that must already be mapped by the bootloader/entry path before early console use.

## Build verification

`make -C kernel/xiul arch-build` compiles the common platform validator and architecture-specific platform source into freestanding objects for x86_64 and AArch64. This is a compile check, not a linked or bootable kernel image.

## Current limitations

These are early platform primitives, not a complete board-support package. The x86_64 path assumes legacy COM1 I/O ports. The AArch64 path assumes a PL011-compatible UART supplied by the bootloader. Neither path yet discovers ACPI/UEFI or Device Tree hardware, configures an interrupt controller, initializes SMP, establishes page tables, or enters the imported Mach scheduler. The boot protocol, architecture entry assembly, linker scripts and complete platform bring-up remain to be integrated.
