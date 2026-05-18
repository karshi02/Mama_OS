// Kernel main entry point
// After bootloader switches to protected mode, control jumps here

#include "../drivers/vga.h"
#include "../drivers/keyboard.h"
#include "../include/common.h"

// Simple command handler
static void process_command(char c) {
    if (c == '\n') {
        vga_print("\n> ");
    } else if (c == '\b') {
        // Backspace: move cursor back and clear character
        int row = vga_get_row();
        int col = vga_get_col();
        if (col > 0) {
            vga_set_cursor(row, col - 1);
            vga_putchar(' ');
            vga_set_cursor(row, col - 1);
        }
    } else {
        vga_putchar(c);
    }
}

// Simple shell loop
static void shell_loop(void) {
    vga_print("\n> ");
    
    while (1) {
        if (keyboard_key_pressed()) {
            char c = keyboard_get_char();
            process_command(c);
        }
        
        // Small delay to prevent 100% CPU usage
        for (volatile int i = 0; i < 100000; i++);
    }
}

void kernel_main(void) {
    // Initialize drivers
    vga_init();
    keyboard_init();
    
    // Print welcome message
    vga_print_color("========================================\n", COLOR_GREEN);
    vga_print_color("     Welcome to MyOS v0.1\n", COLOR_YELLOW);
    vga_print_color("========================================\n", COLOR_GREEN);
    vga_print("Kernel initialized successfully.\n\n");
    
    vga_print_color("[INFO] ", COLOR_CYAN);
    vga_print("VGA Driver: Active (80x25 text mode)\n");
    
    vga_print_color("[INFO] ", COLOR_CYAN);
    vga_print("Keyboard Driver: Active\n\n");
    
    vga_print_color("[STATUS] ", COLOR_LIGHT_GREEN);
    vga_print("System ready.\n");
    
    vga_print("========================================\n");
    vga_print("Type anything to test keyboard input.\n");
    vga_print("========================================\n");
    
    // Start interactive shell
    shell_loop();
    
    // Should never reach here
    while (1) {
        __asm__ volatile ("hlt");
    }
}
