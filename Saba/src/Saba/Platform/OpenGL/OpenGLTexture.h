#pragma once

#include "Saba/Renderer/Texture.h"
#include "Saba/Core/Buffer.h"

namespace Saba {

class OpenGLTexture2D : public Texture2D {
public:
    SB_CORE OpenGLTexture2D(Texture2DProps props);
    SB_CORE OpenGLTexture2D(uint32_t width, uint32_t height, void* data = nullptr, TextureFormat format = TextureFormat::RGBA8);
    SB_CORE virtual ~OpenGLTexture2D();

    SB_CORE void Bind(uint32_t slot) const noexcept override;

    SB_CORE const Texture2DProps& GetProps() const noexcept override { return m_Props; }
    SB_CORE uint32_t GetWidth() const noexcept override { return m_Props.Width; }
    SB_CORE uint32_t GetHeight() const noexcept override { return m_Props.Height; }
    SB_CORE void* GetRawPointer() const noexcept override { return (void*)(static_cast<uint64_t>(m_ID)); }

    SB_CORE bool IsLoaded() const noexcept override { return m_Loaded; }
private:
    SB_CORE void Init();
private:
    Texture2DProps m_Props;
    Buffer m_Buffer;
    uint32_t m_ID;
    bool m_Loaded;
};

}
