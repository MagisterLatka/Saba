#pragma once

#include "Saba/Renderer/RenderPass.h"
#include "Saba/Platform/OpenGL/OpenGLRenderTarget.h"

namespace Saba {

class OpenGLRenderPass : public RenderPass
{
public:
    OpenGLRenderPass();
    virtual ~OpenGLRenderPass();

    void SetRenderTarget(uint32_t index, Ref<RenderTarget> renderTarget) override;
    void SetDepthStencilTarget(Ref<RenderTarget> depthStencilTarget) override;
    Ref<RenderTarget> GetRenderTarget(uint32_t index) const override {
        SB_CORE_ASSERT(index < 8, "Exceeding available number of simultaneous render targets");
        return m_RenderTargets[index];
    }
    Ref<RenderTarget> GetDepthStencilTarget() const noexcept override { return m_DepthStencilTarget; }

    void Bind() const noexcept override;
    void Clear() noexcept override;
    void Resize(uint32_t width, uint32_t height) override;

    uint32_t GetWidth() const noexcept override { return m_Width; }
    uint32_t GetHeight() const noexcept override { return m_Height; }
private:
    void Create();
private:
    std::array<Ref<OpenGLRenderTarget>, 8> m_RenderTargets;
    Ref<OpenGLRenderTarget> m_DepthStencilTarget;

    uint32_t m_ID = 0u, m_Width = 0u, m_Height = 0u;
};

} //namespace Saba
