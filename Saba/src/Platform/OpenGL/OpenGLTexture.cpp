#include "pch.h"
#include "OpenGLTexture.h"

#include <stb_image.h>

namespace Saba {

	OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath)
		: m_InternalFormat(0), m_Format(0)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
		SB_CORE_ASSERT(data, "Failed to load texture {0}!", filepath);

		m_Width = width;
		m_Height = height;

		if (channels == 4)
		{
			m_InternalFormat = GL_RGBA8;
			m_Format = GL_RGBA;
		}
		else if (channels == 3)
		{
			m_InternalFormat = GL_RGB8;
			m_Format = GL_RGB;
		}

		SB_CORE_ASSERT((m_InternalFormat & m_Format), "Invalid format!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
		glTextureStorage2D(m_ID, 1, m_InternalFormat, width, height);

		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_ID, 0, 0, 0, width, height, m_Format, GL_UNSIGNED_BYTE, data);
	}
	OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath, Format format)
		: m_InternalFormat(0), m_Format(0)
	{
		if ((int)format == 0)
		{
			SB_CORE_ASSERT(false, "Invalid format!");
		}

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
		SB_CORE_ASSERT(data, "Failed to load texture {0}!", filepath);

		m_Width = width;
		m_Height = height;

		if (channels == 1)
		{
			if (!((int)format & ((int)Format::R8 | (int)Format::R16 | (int)Format::R16F | (int)Format::R32F)))
			{
				SB_CORE_ASSERT(false, "Invalid format!");
			}
		}
		else if (channels == 2)
		{
			if (!((int)format & ((int)Format::RG8 | (int)Format::RG16 | (int)Format::RG16F | (int)Format::RG32F)))
			{
				SB_CORE_ASSERT(false, "Invalid format!");
			}
		}
		else if (channels == 3)
		{
			if (!((int)format & ((int)Format::RGB8 | (int)Format::RGB16 | (int)Format::RGB16F | (int)Format::RGB32F | (int)Format::SRGB)))
			{
				SB_CORE_ASSERT(false, "Invalid format!");
			}
		}
		else if (channels == 4)
		{
			if (!((int)format & ((int)Format::RGBA8 | (int)Format::RGBA16 | (int)Format::RGBA16F | (int)Format::RGBA32F | (int)Format::SRGBA)))
			{
				SB_CORE_ASSERT(false, "Invalid format!");
			}
		}

		m_Format = FormatToOpenGL(format);
		m_InternalFormat = InternalFormatToOpenGL(format);
		SB_CORE_ASSERT((m_InternalFormat & m_Format), "Invalid format!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
		glTextureStorage2D(m_ID, 1, m_InternalFormat, width, height);

		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_ID, 0, 0, 0, width, height, m_Format, GL_UNSIGNED_BYTE, data);
	}
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: m_InternalFormat(GL_RGBA8), m_Format(GL_RGBA), m_Width(width), m_Height(height)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
		glTextureStorage2D(m_ID, 1, m_InternalFormat, width, height);

		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, Format format)
		: m_InternalFormat(InternalFormatToOpenGL(format)), m_Format(FormatToOpenGL(format)), m_Width(width), m_Height(height)
	{
		SB_CORE_ASSERT((m_InternalFormat & m_Format), "Invalid format!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
		glTextureStorage2D(m_ID, 1, m_InternalFormat, width, height);

		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_ID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = m_InternalFormat == GL_R8 ? 1 : (m_InternalFormat == GL_R16 || m_InternalFormat == GL_R16F) ? 2 : m_InternalFormat == GL_R32F ? 4 :
						m_InternalFormat == GL_RG8 ? 2 : (m_InternalFormat == GL_RG16 || m_InternalFormat == GL_RG16F) ? 4 : m_InternalFormat == GL_RG32F ? 8 :
						(m_InternalFormat == GL_RGB8 || m_InternalFormat == GL_SRGB8) ? 3 : (m_InternalFormat == GL_RGB16 || m_InternalFormat == GL_RGB16F) ? 6 : m_InternalFormat == GL_RGB32F ? 12 :
						(m_InternalFormat == GL_RGBA8 || m_InternalFormat == GL_SRGB8_ALPHA8) ? 4 : (m_InternalFormat == GL_RGBA16 || m_InternalFormat == GL_RGBA16F) ? 8 : m_InternalFormat == GL_RGBA32F ? 16 : 0;
		SB_CORE_ASSERT((bpp * m_Width * m_Height == size), "Size must be the size of whole texture!");
		glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, m_Format, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot)
	{
		glBindTextureUnit(slot, m_ID);
	}

	uint32_t OpenGLTexture2D::FormatToOpenGL(Format format)
	{
		switch (format)
		{
			case Saba::Texture::Format::None:
				return 0;
			case Saba::Texture::Format::R8:
				return GL_RED;
			case Saba::Texture::Format::R16:
				return GL_RED;
			case Saba::Texture::Format::R16F:
				return GL_RED;
			case Saba::Texture::Format::R32F:
				return GL_RED;
			case Saba::Texture::Format::RG8:
				return GL_RG;
			case Saba::Texture::Format::RG16:
				return GL_RG;
			case Saba::Texture::Format::RG16F:
				return GL_RG;
			case Saba::Texture::Format::RG32F:
				return GL_RG;
			case Saba::Texture::Format::RGB8:
				return GL_RGB;
			case Saba::Texture::Format::RGB16:
				return GL_RGB;
			case Saba::Texture::Format::RGB16F:
				return GL_RGB;
			case Saba::Texture::Format::RGB32F:
				return GL_RGB;
			case Saba::Texture::Format::SRGB:
				return GL_RGB;
			case Saba::Texture::Format::RGBA8:
				return GL_RGBA;
			case Saba::Texture::Format::RGBA16:
				return GL_RGBA;
			case Saba::Texture::Format::RGBA16F:
				return GL_RGBA;
			case Saba::Texture::Format::RGBA32F:
				return GL_RGBA;
			case Saba::Texture::Format::SRGBA:
				return GL_RGBA;
		}
		return 0;
	}

	uint32_t OpenGLTexture2D::InternalFormatToOpenGL(Format format)
	{
		switch (format)
		{
			case Saba::Texture::Format::None:
				return 0;
			case Saba::Texture::Format::R8:
				return GL_R8;
			case Saba::Texture::Format::R16:
				return GL_R16;
			case Saba::Texture::Format::R16F:
				return GL_R16F;
			case Saba::Texture::Format::R32F:
				return GL_R32F;
			case Saba::Texture::Format::RG8:
				return GL_RG8;
			case Saba::Texture::Format::RG16:
				return GL_RG16;
			case Saba::Texture::Format::RG16F:
				return GL_RG16F;
			case Saba::Texture::Format::RG32F:
				return GL_RG32F;
			case Saba::Texture::Format::RGB8:
				return GL_RGB8;
			case Saba::Texture::Format::RGB16:
				return GL_RGB16;
			case Saba::Texture::Format::RGB16F:
				return GL_RGB16F;
			case Saba::Texture::Format::RGB32F:
				return GL_RGB32F;
			case Saba::Texture::Format::SRGB:
				return GL_SRGB8;
			case Saba::Texture::Format::RGBA8:
				return GL_RGBA8;
			case Saba::Texture::Format::RGBA16:
				return GL_RGBA16;
			case Saba::Texture::Format::RGBA16F:
				return GL_RGBA16F;
			case Saba::Texture::Format::RGBA32F:
				return GL_RGBA32F;
			case Saba::Texture::Format::SRGBA:
				return GL_SRGB8_ALPHA8;
		}
		return 0;
	}

}
