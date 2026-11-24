# XIUL I/O Streams

The I/O stream layer is XIUL-owned. It defines a consistent path for data moving between device drivers and kernel consumers.

Mach supplies the microkernel foundation and device-facing primitives. Selected Linux drivers are adapted to XIUL's driver contract. I/O streams provide the common data interface; they are not a copy of XNU I/O Kit.

The implementation must define buffering, ownership, back-pressure, cancellation, error propagation, synchronization and teardown semantics before drivers depend on it.
