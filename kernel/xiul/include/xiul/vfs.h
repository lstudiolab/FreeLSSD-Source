#ifndef XIUL_VFS_H
#define XIUL_VFS_H
#include <stddef.h>
#include <stdint.h>
#include "xiul/status.h"
#define XIUL_VFS_MAX_FILESYSTEMS 32u
#define XIUL_VFS_MAX_MOUNTS 32u

typedef struct xiul_mount xiul_mount_t;
typedef struct xiul_filesystem xiul_filesystem_t;
typedef xiul_status_t (*xiul_vfs_mount_fn)(xiul_mount_t *, const char *, void *);
typedef xiul_status_t (*xiul_vfs_unmount_fn)(xiul_mount_t *);

struct xiul_filesystem {
    const char *name;
    xiul_vfs_mount_fn mount;
    xiul_vfs_unmount_fn unmount;
};

struct xiul_mount {
    const xiul_filesystem_t *filesystem;
    const char *source;
    void *private_data;
    unsigned int active;
};

xiul_status_t xiul_vfs_register(const xiul_filesystem_t *filesystem);
xiul_status_t xiul_vfs_unregister(const xiul_filesystem_t *filesystem);
xiul_status_t xiul_vfs_mount(const char *filesystem_name, const char *source,
                             void *mount_options, xiul_mount_t **result);
xiul_status_t xiul_vfs_unmount(xiul_mount_t *mount);
size_t xiul_vfs_filesystem_count(void);
size_t xiul_vfs_mount_count(void);
#ifdef XIUL_TESTING
void xiul_vfs_test_reset(void);
#endif
#endif
