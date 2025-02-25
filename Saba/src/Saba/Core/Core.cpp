#include <pch.h>
#include "Core.h"

#include "Saba/Core/Log.h"

#define SB_BUILD_ID "v0.0.1"

namespace Saba {

static std::filesystem::path s_Current;
void InitializeCore([[maybe_unused]] int argc, char** argv, [[maybe_unused]] char** envp) {
#if defined(SB_PLATFORM_LINUX)
    s_Current = std::filesystem::current_path();
    std::filesystem::current_path(std::filesystem::path(argv[0]).parent_path());
#endif

    Log::Init();

    SB_CORE_INFO("Saba Engine {}", SB_BUILD_ID);
    SB_CORE_INFO("Initializing...");
}
void ShutdownCore() {
    SB_CORE_INFO("Shutting down...");
#if defined(SB_PLATFORM_LINUX)
    std::filesystem::current_path(s_Current);
#endif
}

uint64_t GetTime() noexcept {
    return static_cast<uint64_t>(std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count());
}

} // namespace Saba
