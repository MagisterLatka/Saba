#include <pch.h>
#include "DX11Texture.h"

#include "Saba/Renderer/Renderer.h"
#include "Saba/Platform/DX11/DX11Context.h"
#include <stb_image.h>

namespace Saba {

static DXGI_FORMAT GetFormat(TextureFormat format) {
    switch (format) {
        case TextureFormat::R8:      return DXGI_FORMAT_R8_UNORM;
        case TextureFormat::R32F:    return DXGI_FORMAT_R32_FLOAT;
        case TextureFormat::RGB8:    return DXGI_FORMAT_R8G8B8A8_UNORM;
        case TextureFormat::RGB32F:  return DXGI_FORMAT_R32G32B32_FLOAT;
        case TextureFormat::RGBA8:   return DXGI_FORMAT_R8G8B8A8_UNORM;
        case TextureFormat::RGBA32F: return DXGI_FORMAT_R32G32B32A32_FLOAT;
        default: 
    }
    SB_CORE_THROW_INFO("Unknown texture format");
    return DXGI_FORMAT_R8G8B8A8_UINT;
}
DX11Texture2D::DX11Texture2D(Texture2DProps props)
    : m_Props(std::move(props))
{
    SB_CORE_ASSERT(props.Width > 0u && props.Height > 0u, "Render target expect non-zero width and height");
    SB_CORE_ASSERT(static_cast<uint32_t>(props.Format) > 0 && static_cast<uint32_t>(props.Format) <= static_cast<uint32_t>(TextureFormat::Last), "Unknown texture format");
    Init();
}
DX11Texture2D::DX11Texture2D(uint32_t width, uint32_t height, void* data, TextureFormat format) {
    SB_CORE_ASSERT(width > 0u && height > 0u, "Render target expect non-zero width and height");
    SB_CORE_ASSERT(static_cast<uint32_t>(format) > 0 && static_cast<uint32_t>(format) <= static_cast<uint32_t>(TextureFormat::Last), "Unknown texture format");
    m_Props.Width = width;
    m_Props.Height = height;
    m_Props.Format = format;
    m_Buffer = Buffer(data, width * height * GetBPP(format));
    Init();
}
void DX11Texture2D::Init() {
    Ref<DX11Texture2D> instance = this;
    Renderer::Submit([instance]() mutable {
        if (!instance->m_Props.Filepath.empty()) {
            std::string filepath = instance->m_Props.Filepath.string();
            int width = 0, height = 0, channels;
            switch (instance->m_Props.Format)
            {
                case Saba::TextureFormat::R8:
                    instance->m_Buffer.Data = stbi_load(filepath.c_str(), &width, &height, &channels, 1); break;
                case Saba::TextureFormat::R32F:
                    instance->m_Buffer.Data = stbi_loadf(filepath.c_str(), &width, &height, &channels, 1); break;
                case Saba::TextureFormat::RGB8:
                    instance->m_Buffer.Data = stbi_load(filepath.c_str(), &width, &height, &channels, 4); break;
                case Saba::TextureFormat::RGB32F:
                    instance->m_Buffer.Data = stbi_loadf(filepath.c_str(), &width, &height, &channels, 3); break;
                case Saba::TextureFormat::RGBA8:
                    instance->m_Buffer.Data = stbi_load(filepath.c_str(), &width, &height, &channels, 4); break;
                case Saba::TextureFormat::RGBA32F:
                    instance->m_Buffer.Data = stbi_loadf(filepath.c_str(), &width, &height, &channels, 4); break;
                default: break;
            }
            SB_CORE_ASSERT(instance->m_Buffer.Data, "Failed to load texture from file {0}", filepath);
            instance->m_Props.Width = static_cast<uint32_t>(width);
            instance->m_Props.Height = static_cast<uint32_t>(height);
        }

        D3D11_TEXTURE2D_DESC textureDesc = { 0 };
        textureDesc.Width = instance->m_Props.Width;
        textureDesc.Height = instance->m_Props.Height;
        textureDesc.MipLevels = instance->m_Props.GenerateMipMaps ? CalcNrMips(instance->m_Props.Width, instance->m_Props.Height) : 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = GetFormat(instance->m_Props.Format);
        textureDesc.SampleDesc.Count = 1;
        textureDesc.Usage = D3D11_USAGE_IMMUTABLE; //TODO make data access
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        if (instance->m_Props.GenerateMipMaps) textureDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
        textureDesc.MiscFlags = instance->m_Props.GenerateMipMaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0u;

        D3D11_SUBRESOURCE_DATA textureData = { 0 };
        textureData.pSysMem = instance->m_Buffer.Data;
        textureData.SysMemPitch = instance->m_Props.Width * GetBPP(instance->m_Props.Format);

        ComPtr<ID3D11Texture2D> texture;
        auto device = DX11Context::GetContextFromApplication()->GetDevice();
        HRESULT hr;
        SB_DX_GRAPHICS_CALL_INFO(device->CreateTexture2D(&textureDesc, &textureData, &texture));

        D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
        viewDesc.Format = GetFormat(instance->m_Props.Format);
        viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        viewDesc.Texture2D.MostDetailedMip = 0u;
        viewDesc.Texture2D.MipLevels = instance->m_Props.GenerateMipMaps ? CalcNrMips(instance->m_Props.Width, instance->m_Props.Height) : 1;
        SB_DX_GRAPHICS_CALL_INFO(device->CreateShaderResourceView(texture.Get(), &viewDesc, &instance->m_View));

        if (instance->m_Props.GenerateMipMaps)
            DX11Context::GetContextFromApplication()->GetContext()->GenerateMips(instance->m_View.Get());

        if (instance->m_Buffer.Data) stbi_image_free(instance->m_Buffer.Data);
    });
}

void DX11Texture2D::Bind(uint32_t slot) const noexcept {
    Ref<const DX11Texture2D> instance = this;
    Renderer::Submit([instance, slot]() {
        DX11Context::GetContextFromApplication()->GetContext()->PSSetShaderResources(slot, 1, instance->m_View.GetAddressOf());
    });
}

}
