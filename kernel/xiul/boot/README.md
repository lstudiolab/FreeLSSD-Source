<!--
This file has been modified by L,Studio.
Copyright (c) 2026 L,Studio. All rights reserved.
-->

# XIUL boot contract

XIUL's common kernel entry consumes the versioned `xiul_boot_info_t`
structure defined in `include/xiul/boot.h`. The current hand-off ABI version
is 2.

## Limine boot path

The x86_64 and AArch64 ELF images are linked with `xiul_limine_entry` as
their entry point. They include Limine protocol requests for:

- the higher-half direct map (HHDM);
- the firmware memory map;
- the kernel image's physical and virtual base addresses;
- a 64 KiB boot stack.

The linker scripts retain the request markers and request records. Limine
loads and maps the ELF image, establishes the architecture's initial execution
state, and transfers control to `xiul_limine_entry`. XIUL validates the
responses, copies Limine's pointer-array memory map into a bounded contiguous
array, converts usable regions to XIUL's conventional-memory type, derives
the physical address of that array, and enters `xiul_kernel_entry`.

Use `make -C kernel/xiul limine-image-x86_64` or
`make -C kernel/xiul limine-image-arm64` to create an architecture-specific
bootable ISO. The build downloads the official Limine v12.9.6 binary package
and verifies its SHA-256 before packaging. The Limine bootloader remains a
separate component and is not vendored into XIUL. Disk-image packaging is not
included.

## Legacy loader hand-off

The retained `xiul_arch_entry` assembly stubs support loaders that implement
XIUL's original hand-off ABI directly. On x86_64, the loader passes a pointer
to `xiul_boot_info_t` in RDI while in 64-bit long mode. On AArch64, it passes
the pointer in X0 at EL1 or EL2. In both cases, the loader must map the kernel
image, hand-off structure and memory-map array, provide a valid stack, and
mask interrupts before transferring control.

## Current limitations

Limine boot support does not mean the kernel's runtime is complete. The
AArch64 platform still needs device-tree parsing for UART and PCI ECAM
discovery. Neither architecture yet has a complete Mach pmap/VM bootstrap,
interrupt-controller routing, timer subsystem or SMP bring-up. Exception
vectors are diagnostic fail-stop handlers. The x86_64 ISO has a QEMU smoke test in GitHub Actions. AArch64 ISO runtime
boot verification, a full VM bootstrap and disk-image packaging remain open.

Do not describe XIUL as a complete operating system kernel until its Mach
runtime, Linux driver environment, BSD services and end-to-end hardware paths
have been integrated and tested.
