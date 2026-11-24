# Linux Driver Compatibility Layer

Linux source is used as a pool for selected hardware drivers. Linux is not a second kernel and its scheduler, VM, networking stack, VFS or userspace ABI are not XIUL subsystems.

The compatibility layer must translate the Linux APIs actually used by each selected driver into XIUL contracts, including as applicable:
- device and bus discovery;
- memory allocation and mapping;
- DMA mapping and synchronization;
- interrupt registration;
- locks, atomics and memory ordering;
- timers, workqueues and deferred execution;
- power-management and device lifecycle;
- logging and error reporting.

Drivers are not considered ported merely because their source files are present. Each driver requires an explicit dependency list, adapter coverage, build result and hardware or emulator test.
