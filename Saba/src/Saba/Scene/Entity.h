#pragma once

#include "Saba/Scene/Scene.h"

namespace Saba {

	class Entity
	{
		friend class ScriptableEntity;
	public:
		Entity() = default;
		Entity(entt::entity id, Scene* scene);
		Entity(const Entity&) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			SB_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			auto& component = m_Scene->m_Registry.emplace<T>(m_ID, std::forward<Args>(args)...);
			m_Scene->OnAddComponent<T>(*this, component);
			return component;
		}
		template<typename T>
		T& GetComponent()
		{
			return m_Scene->m_Registry.get<T>(m_ID);
		}
		template<typename T>
		void RemoveComponent()
		{
			m_Scene->m_Registry.remove<T>(m_ID);
		}
		template<typename T>
		bool HasComponent() const
		{
			return m_Scene->m_Registry.has<T>(m_ID);
		}

		operator bool() const { return m_ID != entt::null; }
		operator uint32_t() const { return (uint32_t)m_ID; }
		operator entt::entity() const { return m_ID; }

		bool operator==(const Entity& other) const { return m_ID == other.m_ID && m_Scene == other.m_Scene; }
		bool operator!=(const Entity& other) const { return !(*this == other); }
	private:
		entt::entity m_ID = entt::null;
		Scene* m_Scene = nullptr;
	};
}
