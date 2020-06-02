#include "pch.h"
#include "OpenGLFramebuffer.h"
#include "OpenGLTexture.h"
#include "OpenGLRenderbuffer.h"

#include <glad\glad.h>

namespace Saba {

	OpenGLFramebuffer::OpenGLFramebuffer()
	{
		glCreateFramebuffers(1, &m_ID);
	}
	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_ID);
	}

	void OpenGLFramebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	}
	void OpenGLFramebuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLFramebuffer::BindRead() const
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_ID);
	}
	void OpenGLFramebuffer::UnbindRead() const
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	}
	void OpenGLFramebuffer::BindWrite() const
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_ID);
	}
	void OpenGLFramebuffer::UnbindWrite() const
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::AttachTexture(Ref<Texture2D> texture, Attachment attachment)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, AttachmentInOpenGL(attachment), GL_TEXTURE_2D, dynamic_cast<OpenGLTexture2D*>(&*texture)->m_ID, 0);
	}
	void OpenGLFramebuffer::AttachRenderbuffer(Ref<Renderbuffer> renderbuffer, Attachment attachment)
	{
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, AttachmentInOpenGL(attachment), GL_RENDERBUFFER, dynamic_cast<OpenGLRenderbuffer*>(&*renderbuffer)->m_ID);
	}

	uint32_t OpenGLFramebuffer::AttachmentInOpenGL(Attachment attachment)
	{
		switch (attachment)
		{
			case Saba::Framebuffer::Attachment::None:
				return 0;
			case Saba::Framebuffer::Attachment::Color0:
				return GL_COLOR_ATTACHMENT0;
			case Saba::Framebuffer::Attachment::Color1:
				return GL_COLOR_ATTACHMENT1;
			case Saba::Framebuffer::Attachment::Color2:
				return GL_COLOR_ATTACHMENT2;
			case Saba::Framebuffer::Attachment::Color3:
				return GL_COLOR_ATTACHMENT3;
			case Saba::Framebuffer::Attachment::Color4:
				return GL_COLOR_ATTACHMENT4;
			case Saba::Framebuffer::Attachment::Color5:
				return GL_COLOR_ATTACHMENT5;
			case Saba::Framebuffer::Attachment::Color6:
				return GL_COLOR_ATTACHMENT6;
			case Saba::Framebuffer::Attachment::Color7:
				return GL_COLOR_ATTACHMENT7;
			case Saba::Framebuffer::Attachment::Color8:
				return GL_COLOR_ATTACHMENT8;
			case Saba::Framebuffer::Attachment::Color9:
				return GL_COLOR_ATTACHMENT9;
			case Saba::Framebuffer::Attachment::Color10:
				return GL_COLOR_ATTACHMENT10;
			case Saba::Framebuffer::Attachment::Color11:
				return GL_COLOR_ATTACHMENT11;
			case Saba::Framebuffer::Attachment::Color12:
				return GL_COLOR_ATTACHMENT12;
			case Saba::Framebuffer::Attachment::Color13:
				return GL_COLOR_ATTACHMENT13;
			case Saba::Framebuffer::Attachment::Color14:
				return GL_COLOR_ATTACHMENT14;
			case Saba::Framebuffer::Attachment::Color15:
				return GL_COLOR_ATTACHMENT15;
			case Saba::Framebuffer::Attachment::Color16:
				return GL_COLOR_ATTACHMENT16;
			case Saba::Framebuffer::Attachment::Color17:
				return GL_COLOR_ATTACHMENT17;
			case Saba::Framebuffer::Attachment::Color18:
				return GL_COLOR_ATTACHMENT18;
			case Saba::Framebuffer::Attachment::Color19:
				return GL_COLOR_ATTACHMENT19;
			case Saba::Framebuffer::Attachment::Color20:
				return GL_COLOR_ATTACHMENT20;
			case Saba::Framebuffer::Attachment::Color21:
				return GL_COLOR_ATTACHMENT21;
			case Saba::Framebuffer::Attachment::Color22:
				return GL_COLOR_ATTACHMENT22;
			case Saba::Framebuffer::Attachment::Color23:
				return GL_COLOR_ATTACHMENT23;
			case Saba::Framebuffer::Attachment::Color24:
				return GL_COLOR_ATTACHMENT24;
			case Saba::Framebuffer::Attachment::Color25:
				return GL_COLOR_ATTACHMENT25;
			case Saba::Framebuffer::Attachment::Color26:
				return GL_COLOR_ATTACHMENT26;
			case Saba::Framebuffer::Attachment::Color27:
				return GL_COLOR_ATTACHMENT27;
			case Saba::Framebuffer::Attachment::Color28:
				return GL_COLOR_ATTACHMENT28;
			case Saba::Framebuffer::Attachment::Color29:
				return GL_COLOR_ATTACHMENT29;
			case Saba::Framebuffer::Attachment::Color30:
				return GL_COLOR_ATTACHMENT30;
			case Saba::Framebuffer::Attachment::Color31:
				return GL_COLOR_ATTACHMENT31;
			case Saba::Framebuffer::Attachment::Depth:
				return GL_DEPTH_ATTACHMENT;
			case Saba::Framebuffer::Attachment::Stencil:
				return GL_STENCIL_ATTACHMENT;
			case Saba::Framebuffer::Attachment::DepthStencil:
				return GL_DEPTH_STENCIL_ATTACHMENT;
		}
		return 0;
	}
}
