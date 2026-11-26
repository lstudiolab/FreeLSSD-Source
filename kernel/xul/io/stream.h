/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#ifndef XUL_IO_STREAM_H
#define XUL_IO_STREAM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdatomic.h>
#include "xul/status.h"

/*
 * Bounded single-producer/single-consumer byte stream.
 * Storage is supplied by the caller; no allocation occurs in the I/O path.
 */
typedef struct {
    uint8_t *storage;
    size_t capacity;
    atomic_size_t head;
    atomic_size_t tail;
    atomic_bool closed;
} xul_ring_stream_t;

xul_status_t xul_ring_stream_init(xul_ring_stream_t *stream,
                                    uint8_t *storage, size_t capacity);
xul_status_t xul_ring_stream_write(xul_ring_stream_t *stream,
                                     const void *data, size_t length,
                                     size_t *transferred);
xul_status_t xul_ring_stream_read(xul_ring_stream_t *stream,
                                    void *data, size_t capacity,
                                    size_t *transferred);
xul_status_t xul_ring_stream_close(xul_ring_stream_t *stream);
size_t xul_ring_stream_available(const xul_ring_stream_t *stream);

#endif
