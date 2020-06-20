#include "pch.h"
#include "OpenGLRenderbuffer.h"
#include "OpenGLError.h"

#include <glad\glad.h>

namespace Saba {
	
	OpenGLRenderbuffer::OpenGLRenderbuffer(uint32_t width, uint32_t height, Format format)
		: m_Width(width), m_Height(height)
	{
		GLCall(glCreateRenderbuffers(1, &m_ID));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_ID));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, FormatInOpenGL(format), width, height));
	}
	OpenGLRenderbuffer::~OpenGLRenderbuffer()
	{
		glDeleteRenderbuffers(1, &m_ID);
	}

	void OpenGLRenderbuffer::Bind() const
	{
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_ID));
	}
	void OpenGLRenderbuffer::Unbind() const
	{
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
	}

	uint32_t OpenGLRenderbuffer::FormatInOpenGL(Format format)
	{
		switch (format)
		{
			case Saba::Renderbuffer::Format::None:
				return 0;
			case Saba::Renderbuffer::Format::RGB:
				return GL_RGB;
			case Saba::Renderbuffer::Format::RGBA:
				return GL_RGBA;
			case Saba::Renderbuffer::Format::RGB16:
				return GL_RGB16F;
			case Saba::Renderbuffer::Format::RGBA16:
				return GL_RGBA16F;
			case Saba::Renderbuffer::Format::Depth:
				return GL_DEPTH_COMPONENT;
			case Saba::Renderbuffer::Format::Depth16:
				return GL_DEPTH_COMPONENT16;
			case Saba::Renderbuffer::Format::Depth24:
				return GL_DEPTH_COMPONENT24;
			case Saba::Renderbuffer::Format::Depth32:
				return GL_DEPTH_COMPONENT32F;
			case Saba::Renderbuffer::Format::Depth24Stencil8:
				return GL_DEPTH24_STENCIL8;
		}
		return 0;
	}
}
