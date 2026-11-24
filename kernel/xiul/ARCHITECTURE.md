# XIUL Kernel Architecture

## Logical architecture

```text
Hardware
  |
XIUL machine/platform layer
  |
Mach microkernel foundation (selected osfmk sources: mach/)
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
| Mach (`osfmk` subset) | IPC, tasks, threads, scheduling and microkernel mechanisms | XIUL machine, memory, scheduling and platform contracts |
| Linux | Selected hardware drivers | Linux compatibility layer; XIUL owns device lifecycle |
| FreeBSD | Networking and filesystem components | XIUL network-buffer, locking, timer, VFS and device interfaces |

Mach originates from `osfmk/` in the Darwin XNU repository and is imported into `kernel/xiul/mach/`. The complete XNU source tree is not part of XIUL. XNU's BSD layer, I/O Kit, libkern, libsa, libsyscall and unrelated platform/build components are excluded.

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

1. Audit the imported `mach/` tree and enumerate dependencies on XNU headers or platform facilities outside `osfmk/`.
2. Implement or selectively adapt only the required dependencies inside XIUL; do not vendor the complete XNU tree.
3. Define XIUL types and kernel contracts for memory, locking, devices, buffers, VFS and networking.
4. Establish a bootable Mach-derived base for one supported architecture.
5. Port a small Linux driver subset and test probe/remove, DMA, interrupts and power lifecycle.
6. Port BSD networking components while resolving their dependencies.
7. Port BSD filesystem components behind XIUL's VFS boundary.
8. Implement POSIX-facing interfaces and NKE.
9. Add automated boot, driver, network and filesystem tests.

## Important constraints

- Mach is part of XNU's `osfmk/` source tree, not a separately maintained upstream repository. Some source files depend on headers and platform facilities outside `osfmk/`; review those dependencies individually.
- Do not import unrelated XNU components merely to make the source tree look complete. Include only dependencies required by the selected Mach implementation and adapt them to XIUL.
- Linux-derived code is generally GPL-2.0-only; preserve licensing and conduct a GPL compliance review.
- FreeBSD files have varying license terms; inspect file-level notices and contributed components.
- Preserve Apple Public Source License and other component-specific notices for every imported Mach file.
- Do not claim a subsystem is implemented merely because its upstream source exists.

## Implementation status

The repository tracks upstream source trees, but source presence does not establish integration. The XIUL platform layer, Mach dependency adapters, Linux driver compatibility, BSD networking/VFS adapters, boot sequence and end-to-end kernel build remain work items until implemented and tested.
