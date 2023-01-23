//-----------------------------------------------------------------------------
// Minimal Platform-dependent functionality.
//
// Copyright 2023 vespakoen
//-----------------------------------------------------------------------------

#ifndef SOLVESPACE_MINIMAL_PLATFORM_H
#define SOLVESPACE_MINIMAL_PLATFORM_H

#include <cstdarg>
#include <cstdio>
#include <cassert>
#include <string>
#include "mimalloc.h"
#if defined(WIN32)
#    include <malloc.h>
#endif


namespace SolveSpace {
namespace Platform {

    // Debug print function.
    void DebugPrint(const char *fmt, ...);

    // Temporary arena functions.
    void *AllocTemporary(size_t size);
    void FreeAllTemporary();

#if defined(__GNUC__)
#    define ssassert(condition, message) \
        do { \
            if(__builtin_expect((condition), true) == false) { \
                AssertFailure(__FILE__, __LINE__, __func__, #condition, message); \
                __builtin_unreachable(); \
            } \
        } while(0)
#else
#    define ssassert(condition, message) \
        do { \
            if((condition) == false) { \
                AssertFailure(__FILE__, __LINE__, __func__, #condition, message); \
                abort(); \
            } \
        } while(0)
#endif

    std::string ssprintf(const char *fmt, ...);

    void Error(const char *fmt, ...);

    void FatalError(const std::string &message);

    void AssertFailure(const char *file, unsigned line, const char *function, const char *condition,
                       const char *message);

} // namespace Platform
} // namespace SolveSpace

#endif