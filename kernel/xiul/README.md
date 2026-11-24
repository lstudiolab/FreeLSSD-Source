# XIUL Kernel

XIUL uses Mach as its microkernel foundation, Linux-derived drivers for hardware compatibility, and selected FreeBSD components for networking and filesystems. It does not include or embed the complete XNU kernel.

## Source layout

| Directory | Upstream origin | Intended role |
|---|---|---|
| `mach/` | Selected Mach sources from [lstudiolab/darwin-xnu](https://github.com/lstudiolab/darwin-xnu), primarily `osfmk/` | Mach IPC, tasks, threads and microkernel mechanisms |
| `linux/` | [lstudiolab/linux](https://github.com/lstudiolab/linux) | Selected hardware drivers, adapted through XIUL's Linux compatibility layer |
| `freebsd/` | [lstudiolab/freebsd-src](https://github.com/lstudiolab/freebsd-src) | Selected networking and filesystem components |

Only the Mach source and its required supporting files are to be imported from the Darwin XNU repository. Do not copy the full XNU tree, including unrelated BSD, I/O Kit, userland or platform-specific components.

Mach is not a separately maintained upstream repository: its source lives inside XNU's `osfmk/` tree. The import must preserve the required Mach headers, machine-dependent code, build files, license notices and dependencies. Do not assume that copying `osfmk/` alone produces a standalone or bootable kernel.

## Integration policy

XIUL is not a concatenation of three complete kernels. It owns stable interfaces between the Mach foundation, imported driver code, BSD services and the hardware platform.

- One owner per target for physical memory, virtual memory, scheduling, interrupts, clocks and device lifecycle.
- Linux drivers must be adapted through a Linux compatibility layer.
- FreeBSD networking and filesystem components must be adapted through explicit XIUL interfaces for memory, locks, timers, buffers, VFS and devices.
- Mach code must be adapted to XIUL's platform and subsystem contracts; do not import the rest of XNU to satisfy dependencies without reviewing each dependency.
- POSIX is an interface contract, not a source directory to merge.
- NKE is a kernel-extension interface, not a replacement network stack.
- Preserve upstream provenance, copyright notices and license files.

## Current status

The repository currently contains the FreeBSD source tree under `freebsd/`. The Mach-only source import and Linux driver source import are not present in the current tracked tree yet. The adapters, boot sequence, platform layer and end-to-end kernel build are also not implemented.
