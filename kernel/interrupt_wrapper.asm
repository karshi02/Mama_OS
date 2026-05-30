[BITS 32]

global keyboard_handler_wrapper
extern keyboard_handler_main

section .text

keyboard_handler_wrapper:
    pusha
    call keyboard_handler_main
    popa
    iret
