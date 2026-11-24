#include "xiul/platform.h"
#include "xiul/pci_host.h"

#define COM1 0x3f8u
#define UART_DATA 0u
#define UART_INTERRUPT_ENABLE 1u
#define UART_FIFO_CONTROL 2u
#define UART_LINE_CONTROL 3u
#define UART_MODEM_CONTROL 4u
#define UART_LINE_STATUS 5u

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

xiul_status_t xiul_arch_early_init(const xiul_boot_info_t *boot_info)
{
    (void)boot_info;
    serial_initialize();
    xiul_platform_console_write("XIUL x86_64 platform initialized\r\n");
    platform_initialized = 1;
    return XIUL_OK;
}

xiul_status_t xiul_platform_get_pci_host(xiul_pci_host_t *host)
{
    if (!platform_initialized) return XIUL_E_STATE;
    return xiul_pci_host_init_x86_64(host, 0, UINT8_MAX);
}

void xiul_platform_console_putc(char character)
{
    unsigned int timeout = 1000000u;
    while ((inb(COM1 + UART_LINE_STATUS) & 0x20u) == 0 && timeout != 0) {
        --timeout;
    }
    if (timeout != 0) outb(COM1 + UART_DATA, (uint8_t)character);
}

void xiul_platform_console_write(const char *text)
{
    if (text == 0) return;
    while (*text != '\0') {
        if (*text == '\n') xiul_platform_console_putc('\r');
        xiul_platform_console_putc(*text++);
    }
}

uint64_t xiul_platform_cpu_id(void)
{
    uint32_t eax = 1, ebx, ecx, edx;
    __asm__ volatile ("cpuid"
        : "+a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx));
    (void)ecx;
    (void)edx;
    return (uint64_t)((ebx >> 24) & 0xffu);
}

xiul_irq_state_t xiul_platform_irq_save_disable(void)
{
    uintptr_t flags;
    __asm__ volatile ("pushfq; popq %0; cli" : "=r"(flags) : : "memory");
    return flags;
}

void xiul_platform_irq_restore(xiul_irq_state_t state)
{
    if ((state & (uintptr_t)0x200u) != 0) {
        __asm__ volatile ("sti" : : : "memory");
    }
}

void xiul_platform_cpu_relax(void)
{
    __asm__ volatile ("pause" : : : "memory");
}

_Noreturn void xiul_platform_halt(void)
{
    for (;;) __asm__ volatile ("cli; hlt" : : : "memory");
}
