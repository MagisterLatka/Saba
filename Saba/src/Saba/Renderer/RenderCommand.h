#pragma once

#include "Saba/Renderer/RendererAPI.h"

namespace Saba {

class RenderCommand {
    friend class Renderer;
public:
    SB_CORE static void Draw(RendererAPI::Topology topology, uint32_t verticesCount);
    SB_CORE static void DrawIndexed(RendererAPI::Topology topology, uint32_t IndicesCount);
private:
    SB_CORE static void Init();
    SB_CORE static void Shutdown();
private:
    SB_CORE static Scope<RendererAPI> s_API;
};

}
