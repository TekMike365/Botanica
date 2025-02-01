#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <iomanip>

#ifdef BT_BUILD_DEBUG
std::shared_ptr<spdlog::logger> Log::s_DebugLogger;
int Log::s_DebugLogMask = LogMaskAll;
#endif // BT_BUILD_DEBUG
std::shared_ptr<spdlog::logger> Log::s_Logger;
std::shared_ptr<spdlog::async_logger> Log::s_SimLogger;

void Log::Init()
{
    spdlog::set_pattern("%^[%H:%M:%S-%e] %n: %v%$");

#ifdef BT_BUILD_DEBUG
    s_DebugLogger = spdlog::stdout_color_mt("DEBUG");
    s_DebugLogger->set_level(spdlog::level::trace);
#endif // BT_BUILD_DEBUG
    s_Logger = spdlog::stdout_color_mt("LOG");
    s_Logger->set_level(spdlog::level::trace);

    // GetTime
    time_t t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::stringstream ss;
    ss << "logs/sim_" << std::put_time(&tm, "%d-%m-%Y_%H-%M-%S") << ".log";
    std::string logfile = ss.str();

    Log::Info("Simulation log will be saved to: {}", logfile);

    spdlog::init_thread_pool(4096 * 4, 1);
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());
    sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(logfile));
    s_SimLogger = std::make_shared<spdlog::async_logger>("SIM", begin(sinks), end(sinks), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
    s_SimLogger->set_level(spdlog::level::trace);
}