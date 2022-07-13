/**
 * i386 CPU driver for FOSOS
 * For manipulating CPU tables and exceptions
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <cpu.h>

private DTR gdtr;
private DTR idtr;
private Descriptor gdt[256];
private Descriptor idt[256];
private UInt16 gdtLength;

private void writeRawGDTEntry(UInt16 selector, UInt32 low, UInt32 high) {
	UInt16 i = (selector & 0xfffc) >> 3;
	gdt[i].low = low;
	gdt[i].high = high;

	if(i > gdtLength) {
		gdtr.limit = (gdtLength + 1) * 8;
	}
}

void writeGDTEntry(UInt16 selector, Descriptor desc) {
	writeRawGDTEntry(selector, desc.low, desc.high);
}

void gdt_init() {
	gdtLength = 5;
	gdtr.base = gdt;
	gdtr.limit = (gdtLength * 8) - 1;

	writeRawGDTEntry(0x0000, 0x00000000, 0x00000000);
	writeRawGDTEntry(0x0008, 0x0000ffff, 0x00cf9a00);
	writeRawGDTEntry(0x0010, 0x0000ffff, 0x00cf9200);
	writeRawGDTEntry(0x0018, 0x0000ffff, 0x00cffa00);
	writeRawGDTEntry(0x0020, 0x0000ffff, 0x00cff200);

	gdtr.base = gdt;
	gdtr.limit = (gdtLength * 8) - 1;
	asm volatile("lgdt %0"::"m"(gdtr));
}

#define flushIDT() asm volatile("lidt %0"::"m"(idtr));

void registerInterruptHandler(UInt8 vector, void (*f)()) {
	idt[vector].low = 0x00080000 | LSW((f));
	idt[vector].high = ((UInt32)(f) & 0xffff0000) | 0x8e00;
	flushIDT();
}

void registerUserInterruptHandler(UInt8 vector, void (*f)()) {
	idt[vector].low = 0x00080000 | LSW((f));
	idt[vector].high = ((UInt32)(f) & 0xffff0000) | 0xef00;
	flushIDT();
}

#define CPUPANIC(i, msg) \
	static void _exception ## i () \
	{ \
		dword cr0, cr2, cr3, cr4; \
		asm("movl %%cr0, %%eax":"=a"(cr0)); \
		asm("movl %%cr2, %%eax":"=a"(cr2)); \
		asm("movl %%cr3, %%eax":"=a"(cr3)); \
		asm("movl %%cr4, %%eax":"=a"(cr4)); \
		logf("[CPU] FATAL EXCEPTION: " msg " [CR0=%x CR2=%x CR3=%x CR4=%x]\n", cr0, cr2, cr3, cr4); \
		hang(); \
	}

CPUPANIC(0, "Divide error")
CPUPANIC(1, "Debug exception")
CPUPANIC(2, "Non-Maskable Interrupt")
CPUPANIC(3, "Breakpoint")
CPUPANIC(4, "Overflow")
CPUPANIC(5, "Bounds check")
CPUPANIC(6, "Invalid opcode")
CPUPANIC(7, "Coprocessor not available")
CPUPANIC(8, "Double fault")
CPUPANIC(9, "Coprocessor segment overrun")
CPUPANIC(10, "Invalid TSS")
CPUPANIC(11, "Segment not present")
CPUPANIC(12, "Stack exception")
CPUPANIC(13, "General protection fault")
CPUPANIC(14, "Page fault")
CPUPANIC(15, "Unknown error")
CPUPANIC(16, "Coprocessor error")

private void trap() {
	logf("[CPU] Unhandled IRQ: No handler registered (FIXME: Don't crash here!)\n");
	hang();
}

// 8259 spurious IRQ
// Not sure what it's for, so it gets ignored
extern void isr7();
asm(
	".globl isr7\n"
	"isr7:\n"
	"    iret\n"
);

void idt_init() {
	idtr.base = idt;
	idtr.limit = 0x100 * 8;

	for(UInt8 i = 0xff; i > 0x10; --i) registerInterruptHandler(i, trap);

	registerInterruptHandler(0x00, _exception0);
	registerInterruptHandler(0x01, _exception1);
	registerInterruptHandler(0x02, _exception2);
	registerInterruptHandler(0x03, _exception3);
	registerInterruptHandler(0x04, _exception4);
	registerInterruptHandler(0x05, _exception5);
	registerInterruptHandler(0x06, _exception6);
	registerInterruptHandler(0x07, _exception7);
	registerInterruptHandler(0x08, _exception8);
	registerInterruptHandler(0x09, _exception9);
	registerInterruptHandler(0x0a, _exception10);
	registerInterruptHandler(0x0b, _exception11);
	registerInterruptHandler(0x0c, _exception12);
	registerInterruptHandler(0x0d, _exception13);
	registerInterruptHandler(0x0e, _exception14);
	registerInterruptHandler(0x0f, _exception15);
	registerInterruptHandler(0x10, _exception16);

	registerInterruptHandler(IRQ_VECTOR_BASE + 7, isr7);

	flushIDT();
}