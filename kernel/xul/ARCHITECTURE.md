<!--
This file has been modified by L,Studio.
Copyright (c) 2026 L,Studio. All rights reserved.
-->

# XUL Kernel Architecture

XUL is the kernel of FreeLSSD. Its design follows the layered organization of Darwin/XNU, but XUL is an independent integration of Mach mechanisms, BSD kernel functionality and Linux driver infrastructure.

## Kernel ownership model

```text
FreeLSSD
  +-- User space (developed by L,Studio)
  |     +-- FreeLSSD native APIs and frameworks
  |     +-- BSD / POSIX APIs
  |     +-- Classic compatibility APIs
  |     +-- QuickTime-compatible APIs
  |     +-- FreeLSSD GUI, services and utilities
  |
  +-- XUL kernel
        +-- Mach foundation
        |     +-- Tasks, threads and scheduling
        |     +-- IPC and synchronization primitives
        |     +-- Virtual-memory mechanisms
        |     +-- I/O streams and device coordination
        |     +-- Mach-native drivers
        |
        +-- Linux driver environment
        |     +-- Linux driver model and required kernel APIs
        |     +-- Linux PCI / USB / bus drivers
        |     +-- Linux storage, network, graphics and input drivers
        |     +-- XUL adapters for memory, DMA, IRQ, locks,
        |         timers, workqueues and device lifecycle
        |
        +-- BSD kernel functionality
              +-- Networking protocols and sockets
              +-- VFS and filesystem implementations
              +-- POSIX syscall and process-facing kernel support
              +-- NKE network extension hooks
  +-- Hardware
```

This is the logical ownership model. It does not claim that upstream code is already linked, bootable or fully adapted.

## Responsibilities

| Component | Responsibility | Resource owner |
|---|---|---|
| Mach | Microkernel mechanisms, IPC, scheduling, VM primitives, I/O streams | XUL core |
| Mach-native drivers | Drivers written specifically for XUL | XUL device framework |
| Linux driver environment | Preserve Linux driver source and the Linux APIs those drivers require | Linux compatibility environment, coordinated by XUL |
| BSD networking | Protocols, sockets, routing and network services | BSD networking subsystem |
| BSD filesystems | VFS, vnode and filesystem behavior | BSD VFS subsystem |
| POSIX | User/kernel ABI and process-facing syscall semantics | BSD-compatible syscall layer |
| NKE | Versioned network extension hooks | XUL/BSD network integration |
| FreeLSSD user space | GUI, frameworks, APIs, services and utilities | FreeLSSD |

## Critical integration rules

1. Mach is the microkernel foundation. XUL must not create competing global owners for CPU scheduling, address-space management, IPC or physical memory.
2. Linux driver source stays in the Linux source tree. Drivers are not copied into a new XUL driver implementation. The compatibility environment must supply the Linux kernel interfaces required by each selected driver.
3. Linux and Mach drivers may coexist. A physical device has exactly one active owner at a time; probe, bind, handoff and removal must be serialized by the XUL device framework.
4. Linux driver access to hardware is mediated by explicit XUL platform contracts for MMIO, port I/O, interrupts, DMA, PCI/USB resources, power and reset. No driver may bypass resource ownership.
5. BSD owns networking protocols and filesystems. A Linux network driver supplies the hardware-facing network device; packets cross a defined adapter into the BSD network stack. Linux networking is not substituted for BSD networking.
6. BSD POSIX APIs are exposed to FreeLSSD user space through XUL syscall and process services. BSD headers alone do not implement a syscall ABI.
7. Mach-native drivers remain supported and are not required to use Linux compatibility.
8. I/O streams are XUL-owned interfaces connecting device producers and kernel consumers. They do not imply that Apple's I/O Kit source has been imported.
9. Upstream source is source material, not proof of integration. Every port must have a dependency inventory, build result and runtime test.
10. Preserve upstream copyright and license notices. Review per-file licensing before linking or distributing Mach-, Linux- or FreeBSD-derived code.

## Initial Mach IPC transport\n\nXUL currently provides a bounded, nonblocking message-port transport in `core/ipc.c`. Ports have one receiving owner, fixed-capacity message queues and generation-tagged handles. Senders enqueue copied messages; only the owner can receive or destroy a port. The core boot lifecycle initializes and shuts down this service. This is an XUL-owned bootstrap primitive, not a claim that the imported Mach IPC subsystem has been compiled or that the full Mach port-rights, notification, memory-descriptor and blocking-wait semantics exist.\n\n## Required integration contracts

- Boot and platform: boot protocol, CPU mode, firmware tables, memory map, early console and CPU topology.
- Mach: machine-dependent startup, pmap/VM, scheduler, IPC, locks, atomics and timers.
- Device/I/O: device identity, ownership, probe/remove, resource allocation, streams and error propagation.
- Linux drivers: Linux device model, bus APIs, memory allocation, MMIO, DMA, IRQ, synchronization, timers, workqueues and power management.
- BSD networking: packet buffers, interfaces, protocol lifecycle, sockets, routing and synchronization.
- BSD VFS: vnode, mount, file handles, buffers, page-cache and block-device contracts.
- POSIX: process/thread model, syscall ABI, user-memory validation, file descriptors, signals and conformance.
- NKE: ABI versioning, registration, hook ordering, reference/lifetime tracking and safe unload.

## Build order

1. Package the Limine-compatible x86_64/AArch64 ELF entry with the matching Limine bootloader, then verify the hand-off under QEMU.
2. Bring up page tables, full exception/interrupt handling, APIC/GIC routing, architected timers and early console. The current x86_64 bootstrap has a temporary 8259/PIT IRQ0 path and interrupt-driven HLT idle loop.
3. Integrate the selected Mach source with XUL platform and VM contracts.
4. Start Mach task/thread, scheduler and IPC services from the XUL boot path.
5. Complete the XUL device ownership and I/O stream layer; the Linux netdev bridge now routes driver ingress/egress through BSD networking and NKE packet hooks.
6. Implement Linux driver compatibility primitives and port one real driver per bus class.
7. Connect Linux network drivers to BSD networking.
8. Adapt BSD networking and VFS/filesystem implementations.
9. Complete POSIX process/syscall behavior and NKE lifecycle.
10. Run QEMU boot, device, networking, filesystem and ABI tests on x86_64 and AArch64, then validate physical hardware.

## Current implementation status

See [INTEGRATION-STATUS.md](INTEGRATION-STATUS.md). A host test or relocatable object build does not establish that XUL is a bootable kernel. Do not call XUL complete until the architecture entry, Mach runtime, Linux driver environment, BSD subsystems and end-to-end boot tests work together.


## Naming

The kernel is named **XUL**. Existing `xul` directory and C identifier spellings are compatibility details retained until the source-tree and ABI migration can be performed atomically.
