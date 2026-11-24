# XIUL architecture platform layer

This directory is XIUL-owned glue; it does not replace the imported Mach machine-dependent implementation.

## Supported compile targets

- `x86_64`: privileged CPU/interrupt primitives and a bounded-timeout 16550 COM1 console.
- `arm64`: DAIF interrupt masking, MPIDR CPU identification, WFE/YIELD idle primitives and a boot-information-configured PL011 console.

Both targets use the common `xiul_boot_info_t` hand-off contract. Physical addresses in the hand-off are not automatically mapped: the architecture entry code and Mach VM bootstrap must establish mappings before dereferencing them.

## Current limitations

These are early platform primitives, not a complete board-support package. The x86_64 path assumes legacy COM1 I/O ports. The AArch64 path assumes a PL011-compatible UART supplied by the bootloader. Neither path yet discovers ACPI/UEFI or Device Tree hardware, configures an interrupt controller, initializes SMP, establishes page tables, or enters the imported Mach scheduler. The boot protocol and architecture entry assembly remain to be integrated.
