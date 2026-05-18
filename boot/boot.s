; Bootloader for MyOS
; Runs in 16-bit real mode, loads kernel, switches to 32-bit protected mode

section .text
use16
global _start

_start:
    ; Set up segment registers
    
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00          ; Stack grows down from bootloader    
    ; Save boot drive number
    mov [boot_drive], dl
    
    ; Print loading message
    mov si, msg_loading
    call print_string
    
    ; Load kernel from disk
    call load_kernel
    
    ; Switch to protected mode
    call switch_to_pm
    
    ; Should never reach here
    jmp $

; Function: Print string in real mode (SI = string pointer)
print_string:       
    mov ah, 0x0E            ; BIOS teletype function
.loop:
    lodsb
    test al, al
    jz .done
    int 0x10
    jmp .loop
.done:
    ret

; Function: Load kernel from disk
load_kernel:
    mov si, msg_loading_kernel
    call print_string
    
    ; Reset disk system
    mov ah, 0x00
    mov dl, [boot_drive]
    int 0x13
    jc .disk_error
    
    ; Read kernel sectors
    ; Kernel is loaded at 0x1000:0x0000 (physical 0x10000 for now)
    ; But we'll use linear addressing later
    mov ax, 0x1000          ; Segment
    mov es, ax
    xor bx, bx              ; Offset 0
 
    mov ah, 0x02            ; BIOS read sectors function
    mov al, 50              ; Number of sectors to read (足够大)
    mov ch, 0               ; Cylinder 0
    mov cl, 2               ; Start from sector 2 (after bootloader)
    mov dh, 0               ; Head 0
    mov dl, [boot_drive]    ; Drive
    int 0x13
    jc .disk_error
    
    mov si, msg_done
    call print_string
    ret

.disk_error:
    mov si, msg_disk_error
    call print_string
    jmp $
; Function: Switch to 32-bit protected mode
switch_to_pm: 
    cli                     ; Disable interrupts
    
    ; Load GDT
    lgdt [gdt_descriptor]
    
    ; Enable protected mode bit in CR0
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    
    ; Far jump to 32-bit code segment
    jmp CODE_SEG:init_pm

use32
init_pm:
    ; Set up segment registers for protected mode
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000        ; Set up stack 
    ; Jump to kernel entry point (0x1000:0x0000 = 0x10000 linear)
    jmp 0x10000
    jmp $

; Global Descriptor Table (GDT)
gdt_start:
    ; Null descriptor
    dd 0x0
    dd 0x0

; Code segment descriptor
gdt_code:
    ; Base=0, Limit=0xFFFFF, 32-bit, 4KB granularity, Code segment
    dw 0xFFFF               ; Limit (0-15)
    dw 0x0000               ; Base (0-15)
    db 0x00                 ; Base (16-23)
    db 0x9A                 ; Access: Present, Ring0, Executable, Readable
    db 0xCF                 ; Flags: 4KB, 32-bit, Limit (16-19)
    db 0x00                 ; Base (24-31)

; Data segment descriptor
gdt_data:
    ; Base=0, Limit=0xFFFFF, 32-bit, 4KB granularity, Data segment
    dw 0xFFFF               ; Limit (0-15)
    dw 0x0000               ; Base (0-15)
    db 0x00                 ; Base (16-23)
    db 0x92                 ; Access: Present, Ring0, Data, Writable
    db 0xCF                 ; Flags: 4KB, 32-bit, Limit (16-19)
    db 0x00                 ; Base (24-31)

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; Size
    dd gdt_start                  ; Address

; Constants for segment selectors
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

; Data variables
boot_drive: db 0

; Messages
msg_loading: db 13, 10, "MyOS Bootloader", 13, 10, 0
msg_loading_kernel: db "Loading kernel... ", 0
msg_done: db "Done", 13, 10, 0
msg_disk_error: db "Disk error!", 13, 10, 0

; Padding and boot signature
times 510-($-$$) db 0
dw 0xAA55                ; Boot signature
