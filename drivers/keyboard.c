#include "keyboard.h"
#include "common.h"
#include "shell.h"

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
    outb(0x20, 0x20);

    if (scancode & 0x80) return;

    char c = 0;
    if (scancode == 0x1C) c = '\n';
    else if (scancode == 0x0E) c = '\b';
    else if (scancode < sizeof(scancode_table))
        c = scancode_table[scancode];

    if (c) shell_handle_char(c);
}
