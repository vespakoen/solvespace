//-----------------------------------------------------------------------------
// Minimal Platform-dependent functionality.
//
// Copyright 2023 vespakoen
//-----------------------------------------------------------------------------

#ifndef SOLVESPACE_MINIMAL_PLATFORM_H
#define SOLVESPACE_MINIMAL_PLATFORM_H

#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include <cstring>
#include "mimalloc.h"

namespace SolveSpace {
namespace Platform {

// Debug print function.
void DebugPrint(const char *fmt, ...);

// Temporary arena functions.
void *AllocTemporary(size_t size);
void FreeAllTemporary();

}
}

#endif
