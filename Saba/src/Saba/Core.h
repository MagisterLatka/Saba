#pragma once

#include "Saba/Platform.h"

#if defined(SB_DEBUG)
#	if defined(SB_PLATFORM_WINDOWS)
#		define SB_BREAK() __debugbreak()
#	elif defined (SB_PLATFORM_LINUX)
#		include <signal.h>
#		define SB_BREAK() raise(SIGTRAP)
#	else
#		error "Platform does not support debug break!"
#	endif
#	define SB_ENABLE_ASSERTS
#else
#	define SB_BREAK()
#endif

#if defined(SB_ENABLE_ASSERTS)
#	define SB_ASSERT(x, ...) { if (!(x)) { SB_FATAL(__VA_ARGS__); SB_BREAK(); } }
#	define SB_CORE_ASSERT(x, ...) { if (!(x)) { SB_CORE_FATAL(__VA_ARGS__); SB_BREAK(); } }
#else
#	define SB_ASSERT(x, ...)
#	define SB_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)
#define SB_BIND_EVENT_FUNC(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Saba {

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> MakeRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> MakeScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
}

