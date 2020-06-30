#include "pch.h"
#include "OpenGLTexture.h"
#include "OpenGLError.h"

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
		else if (channels == 2)
		{
			m_InternalFormat = GL_RG8;
			m_Format = GL_RG;
		}
		else if (channels == 1)
		{
			m_InternalFormat = GL_R8;
			m_Format = GL_RED;
		}

		SB_CORE_ASSERT((m_InternalFormat & m_Format), "Invalid format!");

		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_ID));
		GLCall(glTextureStorage2D(m_ID, 1, m_InternalFormat, width, height));

		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT));

		GLCall(glTextureSubImage2D(m_ID, 0, 0, 0, width, height, m_Format, GL_UNSIGNED_BYTE, data));
	}
	OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath, Format format, Wrap wrapMode)
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

		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_ID));
		GLCall(glTextureStorage2D(m_ID, 1, m_InternalFormat, width, height));

		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, WrapToOpenGL(wrapMode)));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, WrapToOpenGL(wrapMode)));
		if (wrapMode == Wrap::ClampToBorder)
		{
			static constexpr float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
			GLCall(glTextureParameterfv(m_ID, GL_TEXTURE_BORDER_COLOR, borderColor));
		}

		GLCall(glTextureSubImage2D(m_ID, 0, 0, 0, width, height, m_Format, GetDataType(format), data));
	}
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: m_InternalFormat(GL_RGBA8), m_Format(GL_RGBA), m_Width(width), m_Height(height)
	{
		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_ID));
		GLCall(glTextureStorage2D(m_ID, 1, m_InternalFormat, width, height));

		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT));
	}
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, Format format, Wrap wrapMode)
		: m_InternalFormat(InternalFormatToOpenGL(format)), m_Format(FormatToOpenGL(format)), m_Width(width), m_Height(height)
	{
		SB_CORE_ASSERT((m_InternalFormat & m_Format), "Invalid format!");

		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_ID));
		GLCall(glTextureStorage2D(m_ID, 1, m_InternalFormat, width, height));

		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, WrapToOpenGL(wrapMode)));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, WrapToOpenGL(wrapMode)));
		if (wrapMode == Wrap::ClampToBorder)
		{
			static constexpr float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
			GLCall(glTextureParameterfv(m_ID, GL_TEXTURE_BORDER_COLOR, borderColor));
		}
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
						(m_InternalFormat == GL_RGBA8 || m_InternalFormat == GL_SRGB8_ALPHA8) ? 4 : (m_InternalFormat == GL_RGBA16 || m_InternalFormat == GL_RGBA16F) ? 8 : m_InternalFormat == GL_RGBA32F ? 16 :
						m_InternalFormat == GL_DEPTH_COMPONENT16 ? 2 : m_InternalFormat == GL_DEPTH_COMPONENT24 ? 3 : m_InternalFormat == GL_DEPTH_COMPONENT32F ? 4 : m_InternalFormat == GL_DEPTH24_STENCIL8 ? 4 : 0;
		SB_CORE_ASSERT((bpp * m_Width * m_Height == size), "Size must be the size of whole texture!");
		GLCall(glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, m_Format, GetDataType(m_InternalFormat), data));
	}

	void OpenGLTexture2D::Bind(uint32_t slot)
	{
		GLCall(glBindTextureUnit(slot, m_ID));
	}

	uint32_t OpenGLTexture2D::FormatToOpenGL(Format format)
	{
		switch (format)
		{
			case Saba::Texture::Format::None:
				return 0;
			case Saba::Texture::Format::R8:
			case Saba::Texture::Format::R16:
			case Saba::Texture::Format::R16F:
			case Saba::Texture::Format::R32F:
				return GL_RED;
			case Saba::Texture::Format::RG8:
			case Saba::Texture::Format::RG16:
			case Saba::Texture::Format::RG16F:
			case Saba::Texture::Format::RG32F:
				return GL_RG;
			case Saba::Texture::Format::RGB8:
			case Saba::Texture::Format::RGB16:
			case Saba::Texture::Format::RGB16F:
			case Saba::Texture::Format::RGB32F:
			case Saba::Texture::Format::SRGB:
				return GL_RGB;
			case Saba::Texture::Format::RGBA8:
			case Saba::Texture::Format::RGBA16:
			case Saba::Texture::Format::RGBA16F:
			case Saba::Texture::Format::RGBA32F:
			case Saba::Texture::Format::SRGBA:
				return GL_RGBA;
			case Saba::Texture::Format::Depth16:
			case Saba::Texture::Format::Depth24:
			case Saba::Texture::Format::Depth32:
				return GL_DEPTH_COMPONENT;
			case Saba::Texture::Format::Depth24Stencil8:
				return GL_DEPTH_STENCIL;
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
			case Saba::Texture::Format::Depth16:
				return GL_DEPTH_COMPONENT16;
			case Saba::Texture::Format::Depth24:
				return GL_DEPTH_COMPONENT24;
			case Saba::Texture::Format::Depth32:
				return GL_DEPTH_COMPONENT32F;
			case Saba::Texture::Format::Depth24Stencil8:
				return GL_DEPTH24_STENCIL8;
		}
		return 0;
	}
	uint32_t OpenGLTexture2D::GetDataType(GLenum format)
	{
		switch (format)
		{
			case GL_R8:
			case GL_R16:
			case GL_RG8:
			case GL_RG16:
			case GL_RGB8:
			case GL_RGB16:
			case GL_SRGB:
			case GL_RGBA8:
			case GL_RGBA16:
			case GL_SRGB_ALPHA:
				return GL_UNSIGNED_BYTE;
			case GL_R16F:
			case GL_R32F:
			case GL_RG16F:
			case GL_RG32F:
			case GL_RGB16F:
			case GL_RGB32F:
			case GL_RGBA16F:
			case GL_RGBA32F:
			case GL_DEPTH_COMPONENT16:
			case GL_DEPTH_COMPONENT24:
			case GL_DEPTH_COMPONENT32F:
			case GL_DEPTH24_STENCIL8:
				return GL_FLOAT;
		}
		return 0;
	}
	uint32_t OpenGLTexture2D::GetDataType(Format format)
	{
		switch (format)
		{
			case Saba::Texture::Format::None:
				return 0;
			case Saba::Texture::Format::R8:
			case Saba::Texture::Format::R16:
			case Saba::Texture::Format::RG8:
			case Saba::Texture::Format::RG16:
			case Saba::Texture::Format::RGB8:
			case Saba::Texture::Format::RGB16:
			case Saba::Texture::Format::SRGB:
			case Saba::Texture::Format::RGBA8:
			case Saba::Texture::Format::RGBA16:
			case Saba::Texture::Format::SRGBA:
				return GL_UNSIGNED_BYTE;
			case Saba::Texture::Format::R16F:
			case Saba::Texture::Format::R32F:
			case Saba::Texture::Format::RG16F:
			case Saba::Texture::Format::RG32F:
			case Saba::Texture::Format::RGB16F:
			case Saba::Texture::Format::RGB32F:
			case Saba::Texture::Format::RGBA16F:
			case Saba::Texture::Format::RGBA32F:
			case Saba::Texture::Format::Depth16:
			case Saba::Texture::Format::Depth24:
			case Saba::Texture::Format::Depth32:
			case Saba::Texture::Format::Depth24Stencil8:
				return GL_FLOAT;
		}
		return 0;
	}
	uint32_t OpenGLTexture2D::WrapToOpenGL(Wrap wrapMode)
	{
		switch (wrapMode)
		{
			case Saba::Texture::Wrap::None:
				return 0;
			case Saba::Texture::Wrap::ClampToEdge:
				return GL_CLAMP_TO_EDGE;
			case Saba::Texture::Wrap::ClampToBorder:
				return GL_CLAMP_TO_BORDER;
			case Saba::Texture::Wrap::Repeat:
				return GL_REPEAT;
			case Saba::Texture::Wrap::MirroredRepeat:
				return GL_MIRRORED_REPEAT;
		}
		return 0;
	}
	


	OpenGLTextureCubemap::OpenGLTextureCubemap(const std::string& filepath)
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
		else if (channels == 2)
		{
			m_InternalFormat = GL_RG8;
			m_Format = GL_RG;
		}
		else if (channels == 1)
		{
			m_InternalFormat = GL_R8;
			m_Format = GL_RED;
		}

		SB_CORE_ASSERT((m_InternalFormat & m_Format), "Invalid format!");

		GLCall(glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_ID));
		GLCall(glTextureStorage2D(m_ID, 1, m_InternalFormat, width, height));

		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_R, GL_REPEAT));

		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID));
		for (int i = 0; i < 6; i++)
		{
			GLCall(glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, width, height, m_Format, GL_UNSIGNED_BYTE, data));
		}
	}
	OpenGLTextureCubemap::OpenGLTextureCubemap(const std::string& filepath, Format format, Wrap wrapMode)
		: m_InternalFormat(InternalFormatToOpenGL(format)), m_Format(FormatToOpenGL(format))
	{
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

		SB_CORE_ASSERT((m_InternalFormat & m_Format), "Invalid format!");

		GLCall(glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_ID));
		GLCall(glTextureStorage2D(m_ID, 1, m_InternalFormat, width, height));

		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, WrapToOpenGL(wrapMode)));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, WrapToOpenGL(wrapMode)));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_R, WrapToOpenGL(wrapMode)));
		if (wrapMode == Wrap::ClampToBorder)
		{
			static constexpr float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
			GLCall(glTextureParameterfv(m_ID, GL_TEXTURE_BORDER_COLOR, borderColor));
		}

		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID));
		for (int i = 0; i < 6; i++)
		{
			GLCall(glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, width, height, m_Format, GetDataType(m_InternalFormat), data));
		}
	}
	OpenGLTextureCubemap::OpenGLTextureCubemap(const std::array<std::string, 6> & filepaths)
	{
		GLCall(glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_ID));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID));

		for (int i = 0; i < 6; i++)
		{
			int width, height, channels;
			stbi_set_flip_vertically_on_load(1);
			stbi_uc* data = stbi_load(filepaths[i].c_str(), &width, &height, &channels, 0);
			SB_CORE_ASSERT(data, "Failed to load texture {0}!", filepaths[i]);

			if (!m_Width && !m_Height)
			{
				m_Width = width;
				m_Height = height;
			}
			if (!m_InternalFormat && !m_Format)
			{
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
				else if (channels == 2)
				{
					m_InternalFormat = GL_RG8;
					m_Format = GL_RG;
				}
				else if (channels == 1)
				{
					m_InternalFormat = GL_R8;
					m_Format = GL_RED;
				}
			}
			SB_CORE_ASSERT((m_Width == width && m_Height == height), "All cubemap faces has to have same size");

			glTexImage2D(GL_TEXTURE_CUBE_MAP, 0, m_InternalFormat, m_Width, m_Height, 0, m_Format, GL_UNSIGNED_BYTE, data);
		}

		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_R, GL_REPEAT));
	}
	OpenGLTextureCubemap::OpenGLTextureCubemap(const std::array<std::string, 6> & filepaths, Format format, Wrap wrapMode)
		: m_InternalFormat(InternalFormatToOpenGL(format)), m_Format(FormatToOpenGL(format))
	{
		GLCall(glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_ID));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID));

		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		for (int i = 0; i < 6; i++)
		{
			int width, height, channels;
			stbi_set_flip_vertically_on_load(1);
			stbi_uc* data = stbi_load(filepaths[i].c_str(), &width, &height, &channels, 0);
			SB_CORE_ASSERT(data, "Failed to load texture {0}!", filepaths[i]);

			if (!m_Width && !m_Height)
			{
				m_Width = width;
				m_Height = height;
			}
			SB_CORE_ASSERT((m_Width == width && m_Height == height), "All cubemap faces has to have same size");

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

			glTexImage2D(GL_TEXTURE_CUBE_MAP, 0, m_InternalFormat, m_Width, m_Height, 0, m_Format, GetDataType(m_InternalFormat), data);
		}

		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, WrapToOpenGL(wrapMode)));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, WrapToOpenGL(wrapMode)));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_R, WrapToOpenGL(wrapMode)));
		if (wrapMode == Wrap::ClampToBorder)
		{
			static constexpr float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
			GLCall(glTextureParameterfv(m_ID, GL_TEXTURE_BORDER_COLOR, borderColor));
		}
	}
	OpenGLTextureCubemap::OpenGLTextureCubemap(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height), m_InternalFormat(GL_RGBA8), m_Format(GL_RGBA)
	{
		GLCall(glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_ID));
		GLCall(glTextureStorage2D(m_ID, 1, m_InternalFormat, m_Width, m_Height));

		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_R, GL_REPEAT));
	}
	OpenGLTextureCubemap::OpenGLTextureCubemap(uint32_t width, uint32_t height, Format format, Wrap wrapMode)
		: m_Width(width), m_Height(height), m_InternalFormat(InternalFormatToOpenGL(format)), m_Format(FormatToOpenGL(format))
	{
		GLCall(glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_ID));
		GLCall(glTextureStorage2D(m_ID, 1, m_InternalFormat, m_Width, m_Height));

		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, WrapToOpenGL(wrapMode)));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, WrapToOpenGL(wrapMode)));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_R, WrapToOpenGL(wrapMode)));
		if (wrapMode == Wrap::ClampToBorder)
		{
			static constexpr float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
			GLCall(glTextureParameterfv(m_ID, GL_TEXTURE_BORDER_COLOR, borderColor));
		}
	}
	OpenGLTextureCubemap::~OpenGLTextureCubemap()
	{
		glDeleteTextures(1, &m_ID);
	}
	void OpenGLTextureCubemap::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = m_InternalFormat == GL_R8 ? 1 : (m_InternalFormat == GL_R16 || m_InternalFormat == GL_R16F) ? 2 : m_InternalFormat == GL_R32F ? 4 :
						m_InternalFormat == GL_RG8 ? 2 : (m_InternalFormat == GL_RG16 || m_InternalFormat == GL_RG16F) ? 4 : m_InternalFormat == GL_RG32F ? 8 :
						(m_InternalFormat == GL_RGB8 || m_InternalFormat == GL_SRGB8) ? 3 : (m_InternalFormat == GL_RGB16 || m_InternalFormat == GL_RGB16F) ? 6 : m_InternalFormat == GL_RGB32F ? 12 :
						(m_InternalFormat == GL_RGBA8 || m_InternalFormat == GL_SRGB8_ALPHA8) ? 4 : (m_InternalFormat == GL_RGBA16 || m_InternalFormat == GL_RGBA16F) ? 8 : m_InternalFormat == GL_RGBA32F ? 16 :
						m_InternalFormat == GL_DEPTH_COMPONENT16 ? 2 : m_InternalFormat == GL_DEPTH_COMPONENT24 ? 3 : m_InternalFormat == GL_DEPTH_COMPONENT32F ? 4 : m_InternalFormat == GL_DEPTH24_STENCIL8 ? 4 : 0;
		SB_CORE_ASSERT((bpp * m_Width * m_Height == size), "Size must be the size of whole texture!");
		GLCall(glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, m_Format, GetDataType(m_InternalFormat), data));
	}
	void OpenGLTextureCubemap::Bind(uint32_t slot)
	{
		GLCall(glBindTextureUnit(slot, m_ID));
	}

	uint32_t OpenGLTextureCubemap::FormatToOpenGL(Format format)
	{
		switch (format)
		{
			case Saba::Texture::Format::None:
				return 0;
			case Saba::Texture::Format::R8:
			case Saba::Texture::Format::R16:
			case Saba::Texture::Format::R16F:
			case Saba::Texture::Format::R32F:
				return GL_RED;
			case Saba::Texture::Format::RG8:
			case Saba::Texture::Format::RG16:
			case Saba::Texture::Format::RG16F:
			case Saba::Texture::Format::RG32F:
				return GL_RG;
			case Saba::Texture::Format::RGB8:
			case Saba::Texture::Format::RGB16:
			case Saba::Texture::Format::RGB16F:
			case Saba::Texture::Format::RGB32F:
			case Saba::Texture::Format::SRGB:
				return GL_RGB;
			case Saba::Texture::Format::RGBA8:
			case Saba::Texture::Format::RGBA16:
			case Saba::Texture::Format::RGBA16F:
			case Saba::Texture::Format::RGBA32F:
			case Saba::Texture::Format::SRGBA:
				return GL_RGBA;
			case Saba::Texture::Format::Depth16:
			case Saba::Texture::Format::Depth24:
			case Saba::Texture::Format::Depth32:
				return GL_DEPTH_COMPONENT;
			case Saba::Texture::Format::Depth24Stencil8:
				return GL_DEPTH_STENCIL;
		}
		return 0;
	}
	uint32_t OpenGLTextureCubemap::InternalFormatToOpenGL(Format format)
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
			case Saba::Texture::Format::Depth16:
				return GL_DEPTH_COMPONENT16;
			case Saba::Texture::Format::Depth24:
				return GL_DEPTH_COMPONENT24;
			case Saba::Texture::Format::Depth32:
				return GL_DEPTH_COMPONENT32F;
			case Saba::Texture::Format::Depth24Stencil8:
				return GL_DEPTH24_STENCIL8;
		}
		return 0;
	}
	uint32_t OpenGLTextureCubemap::GetDataType(GLenum format)
	{
		switch (format)
		{
			case GL_R8:
			case GL_R16:
			case GL_RG8:
			case GL_RG16:
			case GL_RGB8:
			case GL_RGB16:
			case GL_SRGB:
			case GL_RGBA8:
			case GL_RGBA16:
			case GL_SRGB_ALPHA:
				return GL_UNSIGNED_BYTE;
			case GL_R16F:
			case GL_R32F:
			case GL_RG16F:
			case GL_RG32F:
			case GL_RGB16F:
			case GL_RGB32F:
			case GL_RGBA16F:
			case GL_RGBA32F:
			case GL_DEPTH_COMPONENT16:
			case GL_DEPTH_COMPONENT24:
			case GL_DEPTH_COMPONENT32F:
			case GL_DEPTH24_STENCIL8:
				return GL_FLOAT;
		}
		return 0;
	}
	uint32_t OpenGLTextureCubemap::GetDataType(Format format)
	{
		switch (format)
		{
			case Saba::Texture::Format::None:
				return 0;
			case Saba::Texture::Format::R8:
			case Saba::Texture::Format::R16:
			case Saba::Texture::Format::RG8:
			case Saba::Texture::Format::RG16:
			case Saba::Texture::Format::RGB8:
			case Saba::Texture::Format::RGB16:
			case Saba::Texture::Format::SRGB:
			case Saba::Texture::Format::RGBA8:
			case Saba::Texture::Format::RGBA16:
			case Saba::Texture::Format::SRGBA:
				return GL_UNSIGNED_BYTE;
			case Saba::Texture::Format::R16F:
			case Saba::Texture::Format::R32F:
			case Saba::Texture::Format::RG16F:
			case Saba::Texture::Format::RG32F:
			case Saba::Texture::Format::RGB16F:
			case Saba::Texture::Format::RGB32F:
			case Saba::Texture::Format::RGBA16F:
			case Saba::Texture::Format::RGBA32F:
			case Saba::Texture::Format::Depth16:
			case Saba::Texture::Format::Depth24:
			case Saba::Texture::Format::Depth32:
			case Saba::Texture::Format::Depth24Stencil8:
				return GL_FLOAT;
		}
		return 0;
	}
	uint32_t OpenGLTextureCubemap::WrapToOpenGL(Wrap wrapMode)
	{
		switch (wrapMode)
		{
			case Saba::Texture::Wrap::None:
				return 0;
			case Saba::Texture::Wrap::ClampToEdge:
				return GL_CLAMP_TO_EDGE;
			case Saba::Texture::Wrap::ClampToBorder:
				return GL_CLAMP_TO_BORDER;
			case Saba::Texture::Wrap::Repeat:
				return GL_REPEAT;
			case Saba::Texture::Wrap::MirroredRepeat:
				return GL_MIRRORED_REPEAT;
		}
		return 0;
	}
}
