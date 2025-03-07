#pragma once

#include "Saba/Renderer/RenderTarget.h"

namespace Saba {

class RenderPass : public RefCounted
{
public:
    SB_CORE virtual ~RenderPass() = default;

    SB_CORE virtual void SetRenderTarget(uint32_t index = 0, Ref<RenderTarget> renderTarget = {}) = 0;
    SB_CORE virtual void SetDepthStencilTarget(Ref<RenderTarget> depthStencilTarget = {}) = 0;
    SB_CORE virtual Ref<RenderTarget> GetRenderTarget(uint32_t index = 0) const = 0;
    SB_CORE virtual Ref<RenderTarget> GetDepthStencilTarget() const noexcept = 0;

    SB_CORE virtual uint32_t GetWidth() const noexcept = 0;
    SB_CORE virtual uint32_t GetHeight() const noexcept = 0;

    SB_CORE virtual void Bind() const noexcept = 0;
    SB_CORE virtual void Clear(const glm::vec4& clearVal = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), float depth = 0.0f, uint8_t stencil = 0u) noexcept = 0;
    SB_CORE virtual void Resize(uint32_t width, uint32_t height) = 0;

    SB_CORE static Ref<RenderPass> Create();
};

}
