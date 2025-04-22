#include <pch.h>
#include "InputLayout.h"

#include "Saba/Renderer/RendererAPI.h"
#if defined(SB_PLATFORM_WINDOWS)
#   include "Saba/Platform/DX11/DX11InputLayout.h"
#endif
#include "Saba/Platform/OpenGL/OpenGLInputLayout.h"

namespace Saba {

Ref<InputLayout> InputLayout::Create(const std::initializer_list<Ref<VertexBuffer>>& vertexBuffers,
    const Ref<Shader>& shader, const Ref<IndexBuffer>& indexBuffer)
{
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:    SB_CORE_THROW_INFO("None API is not supported"); return nullptr;
        case RendererAPI::API::OpenGL:  return Ref<OpenGLInputLayout>::Create(vertexBuffers, shader, indexBuffer);
#if defined(SB_PLATFORM_WINDOWS)
        case RendererAPI::API::DX11:    return Ref<DX11InputLayout>::Create(vertexBuffers, shader, indexBuffer);
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DX12 is not supported yet"); return nullptr;
#else
        case RendererAPI::API::DX11:
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DX11 and DX12 are not supported on non-windows systems"); return nullptr;
#endif
        case RendererAPI::API::Vulkan:  SB_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
    }

    SB_CORE_THROW_INFO("Unknown API");
    return nullptr;
}

} //namespace Saba
