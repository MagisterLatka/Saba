#pragma once

#include "Saba\Renderer\Framebuffer.h"

namespace Saba {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer();
		virtual ~OpenGLFramebuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void BindRead() const override;
		virtual void UnbindRead() const override;
		virtual void BindWrite() const override;
		virtual void UnbindWrite() const override;

		virtual void DrawMode(Attachment attachments) override;
		virtual void ReadMode(Attachment attachments) override;

		virtual void AttachTexture(Ref<Texture2D> texture, Attachment attachment) override;
		virtual void AttachTexture(Ref<TextureCubemap> texture, Attachment attachment) override;
		virtual void AttachRenderbuffer(Ref<Renderbuffer> renderbuffer, Attachment attachment) override;
	private:
		static uint32_t AttachmentInOpenGL(Attachment attachment);
	private:
		uint32_t m_ID;
	};
}
