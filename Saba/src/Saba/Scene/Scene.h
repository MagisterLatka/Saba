#pragma once

#include "Saba/Timestep.h"
#include "Saba/Renderer/Shader.h"
#include "Saba/Events/Event.h"

#include <entt.hpp>

namespace Saba {

	class Scene
	{
		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	public:
		Scene() = default;
		~Scene() = default;

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		Entity GetPrimary2DCameraEntity();
		Entity GetPrimary3DCameraEntity();

		void OnStart();
		void OnEnd();
		void OnEvent(Event& event);
		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);

		glm::uvec2 GetViewportSize() const { return m_ViewportSize; }
		void SetViewportPos(glm::uvec2 pos) { m_ViewportPos = pos; }
		glm::uvec2 GetViewportPos() const { return m_ViewportPos; }

		void SetViewportFocusState(bool focused) { m_ViewportFocused = focused; }
		bool IsViewportFocused() const { return m_ViewportFocused; }
		void SetViewportHoverState(bool hovered) { m_ViewportHovered = hovered; }
		bool IsViewportHovered() const { return m_ViewportHovered; }
	private:
		template<typename T>
		void OnAddComponent(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		glm::uvec2 m_ViewportSize = { 0, 0 };
		glm::uvec2 m_ViewportPos = { 0, 0 };
		bool m_ViewportFocused = false, m_ViewportHovered = false;
	};
}
