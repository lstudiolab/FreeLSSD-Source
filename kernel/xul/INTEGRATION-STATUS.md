<!--
This file has been modified by L,Studio.
Copyright (c) 2026 L,Studio. All rights reserved.
-->

# XUL Integration Status

Last updated: 2026-11-26

This status distinguishes imported source, XUL-owned tested code and upstream integration. An imported source directory is not proof that the code has been adapted, linked or executed.

## Architecture baseline

- Mach is the microkernel foundation and supports Mach-native drivers.
- Linux driver source remains in the Linux tree. The XUL Linux driver environment must preserve the Linux driver model and implement the kernel APIs required by each selected driver.
- BSD provides networking, filesystems, POSIX syscall semantics and Unix kernel functionality.
- XUL owns device arbitration, I/O streams and integration contracts.
- FreeLSSD owns user space, GUI, frameworks, APIs, services and utilities.

The version-2 boot hand-off provides both physical and mapped virtual addresses for the XUL memory map. XUL initializes a tested, fixed-capacity bootstrap physical-page allocator from conventional-memory regions before PCI enumeration. It is not a Mach pmap or a general virtual-memory manager.

A physical device may be owned by one Mach-native or Linux driver at a time. Linux network devices connect to the BSD network stack through an explicit packet adapter.

## Verified implementation

| Area | XUL-owned implementation | Upstream integration / runtime status |
|---|---|---|
| Subsystem lifecycle | Dependency validation, cycle detection, phase-aware startup, reverse shutdown and failure rollback | Not connected to Mach bootstrap |
| I/O streams | Bounded single-producer/single-consumer byte stream with partial nonblocking I/O and close semantics | Not yet connected to Mach IPC or real device drivers |
| Device framework | Fixed-capacity device/driver registry, bus and ID matching, probe/bind and remove/unbind | No production hotplug serialization or full resource ownership |
| Physical memory | EFI conventional-memory map ingestion, page alignment, extent coalescing, contiguous page allocation/release and accounting | Bootstrap allocator only; no page tables, pmap, virtual allocator, DMA zones or SMP-safe locking |
| Freestanding runtime | XUL-owned `memset`, `memcpy`, `memmove` and `memcmp` primitives required by kernel objects | Minimal C runtime only; no full compiler runtime or libc |
| PCI | Generic enumeration, BAR sizing and x86_64 CF8/CFC / AArch64 ECAM configuration backends | Platform MMIO mapping, IRQ routing and DMA are incomplete |
| Linux driver bridge | Narrow Linux-style PCI registration adapter plus a Linux `net_device`-style bridge that forwards TX to `ndo_start_xmit` and RX into BSD networking | No general Linux kernel API environment; no upstream Linux driver has been built or run |
| BSD networking | Network-interface and protocol dispatch; Linux netdev ingress/egress bridge; NKE hooks run on both packet directions and can rewrite or drop packets | FreeBSD mbuf, sockets, routing and protocol stack are not adapted or linked |
| BSD VFS | Mount/open/read/write/close and vnode contracts | FreeBSD VFS/vnode/filesystem sources are not adapted or linked |
| POSIX | Syscall dispatch and per-process file-descriptor table connected to VFS | No complete process model, user-memory boundary, signal model or ABI |
| NKE | Priority-ordered packet hooks are now invoked on BSD network ingress and egress; accepted hook edits are propagated and drops return `XUL_E_DROPPED` | No loadable module ABI, hook lifetime protection, concurrent registration synchronization or safe unload |
| x86_64 / AArch64 | Limine protocol requests and entry adapter for HHDM, memory map, executable address and 64 KiB stack; Limine response conversion to XUL boot ABI; legacy hand-off stubs; x86_64 256-entry IDT and AArch64 VBAR vectors with diagnostics | Limine v12.9.6 ISO packaging and x86_64 QEMU smoke test are wired into CI; AArch64 runtime boot not yet verified; no full VM bootstrap, APIC/GIC routing, timer or SMP |

## CI coverage

The XUL GitHub Actions workflow runs source-layout audits and host-side tests, compiles and links XUL-owned freestanding ELF images for x86_64 and AArch64, packages Limine ISOs for both targets, and runs an x86_64 QEMU boot smoke test. These new steps have been added to CI but their first run has not yet been verified in this session. Host tests include the bootstrap physical-page allocator, Linux netdev-to-BSD networking and NKE packet filtering. This validates XUL-owned integration only; it does not validate imported upstream Mach, Linux driver or FreeBSD stack integration.

It does not:
- build a final bootable kernel image;
- compile the imported Mach `osfmk/` runtime;
- compile upstream Linux drivers against a complete XUL Linux compatibility environment;
- compile FreeBSD networking or filesystem code against XUL;
- execute QEMU boot tests or physical-hardware tests.

## Remaining kernel milestones

1. Verify the new x86_64 Limine ISO/QEMU smoke test in CI, then add AArch64 QEMU runtime verification; disk-image packaging remains open.
2. Establish page tables and a virtual-memory/pmap layer on top of the bootstrap physical allocator, then add interrupt controllers, timers, firmware discovery and SMP bring-up. Exception vectors are installed but currently halt on every exception.
3. Audit selected Mach source dependencies and connect Mach VM, scheduler, task/thread and IPC bootstrap to XUL.
4. Complete device resource ownership, MMIO mapping, IRQ routing, DMA and I/O stream integration.
5. Implement Linux driver compatibility APIs in dependency-driven layers and build/run an initial real PCI driver.
6. Port and run a real upstream Linux network driver through the new netdev bridge, then adapt FreeBSD mbuf, sockets, routing and protocol code.
7. Adapt FreeBSD VFS/vnode/buffer/page-cache contracts and integrate an initial filesystem.
8. Complete process creation, user-memory validation, POSIX syscall ABI, file descriptors and signals.
9. Implement NKE ABI versioning, lifetime tracking, synchronization and safe unload.
10. Add QEMU boot, device, networking, filesystem and ABI tests for x86_64 and AArch64, followed by physical hardware validation.

XUL remains integration-in-progress. It must not be described as complete or bootable until these milestones are implemented and verified.
