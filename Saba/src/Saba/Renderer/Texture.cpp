#include "pch.h"
#include "Texture.h"

#include "RendererAPI.h"
#include "Platform\OpenGL\OpenGLTexture.h"

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

	Ref<Texture2D> Texture2D::Create(const std::string& filepath, Format format)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SB_CORE_ASSERT(false, "None API is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	return MakeRef<OpenGLTexture2D>(filepath, format);
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

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, Format format)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SB_CORE_ASSERT(false, "None API is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	return MakeRef<OpenGLTexture2D>(width, height, format);
		}

		SB_CORE_ASSERT(false, "Unknown API");
		return nullptr;
	}

}
