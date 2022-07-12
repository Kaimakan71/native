/**
 * FOSOS debug logging functions
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <io.h>
#include <stdarg.h>

#define putChar(ch) outb(0x3f8, ch);
void logf(const char* fmt, ... );