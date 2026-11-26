# Imported Mach source

This directory contains the imported `osfmk/` subtree from `lstudiolab/darwin-xnu`. The complete XNU repository is deliberately not imported. The imported subtree contains Mach task/thread, IPC, VM, kernel, and architecture-specific source, plus some XNU-specific services that must be reviewed before inclusion.

## Architecture source present

- x86_64: `x86_64/`, `i386/`, and shared `machine/` sources.
- ARM64: `arm64/`, `arm/`, and shared `machine/` sources.
- Shared core: `kern/`, `ipc/`, `vm/`, `mach/`, and related Mach directories.

## Why it is not yet buildable as a standalone XIUL kernel

The imported `osfmk/` tree is not self-contained. Its makefiles reference XNU build infrastructure such as `makedefs/MakeInc.*`. The C sources also include headers and services outside `osfmk/`, including platform expert, libkern, IOKit, BSD and generated configuration/assym headers. For example, the imported architecture and kernel sources include `pexpert/*`, `libkern/*`, `IOKit/*` and `sys/*` interfaces.

XIUL must not solve this by importing the complete XNU repository. Instead:
1. inventory the exact dependencies of the Mach components selected for XIUL;
2. replace Apple platform-expert and I/O Kit dependencies with XIUL platform/device contracts where feasible;
3. extract only required portable headers or source files with their original license notices;
4. implement missing Mach-to-XIUL adapters;
5. compile the selected Mach object set independently for x86_64 and ARM64;
6. link and boot-test the resulting images.

## Current integration status

The source import is complete for the `osfmk/` subtree. The XIUL platform C objects compile for both target architectures, but the imported Mach kernel itself is not yet compiled, linked or booted by the XIUL build. Do not treat source presence or the platform-object cross-build as a Mach kernel build.
