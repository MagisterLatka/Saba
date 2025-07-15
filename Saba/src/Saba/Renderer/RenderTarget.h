#pragma once

#include "Saba/Core/Core.h"

namespace Saba {

class RenderTarget : public RefCounted {
public:
    enum class Format : uint8_t {
        R8, R32F, R32_UINT,
        RG8, RG16F, RG32F, RG32_UINT,
        RGB8, RGB32F, RGB32_UINT,
        RGBA8, RGBA16F, RGBA32F, RGBA32_UINT,
        Depth32F, Depth24Stencil8
    };
    enum Usage : uint8_t {
        NoneUsage = 0u,
        TransferSourceUsage = BIT(0), TransferDestinationUsage = BIT(1),
        TextureUsage = BIT(2), InputAttachmentUsage = BIT(3)
    };
    union ClearValue {
        glm::vec4 colorFloat;
        glm::ivec4 colorInt;
        glm::uvec4 colorUInt;
        struct {
            float depth;
            uint32_t stencil;
        } depthStencil;
    };
public:
    virtual ~RenderTarget() = default;

    virtual void Resize(uint32_t width, uint32_t height) = 0;

    virtual uint32_t GetWidth() const noexcept = 0;
    virtual uint32_t GetHeight() const noexcept = 0;
    virtual Format GetFormat() const noexcept = 0;
    virtual Usage GetUsage() const noexcept = 0;
    virtual const ClearValue& GetClearValue() const noexcept = 0;

    SB_CORE static Ref<RenderTarget> Create(uint32_t width, uint32_t height,
        Format format = Format::RGBA8, Usage usage = NoneUsage,
        ClearValue clearVal = {{ 0.0f, 0.0f, 0.0f, 1.0f }});
};

} //namespace Saba
