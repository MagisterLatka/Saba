#pragma once

#include "Saba\Renderer\Renderbuffer.h"

namespace Saba {

	class OpenGLRenderbuffer : public Renderbuffer
	{
		friend class OpenGLFramebuffer;
	public:
		OpenGLRenderbuffer(uint32_t width, uint32_t height, Format format);
		virtual ~OpenGLRenderbuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
	private:
		static uint32_t FormatInOpenGL(Format format);
	private:
		uint32_t m_ID;
		uint32_t m_Width, m_Height;
	};
}
