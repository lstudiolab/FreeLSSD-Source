/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include "xul/platform.h"
#include "xul/pci_host.h"

#define PL011_DR 0x00u
#define PL011_FR 0x18u
#define PL011_IBRD 0x24u
#define PL011_FBRD 0x28u
#define PL011_LCRH 0x2cu
#define PL011_CR 0x30u
#define PL011_ICR 0x44u
#define PL011_FR_TXFF (1u << 5)
extern int xul_arm64_exception_initialize(void);

static volatile uint32_t *uart_registers;
static xul_arm64_pci_ecam_context_t pci_ecam_context;
static uint64_t pci_ecam_virtual;
static uint16_t pci_segment;
static uint8_t pci_first_bus, pci_last_bus;
static unsigned int platform_initialized;

static void serial_initialize(const xul_boot_info_t *boot_info)
{
    uint64_t denominator;
    uint32_t divisor, remainder;
    if (boot_info->uart_virtual == 0 ||
        boot_info->uart_clock_hz == 0 ||
        boot_info->uart_baud_rate == 0) return;

    uart_registers = (volatile uint32_t *)(uintptr_t)boot_info->uart_virtual;
    uart_registers[PL011_CR / 4u] = 0;
    uart_registers[PL011_ICR / 4u] = 0x7ffu;
    denominator = UINT64_C(16) * boot_info->uart_baud_rate;
    divisor = (uint32_t)(boot_info->uart_clock_hz / denominator);
    remainder = (uint32_t)(boot_info->uart_clock_hz % denominator);
    if (divisor == 0) { uart_registers = 0; return; }
    uart_registers[PL011_IBRD / 4u] = divisor;
    uart_registers[PL011_FBRD / 4u] =
        (uint32_t)(((uint64_t)remainder * 64u + denominator / 2u) / denominator);
    uart_registers[PL011_LCRH / 4u] = (3u << 5) | (1u << 4);
    uart_registers[PL011_CR / 4u] = (1u << 9) | (1u << 8) | 1u;
}

xul_status_t xul_arch_early_init(const xul_boot_info_t *boot_info)
{
    serial_initialize(boot_info);
    if (xul_arm64_exception_initialize() != 0) return XUL_E_STATE;
    pci_ecam_virtual = boot_info->pci_ecam_virtual;
    pci_segment = boot_info->pci_segment;
    pci_first_bus = boot_info->pci_first_bus;
    pci_last_bus = boot_info->pci_last_bus;
    platform_initialized = 1;
    xul_platform_console_write("XUL AArch64 platform initialized\r\n");
    return XUL_OK;
}

xul_status_t xul_platform_get_pci_host(xul_pci_host_t *host)
{
    if (!platform_initialized) return XUL_E_STATE;
    if (pci_ecam_virtual == 0 || pci_first_bus > pci_last_bus)
        return XUL_E_UNSUPPORTED;
    return xul_pci_host_init_arm64_ecam(host, &pci_ecam_context,
        (volatile void *)(uintptr_t)pci_ecam_virtual, pci_segment,
        pci_first_bus, pci_last_bus);
}

void xul_platform_console_putc(char character)
{
    if (uart_registers == 0) return;
    {
        unsigned int timeout = 1000000u;
        while ((uart_registers[PL011_FR / 4u] & PL011_FR_TXFF) != 0 && timeout != 0) {
            --timeout;
            __asm__ volatile ("yield" : : : "memory");
        }
        if (timeout != 0) uart_registers[PL011_DR / 4u] = (uint32_t)(uint8_t)character;
    }
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
    uint64_t mpidr;
    __asm__ volatile ("mrs %0, mpidr_el1" : "=r"(mpidr));
    return mpidr & UINT64_C(0xffffff);
}

xul_irq_state_t xul_platform_irq_save_disable(void)
{
    uint64_t state;
    __asm__ volatile ("mrs %0, daif; msr daifset, #2"
                      : "=r"(state) : : "memory");
    return (xul_irq_state_t)state;
}

void xul_platform_irq_restore(xul_irq_state_t state)
{
    uint64_t value = (uint64_t)state;
    __asm__ volatile ("msr daif, %0" : : "r"(value) : "memory");
}

void xul_platform_cpu_relax(void)
{
    __asm__ volatile ("yield" : : : "memory");
}

_Noreturn void xul_platform_halt(void)
{
    for (;;) __asm__ volatile ("msr daifset, #0xf; wfe" : : : "memory");
}
