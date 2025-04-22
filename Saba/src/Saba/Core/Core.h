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

#include <Saba/Core/Exception.h>

#define SB_EXPAND(x) x
#define SB_BIND_EVENT_FN(fn) [this](auto &&...args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#define BIT(x) (1 << x)

#if defined(SB_DEBUG) || defined(SB_RELEASE)
#   define SB_CORE_ASSERT(x, ...) { if (!(x)) { std::string message = "Assertion failed: "; __VA_OPT__(message += std::format(__VA_ARGS__));\
        SB_CORE_CRITICAL(message); throw Saba::MessageException(__LINE__, __FILE__, message, true); } }
#   define SB_ASSERT(x, ...) { if (!(x)) { std::string message = "Assertion failed: "; __VA_OPT__(message += std::format(__VA_ARGS__));\
        SB_CRITICAL(message); throw Saba::MessageException(__LINE__, __FILE__, message, false); } }
#else
#   define SB_CORE_ASSERT(x, ...) { [[maybe_unused]] auto _ = (x); }
#   define SB_ASSERT(x, ...) { [[maybe_unused]] auto _ = (x); }
#endif

#if defined(SB_DEBUG) || defined(SB_RELEASE)
#   define SB_CORE_THROW throw Saba::SabaException(__LINE__, __FILE__, true);
#   define SB_THROW throw Saba::SabaException(__LINE__, __FILE__, false);

#   define SB_CORE_THROW_INFO(...) { SB_CORE_CRITICAL(__VA_ARGS__); throw Saba::MessageException(__LINE__, __FILE__, std::format(__VA_ARGS__), true); }
#   define SB_THROW_INFO(...) { SB_CRITICAL(__VA_ARGS__); throw Saba::MessageException(__LINE__, __FILE__, std::format(__VA_ARGS__), false); }
#else
#   define SB_CORE_THROW
#   define SB_THROW

#   define SB_CORE_THROW_INFO(...)
#   define SB_THROW_INFO(...)
#endif

#include <Saba/Core/Timer.h>

namespace Saba {

SB_CORE void InitializeCore(int argc, char** argv, char** envp);
SB_CORE void ShutdownCore();
SB_CORE uint64_t GetTime() noexcept;

} // namespace Saba

#include <Saba/Core/Ref.h>

template <typename T> using Scope = std::unique_ptr<T>;

template <typename T, typename... Args>
Scope<T> CreateScope(Args &&...args) noexcept {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T, typename... U>
concept IsOneOf = (std::same_as<T, U> || ...);
