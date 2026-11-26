<!--
This file has been modified by L,Studio.
Copyright (c) 2026 L,Studio. All rights reserved.
-->

# fBoot

fBoot is the native FreeLSSD boot manager and bootloader. It is an L,Studio implementation and is not based on Limine, GRUB, systemd-boot, or Apple's proprietary boot code.

The name is written **fBoot**. Its responsibilities are deliberately separated from XUL:

1. Firmware entry and platform discovery.
2. Boot selection and configuration.
3. Secure validation of the selected kernel image.
4. ELF64 loading and relocation policy.
5. Physical memory-map capture and architecture page-table setup.
6. A versioned hand-off to XUL.
7. Firmware-service shutdown before kernel ownership begins.

## Design

fBoot uses UEFI as the firmware interface. It does not use UEFI as its own bootloader implementation: fBoot owns the boot policy, image loader, memory preparation, architecture transition and XUL hand-off. UEFI supplies firmware protocols for filesystems, graphics, input, memory allocation and firmware tables.

Initial target order:

- x86_64 UEFI: first implementation and QEMU OVMF validation.
- AArch64 UEFI: same protocol and policy, separate architecture transition and page-table code.
- Legacy BIOS is not in the initial scope.

## Boot flow

```text
UEFI firmware
  -> fBoot.efi
      -> discover boot volume and configuration
      -> load and validate XUL ELF64 image
      -> allocate and map PT_LOAD segments
      -> prepare XUL boot information and memory map
      -> prepare architecture page tables and stack
      -> ExitBootServices
      -> transfer control to XUL entry
          -> XUL owns CPU, memory and devices
```

## Security requirements

- Reject malformed ELF headers, unsupported machine types, invalid program-header tables, integer overflows, overlapping load segments and out-of-range file data.
- Never execute a segment without execute permission or write a read-only segment after relocation.
- Do not call boot services after a successful ExitBootServices.
- Retry GetMemoryMap/ExitBootServices correctly if the map key becomes stale.
- Validate configuration paths and bound all allocations.
- Treat secure boot verification as a required future policy gate before claiming a production secure-boot chain.

## Current implementation status

This directory establishes the independent fBoot design and boot contract. The existing XUL Limine adapter and image builder are still present until fBoot's UEFI executable can load the real XUL ELF, establish mappings, hand off the complete memory map, and pass QEMU firmware tests. fBoot must not be described as bootable or complete before those tests pass.

See [PROTOCOL.md](PROTOCOL.md) for the fBoot-to-XUL contract.
