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

#define SB_CORE_ASSERT(x, ...) { if (!(x)) { std::string message = "Assertion failed"; __VA_OPT__(message += Saba::Format(": {-1}", -1, __VA_ARGS__));\
    SB_CORE_CRITICAL(message); throw Saba::MessageException(__LINE__, __FILE__, message, true); } }
#define SB_ASSERT(x, ...) { if (!(x)) { std::string message = "Assertion failed"; __VA_OPT__(message += Saba::Format(": {-1}", -1, __VA_ARGS__));\
    SB_CRITICAL(message); throw Saba::MessageException(__LINE__, __FILE__, message, false); } }

#define SB_CORE_THROW throw Saba::SabaException(__LINE__, __FILE__, true);
#define SB_THROW throw Saba::SabaException(__LINE__, __FILE__, false);

#define SB_CORE_THROW_INFO(message) { SB_CORE_CRITICAL(message); throw Saba::MessageException(__LINE__, __FILE__, (message), true); }
#define SB_THROW_INFO(message) { SB_CRITICAL(message); throw Saba::MessageException(__LINE__, __FILE__, (message), false); }

#include <Saba/Core/Timer.h>

namespace Saba {

void SB_CORE InitializeCore(int argc, char** argv, char** envp);
void SB_CORE ShutdownCore();
uint64_t SB_CORE GetTime() noexcept;

} // namespace Saba

#include <Saba/Core/Ref.h>

template <typename T> using Scope = std::unique_ptr<T>;

template <typename T, typename... Args>
Scope<T> CreateScope(Args &&...args) noexcept {
  return std::make_unique<T>(std::forward<Args>(args)...);
}
