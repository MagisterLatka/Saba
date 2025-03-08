#pragma once

#include "Saba/Scene/Entity.h"

namespace Saba {

class SceneHierarchyPanel {
public:
    SB_CORE SceneHierarchyPanel() = default;
    SB_CORE SceneHierarchyPanel(Ref<Scene> scene) noexcept
        : m_Scene(std::move(scene)) {}
    SB_CORE ~SceneHierarchyPanel() = default;

    SB_CORE void SetScene(Ref<Scene> scene) noexcept;
    SB_CORE void SetSelected(Entity entity = {}) noexcept;
    SB_CORE Entity GetSelected() const noexcept { return m_Selected; }

    SB_CORE void OnUIRender();
private:
    SB_CORE void DrawEntityNode(Entity entity);
    SB_CORE void DrawComponents(Entity entity);
private:
    Ref<Scene> m_Scene;
    Entity m_Selected;
};

}