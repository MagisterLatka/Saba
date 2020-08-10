#include "pch.h"
#include "Saba/Renderer/Texture.h"

#include "Saba/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Saba {

	Ref<Texture2D> Texture2D::Create(const std::string& filepath)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SB_CORE_ASSERT(false, "None API is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	return MakeRef<OpenGLTexture2D>(filepath);
		}

		SB_CORE_ASSERT(false, "Unknown API");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SB_CORE_ASSERT(false, "None API is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	return MakeRef<OpenGLTexture2D>(width, height);
		}

		SB_CORE_ASSERT(false, "Unknown API");
		return nullptr;
	}

	Saba::Ref<Saba::Texture2D> Texture2D::Create(const TextureData& data)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SB_CORE_ASSERT(false, "None API is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	return MakeRef<OpenGLTexture2D>(data);
		}

		SB_CORE_ASSERT(false, "Unknown API");
		return nullptr;
	}
}
