#ifndef XIUL_VFS_H
#define XIUL_VFS_H
#include <stddef.h>
#include <stdint.h>
#include "xiul/status.h"
#define XIUL_VFS_MAX_FILESYSTEMS 32u
#define XIUL_VFS_MAX_MOUNTS 32u
#define XIUL_VFS_MAX_FILES 64u
#define XIUL_VFS_OPEN_READ (1u << 0)
#define XIUL_VFS_OPEN_WRITE (1u << 1)

typedef struct xiul_mount xiul_mount_t;
typedef struct xiul_filesystem xiul_filesystem_t;
typedef struct xiul_vnode xiul_vnode_t;
typedef struct xiul_file xiul_file_t;

typedef struct {
    xiul_status_t (*read)(xiul_vnode_t *, uint64_t, void *, size_t, size_t *);
    xiul_status_t (*write)(xiul_vnode_t *, uint64_t, const void *, size_t, size_t *);
    xiul_status_t (*close)(xiul_vnode_t *);
} xiul_vnode_ops_t;

struct xiul_vnode {
    const xiul_vnode_ops_t *operations;
    uint64_t size;
    uint32_t mode;
    void *private_data;
};

typedef xiul_status_t (*xiul_vfs_mount_fn)(xiul_mount_t *, const char *, void *);
typedef xiul_status_t (*xiul_vfs_unmount_fn)(xiul_mount_t *);
typedef xiul_status_t (*xiul_vfs_open_fn)(xiul_mount_t *, const char *,
                                          uint32_t, xiul_vnode_t *);

struct xiul_filesystem {
    const char *name;
    xiul_vfs_mount_fn mount;
    xiul_vfs_unmount_fn unmount;
    xiul_vfs_open_fn open;
};

struct xiul_mount {
    const xiul_filesystem_t *filesystem;
    const char *source;
    void *private_data;
    size_t open_files;
    unsigned int active;
};

struct xiul_file {
    xiul_mount_t *mount;
    xiul_vnode_t vnode;
    uint64_t offset;
    uint32_t flags;
    unsigned int active;
};

xiul_status_t xiul_vfs_register(const xiul_filesystem_t *filesystem);
xiul_status_t xiul_vfs_unregister(const xiul_filesystem_t *filesystem);
xiul_status_t xiul_vfs_mount(const char *filesystem_name, const char *source,
                             void *mount_options, xiul_mount_t **result);
xiul_status_t xiul_vfs_unmount(xiul_mount_t *mount);
xiul_status_t xiul_vfs_open(xiul_mount_t *mount, const char *path,
                             uint32_t flags, xiul_file_t **result);
xiul_status_t xiul_vfs_read(xiul_file_t *file, void *buffer, size_t length,
                             size_t *transferred);
xiul_status_t xiul_vfs_write(xiul_file_t *file, const void *buffer, size_t length,
                              size_t *transferred);
xiul_status_t xiul_vfs_close(xiul_file_t *file);
size_t xiul_vfs_filesystem_count(void);
size_t xiul_vfs_mount_count(void);
size_t xiul_vfs_file_count(void);
#ifdef XIUL_TESTING
void xiul_vfs_test_reset(void);
#endif
#endif
