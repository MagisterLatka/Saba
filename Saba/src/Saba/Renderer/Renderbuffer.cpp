#include "pch.h"
#include "Renderbuffer.h"
#include "RendererAPI.h"
#include "Platform\OpenGL\OpenGLRenderbuffer.h"

namespace Saba {

	Ref<Renderbuffer> Renderbuffer::Create(uint32_t width, uint32_t height, Format format)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SB_CORE_ASSERT(false, "None API is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	return MakeRef<OpenGLRenderbuffer>(width, height, format);
		}

		SB_CORE_ASSERT(false, "Unknown API");
		return nullptr;
	}
}
