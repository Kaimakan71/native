# FOSOS makefile
#
# Copyright (c) 2022, the FOSOS developers.
# SPDX-License-Identifier: BSD-2-Clause

# Object files / modules to put in the kenel binary
OBJ=\
	_start.o \
	init.o \
	arch/i386/io.o \
	log.o \
	arch/i386/i8259.o \
	arch/i386/cpu.o \
	arch/i386/i8253.o

# Arguments to pass QEMU
EMU=-drive format=raw,file=build/disk.img,if=floppy -boot a -m size=1M,maxmem=1M -serial stdio -name Native

all: build/disk.img

# Create a bootable disk
build/disk.img: build/boot.bin build/kernel.bin
	@echo "Creating disk image"; cat build/boot.bin build/kernel.bin > build/disk.img

# Assemble the bootloader
build/boot.bin: arch/i386/boot.asm
	@echo "Assembling bootloader"; nasm arch/i386/boot.asm -f bin -o build/boot.bin

# Link the kernel binary
build/kernel.bin: $(OBJ)
	@echo "Linking object files"; ld -T build/linker.ld --strip-debug -melf_i386 --gc-sections --build-id=none -z norelro -z now -o build/kernel.bin -Ttext 0x80000 $(OBJ)

# Compile a c source file
.c.o:
	@echo "Compiling $<"; gcc -Wextra -Wall -Os -fno-asynchronous-unwind-tables -fno-omit-frame-pointer -ffreestanding -fno-stack-protector -fno-ident -fno-builtin -mregparm=3 -march=i386 -m32 -fno-exceptions -fmerge-all-constants -fno-unroll-loops -fno-pie -fno-pic -nostdinc -nostdlib -Iinclude -o $@ -c $<

# Clean up binary files
clean:
	@echo "Cleaning"; rm -f build/boot.bin build/kernel.bin $(OBJ)

# Run QEMU with our disk image
run:
	@qemu-system-i386 $(EMU)

# Build and run in one command
# This only works on WSL machines
wsl: build/disk.img
	@"/mnt/c/Program Files/qemu/qemu-system-i386.exe" $(EMU)

# Commit all changes to the remote git repository
update:
	@git add --all
	@git commit
	@git push