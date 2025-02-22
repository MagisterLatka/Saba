#include <pch.h>
#include "Log.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Saba {

SB_CORE std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
SB_CORE std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

void Log::Init() {
    std::array<spdlog::sink_ptr, 2> logSinks;
    logSinks[0] = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    logSinks[0]->set_pattern("%^[%T] %n: %v%$");
    logSinks[1] = std::make_shared<spdlog::sinks::basic_file_sink_mt>("Saba.log", true);
    logSinks[1]->set_pattern("[%T] [%l] %n: %v");

    s_CoreLogger = std::make_shared<spdlog::logger>("SABA", begin(logSinks), end(logSinks));
    spdlog::register_logger(s_CoreLogger);
    s_CoreLogger->set_level(spdlog::level::trace);
    s_CoreLogger->flush_on(spdlog::level::trace);

    s_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
    spdlog::register_logger(s_ClientLogger);
    s_ClientLogger->set_level(spdlog::level::trace);
    s_ClientLogger->flush_on(spdlog::level::trace);
}
} // namespace Saba
