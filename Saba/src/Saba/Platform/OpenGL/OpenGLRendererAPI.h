#pragma once

#include "Saba/Renderer/RendererAPI.h"

namespace Saba {

class OpenGLRendererAPI : public RendererAPI {
public:
    SB_CORE void Init() override;
    SB_CORE void Shutdown() override;

    SB_CORE void Draw(Topology topology, uint32_t verticesCount) override;
    SB_CORE void DrawIndexed(Topology topology, uint32_t indicesCount) override;
    SB_CORE void DrawIndexedInstanced(Topology topology, uint32_t indicesCount, uint32_t instancesCount) override;

    SB_CORE void SetDepthTestOptions(bool enable, bool writeMask, ComparisonFunc compFunc) override;
    SB_CORE void SetStencilTestOptions(bool enable, uint8_t writeMask, uint8_t readMask, ComparisonFunc frontFaceFunc,
        ComparisonFunc backFaceFunc, uint32_t stencilRefVal) override;
    SB_CORE void SetFrontFaceStencilOperations(StencilOperation stencilFail, StencilOperation depthFail, StencilOperation pass) override;
    SB_CORE void SetBackFaceStencilOperations(StencilOperation stencilFail, StencilOperation depthFail, StencilOperation pass) override;
    SB_CORE void SetRasterizerOptions(TriangleFillMode fillMode, TriangleCullMode cullMode, bool isFrontFaceCounterClockwise) override;
    SB_CORE void SetBlendOptions(uint32_t i, bool enable, BlendOption sourceBlend, BlendOption destinationBlend,
        BlendOperation operation, BlendOption sourceAlphaBlend, BlendOption destinationAlphaBlend, BlendOperation alphaOperation,
        uint8_t writeMask, glm::vec4 blendFactor) override;
private:
    SB_CORE static uint32_t GetTopology(Topology topology);
    SB_CORE void InitShaders();
};

}
