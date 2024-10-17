#pragma once

#include <spdlog/spdlog.h>

namespace Botanica
{
    class Log
    {
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger> &GetCoreLogger() { return s_CoreLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
    };

}

#define BT_CORE_TRACE(...) Botanica::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define BT_CORE_INFO(...) Botanica::Log::GetCoreLogger()->info(__VA_ARGS__)
#define BT_CORE_WARN(...) Botanica::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define BT_CORE_ERROR(...) Botanica::Log::GetCoreLogger()->error(__VA_ARGS__)
