#ifndef VGA_H
#define VGA_H

#include "../include/common.h"

// Initialize VGA text mode
void vga_init(void);

// Clear entire screen
void vga_clear(void);

// Print single character at current cursor position
void vga_putchar(char c);

// Print string at current cursor position
void vga_print(const char* str);

// Print string with specified color
void vga_print_color(const char* str, vga_color_t color);

// Set cursor position (row, col)
void vga_set_cursor(int row, int col);

// Get current row
int vga_get_row(void);

// Get current column
int vga_get_col(void);

#endif
