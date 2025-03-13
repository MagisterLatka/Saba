#pragma once

#include "Saba/Renderer/RenderTarget.h"
#include <d3d11.h>

namespace Saba {

class DX11RenderTarget : public RenderTarget
{
    friend class DX11RenderPass;
public:
    SB_CORE DX11RenderTarget(uint32_t width, uint32_t height, RenderTargetFormat format, glm::vec4 clearVal);
    SB_CORE virtual ~DX11RenderTarget() = default;

    SB_CORE void Resize(uint32_t width, uint32_t height, bool forceResize = false) override;

    SB_CORE void BindTexture(uint32_t slot = 0u) const override;
    SB_CORE void* GetRawTexturePointer() const noexcept override { return m_View.Get(); }
    SB_CORE void ReadPixel(void* data, uint32_t bufferSize, uint32_t xCoord, uint32_t yCoord) override;

    SB_CORE uint32_t GetWidth() const noexcept override { return m_Width; }
    SB_CORE uint32_t GetHeight() const noexcept override { return m_Height; }
private:
    SB_CORE void SetClearValue(glm::vec4 clearVal) noexcept override { m_ClearValue = clearVal; }
    SB_CORE void SetDepthStencilClearValue(float depth, uint8_t stencil) noexcept override { m_DepthClearValue = depth; m_StencilClearValue = stencil; }
    SB_CORE void Clear() const noexcept override;
private:
    ComPtr<ID3D11Texture2D> m_Texture, m_ReadBuffer;
    ComPtr<ID3D11RenderTargetView> m_RenderTarget;
    ComPtr<ID3D11DepthStencilView> m_DepthStencil;
    ComPtr<ID3D11ShaderResourceView> m_View;
    uint32_t m_Width = 0, m_Height = 0;
    RenderTargetFormat m_Format;
    glm::vec4 m_ClearValue;
    float m_DepthClearValue = 1.0f;
    uint8_t m_StencilClearValue = 0u;
};
}
