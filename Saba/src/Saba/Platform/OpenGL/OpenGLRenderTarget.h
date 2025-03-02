#pragma once

#include "Saba/Renderer/RenderTarget.h"

namespace Saba {

class OpenGLRenderTarget : public RenderTarget {
public:
    SB_CORE OpenGLRenderTarget(uint32_t width, uint32_t height, RenderTargetFormat format);
    SB_CORE virtual ~OpenGLRenderTarget();

    SB_CORE void Bind() const noexcept override;
    SB_CORE void Clear(const glm::vec4& clearVal) noexcept override;
    SB_CORE void Resize(uint32_t width, uint32_t height, bool forceResize = false) override;

    SB_CORE void BindTexture(uint32_t slot = 0u) const noexcept override;
    SB_CORE void* GetRawTexturePointer() const noexcept override { return (void*)(static_cast<uint64_t>(m_Texture)); }
private:
    uint32_t m_ID = 0u, m_Texture = 0u, m_Width = 0u, m_Height = 0u;
    RenderTargetFormat m_Format;
};

}
