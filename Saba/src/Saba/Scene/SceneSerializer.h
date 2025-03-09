#pragma once

#include "Saba/Scene/Scene.h"

namespace Saba {

class SceneSerializer {
public:
    SB_CORE SceneSerializer(Ref<Scene> scene) noexcept
        : m_Scene(std::move(scene)) {}

    SB_CORE void Serialize(const std::filesystem::path& filepath);
    SB_CORE bool Deserialize(const std::filesystem::path& filepath);
private:
    Ref<Scene> m_Scene;
};

}
