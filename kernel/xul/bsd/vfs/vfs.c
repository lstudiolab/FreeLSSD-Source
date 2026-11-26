/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include "xul/vfs.h"

static const xul_filesystem_t *filesystems[XUL_VFS_MAX_FILESYSTEMS];
static xul_mount_t mounts[XUL_VFS_MAX_MOUNTS];
static xul_file_t files[XUL_VFS_MAX_FILES];
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

xul_status_t xul_vfs_register(const xul_filesystem_t *filesystem)
{
    size_t i;
    if (filesystem == 0 || filesystem->name == 0 ||
        filesystem->name[0] == '\0' || filesystem->mount == 0 ||
        filesystem->unmount == 0 || filesystem->open == 0) return XUL_E_INVALID;
    for (i = 0; i < filesystem_count; ++i) {
        if (filesystems[i] == filesystem ||
            name_equal(filesystems[i]->name, filesystem->name)) return XUL_E_DUPLICATE;
    }
    if (filesystem_count == XUL_VFS_MAX_FILESYSTEMS) return XUL_E_CAPACITY;
    filesystems[filesystem_count++] = filesystem;
    return XUL_OK;
}

xul_status_t xul_vfs_unregister(const xul_filesystem_t *filesystem)
{
    size_t i, j;
    if (filesystem == 0) return XUL_E_INVALID;
    for (i = 0; i < filesystem_count; ++i) {
        if (filesystems[i] != filesystem) continue;
        for (j = 0; j < XUL_VFS_MAX_MOUNTS; ++j)
            if (mounts[j].active && mounts[j].filesystem == filesystem) return XUL_E_BUSY;
        for (; i + 1 < filesystem_count; ++i) filesystems[i] = filesystems[i + 1];
        filesystems[--filesystem_count] = 0;
        return XUL_OK;
    }
    return XUL_E_NOT_FOUND;
}

xul_status_t xul_vfs_mount(const char *filesystem_name, const char *source,
                             void *mount_options, xul_mount_t **result)
{
    const xul_filesystem_t *filesystem = 0;
    xul_mount_t *mount = 0;
    size_t i;
    xul_status_t status;
    if (filesystem_name == 0 || filesystem_name[0] == '\0' ||
        source == 0 || result == 0) return XUL_E_INVALID;
    *result = 0;
    for (i = 0; i < filesystem_count; ++i)
        if (name_equal(filesystems[i]->name, filesystem_name)) {
            filesystem = filesystems[i]; break;
        }
    if (filesystem == 0) return XUL_E_NOT_FOUND;
    for (i = 0; i < XUL_VFS_MAX_MOUNTS; ++i)
        if (!mounts[i].active) { mount = &mounts[i]; break; }
    if (mount == 0) return XUL_E_CAPACITY;
    mount->filesystem = filesystem;
    mount->source = source;
    mount->private_data = 0;
    mount->open_files = 0;
    mount->active = 1;
    status = filesystem->mount(mount, source, mount_options);
    if (status != XUL_OK) {
        mount->filesystem = 0; mount->source = 0;
        mount->private_data = 0; mount->open_files = 0; mount->active = 0;
        return status;
    }
    ++mount_count;
    *result = mount;
    return XUL_OK;
}

xul_status_t xul_vfs_unmount(xul_mount_t *mount)
{
    xul_status_t status;
    if (mount == 0 || !mount->active || mount->filesystem == 0)
        return XUL_E_INVALID;
    if (mount->open_files != 0) return XUL_E_BUSY;
    status = mount->filesystem->unmount(mount);
    if (status != XUL_OK) return status;
    mount->filesystem = 0; mount->source = 0;
    mount->private_data = 0; mount->active = 0;
    --mount_count;
    return XUL_OK;
}

xul_status_t xul_vfs_open(xul_mount_t *mount, const char *path,
                             uint32_t flags, xul_file_t **result)
{
    xul_file_t *file = 0;
    size_t i;
    xul_status_t status;
    if (mount == 0 || !mount->active || mount->filesystem == 0 ||
        path == 0 || path[0] == '\0' || result == 0 ||
        (flags & (XUL_VFS_OPEN_READ | XUL_VFS_OPEN_WRITE)) == 0 ||
        (flags & ~(XUL_VFS_OPEN_READ | XUL_VFS_OPEN_WRITE)) != 0)
        return XUL_E_INVALID;
    *result = 0;
    for (i = 0; i < XUL_VFS_MAX_FILES; ++i)
        if (!files[i].active) { file = &files[i]; break; }
    if (file == 0) return XUL_E_CAPACITY;
    file->mount = mount;
    file->offset = 0;
    file->flags = flags;
    file->vnode.operations = 0;
    file->vnode.size = 0;
    file->vnode.mode = 0;
    file->vnode.private_data = 0;
    status = mount->filesystem->open(mount, path, flags, &file->vnode);
    if (status != XUL_OK) {
        file->mount = 0;
        return status;
    }
    if (file->vnode.operations == 0 ||
        file->vnode.operations->close == 0 ||
        ((flags & XUL_VFS_OPEN_READ) != 0 && file->vnode.operations->read == 0) ||
        ((flags & XUL_VFS_OPEN_WRITE) != 0 && file->vnode.operations->write == 0)) {
        if (file->vnode.operations != 0 && file->vnode.operations->close != 0)
            (void)file->vnode.operations->close(&file->vnode);
        file->mount = 0;
        return XUL_E_INVALID;
    }
    file->active = 1;
    ++mount->open_files;
    ++file_count;
    *result = file;
    return XUL_OK;
}

xul_status_t xul_vfs_read(xul_file_t *file, void *buffer, size_t length,
                             size_t *transferred)
{
    xul_status_t status;
    size_t amount = 0;
    if (file == 0 || !file->active || transferred == 0 ||
        (length != 0 && buffer == 0) || (file->flags & XUL_VFS_OPEN_READ) == 0)
        return XUL_E_INVALID;
    *transferred = 0;
    status = file->vnode.operations->read(&file->vnode, file->offset,
                                           buffer, length, &amount);
    if (status != XUL_OK) return status;
    if (amount > length || UINT64_MAX - file->offset < amount)
        return XUL_E_STATE;
    file->offset += amount;
    *transferred = amount;
    return XUL_OK;
}

xul_status_t xul_vfs_write(xul_file_t *file, const void *buffer, size_t length,
                              size_t *transferred)
{
    xul_status_t status;
    size_t amount = 0;
    if (file == 0 || !file->active || transferred == 0 ||
        (length != 0 && buffer == 0) || (file->flags & XUL_VFS_OPEN_WRITE) == 0)
        return XUL_E_INVALID;
    *transferred = 0;
    status = file->vnode.operations->write(&file->vnode, file->offset,
                                            buffer, length, &amount);
    if (status != XUL_OK) return status;
    if (amount > length || UINT64_MAX - file->offset < amount) return XUL_E_STATE;
    file->offset += amount;
    if (file->offset > file->vnode.size) file->vnode.size = file->offset;
    *transferred = amount;
    return XUL_OK;
}

xul_status_t xul_vfs_close(xul_file_t *file)
{
    xul_status_t status;
    if (file == 0 || !file->active || file->mount == 0)
        return XUL_E_INVALID;
    status = file->vnode.operations->close(&file->vnode);
    if (status != XUL_OK) return status;
    file->active = 0;
    --file->mount->open_files;
    --file_count;
    file->mount = 0;
    file->vnode.operations = 0;
    file->vnode.private_data = 0;
    return XUL_OK;
}

size_t xul_vfs_filesystem_count(void) { return filesystem_count; }
size_t xul_vfs_mount_count(void) { return mount_count; }
size_t xul_vfs_file_count(void) { return file_count; }

#ifdef XUL_TESTING
void xul_vfs_test_reset(void)
{
    size_t i;
    for (i = 0; i < XUL_VFS_MAX_FILES; ++i) {
        files[i].mount = 0; files[i].active = 0;
        files[i].vnode.operations = 0; files[i].vnode.private_data = 0;
    }
    for (i = 0; i < XUL_VFS_MAX_MOUNTS; ++i) {
        mounts[i].filesystem = 0; mounts[i].source = 0;
        mounts[i].private_data = 0; mounts[i].open_files = 0; mounts[i].active = 0;
    }
    for (i = 0; i < filesystem_count; ++i) filesystems[i] = 0;
    filesystem_count = 0; mount_count = 0; file_count = 0;
}
#endif
