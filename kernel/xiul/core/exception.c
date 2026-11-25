/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include "xiul/platform.h"

_Noreturn void xiul_exception_fatal(void)
{
    xiul_platform_console_write("XIUL: fatal CPU exception; system halted\n");
    xiul_platform_halt();
}
