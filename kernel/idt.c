#include <stdint.h>
#include "idt.h"
#include "common.h"

#define IDT_SIZE 256

static struct IDTEntry IDT[IDT_SIZE];
static struct IDTPtr idtr;

extern void _keyboard_handler_wrapper(void);

void set_idt_gate(int vector, uint32_t handler, uint16_t selector, uint8_t flags) {
    IDT[vector].base_low = handler & 0xFFFF;
    IDT[vector].selector = selector;
    IDT[vector].zero = 0;
    IDT[vector].type_attr = flags;
    IDT[vector].base_high = (handler >> 16) & 0xFFFF;
}

void init_idt(void) {
    idtr.base = (uint32_t)&IDT;
    idtr.limit = sizeof(IDT) - 1;
    
    asm volatile("lidt %0" : : "m"(idtr));
    set_idt_gate(0x21, (uint32_t)_keyboard_handler_wrapper, 0x08, 0x8E);
}
