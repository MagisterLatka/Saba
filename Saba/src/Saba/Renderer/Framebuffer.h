#pragma once

#include "Texture.h"
#include "Renderbuffer.h"

namespace Saba {

	class Framebuffer
	{
	public:
		enum class Attachment : uint64_t
		{
			None = 0, Color0 = BIT(0), Color1 = BIT(1), Color2 = BIT(2), Color3 = BIT(3), Color4 = BIT(4), Color5 = BIT(5), Color6 = BIT(6), Color7 = BIT(7), Color8 = BIT(8), Color9 = BIT(9),
			Color10 = BIT(10), Color11 = BIT(11), Color12 = BIT(12), Color13 = BIT(13), Color14 = BIT(14), Color15 = BIT(15), Color16 = BIT(16), Color17 = BIT(17), Color18 = BIT(18), Color19 = BIT(19),
			Color20 = BIT(20), Color21 = BIT(21), Color22 = BIT(22), Color23 = BIT(23), Color24 = BIT(24), Color25 = BIT(25), Color26 = BIT(26), Color27 = BIT(27), Color28 = BIT(28), Color29 = BIT(29),
			Color30 = BIT(30), Color31 = (uint64_t)1 << 31, Depth = (uint64_t)1 << 32, Stencil = (uint64_t)1 << 33, DepthStencil = (uint64_t)1 << 34
		};
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void BindRead() const = 0;
		virtual void UnbindRead() const = 0;
		virtual void BindWrite() const = 0;
		virtual void UnbindWrite() const = 0;

		virtual void DrawMode(Attachment attachments) = 0;
		virtual void ReadMode(Attachment attachments) = 0;

		virtual void AttachTexture(Ref<Texture2D> texture, Attachment attachment) = 0;
		virtual void AttachTexture(Ref<TextureCubemap> texture, Attachment attachment) = 0;
		virtual void AttachRenderbuffer(Ref<Renderbuffer> renderbuffer, Attachment attachment) = 0;

		static Ref<Framebuffer> Create();
	};
}
