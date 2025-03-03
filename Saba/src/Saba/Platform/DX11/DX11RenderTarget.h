#pragma once

#include "Saba/Renderer/RenderTarget.h"
#include <d3d11.h>

namespace Saba {

class DX11RenderTarget : public RenderTarget
{
    friend class DX11RenderPass;
public:
    SB_CORE DX11RenderTarget(uint32_t width, uint32_t height, RenderTargetFormat format);
    SB_CORE virtual ~DX11RenderTarget() = default;

    SB_CORE void Resize(uint32_t width, uint32_t height, bool forceResize = false) override;

    SB_CORE void BindTexture(uint32_t slot = 0u) const noexcept override;
    SB_CORE void* GetRawTexturePointer() const noexcept override { return m_View.Get(); }
private:
    SB_CORE void Clear(const glm::vec4& clearVal = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), float depth = 0.0f, uint8_t stencil = 0u) const noexcept override;
private:
    ComPtr<ID3D11RenderTargetView> m_RenderTarget;
    ComPtr<ID3D11DepthStencilView> m_DepthStencil;
    ComPtr<ID3D11ShaderResourceView> m_View;
    uint32_t m_Width = 0, m_Height = 0;
    RenderTargetFormat m_Format;
};
}
