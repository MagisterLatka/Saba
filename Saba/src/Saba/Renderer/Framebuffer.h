#pragma once

#include "Texture.h"
#include "Renderbuffer.h"

namespace Saba {

	class Framebuffer
	{
	public:
		enum class Attachment
		{
			None = 0, Color0, Color1, Color2, Color3, Color4, Color5, Color6, Color7, Color8, Color9, Color10,
			Color11, Color12, Color13, Color14, Color15, Color16, Color17, Color18, Color19, Color20,
			Color21, Color22, Color23, Color24, Color25, Color26, Color27, Color28, Color29, Color30,
			Color31, Depth, Stencil, DepthStencil
		};
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void BindRead() const = 0;
		virtual void UnbindRead() const = 0;
		virtual void BindWrite() const = 0;
		virtual void UnbindWrite() const = 0;

		virtual void AttachTexture(Ref<Texture2D> texture, Attachment attachment) = 0;
		virtual void AttachRenderbuffer(Ref<Renderbuffer> renderbuffer, Attachment attachment) = 0;

		static Ref<Framebuffer> Create();
	};
}
