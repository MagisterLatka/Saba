#include <pch.h>
#include "Renderer.h"

#include "Saba/Renderer/RendererAPI.h"
#include "Saba/Renderer/RenderCommand.h"
#include "Saba/Renderer/Renderer2D.h"
#include "Saba/Renderer/Renderer3D.h"

namespace Saba {

struct RendererData {
    Scope<RenderCommandQueue> commandQueue;
    Scope<ShaderLibrary> shaderLibrary;
    Scope<MeshLibrary> meshLibrary;
};
static RendererData s_Data;

void Renderer::Init() {
    s_Data.commandQueue = CreateScope<RenderCommandQueue>();
    s_Data.shaderLibrary = CreateScope<ShaderLibrary>();
    s_Data.meshLibrary = CreateScope<MeshLibrary>();
    RenderCommand::Init();
    s_Data.commandQueue->Execute();
    Renderer2D::Init();
    Renderer3D::Init();
    s_Data.commandQueue->Execute();
}
void Renderer::Shutdown() {
    Renderer3D::Shutdown();
    Renderer2D::Shutdown();
    s_Data.shaderLibrary.reset();
    RenderCommand::Shutdown();
    s_Data.commandQueue->Execute();
    s_Data.commandQueue.reset();
}

void Renderer::Render() {
    s_Data.commandQueue->Execute();
}

ShaderLibrary& Renderer::GetShaderLibrary() noexcept {
    return *s_Data.shaderLibrary;
}
MeshLibrary& Renderer::GetMeshLibrary() noexcept {
    return *s_Data.meshLibrary;
}
RenderCommandQueue& Renderer::GetRenderCommandQueue() noexcept {
    return *s_Data.commandQueue;
}

}
