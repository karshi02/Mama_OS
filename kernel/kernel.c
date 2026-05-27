#include <stdint.h>
#include "vga.h"
#include "keyboard.h"
#include "pic.h"
#include "idt.h"

void kernel_main(void) {
    vga_init();
    vga_print("Welcome to MyOS\n");
    vga_print("Kernel initialized successfully.\n");
    vga_print("Type something: ");
    
    remap_pic();
    init_idt();
    enable_keyboard();
    
    asm volatile("sti");
    
    while(1) {
        // idle
    }
}
