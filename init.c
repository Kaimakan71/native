/**
 * FOSOS kernel initialization
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <types.h>
#include <log.h>
#include <i8259.h>
#include <cpu.h>
#include <i8253.h>

void init() {
	logf("[Kernel] Starting FOSOS...\n");

	// Initialize the Programmable Interrupt Controller and CPU tables
	pic_init();
	gdt_init();
	idt_init();

	// Initialize drivers (register IRQ handlers here)
	pit_init();

	// Enable interrupts
	asm volatile("sti");

	// Stay idle until we get an interrupt
	for(;;) asm volatile("hlt");
}