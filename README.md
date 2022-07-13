# FOSOS v1.0 "Native"
A modern 32-bit operating system written from scratch in C and licensed under the tiny [BSD 2-Clause license](LICENSE)

## Features & To-do
Note: in order of priority
- [x] 32-bit kernel
- [x] Serial debugging (print to host console)
- [x] i8259 Interrupt Controller driver
- [x] Kernel IDT & GDT
- [x] i8253 Interval Timer driver
- [ ] Heap manager (malloc & free)
- [ ] Keyboard driver & shell
- [ ] Cooperative multitasking
- [ ] PCI driver / device detector
- [ ] IDE/ATA disk driver

## Authors
* Creator & main development - [@Kaimakan71](https://github.com/Kaimakan71)
* C bugs help :~) - [@CoherenceLabs](https://github.com/CoherenceLabs)
* Helpful articles/tutorials - [OSDev Wiki](https://wiki.osdev.org)

## Prerequisites
* [QEMU](https://www.qemu.org) (emulator)
* [Make](https://www.gnu.org/software/make) (build system)
* [NASM](https://www.nasm.us) (assembler)
* [GCC](https://gcc.gnu.org) (compiler collection)

## Running & Building
A prebuilt binary is included with each update so you can easily test it. Just clone the repository and run `make run`! Building is simple using the Makefile included. Just `make` and all changed files will be recompiled.

Copyright (c) 2022, the FOSOS developers.