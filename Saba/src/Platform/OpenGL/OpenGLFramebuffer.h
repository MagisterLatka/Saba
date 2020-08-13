#pragma once

#include "Saba/Renderer/Framebuffer.h"
#include <glad/glad.h>

namespace Saba {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpec& spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual void* GetAttachmentID(int attachment) const override { SB_CORE_ASSERT(0 <= attachment && attachment <= 8, "Invalid attachment id!");
																		if (attachment == 8) return (void*)(uint64_t)m_DepthAttachment; else return (void*)(uint64_t)m_ColorAttachments[attachment]; }
		virtual const FramebufferSpec& GetSpecification() const override { return m_Spec; }
	private:
		static GLenum InternalFormatInOpenGL(FramebufferFormat format);
	private:
		uint32_t m_ID = 0;
		std::array<uint32_t, 8> m_ColorAttachments = { 0 };
		uint32_t m_DepthAttachment = 0;
		FramebufferSpec m_Spec;
	};
}
