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

## XIUL development checks

```sh
make -C kernel/xiul source-audit
make -C kernel/xiul test
```

The host tests cover the XIUL subsystem lifecycle manager, I/O stream primitive and device/driver registry. They do not build the imported upstream kernels or prove that XIUL boots. The Mach platform port, Linux driver compatibility APIs, BSD networking/filesystem adapters, POSIX/NKE implementations and end-to-end hardware tests remain integration work.

## Licensing

Third-party source retains its original license and notices. Review the license of every file and component before importing, linking, modifying or redistributing it.
