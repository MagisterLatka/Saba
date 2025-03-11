#include <pch.h>
#include "RenderTarget.h"

#include "Saba/Renderer/RendererAPI.h"
#if defined(SB_PLATFORM_WINDOWS)
#    include "Saba/Platform/DX11/DX11RenderTarget.h"
#endif
#include "Saba/Platform/OpenGL/OpenGLRenderTarget.h"

namespace Saba {

Ref<RenderTarget> RenderTarget::Create(uint32_t width, uint32_t height, RenderTargetFormat format, glm::vec4 clearVal) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:    SB_CORE_THROW_INFO("None API is not supported"); return nullptr;
        case RendererAPI::API::OpenGL:  return Ref<OpenGLRenderTarget>::Create(width, height, format, clearVal);
        case RendererAPI::API::Vulkan:  SB_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
#if defined(SB_PLATFORM_WINDOWS)
        case RendererAPI::API::DX11:    return Ref<DX11RenderTarget>::Create(width, height, format, clearVal);
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DirectX 12 is not supported yet"); return nullptr;
#else
        case RendererAPI::API::DX11:
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DX11 and DX12 are not supported on non-windows systems"); return nullptr;
#endif
    }

    SB_CORE_THROW_INFO("Unknown API");
    return nullptr;
}

}
