#pragma once

#include "Saba/Renderer/Texture.h"
#include "Saba/Scene/SceneCamera.h"
#include "Saba/Scene/ScriptableEntity.h"

namespace Saba {

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}

		operator std::string() { return Tag; }
	};

	struct TransformComponent
	{
		glm::mat4 Transform = glm::mat4(1.0f);
		glm::vec3 Pos = glm::vec3(0.0f);
		glm::vec3 Scale = glm::vec3(1.0f);
		glm::vec3 EulerAngles = glm::vec3(0.0f, 0.0f, 0.0f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) {}

		operator glm::mat4() { return Transform; }
		operator const glm::mat4() const { return Transform; }
	};

	struct SpriteComponent
	{
		bool UseTransform = true;
		glm::vec3 Pos = { 0.0f, 0.0f, 0.0f };
		glm::vec2 Size = { 0.0f, 0.0f };
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture = {};
		float TillingFactor = 1.0f;

		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent(glm::vec2 pos, glm::vec2 size, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }, Ref<Texture2D> texture = {}, float tillingFactor = 1.0f)
			: UseTransform(false), Pos(pos, 0.0f), Size(size), Color(color), Texture(texture), TillingFactor(tillingFactor) {}
		SpriteComponent(glm::vec3 pos, glm::vec2 size, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }, Ref<Texture2D> texture = {}, float tillingFactor = 1.0f)
			: UseTransform(false), Pos(pos), Size(size), Color(color), Texture(texture), TillingFactor(tillingFactor) {}
		SpriteComponent(glm::vec4 color, Ref<Texture2D> texture = {}, float tillingFactor = 1.0f)
			: UseTransform(true), Color(color), Texture(texture), TillingFactor(tillingFactor) {}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = delete;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(SceneCamera camera)
			: Camera(camera) {}
		CameraComponent(SceneCamera::Type type)
			: Camera(type) {}
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity*(*CreateScript)() = nullptr;
		void(*DestroyScript)(NativeScriptComponent*) = nullptr;

		template<typename T>
		void Bind()
		{
			CreateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* component) { delete component->Instance; component->Instance = nullptr; };
		}
		template<typename T>
		T* GetInstance()
		{
			return dynamic_cast<T*>(Instance);
		}
	};
}
