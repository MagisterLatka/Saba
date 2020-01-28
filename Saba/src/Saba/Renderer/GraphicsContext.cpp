#include "pch.h"
#include "GraphicsContext.h"

#include "Platform\OpenGL\OpenGLContext.h"

namespace Saba {

	GraphicsContext* GraphicsContext::Create(void* window)
	{
#if defined(SB_USE_OPENGL)
		return new OpenGLContext((GLFWwindow*)window);
#endif
	}

}
