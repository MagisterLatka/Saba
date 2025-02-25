#include <pch.h>
#include "DX11RendererAPI.h"

#include "Saba/Platform/DX11/DX11Context.h"

namespace Saba {

void DX11RendererAPI::Init() {}
void DX11RendererAPI::Shutdown() {}

void DX11RendererAPI::Draw(Topology topology, uint32_t verticesCount) {
    auto context = DX11Context::GetContextFromApplication()->GetContext();
    SetTopology(context, topology);
    context->Draw(verticesCount, 0u);
}
void DX11RendererAPI::DrawIndexed(Topology topology, uint32_t indicesCount) {
    auto context = DX11Context::GetContextFromApplication()->GetContext();
    SetTopology(context, topology);
    context->DrawIndexed(indicesCount, 0u, 0u);
}

void DX11RendererAPI::SetTopology(ComPtr<ID3D11DeviceContext> context, Topology topology) {
    switch (topology) {
        case Saba::RendererAPI::Topology::Points:
            context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
            return;
        case Saba::RendererAPI::Topology::Lines:
            context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
            return;
        case Saba::RendererAPI::Topology::LineStrip:
            context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
            return;
        case Saba::RendererAPI::Topology::Triangles:
            context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            return;
        case Saba::RendererAPI::Topology::TriangleStrip:
            context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
            return;
    }
    SB_CORE_THROW_INFO("Unknown topology");
}

}
