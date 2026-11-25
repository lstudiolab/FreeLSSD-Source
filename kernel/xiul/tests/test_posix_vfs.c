/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "xiul/posix.h"

static char content[32];
static size_t content_length;
static xiul_status_t fs_mount(xiul_mount_t *m, const char *s, void *o)
{ (void)s; (void)o; m->private_data = 0; return XIUL_OK; }
static xiul_status_t fs_unmount(xiul_mount_t *m)
{ (void)m; return XIUL_OK; }
static xiul_status_t node_read(xiul_vnode_t *n, uint64_t off, void *buf,
                               size_t len, size_t *done)
{
    size_t amount = off < content_length ? content_length - (size_t)off : 0;
    (void)n;
    if (amount > len) amount = len;
    if (amount) memcpy(buf, content + (size_t)off, amount);
    *done = amount;
    return XIUL_OK;
}
static xiul_status_t node_write(xiul_vnode_t *n, uint64_t off, const void *buf,
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
    return XIUL_OK;
}
static xiul_status_t node_close(xiul_vnode_t *n)
{ (void)n; return XIUL_OK; }
static const xiul_vnode_ops_t node_ops = {node_read, node_write, node_close};
static xiul_status_t fs_open(xiul_mount_t *m, const char *p, uint32_t f, xiul_vnode_t *n)
{
    (void)m; (void)f;
    if (strcmp(p, "/data") != 0) return XIUL_E_NOT_FOUND;
    n->operations = &node_ops; n->size = content_length; return XIUL_OK;
}
int main(void)
{
    const xiul_filesystem_t fs = {"posix-test", fs_mount, fs_unmount, fs_open};
    xiul_mount_t *mount = 0;
    xiul_file_t *file = 0;
    xiul_posix_process_t process;
    int fd = -1;
    size_t done = 0;
    char out[8] = {0};
    xiul_vfs_test_reset();
    xiul_posix_process_init(&process);
    content_length = 0;
    assert(xiul_vfs_register(&fs) == XIUL_OK);
    assert(xiul_vfs_mount("posix-test", "ram", 0, &mount) == XIUL_OK);
    assert(xiul_vfs_open(mount, "/data", XIUL_VFS_OPEN_READ | XIUL_VFS_OPEN_WRITE, &file) == XIUL_OK);
    assert(xiul_posix_fd_install(&process, file, &fd) == XIUL_OK && fd == 0);
    assert(xiul_posix_fd_install(&process, file, &fd) == XIUL_E_DUPLICATE);
    assert(xiul_posix_fd_write(&process, fd, "abc", 3, &done) == XIUL_OK && done == 3);
    assert(xiul_posix_fd_close(&process, fd) == XIUL_OK);
    assert(xiul_posix_fd_read(&process, fd, out, 3, &done) == XIUL_E_NOT_FOUND);
    assert(xiul_vfs_open(mount, "/data", XIUL_VFS_OPEN_READ, &file) == XIUL_OK);
    assert(xiul_posix_fd_install(&process, file, &fd) == XIUL_OK && fd == 0);
    assert(xiul_posix_fd_read(&process, fd, out, 3, &done) == XIUL_OK && done == 3);
    assert(memcmp(out, "abc", 3) == 0);
    assert(xiul_posix_process_destroy(&process) == XIUL_OK);
    assert(xiul_vfs_unmount(mount) == XIUL_OK);
    assert(xiul_vfs_unregister(&fs) == XIUL_OK);
    xiul_vfs_test_reset();
    puts("XIUL POSIX descriptor/VFS integration tests: PASS");
    return 0;
}
