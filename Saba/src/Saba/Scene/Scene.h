#pragma once

#include "Saba/Timestep.h"
#include "Saba/Renderer/Shader.h"
#include "Saba/Events/Event.h"

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

		void OnEvent(Event& event);
		void OnUpdate(Timestep ts, Ref<Shader> shader);
		void OnViewportResize(uint32_t width, uint32_t height);
		glm::uvec2 GetViewportSize() const { return m_ViewportSize; }
		void SetViewportPos(glm::uvec2 pos) { m_ViewportPos = pos; }
		glm::uvec2 GetViewportPos() const { return m_ViewportPos; }
	private:
		entt::registry m_Registry;
		glm::uvec2 m_ViewportSize;
		glm::uvec2 m_ViewportPos;
	};
}
