#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Saba {

	class Log
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger> GetCoreLogger() { return s_CoreLogger; }
		static std::shared_ptr<spdlog::logger> GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#if defined(SB_DEBUG)
#	define SB_CORE_TRACE(...)	::Saba::Log::GetCoreLogger()->trace(__VA_ARGS__)
#	define SB_CORE_INFO(...)	::Saba::Log::GetCoreLogger()->info(__VA_ARGS__)
#	define SB_CORE_WARN(...)	::Saba::Log::GetCoreLogger()->warn(__VA_ARGS__)
#	define SB_CORE_ERROR(...)	::Saba::Log::GetCoreLogger()->error(__VA_ARGS__)
#	define SB_CORE_FATAL(...)	::Saba::Log::GetCoreLogger()->critical(__VA_ARGS__)

#	define SB_TRACE(...)	::Saba::Log::GetClientLogger()->trace(__VA_ARGS__)
#	define SB_INFO(...)		::Saba::Log::GetClientLogger()->info(__VA_ARGS__)
#	define SB_WARN(...)		::Saba::Log::GetClientLogger()->warn(__VA_ARGS__)
#	define SB_ERROR(...)	::Saba::Log::GetClientLogger()->error(__VA_ARGS__)
#	define SB_FATAL(...)	::Saba::Log::GetClientLogger()->critical(__VA_ARGS__)
#else
#	define SB_CORE_TRACE(...)
#	define SB_CORE_INFO(...)
#	define SB_CORE_WARN(...)
#	define SB_CORE_ERROR(...)
#	define SB_CORE_FATAL(...)

#	define SB_TRACE(...)
#	define SB_INFO(...)
#	define SB_WARN(...)
#	define SB_ERROR(...)
#	define SB_FATAL(...)
#endif
