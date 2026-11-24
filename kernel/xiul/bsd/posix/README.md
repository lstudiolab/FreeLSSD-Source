# POSIX ABI

XIUL-owned code now provides a bounded syscall dispatch table with registration, duplicate detection, dispatch and unregister behavior.

This is only the syscall dispatch mechanism. It does not yet implement process creation, file descriptors, signals, credentials, user-pointer validation, syscall numbering compatibility or POSIX conformance. Those facilities require the Mach task/thread and VM integration plus concrete syscall implementations.

Run `make -C kernel/xiul test` for syscall dispatch tests.
