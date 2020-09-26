#include "pch.h"
#include "Saba/Scene/Scene.h"
#include "Saba/Scene/Entity.h"
#include "Saba/Scene/Components.h"
#include "Saba/Scene/ScriptableEntity.h"

#include "Saba/Renderer/Renderer2D.h"

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
			component.Transform = glm::translate(glm::mat4(1.0f), component.Pos) * glm::scale(glm::toMat4(glm::quat(component.EulerAngles)), component.Scale);
			component.EulerAngles.x = glm::clamp(component.EulerAngles.x, -glm::half_pi<float>(), glm::half_pi<float>());
			component.EulerAngles.y = glm::mod(component.EulerAngles.y + glm::pi<float>(), glm::two_pi<float>()) - glm::pi<float>();
			component.EulerAngles.z = glm::mod(component.EulerAngles.z + glm::pi<float>(), glm::two_pi<float>()) - glm::pi<float>();
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

		if (renderCamera2D)
		{
			Renderer2D::BeginScene(*renderCamera2D, *cameraTransform2D);
			auto sprites = m_Registry.group<SpriteComponent>(entt::get<TransformComponent>);
			for (auto entity : sprites)
			{
				auto [transform, sprite] = sprites.get<TransformComponent, SpriteComponent>(entity);
				if (sprite.UseTransform)
					Renderer2D::DrawQuad(transform, sprite.Color, sprite.Texture, sprite.TillingFactor);
				else
					Renderer2D::DrawQuad(sprite.Pos, sprite.Size, sprite.Color, sprite.Texture, sprite.TillingFactor);
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

}
