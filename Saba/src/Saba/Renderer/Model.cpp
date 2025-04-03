#include <pch.h>
#include "Model.h"

#include "Saba/Renderer/RendererAPI.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Saba {

Model::Model(std::initializer_list<Ref<Mesh>> meshes, Ref<Material> material)
    : m_Meshes(meshes), m_Material(std::move(material)) {}

Model::Model(const std::filesystem::path& filepath, Ref<Material> material)
    : m_Material(std::move(material))
{
    ProcessModel(filepath);
}

void Model::AddMesh(Ref<Mesh> mesh) {
    m_Meshes.emplace_back(std::move(mesh));
}

void Model::ProcessModel(const std::filesystem::path& filepath) {
    Assimp::Importer importer;
    const std::string path = filepath.string();
    uint32_t flags = aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals |
        aiProcess_ImproveCacheLocality | aiProcess_GenUVCoords | aiProcess_OptimizeMeshes;
    if (RendererAPI::GetAPI() == RendererAPI::API::OpenGL)
        flags |= aiProcess_FlipUVs;
    const auto* scene = importer.ReadFile(path.c_str(), flags);

    if (scene == nullptr || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0u || scene->mRootNode == nullptr) {
        SB_CORE_ERROR("Failed to load model from {0}\n{1}", path, importer.GetErrorString());
        return;
    }

    for (uint32_t n = 0; n < scene->mNumMeshes; ++n) {
        const auto* mesh = scene->mMeshes[n];
        std::vector<MeshVertex> vertices;
        vertices.reserve(mesh->mNumVertices);
        for (uint32_t i = 0; i < mesh->mNumVertices; ++i) {
            MeshVertex vertex;
            vertex.Pos = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.0f };
            vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
            vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
            vertex.UV = mesh->mTextureCoords[0] != nullptr ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) : glm::vec2(0.0f, 0.0f);
            vertex.Color = mesh->mColors[0] != nullptr ? glm::vec4(mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b, mesh->mColors[0][i].a)
                : glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            vertices.emplace_back(vertex);
        }
        std::vector<uint32_t> indices;
        indices.reserve(static_cast<uint64_t>(mesh->mNumFaces) * 3u);
        for (uint32_t i = 0; i < mesh->mNumFaces; ++i) {
            const auto face = mesh->mFaces[i];
            for (uint32_t j = 0; j < face.mNumIndices; ++j) {
                indices.push_back(face.mIndices[j]);
            }
        }
        m_Meshes.emplace_back(Ref<Mesh>::Create(std::move(vertices), std::move(indices)));
    }
}

}