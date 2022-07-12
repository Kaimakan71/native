/**
 * FOSOS debug logging functions
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <log.h>

const char hexChars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

void printUInt(UInt32 number) {
	UInt32 divisor = 1000000000;
	char character;
	char padding = 1;

	for(;;) {
		character = '0' + (number / divisor);
		number %= divisor;

		if(character != '0') padding = 0;
		if(!padding || divisor == 1) putChar(character);

		if(divisor == 1) break;
		divisor /= 10;
	}
}

void printInt(int number) {
	if(number < 0) {
		putChar('-');
		printUInt(0 - number);
	} else printUInt(number);
}

void printHex(UInt32 number, UInt8 fields) {
	byte shr_count = fields * 4;
	while(shr_count) {
		shr_count -= 4;
		putChar(hexChars[(number >> shr_count) & 0x0f]);
	}
}

void logf(const char* fmt, ... ) {
	const char* p;
	va_list ap;
	va_start(ap, fmt);

	for(p = fmt; *p; p++) {
		if(*p == '%' && *(p + 1)) {
			p++;
			switch(*p) {
				case 's':
					// String
					{
						const char* sp = va_arg(ap, const char*);
						if(!sp) {
							putChar('<');
							putChar('N');
							putChar('u');
							putChar('L');
							putChar('>');
						} else while(*sp != '\0') putChar(*sp++);
					}
					break;

				case 'i':
					// Signed integer
					printInt(va_arg(ap, int));
					break;

				case 'u':
					// Unsigned integer
					printUInt(va_arg(ap, UInt32));
					break;

				case 'X':
					// Hexadecimal dword
					putChar('0');
					putChar('x');
				case 'x':
					printHex(va_arg(ap, UInt32), 8);
					break;

				case 'B':
					// Hex byte
					putChar('0');
					putChar('x');
				case 'b':
					printHex(va_arg(ap, UInt32), 2);
					break;

				case 'W':
					// Hex word
					putChar('0');
					putChar('x');
				case 'w':
					printHex(va_arg(ap, UInt32), 4);
					break;

				case 'c':
					// Character
					putChar((char)va_arg(ap, int));
					break;

				case 'n':
					// Number - padded to 2 characters
					{
						UInt32 value = va_arg(ap, UInt32);
						if(value < 10) putChar('0');
						printUInt(value);
					}
					break;
			}
		} else putChar(*p);
	}

	va_end(ap);
}