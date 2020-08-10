#include "pch.h"
#include "Saba/Renderer/Buffer.h"

#include "Saba/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Saba {

	Ref<VertexBuffer> VertexBuffer::Create(void* data, uint32_t size, BufferUsage usage)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SB_CORE_ASSERT(false, "None API is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	return MakeRef<OpenGLVertexBuffer>(data, size, usage);
		}

		SB_CORE_ASSERT(false, "Unknown API");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* data, uint32_t count)
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
