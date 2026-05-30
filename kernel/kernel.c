#include "vga.h"
#include "idt.h"
#include "pic.h"

void kernel_main(void) {
    vga_init();
    vga_clear();
    vga_print("Welcome to MyOS!\n");
    vga_print("Initializing system...\n");

    remap_pic();
    init_idt();
    enable_keyboard();

    vga_print("Keyboard ready. Start typing!\n");
    vga_print("> ");

    asm volatile("sti");

    while(1) {
        asm volatile("hlt");
    }
}
