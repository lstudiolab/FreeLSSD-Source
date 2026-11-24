# FreeLSSD

FreeLSSD is the operating-system project. Its kernel is **XIUL**.

## Kernel architecture

XIUL is a hybrid Mach/BSD kernel architecture with a Linux driver compatibility environment:

- **Mach** is the microkernel foundation: tasks, threads, IPC, scheduling and core machine mechanisms.
- **BSD-derived services** provide networking, filesystems/VFS, POSIX-facing interfaces and the NKE extension ABI.
- **Linux source** is used selectively for hardware drivers, adapted through XIUL's Linux compatibility layer.
- **FreeLSSD Core OS (Engine)** sits above XIUL and provides system utilities and user-space services.

XIUL is not three complete kernels concatenated together. It defines explicit interfaces between these subsystems and assigns one owner to core resources.

## Source layout

The XIUL source and integration plan lives under `kernel/xiul/`:

- `mach/` — selected Mach `osfmk/` source; the complete XNU tree is excluded.
- `linux/` — Linux kernel source used as a source pool for selected drivers.
- `freebsd/` — FreeBSD source used as a source pool for selected networking and filesystem components.
- `core/`, `include/xiul/`, `io/` — XIUL-owned kernel contracts and integration code.
- `compat/linux/` — Linux driver compatibility layer.
- `bsd/net/`, `bsd/vfs/`, `bsd/posix/`, `bsd/nke/` — BSD service adapters and XIUL-owned ABI surfaces.

Read [the XIUL architecture](kernel/xiul/ARCHITECTURE.md) and [the XIUL integration guide](kernel/xiul/README.md) before changing kernel source.

## Source layout audit

```sh
make -C kernel/xiul source-audit
```

This checks the expected directory structure only. It is not a kernel build or boot test. Do not claim XIUL is bootable until the platform layer, adapters, build system and end-to-end tests pass.

## Licensing

Third-party source retains its original license and notices. Review the license of every file and component before importing, linking, modifying or redistributing it.
