<!--
This file has been modified by L,Studio.
Copyright (c) 2026 L,Studio. All rights reserved.
-->

# XIUL Integration Status

Last updated: 2026-11-25

This status distinguishes imported source, XIUL-owned tested code and upstream integration. An imported source directory is not proof that the code has been adapted, linked or executed.

## Architecture baseline

- Mach is the microkernel foundation and supports Mach-native drivers.
- Linux driver source remains in the Linux tree. The XIUL Linux driver environment must preserve the Linux driver model and implement the kernel APIs required by each selected driver.
- BSD provides networking, filesystems, POSIX syscall semantics and Unix kernel functionality.
- XIUL owns device arbitration, I/O streams and integration contracts.
- FreeLSSD owns user space, GUI, frameworks, APIs, services and utilities.

A physical device may be owned by one Mach-native or Linux driver at a time. Linux network devices connect to the BSD network stack through an explicit packet adapter.

## Verified implementation

| Area | XIUL-owned implementation | Upstream integration / runtime status |
|---|---|---|
| Subsystem lifecycle | Dependency validation, cycle detection, phase-aware startup, reverse shutdown and failure rollback | Not connected to Mach bootstrap |
| I/O streams | Bounded single-producer/single-consumer byte stream with partial nonblocking I/O and close semantics | Not yet connected to Mach IPC or real device drivers |
| Device framework | Fixed-capacity device/driver registry, bus and ID matching, probe/bind and remove/unbind | No production hotplug serialization or full resource ownership |
| PCI | Generic enumeration, BAR sizing and x86_64 CF8/CFC / AArch64 ECAM configuration backends | Platform MMIO mapping, IRQ routing and DMA are incomplete |
| Linux driver bridge | Narrow Linux-style PCI registration adapter | No general Linux kernel API environment; no upstream Linux driver has been built or run |
| BSD networking | Network-interface and protocol-handler dispatch boundary | FreeBSD mbuf, sockets, routing and protocol stack are not adapted or linked |
| BSD VFS | Mount/open/read/write/close and vnode contracts | FreeBSD VFS/vnode/filesystem sources are not adapted or linked |
| POSIX | Syscall dispatch and per-process file-descriptor table connected to VFS | No complete process model, user-memory boundary, signal model or ABI |
| NKE | Priority-ordered packet hooks | No loadable module ABI or safe unload |
| x86_64 / AArch64 | Loader hand-off entry stubs, bootstrap stacks, linker scripts, early console, CPU/IRQ primitives and PCI config access; Makefile links XIUL-owned ELF images | Loader required; no page-table/VM bootstrap, exception vectors, interrupt-controller setup, timer, SMP or QEMU boot verification |

## CI coverage

The XIUL GitHub Actions workflow runs source-layout audits and host-side tests, then compiles and links XIUL-owned freestanding ELF images for x86_64 and AArch64 and publishes them as workflow artifacts. This validates the current XIUL-owned build only; it does not validate imported upstream kernel integration or bootability.

It does not:
- build a final bootable kernel image;
- compile the imported Mach `osfmk/` runtime;
- compile upstream Linux drivers against a complete XIUL Linux compatibility environment;
- compile FreeBSD networking or filesystem code against XIUL;
- execute QEMU boot tests or physical-hardware tests.

## Remaining kernel milestones

1. Implement a real UEFI/BIOS/Multiboot loader for each target; current entry stubs only consume the documented XIUL hand-off ABI and are not standalone bootloaders.
2. Establish page tables, exception vectors, interrupt controllers, timers, firmware discovery and SMP bring-up.
3. Audit selected Mach source dependencies and connect Mach VM, scheduler, task/thread and IPC bootstrap to XIUL.
4. Complete device resource ownership, MMIO mapping, IRQ routing, DMA and I/O stream integration.
5. Implement Linux driver compatibility APIs in dependency-driven layers and build/run an initial real PCI driver.
6. Connect Linux network drivers to BSD packet buffers and adapt a real BSD networking path.
7. Adapt FreeBSD VFS/vnode/buffer/page-cache contracts and integrate an initial filesystem.
8. Complete process creation, user-memory validation, POSIX syscall ABI, file descriptors and signals.
9. Implement NKE ABI versioning, lifetime tracking, synchronization and safe unload.
10. Add QEMU boot, device, networking, filesystem and ABI tests for x86_64 and AArch64, followed by physical hardware validation.

XIUL remains integration-in-progress. It must not be described as complete or bootable until these milestones are implemented and verified.
