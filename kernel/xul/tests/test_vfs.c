/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "xul/vfs.h"

static char storage[64];
static size_t used;
static int mounted, unmounted, closed;

static xul_status_t mount_fs(xul_mount_t *mount, const char *source, void *options)
{ (void)options; ++mounted; mount->private_data = (void *)source; return XUL_OK; }
static xul_status_t unmount_fs(xul_mount_t *mount)
{ (void)mount; ++unmounted; return XUL_OK; }
static xul_status_t vnode_read(xul_vnode_t *node, uint64_t offset,
                                void *buffer, size_t length, size_t *done)
{
    size_t available = offset < used ? used - (size_t)offset : 0;
    size_t amount = available < length ? available : length;
    (void)node;
    if (amount != 0) memcpy(buffer, storage + (size_t)offset, amount);
    *done = amount;
    return XUL_OK;
}
static xul_status_t vnode_write(xul_vnode_t *node, uint64_t offset,
                                 const void *buffer, size_t length, size_t *done)
{
    size_t amount = length;
    (void)node;
    if (offset >= sizeof(storage)) amount = 0;
    else if (amount > sizeof(storage) - (size_t)offset)
        amount = sizeof(storage) - (size_t)offset;
    if (amount != 0) memcpy(storage + (size_t)offset, buffer, amount);
    if (offset + amount > used) used = (size_t)offset + amount;
    *done = amount;
    return XUL_OK;
}
static xul_status_t vnode_close(xul_vnode_t *node)
{ (void)node; ++closed; return XUL_OK; }
static const xul_vnode_ops_t vnode_ops = {vnode_read, vnode_write, vnode_close};
static xul_status_t open_fs(xul_mount_t *mount, const char *path,
                             uint32_t flags, xul_vnode_t *node)
{
    (void)mount; (void)flags;
    if (strcmp(path, "/hello.txt") != 0) return XUL_E_NOT_FOUND;
    node->operations = &vnode_ops;
    node->size = used;
    return XUL_OK;
}

int main(void)
{
    xul_mount_t *mount = 0;
    xul_file_t *file = 0;
    size_t done = 0;
    char output[16] = {0};
    const xul_filesystem_t fs = {"memfs", mount_fs, unmount_fs, open_fs};

    xul_vfs_test_reset();
    used = mounted = unmounted = closed = 0;
    assert(xul_vfs_register(&fs) == XUL_OK);
    assert(xul_vfs_mount("memfs", "ram0", 0, &mount) == XUL_OK);
    assert(mount != 0 && mount->active && mounted == 1);
    assert(xul_vfs_open(mount, "/hello.txt",
        XUL_VFS_OPEN_READ | XUL_VFS_OPEN_WRITE, &file) == XUL_OK);
    assert(xul_vfs_write(file, "hello", 5, &done) == XUL_OK && done == 5);
    assert(xul_vfs_read(file, output, 5, &done) == XUL_OK && done == 0);
    assert(xul_vfs_unmount(mount) == XUL_E_BUSY);
    assert(xul_vfs_close(file) == XUL_OK && closed == 1);

    assert(xul_vfs_open(mount, "/hello.txt", XUL_VFS_OPEN_READ, &file) == XUL_OK);
    assert(xul_vfs_read(file, output, 5, &done) == XUL_OK && done == 5);
    assert(memcmp(output, "hello", 5) == 0);
    assert(xul_vfs_close(file) == XUL_OK && closed == 2);
    assert(xul_vfs_unmount(mount) == XUL_OK && unmounted == 1);
    assert(xul_vfs_unregister(&fs) == XUL_OK);
    assert(xul_vfs_mount_count() == 0 && xul_vfs_file_count() == 0);
    xul_vfs_test_reset();
    puts("XUL VFS file and mount lifecycle tests: PASS");
    return 0;
}
