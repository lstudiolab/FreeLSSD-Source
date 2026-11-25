/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#ifndef XIUL_IO_STREAM_H
#define XIUL_IO_STREAM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdatomic.h>
#include "xiul/status.h"

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
} xiul_ring_stream_t;

xiul_status_t xiul_ring_stream_init(xiul_ring_stream_t *stream,
                                    uint8_t *storage, size_t capacity);
xiul_status_t xiul_ring_stream_write(xiul_ring_stream_t *stream,
                                     const void *data, size_t length,
                                     size_t *transferred);
xiul_status_t xiul_ring_stream_read(xiul_ring_stream_t *stream,
                                    void *data, size_t capacity,
                                    size_t *transferred);
xiul_status_t xiul_ring_stream_close(xiul_ring_stream_t *stream);
size_t xiul_ring_stream_available(const xiul_ring_stream_t *stream);

#endif
