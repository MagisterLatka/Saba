#include <pch.h>
#include "Scene.h"

#include "Saba/Scene/ScriptableEntity.h"
#include "Saba/Renderer/Renderer2D.h"

namespace Saba {

static uint32_t s_SceneCounter = 0u;
Scene::Scene(std::string name) noexcept
    : m_Name(std::move(name)), m_ID(s_SceneCounter++)
{
    m_SceneEntity = m_Registry.create();
    m_Registry.emplace<SceneComponent>(m_SceneEntity, m_ID);
    m_Registry.emplace<TagComponent>(m_SceneEntity, "SceneEntity");
}
Scene::~Scene() {
    m_Registry.clear();
}

Entity Scene::CreateEntity(std::string name) {
    return CreateEntityWithID(UUID(), std::move(name));
}
Entity Scene::CreateEntityWithID(UUID id, std::string name) {
    auto entity = Entity(m_Registry.create(), this);
    
    entity.AddComponent<IDComponent>(id);
    entity.AddComponent<TransformComponent>();
    auto& tag = entity.AddComponent<TagComponent>(std::move(name)).Tag;
    if (tag.empty()) tag = "Entity";
    return entity;
}
void Scene::DestroyEntity(Entity entity) {
    m_Registry.destroy(entity.m_Handle);
}

Entity Scene::SetCameraEntity(Entity entity) {
    m_Camera = entity;
    return entity;
}
Entity Scene::CreateAndSetCameraEntity() {
    auto entity = CreateEntity("Camera");
    entity.AddComponent<CameraComponent>(Ref<OrthographicCamera>::Create(16.0f / 9.0f));
    m_Camera = entity;
    return entity;
}
Entity Scene::CreateAndSetCameraEntity(Ref<Camera> camera) {
    auto entity = CreateEntity("Camera");
    entity.AddComponent<CameraComponent>(std::move(camera));
    m_Camera = entity;
    return entity;
}

void Scene::OnEvent(Event& e) {
    for (auto [entity, nsc] : m_Registry.view<NativeScriptComponent>().each()) {
        if (nsc.Instance == nullptr) {
            nsc.Instance = nsc.InstantiateScript();
            nsc.Instance->m_Entity = Entity(entity, this);
            nsc.Instance->OnCreate();
        }
        nsc.Instance->OnEvent(e);
    }
}
void Scene::OnUpdate([[maybe_unused]] Timestep ts) {
    for (auto [entity, nsc] : m_Registry.view<NativeScriptComponent>().each()) {
        if (nsc.Instance == nullptr) {
            nsc.Instance = nsc.InstantiateScript();
            nsc.Instance->m_Entity = Entity(entity, this);
            nsc.Instance->OnCreate();
        }
        nsc.Instance->OnUpdate(ts);
    }
    for (auto [entity, tc] : m_Registry.view<TransformComponent>(entt::exclude<SpriteComponent>).each()) {
        tc.Orientation = glm::mod(tc.Orientation, glm::two_pi<float>());
        const auto rotation = glm::quat(tc.Orientation);
        tc.Transform = glm::translate(glm::mat4(1.0f), tc.Position) * glm::scale(glm::toMat4(rotation), tc.Size);
    }

    if (m_Camera != entt::null) {
        auto camera = m_Registry.get<CameraComponent>(m_Camera).Camera;
        const auto& transform = m_Registry.get<TransformComponent>(m_Camera).Transform;
        Renderer2D::SetViewProjectionMatrix(camera->GetProjectionMatrix() * glm::inverse(transform));

        auto groupQuads = m_Registry.group<const SpriteComponent>(entt::get<const TransformComponent>);
        for (auto entity : groupQuads) {
            const auto& [tc, sc] = groupQuads.get<TransformComponent, SpriteComponent>(entity);
            Renderer2D::SubmitQuad(tc.Position, tc.Size, tc.Orientation.z, sc.Color, sc.Texture, sc.TillingFactor, static_cast<uint32_t>(entity));
        }
        Renderer2D::DrawQuads();

        auto groupCircles = m_Registry.group<const CircleComponent>(entt::get<const TransformComponent>);
        for (auto entity : groupCircles) {
            const auto& [tc, cc] = groupCircles.get<TransformComponent, CircleComponent>(entity);
            Renderer2D::SubmitCircle(tc.Position, tc.Size.x, cc.Color, cc.Thickness, cc.Fade, static_cast<uint32_t>(entity));
        }
        Renderer2D::DrawCircles();
    }
}
void Scene::OnViewportResize(uint32_t width, uint32_t height) {
    m_ViewportSize = { width, height };

    for (auto [entity, nsc] : m_Registry.view<NativeScriptComponent>().each()) {
        if (nsc.Instance == nullptr) {
            nsc.Instance = nsc.InstantiateScript();
            nsc.Instance->m_Entity = Entity(entity, this);
            nsc.Instance->OnCreate();
        }
        nsc.Instance->OnViewportResize(width, height);
    }
    for (auto [entity, cc] : m_Registry.view<CameraComponent>().each()) {
        if (cc.Camera)
            cc.Camera->SetViewportSize(width, height);
    }
}

template<>
SB_CORE void Scene::OnComponentAdd<IDComponent>([[maybe_unused]] Entity entity, [[maybe_unused]] IDComponent& component) {}
template<>
SB_CORE void Scene::OnComponentAdd<TagComponent>([[maybe_unused]] Entity entity, [[maybe_unused]] TagComponent& component) {}
template<>
SB_CORE void Scene::OnComponentAdd<TransformComponent>([[maybe_unused]] Entity entity, [[maybe_unused]] TransformComponent& component) {}
template<>
SB_CORE void Scene::OnComponentAdd<CircleComponent>([[maybe_unused]] Entity entity, [[maybe_unused]] CircleComponent& component) {}
template<>
SB_CORE void Scene::OnComponentAdd<SpriteComponent>([[maybe_unused]] Entity entity, [[maybe_unused]] SpriteComponent& component) {}
template<>
SB_CORE void Scene::OnComponentAdd<CameraComponent>([[maybe_unused]] Entity entity, [[maybe_unused]] CameraComponent& component) {
    if (component.Camera)
        component.Camera->SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
}
template<>
SB_CORE void Scene::OnComponentAdd<NativeScriptComponent>([[maybe_unused]] Entity entity, [[maybe_unused]] NativeScriptComponent& component) {}

}
