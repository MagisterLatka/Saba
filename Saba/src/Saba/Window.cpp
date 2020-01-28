#include "pch.h"
#include "Window.h"

#include "Platform\GLFW\WindowGLFW.h"

namespace Saba {

	Window* Window::Create(const WindowProps& props)
	{
#if defined(SB_USE_GLFW)
		return new WindowGLFW(props);
#endif
	}

}
