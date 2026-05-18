# MyOS Makefile สำหรับ Windows UCRT64 (ใช้ PE + objcopy)

ASM = nasm
CC = gcc
LD = ld
OBJCOPY = objcopy
QEMU = qemu-system-x86_64

BUILD_DIR = build

# 32-bit flags (ไม่มี -fno-rtti เพราะใช้กับ C ไม่ได้)
CFLAGS = -m32 -ffreestanding -nostdlib -nostartfiles -fno-stack-protector \
         -Wall -Wextra -Iinclude -Idrivers

# ใช้ i386pe (Windows PE format) แทน elf_i386
LDFLAGS = -mi386pe -T linker.ld

BOOT_SRC = boot/boot.s
KERNEL_SRC = kernel/kernel.c
DRIVERS_SRC = drivers/vga.c drivers/keyboard.c

KERNEL_OBJ = $(BUILD_DIR)/kernel.o $(BUILD_DIR)/vga.o $(BUILD_DIR)/keyboard.o
BOOT_BIN = $(BUILD_DIR)/boot.bin
KERNEL_PE = $(BUILD_DIR)/kernel.pe
KERNEL_BIN = $(BUILD_DIR)/kernel.bin
FINAL_IMG = $(BUILD_DIR)/myos.img

all: $(FINAL_IMG)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BOOT_BIN): $(BOOT_SRC) | $(BUILD_DIR)
	$(ASM) -f bin -o $@ $<

$(BUILD_DIR)/kernel.o: $(KERNEL_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/vga.o: drivers/vga.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/keyboard.o: drivers/keyboard.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# สร้าง PE executable (Windows format)
$(KERNEL_PE): $(KERNEL_OBJ) linker.ld | $(BUILD_DIR)
	$(LD) $(LDFLAGS) -o $@ $(KERNEL_OBJ)

# แปลง PE เป็น raw binary สำหรับ boot
$(KERNEL_BIN): $(KERNEL_PE)
	$(OBJCOPY) -O binary $< $@

# รวม bootloader + kernel เป็น disk image
$(FINAL_IMG): $(BOOT_BIN) $(KERNEL_BIN)
	dd if=/dev/zero of=$@ bs=512 count=2880 2>/dev/null || true
	dd if=$(BOOT_BIN) of=$@ bs=512 conv=notrunc 2>/dev/null || true
	dd if=$(KERNEL_BIN) of=$@ bs=512 seek=2 conv=notrunc 2>/dev/null || true
	@echo "========================================="
	@echo "Build successful! Image size: $$(wc -c < $@) bytes"
	@echo "Run 'make run' to start QEMU"
	@echo "========================================="

run: $(FINAL_IMG)
	$(QEMU) -drive format=raw,file=$(FINAL_IMG) -m 256M

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean
