#pragma once

#include "Saba/Core/Core.h"
#include "Saba/Core/Timestep.h"

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

    SB_CORE void OnUpdate(Timestep ts);
private:
    std::string m_Name;
    entt::registry m_Registry;
};

}
