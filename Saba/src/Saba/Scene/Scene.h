#pragma once

#include "Saba/Core/Core.h"
#include "Saba/Core/Timestep.h"
#include "Saba/Scene/Components.h"
#include "Saba/Renderer/Camera.h"
#include "Saba/Events/Event.h"

#include <entt.hpp>

namespace Saba {

class Entity;
class Scene : public RefCounted {
    friend class Entity;
public:
    SB_CORE Scene(std::string name = "Scene") noexcept
        : m_Name(std::move(name)) {}
    SB_CORE ~Scene();

    SB_CORE Entity CreateEntity(std::string name = {});
    SB_CORE void DestroyEntity(Entity entity);

    SB_CORE Entity SetCameraEntity(Entity entity);
    SB_CORE Entity CreateAndSetCameraEntity();
    SB_CORE Entity CreateAndSetCameraEntity(Ref<Camera> camera);
    
    SB_CORE void OnEvent(Event& e);
    SB_CORE void OnUpdate(Timestep ts);
    SB_CORE void OnViewportResize(uint32_t width, uint32_t height);
private:
    template<Component T>
    void OnComponentAdd(Entity entity, T& component);
private:
    std::string m_Name;

    entt::registry m_Registry;
    entt::entity m_Camera = entt::null;

    glm::uvec2 m_ViewportSize = { 0u, 0u };
};

}
