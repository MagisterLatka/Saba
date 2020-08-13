#include "pch.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Saba {

	static constexpr uint32_t s_MaxFramebufferSize = 8192;
	static constexpr uint8_t s_MaxFramebufferSamples = 8;

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpec& spec)
		: m_Spec(spec)
	{
		SB_CORE_ASSERT(spec.Width != 0 && spec.Height != 0, "Framebuffer size cannot be 0!");
		SB_CORE_ASSERT(spec.Attachments != FramebufferAttachments::NoneAttachment, "Framebuffer needs at least one attachment");
		SB_CORE_ASSERT(0 < spec.Samples && spec.Samples <= s_MaxFramebufferSamples, "Invalid samples count!");
		Invalidate();
	}
	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_ID);
		for (auto& attachment : m_ColorAttachments)
		{
			if (attachment != 0)
				glDeleteTextures(1, &attachment);
		}
		if (m_DepthAttachment != 0)
			glDeleteTextures(1, &m_DepthAttachment);
	}

	void OpenGLFramebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
		glViewport(0, 0, m_Spec.Width, m_Spec.Height);
	}
	void OpenGLFramebuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
		{
			SB_CORE_WARN("Attempting to resize framebuffer to {0}, {1}", width, height);
			return;
		}

		m_Spec.Width = width;
		m_Spec.Height = height;

		Invalidate();
	}

	void OpenGLFramebuffer::Invalidate()
	{
		if (m_ID)
		{
			glDeleteFramebuffers(1, &m_ID);
			for (auto& attachment : m_ColorAttachments)
			{
				if (attachment != 0)
					glDeleteTextures(1, &attachment);
			}
			if (m_DepthAttachment != 0)
				glDeleteTextures(1, &m_DepthAttachment);
		}

		glCreateFramebuffers(1, &m_ID);

		for (int i = 0; i < m_ColorAttachments.size(); i++)
		{
			if (static_cast<uint32_t>(m_Spec.Attachments) & BIT(i))
			{
				if (m_Spec.Samples == 1)
				{
					glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachments[i]);
					glTextureStorage2D(m_ColorAttachments[i], 1, InternalFormatInOpenGL(m_Spec.Format), m_Spec.Width, m_Spec.Height);
					glNamedFramebufferTexture(m_ID, GL_COLOR_ATTACHMENT0 + i, m_ColorAttachments[i], 0);
				}
				else
				{
					glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &m_ColorAttachments[i]);
					glTextureStorage2DMultisample(m_ColorAttachments[i], m_Spec.Samples, InternalFormatInOpenGL(m_Spec.Format), m_Spec.Width, m_Spec.Height, GL_TRUE);
					glNamedFramebufferTexture(m_ID, GL_COLOR_ATTACHMENT0 + i, m_ColorAttachments[i], 0);
				}
			}
		}
		if (static_cast<uint32_t>(m_Spec.Attachments) & static_cast<uint32_t>(FramebufferAttachments::DepthAttachment))
		{
			if (m_Spec.Samples == 1)
			{
				glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
				glTextureStorage2D(m_DepthAttachment, 1, GL_DEPTH_COMPONENT16, m_Spec.Width, m_Spec.Height);
				glNamedFramebufferTexture(m_ID, GL_DEPTH_ATTACHMENT, m_DepthAttachment, 0);
			}
			else
			{
				glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &m_DepthAttachment);
				glTextureStorage2DMultisample(m_DepthAttachment, m_Spec.Samples, GL_DEPTH_COMPONENT16, m_Spec.Width, m_Spec.Height, GL_TRUE);
				glNamedFramebufferTexture(m_ID, GL_DEPTH_ATTACHMENT, m_DepthAttachment, 0);
			}
		}

		SB_CORE_ASSERT(glCheckNamedFramebufferStatus(m_ID, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");
	}

	GLenum OpenGLFramebuffer::InternalFormatInOpenGL(FramebufferFormat format)
	{
		switch (format)
		{
			case FramebufferFormat::None:
				return 0;
			case FramebufferFormat::R8:
				return GL_R8;
			case FramebufferFormat::R16:
				return GL_R16;
			case FramebufferFormat::R16F:
				return GL_R16F;
			case FramebufferFormat::R32F:
				return GL_R32F;
			case FramebufferFormat::RG8:
				return GL_RG8;
			case FramebufferFormat::RG16:
				return GL_RG16;
			case FramebufferFormat::RG16F:
				return GL_RG16F;
			case FramebufferFormat::RG32F:
				return GL_RG32F;
			case FramebufferFormat::RGB8:
				return GL_RGB8;
			case FramebufferFormat::RGB16:
				return GL_RGB16;
			case FramebufferFormat::RGB16F:
				return GL_RGB16F;
			case FramebufferFormat::RGB32F:
				return GL_RGB32F;
			case FramebufferFormat::SRGB:
				return GL_SRGB8;
			case FramebufferFormat::RGBA8:
				return GL_RGBA8;
			case FramebufferFormat::RGBA16:
				return GL_RGBA16;
			case FramebufferFormat::RGBA16F:
				return GL_RGBA16F;
			case FramebufferFormat::RGBA32F:
				return GL_RGBA32F;
			case FramebufferFormat::SRGB_ALPHA:
				return GL_SRGB8_ALPHA8;
		}
		SB_CORE_ASSERT(false, "Unknown format type!");
		return 0;
	}
}
