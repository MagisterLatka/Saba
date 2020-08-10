#pragma once

#include "Saba/Core.h"

namespace Saba {

	enum class Format {
		None,
		R8, R16, R16F, R32F,
		RG8, RG16, RG16F, RG32F,
		RGB8, RGB16, RGB16F, RGB32F, SRGB,
		RGBA8, RGBA16, RGBA16F, RGBA32F, SRGB_ALPHA
	};
	enum class Filter
	{
		None, Linear, Nearest, Linear_MipLinear, Nearest_MipLinear, Linear_MipNearest, Nearest_MipNearest
	};
	enum class WrapMode
	{
		None, Repeat, MirroredRepeat, ClampToEdge, ClampToBorder
	};

	struct TextureData
	{
		std::string Filepath = "";
		uint32_t Width = 0, Height = 0;
		uint8_t MipmapLevels = 1;

		Format TexFormat = Format::None;
		Filter MinificationFilter = Filter::None;
		Filter MagnificationFilter = Filter::None;
		WrapMode TexWrapMode = WrapMode::None;
	};

	class Texture
	{
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
		static Ref<Texture2D> Create(const TextureData& data);
		static Ref<Texture2D> Create(const std::string& filepath);
		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
	};

}
