#pragma once

#ifdef BT_ENABLE_ASSERTS
#include "Log.h"

#ifdef BT_PLATFORM_LINUX
#include <cstdlib>
#define BT_ASSERT(x, ...)                                       \
    {                                                           \
        if (!(x))                                               \
        {                                                       \
            BT_DLOG_ERROR("Assertion failed: {}", __VA_ARGS__); \
            abort();                                            \
        }                                                       \
    }
#else // BT_PLATFORM_LINUX
#ifdef BT_PLATFORM_WINDOWS
#define BT_ASSERT(x, ...)                                       \
    {                                                           \
        if (!(x))                                               \
        {                                                       \
            BT_DLOG_ERROR("Assertion failed: {}", __VA_ARGS__); \
            __debugbreak();                                     \
        }                                                       \
    }
#else // BT_PLATFORM_WINDOWS
#define BT_ASSERT(x, ...)
Log::Warn("Asserts are not supported on this platform.");
#endif // BT_PLATFORM_WINDOWS
#endif // BT_PLATFORM_LINUX


#else // BT_ENABLE_ASSERTS
#define BT_ASSERT(x, ...)
#endif // BT_ENABLE_ASSERTS

#define BIT(x) (1 << x)

inline uint32_t PCGHash(uint32_t seed)
{
    uint32_t state = seed * 747796405u + 2891336453u;
    uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}