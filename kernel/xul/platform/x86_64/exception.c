/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include <stddef.h>
#include <stdint.h>
#include "xul/platform.h"

#define XUL_IDT_ENTRIES 256u
#define XUL_IDT_INTERRUPT_GATE 0x8eu

typedef struct {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t attributes;
    uint16_t offset_middle;
    uint32_t offset_high;
    uint32_t reserved;
} xul_idt_gate_t;

typedef struct {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) xul_idtr_t;

extern void (*xul_x86_64_isr_stub_table[XUL_IDT_ENTRIES])(void);

static xul_idt_gate_t idt[XUL_IDT_ENTRIES];
static unsigned int idt_initialized;

static uint16_t current_code_selector(void)
{
    uint16_t selector;
    __asm__ volatile ("mov %%cs, %0" : "=r"(selector));
    return selector;
}

static void set_gate(size_t vector, uintptr_t address, uint16_t selector)
{
    xul_idt_gate_t *gate = &idt[vector];
    gate->offset_low = (uint16_t)address;
    gate->selector = selector;
    gate->ist = 0;
    gate->attributes = XUL_IDT_INTERRUPT_GATE;
    gate->offset_middle = (uint16_t)(address >> 16);
    gate->offset_high = (uint32_t)(address >> 32);
    gate->reserved = 0;
}

xul_status_t xul_x86_64_exception_initialize(void)
{
    xul_idtr_t descriptor;
    uint16_t selector;
    size_t index;

    if (idt_initialized) return XUL_E_STATE;
    selector = current_code_selector();
    if (selector == 0) return XUL_E_STATE;

    for (index = 0; index < XUL_IDT_ENTRIES; ++index) {
        set_gate(index, (uintptr_t)xul_x86_64_isr_stub_table[index],
                 selector);
    }

    descriptor.limit = (uint16_t)(sizeof(idt) - 1u);
    descriptor.base = (uint64_t)(uintptr_t)idt;
    __asm__ volatile ("lidt %0" : : "m"(descriptor) : "memory");
    idt_initialized = 1;
    return XUL_OK;
}
