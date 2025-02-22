#include <pch.h>
#include "Window.h"

#if defined(SB_PLATFORM_WINDOWS)
#   include "Saba/Platform/Windows/WindowsWindow.h"
#elif defined(SB_PLATFORM_LINUX)
#   include "Saba/Platform/Linux/LinuxWindow.h"
#else

#endif

namespace Saba {

Ref<Window> Window::Create(const WindowProps& props) {
#if defined(SB_PLATFORM_WINDOWS)
    return Ref<WindowsWindow>::Create(props);
#elif defined(SB_PLATFORM_LINUX)
    return Ref<LinuxWindow>::Create(props);
#else
    return nullptr;
#endif
}

std::optional<int> Window::ProcessEvents() {
#if defined(SB_PLATFORM_WINDOWS)
    return WindowsWindow::ProcessEvents();
#elif defined(SB_PLATFORM_LINUX)
    return LinuxWindow::ProcessEvents();
#else
    return {};
#endif
}

}
