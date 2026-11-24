#include <assert.h>
#include <stdio.h>
#include "xiul/vfs.h"
static int mounted, unmounted;
static xiul_status_t mount_fs(xiul_mount_t *m, const char *source, void *options)
{ (void)options; ++mounted; m->private_data = (void *)source; return XIUL_OK; }
static xiul_status_t unmount_fs(xiul_mount_t *m)
{ (void)m; ++unmounted; return XIUL_OK; }
int main(void)
{
    xiul_mount_t *mount = 0;
    const xiul_filesystem_t fs = {"memfs", mount_fs, unmount_fs};
    xiul_vfs_test_reset();
    assert(xiul_vfs_register(&fs) == XIUL_OK);
    assert(xiul_vfs_mount("memfs", "ram0", 0, &mount) == XIUL_OK);
    assert(mount != 0 && mount->active && mounted == 1);
    assert(xiul_vfs_unregister(&fs) == XIUL_E_BUSY);
    assert(xiul_vfs_unmount(mount) == XIUL_OK && unmounted == 1);
    assert(xiul_vfs_unregister(&fs) == XIUL_OK);
    assert(xiul_vfs_mount_count() == 0);
    xiul_vfs_test_reset();
    puts("XIUL VFS lifecycle tests: PASS");
    return 0;
}
