#include "utils.h"

int64_t GetMilliseconds()
{
    auto timestamp = std::chrono::steady_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(timestamp).count();
}