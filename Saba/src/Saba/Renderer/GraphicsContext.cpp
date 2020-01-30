#include "pch.h"
#include "GraphicsContext.h"

#include "RendererAPI.h"
#include "Platform\OpenGL\OpenGLContext.h"

namespace Saba {

	GraphicsContext* GraphicsContext::Create(void* window)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SB_CORE_ASSERT(false, "None API is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	return new OpenGLContext((GLFWwindow*)window);
		}

		SB_CORE_ASSERT(false, "Unknown API");
		return nullptr;
	}

}
