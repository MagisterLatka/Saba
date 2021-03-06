#pragma once

#include "Saba/Renderer/Texture.h"
#include <glad/glad.h>

namespace Saba {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const TextureData& texData);
		OpenGLTexture2D(const std::string& filepath);
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(void* id);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot) override;
	private:
		static GLenum InternalFormatInOpenGL(Format format);
		static GLenum FormatInOpenGL(Format format);
		static GLenum MinFilterInOpenGL(Filter filter);
		static GLenum MagFilterInOpenGL(Filter filter);
		static GLenum WrapModeInOpenGL(WrapMode mode);
		static GLenum GetDataType(GLenum format);
	private:
		bool m_WrapperOnly = false;
		uint32_t m_ID;
		uint32_t m_Width, m_Height;
		GLenum m_InternalFormat, m_Format;
	};

}

