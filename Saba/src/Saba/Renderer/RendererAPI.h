#pragma once

#include "Saba/Core/Core.h"

namespace Saba {

class RendererAPI {
    friend class Application;
    friend class RenderCommand;
public:
    enum class API : uint8_t { None = 0, OpenGL, DX11, DX12, Vulkan };
    enum class Topology : uint8_t { None = 0, Points, Lines, LineStrip, Triangles, TriangleStrip };
public:
    SB_CORE static API GetAPI() { return s_API; }

    SB_CORE virtual ~RendererAPI() = default;

    SB_CORE virtual void Init() = 0;
    SB_CORE virtual void Shutdown() = 0;

    SB_CORE virtual void Draw(Topology topology, uint32_t verticesCount) = 0;
    SB_CORE virtual void DrawIndexed(Topology topology, uint32_t indicesCount) = 0;
private:
    SB_CORE static Scope<RendererAPI> Create();
private:
    SB_CORE static API s_API;
};

}
