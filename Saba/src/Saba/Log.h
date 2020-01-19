#pragma once

#include "spdlog\spdlog.h"
#include "spdlog\fmt\ostr.h"

namespace Saba {

	class Log
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger> GetCoreLogger() { return s_CoreLogger; }
		static std::shared_ptr<spdlog::logger> GetLogger() { return s_Logger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_Logger;
	};
}

#if defined(DEBUG)
#	define SB_CORE_TRACE(...)	::Saba::Log::GetCoreLogger()->trace(__VA_ARGS__)
#	define SB_CORE_INFO(...)	::Saba::Log::GetCoreLogger()->info(__VA_ARGS__)
#	define SB_CORE_WARN(...)	::Saba::Log::GetCoreLogger()->warn(__VA_ARGS__)
#	define SB_CORE_ERROR(...)	::Saba::Log::GetCoreLogger()->error(__VA_ARGS__)
#	define SB_CORE_FATAL(...)	::Saba::Log::GetCoreLogger()->critical(__VA_ARGS__)

#	define SB_TRACE(...)	::Saba::Log::GetLogger()->trace(__VA_ARGS__)
#	define SB_INFO(...)		::Saba::Log::GetLogger()->info(__VA_ARGS__)
#	define SB_WARN(...)		::Saba::Log::GetLogger()->warn(__VA_ARGS__)
#	define SB_ERROR(...)	::Saba::Log::GetLogger()->error(__VA_ARGS__)
#	define SB_FATAL(...)	::Saba::Log::GetLogger()->critical(__VA_ARGS__)

#	 define SB_CORE_ASSERT(x, ...)	if (!x) { SB_CORE_FATAL(__VA_ARGS__); __debugbreak(); }
#	 define SB_ASSERT(x, ...)		if (!x) { SB_FATAL(__VA_ARGS__); __debugbreak(); }
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

#	 define SB_CORE_ASSERT(x, ...)
#	 define SB_ASSERT(x, ...)
#endif
