/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "xul/ipc.h"

static void test_message_round_trip(void)
{
    const char payload[] = "Mach IPC";
    xul_port_t port;
    xul_ipc_message_t message;

    assert(xul_port_create(10, &port) == XUL_E_STATE);
    assert(xul_ipc_initialize() == XUL_OK);
    assert(xul_port_create(10, &port) == XUL_OK);
    assert(xul_ipc_port_count() == 1);
    assert(xul_port_receive(10, port, &message) == XUL_E_BUSY);
    assert(xul_port_receive(11, port, &message) == XUL_E_UNSUPPORTED);
    assert(xul_port_send(port, 11, 42, payload, sizeof(payload)) == XUL_OK);
    assert(xul_port_receive(10, port, &message) == XUL_OK);
    assert(message.sender == 11);
    assert(message.id == 42);
    assert(message.size == sizeof(payload));
    assert(memcmp(message.data, payload, sizeof(payload)) == 0);
    assert(xul_port_destroy(11, port) == XUL_E_UNSUPPORTED);
    assert(xul_port_destroy(10, port) == XUL_OK);
    assert(xul_port_send(port, 11, 1, payload, sizeof(payload)) == XUL_E_NOT_FOUND);
    assert(xul_ipc_port_count() == 0);
}

static void test_capacity_and_stale_handles(void)
{
    xul_port_t ports[XUL_IPC_MAX_PORTS];
    xul_port_t replacement;
    xul_ipc_message_t message;
    size_t i;

    for (i = 0; i < XUL_IPC_MAX_PORTS; ++i)
        assert(xul_port_create(100u + i, &ports[i]) == XUL_OK);
    assert(xul_port_create(999, &replacement) == XUL_E_CAPACITY);
    assert(xul_port_destroy(100, ports[0]) == XUL_OK);
    assert(xul_port_create(999, &replacement) == XUL_OK);
    assert(replacement != ports[0]);
    assert(xul_port_receive(100, ports[0], &message) == XUL_E_NOT_FOUND);
    for (i = 1; i < XUL_IPC_MAX_PORTS; ++i)
        assert(xul_port_destroy(100u + i, ports[i]) == XUL_OK);
    assert(xul_port_destroy(999, replacement) == XUL_OK);
    assert(xul_ipc_port_count() == 0);
}

static void test_queue_bounds(void)
{
    xul_port_t port;
    unsigned int i;
    uint32_t value = 7;
    xul_ipc_message_t message;

    assert(xul_port_create(500, &port) == XUL_OK);
    for (i = 0; i < XUL_IPC_QUEUE_DEPTH; ++i)
        assert(xul_port_send(port, 501, i, &value, sizeof(value)) == XUL_OK);
    assert(xul_port_send(port, 501, 99, &value, sizeof(value)) == XUL_E_BUSY);
    for (i = 0; i < XUL_IPC_QUEUE_DEPTH; ++i) {
        assert(xul_port_receive(500, port, &message) == XUL_OK);
        assert(message.id == i);
    }
    assert(xul_port_receive(500, port, &message) == XUL_E_BUSY);
    assert(xul_port_destroy(500, port) == XUL_OK);
}

int main(void)
{
    test_message_round_trip();
    test_capacity_and_stale_handles();
    test_queue_bounds();
    xul_ipc_shutdown();
    puts("XUL Mach-style IPC tests: PASS");
    return 0;
}
