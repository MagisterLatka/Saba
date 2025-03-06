#pragma once

#include "Saba/Renderer/RendererAPI.h"

namespace Saba {

class OpenGLRendererAPI : public RendererAPI {
public:
    SB_CORE void Init() override;
    SB_CORE void Shutdown() override;

    SB_CORE void Draw(Topology topology, uint32_t verticesCount) override;
    SB_CORE void DrawIndexed(Topology topology, uint32_t indicesCount) override;
private:
    SB_CORE static int GetTopology(Topology topology);
    SB_CORE void InitShaders();
};

}
