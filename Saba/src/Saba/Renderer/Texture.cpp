#include <pch.h>
#include "Texture.h"

#include "Saba/Renderer/RendererAPI.h"
#if defined(SB_PLATFORM_WINDOWS)
#    include "Saba/Platform/DX11/DX11Texture.h"
#endif
#include "Saba/Platform/OpenGL/OpenGLTexture.h"

#include <stb_image.h>

namespace Saba {

Ref<Texture2D> Texture2D::Create(Texture2DProps props)
{
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:    SB_CORE_THROW_INFO("None API is not supported"); return nullptr;
        case RendererAPI::API::OpenGL:  return Ref<OpenGLTexture2D>::Create(std::move(props));
        case RendererAPI::API::Vulkan:  SB_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
#if defined(SB_PLATFORM_WINDOWS)
        case RendererAPI::API::DX11:    return Ref<DX11Texture2D>::Create(std::move(props));
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DirectX 12 is not supported yet"); return nullptr;
#else
        case RendererAPI::API::DX11:
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DX11 and DX12 are not supported on non-windows systems"); return nullptr;
#endif
    }

    SB_CORE_THROW_INFO("Unknown API");
    return nullptr;
}
Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, void* data, TextureFormat format) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:    SB_CORE_THROW_INFO("None API is not supported"); return nullptr;
        case RendererAPI::API::OpenGL:    return Ref<OpenGLTexture2D>::Create(width, height, data, format);
        case RendererAPI::API::Vulkan:    SB_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
#if defined(SB_PLATFORM_WINDOWS)
        case RendererAPI::API::DX11:    return Ref<DX11Texture2D>::Create(width, height, data, format);
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DirectX 12 is not supported yet"); return nullptr;
#else
        case RendererAPI::API::DX11:
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DX11 and DX12 are not supported on non-windows systems"); return nullptr;
#endif
    }

    SB_CORE_THROW_INFO("Unknown API");
    return nullptr;
}

uint32_t Texture2D::GetBPP(TextureFormat format) {
    switch (format) {
        case Saba::TextureFormat::R8:        return 1 * sizeof(uint8_t);
        case Saba::TextureFormat::R32F:    return 1 * sizeof(float);
        case Saba::TextureFormat::RGB8:    return 3 * sizeof(uint8_t);
        case Saba::TextureFormat::RGB32F:    return 3 * sizeof(float);
        case Saba::TextureFormat::RGBA8:    return 4 * sizeof(uint8_t);
        case Saba::TextureFormat::RGBA32F:    return 4 * sizeof(float);
        default: break;
    }
    SB_CORE_THROW_INFO("Unknown texture format");
    return 0;
}
uint32_t Texture2D::CalcNrMips(uint32_t width, uint32_t height) {
    return static_cast<uint32_t>(std::max(std::bit_width(width), std::bit_width(height)));
}
void* Texture2D::Decode(const void* data, uint32_t size, uint32_t& width, uint32_t& height) {
    int x, y, channels;
    void* output = stbi_load_from_memory((uint8_t*)data, static_cast<int>(size), &x, &y, &channels, 4);
    width = static_cast<uint32_t>(x);
    height = static_cast<uint32_t>(y);
    return output;
}

}
