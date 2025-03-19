#include <pch.h>
#include "Renderer3D.h"

#include "Saba/Renderer/InputLayout.h"
#include "Saba/Renderer/Renderer.h"
#include "Saba/Renderer/RenderCommand.h"

namespace Saba {


struct MeshInstanceData {
    glm::mat4 transform;
    uint32_t entityID;
};
static constexpr uint32_t c_MaxInstances = 1000u;
static constexpr uint32_t c_InstanceBufferSize = c_MaxInstances * static_cast<uint32_t>(sizeof(MeshInstanceData));
static constexpr uint32_t c_MaxLights = 100u;

struct MeshData {
    Ref<InputLayout> inputLayout;
    MeshInstanceData* instanceData, *instanceInsert;
    uint32_t instanceCount = 0u;
};
struct LightData {
    glm::vec4 lightPos;
    glm::vec4 lightColor;
    glm::vec4 lightRadiusConstants;
};
struct LightsBufferData {
    LightData lights[c_MaxLights];
    glm::vec4 viewPos_LightsCount;
};
struct Renderer3DData {
    std::unordered_map<UUID, MeshData> meshes;

    LightsBufferData* lightsData;
    LightData* lightsInsert;
    uint32_t lightsCount = 0u;
    glm::vec3 viewPos;
    Ref<ConstantBuffer> lightsBuffer;

    Ref<Shader> shader;
    Ref<ConstantBuffer> viewProj;
};
static Renderer3DData s_Data;

void Renderer3D::Init() {
    s_Data.shader = Renderer::GetShaderLibrary().Get("meshClearShader");

    UniformBuffer<sizeof(glm::mat4), 1> uniformBuffer;
    uniformBuffer.Push("u_ViewProjMat", glm::mat4(1.0f));
    s_Data.viewProj = ConstantBuffer::Create(BufferShaderBinding::Vertex, uniformBuffer);

    Buffer lightsBuffer = Buffer(new LightsBufferData, sizeof(LightsBufferData));
    s_Data.lightsBuffer = ConstantBuffer::Create(BufferShaderBinding::Fragment, std::move(lightsBuffer));
    s_Data.lightsData = s_Data.lightsBuffer->GetLocalData().As<LightsBufferData>();
    s_Data.lightsInsert = s_Data.lightsData->lights;

    std::vector<Saba::MeshVertex> vertices = {
        { { -0.1f, -0.1f, -0.1f }, {  0.0f, -1.0f,  0.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f} },
        { {  0.1f, -0.1f, -0.1f }, {  0.0f, -1.0f,  0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f} },
        { {  0.1f, -0.1f,  0.1f }, {  0.0f, -1.0f,  0.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f} },
        { { -0.1f, -0.1f,  0.1f }, {  0.0f, -1.0f,  0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f} },

        { { -0.1f,  0.1f, -0.1f }, {  0.0f,  1.0f,  0.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f} },
        { {  0.1f,  0.1f, -0.1f }, {  0.0f,  1.0f,  0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f} },
        { {  0.1f,  0.1f,  0.1f }, {  0.0f,  1.0f,  0.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f} },
        { { -0.1f,  0.1f,  0.1f }, {  0.0f,  1.0f,  0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f} },

        { { -0.1f, -0.1f, -0.1f }, { -1.0f,  0.0f,  0.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f} },
        { { -0.1f,  0.1f, -0.1f }, { -1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f} },
        { { -0.1f,  0.1f,  0.1f }, { -1.0f,  0.0f,  0.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f} },
        { { -0.1f, -0.1f,  0.1f }, { -1.0f,  0.0f,  0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f} },

        { {  0.1f, -0.1f, -0.1f }, {  1.0f,  0.0f,  0.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f} },
        { {  0.1f,  0.1f, -0.1f }, {  1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f} },
        { {  0.1f,  0.1f,  0.1f }, {  1.0f,  0.0f,  0.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f} },
        { {  0.1f, -0.1f,  0.1f }, {  1.0f,  0.0f,  0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f} },

        { { -0.1f, -0.1f, -0.1f }, {  0.0f,  0.0f, -1.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f} },
        { {  0.1f, -0.1f, -0.1f }, {  0.0f,  0.0f, -1.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f} },
        { {  0.1f,  0.1f, -0.1f }, {  0.0f,  0.0f, -1.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f} },
        { { -0.1f,  0.1f, -0.1f }, {  0.0f,  0.0f, -1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f} },

        { { -0.1f, -0.1f,  0.1f }, {  0.0f,  0.0f,  1.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f} },
        { {  0.1f, -0.1f,  0.1f }, {  0.0f,  0.0f,  1.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f} },
        { {  0.1f,  0.1f,  0.1f }, {  0.0f,  0.0f,  1.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f} },
        { { -0.1f,  0.1f,  0.1f }, {  0.0f,  0.0f,  1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f} }
    };
    std::vector<uint32_t> indices = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20
    };
    auto mesh = Renderer::GetMeshLibrary().Load("Cube", std::move(vertices), std::move(indices));
    InitMesh(mesh);
}
void Renderer3D::InitMesh(Ref<Mesh> mesh) {
    MeshData meshData;

    BufferLayout verticesLayout = {
        { "Position", BufferLayoutElementDataType::Float3 },
        { "Normal", BufferLayoutElementDataType::Float3 },
        { "UV", BufferLayoutElementDataType::Float2 },
        { "Color", BufferLayoutElementDataType::Float4 }
    };
    Ref<VertexBuffer> verticesVBO = VertexBuffer::Create(std::move(verticesLayout), mesh->GetVertices().data(),
        static_cast<uint32_t>(mesh->GetVertices().size() * sizeof(MeshVertex)), BufferUsage::Immutable);
    
    BufferLayout instancesLayout = {
        { "Transform", BufferLayoutElementDataType::Mat4, false, 1u },
        { "EntityID", BufferLayoutElementDataType::UInt, false, 1u }
    };
    Buffer instancesBuffer = Buffer(new MeshInstanceData[c_MaxInstances], c_InstanceBufferSize, true);
    Ref<VertexBuffer> instancesVBO = VertexBuffer::Create(std::move(instancesLayout), std::move(instancesBuffer), BufferUsage::Dynamic);
    meshData.instanceData = reinterpret_cast<MeshInstanceData*>(instancesVBO->GetLocalData().Data);
    meshData.instanceInsert = meshData.instanceData;

    Ref<IndexBuffer> ibo = IndexBuffer::Create(mesh->GetIndices().data(), static_cast<uint32_t>(mesh->GetIndices().size() * sizeof(uint32_t)),
        BufferUsage::Immutable);
    meshData.inputLayout = InputLayout::Create({ verticesVBO, instancesVBO }, s_Data.shader, ibo);

    s_Data.meshes.emplace(mesh->GetID(), std::move(meshData));
}
void Renderer3D::Shutdown() {
    for (auto& mesh : s_Data.meshes | std::views::values) {
        mesh.inputLayout.Reset();
    }
    s_Data.shader.Reset();
    s_Data.viewProj.Reset();
}

void Renderer3D::SetShader(Ref<Shader> shader) {
    if (shader)
        s_Data.shader = shader;
    else
        s_Data.shader = Renderer::GetShaderLibrary().Get("meshClearShader");
}
void Renderer3D::SetViewProjectionMatrix(const glm::mat4& viewProjMat) {
    switch (RendererAPI::GetAPI()) {
        default: break;
        case RendererAPI::API::None:
            SB_CORE_THROW_INFO("None API is not supported.");
            break;
        case RendererAPI::API::OpenGL:
        case RendererAPI::API::Vulkan:
            s_Data.viewProj->SetData(glm::value_ptr(viewProjMat), sizeof(glm::mat4));
            break;
        case RendererAPI::API::DX11:
        case RendererAPI::API::DX12:
            glm::mat4 transposed = glm::transpose(viewProjMat);
            s_Data.viewProj->SetData(glm::value_ptr(transposed), sizeof(glm::mat4));
            break;
    }
}
void Renderer3D::SetCameraPosition(const glm::vec3& pos) {
    s_Data.viewPos = pos;
}

void Renderer3D::Draw() {
    for (auto id : s_Data.meshes | std::views::keys)
        DrawMesh(id);
}

void Renderer3D::SubmitMesh(Ref<Mesh> mesh) {
    if (s_Data.meshes.contains(mesh->GetID()))
        SB_CORE_WARN("Renderer3D already has mesh with ID: {0}.", static_cast<uint64_t>(mesh->GetID()));
    else
        InitMesh(mesh);
}
void Renderer3D::SubmitMeshInstance(Ref<Mesh> mesh, const glm::mat4& transform, uint32_t entityID) {
    if (!s_Data.meshes.contains(mesh->GetID()))
        InitMesh(mesh);

    SubmitMeshInstance(mesh->GetID(), transform, entityID);
}
void Renderer3D::SubmitMeshInstance(UUID meshID, const glm::mat4& transform, uint32_t entityID) {
    SB_CORE_ASSERT(s_Data.meshes.contains(meshID), "Renderer3D does not have mesh with ID: {0}.", static_cast<uint32_t>(meshID));

    auto& meshData = s_Data.meshes[meshID];
    if (meshData.instanceCount >= c_MaxInstances) {
        DrawMesh(meshID);
        Renderer::Render();
    }

    switch (RendererAPI::GetAPI()) {
    default: break;
    case RendererAPI::API::None:
        SB_CORE_THROW_INFO("None API is not supported.");
        break;
    case RendererAPI::API::OpenGL:
    case RendererAPI::API::Vulkan:
        meshData.instanceInsert->transform = transform;
        break;
    case RendererAPI::API::DX11:
    case RendererAPI::API::DX12:
        meshData.instanceInsert->transform = glm::transpose(transform);
        break;
    }
    meshData.instanceInsert->entityID = entityID;
    ++meshData.instanceInsert;
    ++meshData.instanceCount;
}

void Renderer3D::DrawMesh(Ref<Mesh> mesh) {
    if (!s_Data.meshes.contains(mesh->GetID())) {
        SB_CORE_WARN("Renderer3D does not have mesh with ID: {0}.", static_cast<uint64_t>(mesh->GetID()));
        return;
    }
    DrawMesh(mesh->GetID());
}
void Renderer3D::DrawMesh(UUID meshID) {
    if (!s_Data.meshes.contains(meshID)) {
        SB_CORE_WARN("Renderer3D does not have mesh with ID: {0}.", static_cast<uint64_t>(meshID));
        return;
    }

    auto& meshData = s_Data.meshes[meshID];
    if (meshData.instanceCount == 0u)
        return;

    meshData.inputLayout->GetVertexBuffer(1u)->UploadCurrent();
    meshData.inputLayout->Bind();

    s_Data.shader->Bind();
    s_Data.viewProj->Bind(0u);

    s_Data.lightsData->viewPos_LightsCount = glm::vec4(s_Data.viewPos, static_cast<float>(s_Data.lightsCount));
    s_Data.lightsBuffer->UploadCurrent();
    s_Data.lightsBuffer->Bind(1u);

    RenderCommand::DrawIndexedInstanced(RendererAPI::Topology::Triangles, meshData.inputLayout->GetIndexBuffer()->GetIndicesCount(), meshData.instanceCount);

    meshData.instanceInsert = meshData.instanceData;
    meshData.instanceCount = 0u;
}

void Renderer3D::SubmitLight(const glm::vec3& lightPos, const glm::vec3& lightColor, float radius) {
    if (s_Data.lightsCount >= c_MaxLights) {
        SB_CORE_WARN("Exceeded number of simultaneous lights of Renderer3D");
        return;
    }

    s_Data.lightsInsert->lightPos = glm::vec4(lightPos, 0.0f);
    s_Data.lightsInsert->lightColor = glm::vec4(lightColor, 0.0f);
    s_Data.lightsInsert->lightRadiusConstants = glm::vec4(1.0f, 4.5f / radius, 75.0f / (radius * radius), 0.0f);
    ++s_Data.lightsInsert;
    ++s_Data.lightsCount;
}
void Renderer3D::ResetLights() {
    s_Data.lightsInsert = s_Data.lightsData->lights;
    s_Data.lightsCount = 0u;
}

}
