#include "pic.h"
#include "common.h"

#define PIC1_CMD  0x20
#define PIC1_DATA 0x21
#define PIC2_CMD  0xA0
#define PIC2_DATA 0xA1

void remap_pic(void) {
    // ICW1: start init
    outb(PIC1_CMD,  0x11);
    outb(PIC2_CMD,  0x11);
    // ICW2: vector offset
    outb(PIC1_DATA, 0x20);  // IRQ0-7  -> INT 0x20-0x27
    outb(PIC2_DATA, 0x28);  // IRQ8-15 -> INT 0x28-0x2F
    // ICW3: cascade
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);
    // ICW4: 8086 mode
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);
    // mask all except IRQ1 (keyboard)
    outb(PIC1_DATA, 0xFD);
    outb(PIC2_DATA, 0xFF);
}

void enable_keyboard(void) {
    uint8_t mask = inb(PIC1_DATA);
    mask &= ~(1 << 1);  // clear bit 1 = unmask IRQ1
    outb(PIC1_DATA, mask);
}
