#include "pch.h"
#include "OpenGLFramebuffer.h"
#include "OpenGLTexture.h"
#include "OpenGLRenderbuffer.h"
#include "OpenGLError.h"

#include <glad\glad.h>

namespace Saba {

	OpenGLFramebuffer::OpenGLFramebuffer()
	{
		GLCall(glCreateFramebuffers(1, &m_ID));
	}
	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_ID);
	}

	void OpenGLFramebuffer::Bind() const
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_ID));
	}
	void OpenGLFramebuffer::Unbind() const
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
	void OpenGLFramebuffer::BindRead() const
	{
		GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_ID));
	}
	void OpenGLFramebuffer::UnbindRead() const
	{
		GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, 0));
	}
	void OpenGLFramebuffer::BindWrite() const
	{
		GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_ID));
	}
	void OpenGLFramebuffer::UnbindWrite() const
	{
		GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
	}

	void OpenGLFramebuffer::DrawMode(Attachment attachments)
	{
		GLCall(glDrawBuffer(AttachmentInOpenGL(attachments)));
	}

	void OpenGLFramebuffer::ReadMode(Attachment attachments)
	{
		GLCall(glReadBuffer(AttachmentInOpenGL(attachments)));
	}

	void OpenGLFramebuffer::AttachTexture(Ref<Texture2D> texture, Attachment attachment)
	{
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, AttachmentInOpenGL(attachment), GL_TEXTURE_2D, dynamic_cast<OpenGLTexture2D*>(&*texture)->m_ID, 0));
	}
	void OpenGLFramebuffer::AttachRenderbuffer(Ref<Renderbuffer> renderbuffer, Attachment attachment)
	{
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, AttachmentInOpenGL(attachment), GL_RENDERBUFFER, dynamic_cast<OpenGLRenderbuffer*>(&*renderbuffer)->m_ID));
	}

	uint32_t OpenGLFramebuffer::AttachmentInOpenGL(Attachment attachment)
	{
		GLenum a = 0;
		if ((uint64_t)attachment & (uint64_t)Attachment::None) a |= GL_NONE;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color0) a |= GL_COLOR_ATTACHMENT0;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color1) a |= GL_COLOR_ATTACHMENT1;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color2) a |= GL_COLOR_ATTACHMENT2;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color3) a |= GL_COLOR_ATTACHMENT3;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color4) a |= GL_COLOR_ATTACHMENT4;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color5) a |= GL_COLOR_ATTACHMENT5;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color6) a |= GL_COLOR_ATTACHMENT6;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color7) a |= GL_COLOR_ATTACHMENT7;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color8) a |= GL_COLOR_ATTACHMENT8;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color9) a |= GL_COLOR_ATTACHMENT9;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color10) a |= GL_COLOR_ATTACHMENT10;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color11) a |= GL_COLOR_ATTACHMENT11;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color12) a |= GL_COLOR_ATTACHMENT12;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color13) a |= GL_COLOR_ATTACHMENT13;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color14) a |= GL_COLOR_ATTACHMENT14;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color15) a |= GL_COLOR_ATTACHMENT15;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color16) a |= GL_COLOR_ATTACHMENT16;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color17) a |= GL_COLOR_ATTACHMENT17;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color18) a |= GL_COLOR_ATTACHMENT18;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color19) a |= GL_COLOR_ATTACHMENT19;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color20) a |= GL_COLOR_ATTACHMENT20;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color21) a |= GL_COLOR_ATTACHMENT21;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color22) a |= GL_COLOR_ATTACHMENT22;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color23) a |= GL_COLOR_ATTACHMENT23;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color24) a |= GL_COLOR_ATTACHMENT24;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color25) a |= GL_COLOR_ATTACHMENT25;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color26) a |= GL_COLOR_ATTACHMENT26;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color27) a |= GL_COLOR_ATTACHMENT27;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color28) a |= GL_COLOR_ATTACHMENT28;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color29) a |= GL_COLOR_ATTACHMENT29;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color30) a |= GL_COLOR_ATTACHMENT30;
		if ((uint64_t)attachment & (uint64_t)Attachment::Color31) a |= GL_COLOR_ATTACHMENT31;
		if ((uint64_t)attachment & (uint64_t)Attachment::Depth) a |= GL_DEPTH_ATTACHMENT;
		if ((uint64_t)attachment & (uint64_t)Attachment::Stencil) a |= GL_STENCIL_ATTACHMENT;
		if ((uint64_t)attachment & (uint64_t)Attachment::DepthStencil) a |= GL_DEPTH_STENCIL_ATTACHMENT;
		return a;
	}
}
