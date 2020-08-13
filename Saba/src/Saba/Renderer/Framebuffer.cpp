#include "pch.h"
#include "Saba/Renderer/Framebuffer.h"

#include "Saba/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Saba {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpec& spec)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SB_CORE_ASSERT(false, "None API is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	return MakeRef<OpenGLFramebuffer>(spec);
		}

		SB_CORE_ASSERT(false, "Unknown API");
		return nullptr;
	}
}
