#include <pch.h>
#include "DX11RenderPass.h"

#include "Saba/Renderer/Renderer.h"
#include "Saba/Platform/DX11/DX11Context.h"

namespace Saba {

void DX11RenderPass::SetRenderTarget(uint32_t index, Ref<RenderTarget> renderTarget) {
    SB_CORE_ASSERT(index < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, "Exceeding available number of simultaneous render targets");
    m_RenderTargets[index] = renderTarget.As<DX11RenderTarget>();
    SB_CORE_ASSERT(!(static_cast<uint32_t>(m_RenderTargets[index]->m_Format) & 0x30u), "Render target cannot have depth or depth stencil format");
}
void DX11RenderPass::SetDepthStencilTarget(Ref<RenderTarget> depthStencilTarget) {
    m_DepthStencilTarget = depthStencilTarget.As<DX11RenderTarget>();
    if (m_DepthStencilTarget)
        SB_CORE_ASSERT(static_cast<uint32_t>(m_DepthStencilTarget->m_Format) & 0x30u, "Depth stencil target must have depth or depth stencil format");
}

void DX11RenderPass::Bind() const noexcept {
    Ref<const DX11RenderPass> instance = this;
    Renderer::Submit([instance]() {
        ID3D11RenderTargetView* views[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT] = { 0 };
        for (uint32_t i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
            if (instance->m_RenderTargets[i])
                views[i] = instance->m_RenderTargets[i]->m_RenderTarget.Get();
        }

        DX11Context::GetContextFromApplication()->GetContext()->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, views,
            instance->m_DepthStencilTarget ? instance->m_DepthStencilTarget->m_DepthStencil.Get() : nullptr);

        D3D11_VIEWPORT viewport;
        viewport.Width = static_cast<float>(instance->m_Width);
        viewport.Height = static_cast<float>(instance->m_Height);
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        DX11Context::GetContextFromApplication()->GetContext()->RSSetViewports(1u, &viewport);
    });
}

void DX11RenderPass::Clear(const glm::vec4& clearVal, float depth, uint8_t stencil) noexcept {
    for (const auto& renderTarget : m_RenderTargets) {
        if (renderTarget)
            renderTarget->Clear(clearVal);
    }
    if (m_DepthStencilTarget)
        m_DepthStencilTarget->Clear(glm::vec4(0.0f), depth, stencil);
}

void DX11RenderPass::Resize(uint32_t width, uint32_t height) {
    m_Width = width;
    m_Height = height;
    for (auto& renderTarget : m_RenderTargets) {
        if (renderTarget)
            renderTarget->Resize(m_Width, m_Height);
    }
    if (m_DepthStencilTarget)
        m_DepthStencilTarget->Resize(m_Width, m_Height);
}

}
