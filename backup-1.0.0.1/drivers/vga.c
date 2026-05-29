#include <stdint.h>
#include "vga.h"
#include "common.h"

#define VGA_MEMORY 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static int cursor_x = 0;
static int cursor_y = 0;

void vga_putchar(char c) {
    uint16_t* vga_buffer = (uint16_t*)VGA_MEMORY;
    
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        uint16_t attribute = 0x0F00;  // white on black
        vga_buffer[cursor_y * VGA_WIDTH + cursor_x] = attribute | (uint16_t)c;
        cursor_x++;
    }
    
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }
    
    if (cursor_y >= VGA_HEIGHT) {
        cursor_y = VGA_HEIGHT - 1;
    }
}

void vga_print(const char* str) {
    while (*str) {
        vga_putchar(*str++);
    }
}

void vga_init(void) {
    cursor_x = 0;
    cursor_y = 0;
    vga_clear();
}

void vga_clear(void) {
    uint16_t* vga_buffer = (uint16_t*)VGA_MEMORY;
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = 0x0F00 | (uint16_t)' ';
    }
    cursor_x = 0;
    cursor_y = 0;
}
