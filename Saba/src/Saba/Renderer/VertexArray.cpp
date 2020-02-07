#include "pch.h"
#include "VertexArray.h"

#include "RendererAPI.h"
#include "Platform\OpenGL\OpenGLVertexArray.h"

namespace Saba {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SB_CORE_ASSERT(false, "None API is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	return MakeRef<OpenGLVertexArray>();
		}

		SB_CORE_ASSERT(false, "Unknown API");
		return nullptr;
	}

}
