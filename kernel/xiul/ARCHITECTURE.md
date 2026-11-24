# XIUL Kernel Architecture

## System placement

```text
Hardware
  |
XIUL machine/platform layer
  |
Mach-derived kernel foundation (XNU / osfmk)
  |
XIUL subsystem and compatibility interfaces
  |-- Linux driver compatibility
  |-- BSD networking services
  |-- BSD filesystem / VFS services
  |-- POSIX-facing syscall compatibility
  |-- NKE (Network Kernel Extension) interface
  |
Core OS (Engine)
  |-- System Utilities
  |-- User-space services
```

This is a logical ownership diagram, not a claim that the upstream kernels already interoperate. The Mach foundation is the initial design anchor. The other source trees contribute selected components only after their dependencies have been isolated.

## Ownership model

| Concern | XIUL owner | Integration policy |
|---|---|---|
| Boot and machine setup | XIUL platform layer | One implementation per supported architecture/platform |
| CPU scheduling | Mach-derived core initially | Linux and FreeBSD schedulers are not loaded alongside it |
| Virtual memory | Mach-derived core initially | Imported code must use XIUL VM adapters |
| IPC | Mach-derived core initially | XIUL defines the kernel/user boundary |
| Interrupts and DMA | XIUL platform layer | Driver shims translate upstream expectations |
| Device drivers | XIUL driver framework | Selected Linux drivers are adapted; no wholesale Linux kernel |
| Networking | BSD-derived service layer | Port behind XIUL buffer, lock, timer and device interfaces |
| Filesystems | XIUL VFS boundary | Select and adapt filesystems; do not combine VFS internals directly |
| POSIX | XIUL syscall boundary | Define stable syscall semantics and compatibility tests |
| NKE | XIUL extension ABI | Versioned, validated kernel extension contract |

## Integration stages

1. **Source pinning** — track XNU, Linux and FreeBSD as independent submodules at known commits.
2. **Kernel contract** — define XIUL types, lifecycle, locking, memory, device, buffer, VFS and network interfaces.
3. **Bootable Mach-derived base** — select one supported architecture and establish a repeatable boot path.
4. **Driver compatibility** — port a small, well-supported Linux driver subset and test probe/remove, DMA, interrupts and power lifecycle.
5. **BSD service integration** — port networking and filesystem components individually, resolving dependencies rather than importing whole kernel subsystems.
6. **POSIX and NKE** — expose user-space interfaces and a versioned extension model.
7. **Validation** — automated unit tests, kernel tests, boot tests, driver tests and network/filesystem conformance.

## Critical constraints

- **XNU is not just Mach.** Its Mach layer is integrated with BSD, IOKit, libkern, platform expert code and XNU-specific build tooling. The `osfmk` directory is a source reference, not a standalone build target.
- **The Linux kernel is GPL-2.0-only.** Linux-derived code must retain its license and notices; distribution and linking choices require a GPL compliance review.
- **FreeBSD source is BSD-licensed in many parts, but individual files and contributed components can carry other licenses.** Preserve file-level notices.
- **XNU uses Apple Public Source License and other component-specific licensing.** Preserve its license files and review the exact files selected for import.
- **POSIX is an interface standard, not a kernel subsystem that can be merged as a directory.**
- **NKE is an extension mechanism, not a replacement for a network stack.**

## Current implementation boundary

The repository currently pins the three upstream trees and defines the intended ownership model. No claim is made that their code has been merged, that a driver can load, or that XIUL can boot. Each integration stage must land with its own build and tests.
