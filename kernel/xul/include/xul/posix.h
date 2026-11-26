/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#ifndef XUL_POSIX_H
#define XUL_POSIX_H
#include <stddef.h>
#include <stdint.h>
#include "xul/status.h"
#include "xul/vfs.h"
#define XUL_POSIX_SYSCALL_MAX 256u
#define XUL_POSIX_ARG_MAX 6u
#define XUL_POSIX_FD_MAX 64u
#define XUL_POSIX_ENOSYS 38

typedef struct {
    uint64_t arguments[XUL_POSIX_ARG_MAX];
    void *thread_context;
} xul_posix_call_t;
typedef int64_t (*xul_posix_syscall_fn)(xul_posix_call_t *);

typedef struct {
    xul_file_t *files[XUL_POSIX_FD_MAX];
} xul_posix_process_t;

xul_status_t xul_posix_register(uint32_t number, xul_posix_syscall_fn handler);
xul_status_t xul_posix_unregister(uint32_t number);
int64_t xul_posix_dispatch(uint32_t number, xul_posix_call_t *call);
size_t xul_posix_registered_count(void);

void xul_posix_process_init(xul_posix_process_t *process);
xul_status_t xul_posix_process_destroy(xul_posix_process_t *process);
xul_status_t xul_posix_fd_install(xul_posix_process_t *process,
                                     xul_file_t *file, int *descriptor);
xul_status_t xul_posix_fd_close(xul_posix_process_t *process, int descriptor);
xul_status_t xul_posix_fd_read(xul_posix_process_t *process, int descriptor,
                                  void *buffer, size_t length, size_t *transferred);
xul_status_t xul_posix_fd_write(xul_posix_process_t *process, int descriptor,
                                   const void *buffer, size_t length,
                                   size_t *transferred);
#ifdef XUL_TESTING
void xul_posix_test_reset(void);
#endif
#endif
