#include "pch.h"
#include "Buffer.h"

#include "RendererAPI.h"
#include "Platform\OpenGL\OpenGLBuffer.h"

namespace Saba {

	Ref<VertexBuffer> VertexBuffer::Create(float* data, uint size)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SB_CORE_ASSERT(false, "None API is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	return MakeRef<OpenGLVertexBuffer>(data, size);
		}

		SB_CORE_ASSERT(false, "Unknown API");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint* data, uint count)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SB_CORE_ASSERT(false, "None API is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	return MakeRef<OpenGLIndexBuffer>(data, count);
		}

		SB_CORE_ASSERT(false, "Unknown API");
		return nullptr;
	}

}
