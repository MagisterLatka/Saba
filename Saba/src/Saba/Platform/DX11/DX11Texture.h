#pragma once

#include "Saba/Renderer/Texture.h"
#include "Saba/Core/Buffer.h"
#include <d3d11.h>

namespace Saba {

class DX11Texture2D : public Texture2D
{
public:
    SB_CORE DX11Texture2D(Texture2DProps props);
    SB_CORE DX11Texture2D(uint32_t width, uint32_t height, void* data = nullptr, TextureFormat format = TextureFormat::RGBA8);
    SB_CORE virtual ~DX11Texture2D() = default;

    SB_CORE void Bind(uint32_t slot) const noexcept override;

    SB_CORE const Texture2DProps& GetProps() const noexcept override { return m_Props; }
    SB_CORE uint32_t GetWidth() const noexcept override { return m_Props.Width; }
    SB_CORE uint32_t GetHeight() const noexcept override { return m_Props.Height; }
    SB_CORE void* GetRawPointer() const noexcept override { return m_View.Get(); }
private:
    SB_CORE void Init();
private:
    Texture2DProps m_Props;
    Buffer m_Buffer;
    ComPtr<ID3D11ShaderResourceView> m_View;
};

}
