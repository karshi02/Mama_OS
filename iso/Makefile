ASM = nasm
CC = gcc
LD = ld
OBJCOPY = objcopy
QEMU = qemu-system-x86_64

BUILD_DIR = build
# Remove -nostdlib -nostartfiles for kernel development
CFLAGS = -m32 -ffreestanding -fno-stack-protector -Wall -Wextra -Iinclude -Idrivers -std=gnu99
# Use ELF format instead of PE
LDFLAGS = -m elf_i386 -T linker.ld -nostdlib

BOOT_SRC    = boot/boot.s
WRAPPER_SRC = kernel/interrupt_wrapper.asm

KERNEL_OBJ = $(BUILD_DIR)/kernel.o $(BUILD_DIR)/vga.o $(BUILD_DIR)/keyboard.o $(BUILD_DIR)/idt.o $(BUILD_DIR)/pic.o $(BUILD_DIR)/interrupt_wrapper.o

BOOT_BIN   = $(BUILD_DIR)/boot.bin
KERNEL_ELF = $(BUILD_DIR)/kernel.elf
KERNEL_BIN = $(BUILD_DIR)/kernel.bin
FINAL_IMG  = $(BUILD_DIR)/myos.img

all: $(FINAL_IMG)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BOOT_BIN): $(BOOT_SRC) | $(BUILD_DIR)
	$(ASM) -f bin -o $@ $<

$(BUILD_DIR)/kernel.o: kernel/kernel.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/vga.o: drivers/vga.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/keyboard.o: drivers/keyboard.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/idt.o: kernel/idt.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/pic.o: kernel/pic.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/interrupt_wrapper.o: $(WRAPPER_SRC) | $(BUILD_DIR)
	$(ASM) -f elf32 -o $@ $<

# Link as ELF format (matches linker.ld)
$(KERNEL_ELF): $(KERNEL_OBJ) linker.ld | $(BUILD_DIR)
	$(LD) $(LDFLAGS) -o $@ $(KERNEL_OBJ)

# Extract raw binary from ELF for bootloader
$(KERNEL_BIN): $(KERNEL_ELF)
	$(OBJCOPY) -O binary $< $@

# Create floppy image (bootloader + kernel)
$(FINAL_IMG): $(BOOT_BIN) $(KERNEL_BIN) | $(BUILD_DIR)
	dd if=/dev/zero of=$@ bs=512 count=2880 2>/dev/null || true
	dd if=$(BOOT_BIN) of=$@ bs=512 conv=notrunc 2>/dev/null || true
	dd if=$(KERNEL_BIN) of=$@ bs=512 seek=2 conv=notrunc 2>/dev/null || true
	@echo "Build successful! Image: $@"

run: $(FINAL_IMG)
	$(QEMU) -drive format=raw,file=$< -m 256M

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean
