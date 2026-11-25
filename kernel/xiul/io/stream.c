/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include <stdint.h>
#include "stream.h"

static size_t copy_into_ring(uint8_t *ring, size_t capacity, size_t position,
                             const uint8_t *source, size_t length)
{
    size_t offset = position % capacity;
    size_t first = capacity - offset;
    if (first > length) first = length;
    for (size_t i = 0; i < first; ++i) ring[offset + i] = source[i];
    for (size_t i = first; i < length; ++i) ring[i - first] = source[i];
    return length;
}

static size_t copy_from_ring(uint8_t *destination, const uint8_t *ring,
                             size_t capacity, size_t position, size_t length)
{
    size_t offset = position % capacity;
    size_t first = capacity - offset;
    if (first > length) first = length;
    for (size_t i = 0; i < first; ++i) destination[i] = ring[offset + i];
    for (size_t i = first; i < length; ++i) destination[i] = ring[i - first];
    return length;
}

xiul_status_t xiul_ring_stream_init(xiul_ring_stream_t *stream,
                                    uint8_t *storage, size_t capacity)
{
    if (stream == 0 || storage == 0 || capacity == 0 || capacity > SIZE_MAX / 2) return XIUL_E_INVALID;
    stream->storage = storage;
    stream->capacity = capacity;
    atomic_init(&stream->head, 0);
    atomic_init(&stream->tail, 0);
    atomic_init(&stream->closed, false);
    return XIUL_OK;
}

xiul_status_t xiul_ring_stream_write(xiul_ring_stream_t *stream,
                                     const void *data, size_t length,
                                     size_t *transferred)
{
    size_t head, tail, used, amount;
    if (stream == 0 || transferred == 0 || (length != 0 && data == 0)) {
        return XIUL_E_INVALID;
    }
    *transferred = 0;
    if (atomic_load_explicit(&stream->closed, memory_order_acquire)) {
        return XIUL_E_STATE;
    }
    head = atomic_load_explicit(&stream->head, memory_order_relaxed);
    tail = atomic_load_explicit(&stream->tail, memory_order_acquire);
    used = head - tail;
    if (used > stream->capacity) return XIUL_E_STATE;
    amount = stream->capacity - used;
    if (amount > length) amount = length;
    if (amount == 0 && length != 0) return XIUL_E_BUSY;
    copy_into_ring(stream->storage, stream->capacity, head,
                   (const uint8_t *)data, amount);
    atomic_store_explicit(&stream->head, head + amount, memory_order_release);
    *transferred = amount;
    return XIUL_OK;
}

xiul_status_t xiul_ring_stream_read(xiul_ring_stream_t *stream,
                                    void *data, size_t capacity,
                                    size_t *transferred)
{
    size_t head, tail, available, amount;
    if (stream == 0 || transferred == 0 || (capacity != 0 && data == 0)) {
        return XIUL_E_INVALID;
    }
    *transferred = 0;
    tail = atomic_load_explicit(&stream->tail, memory_order_relaxed);
    head = atomic_load_explicit(&stream->head, memory_order_acquire);
    available = head - tail;
    if (available > stream->capacity) return XIUL_E_STATE;
    amount = available < capacity ? available : capacity;
    if (amount == 0) {
        return atomic_load_explicit(&stream->closed, memory_order_acquire)
            ? XIUL_OK : XIUL_E_BUSY;
    }
    copy_from_ring((uint8_t *)data, stream->storage, stream->capacity,
                   tail, amount);
    atomic_store_explicit(&stream->tail, tail + amount, memory_order_release);
    *transferred = amount;
    return XIUL_OK;
}

xiul_status_t xiul_ring_stream_close(xiul_ring_stream_t *stream)
{
    if (stream == 0) return XIUL_E_INVALID;
    atomic_store_explicit(&stream->closed, true, memory_order_release);
    return XIUL_OK;
}

size_t xiul_ring_stream_available(const xiul_ring_stream_t *stream)
{
    size_t head, tail, used;
    if (stream == 0 || stream->capacity == 0) return 0;
    head = atomic_load_explicit(&stream->head, memory_order_acquire);
    tail = atomic_load_explicit(&stream->tail, memory_order_acquire);
    used = head - tail;
    return used <= stream->capacity ? used : 0;
}
