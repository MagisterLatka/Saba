#pragma once

#include "Saba/Core/UUID.h"
#include "Saba/Renderer/Texture.h"
#include "Saba/Renderer/Camera.h"
#include "Saba/Renderer/Mesh.h"

namespace Saba {

struct IDComponent {
    UUID ID;

    SB_CORE IDComponent() noexcept = default;
    SB_CORE IDComponent(UUID id) noexcept : ID(id) {}
    SB_CORE IDComponent(const IDComponent&) noexcept = default;
    SB_CORE IDComponent(IDComponent&&) noexcept = default;

    SB_CORE IDComponent& operator=(const IDComponent&) noexcept = default;
    SB_CORE IDComponent& operator=(IDComponent&&) noexcept = default;
};

struct TagComponent {
    std::string Tag;

    SB_CORE TagComponent() noexcept = default;
    SB_CORE TagComponent(const TagComponent&) noexcept = default;
    SB_CORE TagComponent(TagComponent&&) noexcept = default;
    SB_CORE TagComponent(std::string tag) noexcept : Tag(std::move(tag)) {}

    SB_CORE TagComponent& operator=(std::string tag) noexcept { Tag = std::move(tag); return *this; }
    SB_CORE TagComponent& operator=(const TagComponent& other) noexcept = default;
    SB_CORE TagComponent& operator=(TagComponent&& other) noexcept = default;

    SB_CORE operator std::string&() noexcept { return Tag; }
    SB_CORE operator const std::string&() const noexcept { return Tag; }
};

struct TransformComponent {
    glm::mat4 Transform = glm::mat4(1.0f);
    glm::vec3 Position = glm::vec3(0.0f), Orientation = glm::vec3(0.0f), Size = glm::vec3(1.0f);

    SB_CORE TransformComponent() noexcept = default;
    SB_CORE TransformComponent(const TransformComponent&) noexcept = default;
    SB_CORE TransformComponent(TransformComponent&&) noexcept = default;
    SB_CORE TransformComponent(glm::mat4 transform) noexcept : Transform(transform) {
        glm::quat orientation;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(Transform, Size, orientation, Position, skew, perspective);
        Orientation = glm::eulerAngles(orientation);
    }
    SB_CORE TransformComponent(glm::vec3 position, glm::vec3 orientation = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f)) noexcept
        : Position(position), Orientation(orientation), Size(size) {}

    SB_CORE TransformComponent& operator=(const TransformComponent&) noexcept = default;
    SB_CORE TransformComponent& operator=(TransformComponent&&) noexcept = default;
    SB_CORE TransformComponent& operator=(glm::mat4 transform) noexcept {
        Transform = transform;
        glm::quat orientation;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(Transform, Size, orientation, Position, skew, perspective);
        Orientation = glm::eulerAngles(orientation);
        return *this;
    }

    SB_CORE operator glm::mat4&() noexcept { return Transform; }
    SB_CORE operator const glm::mat4&() const noexcept { return Transform; }
};

struct CircleComponent {
    glm::vec4 Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    float Thickness = 1.0f, Fade = 0.005f;

    SB_CORE CircleComponent() noexcept = default;
    SB_CORE CircleComponent(const CircleComponent&) noexcept = default;
    SB_CORE CircleComponent(CircleComponent&&) noexcept = default;
    SB_CORE CircleComponent(glm::vec4 color, float thickness = 1.0f, float fade = 0.005f) noexcept
        : Color(color), Thickness(thickness), Fade(fade) {}

    SB_CORE CircleComponent& operator=(const CircleComponent&) noexcept = default;
    SB_CORE CircleComponent& operator=(CircleComponent&&) noexcept = default;
};

struct SpriteComponent {
    glm::vec4 Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    Ref<Texture2D> Texture;
    float TillingFactor = 1.0f;

    SB_CORE SpriteComponent() noexcept = default;
    SB_CORE SpriteComponent(const SpriteComponent&) noexcept = default;
    SB_CORE SpriteComponent(SpriteComponent&&) noexcept = default;
    SB_CORE SpriteComponent(glm::vec4 color, Ref<Texture2D> texture = {}, float tillingFactor = 1.0f) noexcept
        : Color(color), Texture(std::move(texture)), TillingFactor(tillingFactor) {}

    SB_CORE SpriteComponent& operator=(const SpriteComponent&) noexcept = default;
    SB_CORE SpriteComponent& operator=(SpriteComponent&&) noexcept = default;
};

struct CameraComponent {
    Ref<Saba::Camera> Camera;

    SB_CORE CameraComponent() noexcept = default;
    SB_CORE CameraComponent(const CameraComponent&) noexcept = default;
    SB_CORE CameraComponent(CameraComponent&&) noexcept = default;
    SB_CORE CameraComponent(Ref<Saba::Camera> camera) noexcept : Camera(std::move(camera)) {}

    SB_CORE CameraComponent& operator=(const CameraComponent&) noexcept = default;
    SB_CORE CameraComponent& operator=(CameraComponent&&) noexcept = default;
};

struct MeshComponent {
    Ref<Saba::Mesh> Mesh;

    SB_CORE MeshComponent() noexcept = default;
    SB_CORE MeshComponent(const MeshComponent&) noexcept = default;
    SB_CORE MeshComponent(MeshComponent&&) noexcept = default;
    SB_CORE MeshComponent(Ref<Saba::Mesh> mesh) noexcept : Mesh(std::move(mesh)) {}

    SB_CORE MeshComponent& operator=(const MeshComponent&) noexcept = default;
    SB_CORE MeshComponent& operator=(MeshComponent&&) noexcept = default;
};

class ScriptableEntity;
struct NativeScriptComponent {
    ScriptableEntity* Instance = nullptr;

    ScriptableEntity* (*InstantiateScript)();
    void (*DestroyScript)(NativeScriptComponent*);

    template<typename T>
    requires(std::is_base_of_v<ScriptableEntity, T>)
    void Bind() {
        InstantiateScript = []() { return reinterpret_cast<ScriptableEntity*>(new T()); };
        DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
    }
    template<typename T>
    requires(std::is_base_of_v<ScriptableEntity, T>)
    T* GetInstance() {
        return dynamic_cast<T*>(Instance);
    }
    template<typename T>
    requires(std::is_base_of_v<ScriptableEntity, T>)
    const T* GetInstance() const {
        return dynamic_cast<T*>(Instance);
    }
};

template<typename T>
concept Component = std::is_same_v<T, IDComponent> || std::is_same_v<T, TagComponent> || std::is_same_v<T, TransformComponent>
    || std::is_same_v<T, CircleComponent> || std::is_same_v<T, SpriteComponent> || std::is_same_v<T, MeshComponent>
    || std::is_same_v<T, CameraComponent> || std::is_same_v<T, NativeScriptComponent>;

}
