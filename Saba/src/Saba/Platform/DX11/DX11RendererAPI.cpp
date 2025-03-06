#include <pch.h>
#include "DX11RendererAPI.h"

#include "Saba/Platform/DX11/DX11Context.h"

#include "Saba/Renderer/Renderer.h"

namespace Saba {

void DX11RendererAPI::Init() {
    InitShaders();
}
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
        default: break;
    }
    SB_CORE_THROW_INFO("Unknown topology");
}

void DX11RendererAPI::InitShaders() {
    const std::string vertex2D = R"(
        struct VSOut {
            float4 color : Color;
            float2 uv : UV;
            int tid : TID;
            float tillingFactor : TillingFactor;
            float4 pos : SV_Position;
        };
        cbuffer ConstBuf {
            matrix<float, 4, 4> u_ViewProjMat;
        }

        VSOut main(float4 pos : Position, float4 color : Color, float2 uv : UV, int tid : TID, float tillingFactor : TillingFactor) {
            VSOut output;
            output.color = color;
            output.uv = uv;
            output.tid = tid;
            output.tillingFactor = tillingFactor;
            output.pos = mul(pos, u_ViewProjMat);
            return output;
        }
    )";

    const std::string fragment2D = R"(
        struct FSIn {
            float4 color : Color;
            float2 uv : UV;
            int tid : TID;
            float tillingFactor : TillingFactor;
        };
        Texture2D<float4> u_Textures[16];
        SamplerState u_Samplers[16];

        float4 GetDataFromTexture(int tid, float2 uv, float tillingFactor) {
            float4 output = u_Textures[0].Sample(u_Samplers[0], uv * tillingFactor) * (1 - abs(sign(tid - 0)));
            output += u_Textures[1].Sample(u_Samplers[1], uv * tillingFactor) * (1 - abs(sign(tid - 1)));
            output += u_Textures[2].Sample(u_Samplers[2], uv * tillingFactor) * (1 - abs(sign(tid - 2)));
            output += u_Textures[3].Sample(u_Samplers[3], uv * tillingFactor) * (1 - abs(sign(tid - 3)));
            output += u_Textures[4].Sample(u_Samplers[4], uv * tillingFactor) * (1 - abs(sign(tid - 4)));
            output += u_Textures[5].Sample(u_Samplers[5], uv * tillingFactor) * (1 - abs(sign(tid - 5)));
            output += u_Textures[6].Sample(u_Samplers[6], uv * tillingFactor) * (1 - abs(sign(tid - 6)));
            output += u_Textures[7].Sample(u_Samplers[7], uv * tillingFactor) * (1 - abs(sign(tid - 7)));
            output += u_Textures[8].Sample(u_Samplers[8], uv * tillingFactor) * (1 - abs(sign(tid - 8)));
            output += u_Textures[9].Sample(u_Samplers[9], uv * tillingFactor) * (1 - abs(sign(tid - 9)));
            output += u_Textures[10].Sample(u_Samplers[10], uv * tillingFactor) * (1 - abs(sign(tid - 10)));
            output += u_Textures[11].Sample(u_Samplers[11], uv * tillingFactor) * (1 - abs(sign(tid - 11)));
            output += u_Textures[12].Sample(u_Samplers[12], uv * tillingFactor) * (1 - abs(sign(tid - 12)));
            output += u_Textures[13].Sample(u_Samplers[13], uv * tillingFactor) * (1 - abs(sign(tid - 13)));
            output += u_Textures[14].Sample(u_Samplers[14], uv * tillingFactor) * (1 - abs(sign(tid - 14)));
            output += u_Textures[15].Sample(u_Samplers[15], uv * tillingFactor) * (1 - abs(sign(tid - 15)));
            return output;
        }

        float4 main(FSIn input) : SV_Target {
            float4 output = input.color * GetDataFromTexture(input.tid, input.uv, input.tillingFactor);
            return output;
        }
    )";

    Renderer::GetShaderLibrary().Load("2D", vertex2D, fragment2D);
}

}
