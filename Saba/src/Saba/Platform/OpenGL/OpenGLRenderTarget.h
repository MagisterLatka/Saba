#pragma once

#include "Saba/Renderer/RenderTarget.h"

namespace Saba {

class OpenGLRenderTarget : public RenderTarget {
    friend class OpenGLRenderPass;
public:
    SB_CORE OpenGLRenderTarget(uint32_t width, uint32_t height, RenderTargetFormat format, glm::vec4 clearVal);
    SB_CORE virtual ~OpenGLRenderTarget();

    SB_CORE void Resize(uint32_t width, uint32_t height, bool forceResize = false) override;

    SB_CORE void BindTexture(uint32_t slot = 0u) const override;
    SB_CORE void* GetRawTexturePointer() const noexcept override { return (void*)(static_cast<uint64_t>(m_ID)); }
    SB_CORE void ReadPixel(void* data, uint32_t xCoord, uint32_t yCoord) override;

    SB_CORE uint32_t GetWidth() const noexcept override { return m_Width; }
    SB_CORE uint32_t GetHeight() const noexcept override { return m_Height; }
private:
    SB_CORE void SetClearValue(glm::vec4 clearVal) noexcept override { m_ClearValue = clearVal; }
    SB_CORE void SetDepthStencilClearValue(float depth, uint8_t stencil) noexcept override { m_DepthClearValue = depth; m_StencilClearValue = stencil; }
    SB_CORE void Clear() const noexcept override {}
private:
    uint32_t m_ID = 0u, m_ReadBuffer = 0u, m_Width = 0u, m_Height = 0u;
    RenderTargetFormat m_Format;
    glm::vec4 m_ClearValue;
    float m_DepthClearValue = 1.0f;
    uint8_t m_StencilClearValue = 0u;
};

}
