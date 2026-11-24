# XIUL Kernel

XIUL is FreeLSSD's hybrid kernel architecture. It follows the Mach/BSD structure of XNU while using selected Linux driver code for broader hardware support. XIUL is its own kernel integration project; it is not a renamed copy of XNU, Linux or FreeBSD.

## Architecture

```text
                         FreeLSSD Core OS (Engine)
                         +------------------------+
                         |    System Utilities    |
                         +------------------------+
                         |      XIUL Kernel       |
                         |  BSD-derived services  |
                         |  - Networking           |
                         |  - Filesystems / VFS    |
                         |  - POSIX interfaces     |
                         |  - NKE extension ABI    |
                         |------------------------|
                         |  Linux driver support  |
                         |  - Selected drivers    |
                         |  - Linux API shim      |
                         |------------------------|
                         |  Mach microkernel      |
                         |  - Tasks and threads   |
                         |  - IPC and scheduling  |
                         |  - VM primitives       |
                         |  - I/O stream core     |
                         |  - Driver framework    |
                         +------------------------+
                                  Hardware
```

This is the intended logical layering. The interfaces and source imports do not imply that all components are already ported, linked or bootable.

## Source ownership

| XIUL area | Source of implementation | Responsibility |
|---|---|---|
| `mach/` | Selected `osfmk/` sources from `lstudiolab/darwin-xnu` | Mach microkernel foundation |
| `linux/` | `lstudiolab/linux` | Source pool for selected hardware drivers only |
| `freebsd/` | `lstudiolab/freebsd-src` | Source pool for BSD networking and filesystem components |
| `core/` | XIUL-owned | Boot coordination, subsystem lifecycle and shared contracts |
| `compat/linux/` | XIUL-owned | Linux driver API compatibility |
| `bsd/` | XIUL-owned adapters plus selected FreeBSD code | Network, VFS/filesystem, POSIX and NKE integration |
| `io/` | XIUL-owned | I/O stream and device-facing contracts |
| `include/xiul/` | XIUL-owned | Stable internal kernel interfaces |

The complete XNU source tree is excluded. Only the selected Mach `osfmk/` source import belongs in `mach/`. Do not add XNU's BSD layer, I/O Kit, libkern, libsa, libsyscall, unrelated platform code or XNU build orchestration to XIUL.

## Integration rules

1. Mach is the kernel foundation and owns the initial task/thread, IPC, scheduling and VM implementation. There must be one owner for each core resource.
2. Linux is not a second kernel inside XIUL. Linux source is used selectively for drivers. A Linux compatibility layer must translate Linux driver APIs to XIUL's device, memory, locking, interrupt, DMA, timer and power interfaces.
3. BSD-derived code provides networking and filesystems through XIUL-owned network-buffer, socket, VFS, vnode, locking and device contracts.
4. POSIX is an ABI/API contract implemented at the syscall and user/kernel boundary. It is not another imported kernel.
5. NKE is a versioned network-kernel-extension interface. It is not a separate network stack.
6. Mach's historical I/O Kit dependencies must not cause the complete XNU tree to be imported. XIUL owns its I/O stream and driver interfaces; dependencies are adapted individually.
7. Upstream source remains attributed and licensed under its original terms. Preserve per-file notices and review licensing before linking or redistribution.

## Build, tests and status

Run:

```sh
make -C kernel/xiul source-audit
make -C kernel/xiul test
```

The host test suite validates kernel boot orchestration, subsystem lifecycle, I/O ring stream, device/driver registry, synthetic PCI enumeration and BAR sizing, Linux PCI adapter bridge, boot handoff validation, network-interface dispatch, VFS mount/file operations, POSIX syscall dispatch and POSIX file-descriptor integration with VFS, and NKE packet hooks. CI also cross-compiles the XIUL-owned x86_64 and AArch64 platform and PCI configuration-space objects. These checks do not compile the imported upstream Mach, Linux or FreeBSD kernel source trees and do not create a bootable XIUL image.

The imported upstream trees still require dependency audits and architecture-specific ports. The current platform code is early CPU/console/interrupt glue only. XIUL must not be described as bootable until the boot protocol, entry assembly, Mach dependency adapters, VM/interrupt-controller/SMP bring-up, Linux driver shims, BSD stack ports, linker/build rules and end-to-end boot tests work together on both targets.
