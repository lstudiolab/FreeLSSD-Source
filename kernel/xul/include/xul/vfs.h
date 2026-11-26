/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#ifndef XUL_VFS_H
#define XUL_VFS_H
#include <stddef.h>
#include <stdint.h>
#include "xul/status.h"
#define XUL_VFS_MAX_FILESYSTEMS 32u
#define XUL_VFS_MAX_MOUNTS 32u
#define XUL_VFS_MAX_FILES 64u
#define XUL_VFS_OPEN_READ (1u << 0)
#define XUL_VFS_OPEN_WRITE (1u << 1)

typedef struct xul_mount xul_mount_t;
typedef struct xul_filesystem xul_filesystem_t;
typedef struct xul_vnode xul_vnode_t;
typedef struct xul_file xul_file_t;

typedef struct {
    xul_status_t (*read)(xul_vnode_t *, uint64_t, void *, size_t, size_t *);
    xul_status_t (*write)(xul_vnode_t *, uint64_t, const void *, size_t, size_t *);
    xul_status_t (*close)(xul_vnode_t *);
} xul_vnode_ops_t;

struct xul_vnode {
    const xul_vnode_ops_t *operations;
    uint64_t size;
    uint32_t mode;
    void *private_data;
};

typedef xul_status_t (*xul_vfs_mount_fn)(xul_mount_t *, const char *, void *);
typedef xul_status_t (*xul_vfs_unmount_fn)(xul_mount_t *);
typedef xul_status_t (*xul_vfs_open_fn)(xul_mount_t *, const char *,
                                          uint32_t, xul_vnode_t *);

struct xul_filesystem {
    const char *name;
    xul_vfs_mount_fn mount;
    xul_vfs_unmount_fn unmount;
    xul_vfs_open_fn open;
};

struct xul_mount {
    const xul_filesystem_t *filesystem;
    const char *source;
    void *private_data;
    size_t open_files;
    unsigned int active;
};

struct xul_file {
    xul_mount_t *mount;
    xul_vnode_t vnode;
    uint64_t offset;
    uint32_t flags;
    unsigned int active;
};

xul_status_t xul_vfs_register(const xul_filesystem_t *filesystem);
xul_status_t xul_vfs_unregister(const xul_filesystem_t *filesystem);
xul_status_t xul_vfs_mount(const char *filesystem_name, const char *source,
                             void *mount_options, xul_mount_t **result);
xul_status_t xul_vfs_unmount(xul_mount_t *mount);
xul_status_t xul_vfs_open(xul_mount_t *mount, const char *path,
                             uint32_t flags, xul_file_t **result);
xul_status_t xul_vfs_read(xul_file_t *file, void *buffer, size_t length,
                             size_t *transferred);
xul_status_t xul_vfs_write(xul_file_t *file, const void *buffer, size_t length,
                              size_t *transferred);
xul_status_t xul_vfs_close(xul_file_t *file);
size_t xul_vfs_filesystem_count(void);
size_t xul_vfs_mount_count(void);
size_t xul_vfs_file_count(void);
#ifdef XUL_TESTING
void xul_vfs_test_reset(void);
#endif
#endif
