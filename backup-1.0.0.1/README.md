# MyOS Specification

## Project Name
MyOS

---

## Goal
Create a simple custom operating system written in Assembly and C.

The operating system must:
- Boot successfully in QEMU
- Display text on screen
- Support keyboard input
- Have a minimal kernel
- Be modular and easy to expand

---

## Architecture
- CPU Architecture: x86_64
- Boot Mode: BIOS
- Language:
  - Assembly (Bootloader)
  - C (Kernel)
- Build Tool: GCC + LD + Make

---

## Project Structure

```text
myos/
├── boot/
├── kernel/
├── drivers/
├── include/
├── build/
├── docs/
└── Makefile
```

---

## Bootloader Requirements

The bootloader must:
- Run in 16-bit real mode
- Load the kernel into memory
- Switch to protected mode
- Jump to kernel entry point

---

## Kernel Requirements

The kernel must:
- Initialize VGA text mode
- Print text to screen
- Handle infinite kernel loop safely
- Support future driver expansion

Example output:

```text
Welcome to MyOS
Kernel initialized successfully.
```

---

## Drivers

### VGA Driver
Must support:
- Print character
- Print string
- Clear screen

### Keyboard Driver
Must support:
- Read keyboard input
- Detect key presses

---

## Memory Layout

| Section | Address |
|---|---|
| Bootloader | 0x7C00 |
| Kernel | 0x1000 |

---

## Build Requirements

The project must compile using:

```bash
make
```

Run using:

```bash
make run
```

---

## Emulator

Primary emulator:
- QEMU

Optional:
- VirtualBox

---

## Future Features

Planned features:
- Interrupt handling
- Memory manager
- Filesystem
- Simple shell
- Multitasking
- Mouse support

---

## Rules

- Keep code modular
- Avoid hardcoded magic values
- Comment low-level Assembly code
- Separate drivers from kernel logic

---

## Status

Current Version: v0.1-alpha

myos/
├── boot/
│   └── boot.s              # Bootloader (16-bit real mode -> protected mode)
├── kernel/
│   └── kernel.c            # Main kernel
├── drivers/
│   ├── vga.c               # VGA driver
│   ├── vga.h
│   ├── keyboard.c          # Keyboard driver
│   └── keyboard.h
├── include/
│   └── common.h            # Shared definitions
├── build/                  # (สร้างตอน compile)
├── docs/
│   └── README.md
├── Makefile
└── linker.ld
