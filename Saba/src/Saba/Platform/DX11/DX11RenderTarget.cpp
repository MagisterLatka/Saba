#include <pch.h>
#include "DX11RenderTarget.h"

#include "Saba/Renderer/Renderer.h"
#include "Saba/Platform/DX11/DX11Context.h"

namespace Saba {

static DXGI_FORMAT GetFormat(RenderTargetFormat format) {
    switch (format) {
        case RenderTargetFormat::R8:              return DXGI_FORMAT_R8_UNORM;
        case RenderTargetFormat::R32F:            return DXGI_FORMAT_R32_FLOAT;
        case RenderTargetFormat::RG8:             return DXGI_FORMAT_R8G8_UNORM;
        case RenderTargetFormat::RG16F:           return DXGI_FORMAT_R16G16_FLOAT;
        case RenderTargetFormat::RG32F:           return DXGI_FORMAT_R32G32B32_FLOAT;
        case RenderTargetFormat::RGB8:            return DXGI_FORMAT_R8G8B8A8_UNORM;
        case RenderTargetFormat::RGB32F:          return DXGI_FORMAT_R32G32B32_FLOAT;
        case RenderTargetFormat::RGBA8:           return DXGI_FORMAT_R8G8B8A8_UNORM;
        case RenderTargetFormat::RGBA16F:         return DXGI_FORMAT_R16G16B16A16_FLOAT;
        case RenderTargetFormat::RGBA32F:         return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case RenderTargetFormat::Depth32F:        return DXGI_FORMAT_D32_FLOAT;
        case RenderTargetFormat::Depth24Stencil8: return DXGI_FORMAT_D24_UNORM_S8_UINT;
        default: break;
    }
    SB_CORE_THROW_INFO("Unknown render target format");
    return DXGI_FORMAT_R8G8B8A8_UINT;
}
static uint32_t GetBPP(RenderTargetFormat format) {
    switch (format) {
        case RenderTargetFormat::R8:
            return 1;
        case RenderTargetFormat::R32F:
        case RenderTargetFormat::RG16F:
        case RenderTargetFormat::RGBA8:
        case RenderTargetFormat::Depth32F:
        case RenderTargetFormat::Depth24Stencil8:
            return 4;
        case RenderTargetFormat::RG8:
            return 2;
        case RenderTargetFormat::RG32F:
        case RenderTargetFormat::RGBA16F:
            return 8;
        case RenderTargetFormat::RGB8:
            return 3;
        case RenderTargetFormat::RGB32F:
            return 12;
        case RenderTargetFormat::RGBA32F:
            return 16;
        default: break;
    }
    SB_CORE_THROW_INFO("Unknown render target format");
    return 0;
}

DX11RenderTarget::DX11RenderTarget(uint32_t width, uint32_t height, RenderTargetFormat format)
    : m_Width(width), m_Height(height), m_Format(format)
{
    SB_CORE_ASSERT(width > 0u && height > 0u, "Render target expect non-zero width and height");
    SB_CORE_ASSERT(static_cast<uint32_t>(format) > 0 && static_cast<uint32_t>(format) <= static_cast<uint32_t>(RenderTargetFormat::Last), "Unknown render target format");
    Resize(m_Width, m_Height, true);
}
void DX11RenderTarget::Resize(uint32_t width, uint32_t height, bool forceResize) {
    if (m_Width == width && m_Height == height && !forceResize) return;
    SB_CORE_ASSERT(width > 0u && height > 0u, "Render target expect non-zero width and height");
    m_Width = width;
    m_Height = height;

    Ref<DX11RenderTarget> instance = this;
    Renderer::Submit([instance]() mutable {
        auto device = DX11Context::GetContextFromApplication()->GetDevice();
        HRESULT hr;

        D3D11_TEXTURE2D_DESC textureDesc = { 0 };
        textureDesc.Width = instance->m_Width;
        textureDesc.Height = instance->m_Height;
        textureDesc.MipLevels = 1u;
        textureDesc.ArraySize = 1u;
        textureDesc.Format = GetFormat(instance->m_Format);
        textureDesc.SampleDesc.Count = 1u;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = static_cast<uint32_t>(instance->m_Format) & 0x30u ? D3D11_BIND_DEPTH_STENCIL : D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

        ComPtr<ID3D11Texture2D> texture;
        SB_DX_GRAPHICS_CALL_INFO(device->CreateTexture2D(&textureDesc, nullptr, &texture));

        if ((uint32_t)instance->m_Format & 0x30u) {
            D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilDesc;
            depthStencilDesc.Format = textureDesc.Format; 
            depthStencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
            depthStencilDesc.Flags = 0u;
            depthStencilDesc.Texture2D.MipSlice = 0u;

            SB_DX_GRAPHICS_CALL_INFO(device->CreateDepthStencilView(texture.Get(), &depthStencilDesc, &instance->m_DepthStencil));
        }
        else {
            D3D11_RENDER_TARGET_VIEW_DESC renderTargetDesc;
            renderTargetDesc.Format = textureDesc.Format;
            renderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
            renderTargetDesc.Texture2D.MipSlice = 0u;

            SB_DX_GRAPHICS_CALL_INFO(device->CreateRenderTargetView(texture.Get(), &renderTargetDesc, &instance->m_RenderTarget));

            D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceDesc;
            shaderResourceDesc.Format = textureDesc.Format;
            shaderResourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            shaderResourceDesc.Texture2D.MostDetailedMip = 0u;
            shaderResourceDesc.Texture2D.MipLevels = 1u;

            SB_DX_GRAPHICS_CALL_INFO(device->CreateShaderResourceView(texture.Get(), &shaderResourceDesc, &instance->m_View));
        }
    });
}

void DX11RenderTarget::BindTexture(uint32_t slot) const noexcept {
    Ref<const DX11RenderTarget> instance = this;
    Renderer::Submit([instance, slot]() {
        DX11Context::GetContextFromApplication()->GetContext()->PSSetShaderResources(slot, 1u, instance->m_View.GetAddressOf());
    });
}

void DX11RenderTarget::Clear(const glm::vec4& clearVal, float depth, uint8_t stencil) const noexcept {
    Ref<const DX11RenderTarget> instance = this;
    Renderer::Submit([instance, clearVal, depth, stencil]() {
        if (static_cast<uint32_t>(instance->m_Format) & 0x30u)
            DX11Context::GetContextFromApplication()->GetContext()->ClearDepthStencilView(instance->m_DepthStencil.Get(),
                instance->m_Format == RenderTargetFormat::Depth32F ? D3D11_CLEAR_DEPTH : D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
        else
            DX11Context::GetContextFromApplication()->GetContext()->ClearRenderTargetView(instance->m_RenderTarget.Get(), glm::value_ptr(clearVal));
    });
}

}
