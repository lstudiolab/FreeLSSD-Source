# XIUL Core

This directory contains XIUL-owned orchestration and shared kernel services. It must not become a second copy of Mach, Linux or FreeBSD.

Core responsibilities:
- establish platform services before starting dependent subsystems;
- coordinate subsystem initialization and shutdown;
- own stable internal contracts in `include/xiul/`;
- enforce one owner for physical memory, scheduling, interrupts and device lifecycle;
- expose diagnostics and explicit subsystem readiness.

The core must not report a subsystem as ready until its adapter has registered and passed its initialization checks.
