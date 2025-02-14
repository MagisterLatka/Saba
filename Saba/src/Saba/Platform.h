#if defined(_WIN32)
#   if defined(_WIN64)
#       define SB_PLATFORM_WINDOWS
#   else
#       error "x86 Builds are not supported!"
#   endif
#elif defined(__APPLE__) || defined(__MACH__)
#   include <TargetConditionals.h>
#   if TARGET_IPHONE_SIMULATOR == 1
#       error "IOS simulator is not supported!"
#   elif TARGET_OS_IPHONE == 1
#       define SB_PLATFORM_IOS
#       error "IOS is not supported!"
#   elif TARGET_OS_MAC == 1
#       define SB_PLATFORM_MACOS
#       error "MacOS is not supported!"
#   else
#       error "Unknown Apple platform!"
#   endif
#elif defined(__ANDROID__)
#   define SB_PLATFORM_ANDROID
#   error "Android is not supported!"
#elif defined(__linux__)
#   define SB_PLATFORM_LINUX
#else
#   error "Unknown platform!"
#endif

#if defined(SB_DEBUG) && defined(SB_COMPILER_CL)
#   define SB_BREAK() __debugbreak()
#else
#   define SB_BREAK()
#endif
