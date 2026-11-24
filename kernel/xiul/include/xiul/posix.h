#ifndef XIUL_POSIX_H
#define XIUL_POSIX_H
#include <stddef.h>
#include <stdint.h>
#include "xiul/status.h"
#include "xiul/vfs.h"
#define XIUL_POSIX_SYSCALL_MAX 256u
#define XIUL_POSIX_ARG_MAX 6u
#define XIUL_POSIX_FD_MAX 64u
#define XIUL_POSIX_ENOSYS 38

typedef struct {
    uint64_t arguments[XIUL_POSIX_ARG_MAX];
    void *thread_context;
} xiul_posix_call_t;
typedef int64_t (*xiul_posix_syscall_fn)(xiul_posix_call_t *);

typedef struct {
    xiul_file_t *files[XIUL_POSIX_FD_MAX];
} xiul_posix_process_t;

xiul_status_t xiul_posix_register(uint32_t number, xiul_posix_syscall_fn handler);
xiul_status_t xiul_posix_unregister(uint32_t number);
int64_t xiul_posix_dispatch(uint32_t number, xiul_posix_call_t *call);
size_t xiul_posix_registered_count(void);

void xiul_posix_process_init(xiul_posix_process_t *process);
xiul_status_t xiul_posix_process_destroy(xiul_posix_process_t *process);
xiul_status_t xiul_posix_fd_install(xiul_posix_process_t *process,
                                     xiul_file_t *file, int *descriptor);
xiul_status_t xiul_posix_fd_close(xiul_posix_process_t *process, int descriptor);
xiul_status_t xiul_posix_fd_read(xiul_posix_process_t *process, int descriptor,
                                  void *buffer, size_t length, size_t *transferred);
xiul_status_t xiul_posix_fd_write(xiul_posix_process_t *process, int descriptor,
                                   const void *buffer, size_t length,
                                   size_t *transferred);
#ifdef XIUL_TESTING
void xiul_posix_test_reset(void);
#endif
#endif
