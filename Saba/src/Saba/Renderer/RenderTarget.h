#pragma once

#include "Saba/Core/Core.h"

namespace Saba {

enum class RenderTargetFormat : uint8_t {
    None = 0,
    R8, R32F, R32_UINT,
    RG8, RG16F, RG32F, RG32_UINT,
    RGB8, RGB32F, RGB32_UINT,
    RGBA8, RGBA16F, RGBA32F, RGBA32_UINT,
    Depth32F = 0x10, Depth24Stencil8 = 0x20,
    Last = Depth24Stencil8
};
class RenderTarget : public RefCounted {
public:
    virtual ~RenderTarget() = default;

    virtual void Resize(uint32_t width, uint32_t height, bool forceResize = false) = 0;

    virtual void BindTexture(uint32_t slot = 0u) const = 0;
    virtual uint64_t GetRawTexturePointer() const noexcept = 0;
    virtual void ReadPixel(void* data, uint32_t bufferSize, uint32_t xCoord, uint32_t yCoord) = 0;

    virtual uint32_t GetWidth() const noexcept = 0;
    virtual uint32_t GetHeight() const noexcept = 0;

    SB_CORE static Ref<RenderTarget> Create(uint32_t width, uint32_t height, RenderTargetFormat format = RenderTargetFormat::RGBA8,
        glm::vec4 clearVal = { 0.0f, 0.0f, 0.0f, 1.0f});
protected:
    virtual void SetClearValue(glm::vec4 clearVal) noexcept = 0;
    virtual void SetDepthStencilClearValue(float depth, uint8_t = 0u) noexcept = 0;
    virtual void Clear() const noexcept = 0;
};

} //namespace Saba
