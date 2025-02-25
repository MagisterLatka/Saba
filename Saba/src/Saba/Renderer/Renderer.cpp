#include <pch.h>
#include "Renderer.h"

#include "Saba/Renderer/RendererAPI.h"
#include "Saba/Renderer/RenderCommand.h"

namespace Saba {

struct RendererData {
    Scope<RenderCommandQueue> commandQueue;
    Scope<ShaderLibrary> shaderLibrary;
};
static RendererData s_Data;

void Renderer::Init() {
    s_Data.commandQueue = CreateScope<RenderCommandQueue>();
    s_Data.shaderLibrary = CreateScope<ShaderLibrary>();
    RenderCommand::Init();
    s_Data.commandQueue->Execute();
}
void Renderer::Shutdown() {
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
RenderCommandQueue& Renderer::GetRenderCommandQueue() noexcept {
    return *s_Data.commandQueue;
}

}
