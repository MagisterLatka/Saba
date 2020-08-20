#pragma once

#include "Saba/Timestep.h"
#include "Saba/Renderer/Shader.h"

#include <entt.hpp>

namespace Saba {

	class Entity;
	class Scene
	{
		friend class Entity;
	public:
		Scene() = default;
		~Scene() = default;

		Entity CreateEntity(const std::string& name = std::string());

		void OnUpdate(Timestep ts, Ref<Shader> shader);
		void OnViewportResize(uint32_t width, uint32_t height);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	};
}
