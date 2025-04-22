#pragma once

#include "Saba/Renderer/Mesh.h"
#include "Saba/Renderer/Material.h"

namespace Saba {

class Model : public RefCounted {
public:
    Model() = default;
    SB_CORE Model(std::initializer_list<Ref<Mesh>> meshes, Ref<Material> material = {});
    SB_CORE Model(const std::filesystem::path& filepath, Ref<Material> material = {});

    SB_CORE void AddMesh(Ref<Mesh> mesh);
    const std::vector<Ref<Mesh>>& GetMeshes() const { return m_Meshes; }

    void SetMaterial(Ref<Material> material) { m_Material = std::move(material); }
    Ref<Material> GetMaterial() const { return m_Material; }
private:
    void ProcessModel(const std::filesystem::path& filepath);
private:
    std::vector<Ref<Mesh>> m_Meshes;
    Ref<Material> m_Material;
    UUID m_ID;
};

} //namespace Saba
