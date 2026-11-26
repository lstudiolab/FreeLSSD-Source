/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#ifndef XUL_OWNERSHIP_H
#define XUL_OWNERSHIP_H

/*
 * Global kernel facilities have exactly one active owner per boot:
 *
 * - scheduler and thread lifecycle
 * - physical and virtual memory
 * - interrupt controller and interrupt dispatch
 * - clock/timer source
 * - device discovery and device lifecycle
 *
 * Imported Linux and FreeBSD components must call XUL adapters. They must
 * not initialize a competing global facility or bypass the XUL owner.
 *
 * This header intentionally defines policy only. Concrete interfaces belong
 * in separate headers when their contracts and target semantics are ready.
 */

#define XUL_OWNER_MACH_CORE       1
#define XUL_OWNER_XUL_PLATFORM   1
#define XUL_IMPORT_LINUX_DRIVERS  1
#define XUL_IMPORT_BSD_SERVICES   1

#endif
