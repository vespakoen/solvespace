#include "minimalplatform.h"

namespace SolveSpace {
namespace Platform {

std::string ssprintf(const char *fmt, ...) {
    va_list va;

    va_start(va, fmt);
    int size = vsnprintf(NULL, 0, fmt, va);
    ssassert(size >= 0, "vsnprintf could not encode string");
    va_end(va);

    std::string result;
    result.resize(size + 1);

    va_start(va, fmt);
    vsnprintf(&result[0], size + 1, fmt, va);
    va_end(va);

    result.resize(size);
    return result;
}

void Error(const char *fmt, ...) {
    va_list args;
    char buf[1000];
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
}

void FatalError(const std::string &message) {
    fprintf(stderr, "%s", message.c_str());
    abort();
}

void AssertFailure(const char *file, unsigned line, const char *function, const char *condition,
                   const char *message) {
    std::string formattedMsg;
    formattedMsg += ssprintf("File %s, line %u, function %s:\n", file, line, function);
    formattedMsg += ssprintf("Assertion failed: %s.\n", condition);
    formattedMsg += ssprintf("Message: %s.\n", message);
    FatalError(formattedMsg);
}

#if defined(WIN32)
    void DebugPrint(const char *fmt, ...) {
        va_list va;
        va_start(va, fmt);
        int len = _vscprintf(fmt, va) + 1;
        va_end(va);

        va_start(va, fmt);
        char *buf = (char *)_alloca(len);
        _vsnprintf(buf, len, fmt, va);
        va_end(va);

        // The native version of OutputDebugString, unlike most others,
        // is OutputDebugStringA.
        OutputDebugStringA(buf);
        OutputDebugStringA("\n");
#    ifndef NDEBUG
        // Duplicate to stderr in debug builds, but not in release; this is slow.
        fputs(buf, stderr);
        fputc('\n', stderr);
#    endif
    }
#endif

#if !defined(WIN32)
    void DebugPrint(const char *fmt, ...) {
        va_list va;
        va_start(va, fmt);
        vfprintf(stderr, fmt, va);
        fputc('\n', stderr);
        va_end(va);
    }
#endif

    struct MimallocHeap {
        mi_heap_t *heap = NULL;

        ~MimallocHeap() {
            if(heap != NULL)
                mi_heap_destroy(heap);
        }
    };

    static thread_local MimallocHeap TempArena;

    void *AllocTemporary(size_t size) {
        if(TempArena.heap == NULL) {
            TempArena.heap = mi_heap_new();
            ssassert(TempArena.heap != NULL, "out of memory");
        }
        void *ptr = mi_heap_zalloc(TempArena.heap, size);
        ssassert(ptr != NULL, "out of memory");
        return ptr;
    }

    void FreeAllTemporary() {
        MimallocHeap temp;
        std::swap(TempArena.heap, temp.heap);
    }
} // namespace Platform
} // namespace SolveSpace