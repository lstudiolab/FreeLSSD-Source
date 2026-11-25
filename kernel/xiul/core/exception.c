/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include <stdint.h>
#include "xiul/platform.h"

static void write_hex(uint64_t value)
{
    static const char digits[] = "0123456789abcdef";
    char buffer[16];
    unsigned int index;
    for (index = 0; index < 16; ++index) {
        buffer[15u - index] = digits[value & 0xfu];
        value >>= 4;
    }
    xiul_platform_console_write("0x");
    for (index = 0; index < 16; ++index) xiul_platform_console_putc(buffer[index]);
}

_Noreturn void xiul_exception_dispatch(uint64_t vector, uint64_t error_code)
{
    xiul_platform_console_write("XIUL: CPU exception vector=");
    write_hex(vector);
    xiul_platform_console_write(" error=");
    write_hex(error_code);
    xiul_platform_console_write("\n");
    xiul_platform_halt();
}

_Noreturn void xiul_exception_fatal(void)
{
    xiul_platform_console_write("XIUL: fatal CPU exception; system halted\n");
    xiul_platform_halt();
}
