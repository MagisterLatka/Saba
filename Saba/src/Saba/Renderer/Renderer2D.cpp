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

static constexpr uint32_t c_MaxQuads = 50000u;
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

void Renderer2D::Init() {
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

    s_Data.shader = Renderer::GetShaderLibrary().Get("2D");
    UniformBuffer<sizeof(glm::mat4), 1> uniformBuffer;
    uniformBuffer.Push("u_ViewProjMat", glm::mat4(1.0f));
    s_Data.viewProj = ConstantBuffer::Create(BufferShaderBinding::Vertex, uniformBuffer);

    BufferLayout layout = {
        { "Position", BufferLayoutElementDataType::Float4 },
        { "Color", BufferLayoutElementDataType::Float4 },
        { "UV", BufferLayoutElementDataType::Float2 },
        { "TID", BufferLayoutElementDataType::UInt },
        { "TillingFactor", BufferLayoutElementDataType::Float }
    };
    Buffer quadVertexDataBuffer = Buffer(new VertexData[static_cast<uint64_t>(c_MaxQuads) * 4], c_QuadBufferSize, true);
    Ref<VertexBuffer> quadVBO = VertexBuffer::Create(layout, std::move(quadVertexDataBuffer), BufferUsage::Dynamic);
    s_Data.quadVertexData = reinterpret_cast<VertexData*>(quadVBO->GetLocalData().Data);
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
}

void Renderer2D::SetViewProjectionMatrix(const glm::mat4& viewProjMat) {
    s_Data.viewProj->SetData(glm::value_ptr(viewProjMat), sizeof(glm::mat4));
}
void Renderer2D::SubmitQuad(const glm::vec2& pos, const glm::vec2& size, float rotation, const glm::vec4& color, Ref<Texture2D> texture, float tillingFactor) {
    SubmitQuad(glm::vec3(pos, 0.0f), size, rotation, color, texture, tillingFactor);
}
void Renderer2D::SubmitQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const glm::vec4& color, Ref<Texture2D> texture, float tillingFactor) {
    if (s_Data.quadCount >= c_MaxQuads) {
        DrawQuads();
        Renderer::Render();
    }

    uint32_t tid = 0u;
    if (texture) {
        for (uint32_t i = 1u; i < s_Data.quadTextureIndex; ++i) {
            if (s_Data.quadTextures[i] == texture) {
                tid = i;
                break;
            }
        }
        if (tid == 0u) {
            if (s_Data.quadTextureIndex >= c_MaxTextures) {
                DrawQuads();
                Renderer::Render();
            }

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
    const float sinRot = glm::sin(rotation), cosRot = glm::cos(rotation);
    for (uint32_t i = 0; i < 4; ++i) {
        s_Data.quadInsert->pos = glm::vec4((cosRot * position[i].x - sinRot * position[i].y) * size.x + pos.x,
            (sinRot * position[i].x + cosRot * position[i].y) * size.y + pos.y, pos.z, 1.0f);
        s_Data.quadInsert->color = color;
        s_Data.quadInsert->tid = tid;
        s_Data.quadInsert->tillingFactor = tillingFactor;
        ++s_Data.quadInsert;
    }

    ++s_Data.quadCount;
    ++s_Data.stats.QuadCount;
}
void Renderer2D::SubmitQuad(const glm::mat4& transform, const glm::vec4& color, Ref<Texture2D> texture, float tillingFactor) {
    if (s_Data.quadCount >= c_MaxQuads) {
        DrawQuads();
        Renderer::Render();
    }

    uint32_t tid = 0u;
    if (texture) {
        for (uint32_t i = 1u; i < s_Data.quadTextureIndex; ++i) {
            if (s_Data.quadTextures[i] == texture) {
                tid = i;
                break;
            }
        }
        if (tid == 0u) {
            if (s_Data.quadTextureIndex >= c_MaxTextures) {
                DrawQuads();
                Renderer::Render();
            }

            tid = s_Data.quadTextureIndex++;
            s_Data.quadTextures[tid] = texture;
        }
    }

    static constexpr auto position = std::to_array<glm::vec4>({
        { -0.5f,  0.5f, 0.0f, 1.0f },
        {  0.5f,  0.5f, 0.0f, 1.0f },
        {  0.5f, -0.5f, 0.0f, 1.0f },
        { -0.5f, -0.5f, 0.0f, 1.0f }
    });
    for (uint32_t i = 0; i < 4; ++i) {
        s_Data.quadInsert->pos = transform * position[i];
        s_Data.quadInsert->color = color;
        s_Data.quadInsert->tid = tid;
        s_Data.quadInsert->tillingFactor = tillingFactor;
        ++s_Data.quadInsert;
    }

    ++s_Data.quadCount;
    ++s_Data.stats.QuadCount;
}
void Renderer2D::DrawQuads()
{
    if (s_Data.quadCount == 0u)
        return;

    s_Data.quadInputLayout->GetVertexBuffer()->UploadCurrent();
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
