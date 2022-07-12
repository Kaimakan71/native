/*
	FOSOS linker script

	Copyright (c) 2022, the FOSOS developers.
	SPDX-License-Identifier: BSD-2-Clause
*/

/* ENTRY is executed first, so we use _start */
ENTRY(_start)

SECTIONS
{
	/* Start at 512K, where the bootloader loads us */
	. = 0x80000;

	/* Executable code first */
	.text BLOCK(4K) : ALIGN(4K)
	{
		_start.o
		*(.text)
	}

	/* Read-only data */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	/* Initialized read/write data */
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	/* Uninitialized read/write data */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
}
