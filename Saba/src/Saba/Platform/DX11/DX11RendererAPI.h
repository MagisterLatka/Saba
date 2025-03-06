#pragma once

#include "Saba/Renderer/RendererAPI.h"

#include <d3d11.h>

namespace Saba {

class DX11RendererAPI : public RendererAPI
{
public:
    SB_CORE void Init() override;
    SB_CORE void Shutdown() override;

    SB_CORE void Draw(Topology topology, uint32_t verticesCount) override;
    SB_CORE void DrawIndexed(Topology topology, uint32_t indicesCount) override;
private:
    SB_CORE static void SetTopology(ComPtr<ID3D11DeviceContext> context, Topology topology);
    SB_CORE void InitShaders();
};

}
