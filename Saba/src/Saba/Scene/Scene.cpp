#include "pch.h"
#include "Saba/Scene/Scene.h"
#include "Saba/Scene/Entity.h"
#include "Saba/Scene/Components.h"

#include "Saba/Renderer/Renderer2D.h"

namespace Saba {

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>() = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::OnUpdate(Timestep ts, Ref<Shader> shader)
	{
		Camera* renderCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{		
			auto group = m_Registry.group<CameraComponent>(entt::get<TransformComponent>);
			for (auto entity : group)
			{
				auto& [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);
				if (camera.Primary)
				{
					renderCamera = &camera.Camera;
					cameraTransform = &group.get<TransformComponent>(entity).Transform;
				}
			}
		}

		if (renderCamera)
		{
			Renderer2D::BeginScene(shader, *renderCamera, *cameraTransform);

			auto group = m_Registry.group<TransformComponent, SpriteComponent>();
			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
				if (sprite.UseTransform)
					Renderer2D::DrawQuad(transform, sprite.Color, sprite.Texture, sprite.TillingFactor);
				else
					Renderer2D::DrawQuad(sprite.Pos, sprite.Size, sprite.Color, sprite.Texture, sprite.TillingFactor);
			}
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			if (auto& camera = view.get<CameraComponent>(entity); !camera.FixedAspectRatio)
				camera.Camera.SetViewportSize(width, height);
		}
	}

}
