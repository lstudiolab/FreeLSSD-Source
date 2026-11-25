/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include "xiul/posix.h"

void xiul_posix_process_init(xiul_posix_process_t *process)
{
    size_t i;
    if (process == 0) return;
    for (i = 0; i < XIUL_POSIX_FD_MAX; ++i) process->files[i] = 0;
}

xiul_status_t xiul_posix_fd_install(xiul_posix_process_t *process,
                                     xiul_file_t *file, int *descriptor)
{
    size_t i;
    if (process == 0 || file == 0 || !file->active || descriptor == 0)
        return XIUL_E_INVALID;
    for (i = 0; i < XIUL_POSIX_FD_MAX; ++i)
        if (process->files[i] == file) return XIUL_E_DUPLICATE;
    for (i = 0; i < XIUL_POSIX_FD_MAX; ++i) {
        if (process->files[i] != 0) continue;
        process->files[i] = file;
        *descriptor = (int)i;
        return XIUL_OK;
    }
    return XIUL_E_CAPACITY;
}

xiul_status_t xiul_posix_fd_close(xiul_posix_process_t *process, int descriptor)
{
    xiul_status_t status;
    if (process == 0 || descriptor < 0 || descriptor >= (int)XIUL_POSIX_FD_MAX)
        return XIUL_E_INVALID;
    if (process->files[descriptor] == 0) return XIUL_E_NOT_FOUND;
    status = xiul_vfs_close(process->files[descriptor]);
    if (status == XIUL_OK) process->files[descriptor] = 0;
    return status;
}

xiul_status_t xiul_posix_fd_read(xiul_posix_process_t *process, int descriptor,
                                  void *buffer, size_t length, size_t *transferred)
{
    if (process == 0 || descriptor < 0 || descriptor >= (int)XIUL_POSIX_FD_MAX)
        return XIUL_E_INVALID;
    if (process->files[descriptor] == 0) return XIUL_E_NOT_FOUND;
    return xiul_vfs_read(process->files[descriptor], buffer, length, transferred);
}

xiul_status_t xiul_posix_fd_write(xiul_posix_process_t *process, int descriptor,
                                   const void *buffer, size_t length,
                                   size_t *transferred)
{
    if (process == 0 || descriptor < 0 || descriptor >= (int)XIUL_POSIX_FD_MAX)
        return XIUL_E_INVALID;
    if (process->files[descriptor] == 0) return XIUL_E_NOT_FOUND;
    return xiul_vfs_write(process->files[descriptor], buffer, length, transferred);
}

xiul_status_t xiul_posix_process_destroy(xiul_posix_process_t *process)
{
    size_t i;
    xiul_status_t first_error = XIUL_OK;
    if (process == 0) return XIUL_E_INVALID;
    for (i = 0; i < XIUL_POSIX_FD_MAX; ++i) {
        if (process->files[i] == 0) continue;
        {
            xiul_status_t status = xiul_vfs_close(process->files[i]);
            if (status == XIUL_OK) process->files[i] = 0;
            else if (first_error == XIUL_OK) first_error = status;
        }
    }
    return first_error;
}
