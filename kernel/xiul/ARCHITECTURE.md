# XIUL Kernel Architecture

## Layer model

XIUL adopts the broad Mach/BSD separation used by XNU, but it is an independently integrated kernel with a Linux driver compatibility environment.

```text
FreeLSSD Core OS (Engine)
  +-- System Utilities
  +-- XIUL Kernel
        +-- BSD service layer
        |     +-- Networking
        |     +-- Filesystem / VFS
        |     +-- POSIX-facing interfaces
        |     +-- NKE (network extension ABI)
        +-- Linux driver compatibility
        |     +-- Selected Linux drivers
        |     +-- Linux API-to-XIUL adapters
        +-- Mach microkernel foundation
              +-- Tasks, threads and scheduling
              +-- IPC
              +-- VM primitives
              +-- I/O streams
              +-- XIUL device/driver framework
  +-- Hardware
```

The diagram describes intended ownership and dependency direction, not completed functionality.

## Component map

| Component | Source | XIUL responsibility |
|---|---|---|
| Mach | Selected XNU `osfmk/` source in `mach/` | Microkernel mechanisms and primitives |
| Linux drivers | Selected files from `linux/` | Hardware support only, through `compat/linux/` |
| BSD networking | Selected files from `freebsd/` | Network protocols and services, through `bsd/net/` |
| BSD filesystems | Selected files from `freebsd/` | Filesystem implementations, through `bsd/vfs/` |
| POSIX | XIUL-owned implementation | Syscall ABI and user-space contract |
| NKE | XIUL-owned implementation | Versioned network extension registration and lifecycle |
| I/O streams | XIUL-owned implementation | Common data path between devices, drivers and consumers |
| Core OS | FreeLSSD user space | System utilities and user-space services |

## Ownership constraints

- Mach-derived code initially owns CPU scheduling, task/thread primitives, IPC and virtual-memory mechanisms.
- XIUL's platform layer owns boot, CPU bring-up, interrupt routing, clocks, physical device discovery and DMA policy.
- The XIUL device framework owns device lifecycle. Linux drivers must not introduce a competing device manager.
- BSD network code owns protocol behavior after it has been adapted to XIUL's packet/buffer and synchronization contracts.
- BSD filesystem code plugs into the XIUL VFS contract; it does not replace Mach VM or device ownership.
- POSIX compatibility is implemented at the syscall boundary.
- NKE extensions attach to the BSD networking path through a controlled, versioned interface.
- I/O streams are XIUL-owned abstractions. They are not a claim that XNU I/O Kit has been imported.

## Required integration contracts

The compatibility work must define and test these boundaries before upstream components are linked:

1. Machine/platform: boot, CPU topology, interrupts, timers, DMA and power events.
2. Mach adaptation: allocator/VM, locks, atomics, scheduler hooks, IPC and platform dependencies.
3. Device and I/O: device registration, driver probe/remove, resource ownership, I/O streams and error propagation.
4. Linux driver shim: Linux device model, bus, PCI/USB as applicable, DMA mapping, IRQ, timers, workqueues, memory allocation and power management.
5. BSD network: mbufs or an equivalent adapted packet-buffer contract, network interfaces, sockets, routing, protocol lifecycle and synchronization.
6. BSD VFS: vnode/inode mapping, mount lifecycle, file handles, buffers, page cache interaction and error semantics.
7. POSIX: syscall numbers, argument validation, process/thread semantics, file descriptors, signals and conformance tests.
8. NKE: ABI versioning, registration, ordering, unload safety, ownership and network-path hooks.

## Build order

1. Audit the selected Mach tree for dependencies outside `osfmk/`; adapt only required dependencies.
2. Select one initial architecture and implement XIUL platform startup.
3. Establish a minimal Mach-derived kernel build and boot test.
4. Implement XIUL device and I/O stream contracts.
5. Port a small, well-supported Linux driver subset through the compatibility layer.
6. Port BSD networking components behind the XIUL network contracts.
7. Port BSD filesystems behind the XIUL VFS contracts.
8. Implement POSIX syscall coverage and NKE lifecycle.
9. Add automated build, boot, device, networking, filesystem and ABI tests.

## Source and licensing policy

The complete XNU source tree is not part of XIUL. Only selected Mach `osfmk/` files are imported. Do not pull in XNU BSD, I/O Kit, libkern, libsa, libsyscall or unrelated platform/build code to resolve a dependency without first documenting and reviewing that dependency.

Linux-derived files generally carry GPL-2.0 terms; FreeBSD and Mach files have their own license and notice requirements. Preserve all notices and review file-level licenses before combining, linking or distributing components.

## Implementation status

Implemented in XIUL-owned code and covered by host-side CI tests:

- fixed-capacity subsystem registry with dependency validation, cycle detection, phase-aware startup, reverse shutdown and failure rollback;
- bounded single-producer/single-consumer I/O byte stream with nonblocking partial reads/writes and close semantics;
- fixed-capacity device/driver registry with platform, PCI, USB and VirtIO ID matching, probe/bind and remove/unbind lifecycle;
- shared status codes, public integration headers, source manifest, source-layout audit and CI test workflow.

Not yet integrated or verified:

- a bootable Mach-derived kernel image and platform entry path;
- complete Mach dependency adaptation and a real Mach build for a selected architecture;
- Linux kernel API shims for memory, DMA, IRQ, timers, workqueues, locking, PCI/USB and power management, plus actual Linux driver ports;
- FreeBSD networking and filesystem code adapted and linked against XIUL packet/VFS interfaces;
- implemented POSIX syscall/process ABI and NKE load/packet lifecycle;
- end-to-end boot, hardware, networking, filesystem and ABI tests.

The source trees are source material, not proof of integration. XIUL is still an integration-in-progress and must not be called a working or bootable kernel until these items are complete.
