#pragma once

#include "Saba\Renderer\Texture.h"
#include <glad\glad.h>

namespace Saba {

	class OpenGLTexture2D : public Texture2D
	{
		friend class OpenGLFramebuffer;
	public:
		OpenGLTexture2D(const std::string& filepath);
		OpenGLTexture2D(const std::string& filepath, Format format, Wrap wrap);
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(uint32_t width, uint32_t height, Format format, Wrap wrap);
		virtual ~OpenGLTexture2D();

		inline virtual uint32_t GetWidth() const override { return m_Width; }
		inline virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot) override;
	private:
		static uint32_t FormatToOpenGL(Format format);
		static uint32_t InternalFormatToOpenGL(Format format);
		static uint32_t GetDataType(Format format);
		static uint32_t GetDataType(GLenum format);
		static uint32_t WrapToOpenGL(Wrap wrap);
	private:
		uint32_t m_ID;
		uint32_t m_Width, m_Height;
		GLenum m_InternalFormat, m_Format;
	};

}

