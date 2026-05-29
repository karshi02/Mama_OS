[BITS 16]
[ORG 0x7C00]

_start:
    ; Set up segments
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Save boot drive
    mov [boot_drive], dl

    ; Print loading message
    mov si, msg_loading
    call print_string

    ; Load kernel
    call load_kernel

    ; Switch to protected mode
    call switch_to_pm

    ; Should never get here
    jmp $

print_string:
    mov ah, 0x0E
.loop:
    lodsb
    test al, al
    jz .done
    int 0x10
    jmp .loop
.done:
    ret

load_kernel:
    ; Reset disk system
    mov ah, 0x00
    mov dl, [boot_drive]
    int 0x13
    jc disk_error

    ; Load kernel from sectors 2+
    ; Address: 0x1000:0x0000 (linear 0x10000)
    mov ax, 0x1000
    mov es, ax
    xor bx, bx

    mov ah, 0x02        ; Read sectors
    mov al, 50          ; Number of sectors to read (adjust based on kernel size)
    mov ch, 0           ; Cylinder 0
    mov cl, 2           ; Start from sector 2 (sector 1 is bootloader)
    mov dh, 0           ; Head 0
    mov dl, [boot_drive] ; Drive
    int 0x13
    jc disk_error

    ret

disk_error:
    mov si, msg_disk_error
    call print_string
    jmp $

switch_to_pm:
    cli                 ; Disable interrupts
    lgdt [gdt_descriptor] ; Load GDT
    
    ; Enable protected mode
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    
    ; Far jump to 32-bit code
    jmp 0x08:init_pm

[BITS 32]
init_pm:
    ; Set up data segments
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    ; Set up stack (above kernel)
    mov esp, 0x90000
    
    ; Jump to kernel entry point
    ; Kernel loaded at 0x1000:0x0000 = linear 0x10000
    ; But we're in protected mode now, so just use linear address
    jmp 0x10000

; Global Descriptor Table
gdt_start:
    ; Null descriptor
    dq 0x0000000000000000

gdt_code:
    ; Code segment descriptor
    dw 0xFFFF           ; Limit (bits 0-15)
    dw 0x0000           ; Base (bits 0-15)
    db 0x00             ; Base (bits 16-23)
    db 0x9A             ; Present, ring 0, code, non-conforming, readable
    db 0xCF             ; Granularity (4KB), 32-bit, Limit (bits 16-19)
    db 0x00             ; Base (bits 24-31)

gdt_data:
    ; Data segment descriptor
    dw 0xFFFF           ; Limit (bits 0-15)
    dw 0x0000           ; Base (bits 0-15)
    db 0x00             ; Base (bits 16-23)
    db 0x92             ; Present, ring 0, data, expand-up, writable
    db 0xCF             ; Granularity (4KB), 32-bit, Limit (bits 16-19)
    db 0x00             ; Base (bits 24-31)

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; Size of GDT
    dd gdt_start                ; Address of GDT

; Variables
boot_drive:     db 0

; Messages
msg_loading:    db "MyOS Bootloader", 13, 10, 0
msg_disk_error: db "Disk error!", 13, 10, 0

; Boot signature
times 510-($-$$) db 0
dw 0xAA55
