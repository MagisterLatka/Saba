#include "pch.h"
#include "Framebuffer.h"
#include "RendererAPI.h"
#include "Platform\OpenGL\OpenGLFramebuffer.h"

namespace Saba {

	Ref<Framebuffer> Framebuffer::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SB_CORE_ASSERT(false, "None API is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	return MakeRef<OpenGLFramebuffer>();
		}

		SB_CORE_ASSERT(false, "Unknown API");
		return nullptr;
	}
}
