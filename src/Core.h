#pragma once

#ifdef BT_PLATFORM_UNKNOWN
#error Unknown platform!
#endif // BT_PLATFORM_UNKNOWN

#ifdef BT_PLATFORM_WINDOWS
#error Botanica only knows linux for now.
#endif // BT_PLATFORM_WINDOWS

#ifdef BT_PLATFORM_LINUX
#endif // BT_PLATFORM_LINUX

#ifdef BT_ENABLE_ASSERTS

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
#endif // BT_PLATFORM_LINUX

#else  // BT_ENABLE_ASSERTS
#define BT_CORE_ASSERT(x, ...)
#endif // BT_ENABLE_ASSERTS
