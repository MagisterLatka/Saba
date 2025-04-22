#include <pch.h>
#include "Scene.h"

#include "Saba/Scene/ScriptableEntity.h"
#include "Saba/Renderer/Renderer2D.h"
#include "Saba/Renderer/Renderer3D.h"
#include "Saba/Renderer/Renderer.h"

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

template<Component T>
static void CopyComponent(Entity src, Entity dst) {
    if (src.HasComponent<T>())
        dst.AddOrReplaceComponent<T>(src.GetComponent<T>());
}
void Scene::DuplicateEntity(Entity entity) {
    Entity e = CreateEntity(entity.GetComponent<TagComponent>().Tag);
    CopyComponent<TransformComponent>(entity, e);
    CopyComponent<SpriteComponent>(entity, e);
    CopyComponent<CircleComponent>(entity, e);
    CopyComponent<ModelComponent>(entity, e);
    CopyComponent<LightComponent>(entity, e);
    CopyComponent<CameraComponent>(entity, e);
    CopyComponent<NativeScriptComponent>(entity, e);
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

void Scene::OnUpdateEditor([[maybe_unused]] Timestep ts, const EditorCamera& camera) {
    for (auto [entity, tc] : m_Registry.view<TransformComponent>(entt::exclude<SpriteComponent, CircleComponent, LightComponent>).each()) {
        tc.Orientation = glm::mod(tc.Orientation, glm::two_pi<float>());
        const auto rotation = glm::quat(tc.Orientation);
        tc.Transform = glm::translate(glm::mat4(1.0f), tc.Position) * glm::scale(glm::toMat4(rotation), tc.Size);
    }

    Renderer2D::SetViewProjectionMatrix(camera.GetProjectionViewMatrix());
    auto groupQuads = m_Registry.group<const SpriteComponent>(entt::get<const TransformComponent>);
    for (auto entity : groupQuads) {
        const auto& [tc, sc] = groupQuads.get<const TransformComponent, const SpriteComponent>(entity);
        Renderer2D::SubmitQuad(tc.Position, tc.Size, tc.Orientation.z, sc.Color, sc.Texture, sc.TillingFactor, static_cast<uint32_t>(entity));
    }
    Renderer2D::DrawQuads();

    auto groupCircles = m_Registry.group<const CircleComponent>(entt::get<const TransformComponent>);
    for (auto entity : groupCircles) {
        const auto& [tc, cc] = groupCircles.get<const TransformComponent, const CircleComponent>(entity);
        Renderer2D::SubmitCircle(tc.Position, tc.Size.x, cc.Color, cc.Thickness, cc.Fade, static_cast<uint32_t>(entity));
    }
    Renderer2D::DrawCircles();

    Renderer3D::SetShader(Renderer::GetShaderLibrary().Get("meshPBRShader"));
    Renderer3D::SetViewProjectionMatrix(camera.GetProjectionViewMatrix());
    Renderer3D::SetCameraPosition(camera.GetPosition());
    Renderer3D::ResetLights();
    auto viewLights = m_Registry.view<const LightComponent>();
    for (auto entity : viewLights) {
        const auto& lc = viewLights.get<const LightComponent>(entity);
        Renderer3D::SubmitLight(lc.LightPos, glm::vec3(lc.LightColor) * lc.LightColor.a, lc.Radius);
    }
    auto groupMesh = m_Registry.group<const ModelComponent>(entt::get<const TransformComponent>);
    for (auto entity : groupMesh) {
        const auto& [tc, mc] = groupMesh.get<const TransformComponent, const ModelComponent>(entity);
        for (const auto& mesh : mc.Model->GetMeshes()) {
            Renderer3D::SubmitMeshInstance(mesh, tc.Transform, mc.Model->GetMaterial(), static_cast<uint32_t>(entity));
        }
    }
    Renderer3D::Draw();
    Renderer::Render();

    Renderer3D::SetShader({});
    auto cubeMeshID = Renderer::GetMeshLibrary().Get("Cube")->GetID();
    for (auto entity : viewLights) {
        const auto& lc = viewLights.get<const LightComponent>(entity);
        Renderer3D::SubmitMeshInstance(cubeMeshID, glm::scale(glm::translate(lc.LightPos), glm::vec3(0.1f)), {}, static_cast<uint32_t>(entity));
    }
    Renderer3D::DrawMesh(cubeMeshID);
}
void Scene::OnUpdateRuntime(Timestep ts) {
    for (auto [entity, nsc] : m_Registry.view<NativeScriptComponent>().each()) {
        if (nsc.Instance == nullptr) {
            nsc.Instance = nsc.InstantiateScript();
            nsc.Instance->m_Entity = Entity(entity, this);
            nsc.Instance->OnCreate();
        }
        nsc.Instance->OnUpdate(ts);
    }
    for (auto [entity, tc] : m_Registry.view<TransformComponent>(entt::exclude<SpriteComponent, CircleComponent, LightComponent>).each()) {
        tc.Orientation = glm::mod(tc.Orientation, glm::two_pi<float>());
        const auto rotation = glm::quat(tc.Orientation);
        tc.Transform = glm::translate(glm::mat4(1.0f), tc.Position) * glm::scale(glm::toMat4(rotation), tc.Size);
    }

    if (m_Camera != entt::null) {
        auto camera = m_Registry.get<CameraComponent>(m_Camera).Camera;
        const auto& transform = m_Registry.get<TransformComponent>(m_Camera).Transform;
        glm::mat4 viewProjMat = camera->GetProjectionMatrix() * glm::inverse(transform);
        Renderer2D::SetViewProjectionMatrix(viewProjMat);

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

        Renderer3D::SetShader(Renderer::GetShaderLibrary().Get("meshPBRShader"));
        Renderer3D::SetViewProjectionMatrix(viewProjMat);
        Renderer3D::SetCameraPosition(m_Registry.get<TransformComponent>(m_Camera).Position);
        Renderer3D::ResetLights();
        auto viewLights = m_Registry.view<const LightComponent>();
        for (auto entity : viewLights) {
            const auto& lc = viewLights.get<const LightComponent>(entity);
            Renderer3D::SubmitLight(lc.LightPos, glm::vec3(lc.LightColor) * lc.LightColor.a, lc.Radius);
        }
        auto groupMesh = m_Registry.group<const ModelComponent>(entt::get<const TransformComponent>);
        for (auto entity : groupMesh) {
            const auto& [tc, mc] = groupMesh.get<const TransformComponent, const ModelComponent>(entity);
            for (const auto& mesh : mc.Model->GetMeshes()) {
                Renderer3D::SubmitMeshInstance(mesh, tc.Transform, mc.Model->GetMaterial(), static_cast<uint32_t>(entity));
            }
        }
        Renderer3D::Draw();
        Renderer::Render();

        Renderer3D::SetShader({});
        auto cubeMeshID = Renderer::GetMeshLibrary().Get("Cube")->GetID();
        for (auto entity : viewLights) {
            const auto& lc = viewLights.get<const LightComponent>(entity);
            Renderer3D::SubmitMeshInstance(cubeMeshID, glm::scale(glm::translate(lc.LightPos), glm::vec3(0.1f)), {}, static_cast<uint32_t>(entity));
        }
        Renderer3D::DrawMesh(cubeMeshID);
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

template<Component T>
static void CopyComponent(entt::registry& src, entt::registry& dst, const std::unordered_map<UUID, entt::entity>& entities) {
    for (auto e : src.view<T>()) {
        UUID uuid = src.get<IDComponent>(e).ID;
        SB_CORE_ASSERT(entities.contains(uuid));
        entt::entity id = entities.at(uuid);
        T& component = src.get<T>(e);
        dst.emplace_or_replace<T>(id, component);
    }
}
Ref<Scene> Scene::Copy(Ref<Scene> scene) {
    Ref<Scene> newScene = Ref<Scene>::Create(scene->m_Name);
    newScene->m_ViewportSize = scene->m_ViewportSize;

    std::unordered_map<UUID, entt::entity> entities;
    for (auto e : scene->m_Registry.view<IDComponent>() | std::views::reverse) {
        UUID uuid = scene->m_Registry.get<IDComponent>(e).ID;
        Entity entity = newScene->CreateEntityWithID(uuid, scene->m_Registry.get<TagComponent>(e).Tag);
        entities[uuid] = entity;
    }

    CopyComponent<TransformComponent>(scene->m_Registry, newScene->m_Registry, entities);
    CopyComponent<SpriteComponent>(scene->m_Registry, newScene->m_Registry, entities);
    CopyComponent<CircleComponent>(scene->m_Registry, newScene->m_Registry, entities);
    CopyComponent<CameraComponent>(scene->m_Registry, newScene->m_Registry, entities);
    CopyComponent<ModelComponent>(scene->m_Registry, newScene->m_Registry, entities);
    CopyComponent<LightComponent>(scene->m_Registry, newScene->m_Registry, entities);
    CopyComponent<NativeScriptComponent>(scene->m_Registry, newScene->m_Registry, entities);

    if (scene->m_Camera != entt::null)
        newScene->m_Camera = entities.at(scene->m_Registry.get<IDComponent>(scene->m_Camera).ID);

    return newScene;
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
SB_CORE void Scene::OnComponentAdd<ModelComponent>([[maybe_unused]] Entity entity, [[maybe_unused]] ModelComponent& component) {}
template<>
SB_CORE void Scene::OnComponentAdd<LightComponent>([[maybe_unused]] Entity entity, [[maybe_unused]] LightComponent& component) {}
template<>
SB_CORE void Scene::OnComponentAdd<SpriteComponent>([[maybe_unused]] Entity entity, [[maybe_unused]] SpriteComponent& component) {}
template<>
SB_CORE void Scene::OnComponentAdd<CameraComponent>([[maybe_unused]] Entity entity, [[maybe_unused]] CameraComponent& component) {
    if (component.Camera)
        component.Camera->SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
}
template<>
SB_CORE void Scene::OnComponentAdd<NativeScriptComponent>([[maybe_unused]] Entity entity, [[maybe_unused]] NativeScriptComponent& component) {}

} //namespace Saba
