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

    SB_CORE void SetDepthTestOptions(bool enable, bool writeMask, ComparisonFunc compFunc) override;
    SB_CORE void SetStencilTestOptions(bool enable, uint8_t writeMask, uint8_t readMask, ComparisonFunc frontFaceFunc,
        ComparisonFunc backFaceFunc, uint32_t stencilRefVal) override;
    SB_CORE void SetFrontFaceStencilOperations(StencilOperation stencilFail, StencilOperation depthFail, StencilOperation pass) override;
    SB_CORE void SetBackFaceStencilOperations(StencilOperation stencilFail, StencilOperation depthFail, StencilOperation pass) override;
    SB_CORE void SetRasterizerOptions(TriangleFillMode fillMode, TriangleCullMode cullMode, bool isFrontFaceCounterClockwise) override;
    SB_CORE void SetBlendOptions(uint32_t i, bool enable, BlendOption sourceBlend, BlendOption destinationBlend, BlendOperation operation,
        BlendOption sourceAlphaBlend, BlendOption destinationAlphaBlend, BlendOperation alphaOperation, uint8_t writeMask, glm::vec4 blendFactor) override;
private:
    SB_CORE static void SetTopology(ComPtr<ID3D11DeviceContext> context, Topology topology);
    SB_CORE void InitShaders();
    SB_CORE void RecreateDepthStencilState();
private:
    ComPtr<ID3D11DepthStencilState> m_DepthStencilState;
    D3D11_DEPTH_STENCIL_DESC m_DepthStencilDesc;
    uint32_t m_StencilRefVal = 0u;
    ComPtr<ID3D11RasterizerState> m_RasterizerState;
    ComPtr<ID3D11BlendState> m_BlendState;
    D3D11_BLEND_DESC m_BlendDesc;
};

}
