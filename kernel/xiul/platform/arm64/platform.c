#include "xiul/platform.h"

#define PL011_DR 0x00u
#define PL011_FR 0x18u
#define PL011_IBRD 0x24u
#define PL011_FBRD 0x28u
#define PL011_LCRH 0x2cu
#define PL011_CR 0x30u
#define PL011_ICR 0x44u
#define PL011_FR_TXFF (1u << 5)

static volatile uint32_t *uart_registers;

static void serial_initialize(const xiul_boot_info_t *boot_info)
{
    uint32_t divisor, remainder;
    if (boot_info->uart_virtual == 0 ||
        boot_info->uart_clock_hz == 0 ||
        boot_info->uart_baud_rate == 0) return;

    uart_registers = (volatile uint32_t *)(uintptr_t)boot_info->uart_virtual;
    uart_registers[PL011_CR / 4u] = 0;
    uart_registers[PL011_ICR / 4u] = 0x7ffu;
    divisor = boot_info->uart_clock_hz / (16u * boot_info->uart_baud_rate);
    remainder = boot_info->uart_clock_hz % (16u * boot_info->uart_baud_rate);
    uart_registers[PL011_IBRD / 4u] = divisor;
    uart_registers[PL011_FBRD / 4u] =
        (remainder * 64u + (8u * boot_info->uart_baud_rate)) /
        (16u * boot_info->uart_baud_rate);
    uart_registers[PL011_LCRH / 4u] = (3u << 5) | (1u << 4);
    uart_registers[PL011_CR / 4u] = (1u << 9) | (1u << 8) | 1u;
}

xiul_status_t xiul_arch_early_init(const xiul_boot_info_t *boot_info)
{
    serial_initialize(boot_info);
    xiul_platform_console_write("XIUL AArch64 platform initialized\r\n");
    return XIUL_OK;
}

void xiul_platform_console_putc(char character)
{
    if (uart_registers == 0) return;
    while ((uart_registers[PL011_FR / 4u] & PL011_FR_TXFF) != 0) {
        __asm__ volatile ("yield" : : : "memory");
    }
    uart_registers[PL011_DR / 4u] = (uint32_t)(uint8_t)character;
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
    uint64_t mpidr;
    __asm__ volatile ("mrs %0, mpidr_el1" : "=r"(mpidr));
    return mpidr & UINT64_C(0xffffff);
}

xiul_irq_state_t xiul_platform_irq_save_disable(void)
{
    uint64_t state;
    __asm__ volatile ("mrs %0, daif; msr daifset, #2"
                      : "=r"(state) : : "memory");
    return (xiul_irq_state_t)state;
}

void xiul_platform_irq_restore(xiul_irq_state_t state)
{
    uint64_t value = (uint64_t)state;
    __asm__ volatile ("msr daif, %0" : : "r"(value) : "memory");
}

void xiul_platform_cpu_relax(void)
{
    __asm__ volatile ("yield" : : : "memory");
}

_Noreturn void xiul_platform_halt(void)
{
    for (;;) __asm__ volatile ("msr daifset, #0xf; wfe" : : : "memory");
}
