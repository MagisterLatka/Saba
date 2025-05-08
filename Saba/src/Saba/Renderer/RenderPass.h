#pragma once

#include "Saba/Renderer/RenderTarget.h"

namespace Saba {

class RenderSubpass : public RefCounted {
public:
public:
    RenderSubpass(std::vector<Ref<RenderTarget>> renderTargets, Ref<RenderTarget> depthStencilTarget = {},
        std::vector<Ref<RenderTarget>> inputTargets = {})
        : m_RenderTargets(std::move(renderTargets)), m_InputTargets(std::move(inputTargets)),
          m_DepthStencilTarget(std::move(depthStencilTarget)) {}

    const std::vector<Ref<RenderTarget>>& GetRenderTargets() const noexcept { return m_RenderTargets; }
    const std::vector<Ref<RenderTarget>>& GetInputTargets() const noexcept { return m_InputTargets; }
    Ref<RenderTarget> GetDepthStencilTarget() const noexcept { return m_DepthStencilTarget; }
private:
    std::vector<Ref<RenderTarget>> m_RenderTargets, m_InputTargets;
    Ref<RenderTarget> m_DepthStencilTarget;
};

class RenderPass : public RefCounted {
public:
    virtual ~RenderPass() = default;

    virtual Ref<RenderTarget> GetRenderTarget(uint32_t index = 0) const = 0;
    virtual Ref<RenderTarget> GetDepthStencilTarget() const noexcept = 0;

    virtual uint32_t GetWidth() const noexcept = 0;
    virtual uint32_t GetHeight() const noexcept = 0;

    virtual void Bind() const noexcept = 0;
    virtual void Clear() noexcept = 0;
    virtual void Resize(uint32_t width, uint32_t height) = 0;

    SB_CORE static Ref<RenderPass> Create(const std::vector<Ref<RenderSubpass>>& subpasses);
};

} //namespace Saba
