#include <pch.h>
#include "Scene.h"

#include "Saba/Scene/Entity.h"
#include "Saba/Renderer/Renderer2D.h"

namespace Saba {

Scene::~Scene() {
    m_Registry.clear();
}

Entity Scene::CreateEntity(std::string name) {
    auto entity = Entity(m_Registry.create(), this);

    if (name.empty())
        entity.AddComponent<TagComponent>("Entity");
    else
        entity.AddComponent<TagComponent>(std::move(name));
    
    entity.AddComponent<TransformComponent>();
    return entity;
}
void Scene::DestroyEntity(Entity entity) {
    m_Registry.destroy(entity.m_Handle);
}

void Scene::OnUpdate([[maybe_unused]] Timestep ts) {
    for (auto [entity, tc] : m_Registry.view<TransformComponent>(entt::exclude<SpriteComponent>).each()) {
        tc.Orientation = glm::mod(tc.Orientation, glm::two_pi<float>());
        const auto rotation = glm::quat(tc.Orientation);
        tc.Transform = glm::translate(glm::mat4(1.0f), tc.Position) * glm::scale(glm::toMat4(rotation), tc.Size);
    }

    {
        auto group = m_Registry.group<const SpriteComponent>(entt::get<const TransformComponent>);
        for (auto entity : group) {
            const auto& [tc, sc] = group.get<TransformComponent, SpriteComponent>(entity);
            Renderer2D::SubmitQuad(tc.Position, tc.Size, tc.Orientation.z, sc.Color, sc.Texture, sc.TillingFactor);
        }
        Renderer2D::DrawQuads();
    }
}

}
