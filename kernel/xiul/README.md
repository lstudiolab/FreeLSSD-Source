# XIUL Kernel

The upstream source trees are located directly in this directory:

| Directory | Source | Intended role |
|---|---|---|
| `xnu/` | [lstudiolab/darwin-xnu](https://github.com/lstudiolab/darwin-xnu) | Mach foundation and XNU reference implementation |
| `linux/` | [lstudiolab/linux](https://github.com/lstudiolab/linux) | Selected Linux device-driver source |
| `freebsd/` | [lstudiolab/freebsd-src](https://github.com/lstudiolab/freebsd-src) | Selected BSD networking and filesystem source |

Initialize all source trees with:

```sh
git submodule update --init --recursive
```

## Integration policy

XIUL is not a concatenation of three complete kernels. It owns stable interfaces between the Mach-derived foundation, imported driver code, BSD services and the hardware platform.

- One owner per target for physical memory, virtual memory, scheduling, interrupts, clocks and device lifecycle.
- Linux drivers must be adapted through a Linux compatibility layer.
- FreeBSD networking and filesystem components must be adapted through explicit XIUL interfaces for memory, locks, timers, buffers, VFS and devices.
- XNU's `osfmk` is not standalone; Mach code depends on XNU platform and supporting subsystems.
- POSIX is an interface contract, not a source directory to merge.
- NKE is a kernel-extension interface, not a replacement network stack.
- Preserve upstream provenance, copyright notices and license files.

## Current status

The source trees are placed under this directory as submodules. The XIUL subsystem contract and architecture are documented, but the adapters, boot sequence, platform layer and end-to-end kernel build are not yet implemented.
