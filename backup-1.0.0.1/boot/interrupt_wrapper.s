[BITS 32]
global _keyboard_handler_wrapper
extern _keyboard_handler_main

section .text
_keyboard_handler_wrapper:
    pusha
    cld
    call _keyboard_handler_main
    popa
    iretd
