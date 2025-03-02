#pragma once

#include "Saba/Core/Core.h"

namespace Saba {

enum class RenderTargetFormat : uint8_t {
    None = 0,
    R8, R32F,
    RG8, RG16F, RG32F,
    RGB8, RGB32F,
    RGBA8, RGBA16F, RGBA32F,
    Last = RGBA32F
};
class RenderTarget : public RefCounted {
public:
    SB_CORE virtual ~RenderTarget() = default;

    SB_CORE virtual void Bind() const noexcept = 0; //TODO move to render pass
    SB_CORE virtual void Clear(const glm::vec4& clearVal = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) noexcept = 0;
    SB_CORE virtual void Resize(uint32_t width, uint32_t height, bool forceResize = false) = 0;

    SB_CORE virtual void BindTexture(uint32_t slot = 0u) const noexcept = 0;
    SB_CORE virtual void* GetRawTexturePointer() const noexcept = 0;

    SB_CORE static Ref<RenderTarget> Create(uint32_t width, uint32_t height, RenderTargetFormat format = RenderTargetFormat::RGBA8);
};

}
