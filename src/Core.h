#pragma once

#ifdef BT_ENABLE_ASSERTS
#include "Log.h"

#ifdef BT_PLATFORM_LINUX
#include <cstdlib>
#define BT_CORE_ASSERT(x, ...)                                  \
    {                                                           \
        if (!(x))                                               \
        {                                                       \
            BT_CORE_ERROR("Assertion failed: {}", __VA_ARGS__); \
            abort();                                            \
        }                                                       \
    }
#else
BT_CORE_WARN("Asserts are not supported on this platform.");
#endif // BT_PLATFORM_LINUX

#else // BT_ENABLE_ASSERTS
#define BT_CORE_ASSERT(x, ...)
#endif // BT_ENABLE_ASSERTS

#define BIT(x) (1 << x)
