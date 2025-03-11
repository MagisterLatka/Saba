#pragma once

#include "Saba/Scene/Scene.h"
#include "Saba/Scene/Components.h"

namespace Saba {

class Entity {
    friend class Scene;
public:
    SB_CORE Entity() noexcept = default;
    SB_CORE Entity(entt::entity handle, Scene* scene) noexcept
        : m_Handle(handle), m_Scene(scene) {}
    SB_CORE ~Entity() noexcept = default;

    template<Component T>
    bool HasComponent() const {
        return m_Scene->m_Registry.all_of<T>(m_Handle);
    }
    template<Component T, typename ...Args>
    requires(std::is_constructible_v<T, Args...>)
    T& AddComponent(Args&& ...args) {
        SB_CORE_ASSERT(!HasComponent<T>(), "Entity already has this component");
        auto& component = m_Scene->m_Registry.emplace<T>(m_Handle, std::forward<Args>(args)...);
        m_Scene->OnComponentAdd<T>(*this, component);
        return component;
    }
    template<Component T>
    void RemoveComponent() {
        SB_CORE_ASSERT(HasComponent<T>(), "Entity does not have this component");
        m_Scene->m_Registry.erase<T>(m_Handle);
    }
    template<Component T>
    T& GetComponent() {
        SB_CORE_ASSERT(HasComponent<T>(), "Entity does not have this component");
        return m_Scene->m_Registry.get<T>(m_Handle);
    }
    template<Component T>
    const T& GetComponent() const {
        SB_CORE_ASSERT(HasComponent<T>(), "Entity does not have this component");
        return m_Scene->m_Registry.get<const T>(m_Handle);
    }

    SB_CORE const Scene* GetScene() const noexcept { return m_Scene; }

    SB_CORE glm::mat4& GetTransform() { return m_Scene->m_Registry.get<TransformComponent>(m_Handle).Transform; }
    SB_CORE const glm::mat4& GetTransform() const { return m_Scene->m_Registry.get<TransformComponent>(m_Handle).Transform; }
    SB_CORE TransformComponent& GetTransformComponent() { return m_Scene->m_Registry.get<TransformComponent>(m_Handle); }
    SB_CORE const TransformComponent& GetTransformComponent() const { return m_Scene->m_Registry.get<TransformComponent>(m_Handle); }

    SB_CORE operator uint32_t() const noexcept { return static_cast<uint32_t>(m_Handle); }
    SB_CORE operator bool() const noexcept { return m_Handle != entt::null && m_Scene != nullptr; }
    SB_CORE operator entt::entity() const noexcept { return m_Handle; }

    SB_CORE bool operator==(const Entity& other) const noexcept { return m_Handle == other.m_Handle && m_Scene == other.m_Scene; }
private:
    entt::entity m_Handle = entt::null;
    Scene* m_Scene = nullptr;
};

}
