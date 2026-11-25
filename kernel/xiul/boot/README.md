<!--
This file has been modified by L,Studio.
Copyright (c) 2026 L,Studio. All rights reserved.
-->

# XIUL boot contract

XIUL's architecture entry points consume a pointer to the versioned
`xiul_boot_info_t` structure defined in `include/xiul/boot.h`. The current hand-off ABI version is 2.

This is a loader hand-off ABI, not a complete bootloader implementation.
The loader must establish the architecture's required execution mode,
provide a valid stack, map the kernel image and the memory-map array, and populate every physical
address and optional device mapping accurately. `memory_map_physical` records the physical address;
`memory_map_virtual` is the directly dereferenceable virtual mapping used during early bootstrap.

## x86_64 entry

The loader transfers control to `xiul_arch_entry` in 64-bit long mode
with the hand-off pointer in RDI (System V AMD64 ABI). Paging must be
enabled, the kernel image and stack mapped, and interrupts disabled.
The entry switches to an XIUL-owned bootstrap stack before calling C.

## AArch64 entry

The loader transfers control to `xiul_arch_entry` at EL1 or EL2 with
the hand-off pointer in X0. The kernel image and bootstrap stack must
be mapped, and asynchronous exceptions masked. The entry establishes
an aligned bootstrap stack before calling C.

## Current limitations

These entry stubs are not UEFI applications, BIOS boot sectors, or
Multiboot2 loaders. A loader is still required to load the ELF image,
establish page tables (including a mapping for `memory_map_virtual`) and construct the hand-off structure. The current
platform code also does not establish a complete virtual-memory manager,
exception vector table, interrupt controller, timer, or SMP environment.
Do not treat entry-stub compilation as proof of a bootable kernel.
