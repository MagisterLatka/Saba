#pragma once

#include "Saba/Core/Core.h"
#include "Saba/Core/UUID.h"

namespace Saba {

struct MeshVertex {
    glm::vec4 Pos;
    glm::vec3 Normal;
    glm::vec3 Tangent;
    glm::vec2 UV;
    glm::vec4 Color;
};
class Mesh : public RefCounted {
public:
    SB_CORE Mesh(std::vector<MeshVertex> vertices, std::vector<uint32_t> indices);

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    SB_CORE Mesh(Mesh&&) = default;
    SB_CORE Mesh& operator=(Mesh&&) = default;

    SB_CORE UUID GetID() const noexcept { return m_ID; }
    SB_CORE const std::vector<MeshVertex>& GetVertices() const noexcept { return m_Vertices; }
    SB_CORE const std::vector<uint32_t>& GetIndices() const noexcept { return m_Indices; }
private:
    std::vector<MeshVertex> m_Vertices;
    std::vector<uint32_t> m_Indices;
    UUID m_ID;
};

class MeshLibrary {
public:
    SB_CORE MeshLibrary() = default;
    SB_CORE ~MeshLibrary() = default;

    SB_CORE Ref<Mesh> Add(const std::string& name, Ref<Mesh> mesh);
    SB_CORE Ref<Mesh> Load(const std::string& name, std::vector<MeshVertex> vertices, std::vector<uint32_t> indices);
    SB_CORE Ref<Mesh> Get(const std::string& name) const;
private:
    std::unordered_map<std::string, Ref<Mesh>> m_Meshes;
};

}
