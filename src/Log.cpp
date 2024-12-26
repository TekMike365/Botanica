#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

#ifdef BT_BUILD_DEBUG
std::shared_ptr<spdlog::logger> Log::s_DebugLogger;
int Log::s_DebugLogMask = LogMaskAll;
#endif // BT_BUILD_DEBUG

void Log::Init()
{
#ifdef BT_BUILD_DEBUG
    spdlog::set_pattern("%^[%T] %n: %v%$");
    s_DebugLogger = spdlog::stdout_color_mt("DEBUG");
    s_DebugLogger->set_level(spdlog::level::trace);
#endif // BT_BUILD_DEBUG
}