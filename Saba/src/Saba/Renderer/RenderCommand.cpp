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
    Renderer::Submit([topology, indicesCount]() { s_API->DrawIndexed(topology, indicesCount); });
}
void RenderCommand::DrawIndexedInstanced(RendererAPI::Topology topology, uint32_t indicesCount, uint32_t instancesCount) {
    Renderer::Submit([topology, indicesCount, instancesCount]() { s_API->DrawIndexedInstanced(topology, indicesCount, instancesCount); });
}

void RenderCommand::SetDepthTestOptions(bool enable, bool writeMask, RendererAPI::ComparisonFunc compFunc) {
    Renderer::Submit([enable, writeMask, compFunc]() { s_API->SetDepthTestOptions(enable, writeMask, compFunc); });
}
void RenderCommand::SetStencilTestOptions(bool enable, uint8_t writeMask, uint8_t readMask, RendererAPI::ComparisonFunc frontFaceFunc,
    RendererAPI::ComparisonFunc backFaceFunc, uint32_t stencilRefVal)
{
    Renderer::Submit([enable, writeMask, readMask, frontFaceFunc, backFaceFunc, stencilRefVal]() {
        s_API->SetStencilTestOptions(enable, writeMask, readMask, frontFaceFunc, backFaceFunc, stencilRefVal);
    });
}
void RenderCommand::SetFrontFaceStencilOperations(RendererAPI::StencilOperation stencilFail, RendererAPI::StencilOperation depthFail,
    RendererAPI::StencilOperation pass)
{
    Renderer::Submit([stencilFail, depthFail, pass]() { s_API->SetFrontFaceStencilOperations(stencilFail, depthFail, pass); });
}
void RenderCommand::SetBackFaceStencilOperations(RendererAPI::StencilOperation stencilFail, RendererAPI::StencilOperation depthFail,
    RendererAPI::StencilOperation pass)
{
    Renderer::Submit([stencilFail, depthFail, pass]() { s_API->SetBackFaceStencilOperations(stencilFail, depthFail, pass); });
}
void RenderCommand::SetBlendOptions(uint32_t i, bool enable, RendererAPI::BlendOption sourceBlend, RendererAPI::BlendOption destinationBlend,
    RendererAPI::BlendOperation operation, RendererAPI::BlendOption sourceAlphaBlend, RendererAPI::BlendOption destinationAlphaBlend,
    RendererAPI::BlendOperation alphaOperation, uint8_t writeMask, glm::vec4 blendFactor)
{
    Renderer::Submit([i, enable, sourceBlend, destinationBlend, operation, sourceAlphaBlend, destinationAlphaBlend, alphaOperation, writeMask, blendFactor]() {
        s_API->SetBlendOptions(i, enable, sourceBlend, destinationBlend, operation, sourceAlphaBlend, destinationAlphaBlend, alphaOperation, writeMask, blendFactor);
    });
}
void RenderCommand::SetRasterizerOptions(RendererAPI::TriangleFillMode fillMode, RendererAPI::TriangleCullMode cullMode, bool isFrontFaceCounterClockwise) {
    Renderer::Submit([fillMode, cullMode, isFrontFaceCounterClockwise]() {
        s_API->SetRasterizerOptions(fillMode, cullMode, isFrontFaceCounterClockwise);
    });
}

} //namespace Saba
