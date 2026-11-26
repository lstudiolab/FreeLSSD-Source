/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "xul/posix.h"

static char content[32];
static size_t content_length;
static xul_status_t fs_mount(xul_mount_t *m, const char *s, void *o)
{ (void)s; (void)o; m->private_data = 0; return XUL_OK; }
static xul_status_t fs_unmount(xul_mount_t *m)
{ (void)m; return XUL_OK; }
static xul_status_t node_read(xul_vnode_t *n, uint64_t off, void *buf,
                               size_t len, size_t *done)
{
    size_t amount = off < content_length ? content_length - (size_t)off : 0;
    (void)n;
    if (amount > len) amount = len;
    if (amount) memcpy(buf, content + (size_t)off, amount);
    *done = amount;
    return XUL_OK;
}
static xul_status_t node_write(xul_vnode_t *n, uint64_t off, const void *buf,
                                size_t len, size_t *done)
{
    size_t amount = len;
    (void)n;
    if (off >= sizeof(content)) amount = 0;
    else if (amount > sizeof(content) - (size_t)off)
        amount = sizeof(content) - (size_t)off;
    if (amount) memcpy(content + (size_t)off, buf, amount);
    if (off + amount > content_length) content_length = (size_t)off + amount;
    *done = amount;
    return XUL_OK;
}
static xul_status_t node_close(xul_vnode_t *n)
{ (void)n; return XUL_OK; }
static const xul_vnode_ops_t node_ops = {node_read, node_write, node_close};
static xul_status_t fs_open(xul_mount_t *m, const char *p, uint32_t f, xul_vnode_t *n)
{
    (void)m; (void)f;
    if (strcmp(p, "/data") != 0) return XUL_E_NOT_FOUND;
    n->operations = &node_ops; n->size = content_length; return XUL_OK;
}
int main(void)
{
    const xul_filesystem_t fs = {"posix-test", fs_mount, fs_unmount, fs_open};
    xul_mount_t *mount = 0;
    xul_file_t *file = 0;
    xul_posix_process_t process;
    int fd = -1;
    size_t done = 0;
    char out[8] = {0};
    xul_vfs_test_reset();
    xul_posix_process_init(&process);
    content_length = 0;
    assert(xul_vfs_register(&fs) == XUL_OK);
    assert(xul_vfs_mount("posix-test", "ram", 0, &mount) == XUL_OK);
    assert(xul_vfs_open(mount, "/data", XUL_VFS_OPEN_READ | XUL_VFS_OPEN_WRITE, &file) == XUL_OK);
    assert(xul_posix_fd_install(&process, file, &fd) == XUL_OK && fd == 0);
    assert(xul_posix_fd_install(&process, file, &fd) == XUL_E_DUPLICATE);
    assert(xul_posix_fd_write(&process, fd, "abc", 3, &done) == XUL_OK && done == 3);
    assert(xul_posix_fd_close(&process, fd) == XUL_OK);
    assert(xul_posix_fd_read(&process, fd, out, 3, &done) == XUL_E_NOT_FOUND);
    assert(xul_vfs_open(mount, "/data", XUL_VFS_OPEN_READ, &file) == XUL_OK);
    assert(xul_posix_fd_install(&process, file, &fd) == XUL_OK && fd == 0);
    assert(xul_posix_fd_read(&process, fd, out, 3, &done) == XUL_OK && done == 3);
    assert(memcmp(out, "abc", 3) == 0);
    assert(xul_posix_process_destroy(&process) == XUL_OK);
    assert(xul_vfs_unmount(mount) == XUL_OK);
    assert(xul_vfs_unregister(&fs) == XUL_OK);
    xul_vfs_test_reset();
    puts("XUL POSIX descriptor/VFS integration tests: PASS");
    return 0;
}
