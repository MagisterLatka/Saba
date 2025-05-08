#pragma once

#include "Saba/Core/Core.h"

namespace Saba {

class RenderTarget : public RefCounted {
public:
    enum Format : uint8_t {
        R8, R32F, R32_UINT,
        RG8, RG16F, RG32F, RG32_UINT,
        RGB8, RGB32F, RGB32_UINT,
        RGBA8, RGBA16F, RGBA32F, RGBA32_UINT,
        Depth32F, Depth24Stencil8,
        First = R8, Last = Depth24Stencil8
    };
    enum Usage : uint8_t {
        ColorAttachment = 0, DepthStencilAttachment = 1,
        CopySource = BIT(1),
        TextureAttachment = BIT(2),
        InputAttachment = BIT(3)
    };
    union ClearValue {
        glm::vec4 floatVal;
        glm::ivec4 intVal;
        glm::uvec4 uintVal;
        struct depthStencilVal {
            float depthVal;
            uint32_t stencilVal;
        };
    };
public:
    virtual ~RenderTarget() = default;

    virtual uint64_t GetRawTexturePointer() const noexcept = 0; //TODO
    virtual void ReadPixel(void* data, uint32_t bufferSize, uint32_t xCoord, uint32_t yCoord) = 0; //TODO

    virtual uint32_t GetWidth() const noexcept = 0;
    virtual uint32_t GetHeight() const noexcept = 0;

    SB_CORE static Ref<RenderTarget> Create(uint32_t width, uint32_t height, Format format = Format::RGBA8,
        Usage usage = Usage::ColorAttachment, ClearValue clearValue = { glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) });
};

} //namespace Saba
