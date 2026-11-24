# FreeLSSD

FreeLSSD is the operating-system project. Its kernel project is **XIUL**.

## XIUL Kernel

XIUL is a hybrid-kernel integration project built around three upstream codebases:

- **Mach / XNU** — the initial low-level kernel foundation and Mach interfaces.
- **Linux** — selected Linux device drivers, imported through explicit compatibility adapters.
- **FreeBSD** — selected BSD kernel services, networking and filesystem components, integrated behind XIUL-owned interfaces.

These codebases are tracked as Git submodules. They are not copied into this repository, and their original histories and licenses remain intact.

See [the XIUL architecture](kernel/xiul/ARCHITECTURE.md) before changing subsystem boundaries.

## Checkout

Clone with all source dependencies:

```sh
git clone --recurse-submodules https://github.com/lstudiolab/FreeLSSD-Source.git
cd FreeLSSD-Source
```

If the repository was cloned without submodules:

```sh
git submodule update --init --recursive
```

## Current status

This commit establishes the source layout, pinned upstream dependencies, subsystem contracts and architecture rules. It is **not yet a bootable or merged kernel**. Porting, adapter implementations, a target platform, a boot path and hardware testing remain required.

## Licensing

FreeLSSD's own files are covered by the repository's top-level license unless a file says otherwise. Third-party source remains under its original license. Read each upstream license before copying, modifying, linking or redistributing its code.
