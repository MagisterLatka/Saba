#include "Log.h"

#include "spdlog\sinks\stdout_color_sinks.h"

namespace Saba {

	std::shared_ptr<spdlog::logger> Log::s_Logger;
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_CoreLogger = spdlog::stdout_color_mt("CORE");
		s_CoreLogger->set_level(spdlog::level::trace);
		s_Logger = spdlog::stdout_color_mt("CLIENT");
		s_Logger->set_level(spdlog::level::trace);
	}

}
