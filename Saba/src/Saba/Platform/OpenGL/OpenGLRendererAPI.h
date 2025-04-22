#pragma once

#include "Saba/Renderer/RendererAPI.h"

namespace Saba {

class OpenGLRendererAPI : public RendererAPI {
public:
    void Init() override;
    void Shutdown() override;

    void Draw(Topology topology, uint32_t verticesCount) override;
    void DrawIndexed(Topology topology, uint32_t indicesCount) override;
    void DrawIndexedInstanced(Topology topology, uint32_t indicesCount, uint32_t instancesCount) override;

    void SetDepthTestOptions(bool enable, bool writeMask, ComparisonFunc compFunc) override;
    void SetStencilTestOptions(bool enable, uint8_t writeMask, uint8_t readMask, ComparisonFunc frontFaceFunc,
        ComparisonFunc backFaceFunc, uint32_t stencilRefVal) override;
    void SetFrontFaceStencilOperations(StencilOperation stencilFail, StencilOperation depthFail, StencilOperation pass) override;
    void SetBackFaceStencilOperations(StencilOperation stencilFail, StencilOperation depthFail, StencilOperation pass) override;
    void SetRasterizerOptions(TriangleFillMode fillMode, TriangleCullMode cullMode, bool isFrontFaceCounterClockwise) override;
    void SetBlendOptions(uint32_t i, bool enable, BlendOption sourceBlend, BlendOption destinationBlend,
        BlendOperation operation, BlendOption sourceAlphaBlend, BlendOption destinationAlphaBlend, BlendOperation alphaOperation,
        uint8_t writeMask, glm::vec4 blendFactor) override;
private:
    static uint32_t GetTopology(Topology topology);
    static void InitShaders();
};

} //namespace Saba
