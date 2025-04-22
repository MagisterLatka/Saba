#pragma once

#include "Saba/Scene/Entity.h"

namespace Saba {

class SceneHierarchyPanel {
public:
    SceneHierarchyPanel() = default;
    SceneHierarchyPanel(Ref<Scene> scene) noexcept
        : m_Scene(std::move(scene)) {}
    ~SceneHierarchyPanel() = default;

    SB_CORE void SetScene(Ref<Scene> scene) noexcept;
    SB_CORE void SetSelected(Entity entity = {}) noexcept;
    Entity GetSelected() const noexcept { return m_Selected; }

    SB_CORE void Open() noexcept;

    SB_CORE void OnUIRender();
private:
    void DrawEntityNode(Entity entity);
    static void DrawComponents(Entity entity);
private:
    Ref<Scene> m_Scene;
    Entity m_Selected;
    bool m_Open;
};

}
