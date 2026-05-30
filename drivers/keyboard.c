#include "keyboard.h"
#include "vga.h"
#include "common.h"

#define KEYBOARD_PORT 0x60

static const char scancode_table[] = {
    0,    0,   '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=',  0,   0,   'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i', 'o', 'p', '[', ']',  0,   0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', 0,  '\\','z', 'x', 'c', 'v', 'b', 'n',
    'm', ',', '.', '/',  0,  '*',  0,  ' '
};

void init_keyboard(void) {}

void keyboard_handler_main(void) {
    uint8_t scancode = inb(KEYBOARD_PORT);

    // send EOI to PIC
    outb(0x20, 0x20);

    // ignore key release (bit 7 set)
    if (scancode & 0x80) return;

    if (scancode == 0x1C) {
        vga_putchar('\n');
        return;
    }
    if (scancode == 0x0E) {
        vga_putchar('\b');
        return;
    }

    if (scancode < sizeof(scancode_table)) {
        char c = scancode_table[scancode];
        if (c) vga_putchar(c);
    }
}
