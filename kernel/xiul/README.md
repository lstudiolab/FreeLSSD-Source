<!--
This file has been modified by L,Studio.
Copyright (c) 2026 L,Studio. All rights reserved.
-->

# XIUL Kernel

XIUL is the kernel of FreeLSSD. It uses a Mach microkernel foundation, BSD kernel functionality and Linux's driver ecosystem. FreeLSSD's GUI, user space, frameworks and API ecosystem are developed separately by L,Studio.

## Architecture

- **Mach:** microkernel foundation, tasks, threads, scheduling, IPC, VM primitives, I/O streams and Mach-native drivers.
- **Linux:** existing Linux driver source and the Linux kernel APIs required to execute those drivers. Linux is used for hardware compatibility, not as Stela's user-space environment or networking/filesystem stack.
- **BSD:** networking, sockets, routing, VFS, filesystems, POSIX syscall semantics and Unix kernel functionality.
- **XIUL:** boot hand-off validation, bootstrap physical-page allocation, device ownership, I/O coordination and integration contracts between the three.
- **Stela:** user space, GUI, native frameworks, BSD/Classic/QuickTime-compatible APIs, services and system utilities.

A device can use a Mach-native driver or a Linux driver. XIUL's device framework arbitrates ownership so that two drivers cannot concurrently control the same physical device. Linux network drivers hand packets to BSD networking through an explicit adapter.

## Source ownership

| XIUL area | Source | Purpose |
|---|---|---|
| `mach/` | Selected XNU `osfmk/` source from `lstudiolab/darwin-xnu` | Mach microkernel mechanisms |
| `linux/` | `lstudiolab/linux` | Linux driver source and required driver infrastructure |
| `freebsd/` | `lstudiolab/freebsd-src` | BSD networking and filesystem source |
| `core/` | XIUL-owned | Boot coordination, subsystem lifecycle and device ownership |
| `compat/linux/` | XIUL-owned | Linux driver API adapters |
| `bsd/` | XIUL-owned adapters plus selected FreeBSD code | Networking, VFS, POSIX and NKE |
| `io/` | XIUL-owned | I/O streams |
| `include/xiul/` | XIUL-owned | Stable internal kernel interfaces |

The complete XNU source tree is excluded. Only selected Mach `osfmk/` source belongs in `mach/`. Do not import XNU's BSD, I/O Kit, libkern, libsa, libsyscall or unrelated platform/build code.

## Build and test

```sh
make -C kernel/xiul source-audit
make -C kernel/xiul test
make -C kernel/xiul arch-build
```

These commands validate XIUL-owned host tests (including physical-page allocation) and link freestanding ELF images for x86_64 and AArch64. The version-2 boot ABI requires the loader to provide a mapped memory-map array; XIUL initializes its bootstrap physical-page allocator from conventional-memory entries. The ELF images require an external loader that constructs `xiul_boot_info_t`; they are not standalone bootable media. The commands do not compile upstream Mach, Linux drivers or FreeBSD networking/filesystems against XIUL. See `boot/README.md` for the exact loader hand-off contract.

See [ARCHITECTURE.md](ARCHITECTURE.md) and [INTEGRATION-STATUS.md](INTEGRATION-STATUS.md) for contracts, ownership rules and remaining work. Upstream source directories are not evidence of completed integration.
