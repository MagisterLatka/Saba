#pragma once

#include "Saba/Renderer/RenderPass.h"
#include "Saba/Platform/OpenGL/OpenGLRenderTarget.h"

namespace Saba {

class OpenGLRenderPass : public RenderPass
{
public:
    SB_CORE OpenGLRenderPass();
    SB_CORE virtual ~OpenGLRenderPass();

    SB_CORE void SetRenderTarget(uint32_t index, Ref<RenderTarget> renderTarget) override;
    SB_CORE void SetDepthStencilTarget(Ref<RenderTarget> depthStencilTarget) override;
    SB_CORE Ref<RenderTarget> GetRenderTarget(uint32_t index) const override {
        SB_CORE_ASSERT(index < 8, "Exceeding available number of simultaneous render targets");
        return m_RenderTargets[index];
    }
    SB_CORE Ref<RenderTarget> GetDepthStencilTarget() const noexcept override { return m_DepthStencilTarget; }

    SB_CORE void Bind() const noexcept override;
    SB_CORE void Clear() noexcept override;
    SB_CORE void Resize(uint32_t width, uint32_t height) override;

    SB_CORE uint32_t GetWidth() const noexcept override { return m_Width; }
    SB_CORE uint32_t GetHeight() const noexcept override { return m_Height; }
private:
    SB_CORE void Create();
private:
    std::array<Ref<OpenGLRenderTarget>, 8> m_RenderTargets;
    Ref<OpenGLRenderTarget> m_DepthStencilTarget;

    uint32_t m_ID = 0u, m_Width = 0u, m_Height = 0u;
};

}
