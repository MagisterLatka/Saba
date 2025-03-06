#pragma once

#include "Saba/Renderer/Texture.h"

namespace Saba {

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

template<typename T>
concept Component = std::is_same_v<T, TagComponent> || std::is_same_v<T, TransformComponent> || std::is_same_v<T, SpriteComponent>;

}
