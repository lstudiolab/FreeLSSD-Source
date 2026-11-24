# XIUL Integration Status

Last updated: 2026-11-24

This document separates imported source, XIUL-owned tested code and actual upstream integration. A directory containing upstream code does not mean that code has been ported or linked.

| Area | Imported source | XIUL-owned code | Upstream integration/build |
|---|---|---|---|
| Mach | XNU `osfmk/` subtree only | Boot contract and early platform primitives | Not yet built, linked or booted |
| Linux drivers | Full Linux source tree | Device registry, generic PCI enumeration, x86_64 CF8/CFC and ARM64 ECAM config backends, Linux PCI driver bridge | No upstream Linux driver compiled against XIUL yet |
| FreeBSD networking | Full FreeBSD source tree | Network-interface and packet dispatch boundary | FreeBSD protocol stack not yet adapted or linked |
| FreeBSD VFS/filesystems | Full FreeBSD source tree | Mount/open/read/write/close VFS and vnode contract | FreeBSD VFS/vnode/filesystem sources not yet adapted or linked |
| POSIX | Available in imported source pools | Syscall dispatch plus per-process FD table connected to VFS | No complete process model, syscall ABI or user-memory boundary |
| NKE | Not an upstream kernel dependency | Priority-ordered packet hooks | No loadable module ABI or safe unload |
| x86_64 | Mach and FreeBSD architecture source available | Early CPU/IRQ/COM1 platform objects | No complete boot image |
| ARM64 | Mach and FreeBSD architecture source available | Early CPU/IRQ/PL011 platform objects | No complete boot image |

## Verified CI coverage

The XIUL workflow currently runs:
- source-layout and upstream-manifest audit;
- host tests for kernel boot orchestration, subsystem lifecycle, I/O stream, device/driver registry, boot-info validation, synthetic PCI enumeration/BAR sizing/driver binding, Linux PCI bridge, network interfaces, VFS mount/file lifecycle, POSIX syscall dispatch and POSIX-FD-to-VFS integration, and NKE hooks;
- freestanding object compilation of XIUL-owned x86_64 and AArch64 early platform and PCI configuration-space backend code.

It does not run:
- a build of imported Mach `osfmk/`;
- a build of upstream Linux drivers against XIUL;
- a build of FreeBSD networking or VFS components against XIUL;
- a kernel linker stage, bootloader handoff, QEMU boot, or physical-hardware test.

## Required milestones

1. Mach dependency audit and standalone build integration.
2. x86_64 and AArch64 boot entry, linker scripts, page tables, exception/interrupt controllers, timers and SMP.
3. Mach task/thread, IPC, scheduler and VM bootstrap.
4. Linux PCI/USB, DMA, IRQ, memory, locking, timers and workqueue compatibility; port and test an initial real driver.
5. FreeBSD mbuf/network interface/protocol and socket adapters; port and test a real protocol path.
6. FreeBSD VFS/vnode/buffer/page-cache adapters; port and test an initial filesystem.
7. POSIX process, file descriptor, signal and syscall implementations.
8. NKE module ABI, lifetime tracking, synchronization and unload safety.
9. QEMU boot tests for both architectures, followed by hardware validation.
