# POSIX ABI

XIUL-owned code currently provides:
- a bounded syscall dispatch table with registration, duplicate detection, dispatch and unregister;
- a per-process file-descriptor table;
- descriptor install/close/read/write operations backed by the XIUL VFS file API;
- process teardown that closes open descriptors.

This is not yet a complete POSIX process or syscall ABI. It does not implement process creation, file descriptor inheritance, user-pointer copyin/copyout, signals, credentials, permissions, standard streams, poll/select, pipes, sockets or syscall numbering compatibility. Those facilities require the Mach task/thread and VM integration plus concrete syscall implementations.

The descriptor read/write helpers currently operate on kernel buffers. A syscall boundary must validate and copy user buffers before calling them.

Run `make -C kernel/xiul test` for syscall dispatch and POSIX-descriptor/VFS integration tests.
