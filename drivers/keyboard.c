#include "keyboard.h"
#include "vga.h"
#include "../include/common.h"

// Simple keyboard buffer (circular)
#define KEYBOARD_BUFFER_SIZE 32

static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static int buffer_head = 0;
static int buffer_tail = 0;
static int buffer_count = 0;

// US Keyboard layout mapping (scancode to ASCII)
// Only mapping basic keys for now
static const char scancode_to_ascii[] = {
    0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,   0,   // 0x00-0x0F
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0,   0,   'a', 's', // 0x10-0x1F
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,   '\\', 'z', 'x', 'c', 'v', // 0x20-0x2F
    'b', 'n', 'm', ',', '.', '/', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,    // 0x30-0x3F
    0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1', // 0x40-0x4F
    '2', '3', '0', '.'
};

// Shift version (for uppercase)
static const char scancode_to_ascii_shift[] = {
    0,   0,   '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0,   0,
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0,   0,   'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0,   '|', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', '<', '>', '?', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0
};

static int shift_pressed = 0;
static int caps_lock = 0;

// Keyboard port
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

void keyboard_init(void) {
    buffer_head = 0;
    buffer_tail = 0;
    buffer_count = 0;
    shift_pressed = 0;
    caps_lock = 0;
}

int keyboard_key_pressed(void) {
    return buffer_count > 0;
}

static void keyboard_push_char(char c) {
    if (buffer_count < KEYBOARD_BUFFER_SIZE) {
        keyboard_buffer[buffer_head] = c;
        buffer_head = (buffer_head + 1) % KEYBOARD_BUFFER_SIZE;
        buffer_count++;
    }
}

char keyboard_get_char(void) {
    if (buffer_count == 0) {
        return 0;
    }
    char c = keyboard_buffer[buffer_tail];
    buffer_tail = (buffer_tail + 1) % KEYBOARD_BUFFER_SIZE;
    buffer_count--;
    return c;
}

unsigned char keyboard_get_scancode(void) {
    return inb(KEYBOARD_DATA_PORT);
}

void keyboard_handler(void) {
    unsigned char scancode = keyboard_get_scancode();
    int is_released = (scancode & 0x80) != 0;
    scancode = scancode & 0x7F;  // Mask release bit
    
    // Handle modifier keys
    if (scancode == 0x2A || scancode == 0x36) {  // Left/Right Shift
        shift_pressed = !is_released;
        return;
    }
    
    if (scancode == 0x3A) {  // Caps Lock
        if (!is_released) {
            caps_lock = !caps_lock;
        }
        return;
    }
    
    // Handle Enter and Backspace
    if (scancode == KEY_ENTER && !is_released) {
        keyboard_push_char('\n');
        return;
    }
    
    if (scancode == KEY_BACKSPACE && !is_released) {
        keyboard_push_char('\b');
        return;
    }
    
    // Ignore key releases for normal keys
    if (is_released) {
        return;
    }
    
    // Convert scancode to ASCII
    char ascii = 0;
    if (scancode < sizeof(scancode_to_ascii)) {
        if (shift_pressed) {
            ascii = scancode_to_ascii_shift[scancode];
        } else if (caps_lock && scancode_to_ascii[scancode] >= 'a' && scancode_to_ascii[scancode] <= 'z') {
            ascii = scancode_to_ascii_shift[scancode];
        } else {
            ascii = scancode_to_ascii[scancode];
        }
    }
    
    if (ascii != 0) {
        keyboard_push_char(ascii);
    }
}
