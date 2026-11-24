# BSD VFS Adapter

XIUL-owned VFS contracts now provide filesystem-type registration and mount/unmount lifecycle management with active-mount protection.

This is not yet a vnode/file-operation layer and does not make imported FreeBSD VFS sources linkable. FreeBSD `vfs_subr.c`, `vfs_init.c` and filesystem implementations still depend on FreeBSD vnode, buffer, namecache, VM, locking, UMA and module facilities. Those interfaces must be adapted before selecting filesystem sources can be built.

Run `make -C kernel/xiul test` for mount lifecycle tests.
