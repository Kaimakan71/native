/**
 * FOSOS kernel initialization
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <types.h>
#include <log.h>

void init() {
	logf("[Kernel] Starting FOSOS...\n");

	// Stay idle until we get an interrupt
	for(;;) asm volatile("hlt");
}