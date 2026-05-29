#include <stdint.h>
#include "keyboard.h"
#include "vga.h"
#include "common.h"

#define KEYBOARD_DATA_PORT 0x60

static const char scancode_to_ascii[] = {
    0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
    0,   0,   'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
    0,   0,   'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0,   '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*',
    0,   ' ',   0,   0,   0,   0,   0,   0,   0,   0,   0
};

void keyboard_handler_main(void) {
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);
    outb(0x20, 0x20);

    if (scancode < 0x80 && scancode < sizeof(scancode_to_ascii)) {
        char c = scancode_to_ascii[scancode];
        if (c != 0) {
            vga_putchar(c);
        }
    }
}

void keyboard_init(void) {}
int keyboard_key_pressed(void) { return 0; }
char keyboard_get_char(void) { return 0; }
unsigned char keyboard_get_scancode(void) { return inb(KEYBOARD_DATA_PORT); }
void keyboard_handler(void) { keyboard_handler_main(); }
