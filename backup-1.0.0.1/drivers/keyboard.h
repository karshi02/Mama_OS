#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../include/common.h"

// Initialize keyboard driver
void keyboard_init(void);

// Check if a key has been pressed
int keyboard_key_pressed(void);

// Get the ASCII character of pressed key
char keyboard_get_char(void);

// Get raw scancode
unsigned char keyboard_get_scancode(void);

// Keyboard interrupt handler (to be called from IRQ1)
void keyboard_handler(void);

#endif
