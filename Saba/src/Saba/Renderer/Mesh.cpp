#include <pch.h>
#include "Mesh.h"

namespace Saba {

Mesh::Mesh(std::vector<MeshVertex> vertices, std::vector<uint32_t> indices)
    : m_Vertices(std::move(vertices)), m_Indices(std::move(indices)) {}

Ref<Mesh> MeshLibrary::Add(const std::string& name, Ref<Mesh> mesh) {
    SB_CORE_ASSERT(!m_Meshes.contains(name), "Mesh {0} already exists", name);
    return m_Meshes.emplace(name, mesh).first->second;
}
Ref<Mesh> MeshLibrary::Load(const std::string& name, std::vector<MeshVertex> vertices, std::vector<uint32_t> indices) {
    SB_CORE_ASSERT(!m_Meshes.contains(name), "Mesh {0} already exists", name);
    return m_Meshes.emplace(name, Ref<Mesh>::Create(std::move(vertices), std::move(indices))).first->second;
}
Ref<Mesh> MeshLibrary::Get(const std::string& name) const {
    SB_CORE_ASSERT(m_Meshes.contains(name), "Mesh {0} does not exists", name);
    return m_Meshes.at(name);
}

}
