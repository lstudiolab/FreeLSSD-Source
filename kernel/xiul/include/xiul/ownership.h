#ifndef XIUL_OWNERSHIP_H
#define XIUL_OWNERSHIP_H

/*
 * Global kernel facilities have exactly one active owner per boot:
 *
 * - scheduler and thread lifecycle
 * - physical and virtual memory
 * - interrupt controller and interrupt dispatch
 * - clock/timer source
 * - device discovery and device lifecycle
 *
 * Imported Linux and FreeBSD components must call XIUL adapters. They must
 * not initialize a competing global facility or bypass the XIUL owner.
 *
 * This header intentionally defines policy only. Concrete interfaces belong
 * in separate headers when their contracts and target semantics are ready.
 */

#define XIUL_OWNER_MACH_CORE       1
#define XIUL_OWNER_XIUL_PLATFORM   1
#define XIUL_IMPORT_LINUX_DRIVERS  1
#define XIUL_IMPORT_BSD_SERVICES   1

#endif
