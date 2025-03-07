#pragma once

#include "Saba/Renderer/RenderPass.h"
#include "Saba/Platform/DX11/DX11RenderTarget.h"

namespace Saba {

class DX11RenderPass : public RenderPass
{
public:
    SB_CORE DX11RenderPass() = default;
    SB_CORE virtual ~DX11RenderPass() = default;

    SB_CORE void SetRenderTarget(uint32_t index, Ref<RenderTarget> renderTarget) override;
    SB_CORE void SetDepthStencilTarget(Ref<RenderTarget> depthStencilTarget) override;
    SB_CORE Ref<RenderTarget> GetRenderTarget(uint32_t index) const override {
        SB_CORE_ASSERT(index < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, "Exceeding available number of simultaneous render targets");
        return m_RenderTargets[index];
    }
    SB_CORE Ref<RenderTarget> GetDepthStencilTarget() const noexcept override { return m_DepthStencilTarget; }

    SB_CORE void Bind() const noexcept override;
    SB_CORE void Clear(const glm::vec4& clearVal, float depth, uint8_t stencil) noexcept override;
    SB_CORE void Resize(uint32_t width, uint32_t height) override;

    SB_CORE uint32_t GetWidth() const noexcept override { return m_Width; }
    SB_CORE uint32_t GetHeight() const noexcept override { return m_Height; }
private:
    std::array<Ref<DX11RenderTarget>, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT> m_RenderTargets;
    Ref<DX11RenderTarget> m_DepthStencilTarget;

    uint32_t m_Width = 0u, m_Height = 0u;
};

}
