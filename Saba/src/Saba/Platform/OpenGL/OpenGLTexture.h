#pragma once

#include "Saba/Renderer/Texture.h"
#include "Saba/Core/Buffer.h"

namespace Saba {

class OpenGLTexture2D : public Texture2D {
public:
    OpenGLTexture2D(Texture2DProps props);
    OpenGLTexture2D(uint32_t width, uint32_t height, void* data = nullptr, TextureFormat format = TextureFormat::RGBA8);
    virtual ~OpenGLTexture2D();

    void Bind(uint32_t slot) const noexcept override;

    const Texture2DProps& GetProps() const noexcept override { return m_Props; }
    uint32_t GetWidth() const noexcept override { return m_Props.Width; }
    uint32_t GetHeight() const noexcept override { return m_Props.Height; }
    uint64_t GetRawPointer() const noexcept override { return static_cast<uint64_t>(m_ID); }

    bool IsLoaded() const noexcept override { return m_Loaded; }
private:
    void Init();
private:
    Texture2DProps m_Props;
    Buffer m_Buffer;
    uint32_t m_ID;
    bool m_Loaded;
};

} //namespace Saba
