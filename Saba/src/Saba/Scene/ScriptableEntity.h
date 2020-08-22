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

		template<typename T>
		T& GetComponent() { return m_Entity.GetComponent<T>(); }
	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnEvent(Event& event) {}
		virtual void OnUpdate(Timestep ts) {}
	private:
		Entity m_Entity;
	};
}
