/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 *
 * Minimal freestanding C runtime primitives required by XIUL core code.
 */

#include <stddef.h>

void *memset(void *destination, int value, size_t length)
{
    volatile unsigned char *out = (volatile unsigned char *)destination;
    size_t i;
    for (i = 0; i < length; ++i) out[i] = (unsigned char)value;
    return destination;
}

void *memcpy(void *destination, const void *source, size_t length)
{
    volatile unsigned char *out = (volatile unsigned char *)destination;
    const volatile unsigned char *in = (const volatile unsigned char *)source;
    size_t i;
    for (i = 0; i < length; ++i) out[i] = in[i];
    return destination;
}

void *memmove(void *destination, const void *source, size_t length)
{
    volatile unsigned char *out = (volatile unsigned char *)destination;
    const volatile unsigned char *in = (const volatile unsigned char *)source;
    if (out == in || length == 0) return destination;
    if ((uintptr_t)out < (uintptr_t)in) {
        size_t i;
        for (i = 0; i < length; ++i) out[i] = in[i];
    } else {
        while (length != 0) {
            --length;
            out[length] = in[length];
        }
    }
    return destination;
}

int memcmp(const void *left, const void *right, size_t length)
{
    const volatile unsigned char *a = (const volatile unsigned char *)left;
    const volatile unsigned char *b = (const volatile unsigned char *)right;
    size_t i;
    for (i = 0; i < length; ++i) {
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    return 0;
}
