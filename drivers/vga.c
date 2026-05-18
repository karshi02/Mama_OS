#include "vga.h"
#include "../include/common.h"

static unsigned short* vga_buffer = (unsigned short*)VGA_ADDRESS;
static int cursor_row = 0;
static int cursor_col = 0;
static vga_color_t current_color = COLOR_LIGHT_GREY;

// Create VGA character entry
static inline unsigned short make_vga_entry(char c, vga_color_t color) {
    return (unsigned short)c | ((unsigned short)color << 8);
}

// Update hardware cursor
static void update_hardware_cursor(void) {
    int position = cursor_row * VGA_WIDTH + cursor_col;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(position & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((position >> 8) & 0xFF));
}

// Scroll screen up by one line
static void scroll(void) {
    for (int row = 1; row < VGA_HEIGHT; row++) {
        for (int col = 0; col < VGA_WIDTH; col++) {
            int src = row * VGA_WIDTH + col;
            int dst = (row - 1) * VGA_WIDTH + col;
            vga_buffer[dst] = vga_buffer[src];
        }
    }
    
    // Clear last line
    for (int col = 0; col < VGA_WIDTH; col++) {
        int index = (VGA_HEIGHT - 1) * VGA_WIDTH + col;
        vga_buffer[index] = make_vga_entry(' ', COLOR_BLACK);
    }
    
    cursor_row = VGA_HEIGHT - 1;
}

void vga_init(void) {
    vga_clear();
    current_color = COLOR_LIGHT_GREY;
}

void vga_clear(void) {
    for (int row = 0; row < VGA_HEIGHT; row++) {
        for (int col = 0; col < VGA_WIDTH; col++) {
            int index = row * VGA_WIDTH + col;
            vga_buffer[index] = make_vga_entry(' ', COLOR_BLACK);
        }
    }
    cursor_row = 0;
    cursor_col = 0;
    update_hardware_cursor();
}

void vga_putchar(char c) {
    if (c == '\n') {
        cursor_row++;
        cursor_col = 0;
    } else if (c == '\t') {
        // Tab = 4 spaces
        for (int i = 0; i < 4; i++) {
            vga_putchar(' ');
        }
        return;
    } else {
        int index = cursor_row * VGA_WIDTH + cursor_col;
        vga_buffer[index] = make_vga_entry(c, current_color);
        cursor_col++;
        
        if (cursor_col >= VGA_WIDTH) {
            cursor_col = 0;
            cursor_row++;
        }
    }
    
    if (cursor_row >= VGA_HEIGHT) {
        scroll();
    }
    
    update_hardware_cursor();
}

void vga_print(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        vga_putchar(str[i]);
    }
}

void vga_print_color(const char* str, vga_color_t color) {
    vga_color_t old_color = current_color;
    current_color = color;
    vga_print(str);
    current_color = old_color;
}

void vga_set_cursor(int row, int col) {
    if (row >= 0 && row < VGA_HEIGHT && col >= 0 && col < VGA_WIDTH) {
        cursor_row = row;
        cursor_col = col;
        update_hardware_cursor();
    }
}

int vga_get_row(void) {
    return cursor_row;
}

int vga_get_col(void) {
    return cursor_col;
}

// I/O functions
void outb(unsigned short port, unsigned char data) {
    __asm__ volatile ("outb %0, %1" : : "a"(data), "Nd"(port));
}

unsigned char inb(unsigned short port) {
    unsigned char data;
    __asm__ volatile ("inb %1, %0" : "=a"(data) : "Nd"(port));
    return data;
}
