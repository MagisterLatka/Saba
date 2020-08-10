#include "pch.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Saba {

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
			case API::None:		SB_CORE_ASSERT(false, "None API is not supported"); return nullptr;
			case API::OpenGL:	return MakeScope<OpenGLRendererAPI>();
		}

		SB_CORE_ASSERT(false, "Unknown API");
		return nullptr;
	}

}