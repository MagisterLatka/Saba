#pragma once

#include "Saba/Core/Core.h"

namespace Saba {

class RendererAPI {
    friend class Application;
    friend class RenderCommand;
public:
    enum class API : uint8_t { None = 0, OpenGL, DX11, DX12, Vulkan };
    enum class Topology : uint8_t { None = 0, Points, Lines, LineStrip, Triangles, TriangleStrip };
    enum class ComparisonFunc : uint8_t { None = 0, Never, Less, Equal, LessOrEqual, Greater, GreaterOrEqual, NotEqual, Always };
    enum class StencilOperation : uint8_t { None = 0, Keep, Zero, Replace, Increment, IncrementClamp, Decrement, DecrementClamp, Invert };
    enum class TriangleFillMode : uint8_t { None = 0, Full, Wireframe };
    enum class TriangleCullMode : uint8_t { None = 0, DrawAll, DrawFrontFace, DrawBackFace };
    enum class BlendOption : uint8_t { None = 0, Zero, One, SourceColor, SourceColorInvert, SourceAlpha, SourceAlphaInvert,
        DestinationColor, DestinationColorInvert, DestinationAlpha, DestinationAlphaInvert, BlendFactor, BlendFactorInvert };
    enum class BlendOperation : uint8_t { None = 0, Add, Subtract, ReverseSubtract, Min, Max };
public:
    virtual ~RendererAPI() = default;

    SB_CORE static API GetAPI() { return s_API; }
private:
    virtual void Init() = 0;
    virtual void Shutdown() = 0;

    virtual void Draw(Topology topology, uint32_t verticesCount) = 0;
    virtual void DrawIndexed(Topology topology, uint32_t indicesCount) = 0;
    virtual void DrawIndexedInstanced(RendererAPI::Topology topology, uint32_t indicesCount, uint32_t instancesCount) = 0;

    virtual void SetDepthTestOptions(bool enable, bool writeMask = true, ComparisonFunc compFunc = RendererAPI::ComparisonFunc::Less) = 0;
    virtual void SetStencilTestOptions(bool enable, uint8_t writeMask = 0xffu, uint8_t readMask = 0xffu,
        ComparisonFunc frontFaceFunc = ComparisonFunc::Always, ComparisonFunc backFaceFunc = ComparisonFunc::Always, uint32_t stencilRefVal = 0u) = 0;
    virtual void SetFrontFaceStencilOperations(StencilOperation stencilFail = StencilOperation::Keep, StencilOperation depthFail = StencilOperation::Keep,
        StencilOperation pass = StencilOperation::Keep) = 0;
    virtual void SetBackFaceStencilOperations(StencilOperation stencilFail = StencilOperation::Keep, StencilOperation depthFail = StencilOperation::Keep,
        StencilOperation pass = StencilOperation::Keep) = 0;

    virtual void SetRasterizerOptions(TriangleFillMode fillMode = TriangleFillMode::Full, TriangleCullMode cullMode = TriangleCullMode::DrawFrontFace,
        bool isFrontFaceCounterClockwise = false) = 0;

    virtual void SetBlendOptions(uint32_t i, bool enable, BlendOption sourceBlend = BlendOption::One, BlendOption destinationBlend = BlendOption::Zero,
        BlendOperation operation = BlendOperation::Add, BlendOption sourceAlphaBlend = BlendOption::One, BlendOption destinationAlphaBlend = BlendOption::Zero,
        BlendOperation alphaOperation = BlendOperation::Add, uint8_t writeMask = 1 | 2 | 4 | 8, glm::vec4 blendFactor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)) = 0;

    static Scope<RendererAPI> Create();
private:
    SB_CORE static API s_API;
};

} //namespace Saba
