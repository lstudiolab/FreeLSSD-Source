# XIUL Kernel

XIUL is the kernel integration layer for FreeLSSD. It owns the stable interfaces between the Mach foundation, imported driver code, BSD services and the hardware platform.

## Source dependencies

| Component | Repository path | Intended responsibility |
|---|---|---|
| XNU / Mach | `vendor/xnu` | Mach-derived low-level primitives and reference implementation |
| Linux | `vendor/linux` | Selected device drivers through XIUL driver adapters |
| FreeBSD | `vendor/freebsd-src` | Selected networking and filesystem services through XIUL service adapters |

The source trees are upstream references and integration inputs. XIUL must not assume that a subsystem can be compiled independently merely because its source directory exists.

## Rules

1. XIUL owns its public kernel interfaces; imported code must adapt to them.
2. There is one owner for physical memory, virtual memory, scheduling, interrupts and device lifecycle on a given target. Do not run three competing implementations of the same global resource manager.
3. Linux drivers must use a Linux-compatibility layer. Do not expose Linux internal structures as XIUL's public ABI.
4. BSD networking and filesystem code must use explicit shims for allocation, locking, timers, mbufs/buffers, VFS objects and device access.
5. Mach code must be audited for XNU dependencies (pmap, IPC, task/thread, zone allocator, locks, machine layer and platform expert). Do not treat `osfmk` as a standalone drop-in kernel.
6. Preserve upstream copyright notices, license files and provenance for every imported file.
7. Never silently replace an upstream subsystem with a stub and call it implemented.

## Status

The initial integration contract is documented. The runtime adapters, boot sequence, target-specific machine layer and end-to-end kernel build are not implemented yet.
