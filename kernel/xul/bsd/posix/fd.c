/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include "xul/posix.h"

void xul_posix_process_init(xul_posix_process_t *process)
{
    size_t i;
    if (process == 0) return;
    for (i = 0; i < XUL_POSIX_FD_MAX; ++i) process->files[i] = 0;
}

xul_status_t xul_posix_fd_install(xul_posix_process_t *process,
                                     xul_file_t *file, int *descriptor)
{
    size_t i;
    if (process == 0 || file == 0 || !file->active || descriptor == 0)
        return XUL_E_INVALID;
    for (i = 0; i < XUL_POSIX_FD_MAX; ++i)
        if (process->files[i] == file) return XUL_E_DUPLICATE;
    for (i = 0; i < XUL_POSIX_FD_MAX; ++i) {
        if (process->files[i] != 0) continue;
        process->files[i] = file;
        *descriptor = (int)i;
        return XUL_OK;
    }
    return XUL_E_CAPACITY;
}

xul_status_t xul_posix_fd_close(xul_posix_process_t *process, int descriptor)
{
    xul_status_t status;
    if (process == 0 || descriptor < 0 || descriptor >= (int)XUL_POSIX_FD_MAX)
        return XUL_E_INVALID;
    if (process->files[descriptor] == 0) return XUL_E_NOT_FOUND;
    status = xul_vfs_close(process->files[descriptor]);
    if (status == XUL_OK) process->files[descriptor] = 0;
    return status;
}

xul_status_t xul_posix_fd_read(xul_posix_process_t *process, int descriptor,
                                  void *buffer, size_t length, size_t *transferred)
{
    if (process == 0 || descriptor < 0 || descriptor >= (int)XUL_POSIX_FD_MAX)
        return XUL_E_INVALID;
    if (process->files[descriptor] == 0) return XUL_E_NOT_FOUND;
    return xul_vfs_read(process->files[descriptor], buffer, length, transferred);
}

xul_status_t xul_posix_fd_write(xul_posix_process_t *process, int descriptor,
                                   const void *buffer, size_t length,
                                   size_t *transferred)
{
    if (process == 0 || descriptor < 0 || descriptor >= (int)XUL_POSIX_FD_MAX)
        return XUL_E_INVALID;
    if (process->files[descriptor] == 0) return XUL_E_NOT_FOUND;
    return xul_vfs_write(process->files[descriptor], buffer, length, transferred);
}

xul_status_t xul_posix_process_destroy(xul_posix_process_t *process)
{
    size_t i;
    xul_status_t first_error = XUL_OK;
    if (process == 0) return XUL_E_INVALID;
    for (i = 0; i < XUL_POSIX_FD_MAX; ++i) {
        if (process->files[i] == 0) continue;
        {
            xul_status_t status = xul_vfs_close(process->files[i]);
            if (status == XUL_OK) process->files[i] = 0;
            else if (first_error == XUL_OK) first_error = status;
        }
    }
    return first_error;
}
