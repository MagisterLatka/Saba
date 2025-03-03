#include <pch.h>
#include "Renderer2D.h"

#include "Saba/Renderer/InputLayout.h"
#include "Saba/Renderer/Renderer.h"
#include "Saba/Renderer/RenderCommand.h"

namespace Saba {

struct VertexData {
    glm::vec4 pos;
    glm::vec4 color;
    glm::vec2 uv;
    uint32_t tid;
    float tillingFactor;
};

static constexpr uint32_t c_MaxQuads = 10000u;
static constexpr uint32_t c_QuadBufferSize = c_MaxQuads * 4 * static_cast<uint32_t>(sizeof(VertexData));
static constexpr uint32_t c_MaxTextures = 16u;

struct Renderer2DData {
    VertexData* quadVertexData = nullptr;
    VertexData* quadInsert = nullptr;
    uint32_t quadCount = 0u;
    Ref<InputLayout> quadInputLayout;
    std::array<Ref<Texture2D>, c_MaxTextures> quadTextures;
    uint32_t quadTextureIndex = 1u;

    Ref<Shader> shader;
    Ref<ConstantBuffer> viewProj;

    Renderer2D::Stats stats;
};
static Renderer2DData s_Data;

static void GetShaderSource(std::string& vertexSource, std::string& fragmentSource) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:    SB_CORE_THROW_INFO("None API is not supported"); break;
        case RendererAPI::API::OpenGL: {
            vertexSource = R"(
                #version 460 core

                layout(location = 0) in vec4 i_Pos;
                layout(location = 1) in vec4 i_Color;
                layout(location = 2) in vec2 i_UV;
                layout(location = 3) in int i_TID;
                layout(location = 4) in float i_TillingFactor;

                layout(std140, binding = 0) uniform RendererData
                {
                    mat4 u_ViewProjMat;
                };

                out Data {
                    vec4 color;
                    vec2 uv;
                    flat int tid;
                    flat float tillingFactor;
                } vs_out;

                void main()
                {
                    gl_Position = u_ViewProjMat * i_Pos;
                    vs_out.color = i_Color;
                    vs_out.uv = i_UV;
                    vs_out.tid = i_TID;
                    vs_out.tillingFactor = i_TillingFactor;
                }
            )";

            fragmentSource = R"(
                #version 460 core

                layout(location = 0) out vec4 o_Color;

                in Data {
                    vec4 color;
                    vec2 uv;
                    flat int tid;
                    flat float tillingFactor;
                } fs_in;

                layout(binding = 0) uniform sampler2D u_Tex0;
                layout(binding = 1) uniform sampler2D u_Tex1;
                layout(binding = 2) uniform sampler2D u_Tex2;
                layout(binding = 3) uniform sampler2D u_Tex3;
                layout(binding = 4) uniform sampler2D u_Tex4;
                layout(binding = 5) uniform sampler2D u_Tex5;
                layout(binding = 6) uniform sampler2D u_Tex6;
                layout(binding = 7) uniform sampler2D u_Tex7;
                layout(binding = 8) uniform sampler2D u_Tex8;
                layout(binding = 9) uniform sampler2D u_Tex9;
                layout(binding = 10) uniform sampler2D u_Tex10;
                layout(binding = 11) uniform sampler2D u_Tex11;
                layout(binding = 12) uniform sampler2D u_Tex12;
                layout(binding = 13) uniform sampler2D u_Tex13;
                layout(binding = 14) uniform sampler2D u_Tex14;
                layout(binding = 15) uniform sampler2D u_Tex15;

                vec4 GetDataFromTexture(int tid, vec2 uv, float tillingFactor)
                {
                    vec4 color = texture(u_Tex0, uv * tillingFactor) * (1 - abs(sign(tid - 0)));
                    color += texture(u_Tex1, uv * tillingFactor) * (1 - abs(sign(tid - 1)));
                    color += texture(u_Tex2, uv * tillingFactor) * (1 - abs(sign(tid - 2)));
                    color += texture(u_Tex3, uv * tillingFactor) * (1 - abs(sign(tid - 3)));
                    color += texture(u_Tex4, uv * tillingFactor) * (1 - abs(sign(tid - 4)));
                    color += texture(u_Tex5, uv * tillingFactor) * (1 - abs(sign(tid - 5)));
                    color += texture(u_Tex6, uv * tillingFactor) * (1 - abs(sign(tid - 6)));
                    color += texture(u_Tex7, uv * tillingFactor) * (1 - abs(sign(tid - 7)));
                    color += texture(u_Tex8, uv * tillingFactor) * (1 - abs(sign(tid - 8)));
                    color += texture(u_Tex9, uv * tillingFactor) * (1 - abs(sign(tid - 9)));
                    color += texture(u_Tex10, uv * tillingFactor) * (1 - abs(sign(tid - 10)));
                    color += texture(u_Tex11, uv * tillingFactor) * (1 - abs(sign(tid - 11)));
                    color += texture(u_Tex12, uv * tillingFactor) * (1 - abs(sign(tid - 12)));
                    color += texture(u_Tex13, uv * tillingFactor) * (1 - abs(sign(tid - 13)));
                    color += texture(u_Tex14, uv * tillingFactor) * (1 - abs(sign(tid - 14)));
                    color += texture(u_Tex15, uv * tillingFactor) * (1 - abs(sign(tid - 15)));
                    return color;
                }

                void main()
                {
                    o_Color = fs_in.color * GetDataFromTexture(fs_in.tid, fs_in.uv, fs_in.tillingFactor);
                }
            )";
            break;
        }
        case RendererAPI::API::Vulkan:    SB_CORE_THROW_INFO("Vulkan is not supported yet"); break;
        case RendererAPI::API::DX11: {
            vertexSource = R"(
                struct VSOut
                {
                    float4 color : Color;
                    float2 uv : UV;
                    int tid : TID;
                    float tillingFactor : TillingFactor;
                    float4 pos : SV_Position;
                };
                cbuffer ConstBuf
                {
                    matrix<float, 4, 4> u_ViewProjMat;
                }
                VSOut main(float4 pos : Position, float4 color : Color, float2 uv : UV, int tid : TID, float tillingFactor : TillingFactor)
                {
                    VSOut output;
                    output.color = color;
                    output.uv = uv;
                    output.tid = tid;
                    output.tillingFactor = tillingFactor;
                    output.pos = mul(pos, u_ViewProjMat);
                    return output;
                }
            )";

            fragmentSource = R"(
                struct FSIn
                {
                    float4 color : Color;
                    float2 uv : UV;
                    int tid : TID;
                    float tillingFactor : TillingFactor;
                };
                Texture2D<float4> u_Textures[16];
                SamplerState u_Samplers[16];
                float4 GetDataFromTexture(int tid, float2 uv, float tillingFactor)
                {
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
                float4 main(FSIn input) : SV_Target
                {
                    float4 output = input.color * GetDataFromTexture(input.tid, input.uv, input.tillingFactor);
                    return output;
                }
            )";
            break;
        }
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DirectX 12 is not supported yet"); break;
    }
}

void Renderer2D::Init() {
    s_Data.quadVertexData = new VertexData[static_cast<uint64_t>(c_MaxQuads) * 4];
    s_Data.quadInsert = s_Data.quadVertexData;

    static constexpr auto uv = std::to_array<glm::vec2>({
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f }
    });
    for (uint32_t i = 0; i < c_MaxQuads * 4; i += 4) {
        s_Data.quadVertexData[i + 0].uv = uv[0];
        s_Data.quadVertexData[i + 1].uv = uv[1];
        s_Data.quadVertexData[i + 2].uv = uv[2];
        s_Data.quadVertexData[i + 3].uv = uv[3];
    }

    auto* indices = new uint32_t[static_cast<uint64_t>(c_MaxQuads) * 6];
    uint32_t index = 0;
    for (uint32_t i = 0; i < c_MaxQuads * 6; i += 6) {
        indices[i + 0] = index + 0;
        indices[i + 1] = index + 1;
        indices[i + 2] = index + 2;

        indices[i + 3] = index + 2;
        indices[i + 4] = index + 3;
        indices[i + 5] = index + 0;

        index += 4;
    }

    std::string vertexSource, fragmentSource;
    GetShaderSource(vertexSource, fragmentSource);
    s_Data.shader = Renderer::GetShaderLibrary().Load("QuadShader", vertexSource, fragmentSource);
    UniformBuffer<sizeof(glm::mat4), 1> uniformBuffer;
    uniformBuffer.Push("u_ViewProjMat", glm::mat4(1.0f));
    s_Data.viewProj = ConstantBuffer::Create(BufferShaderBinding::Vertex, uniformBuffer);

    BufferLayout layout = {
        { "Position", BufferLayoutElementDataType::Float4 },
        { "Color", BufferLayoutElementDataType::Float4 },
        { "UV", BufferLayoutElementDataType::Float2 },
        { "TID", BufferLayoutElementDataType::Int },
        { "TillingFactor", BufferLayoutElementDataType::Float }
    };
    Ref<VertexBuffer> quadVBO = VertexBuffer::Create(layout, nullptr, c_MaxQuads * 4 * static_cast<uint32_t>(sizeof(VertexData)), BufferUsage::Dynamic);
    Ref<IndexBuffer> quadIBO = IndexBuffer::Create(indices, c_MaxQuads * 6 * static_cast<uint32_t>(sizeof(uint32_t)));
    s_Data.quadInputLayout = InputLayout::Create({ quadVBO }, s_Data.shader, quadIBO);
    delete[] indices;

    uint32_t texData = 0xffffffffu;
    s_Data.quadTextures[0] = Texture2D::Create(1u, 1u, &texData);
}
void Renderer2D::Shutdown() {
    for (uint32_t i = 0; i < c_MaxTextures; ++i)
        s_Data.quadTextures[i].Reset();
    s_Data.quadInputLayout.Reset();
    s_Data.viewProj.Reset();
    s_Data.shader.Reset();
    delete[] s_Data.quadVertexData;
}

void Renderer2D::SetViewProjectionMatrix(const glm::mat4& viewProjMat) {
    s_Data.viewProj->SetData(glm::value_ptr(viewProjMat), sizeof(glm::mat4));
}
void Renderer2D::SubmitQuad(const glm::vec2& pos, const glm::vec2& size, float rotation, const glm::vec4& color, const Ref<Texture2D>& texture, float tillingFactor) {
    SubmitQuad(glm::vec3(pos, 0.0f), size, rotation, color, texture, tillingFactor);
}
void Renderer2D::SubmitQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const glm::vec4& color, const Ref<Texture2D>& texture, float tillingFactor) {
    if (s_Data.quadCount >= c_MaxQuads)
        Draw();

    uint32_t tid = 0u;
    if (texture) {
        for (uint32_t i = 1u; i < s_Data.quadTextureIndex; ++i) {
            if (s_Data.quadTextures[i] == texture) {
                tid = i;
                break;
            }
        }
        if (tid == 0u) {
            if (s_Data.quadTextureIndex >= c_MaxTextures)
                Draw();

            tid = s_Data.quadTextureIndex++;
            s_Data.quadTextures[tid] = texture;
        }
    }

    static constexpr auto position = std::to_array<glm::vec3>({
        { -0.5f,  0.5f, 0.0f },
        {  0.5f,  0.5f, 0.0f },
        {  0.5f, -0.5f, 0.0f },
        { -0.5f, -0.5f, 0.0f }
    });

    glm::mat3 rotate = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    for (uint32_t i = 0; i < 4; ++i) {
        s_Data.quadInsert->pos = glm::vec4(rotation != 0.0f ? rotate * (position[i] * glm::vec3(size, 1.0f)) + pos : position[i] * glm::vec3(size, 1.0f) + pos, 1.0f);
        s_Data.quadInsert->color = color;
        s_Data.quadInsert->tid = tid;
        s_Data.quadInsert->tillingFactor = tillingFactor;
        ++s_Data.quadInsert;
    }

    ++s_Data.quadCount;
    ++s_Data.stats.QuadCount;
}
void Renderer2D::Draw()
{
    if (s_Data.quadCount == 0u)
        return;

    s_Data.quadInputLayout->GetVertexBuffer()->SetData(s_Data.quadVertexData, c_QuadBufferSize);
    s_Data.shader->Bind();
    s_Data.viewProj->Bind(0);

    for (uint32_t i = 0; i < s_Data.quadTextureIndex; ++i) {
        if (s_Data.quadTextures[i])
            s_Data.quadTextures[i]->Bind(i);
    }

    s_Data.quadInputLayout->Bind();
    RenderCommand::DrawIndexed(RendererAPI::Topology::Triangles, s_Data.quadCount * 6u);

    ++s_Data.stats.DrawCalls;
    s_Data.quadInsert = s_Data.quadVertexData;
    s_Data.quadCount = 0u;
    s_Data.quadTextureIndex = 1u;
}

Renderer2D::Stats Renderer2D::GetStats() noexcept {
    return s_Data.stats;
}
void Renderer2D::ResetStats() noexcept {
    s_Data.stats.QuadCount = 0u;
    s_Data.stats.DrawCalls = 0u;
}

}
