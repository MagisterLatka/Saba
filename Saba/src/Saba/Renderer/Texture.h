#pragma once

#include "Saba/Core/Core.h"

namespace Saba {

enum class TextureFormat : uint8_t {
    None = 0,
    R8, R32F,
    RGB8, RGB32F,
    RGBA8, RGBA32F,
    Last = RGBA32F
};
struct Texture2DProps {
    uint32_t Width = 0u, Height = 0u;
    std::filesystem::path Filepath;
    TextureFormat Format;
    bool GenerateMipMaps = false;
    bool UseSRGB = false;
};

class Texture2D : public RefCounted
{
public:
    SB_CORE virtual ~Texture2D() = default;

    SB_CORE virtual void Bind(uint32_t slot) const noexcept = 0;

    SB_CORE virtual const Texture2DProps& GetProps() const noexcept = 0;
    SB_CORE virtual uint32_t GetWidth() const noexcept = 0;
    SB_CORE virtual uint32_t GetHeight() const noexcept = 0;
    SB_CORE virtual void* GetRawPointer() const noexcept = 0;

    SB_CORE static Ref<Texture2D> Create(Texture2DProps props);
    SB_CORE static Ref<Texture2D> Create(uint32_t width, uint32_t height, void* data = nullptr, TextureFormat format = TextureFormat::RGBA8);

    SB_CORE static uint32_t GetBPP(TextureFormat format);
    SB_CORE static uint32_t CalcNrMips(uint32_t width, uint32_t height);
    SB_CORE static void* Decode(const void* data, uint32_t size, uint32_t& width, uint32_t& height);
};

}
