# XIUL Kernel Architecture

## Logical architecture

```text
Hardware
  |
XIUL machine/platform layer
  |
Mach microkernel foundation (selected sources: mach/)
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
| Mach (`osfmk` subset) | IPC, tasks, threads and microkernel mechanisms | XIUL machine, memory, scheduling and platform contracts |
| Linux | Selected hardware drivers | Linux compatibility layer; XIUL owns device lifecycle |
| FreeBSD | Networking and filesystem components | XIUL network-buffer, locking, timer, VFS and device interfaces |

The Mach code originates from the `osfmk/` directory in the Darwin XNU repository. XIUL must import only the reviewed Mach source and the supporting files it actually requires. The complete XNU source tree is not part of XIUL.

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

1. Select the Mach source files from upstream `osfmk/` and identify their required headers, machine-dependent code and supporting dependencies.
2. Import that reviewed Mach subset into `kernel/xiul/mach/`; do not vendor the complete XNU tree.
3. Define XIUL types and kernel contracts for memory, locking, devices, buffers, VFS and networking.
4. Establish a bootable Mach-derived base for one supported architecture.
5. Port a small Linux driver subset and test probe/remove, DMA, interrupts and power lifecycle.
6. Port BSD networking components while resolving their dependencies.
7. Port BSD filesystem components behind XIUL's VFS boundary.
8. Implement POSIX-facing interfaces and NKE.
9. Add automated boot, driver, network and filesystem tests.

## Important constraints

- Mach is part of XNU's `osfmk/` source tree, not a separately maintained upstream repository. Its source has dependencies on machine-dependent code, platform support, headers and other kernel facilities. A directory-only copy is not guaranteed to compile or boot.
- Do not import unrelated XNU components merely to make the source tree look complete. Review and include only dependencies required by the selected Mach implementation.
- Linux-derived code is generally GPL-2.0-only; preserve licensing and conduct a GPL compliance review.
- FreeBSD files have varying license terms; inspect file-level notices and contributed components.
- Preserve Apple Public Source License and other component-specific notices for every imported Mach file.
- Do not claim a subsystem is implemented merely because its upstream source exists.

## Current implementation boundary

The current tracked tree contains `kernel/xiul/freebsd/`. The Mach-only source subset and Linux driver source are not yet present. Their adapters, the boot sequence, platform layer and end-to-end kernel build remain unimplemented. XIUL is not yet claimed to build or boot.
