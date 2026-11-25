<!--
This file has been modified by L,Studio.
Copyright (c) 2026 L,Studio. All rights reserved.
-->

# Network Kernel Extensions

XIUL-owned code now provides a bounded NKE hook registry, stable priority ordering, packet callback dispatch, drop short-circuiting and unregister behavior.

This is a kernel-internal hook API, not yet a loadable module ABI. Safe unload requires module reference tracking, quiescence/RCU or equivalent callback synchronization, version negotiation, ownership and signature policy. Those facilities must be completed before third-party extensions are accepted.

Run `make -C kernel/xiul test` for hook ordering and packet action tests.
