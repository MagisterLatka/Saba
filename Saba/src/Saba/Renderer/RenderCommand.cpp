#include <pch.h>
#include "RenderCommand.h"

#include "Saba/Renderer/Renderer.h"

#if defined(SB_PLATFORM_WINDOWS)
#   include "Saba/Platform/DX11/DX11RendererAPI.h"
#endif
#include "Saba/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Saba {

Scope<RendererAPI> RenderCommand::s_API;
Scope<RendererAPI> RendererAPI::Create() {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:    SB_CORE_THROW_INFO("None API is not supported"); return nullptr;
        case RendererAPI::API::OpenGL:  return CreateScope<OpenGLRendererAPI>();
#if defined(SB_PLATFORM_WINDOWS)
        case RendererAPI::API::DX11:    return CreateScope<DX11RendererAPI>();
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

void RenderCommand::Init() {
    s_API = RendererAPI::Create();
    Renderer::Submit([]() { s_API->Init(); });
}
void RenderCommand::Shutdown() {
    Renderer::Submit([]() { s_API->Shutdown(); });
}

void RenderCommand::Draw(RendererAPI::Topology topology, uint32_t verticesCount) {
    Renderer::Submit([topology, verticesCount]() { s_API->Draw(topology, verticesCount); });
}
void RenderCommand::DrawIndexed(RendererAPI::Topology topology, uint32_t indicesCount) {
    Renderer::Submit([topology, indicesCount]() { s_API->Draw(topology, indicesCount); });
}

}
