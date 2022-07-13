/**
 * Basic assertions and debugging utilities
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <log.h>

#define hang() asm volatile("cli; hlt");
#define ASSERT(x) if(!(x)) { logf("ASSERTION FAILED\n"); logf(#x " - %s:%u (%s)\n", __FILE__, __LINE__, __PRETTY_FUNCTION__); hang(); }
#define ASSERT_UNREACHABLE() logf("ASSERT UNREACHABLE FAILED\n"); logf("%s:%u (%s)\n", __FILE__, __LINE__, __PRETTY_FUNCTION__); hang();