# XIUL Kernel

XIUL uses Mach as its microkernel foundation, Linux-derived drivers for hardware compatibility, and selected FreeBSD components for networking and filesystems. It does not include the complete XNU kernel.

## Source layout

| Directory | Upstream origin | Intended role |
|---|---|---|
| `mach/` | `osfmk/` from [lstudiolab/darwin-xnu](https://github.com/lstudiolab/darwin-xnu) | Mach IPC, tasks, threads, scheduling and microkernel mechanisms |
| `linux/` | [lstudiolab/linux](https://github.com/lstudiolab/linux) | Hardware drivers, adapted through XIUL's Linux compatibility layer |
| `freebsd/` | [lstudiolab/freebsd-src](https://github.com/lstudiolab/freebsd-src) | Networking and filesystem components |

## Mach import boundary

Mach is maintained upstream inside XNU's `osfmk/` directory; it is not a separate upstream repository. XIUL imports that directory as `kernel/xiul/mach/`. The rest of XNU is excluded: BSD, I/O Kit, libkern, libsa, libsyscall, platform expert code outside the selected Mach tree, XNU build orchestration and unrelated tools are not copied.

The Mach source retains its upstream file layout, headers, machine-dependent code, build files and license notices inside `osfmk/`. Some Mach code may still depend on headers or platform facilities outside `osfmk/`; those dependencies must be identified and implemented or selectively adapted in XIUL rather than importing the full XNU tree. An `osfmk/` source import alone is not a standalone or bootable kernel.

## Integration policy

XIUL is not a concatenation of three complete kernels. It owns stable interfaces between the Mach foundation, imported driver code, BSD services and the hardware platform.

- One owner per target for physical memory, virtual memory, scheduling, interrupts, clocks and device lifecycle.
- Linux drivers must be adapted through a Linux compatibility layer.
- FreeBSD networking and filesystem components must be adapted through explicit XIUL interfaces for memory, locks, timers, buffers, VFS and devices.
- Mach code must be adapted to XIUL's platform and subsystem contracts; do not import unrelated XNU components to satisfy dependencies without reviewing each dependency.
- POSIX is an interface contract, not a source directory to merge.
- NKE is a kernel-extension interface, not a replacement network stack.
- Preserve upstream provenance, copyright notices and license files.

## Build status

Imported upstream source is not equivalent to an integrated implementation. XIUL still requires its platform layer, Mach dependency adapters, Linux driver compatibility layer, BSD networking/VFS adapters, boot path, and end-to-end build and boot tests. Do not claim XIUL builds or boots until those tests pass.
