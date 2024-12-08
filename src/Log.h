#pragma once

#include <spdlog/spdlog.h>

#include "Utils.h"

enum LogMask
{
    LogMaskNone = 0,
    LogMaskTrace = BIT(0),
    LogMaskInfo = BIT(1),
    LogMaskWarn = BIT(2),
    LogMaskError = BIT(3),
    LogMaskAll = LogMaskTrace | LogMaskInfo | LogMaskWarn | LogMaskWarn | LogMaskError,
};

class Log
{
    using PLogger = std::shared_ptr<spdlog::logger>;

public:
    static void Init();

#ifdef BT_BUILD_DEBUG
    static inline PLogger &GetDebugLogger() { return s_DebugLogger; }
    static inline int GetDebugMask() { return s_DebugLogMask; }
    static inline void MaskDebug(int logMask) { s_DebugLogMask = logMask; }
#endif // BT_BUILD_DEBUG

private:
#ifdef BT_BUILD_DEBUG
    static PLogger s_DebugLogger;
    static int s_DebugLogMask;
#endif // BT_BUILD_DEBUG
};

#ifdef BT_BUILD_DEBUG
#define BT_DLOG_MASK Log::GetDebugMask()
#define BT_SET_DLOG_MASK(x) Log::GetDebugMask(x)
#define BT_DLOG_TRACE(...)                                    \
    if (Log::GetDebugMask() & LogMaskTrace) \
    Log::GetDebugLogger()->trace(__VA_ARGS__)
#define BT_DLOG_INFO(...)                                    \
    if (Log::GetDebugMask() & LogMaskInfo) \
    Log::GetDebugLogger()->info(__VA_ARGS__)
#define BT_DLOG_WARN(...)                                    \
    if (Log::GetDebugMask() & LogMaskWarn) \
    Log::GetDebugLogger()->warn(__VA_ARGS__)
#define BT_DLOG_ERROR(...)                                    \
    if (Log::GetDebugMask() & LogMaskError) \
    Log::GetDebugLogger()->error(__VA_ARGS__)
#endif // BT_BUILD_DEBUG
