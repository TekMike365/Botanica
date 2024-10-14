#pragma once

#include <spdlog/spdlog.h>

namespace Botanica::Core
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

#define BT_CORE_TRACE(...) Botanica::Core::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define BT_CORE_INFO(...) Botanica::Core::Log::GetCoreLogger()->info(__VA_ARGS__)
#define BT_CORE_WARN(...) Botanica::Core::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define BT_CORE_ERROR(...) Botanica::Core::Log::GetCoreLogger()->error(__VA_ARGS__)
