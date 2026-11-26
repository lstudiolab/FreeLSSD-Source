<!--
This file has been modified by L,Studio.
Copyright (c) 2026 L,Studio. All rights reserved.
-->

# XUL BSD Service Adapters

Selected FreeBSD components provide BSD-derived networking and filesystem functionality. They must be adapted to XUL-owned interfaces rather than linked as a second complete kernel.

- `net/`: networking protocols and services adapted to XUL packet buffers, interfaces, timers and synchronization.
- `vfs/`: filesystem implementations adapted to the XUL VFS, vnode, buffer and page-cache contracts.
- `posix/`: XUL's POSIX-facing syscall and process ABI implementation. POSIX is an interface contract, not an imported FreeBSD source subtree.
- `nke/`: versioned network-kernel-extension registration and packet-path hooks.

Preserve file-level licensing. Port components individually and record their upstream paths and dependency changes.
