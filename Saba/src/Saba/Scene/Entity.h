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
			return m_Scene->m_Registry.emplace<T>(m_ID, std::forward<Args>(args)...);
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
	private:
		entt::entity m_ID = entt::null;
		Scene* m_Scene = nullptr;
	};
}
