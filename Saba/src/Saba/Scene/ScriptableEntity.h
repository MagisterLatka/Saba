#pragma once

#include "Saba/Scene/Entity.h"
#include "Saba/Events/Event.h"

namespace Saba {

class ScriptableEntity {
    friend class Scene;
public:
    virtual ~ScriptableEntity() noexcept = default;
protected:
    template<Component T>
    bool HasComponent() {
        return m_Entity.HasComponent<T>();
    }
    template<Component T, typename ...Args>
    T& AddComponent(Args&& ...args) {
        return  m_Entity.AddComponent<T>(std::forward<Args>(args)...);
    }
    template<Component T>
    void RemoveComponent() {
        m_Entity.RemoveComponent<T>();
    }
    template<Component T>
    T& GetComponent() {
        return m_Entity.GetComponent<T>();
    }

    const Scene* GetScene() { return m_Entity.GetScene(); }
    glm::mat4& GetTransform() { return m_Entity.GetTransform(); }

    TransformComponent& GetTransformComponent() { return m_Entity.GetTransformComponent(); }

    virtual void OnCreate() {}
    virtual void OnDestroy() {}
    virtual void OnEvent([[maybe_unused]] Event& e) {}
    virtual void OnUpdate([[maybe_unused]] Timestep ts) {}
    virtual void OnViewportResize([[maybe_unused]] uint32_t width, [[maybe_unused]] uint32_t height) {}
private:
    Entity m_Entity;
};

} //namespace Saba
