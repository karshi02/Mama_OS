#include "idt.h"
#include "common.h"
#include <stdint.h>

#define IDT_SIZE 256

struct IDTEntry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  type_attr;
    uint16_t base_high;
} __attribute__((packed));

struct IDTPtr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

static struct IDTEntry IDT[IDT_SIZE];
static struct IDTPtr   idtr;

extern void keyboard_handler_wrapper(void);

static void set_gate(int n, uint32_t handler, uint16_t sel, uint8_t flags) {
    IDT[n].base_low  = handler & 0xFFFF;
    IDT[n].selector  = sel;
    IDT[n].zero      = 0;
    IDT[n].type_attr = flags;
    IDT[n].base_high = (handler >> 16) & 0xFFFF;
}

void init_idt(void) {
    idtr.limit = sizeof(IDT) - 1;
    idtr.base  = (uint32_t)&IDT;

    // IRQ1 (keyboard) -> INT 0x21
    set_gate(0x21, (uint32_t)keyboard_handler_wrapper, 0x08, 0x8E);

    asm volatile("lidt %0" : : "m"(idtr));
}
