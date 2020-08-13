#pragma once

#include "Saba/Core.h"

namespace Saba {

	enum FramebufferAttachments
	{
		NoneAttachment = 0,
		ColorAttachment0 = BIT(0), ColorAttachment1 = BIT(1), ColorAttachment2 = BIT(2), ColorAttachment3 = BIT(3), ColorAttachment4 = BIT(4), ColorAttachment5 = BIT(5),
		ColorAttachment6 = BIT(6), ColorAttachment7 = BIT(7), DepthAttachment = BIT(8)
	};
	enum class FramebufferFormat {
		None,
		R8, R16, R16F, R32F,
		RG8, RG16, RG16F, RG32F,
		RGB8, RGB16, RGB16F, RGB32F, SRGB,
		RGBA8, RGBA16, RGBA16F, RGBA32F, SRGB_ALPHA
	};

	struct FramebufferSpec
	{
		uint32_t Width, Height;
		FramebufferAttachments Attachments;
		FramebufferFormat Format;
		uint8_t Samples = 1;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual void* GetAttachmentID(int attachment) const = 0;
		virtual const FramebufferSpec& GetSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpec& spec);
	};
}
