#pragma once

#include "Saba/Renderer/Mesh.h"
#include "Saba/Renderer/Material.h"

namespace Saba {

class Model : public RefCounted {
public:
    SB_CORE Model() = default;
    SB_CORE Model(std::initializer_list<Ref<Mesh>> meshes, Ref<Material> material = {});
    SB_CORE Model(const std::filesystem::path& filepath, Ref<Material> material = {});

    SB_CORE void AddMesh(Ref<Mesh> mesh);
    SB_CORE const std::vector<Ref<Mesh>>& GetMeshes() const { return m_Meshes; }

    SB_CORE void SetMaterial(Ref<Material> material) { m_Material = std::move(material); }
    SB_CORE Ref<Material> GetMaterial() const { return m_Material; }
private:
    SB_CORE void ProcessModel(const std::filesystem::path& filepath);
private:
    std::vector<Ref<Mesh>> m_Meshes;
    Ref<Material> m_Material;
    UUID m_ID;
};

}