#include "xiul/vfs.h"

static const xiul_filesystem_t *filesystems[XIUL_VFS_MAX_FILESYSTEMS];
static xiul_mount_t mounts[XIUL_VFS_MAX_MOUNTS];
static xiul_file_t files[XIUL_VFS_MAX_FILES];
static size_t filesystem_count;
static size_t mount_count;
static size_t file_count;

static int name_equal(const char *a, const char *b)
{
    size_t i = 0;
    if (a == 0 || b == 0) return 0;
    while (a[i] && b[i]) { if (a[i] != b[i]) return 0; ++i; }
    return a[i] == b[i];
}

xiul_status_t xiul_vfs_register(const xiul_filesystem_t *filesystem)
{
    size_t i;
    if (filesystem == 0 || filesystem->name == 0 ||
        filesystem->name[0] == '\0' || filesystem->mount == 0 ||
        filesystem->unmount == 0 || filesystem->open == 0) return XIUL_E_INVALID;
    for (i = 0; i < filesystem_count; ++i) {
        if (filesystems[i] == filesystem ||
            name_equal(filesystems[i]->name, filesystem->name)) return XIUL_E_DUPLICATE;
    }
    if (filesystem_count == XIUL_VFS_MAX_FILESYSTEMS) return XIUL_E_CAPACITY;
    filesystems[filesystem_count++] = filesystem;
    return XIUL_OK;
}

xiul_status_t xiul_vfs_unregister(const xiul_filesystem_t *filesystem)
{
    size_t i, j;
    if (filesystem == 0) return XIUL_E_INVALID;
    for (i = 0; i < filesystem_count; ++i) {
        if (filesystems[i] != filesystem) continue;
        for (j = 0; j < XIUL_VFS_MAX_MOUNTS; ++j)
            if (mounts[j].active && mounts[j].filesystem == filesystem) return XIUL_E_BUSY;
        for (; i + 1 < filesystem_count; ++i) filesystems[i] = filesystems[i + 1];
        filesystems[--filesystem_count] = 0;
        return XIUL_OK;
    }
    return XIUL_E_NOT_FOUND;
}

xiul_status_t xiul_vfs_mount(const char *filesystem_name, const char *source,
                             void *mount_options, xiul_mount_t **result)
{
    const xiul_filesystem_t *filesystem = 0;
    xiul_mount_t *mount = 0;
    size_t i;
    xiul_status_t status;
    if (filesystem_name == 0 || filesystem_name[0] == '\0' ||
        source == 0 || result == 0) return XIUL_E_INVALID;
    *result = 0;
    for (i = 0; i < filesystem_count; ++i)
        if (name_equal(filesystems[i]->name, filesystem_name)) {
            filesystem = filesystems[i]; break;
        }
    if (filesystem == 0) return XIUL_E_NOT_FOUND;
    for (i = 0; i < XIUL_VFS_MAX_MOUNTS; ++i)
        if (!mounts[i].active) { mount = &mounts[i]; break; }
    if (mount == 0) return XIUL_E_CAPACITY;
    mount->filesystem = filesystem;
    mount->source = source;
    mount->private_data = 0;
    mount->open_files = 0;
    mount->active = 1;
    status = filesystem->mount(mount, source, mount_options);
    if (status != XIUL_OK) {
        mount->filesystem = 0; mount->source = 0;
        mount->private_data = 0; mount->open_files = 0; mount->active = 0;
        return status;
    }
    ++mount_count;
    *result = mount;
    return XIUL_OK;
}

xiul_status_t xiul_vfs_unmount(xiul_mount_t *mount)
{
    xiul_status_t status;
    if (mount == 0 || !mount->active || mount->filesystem == 0)
        return XIUL_E_INVALID;
    if (mount->open_files != 0) return XIUL_E_BUSY;
    status = mount->filesystem->unmount(mount);
    if (status != XIUL_OK) return status;
    mount->filesystem = 0; mount->source = 0;
    mount->private_data = 0; mount->active = 0;
    --mount_count;
    return XIUL_OK;
}

xiul_status_t xiul_vfs_open(xiul_mount_t *mount, const char *path,
                             uint32_t flags, xiul_file_t **result)
{
    xiul_file_t *file = 0;
    size_t i;
    xiul_status_t status;
    if (mount == 0 || !mount->active || mount->filesystem == 0 ||
        path == 0 || path[0] == '\0' || result == 0 ||
        (flags & (XIUL_VFS_OPEN_READ | XIUL_VFS_OPEN_WRITE)) == 0 ||
        (flags & ~(XIUL_VFS_OPEN_READ | XIUL_VFS_OPEN_WRITE)) != 0)
        return XIUL_E_INVALID;
    *result = 0;
    for (i = 0; i < XIUL_VFS_MAX_FILES; ++i)
        if (!files[i].active) { file = &files[i]; break; }
    if (file == 0) return XIUL_E_CAPACITY;
    file->mount = mount;
    file->offset = 0;
    file->flags = flags;
    file->vnode.operations = 0;
    file->vnode.size = 0;
    file->vnode.mode = 0;
    file->vnode.private_data = 0;
    status = mount->filesystem->open(mount, path, flags, &file->vnode);
    if (status != XIUL_OK) {
        file->mount = 0;
        return status;
    }
    if (file->vnode.operations == 0 ||
        file->vnode.operations->close == 0 ||
        ((flags & XIUL_VFS_OPEN_READ) != 0 && file->vnode.operations->read == 0) ||
        ((flags & XIUL_VFS_OPEN_WRITE) != 0 && file->vnode.operations->write == 0)) {
        if (file->vnode.operations != 0 && file->vnode.operations->close != 0)
            (void)file->vnode.operations->close(&file->vnode);
        file->mount = 0;
        return XIUL_E_INVALID;
    }
    file->active = 1;
    ++mount->open_files;
    ++file_count;
    *result = file;
    return XIUL_OK;
}

xiul_status_t xiul_vfs_read(xiul_file_t *file, void *buffer, size_t length,
                             size_t *transferred)
{
    xiul_status_t status;
    size_t amount = 0;
    if (file == 0 || !file->active || transferred == 0 ||
        (length != 0 && buffer == 0) || (file->flags & XIUL_VFS_OPEN_READ) == 0)
        return XIUL_E_INVALID;
    *transferred = 0;
    status = file->vnode.operations->read(&file->vnode, file->offset,
                                           buffer, length, &amount);
    if (status != XIUL_OK) return status;
    if (amount > length) return XIUL_E_STATE;
    file->offset += amount;
    *transferred = amount;
    return XIUL_OK;
}

xiul_status_t xiul_vfs_write(xiul_file_t *file, const void *buffer, size_t length,
                              size_t *transferred)
{
    xiul_status_t status;
    size_t amount = 0;
    if (file == 0 || !file->active || transferred == 0 ||
        (length != 0 && buffer == 0) || (file->flags & XIUL_VFS_OPEN_WRITE) == 0)
        return XIUL_E_INVALID;
    *transferred = 0;
    status = file->vnode.operations->write(&file->vnode, file->offset,
                                            buffer, length, &amount);
    if (status != XIUL_OK) return status;
    if (amount > length || UINT64_MAX - file->offset < amount) return XIUL_E_STATE;
    file->offset += amount;
    if (file->offset > file->vnode.size) file->vnode.size = file->offset;
    *transferred = amount;
    return XIUL_OK;
}

xiul_status_t xiul_vfs_close(xiul_file_t *file)
{
    xiul_status_t status;
    if (file == 0 || !file->active || file->mount == 0)
        return XIUL_E_INVALID;
    status = file->vnode.operations->close(&file->vnode);
    if (status != XIUL_OK) return status;
    file->active = 0;
    --file->mount->open_files;
    --file_count;
    file->mount = 0;
    file->vnode.operations = 0;
    file->vnode.private_data = 0;
    return XIUL_OK;
}

size_t xiul_vfs_filesystem_count(void) { return filesystem_count; }
size_t xiul_vfs_mount_count(void) { return mount_count; }
size_t xiul_vfs_file_count(void) { return file_count; }

#ifdef XIUL_TESTING
void xiul_vfs_test_reset(void)
{
    size_t i;
    for (i = 0; i < XIUL_VFS_MAX_FILES; ++i) {
        files[i].mount = 0; files[i].active = 0;
        files[i].vnode.operations = 0; files[i].vnode.private_data = 0;
    }
    for (i = 0; i < XIUL_VFS_MAX_MOUNTS; ++i) {
        mounts[i].filesystem = 0; mounts[i].source = 0;
        mounts[i].private_data = 0; mounts[i].open_files = 0; mounts[i].active = 0;
    }
    for (i = 0; i < filesystem_count; ++i) filesystems[i] = 0;
    filesystem_count = 0; mount_count = 0; file_count = 0;
}
#endif
