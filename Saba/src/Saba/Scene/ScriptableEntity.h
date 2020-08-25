#pragma once

#include "Saba/Scene/Entity.h"
#include "Saba/Events/Event.h"
#include "Saba/Timestep.h"

namespace Saba {

	class ScriptableEntity
	{
		friend class Scene;
	public:
		virtual ~ScriptableEntity() = default;
	protected:
		template<typename T>
		T& GetComponent() { return m_Entity.GetComponent<T>(); }
		template<typename T>
		bool HasComponent() { return m_Entity.HasComponent<T>(); }
		template<typename T, typename... Args>
		T& AddComponent(Args&& ...args) { return m_Entity.AddComponent<T>(std::forward<Args>(args)...); }
		template<typename T>
		void RemoveComponent() { m_Entity.RemoveComponent<T>(); }
		Scene* GetScene() { return m_Entity.m_Scene; }

		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnEvent(Event& event) {}
		virtual void OnUpdate(Timestep ts) {}
	private:
		Entity m_Entity;
	};
}
