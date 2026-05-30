global keyboard_handler_wrapper
extern keyboard_handler_main

section .text
keyboard_handler_wrapper:
    pusha
    push ds
    push es
    push fs
    push gs
    
    call keyboard_handler_main
    
    pop gs
    pop fs
    pop es
    pop ds
    popa
    iret
