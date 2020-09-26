#include "pch.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include <stb_image.h>

namespace Saba {

	OpenGLTexture2D::OpenGLTexture2D(void* id)
		: m_WrapperOnly(true), m_ID((uint32_t)(uint64_t)id) {}
	OpenGLTexture2D::OpenGLTexture2D(const TextureData& texData)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);

		int width = 0, height = 0, channels = 0;
		stbi_uc* data = nullptr;
		if (texData.Filepath != "")
		{
			stbi_set_flip_vertically_on_load(1);
			data = stbi_load(texData.Filepath.c_str(), &width, &height, &channels, 0);
			SB_CORE_ASSERT(data, "Failed to load texture {0}!", texData.Filepath);

			if (!(texData.Width && texData.Height))
			{
				m_Width = width;
				m_Height = height;
			}
			else
			{
				SB_CORE_ASSERT(width == texData.Width && height == texData.Height, "Width/height does not match texture width/height!");
				m_Width = width;
				m_Height = height;
			}
		}
		else
		{
			SB_CORE_ASSERT(texData.Width && texData.Height, "Width and height of a texture must be > 0");
			m_Width = texData.Width;
			m_Height = texData.Height;
		}

		if (channels == 0)
		{
			m_InternalFormat = InternalFormatInOpenGL(texData.TexFormat);
			m_Format = FormatInOpenGL(texData.TexFormat);
		}
		else
		{
			switch (texData.TexFormat)
			{
				case Format::None:
					if (channels == 1)
					{
						m_InternalFormat = GL_R8;
						m_Format = GL_RED;
					}
					else if (channels == 2)
					{
						m_InternalFormat = GL_RG8;
						m_Format = GL_RG;
					}
					else if (channels == 3)
					{
						m_InternalFormat = GL_RGB8;
						m_Format = GL_RGB;
					}
					else if (channels == 4)
					{
						m_InternalFormat = GL_RGBA8;
						m_Format = GL_RGBA;
					}
					break;
				case Format::R8:
				case Format::R16:
				case Format::R16F:
				case Format::R32F:
					SB_CORE_ASSERT(channels == 1, "Format does not match texture format!");
					m_InternalFormat = InternalFormatInOpenGL(texData.TexFormat);
					m_Format = FormatInOpenGL(texData.TexFormat);
					break;
				case Format::RG8:
				case Format::RG16:
				case Format::RG16F:
				case Format::RG32F:
					SB_CORE_ASSERT(channels == 2, "Format does not match texture format!");
					m_InternalFormat = InternalFormatInOpenGL(texData.TexFormat);
					m_Format = FormatInOpenGL(texData.TexFormat);
					break;
				case Format::RGB8:
				case Format::RGB16:
				case Format::RGB16F:
				case Format::RGB32F:
				case Format::SRGB:
					SB_CORE_ASSERT(channels == 3, "Format does not match texture format!");
					m_InternalFormat = InternalFormatInOpenGL(texData.TexFormat);
					m_Format = FormatInOpenGL(texData.TexFormat);
					break;
				case Format::RGBA8:
				case Format::RGBA16:
				case Format::RGBA16F:
				case Format::RGBA32F:
				case Format::SRGB_ALPHA:
					SB_CORE_ASSERT(channels == 4, "Format does not match texture format!");
					m_InternalFormat = InternalFormatInOpenGL(texData.TexFormat);
					m_Format = FormatInOpenGL(texData.TexFormat);
					break;
			}
		}

		GLenum wrapMode = WrapModeInOpenGL(texData.TexWrapMode);
		GLenum minFilter = MinFilterInOpenGL(texData.MinificationFilter);
		GLenum magFilter = MagFilterInOpenGL(texData.MagnificationFilter);

		glTextureStorage2D(m_ID, texData.MipmapLevels, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, minFilter);
		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, magFilter);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, wrapMode);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, wrapMode);
		if (wrapMode == GL_CLAMP_TO_BORDER)
		{
			float borderColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
			glTextureParameterfv(m_ID, GL_TEXTURE_BORDER_COLOR, borderColor);
		}

		if (data)
			glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, m_Format, GetDataType(m_InternalFormat), data);
	}
	OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath)
		: m_InternalFormat(0), m_Format(0)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
		SB_CORE_ASSERT(data, "Failed to load texture {0}!", filepath);

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
		m_Width = width;
		m_Height = height;
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

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		if (!m_WrapperOnly)
			glDeleteTextures(1, &m_ID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		if (!m_WrapperOnly)
		{
			uint32_t bpp = m_Format == GL_RGBA ? 4 : 3;
			SB_CORE_ASSERT((bpp * m_Width * m_Height == size), "Size must be the size of whole texture!");
			glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, m_Format, GL_UNSIGNED_BYTE, data);
		}
	}

	void OpenGLTexture2D::Bind(uint32_t slot)
	{
		glBindTextureUnit(slot, m_ID);
	}

	GLenum OpenGLTexture2D::InternalFormatInOpenGL(Format format)
	{
		switch (format)
		{
			case Format::None:
				return 0;
			case Format::R8:
				return GL_R8;
			case Format::R16:
				return GL_R16;
			case Format::R16F:
				return GL_R16F;
			case Format::R32F:
				return GL_R32F;
			case Format::RG8:
				return GL_RG8;
			case Format::RG16:
				return GL_RG16;
			case Format::RG16F:
				return GL_RG16F;
			case Format::RG32F:
				return GL_RG32F;
			case Format::RGB8:
				return GL_RGB8;
			case Format::RGB16:
				return GL_RGB16;
			case Format::RGB16F:
				return GL_RGB16F;
			case Format::RGB32F:
				return GL_RGB32F;
			case Format::SRGB:
				return GL_SRGB8;
			case Format::RGBA8:
				return GL_RGBA8;
			case Format::RGBA16:
				return GL_RGBA16;
			case Format::RGBA16F:
				return GL_RGBA16F;
			case Format::RGBA32F:
				return GL_RGBA32F;
			case Format::SRGB_ALPHA:
				return GL_SRGB8_ALPHA8;
		}
		SB_CORE_ASSERT(false, "Unknown format type!");
		return 0;
	}
	GLenum OpenGLTexture2D::FormatInOpenGL(Format format)
	{
		switch (format)
		{
			case Format::None:
				return 0;
			case Format::R8:
			case Format::R16:
			case Format::R16F:
			case Format::R32F:
				return GL_RED;
			case Format::RG8:
			case Format::RG16:
			case Format::RG16F:
			case Format::RG32F:
				return GL_RG;
			case Format::RGB8:
			case Format::RGB16:
			case Format::RGB16F:
			case Format::RGB32F:
			case Format::SRGB:
				return GL_RGB;
			case Format::RGBA8:
			case Format::RGBA16:
			case Format::RGBA16F:
			case Format::RGBA32F:
			case Format::SRGB_ALPHA:
				return GL_RGBA;
		}
		SB_CORE_ASSERT(false, "Unknown format type!");
		return 0;
	}
	GLenum OpenGLTexture2D::MinFilterInOpenGL(Filter filter)
	{
		switch (filter)
		{
			case Filter::None:
			case Filter::Linear:
				return GL_LINEAR;
			case Filter::Nearest:
				return GL_NEAREST;
			case Filter::Linear_MipLinear:
				return GL_LINEAR_MIPMAP_LINEAR;
			case Filter::Nearest_MipLinear:
				return GL_NEAREST_MIPMAP_LINEAR;
			case Filter::Linear_MipNearest:
				return GL_LINEAR_MIPMAP_NEAREST;
			case Filter::Nearest_MipNearest:
				return GL_NEAREST_MIPMAP_NEAREST;
		}
		SB_CORE_ASSERT(false, "Unknown filter type!");
		return 0;
	}
	GLenum OpenGLTexture2D::MagFilterInOpenGL(Filter filter)
	{
		switch (filter)
		{
			case Filter::None:
			case Filter::Linear:
			case Filter::Linear_MipLinear:
			case Filter::Linear_MipNearest:
				return GL_LINEAR;
			case Filter::Nearest:
			case Filter::Nearest_MipLinear:
			case Filter::Nearest_MipNearest:
				return GL_NEAREST;
		}
		SB_CORE_ASSERT(false, "Unknown filter type!");
		return 0;
	}
	GLenum OpenGLTexture2D::WrapModeInOpenGL(WrapMode mode)
	{
		switch (mode)
		{
			case WrapMode::None:
			case WrapMode::Repeat:
				return GL_REPEAT;
			case WrapMode::MirroredRepeat:
				return GL_MIRRORED_REPEAT;
			case WrapMode::ClampToEdge:
				GL_CLAMP_TO_EDGE;
			case WrapMode::ClampToBorder:
				GL_CLAMP_TO_BORDER;
		}
		SB_CORE_ASSERT(false, "Unknown wrap mode");
		return 0;
	}
	GLenum OpenGLTexture2D::GetDataType(GLenum format)
	{
		switch (format)
		{
			case GL_R8:
			case GL_R16:
			case GL_RG8:
			case GL_RG16:
			case GL_RGB8:
			case GL_RGB16:
			case GL_SRGB8:
			case GL_RGBA8:
			case GL_RGBA16:
			case GL_SRGB8_ALPHA8:
				return GL_UNSIGNED_BYTE;
			case GL_R16F:
			case GL_R32F:
			case GL_RG16F:
			case GL_RG32F:
			case GL_RGB16F:
			case GL_RGB32F:
			case GL_RGBA16F:
			case GL_RGBA32F:
				return GL_FLOAT;
			default:
				return 0;
		}
	}
}
