#pragma once

#if defined(_WIN32)
#	if defined(_WIN64)
		#define SB_PLATFORM_WINDOWS
	#else
#		error "x86 Builds are not supported!"
#	endif
#elif defined(__APPLE__) || defined(__MACH__)
#	include <TargetConditionals.h>
#	if TARGET_IPHONE_SIMULATOR == 1
#		error "IOS simulator is not supported!"
#	elif TARGET_OS_IPHONE == 1
#		define SB_PLATFORM_IOS
#		error "IOS is not supported!"
#	elif TARGET_OS_MAC == 1
#		define SB_PLATFORM_MACOS
#		error "MacOS is not supported!"
#	else
#		error "Unknown Apple platform!"
#	endif
#elif defined(__ANDROID__)
#	define SB_PLATFORM_ANDROID
#	error "Android is not supported!"
#elif defined(__linux__)
#		define SB_PLATFORM_LINUX
#		error "Linux is not supported!"
#else
#	error "Unknown platform!"
#endif

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

