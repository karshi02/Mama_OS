#include "idt.h"
#include "common.h"

#define IDT_SIZE 256

static struct IDTEntry IDT[IDT_SIZE];
static struct IDTPtr idtr;

extern uint32_t keyboard_handler_wrapper;  // declared in assembly

void set_idt_gate(int vector, uint32_t handler, uint16_t selector, uint8_t flags) {
    IDT[vector].base_low = handler & 0xFFFF;
    IDT[vector].selector = selector;
    IDT[vector].zero = 0;
    IDT[vector].type_attr = flags;
    IDT[vector].base_high = (handler >> 16) & 0xFFFF;
}

void init_idt() {
    idtr.base = (uint32_t)&IDT;
    idtr.limit = sizeof(IDT) - 1;
    
    // load IDT
    asm volatile("lidt %0" : : "m"(idtr));
    
    // set keyboard interrupt (IRQ1 = interrupt 33 = 0x21)
    set_idt_gate(0x21, (uint32_t)&keyboard_handler_wrapper, 0x08, 0x8E);
}
