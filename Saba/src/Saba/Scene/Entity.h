#pragma once

#include "Saba/Scene/Scene.h"
#include "Saba/Scene/Components.h"

namespace Saba {

class Entity {
    friend class Scene;
public:
    Entity() noexcept = default;
    Entity(entt::entity handle, Scene* scene) noexcept
        : m_Handle(handle), m_Scene(scene) {}
    ~Entity() noexcept = default;

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
    template<Component T, typename ...Args>
    requires(std::is_constructible_v<T, Args...>)
    T& AddOrReplaceComponent(Args&& ...args) {
        T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_Handle, std::forward<Args>(args)...);
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

    const Scene* GetScene() const noexcept { return m_Scene; }

    UUID GetID() const noexcept { return GetComponent<IDComponent>().ID; }

    glm::mat4& GetTransform() { return m_Scene->m_Registry.get<TransformComponent>(m_Handle).Transform; }
    const glm::mat4& GetTransform() const { return m_Scene->m_Registry.get<TransformComponent>(m_Handle).Transform; }
    TransformComponent& GetTransformComponent() { return m_Scene->m_Registry.get<TransformComponent>(m_Handle); }
    const TransformComponent& GetTransformComponent() const { return m_Scene->m_Registry.get<TransformComponent>(m_Handle); }

    operator uint32_t() const noexcept { return static_cast<uint32_t>(m_Handle); }
    operator bool() const noexcept { return m_Handle != entt::null && m_Scene != nullptr; }
    operator entt::entity() const noexcept { return m_Handle; }

    bool operator==(const Entity& other) const noexcept { return m_Handle == other.m_Handle && m_Scene == other.m_Scene; }
private:
    entt::entity m_Handle = entt::null;
    Scene* m_Scene = nullptr;
};

} //namespace Saba
