#pragma once

#include "Saba/Renderer/RenderTarget.h"

namespace Saba {

class OpenGLRenderTarget : public RenderTarget {
    friend class OpenGLRenderPass;
public:
    SB_CORE OpenGLRenderTarget(uint32_t width, uint32_t height, RenderTargetFormat format);
    SB_CORE virtual ~OpenGLRenderTarget();

    SB_CORE void Resize(uint32_t width, uint32_t height, bool forceResize = false) override;

    SB_CORE void BindTexture(uint32_t slot = 0u) const noexcept override;
    SB_CORE void* GetRawTexturePointer() const noexcept override { return (void*)(static_cast<uint64_t>(m_ID)); }

    SB_CORE uint32_t GetWidth() const noexcept override { return m_Width; }
    SB_CORE uint32_t GetHeight() const noexcept override { return m_Height; }
private:
    SB_CORE void Clear([[maybe_unused]] const glm::vec4& clearVal = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
        [[maybe_unused]] float depth = 0.0f, [[maybe_unused]] uint8_t stencil = 0u) const noexcept override {}
private:
    uint32_t m_ID = 0u, m_Width = 0u, m_Height = 0u;
    RenderTargetFormat m_Format;
};

}
