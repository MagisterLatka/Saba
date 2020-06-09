#pragma once

namespace Saba {

	class Texture
	{
	public:
		enum class Format
		{
			None = 0, R8 = BIT(0), R16 = BIT(1), R16F = BIT(2), R32F = BIT(3),
			RG8 = BIT(4), RG16 = BIT(5), RG16F = BIT(6), RG32F = BIT(7),
			RGB8 = BIT(8), RGB16 = BIT(9), RGB16F = BIT(10), RGB32F = BIT(11), SRGB = BIT(12),
			RGBA8 = BIT(13), RGBA16 = BIT(14), RGBA16F = BIT(15), RGBA32F = BIT(16), SRGBA = BIT(17)
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
		static Ref<Texture2D> Create(const std::string& filepath, Format format);
		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
		static Ref<Texture2D> Create(uint32_t width, uint32_t height, Format format);
	};

}
