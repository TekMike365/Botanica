#pragma once

#include <memory>

#include <spdlog/spdlog.h>

namespace Botanica
{

    class Log
    {
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetDebugLogger() { return s_DebugLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_DebugLogger;
    };

}

// TODO: strip
#define BT_TRACE(...) Botanica::Log::GetDebugLogger()->trace(__VA_ARGS__)
#define BT_INFO(...) Botanica::Log::GetDebugLogger()->info(__VA_ARGS__)
#define BT_WARN(...) Botanica::Log::GetDebugLogger()->warn(__VA_ARGS__)
#define BT_ERROR(...) Botanica::Log::GetDebugLogger()->error(__VA_ARGS__)
