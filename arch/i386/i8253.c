/*
 * i8253 PIT driver
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <i8253.h>

IRQHANDLER(0);

uint ticks = 0;

void handleIRQ0() {
	if((ticks % 600) == 0) logf("Hello");

	ticks++;

	exitIRQHandler(IRQ_TIMER);
}

void pit_init() {
	logf("[Timer] Initializing i8253 PIT with %uHz square wave\n", TICKS_PER_SECOND);

	outb(PIT_CMD, TIMER0_SELECT | WRITE_WORD | TIMER_MODE_SQUARE_WAVE);

	UInt16 reload = (BASE_FREQ / TICKS_PER_SECOND);
	outb(TIMER0_CMD, LSB(reload));
	outb(TIMER0_CMD, MSB(reload));

	registerIRQHandler(IRQ_TIMER, isr0);
	enableIRQ(IRQ_TIMER);
}