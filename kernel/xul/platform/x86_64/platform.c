/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include "xul/platform.h"
#include "xul/pci_host.h"

#define COM1 0x3f8u
#define UART_DATA 0u
#define UART_INTERRUPT_ENABLE 1u
#define UART_FIFO_CONTROL 2u
#define UART_LINE_CONTROL 3u
#define UART_MODEM_CONTROL 4u
#define UART_LINE_STATUS 5u

extern xul_status_t xul_x86_64_exception_initialize(void);

static inline void outb(uint16_t port, uint8_t value)
{
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t value;
    __asm__ volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static unsigned int platform_initialized;

static void legacy_pic_and_pit_initialize(void)
{
    uint16_t divisor = (uint16_t)(1193182u / 100u);
    outb(0x20u, 0x11u); outb(0xa0u, 0x11u);
    outb(0x21u, 0x20u); outb(0xa1u, 0x28u);
    outb(0x21u, 0x04u); outb(0xa1u, 0x02u);
    outb(0x21u, 0x01u); outb(0xa1u, 0x01u);
    outb(0x21u, 0xfeu);
    outb(0xa1u, 0xffu);
    outb(0x43u, 0x36u);
    outb(0x40u, (uint8_t)(divisor & 0xffu));
    outb(0x40u, (uint8_t)(divisor >> 8));
}

static void serial_initialize(void)
{
    outb(COM1 + UART_INTERRUPT_ENABLE, 0x00);
    outb(COM1 + UART_LINE_CONTROL, 0x80);
    outb(COM1 + UART_DATA, 0x03);
    outb(COM1 + UART_INTERRUPT_ENABLE, 0x00);
    outb(COM1 + UART_LINE_CONTROL, 0x03);
    outb(COM1 + UART_FIFO_CONTROL, 0xc7);
    outb(COM1 + UART_MODEM_CONTROL, 0x0b);
}

xul_status_t xul_arch_early_init(const xul_boot_info_t *boot_info)
{
    xul_status_t status;
    (void)boot_info;
    serial_initialize();
    status = xul_x86_64_exception_initialize();
    if (status != XUL_OK) return status;
    legacy_pic_and_pit_initialize();
    xul_platform_console_write("XUL x86_64 platform initialized; IDT active\r\n");
    platform_initialized = 1;
    return XUL_OK;
}

xul_status_t xul_platform_get_pci_host(xul_pci_host_t *host)
{
    if (!platform_initialized) return XUL_E_STATE;
    return xul_pci_host_init_x86_64(host, 0, UINT8_MAX);
}

void xul_platform_console_putc(char character)
{
    unsigned int timeout = 1000000u;
    while ((inb(COM1 + UART_LINE_STATUS) & 0x20u) == 0 && timeout != 0) {
        --timeout;
    }
    if (timeout != 0) outb(COM1 + UART_DATA, (uint8_t)character);
}

void xul_platform_console_write(const char *text)
{
    if (text == 0) return;
    while (*text != '\0') {
        if (*text == '\n') xul_platform_console_putc('\r');
        xul_platform_console_putc(*text++);
    }
}

uint64_t xul_platform_cpu_id(void)
{
    uint32_t eax = 1, ebx, ecx, edx;
    __asm__ volatile ("cpuid"
        : "+a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx));
    (void)ecx;
    (void)edx;
    return (uint64_t)((ebx >> 24) & 0xffu);
}

xul_irq_state_t xul_platform_irq_save_disable(void)
{
    uintptr_t flags;
    __asm__ volatile ("pushfq; popq %0; cli" : "=r"(flags) : : "memory");
    return flags;
}

void xul_platform_irq_restore(xul_irq_state_t state)
{
    if ((state & (uintptr_t)0x200u) != 0) {
        __asm__ volatile ("sti" : : : "memory");
    } else {
        __asm__ volatile ("cli" : : : "memory");
    }
}

void xul_platform_irq_eoi(uint64_t vector)
{
    if (vector >= 40u && vector <= 47u) outb(0xa0u, 0x20u);
    if (vector >= 32u && vector <= 47u) outb(0x20u, 0x20u);
}

_Noreturn void xul_platform_idle(void)
{
    for (;;) __asm__ volatile ("sti; hlt" : : : "memory");
}

void xul_platform_cpu_relax(void)
{
    __asm__ volatile ("pause" : : : "memory");
}

_Noreturn void xul_platform_halt(void)
{
    for (;;) __asm__ volatile ("cli; hlt" : : : "memory");
}
