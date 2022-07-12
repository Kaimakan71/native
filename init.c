/**
 * FOSOS kernel initialization
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

void init() {
	// Stay idle until we get an interrupt
	for(;;) asm volatile("hlt");
}