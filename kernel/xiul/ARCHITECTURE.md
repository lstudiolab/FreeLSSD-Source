# XIUL Kernel Architecture

## Logical architecture

```text
Hardware
  |
XIUL machine/platform layer
  |
Mach-derived kernel foundation (source: xnu/)
  |
XIUL subsystem and compatibility interfaces
  |-- Linux driver compatibility (source: linux/)
  |-- BSD networking services (source: freebsd/)
  |-- BSD filesystem / VFS services (source: freebsd/)
  |-- POSIX-facing syscall compatibility
  |-- NKE (Network Kernel Extension) interface
  |
FreeLSSD Core OS (Engine)
  |-- System Utilities
  |-- User-space services
```

This is a logical ownership diagram, not a claim that upstream source trees already interoperate.

## Source responsibilities

| Source | Intended contribution | Integration boundary |
|---|---|---|
| XNU / Mach | Low-level kernel foundation and Mach mechanisms | XIUL machine, memory, IPC, scheduling and platform contracts |
| Linux | Selected hardware drivers | Linux compatibility layer; XIUL owns device lifecycle |
| FreeBSD | Networking and filesystem components | XIUL network-buffer, locking, timer, VFS and device interfaces |

## Ownership

| Concern | Initial XIUL owner | Rule |
|---|---|---|
| Boot and machine setup | XIUL platform layer | One implementation per supported target |
| CPU scheduling | Mach-derived core initially | Do not activate competing Linux/BSD schedulers |
| Virtual memory | Mach-derived core initially | Imported code uses XIUL VM adapters |
| IPC | Mach-derived core initially | XIUL defines the kernel/user boundary |
| Interrupts and DMA | XIUL platform layer | Driver shims translate upstream expectations |
| Device drivers | XIUL driver framework | Adapt selected Linux drivers |
| Networking | BSD-derived service layer | Port components behind XIUL interfaces |
| Filesystems | XIUL VFS boundary | Port filesystems individually |
| POSIX | XIUL syscall boundary | Define stable semantics and conformance tests |
| NKE | XIUL extension ABI | Versioned and validated extension contract |

## Integration stages

1. Pin upstream source revisions as submodules.
2. Define XIUL types and kernel contracts for memory, locking, devices, buffers, VFS and networking.
3. Establish a bootable Mach-derived base for one supported architecture.
4. Port a small Linux driver subset and test probe/remove, DMA, interrupts and power lifecycle.
5. Port BSD networking components while resolving their dependencies.
6. Port BSD filesystem components behind XIUL's VFS boundary.
7. Implement POSIX-facing interfaces and NKE.
8. Add automated boot, driver, network and filesystem tests.

## Important constraints

- XNU is an integrated kernel source tree, not merely Mach. `xnu/osfmk` depends on XNU facilities including pmap, IPC, task/thread, allocators, locks, machine code and platform expert components.
- Linux-derived code is generally GPL-2.0-only; preserve licensing and conduct a GPL compliance review.
- FreeBSD files have varying license terms; inspect file-level notices and contributed components.
- XNU includes Apple Public Source License and other component-specific licensing. Inspect the exact files selected for integration.
- Do not claim a subsystem is implemented merely because its upstream source exists.

## Current implementation boundary

The source repositories are now located under `kernel/xiul/`. Their code has not yet been merged into a unified kernel. XIUL is not yet claimed to build or boot.
