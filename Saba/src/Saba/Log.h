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

    SB_CORE static std::shared_ptr<spdlog::logger> GetCoreLogger() noexcept { return s_CoreLogger; }
    SB_CORE static std::shared_ptr<spdlog::logger> GetClientLogger() noexcept { return s_ClientLogger; }
private:
    SB_CORE static std::shared_ptr<spdlog::logger> s_CoreLogger;
    SB_CORE static std::shared_ptr<spdlog::logger> s_ClientLogger;
};

inline std::string Format(const std::string &text) noexcept {
    return text;
}
template <typename T>
std::string Format(const std::string &text, int argNum, T &&arg) noexcept {
    std::ostringstream oss;
    size_t a = text.find('{' + std::to_string(argNum) + '}');
    size_t b = 0;
    while (a != std::string::npos) {
        oss << text.substr(b, a - b);
        oss << arg;
        b = a + 2 + std::to_string(argNum).length();
        a = text.find('{' + std::to_string(argNum) + '}', b);
    }
    if (b < text.length())
        oss << text.substr(b);
    std::string output = oss.str();
    return output;
}
template <typename T, typename... Args>
std::string Format(const std::string &text, int argNum, T &&arg,
                   Args &&...args) noexcept {
    std::ostringstream oss;
    size_t a = text.find('{' + std::to_string(argNum) + '}');
    size_t b = 0;
    while (a != std::string::npos) {
        oss << text.substr(b, a - b);
        oss << arg;
        b = a + 2 + std::to_string(argNum).length();
        a = text.find('{' + std::to_string(argNum) + '}', b);
    }
    if (b < text.length())
        oss << text.substr(b);
    std::string output = oss.str();
    return Format(output, ++argNum, std::forward<Args>(args)...);
}
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
