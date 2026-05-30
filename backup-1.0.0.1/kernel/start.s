section .text
global _start
extern kernel_main

_start:
    ; Set up stack
    mov esp, 0x90000
    
    ; Call kernel main
    call kernel_main
    
    ; Hang if returns
    cli
hang:
    hlt
    jmp hang
