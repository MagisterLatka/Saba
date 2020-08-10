#include "pch.h"
#include "Saba/Renderer/GraphicsContext.h"

#include "Saba/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Saba {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SB_CORE_ASSERT(false, "None API is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	return MakeScope<OpenGLContext>((GLFWwindow*)window);
		}

		SB_CORE_ASSERT(false, "Unknown API");
		return nullptr;
	}

}
