#include "pch.h"
#include "Window.h"

#include "Platform\GLFW\WindowGLFW.h"

namespace Saba {

	Ref<Window> Window::Create(const WindowProps& props)
	{
#if defined(SB_USE_GLFW)
		return MakeRef<WindowGLFW>(props);
#endif
		return nullptr;
	}

}
