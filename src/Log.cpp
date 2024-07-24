#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace Botanica
{

    std::shared_ptr<spdlog::logger> Log::s_DebugLogger;

    void Log::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");

        s_DebugLogger = spdlog::stdout_color_mt("DEBUG");
        s_DebugLogger->set_level(spdlog::level::trace);
        BT_INFO("Initialized Logger.");
    }

}
