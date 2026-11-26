/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include <stdint.h>
#include "xul/platform.h"

static volatile uint64_t xul_timer_ticks;

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

void xul_interrupt_dispatch(uint64_t vector, uint64_t error_code)
{
    if (vector == 32u) {
        if (xul_timer_ticks == 0)
            xul_platform_console_write("XUL: PIT timer interrupt received\n");
        ++xul_timer_ticks;
        xul_platform_irq_eoi(vector);
        return;
    }
    xul_exception_dispatch(vector, error_code);
}

_Noreturn void xul_exception_fatal(void)
{
    xul_platform_console_write("XUL: fatal CPU exception; system halted\n");
    xul_platform_halt();
}
