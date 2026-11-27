<!--
This file has been modified by L,Studio.
Copyright (c) 2026 L,Studio. All rights reserved.
-->

# Imported Mach source

This directory contains the imported `osfmk/` subtree from `lstudiolab/darwin-xnu`. The complete XNU repository is deliberately not imported. The imported subtree contains Mach task/thread, IPC, VM, kernel, and architecture-specific source, plus XNU-specific support components that must be reviewed before inclusion.

## Architecture source present

- x86_64: `x86_64/`, `i386/`, and shared `machine/` sources.
- AArch64: `arm64/`, `arm/`, and shared `machine/` sources.
- Shared core: `kern/`, `ipc/`, `vm/`, `mach/`, and related Mach directories.

## Why it is not yet buildable as a standalone XUL kernel

The imported `osfmk/` tree is not self-contained. Its makefiles reference XNU build infrastructure such as `makedefs/MakeInc.*`. The C sources also include headers and services outside `osfmk/`, including platform expert, libkern, IOKit, BSD and generated configuration/assym headers. Some directories inside this import (for example `libsa/`, `kextd/`, `gssd/` and diagnostic services) are support components, not part of the minimum Mach kernel bootstrap.

XUL must not solve this by importing the complete XNU repository. Instead:
1. select the Mach kernel components required for the first bootstrap (kernel, IPC, VM and architecture-dependent machine code);
2. inventory the exact transitive dependencies of that selected source set;
3. replace Apple platform-expert and I/O Kit dependencies with XUL platform/device contracts where feasible;
4. extract only required portable headers or source files with their original license notices;
5. implement the Mach-to-XUL adapters for memory, scheduling, IPC, locks, timers and CPU startup;
6. compile the selected Mach object set independently for x86_64 and AArch64;
7. link and boot-test the resulting images.

Do not add all of `mach/` to the XUL link merely because it is present. The imported source must be selected by an explicit dependency manifest and built with XUL's own toolchain and configuration.

## Current integration status

The source import is present, but the imported Mach kernel itself is not yet compiled, linked or booted by the XUL build. The current XUL-owned IPC transport is a bootstrap placeholder; it does not replace Mach IPC, task/thread management, VM or scheduling. Do not treat source presence or the platform-object cross-build as a Mach kernel build.
