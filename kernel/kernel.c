#include "vga.h"
#include "idt.h"
#include "pic.h"
#include "shell.h"

void kernel_main(void) {
    vga_init();
    vga_clear();
    vga_print("Welcome to MyOS!\n");

    remap_pic();
    init_idt();
    enable_keyboard();

    shell_init();

    asm volatile("sti");

    while(1) {
        asm volatile("hlt");
    }
}
