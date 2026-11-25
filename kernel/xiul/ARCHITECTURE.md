<!--
This file has been modified by L,Studio.
Copyright (c) 2026 L,Studio. All rights reserved.
-->

# XIUL Kernel Architecture

XIUL is the kernel of Stela. Its design follows the layered organization of Darwin/XNU, but XIUL is an independent integration of Mach mechanisms, BSD kernel functionality and Linux driver infrastructure.

## Kernel ownership model

```text
Stela
  +-- User space (developed by L,Studio)
  |     +-- Stela native APIs and frameworks
  |     +-- BSD / POSIX APIs
  |     +-- Classic compatibility APIs
  |     +-- QuickTime-compatible APIs
  |     +-- Stela GUI, services and utilities
  |
  +-- XIUL kernel
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
        |     +-- XIUL adapters for memory, DMA, IRQ, locks,
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
| Mach | Microkernel mechanisms, IPC, scheduling, VM primitives, I/O streams | XIUL core |
| Mach-native drivers | Drivers written specifically for XIUL | XIUL device framework |
| Linux driver environment | Preserve Linux driver source and the Linux APIs those drivers require | Linux compatibility environment, coordinated by XIUL |
| BSD networking | Protocols, sockets, routing and network services | BSD networking subsystem |
| BSD filesystems | VFS, vnode and filesystem behavior | BSD VFS subsystem |
| POSIX | User/kernel ABI and process-facing syscall semantics | BSD-compatible syscall layer |
| NKE | Versioned network extension hooks | XIUL/BSD network integration |
| Stela user space | GUI, frameworks, APIs, services and utilities | Stela |

## Critical integration rules

1. Mach is the microkernel foundation. XIUL must not create competing global owners for CPU scheduling, address-space management, IPC or physical memory.
2. Linux driver source stays in the Linux source tree. Drivers are not copied into a new XIUL driver implementation. The compatibility environment must supply the Linux kernel interfaces required by each selected driver.
3. Linux and Mach drivers may coexist. A physical device has exactly one active owner at a time; probe, bind, handoff and removal must be serialized by the XIUL device framework.
4. Linux driver access to hardware is mediated by explicit XIUL platform contracts for MMIO, port I/O, interrupts, DMA, PCI/USB resources, power and reset. No driver may bypass resource ownership.
5. BSD owns networking protocols and filesystems. A Linux network driver supplies the hardware-facing network device; packets cross a defined adapter into the BSD network stack. Linux networking is not substituted for BSD networking.
6. BSD POSIX APIs are exposed to Stela user space through XIUL syscall and process services. BSD headers alone do not implement a syscall ABI.
7. Mach-native drivers remain supported and are not required to use Linux compatibility.
8. I/O streams are XIUL-owned interfaces connecting device producers and kernel consumers. They do not imply that Apple's I/O Kit source has been imported.
9. Upstream source is source material, not proof of integration. Every port must have a dependency inventory, build result and runtime test.
10. Preserve upstream copyright and license notices. Review per-file licensing before linking or distributing Mach-, Linux- or FreeBSD-derived code.

## Required integration contracts

- Boot and platform: boot protocol, CPU mode, firmware tables, memory map, early console and CPU topology.
- Mach: machine-dependent startup, pmap/VM, scheduler, IPC, locks, atomics and timers.
- Device/I/O: device identity, ownership, probe/remove, resource allocation, streams and error propagation.
- Linux drivers: Linux device model, bus APIs, memory allocation, MMIO, DMA, IRQ, synchronization, timers, workqueues and power management.
- BSD networking: packet buffers, interfaces, protocol lifecycle, sockets, routing and synchronization.
- BSD VFS: vnode, mount, file handles, buffers, page-cache and block-device contracts.
- POSIX: process/thread model, syscall ABI, user-memory validation, file descriptors, signals and conformance.
- NKE: ABI versioning, registration, hook ordering, reference/lifetime tracking and safe unload.

## Build order

1. Establish a real boot protocol and architecture entry for each supported target.
2. Bring up page tables, exception/interrupt handling, timers and early console.
3. Integrate the selected Mach source with XIUL platform and VM contracts.
4. Start Mach task/thread, scheduler and IPC services from the XIUL boot path.
5. Complete the XIUL device ownership and I/O stream layer.
6. Implement Linux driver compatibility primitives and port one real driver per bus class.
7. Connect Linux network drivers to BSD networking.
8. Adapt BSD networking and VFS/filesystem implementations.
9. Complete POSIX process/syscall behavior and NKE lifecycle.
10. Run QEMU boot, device, networking, filesystem and ABI tests on x86_64 and AArch64, then validate physical hardware.

## Current implementation status

See [INTEGRATION-STATUS.md](INTEGRATION-STATUS.md). A host test or relocatable object build does not establish that XIUL is a bootable kernel. Do not call XIUL complete until the architecture entry, Mach runtime, Linux driver environment, BSD subsystems and end-to-end boot tests work together.
