/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include <stdatomic.h>
#include "xul/ipc.h"

typedef struct {
    uint64_t owner;
    uint16_t generation;
    uint8_t active;
    uint8_t head;
    uint8_t tail;
    uint8_t count;
    xul_ipc_message_t messages[XUL_IPC_QUEUE_DEPTH];
} xul_ipc_port_state_t;

static xul_ipc_port_state_t ports[XUL_IPC_MAX_PORTS];
static atomic_flag ipc_lock = ATOMIC_FLAG_INIT;
static size_t active_ports;
static unsigned int ipc_initialized;

static void lock_ipc(void)
{
    while (atomic_flag_test_and_set_explicit(&ipc_lock, memory_order_acquire)) {
        /* Bounded critical sections; callers must not hold the lock across I/O. */
    }
}

static void unlock_ipc(void)
{
    atomic_flag_clear_explicit(&ipc_lock, memory_order_release);
}

static xul_port_t make_handle(size_t slot, uint16_t generation)
{
    return ((uint32_t)generation << 16) | (uint32_t)(slot + 1u);
}

static xul_ipc_port_state_t *lookup_port(xul_port_t handle)
{
    uint32_t encoded_slot = handle & UINT32_C(0xffff);
    uint16_t generation = (uint16_t)(handle >> 16);
    size_t slot;

    if (encoded_slot == 0 || encoded_slot > XUL_IPC_MAX_PORTS || generation == 0)
        return 0;
    slot = (size_t)encoded_slot - 1u;
    if (!ports[slot].active || ports[slot].generation != generation) return 0;
    return &ports[slot];
}

xul_status_t xul_ipc_initialize(void)
{
    size_t i;
    lock_ipc();
    if (ipc_initialized) {
        unlock_ipc();
        return XUL_E_STATE;
    }
    active_ports = 0;
    for (i = 0; i < XUL_IPC_MAX_PORTS; ++i) {
        uint16_t generation = (uint16_t)(ports[i].generation + 1u);
        if (generation == 0) generation = 1;
        ports[i].owner = 0;
        ports[i].generation = generation;
        ports[i].active = 0;
        ports[i].head = 0;
        ports[i].tail = 0;
        ports[i].count = 0;
    }
    ipc_initialized = 1;
    unlock_ipc();
    return XUL_OK;
}

void xul_ipc_shutdown(void)
{
    size_t i;
    lock_ipc();
    for (i = 0; i < XUL_IPC_MAX_PORTS; ++i) {
        ports[i].active = 0;
        ports[i].owner = 0;
        ports[i].head = 0;
        ports[i].tail = 0;
        ports[i].count = 0;
    }
    active_ports = 0;
    ipc_initialized = 0;
    unlock_ipc();
}

xul_status_t xul_port_create(uint64_t owner, xul_port_t *port)
{
    size_t i;
    if (owner == 0 || port == 0) return XUL_E_INVALID;
    lock_ipc();
    if (!ipc_initialized) {
        unlock_ipc();
        return XUL_E_STATE;
    }
    for (i = 0; i < XUL_IPC_MAX_PORTS; ++i) {
        if (!ports[i].active) {
            ports[i].active = 1;
            ports[i].owner = owner;
            ports[i].head = 0;
            ports[i].tail = 0;
            ports[i].count = 0;
            ++active_ports;
            *port = make_handle(i, ports[i].generation);
            unlock_ipc();
            return XUL_OK;
        }
    }
    unlock_ipc();
    return XUL_E_CAPACITY;
}

xul_status_t xul_port_destroy(uint64_t owner, xul_port_t handle)
{
    xul_ipc_port_state_t *port;
    if (owner == 0) return XUL_E_INVALID;
    lock_ipc();
    if (!ipc_initialized) {
        unlock_ipc();
        return XUL_E_STATE;
    }
    port = lookup_port(handle);
    if (port == 0) {
        unlock_ipc();
        return XUL_E_NOT_FOUND;
    }
    if (port->owner != owner) {
        unlock_ipc();
        return XUL_E_UNSUPPORTED;
    }
    port->active = 0;
    port->owner = 0;
    port->head = 0;
    port->tail = 0;
    port->count = 0;
    if (active_ports != 0) --active_ports;
    ++port->generation;
    if (port->generation == 0) port->generation = 1;
    unlock_ipc();
    return XUL_OK;
}

xul_status_t xul_port_send(xul_port_t handle, uint64_t sender,
                           uint32_t message_id, const void *data, size_t size)
{
    xul_ipc_port_state_t *port;
    xul_ipc_message_t *message;
    size_t i;
    if (sender == 0 || size > XUL_IPC_MAX_MESSAGE_SIZE ||
        (size != 0 && data == 0)) return XUL_E_INVALID;
    lock_ipc();
    if (!ipc_initialized) {
        unlock_ipc();
        return XUL_E_STATE;
    }
    port = lookup_port(handle);
    if (port == 0) {
        unlock_ipc();
        return XUL_E_NOT_FOUND;
    }
    if (port->count == XUL_IPC_QUEUE_DEPTH) {
        unlock_ipc();
        return XUL_E_BUSY;
    }
    message = &port->messages[port->head];
    message->sender = sender;
    message->id = message_id;
    message->size = size;
    for (i = 0; i < size; ++i) message->data[i] = ((const uint8_t *)data)[i];
    port->head = (uint8_t)((port->head + 1u) % XUL_IPC_QUEUE_DEPTH);
    ++port->count;
    unlock_ipc();
    return XUL_OK;
}

xul_status_t xul_port_receive(uint64_t receiver, xul_port_t handle,
                              xul_ipc_message_t *message)
{
    xul_ipc_port_state_t *port;
    size_t i;
    if (receiver == 0 || message == 0) return XUL_E_INVALID;
    lock_ipc();
    if (!ipc_initialized) {
        unlock_ipc();
        return XUL_E_STATE;
    }
    port = lookup_port(handle);
    if (port == 0) {
        unlock_ipc();
        return XUL_E_NOT_FOUND;
    }
    if (port->owner != receiver) {
        unlock_ipc();
        return XUL_E_UNSUPPORTED;
    }
    if (port->count == 0) {
        unlock_ipc();
        return XUL_E_BUSY;
    }
    *message = port->messages[port->tail];
    for (i = message->size; i < XUL_IPC_MAX_MESSAGE_SIZE; ++i)
        message->data[i] = 0;
    port->tail = (uint8_t)((port->tail + 1u) % XUL_IPC_QUEUE_DEPTH);
    --port->count;
    unlock_ipc();
    return XUL_OK;
}

size_t xul_ipc_port_count(void)
{
    size_t count;
    lock_ipc();
    count = active_ports;
    unlock_ipc();
    return count;
}
