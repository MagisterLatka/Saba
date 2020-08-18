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

	void Scene::OnUpdate(Timestep ts)
	{
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
