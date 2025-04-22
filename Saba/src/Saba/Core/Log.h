#pragma once

#if defined(SB_PLATFORM_WINDOWS)
#   if defined(SB_BUILD_DLL)
#       define SB_CORE __declspec(dllexport)
#   else
#       define SB_CORE __declspec(dllimport)
#   endif
#elif defined(SB_PLATFORM_LINUX)
#   if defined(SB_BUILD_DLL)
#       define SB_CORE __attribute__((visibility("default")))
#   else
#       define SB_CORE
#   endif
#else
#       define SB_CORE
#endif

#define SPDLOG_USE_STD_FORMAT
#include <spdlog/spdlog.h>

namespace Saba {

class Log {
public:
    SB_CORE static void Init();

    static std::shared_ptr<spdlog::logger> GetCoreLogger() noexcept { return s_CoreLogger; }
    static std::shared_ptr<spdlog::logger> GetClientLogger() noexcept { return s_ClientLogger; }
private:
    SB_CORE static std::shared_ptr<spdlog::logger> s_CoreLogger;
    SB_CORE static std::shared_ptr<spdlog::logger> s_ClientLogger;
};

} // namespace Saba

#define SB_CORE_TRACE(...)    Saba::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SB_CORE_INFO(...)     Saba::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SB_CORE_WARN(...)     Saba::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SB_CORE_ERROR(...)    Saba::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SB_CORE_CRITICAL(...) Saba::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define SB_TRACE(...)         Saba::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SB_INFO(...)          Saba::Log::GetClientLogger()->info(__VA_ARGS__)
#define SB_WARN(...)          Saba::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SB_ERROR(...)         Saba::Log::GetClientLogger()->error(__VA_ARGS__)
#define SB_CRITICAL(...)      Saba::Log::GetClientLogger()->critical(__VA_ARGS__)
