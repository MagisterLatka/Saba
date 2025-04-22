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
    Mesh(Mesh&&) = default;
    Mesh& operator=(Mesh&&) = default;

    UUID GetID() const noexcept { return m_ID; }
    const std::vector<MeshVertex>& GetVertices() const noexcept { return m_Vertices; }
    const std::vector<uint32_t>& GetIndices() const noexcept { return m_Indices; }
private:
    std::vector<MeshVertex> m_Vertices;
    std::vector<uint32_t> m_Indices;
    UUID m_ID;
};

class MeshLibrary {
public:
    MeshLibrary() = default;
    ~MeshLibrary() = default;

    SB_CORE Ref<Mesh> Add(const std::string& name, Ref<Mesh> mesh);
    SB_CORE Ref<Mesh> Load(const std::string& name, std::vector<MeshVertex> vertices, std::vector<uint32_t> indices);
    SB_CORE Ref<Mesh> Get(const std::string& name) const;
private:
    std::unordered_map<std::string, Ref<Mesh>> m_Meshes;
};

} //namespace Saba
