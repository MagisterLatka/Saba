#pragma once

namespace Saba {

	class Texture
	{
	public:
		enum class Format
		{
			None = 0, R8, R16, R16F, R32F,
			RG8, RG16, RG16F, RG32F,
			RGB8, RGB16, RGB16F, RGB32F, SRGB,
			RGBA8, RGBA16, RGBA16F, RGBA32F, SRGBA,
			Depth16, Depth24, Depth32, Depth24Stencil8
		};
		enum class Wrap
		{
			None = 0, ClampToEdge, ClampToBorder, Repeat, MirroredRepeat
		};
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;
				
		virtual void Bind(uint32_t slot = 0) = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& filepath);
		static Ref<Texture2D> Create(const std::string& filepath, Format format, Wrap wrap = Wrap::Repeat);
		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
		static Ref<Texture2D> Create(uint32_t width, uint32_t height, Format format, Wrap wrap = Wrap::Repeat);
	};

}
