#include "vga.h"

void kernel_main(void) {
    vga_init();
    vga_clear();
    vga_print("Welcome to MyOS!\n");
    vga_print("Kernel loaded successfully at 0x10000\n");
    vga_print("System is running!\n");
    
    // Hang
    while(1) {
        asm volatile("hlt");
    }
}
