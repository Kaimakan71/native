# FOSOS makefile
#
# Copyright (c) 2022, the FOSOS developers.
# SPDX-License-Identifier: BSD-2-Clause

OBJ= \
	_start.o \
	init.o

all: build/disk.img

build/disk.img: build/boot.bin build/kernel.bin
	@echo "Creating disk image"; cat build/boot.bin build/kernel.bin > build/disk.img

build/boot.bin: boot.asm
	@echo "Assembling bootloader"; nasm boot.asm -f bin -o build/boot.bin

build/kernel.bin: $(OBJ)
	@echo "Linking object files"; ld -T build/linker.ld --strip-debug -melf_i386 --gc-sections --build-id=none -z norelro -z now -o build/kernel.bin -Ttext 0x80000 $(OBJ)

.c.o:
	@echo "Compiling $<"; gcc -Wextra -Wall -Wundef -Wwrite-strings -Wno-unused-parameter -Os -fno-asynchronous-unwind-tables -fno-omit-frame-pointer -ffreestanding -fno-stack-protector -fno-ident -fno-builtin -mregparm=3 -march=i386 -m32 -fno-exceptions -fmerge-all-constants -fno-unroll-loops -fno-pie -fno-pic -nostdinc -nostdlib -Iinclude -o $@ -c $<

clean:
	@echo "Cleaning"; rm -f build/boot.bin build/kernel.bin $(OBJ)

run:
	@qemu-system-i386 -drive format=raw,file=build/disk.img,if=floppy -m size=1M,maxmem=1M -serial stdio -name Native