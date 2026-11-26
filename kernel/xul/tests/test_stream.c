/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "stream.h"

int main(void)
{
    uint8_t storage[4];
    char output[8] = {0};
    size_t count = 0;
    xul_ring_stream_t stream;

    assert(xul_ring_stream_init(&stream, storage, sizeof(storage)) == XUL_OK);
    assert(xul_ring_stream_write(&stream, "abcdef", 6, &count) == XUL_OK);
    assert(count == 4);
    assert(xul_ring_stream_available(&stream) == 4);
    assert(xul_ring_stream_write(&stream, "x", 1, &count) == XUL_E_BUSY);
    assert(xul_ring_stream_read(&stream, output, 3, &count) == XUL_OK);
    assert(count == 3 && memcmp(output, "abc", 3) == 0);
    assert(xul_ring_stream_write(&stream, "efg", 3, &count) == XUL_OK);
    assert(count == 3);
    memset(output, 0, sizeof(output));
    assert(xul_ring_stream_read(&stream, output, 4, &count) == XUL_OK);
    assert(count == 4 && memcmp(output, "defg", 4) == 0);
    assert(xul_ring_stream_read(&stream, output, 1, &count) == XUL_E_BUSY);
    assert(xul_ring_stream_close(&stream) == XUL_OK);
    assert(xul_ring_stream_write(&stream, "z", 1, &count) == XUL_E_STATE);
    assert(xul_ring_stream_read(&stream, output, sizeof(output), &count) == XUL_OK);
    assert(count == 0);
    puts("XUL I/O stream tests: PASS");
    return 0;
}
