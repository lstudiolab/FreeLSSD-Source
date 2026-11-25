/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include <stddef.h>
#include <stdint.h>
#include "xiul/platform.h"

#define XIUL_IDT_ENTRIES 256u
#define XIUL_IDT_INTERRUPT_GATE 0x8eu

typedef struct {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t attributes;
    uint16_t offset_middle;
    uint32_t offset_high;
    uint32_t reserved;
} xiul_idt_gate_t;

typedef struct {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) xiul_idtr_t;

extern void (*xiul_x86_64_isr_stub_table[XIUL_IDT_ENTRIES])(void);

static xiul_idt_gate_t idt[XIUL_IDT_ENTRIES];
static unsigned int idt_initialized;

static uint16_t current_code_selector(void)
{
    uint16_t selector;
    __asm__ volatile ("mov %%cs, %0" : "=r"(selector));
    return selector;
}

static void set_gate(size_t vector, uintptr_t address, uint16_t selector)
{
    xiul_idt_gate_t *gate = &idt[vector];
    gate->offset_low = (uint16_t)address;
    gate->selector = selector;
    gate->ist = 0;
    gate->attributes = XIUL_IDT_INTERRUPT_GATE;
    gate->offset_middle = (uint16_t)(address >> 16);
    gate->offset_high = (uint32_t)(address >> 32);
    gate->reserved = 0;
}

xiul_status_t xiul_x86_64_exception_initialize(void)
{
    xiul_idtr_t descriptor;
    uint16_t selector;
    size_t index;

    if (idt_initialized) return XIUL_E_STATE;
    selector = current_code_selector();
    if (selector == 0) return XIUL_E_STATE;

    for (index = 0; index < XIUL_IDT_ENTRIES; ++index) {
        set_gate(index, (uintptr_t)xiul_x86_64_isr_stub_table[index],
                 selector);
    }

    descriptor.limit = (uint16_t)(sizeof(idt) - 1u);
    descriptor.base = (uint64_t)(uintptr_t)idt;
    __asm__ volatile ("lidt %0" : : "m"(descriptor) : "memory");
    idt_initialized = 1;
    return XIUL_OK;
}
