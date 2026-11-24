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
    xiul_ring_stream_t stream;

    assert(xiul_ring_stream_init(&stream, storage, sizeof(storage)) == XIUL_OK);
    assert(xiul_ring_stream_write(&stream, "abcdef", 6, &count) == XIUL_OK);
    assert(count == 4);
    assert(xiul_ring_stream_available(&stream) == 4);
    assert(xiul_ring_stream_write(&stream, "x", 1, &count) == XIUL_E_BUSY);
    assert(xiul_ring_stream_read(&stream, output, 3, &count) == XIUL_OK);
    assert(count == 3 && memcmp(output, "abc", 3) == 0);
    assert(xiul_ring_stream_write(&stream, "efg", 3, &count) == XIUL_OK);
    assert(count == 3);
    memset(output, 0, sizeof(output));
    assert(xiul_ring_stream_read(&stream, output, 4, &count) == XIUL_OK);
    assert(count == 4 && memcmp(output, "defg", 4) == 0);
    assert(xiul_ring_stream_read(&stream, output, 1, &count) == XIUL_E_BUSY);
    assert(xiul_ring_stream_close(&stream) == XIUL_OK);
    assert(xiul_ring_stream_write(&stream, "z", 1, &count) == XIUL_E_STATE);
    assert(xiul_ring_stream_read(&stream, output, sizeof(output), &count) == XIUL_OK);
    assert(count == 0);
    puts("XIUL I/O stream tests: PASS");
    return 0;
}
