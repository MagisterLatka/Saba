#pragma once

#include "Saba/Timestep.h"

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

		void OnUpdate(Timestep ts);
	private:
		entt::registry m_Registry;
	};
}
