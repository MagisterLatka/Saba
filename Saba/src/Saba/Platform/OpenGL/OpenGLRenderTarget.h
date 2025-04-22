#pragma once

#include "Saba/Renderer/RenderTarget.h"

namespace Saba {

class OpenGLRenderTarget : public RenderTarget {
    friend class OpenGLRenderPass;
public:
    OpenGLRenderTarget(uint32_t width, uint32_t height, RenderTargetFormat format, glm::vec4 clearVal);
    virtual ~OpenGLRenderTarget();

    void Resize(uint32_t width, uint32_t height, bool forceResize = false) override;

    void BindTexture(uint32_t slot = 0u) const override;
    uint64_t GetRawTexturePointer() const noexcept override { return (static_cast<uint64_t>(m_ID)); }
    void ReadPixel(void* data, uint32_t bufferSize, uint32_t xCoord, uint32_t yCoord) override;

    uint32_t GetWidth() const noexcept override { return m_Width; }
    uint32_t GetHeight() const noexcept override { return m_Height; }
private:
    void SetClearValue(glm::vec4 clearVal) noexcept override { m_ClearValue = clearVal; }
    void SetDepthStencilClearValue(float depth, uint8_t stencil) noexcept override { m_DepthClearValue = depth; m_StencilClearValue = stencil; }
    void Clear() const noexcept override {}
private:
    uint32_t m_ID = 0u, m_ReadBuffer = 0u, m_Width = 0u, m_Height = 0u;
    RenderTargetFormat m_Format;
    glm::vec4 m_ClearValue;
    float m_DepthClearValue = 1.0f;
    uint8_t m_StencilClearValue = 0u;
};

} //namespace Saba
