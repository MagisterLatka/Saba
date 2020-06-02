#pragma once

namespace Saba {

	class Renderbuffer
	{
	public:
		enum class Format {
			None = 0, RGB, RGBA, RGB16, RGBA16, DEPTH, DEPTH16, DEPTH24, DEPTH32, DEPTH24STENCIL8
		};
	public:
		virtual ~Renderbuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Ref<Renderbuffer> Create(uint32_t width, uint32_t height, Format format);
	};
}
