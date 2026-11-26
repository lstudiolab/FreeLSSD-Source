<!--
This file has been modified by L,Studio.
Copyright (c) 2026 L,Studio. All rights reserved.
-->

# fBoot / XUL hand-off protocol

## Ownership

fBoot owns the machine until it has successfully exited UEFI Boot Services. XUL owns the machine after the architecture entry point is called. No UEFI Boot Services or Runtime Services calls are permitted from XUL's early boot path unless a future explicit runtime-services contract is added.

## Required hand-off state

The hand-off structure is the versioned `xul_boot_info_t` in `kernel/xul/include/xul/boot.h`. fBoot must populate:

- magic, version and exact structure size;
- physical and directly accessible virtual address of the normalized memory map;
- count of normalized memory-map entries;
- firmware system-table/configuration-table location where supported;
- framebuffer address, dimensions, pitch and pixel format when available;
- serial console parameters when discovered;
- PCI ECAM information only when validated from ACPI or device tree;
- boot image physical/virtual bounds and firmware boot mode in a future ABI revision.

Memory-map entries must distinguish usable conventional RAM from loader allocations, kernel image pages, firmware/runtime regions, MMIO, ACPI reclaim/NVS and reserved ranges. Only conventional RAM may be returned to the XUL bootstrap allocator.

## x86_64 entry contract

- UEFI x86_64 long mode is active.
- Interrupts are disabled.
- The stack is 16-byte aligned according to the x86_64 ABI before the call.
- The first argument (RDI) points to the hand-off structure.
- The kernel image's required higher-half mappings are active.
- The hand-off structure and normalized memory map remain mapped.
- The kernel entry address is taken from the validated ELF entry point.

## AArch64 entry contract

- Execution is transferred at the documented EL and with a documented MMU/cache state.
- Interrupts are masked.
- SP is 16-byte aligned.
- X0 points to the hand-off structure.
- Kernel image, hand-off structure and memory map are mapped with the required attributes.
- Cache maintenance and translation-table setup are completed before branch.

## ExitBootServices algorithm

1. Allocate all persistent boot data, page tables, kernel segments, stack and normalized-map capacity.
2. Obtain the current UEFI memory map and its map key.
3. Normalize it into the fBoot/XUL ABI without making further allocations.
4. Call ExitBootServices with that key.
5. If the firmware returns EFI_INVALID_PARAMETER, reacquire the map and retry without allocation; bound retries and fail safely.
6. After success, make no firmware calls and transfer to the architecture entry.

## Compatibility transition

The existing Limine boot path remains available only while fBoot is being implemented and tested. Once fBoot passes x86_64 OVMF boot tests, make fBoot the default image builder. Remove Limine request sections and the Limine adapter only after equivalent fBoot hand-off coverage exists on both x86_64 and AArch64.
