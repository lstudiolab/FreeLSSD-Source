/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include <stdint.h>
#include "xul/platform.h"

static void write_hex(uint64_t value)
{
    static const char digits[] = "0123456789abcdef";
    char buffer[16];
    unsigned int index;
    for (index = 0; index < 16; ++index) {
        buffer[15u - index] = digits[value & 0xfu];
        value >>= 4;
    }
    xul_platform_console_write("0x");
    for (index = 0; index < 16; ++index) xul_platform_console_putc(buffer[index]);
}

_Noreturn void xul_exception_dispatch(uint64_t vector, uint64_t error_code)
{
    xul_platform_console_write("XUL: CPU exception vector=");
    write_hex(vector);
    xul_platform_console_write(" error=");
    write_hex(error_code);
    xul_platform_console_write("\n");
    xul_platform_halt();
}

_Noreturn void xul_exception_fatal(void)
{
    xul_platform_console_write("XUL: fatal CPU exception; system halted\n");
    xul_platform_halt();
}
