#pragma once

#include "Saba/Core/UUID.h"
#include "Saba/Renderer/Texture.h"
#include "Saba/Renderer/Camera.h"
#include "Saba/Renderer/Model.h"

namespace Saba {

struct IDComponent {
    UUID ID;

    IDComponent() noexcept = default;
    IDComponent(UUID id) noexcept : ID(id) {}
    IDComponent(const IDComponent&) noexcept = default;
    IDComponent(IDComponent&&) noexcept = default;

    IDComponent& operator=(const IDComponent&) noexcept = default;
    IDComponent& operator=(IDComponent&&) noexcept = default;
};

struct TagComponent {
    std::string Tag;

    TagComponent() noexcept = default;
    TagComponent(const TagComponent&) noexcept = default;
    TagComponent(TagComponent&&) noexcept = default;
    TagComponent(std::string tag) noexcept : Tag(std::move(tag)) {}

    TagComponent& operator=(std::string tag) noexcept { Tag = std::move(tag); return *this; }
    TagComponent& operator=(const TagComponent& other) noexcept = default;
    TagComponent& operator=(TagComponent&& other) noexcept = default;

    operator std::string&() noexcept { return Tag; }
    operator const std::string&() const noexcept { return Tag; }
};

struct TransformComponent {
    glm::mat4 Transform = glm::mat4(1.0f);
    glm::vec3 Position = glm::vec3(0.0f), Orientation = glm::vec3(0.0f), Size = glm::vec3(1.0f);

    TransformComponent() noexcept = default;
    TransformComponent(const TransformComponent&) noexcept = default;
    TransformComponent(TransformComponent&&) noexcept = default;
    TransformComponent(glm::mat4 transform) noexcept : Transform(transform) {
        glm::quat orientation;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(Transform, Size, orientation, Position, skew, perspective);
        Orientation = glm::eulerAngles(orientation);
    }
    TransformComponent(glm::vec3 position, glm::vec3 orientation = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f)) noexcept
        : Position(position), Orientation(orientation), Size(size) {}

    TransformComponent& operator=(const TransformComponent&) noexcept = default;
    TransformComponent& operator=(TransformComponent&&) noexcept = default;
    TransformComponent& operator=(glm::mat4 transform) noexcept {
        Transform = transform;
        glm::quat orientation;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(Transform, Size, orientation, Position, skew, perspective);
        Orientation = glm::eulerAngles(orientation);
        return *this;
    }

    operator glm::mat4&() noexcept { return Transform; }
    operator const glm::mat4&() const noexcept { return Transform; }
};

struct CircleComponent {
    glm::vec4 Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    float Thickness = 1.0f, Fade = 0.005f;

    CircleComponent() noexcept = default;
    CircleComponent(const CircleComponent&) noexcept = default;
    CircleComponent(CircleComponent&&) noexcept = default;
    CircleComponent(glm::vec4 color, float thickness = 1.0f, float fade = 0.005f) noexcept
        : Color(color), Thickness(thickness), Fade(fade) {}

    CircleComponent& operator=(const CircleComponent&) noexcept = default;
    CircleComponent& operator=(CircleComponent&&) noexcept = default;
};

struct SpriteComponent {
    glm::vec4 Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    Ref<Texture2D> Texture;
    float TillingFactor = 1.0f;

    SpriteComponent() noexcept = default;
    SpriteComponent(const SpriteComponent&) noexcept = default;
    SpriteComponent(SpriteComponent&&) noexcept = default;
    SpriteComponent(glm::vec4 color, Ref<Texture2D> texture = {}, float tillingFactor = 1.0f) noexcept
        : Color(color), Texture(std::move(texture)), TillingFactor(tillingFactor) {}

    SpriteComponent& operator=(const SpriteComponent&) noexcept = default;
    SpriteComponent& operator=(SpriteComponent&&) noexcept = default;
};

struct CameraComponent {
    Ref<Saba::Camera> Camera;

    CameraComponent() noexcept = default;
    CameraComponent(const CameraComponent&) noexcept = default;
    CameraComponent(CameraComponent&&) noexcept = default;
    CameraComponent(Ref<Saba::Camera> camera) noexcept : Camera(std::move(camera)) {}

    CameraComponent& operator=(const CameraComponent&) noexcept = default;
    CameraComponent& operator=(CameraComponent&&) noexcept = default;
};

struct ModelComponent {
    Ref<Saba::Model> Model;

    ModelComponent() noexcept = default;
    ModelComponent(const ModelComponent&) noexcept = default;
    ModelComponent(ModelComponent&&) noexcept = default;
    ModelComponent(Ref<Saba::Model> model) noexcept
        : Model(std::move(model)) {}
    ModelComponent(const std::filesystem::path& filepath, Ref<Material> material = {}) noexcept
        : Model(Ref<Saba::Model>::Create(filepath, std::move(material))) {}

    ModelComponent& operator=(const ModelComponent&) noexcept = default;
    ModelComponent& operator=(ModelComponent&&) noexcept = default;
};

struct LightComponent {
    glm::vec3 LightPos = glm::vec3(0.0f);
    glm::vec4 LightColor = glm::vec4(1.0f);
    float Radius = 1.0f;

    LightComponent() noexcept = default;
    LightComponent(const LightComponent&) noexcept = default;
    LightComponent(LightComponent&&) noexcept = default;
    LightComponent(glm::vec3 lightPos, glm::vec4 lightColor, float radius) noexcept
        : LightPos(lightPos), LightColor(lightColor), Radius(radius) {}

    LightComponent& operator=(const LightComponent&) noexcept = default;
    LightComponent& operator=(LightComponent&&) noexcept = default;
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
        DestroyScript = [](NativeScriptComponent* nsc) { delete reinterpret_cast<T*>(nsc->Instance); nsc->Instance = nullptr; };
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
concept Component = IsOneOf<T, IDComponent, TagComponent, TransformComponent, CircleComponent, SpriteComponent,
    ModelComponent, LightComponent, CameraComponent, NativeScriptComponent>;

} //namespace Saba
