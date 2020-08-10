#include "pch.h"
#include "Saba/Window.h"

#include "Platform/GLFW/WindowGLFW.h"

namespace Saba {

	Scope<Window> Window::Create(const WindowProps& props)
	{
#if defined(SB_USE_GLFW)
		return MakeScope<WindowGLFW>(props);
#else
		SB_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}

}
