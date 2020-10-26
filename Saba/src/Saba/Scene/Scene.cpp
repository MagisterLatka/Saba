#include "pch.h"
#include "Saba/Scene/Scene.h"
#include "Saba/Scene/Entity.h"
#include "Saba/Scene/Components.h"
#include "Saba/Scene/ScriptableEntity.h"

#include "Saba/Renderer/Renderer2D.h"
#include "Saba/Renderer/Renderer3D.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Saba {

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>() = name.empty() ? "Entity" : name;
		return entity;
	}
	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnStart()
	{
		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& component)
		{
			if (!component.Instance)
			{
				component.Instance = component.CreateScript();
				component.Instance->m_Entity = Entity(entity, this);
				component.Instance->OnCreate();
			}
		});
	}
	void Scene::OnEnd()
	{
		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& component)
		{
			component.Instance->OnDestroy();
			component.DestroyScript(&component);
		});
	}

	void Scene::OnEvent(Event& event)
	{
		m_Registry.view<NativeScriptComponent>().each([&event](auto entity, auto& component)
		{
			component.Instance->OnEvent(event);
		});
	}

	void Scene::OnUpdate(Timestep ts)
	{
		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& component)
		{
			component.Instance->OnUpdate(ts);
		});
		m_Registry.view<TransformComponent>().each([](auto entity, auto& component)
		{
			component.Transform = glm::translate(glm::mat4(1.0f), component.Pos) * glm::scale(glm::toMat4(glm::quat(component.Orientation)), component.Scale);
			component.Orientation.x = glm::clamp(component.Orientation.x, -glm::half_pi<float>(), glm::half_pi<float>());
			component.Orientation.y = glm::mod(component.Orientation.y + glm::pi<float>(), glm::two_pi<float>()) - glm::pi<float>();
			component.Orientation.z = glm::mod(component.Orientation.z + glm::pi<float>(), glm::two_pi<float>()) - glm::pi<float>();
		});

		Camera* renderCamera2D = nullptr;
		Camera* renderCamera3D = nullptr;
		glm::mat4* cameraTransform2D = nullptr;
		glm::mat4* cameraTransform3D = nullptr;
		{		
			auto cameras = m_Registry.group<CameraComponent>(entt::get<TransformComponent>);
			for (auto entity : cameras)
			{
				auto [transform, camera] = cameras.get<TransformComponent, CameraComponent>(entity);
				if (camera.Primary)
				{
					if (camera.Camera.GetType() == SceneCamera::Type::Orthographic)
					{
						renderCamera2D = &camera.Camera;
						cameraTransform2D = &cameras.get<TransformComponent>(entity).Transform;
					}
					else
					{
						renderCamera3D = &camera.Camera;
						cameraTransform3D = &cameras.get<TransformComponent>(entity).Transform;
					}
				}
			}
		}

		if (renderCamera3D)
		{
			Renderer3D::BeginScene(*renderCamera3D, *cameraTransform3D);
			auto models = m_Registry.group<ModelComponent>(entt::get<TransformComponent>);
			for (auto entity : models)
			{
				auto [model, transform] = models.get<ModelComponent, TransformComponent>(entity);
				for (auto& mesh : *model.Model)
					Renderer3D::DrawMesh(mesh, transform);
			}
		}
		if (renderCamera2D)
		{
			Renderer2D::BeginScene(*renderCamera2D, *cameraTransform2D);
			auto sprites = m_Registry.group<SpriteComponent>(entt::get<TransformComponent>);
			for (auto entity : sprites)
			{
				auto [transform, sprite] = sprites.get<TransformComponent, SpriteComponent>(entity);
				Renderer2D::DrawRotatedQuad(transform.Pos, { transform.Scale.x, transform.Scale.y }, transform.Orientation.z, sprite.Color, sprite.Texture, sprite.TillingFactor);
			}
			Renderer2D::Flush();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportSize = { (float)width, (float)height };

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			if (auto& camera = view.get<CameraComponent>(entity); !camera.FixedAspectRatio)
				camera.Camera.SetViewportSize(width, height);
		}
	}

	template<typename T>
	void Scene::OnAddComponent(Entity entity, T& component)
	{
		static_assert(false);
	}
	template<>
	void Scene::OnAddComponent<TransformComponent>(Entity entity, TransformComponent& component)
	{

	}
	template<>
	void Scene::OnAddComponent<TagComponent>(Entity entity, TagComponent& component)
	{

	}
	template<>
	void Scene::OnAddComponent<SpriteComponent>(Entity entity, SpriteComponent& component)
	{

	}
	template<>
	void Scene::OnAddComponent<ModelComponent>(Entity entity, ModelComponent& component)
	{

	}
	template<>
	void Scene::OnAddComponent<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
	}
	template<>
	void Scene::OnAddComponent<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{

	}
}
