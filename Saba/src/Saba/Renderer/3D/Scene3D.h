#pragma once

#include "Scene3DObject.h"

namespace Saba {

	class Scene3D
	{
	public:
		Scene3D();
		Scene3D(const Scene3D&) = delete;
		Scene3D operator=(const Scene3D&) = delete;
		~Scene3D();

		void Add(Scene3DObject* object);
		inline Scene3DObject* Get(uint32_t id) const { return m_Objects[id]; }

		void Draw(uint32_t id);
		void DrawAll();
	private:
		std::vector<Scene3DObject*> m_Objects;
	};
}
