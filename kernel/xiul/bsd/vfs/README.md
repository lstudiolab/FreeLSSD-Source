# BSD VFS Adapter

XIUL's VFS boundary now provides:
- filesystem-type registration and unregister protection while mounted;
- mount/unmount lifecycle management;
- a bounded open-file table and vnode operation contract;
- filesystem-provided path open;
- offset-aware read/write dispatch with transfer-length and offset-overflow validation;
- close callbacks and protection against unmounting a filesystem with open files.

This is an XIUL-owned VFS contract, not yet a port of FreeBSD's VFS/vnode implementation. FreeBSD `vfs_subr.c`, `vfs_init.c` and filesystem implementations still depend on FreeBSD vnode, buffer, namecache, VM, locking, UMA and module facilities. Those interfaces must be adapted before selected FreeBSD filesystem sources can be linked.

The current API is single-threaded/externally serialized. A kernel VFS lock, vnode reference counting, pathname component lookup, permissions, page cache, buffer cache, mount namespace and file-descriptor integration are still required.

Run `make -C kernel/xiul test` for mount, open, read, write, close and unmount lifecycle tests.
