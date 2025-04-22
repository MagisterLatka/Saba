#pragma once

#include "Saba/Renderer/RenderTarget.h"

namespace Saba {

class RenderPass : public RefCounted
{
public:
    virtual ~RenderPass() = default;

    virtual void SetRenderTarget(uint32_t index = 0, Ref<RenderTarget> renderTarget = {}) = 0;
    virtual void SetDepthStencilTarget(Ref<RenderTarget> depthStencilTarget = {}) = 0;
    virtual Ref<RenderTarget> GetRenderTarget(uint32_t index = 0) const = 0;
    virtual Ref<RenderTarget> GetDepthStencilTarget() const noexcept = 0;

    virtual uint32_t GetWidth() const noexcept = 0;
    virtual uint32_t GetHeight() const noexcept = 0;

    virtual void Bind() const noexcept = 0;
    virtual void Clear() noexcept = 0;
    virtual void Resize(uint32_t width, uint32_t height) = 0;

    SB_CORE static Ref<RenderPass> Create();
};

} //namespace Saba
