/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#ifndef XUL_IPC_H
#define XUL_IPC_H

#include <stddef.h>
#include <stdint.h>
#include "xul/status.h"

#ifdef __cplusplus
extern "C" {
#endif

#define XUL_IPC_MAX_PORTS 32u
#define XUL_IPC_QUEUE_DEPTH 16u
#define XUL_IPC_MAX_MESSAGE_SIZE 128u

typedef uint32_t xul_port_t;

typedef struct {
    uint64_t sender;
    uint32_t id;
    size_t size;
    uint8_t data[XUL_IPC_MAX_MESSAGE_SIZE];
} xul_ipc_message_t;

/*
 * Bounded, nonblocking Mach-style message ports.
 * A port has one owning task; senders may enqueue, only its owner may receive.
 * Port handles carry generations so a destroyed port cannot be reused through
 * a stale handle. This is an initial IPC transport, not the complete Mach ABI.
 */
xul_status_t xul_ipc_initialize(void);
void xul_ipc_shutdown(void);
xul_status_t xul_port_create(uint64_t owner, xul_port_t *port);
xul_status_t xul_port_destroy(uint64_t owner, xul_port_t port);
xul_status_t xul_port_send(xul_port_t port, uint64_t sender,
                           uint32_t message_id, const void *data, size_t size);
xul_status_t xul_port_receive(uint64_t receiver, xul_port_t port,
                              xul_ipc_message_t *message);
size_t xul_ipc_port_count(void);

#ifdef __cplusplus
}
#endif
#endif
