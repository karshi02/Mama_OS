[BITS 32]
[GLOBAL _keyboard_handler_wrapper]

extern _keyboard_handler_main

_keyboard_handler_wrapper:
    pusha
    call _keyboard_handler_main
    popa
    iret
