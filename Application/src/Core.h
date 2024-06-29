#pragma once

#include <signal.h>

#define BT_ASSERT(x, ...) { if (!(x)) { BT_ERROR("Assertion failed: {}", __VA_ARGS__); raise(SIGTRAP); } }
