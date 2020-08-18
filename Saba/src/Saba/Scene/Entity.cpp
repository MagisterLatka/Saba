#include "pch.h"
#include "Saba/Scene/Entity.h"

namespace Saba {

	Entity::Entity(entt::entity id, Scene* scene)
		: m_ID(id), m_Scene(scene) {}
}
